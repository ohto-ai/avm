// CDragDialog.cpp: 实现文件
//

#include "pch.h"
#include "CDragDialog.h"
#include "afxdialogex.h"
#include "resource.h"

using namespace thatboy::mfc;
// CDragDialog 对话框

IMPLEMENT_DYNAMIC(CDragDialog, CDialogEx)

CDragDialog::CDragDialog(UINT nIDTemplate, CWnd* pParent /*=nullptr*/)
	: CDialogEx(nIDTemplate, pParent)
{
}

CDragDialog::~CDragDialog()
{
}

void CDragDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDragDialog, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SYSCOMMAND()
	ON_COMMAND_RANGE(ID_DRAG_MINIMIZE, ID_DRAG_CLOSE, OnSystemMenuButtonClick)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CTLCOLOR()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_MOVE()
END_MESSAGE_MAP()

// CDragDialog 消息处理程序

// 更新菜单位置
void CDragDialog::UpdateSystemMenuItemPostion(BOOL menuChange /*= FALSE */)
{
	int cx = GetWindowWidth();

	// 系统菜单
	if (nSystemMenuStyle & DS_CLOSE)
		bnSysTemMenuClose.MoveWindow(cx -= MENUBUTTONRANGEWIDTH, MENUBUTTONGAP, MENUBUTTONWIDTH, MENUBUTTONHEIGHT);

	if (nSystemMenuStyle & DS_MAXIMIZE)
	{
		bnSysTemMenuMaximize.MoveWindow(cx -= MENUBUTTONWIDTH, MENUBUTTONGAP, MENUBUTTONWIDTH, MENUBUTTONHEIGHT);
		bnSysTemMenuRestore.MoveWindow(cx, MENUBUTTONGAP, MENUBUTTONWIDTH, MENUBUTTONHEIGHT);
	}

	if (nSystemMenuStyle & DS_MINIMIZE)
		bnSysTemMenuMinimize.MoveWindow(cx -= MENUBUTTONWIDTH, MENUBUTTONGAP, MENUBUTTONWIDTH, MENUBUTTONHEIGHT);

	// 标题
	if (nSystemMenuStyle & DS_CAPTIONTEXT)
		szTitle.MoveWindow(CAPITIONGAP, MENUBUTTONGAP, cx - CAPITIONGAP, CAPITIONHEIGHT);

	if (menuChange)
	{
		bnSysTemMenuClose.EnableWindow(nSystemMenuStyle & DS_CLOSEABLE);
		bnSysTemMenuMaximize.EnableWindow(nSystemMenuStyle & DS_MAXIMIZEABLE);
		bnSysTemMenuRestore.EnableWindow(nSystemMenuStyle & DS_MAXIMIZEABLE);
		bnSysTemMenuMinimize.EnableWindow(nSystemMenuStyle & DS_MINIMIZEABLE);

		szTitle.ShowWindow(nSystemMenuStyle & DS_CAPTIONTEXT ? SW_SHOW : SW_HIDE);

		bnSysTemMenuClose.ShowWindow(nSystemMenuStyle & DS_CLOSE ? SW_SHOW : SW_HIDE);
		bnSysTemMenuMinimize.ShowWindow(nSystemMenuStyle & DS_MINIMIZE ? SW_SHOW : SW_HIDE);
	}
	if (nSystemMenuStyle & DS_MAXIMIZE)
	{
		if (IsZoomed())
		{
			bnSysTemMenuMaximize.ShowWindow(SW_HIDE);
			bnSysTemMenuRestore.ShowWindow(SW_SHOW);
		}
		else
		{
			bnSysTemMenuMaximize.ShowWindow(SW_SHOW);
			bnSysTemMenuRestore.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		bnSysTemMenuMaximize.ShowWindow(SW_HIDE);
		bnSysTemMenuRestore.ShowWindow(SW_HIDE);
	}

	// 重绘，防止崩坏
	bnSysTemMenuClose.Invalidate();
	bnSysTemMenuMaximize.Invalidate();
	bnSysTemMenuRestore.Invalidate();
	bnSysTemMenuMinimize.Invalidate();
}

// 应用菜单图像
void CDragDialog::ApplySystemMenuImages(const CPng& wholeImage, UINT bnImageType, int nImageNum, BYTE cAlphaThreshold)
{
	int bnNum = (bnImageType & DS_CLOSE ? 1 : 0) + (bnImageType & DS_MAXIMIZE ? 2 : 0) + (bnImageType & DS_MINIMIZE ? 1 : 0);

	if (bnNum == 0)
		return;

	int bnWidth, bnHeight;
	bnWidth = wholeImage.GetWidth();
	bnHeight = wholeImage.GetHeight() / bnNum;

	if (bnImageType & DS_MINIMIZE)
		bnSysTemMenuMinimize.ApplyPngImages(wholeImage, CRect(0, 0, bnWidth, bnHeight), false, nImageNum, cAlphaThreshold);
	if (bnImageType & DS_MAXIMIZE)
	{
		bnSysTemMenuMaximize.ApplyPngImages(wholeImage, CRect(0, ((bnImageType | DS_MINIMIZE) ? bnHeight : 0), bnWidth, ((bnImageType | DS_MINIMIZE) ? bnHeight : 0) + bnHeight), false);
		bnSysTemMenuRestore.ApplyPngImages(wholeImage, CRect(0, ((bnImageType | DS_MINIMIZE) ? bnHeight * 2 : bnHeight), bnWidth, ((bnImageType | DS_MINIMIZE) ? bnHeight * 2 : bnHeight) + bnHeight), false);
	}
	if (bnImageType & DS_CLOSE)
		bnSysTemMenuClose.ApplyPngImages(wholeImage, CRect(0, (bnNum - 1) * bnHeight, bnWidth, bnNum * bnHeight), false, nImageNum, cAlphaThreshold);
}

int CDragDialog::GetWindowWidth() const
{
	return mClientRect.Width();
}

int CDragDialog::GetWindowHeight() const
{
	return mClientRect.Height();
}

const CRect& CDragDialog::GetSelfRect() const
{
	return mClientRect;
}

const CRect& CDragDialog::GetWindowRect() const
{
	return mWindowRect;
}

// 设定菜单样式
UINT CDragDialog::ModifyDragStyle(UINT nRemove, UINT nAdd)
{
	nSystemMenuStyle &= ~nRemove;
	nSystemMenuStyle |= nAdd;

	UpdateSystemMenuItemPostion(TRUE);
	return nSystemMenuStyle;
}

UINT CDragDialog::ModifyDragStyle(UINT nStyle)
{
	nSystemMenuStyle = nStyle;
	UpdateSystemMenuItemPostion(TRUE);
	return nSystemMenuStyle;
}

// 获得菜单样式
UINT CDragDialog::GetDragStyle() const
{
	return nSystemMenuStyle;
}

// 设定标题
void CDragDialog::SetWindowText(LPCTSTR lpszString)
{
	CDialogEx::SetWindowText(lpszString);
	szTitle.SetWindowText(lpszString);
}

// 获取窗口矩形
void CDragDialog::GetClientRect(LPRECT lpRect)
{
	CDialogEx::GetClientRect(lpRect);
	lpRect->top = CAPITIONHEIGHT;
}

// 拖拽窗口
LRESULT CDragDialog::OnNcHitTest(CPoint point)
{
	if (nSystemMenuStyle & DS_RESIZE)
	{
		auto& rect = GetWindowRect();
		if (point.x <= rect.left + DIALOGBORDERWIDTH && point.y <= rect.top + DIALOGBORDERWIDTH)
			return HTTOPLEFT;
		else if (point.x >= rect.right - DIALOGBORDERWIDTH && point.y <= rect.top + DIALOGBORDERWIDTH)
			return HTTOPRIGHT;
		else if (point.x <= rect.left + DIALOGBORDERWIDTH && point.y >= rect.bottom - DIALOGBORDERWIDTH)
			return HTBOTTOMLEFT;
		else if (point.x >= rect.right - DIALOGBORDERWIDTH && point.y >= rect.bottom - DIALOGBORDERWIDTH)
			return HTBOTTOMRIGHT;
		else if (point.x <= rect.left + DIALOGBORDERWIDTH)
			return HTLEFT;
		else if (point.x >= rect.right - DIALOGBORDERWIDTH)
			return HTRIGHT;
		else if (point.y <= rect.top + DIALOGBORDERWIDTH)
			return HTTOP;
		else if (point.y >= rect.bottom - DIALOGBORDERWIDTH)
			return HTBOTTOM;
	}

	LRESULT nHitTest = CDialogEx::OnNcHitTest(point);
	return ((nHitTest == HTCLIENT) && ((nSystemMenuStyle & DS_DRAGBODY) || ((ScreenToClient(&point)), point.y < CAPITIONHEIGHT))) ? HTCAPTION : nHitTest;
}

// 响应菜单
void CDragDialog::OnSystemMenuButtonClick(UINT nID)
{
	switch (nID)
	{
	case ID_DRAG_MINIMIZE:
		ShowWindow(SW_MINIMIZE);
		break;
	case ID_DRAG_MAXIMIZE:
		ShowWindow(SW_MAXIMIZE);
		break;
	case ID_DRAG_RESTORE:
		ShowWindow(SW_RESTORE);
		break;
	case ID_DRAG_CLOSE:
		EndDialog(IDCLOSE);
		break;
	default:
		break;
	}
}

// 调整控件位置
void CDragDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	CDialog::GetClientRect(&mClientRect);
	CDialog::GetWindowRect(&mWindowRect);

	UpdateSystemMenuItemPostion();
}

