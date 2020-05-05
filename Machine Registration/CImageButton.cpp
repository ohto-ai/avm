// CImageButton.cpp: 实现文件
//

#include "pch.h"
#include "CImageButton.h"


using namespace thatboy::mfc;
// CImageButton

IMPLEMENT_DYNAMIC(CImageButton, CBitmapButton)

CImageButton::CImageButton()
	: m_bOver(FALSE)
	, m_bTracking(FALSE)
{
}

CImageButton::~CImageButton()
{
}

BEGIN_MESSAGE_MAP(CImageButton, CBitmapButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CImageButton 消息处理程序

// 从文件加载图片
void CImageButton::LoadImagesFromFile(LPCTSTR pszFileName, bool bSizeToCont /*= true*/, int nImageNum, BYTE cAlphaThreshold /*= 1*/)
{
	CPng wholeImage;
	wholeImage.LoadPngFromFile(pszFileName);	// 加载PNG
	ApplyPngImages(wholeImage, CRect(), bSizeToCont, nImageNum, cAlphaThreshold);
}

// 从资源加载图片
void CImageButton::LoadImagesFromResource(LPCTSTR pszResourceName, bool bSizeToCont /*= true*/, int nImageNum /*= 4*/, BYTE cAlphaThreshold /*= 1*/, LPCTSTR lpImageResType /*= TEXT("PNG")*/)
{
	CPng wholeImage;
	wholeImage.LoadPngFromResource(pszResourceName, true, lpImageResType);
	ApplyPngImages(wholeImage, CRect(), bSizeToCont, nImageNum, cAlphaThreshold);
}

// 从资源加载图片
void CImageButton::LoadImagesFromResource(UINT nReourceID, bool bSizeToCont /*= true*/, int nImageNum /*= 4*/, BYTE cAlphaThreshold /*= 1*/, LPCTSTR lpImageResType /*= TEXT("PNG")*/)
{
	CPng wholeImage;
	wholeImage.LoadPngFromResource(nReourceID, true, lpImageResType);
	ApplyPngImages(wholeImage, CRect(), bSizeToCont, nImageNum, cAlphaThreshold);
}

// 应用图片
void CImageButton::ApplyPngImages(const CPng& wholeImage, CRect usedRect, bool bSizeToCont /*= true*/, int nImageNum, BYTE cAlphaThreshold)
{
	ASSERT(nImageNum >= 1 && nImageNum <= 4);
	if (nImageNum < 1)nImageNum = 1;
	else if (nImageNum > 4)nImageNum = 4;


	CPng resizeImage;
	CRgn rg;
	int bnWidth, bnHeight;
	int srcBnWidth, srcBnHeight;

	// 拆分按钮

	if (usedRect.IsRectEmpty())
		usedRect.SetRect(0, 0, wholeImage.GetWidth(), wholeImage.GetHeight());

	srcBnWidth = usedRect.Width() / nImageNum;
	srcBnHeight = usedRect.Height();

	if (bSizeToCont)
	{
		bnWidth = srcBnWidth;
		bnHeight = srcBnHeight;
		SetWindowPos(nullptr, 0, 0, bnWidth, bnHeight, SWP_NOMOVE | SWP_NOZORDER);

		resizeImage.Create(bnWidth * nImageNum, bnHeight, wholeImage.GetBPP());
		wholeImage.BitBlt(CImageDC(resizeImage), CRect(0, 0, bnWidth * nImageNum, bnHeight), usedRect.TopLeft());
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		bnWidth = rect.Width();
		bnHeight = rect.Height();

		resizeImage.Create(bnWidth * nImageNum, bnHeight, wholeImage.GetBPP());
		wholeImage.StretchBlt(CImageDC(resizeImage), CRect(0, 0, bnWidth * nImageNum, bnHeight), usedRect);
	}

	// 创建目标图像
	m_bnImageU.Create(bnWidth, bnHeight, wholeImage.GetBPP());
	m_bnImageD.Create(bnWidth, bnHeight, wholeImage.GetBPP());
	m_bnImageF.Create(bnWidth, bnHeight, wholeImage.GetBPP());
	m_bnImageX.Create(bnWidth, bnHeight, wholeImage.GetBPP());

	// 复制
	resizeImage.BitBlt(CImageDC(m_bnImageU), 0, 0, bnWidth, bnHeight, bnWidth * 0, 0);
	if (nImageNum >= 2)
		resizeImage.BitBlt(CImageDC(m_bnImageD), 0, 0, bnWidth, bnHeight, bnWidth * 1, 0);
	else
		m_bnImageU.BitBlt(CImageDC(m_bnImageD), 0, 0);
	if (nImageNum >= 3)
		resizeImage.BitBlt(CImageDC(m_bnImageF), 0, 0, bnWidth, bnHeight, bnWidth * 2, 0);
	else
		m_bnImageU.BitBlt(CImageDC(m_bnImageF), 0, 0);
	if (nImageNum >= 4)
		resizeImage.BitBlt(CImageDC(m_bnImageX), 0, 0, bnWidth, bnHeight, bnWidth * 3, 0);
	else
		m_bnImageU.BitBlt(CImageDC(m_bnImageX), 0, 0);

	// 计算可点击区域
	rg.CreateRectRgn(0, 0, 0, 0);
	CRgn rgtmp;
	for (int i = 0; i < bnWidth; i++)
	{
		for (int j = 0; j < bnHeight; j++)
		{
			if (static_cast<LPBYTE>(m_bnImageU.GetPixelAddress(i, j))[3] >= cAlphaThreshold)
			{
				rgtmp.CreateRectRgn(i, j, i + 1, j + 1);
				rg.CombineRgn(&rg, &rgtmp, RGN_OR);
				rgtmp.DeleteObject();
			}
			else if (static_cast<LPBYTE>(m_bnImageD.GetPixelAddress(i, j))[3] >= cAlphaThreshold)
			{
				rgtmp.CreateRectRgn(i, j, i + 1, j + 1);
				rg.CombineRgn(&rg, &rgtmp, RGN_OR);
				rgtmp.DeleteObject();
			}
			else if (static_cast<LPBYTE>(m_bnImageF.GetPixelAddress(i, j))[3] >= cAlphaThreshold)
			{
				rgtmp.CreateRectRgn(i, j, i + 1, j + 1);
				rg.CombineRgn(&rg, &rgtmp, RGN_OR);
				rgtmp.DeleteObject();
			}
		}
	}
	CButton::ModifyStyle(0, BS_OWNERDRAW);
	CButton::SetWindowRgn(rg, TRUE);
	rg.DeleteObject();
}

// 绘制按钮
void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_bOver)
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			m_bnImageD.Draw(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem);
		else
			m_bnImageF.Draw(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem);
	}
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		m_bnImageX.Draw(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem);
	else
		m_bnImageU.Draw(lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem);
}

//
void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = CButton::m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 30;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

//
void CImageButton::OnMouseLeave()
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	CButton::InvalidateRect(NULL);
	CButton::OnMouseLeave();
}

//
void CImageButton::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bOver = TRUE;
	CButton::InvalidateRect(NULL);
	CButton::OnMouseHover(nFlags, point);
}

//
void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags, point);
}

//
BOOL CImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT nImageID /*= NULL*/)
{
	auto ret = CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
	if (nImageID != NULL)
		LoadImagesFromResource(nImageID, false);
	return ret;
}