// 创建按钮等
int CDragDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 样式
	ModifyStyle(WS_CAPTION | WS_SYSMENU, WS_BORDER, 0);
	ModifyStyleEx(WS_EX_DLGMODALFRAME, 0, 0);

	// 创建窗口
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	/// 创建子控件

	// 菜单
	bnSysTemMenuClose.Create(TEXT("X"), NULL, RECT{ 0,0,MENUBUTTONWIDTH, MENUBUTTONHEIGHT }, this, ID_DRAG_CLOSE);
	bnSysTemMenuMaximize.Create(TEXT("[  ]"), NULL, RECT{ 0,0,MENUBUTTONWIDTH, MENUBUTTONHEIGHT }, this, ID_DRAG_MAXIMIZE);
	bnSysTemMenuRestore.Create(TEXT("<  >"), NULL, RECT{ 0,0,MENUBUTTONWIDTH, MENUBUTTONHEIGHT }, this, ID_DRAG_RESTORE);
	bnSysTemMenuMinimize.Create(TEXT("_"), NULL, RECT{ 0,0,MENUBUTTONWIDTH, MENUBUTTONHEIGHT }, this, ID_DRAG_MINIMIZE);

	// 标题
	szTitle.Create(TEXT(""), SS_CENTERIMAGE | SS_LEFTNOWORDWRAP, RECT{ 0, 0, CAPITIONWIDTH, MENUBUTTONHEIGHT }, this, ID_DIALOGTITLETEXT);

	// 加载默认按钮图像
#ifdef IDBDS_SYSTEMMENUIMG
	LoadSystemMenuButtonRCImage(IDBDS_SYSTEMMENUIMG);
#endif // IDBDS_SYSTEMMENUIMG

	// 应用样式
	ModifyDragStyle(0, nSystemMenuStyle);

	return 0;
}

// 拦截Enter Escape退出消息
BOOL CDragDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE && nSystemMenuStyle & DS_ESCAPECTRL)
			return TRUE;
		if (pMsg->wParam == VK_RETURN && nSystemMenuStyle & DS_ENTERCTRL)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

// 加载菜单按钮
void CDragDialog::LoadSystemMenuButtonFileImage(LPCTSTR pszFileName, UINT bnImageType, int nImageNum, BYTE cAlphaThreshold)
{
	CPng wholeImage;
	wholeImage.LoadPngFromFile(pszFileName);
	ApplySystemMenuImages(wholeImage, bnImageType, nImageNum, cAlphaThreshold);
}

// 加载菜单按钮
void CDragDialog::LoadSystemMenuButtonRCImage(LPCTSTR pszResourceName, UINT bnImageType, int nImageNum, BYTE cAlphaThreshold, LPCTSTR lpImageResType)
{
	CPng wholeImage;
	wholeImage.LoadPngFromResource(pszResourceName, true, lpImageResType);
	ApplySystemMenuImages(wholeImage, bnImageType, nImageNum, cAlphaThreshold);
}

// 加载菜单按钮
void CDragDialog::LoadSystemMenuButtonRCImage(UINT nReourceID, UINT bnImageType, int nImageNum, BYTE cAlphaThreshold, LPCTSTR lpImageResType)
{
	CPng wholeImage;
	wholeImage.LoadPngFromResource(nReourceID, true, lpImageResType);
	ApplySystemMenuImages(wholeImage, bnImageType, nImageNum, cAlphaThreshold);
}

// 防止全屏
void CDragDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME);
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

// 修改标题字体
HBRUSH CDragDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == ID_DIALOGTITLETEXT)
	{
		CFont font;
		font.CreatePointFont(120, _T("等线"));
		pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
	}
	return hbr;
}

// 防止双击最大化|还原
void CDragDialog::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	if (nSystemMenuStyle & DS_NCDBLCLK)
		CDialogEx::OnNcLButtonDblClk(nHitTest, point);
}

// 实时变化窗口位置
void CDragDialog::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	mWindowRect.MoveToXY(x, y);
}