#pragma once

#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_

#include "CPng.hpp"

namespace thatboy
{
	namespace mfc {

		// CImageButton

		class CImageButton : public CButton
		{
			DECLARE_DYNAMIC(CImageButton)

		public:
			CImageButton();
			virtual ~CImageButton();
		protected:
			DECLARE_MESSAGE_MAP()
			CPng m_bnImageU;
			CPng m_bnImageD;
			CPng m_bnImageF;
			CPng m_bnImageX;

			BOOL m_bOver;
			BOOL m_bTracking;

		public:
			/*
				加载PNG图像文件
				pszFileName 文件名
				size2cont 大小跟随内容
				nImageNum 图像数量
				cAlphaThreshold 透明色阈值
			*/
			void LoadImagesFromFile(LPCTSTR pszFileName, bool bSizeToCont = true, int nImageNum = 4, BYTE cAlphaThreshold = 1);

			/*
				加载PNG图像资源
				pszResourceName 资源名称
				size2cont 大小跟随内容
				nImageNum 图像数量
				cAlphaThreshold 透明色阈值
				lpImageResType 资源类型
			*/
			void LoadImagesFromResource(LPCTSTR pszResourceName, bool bSizeToCont = true, int nImageNum = 4, BYTE cAlphaThreshold = 1, LPCTSTR lpImageResType = TEXT("PNG"));

			/*
				加载PNG图像资源
				nReourceID 资源ID
				size2cont 大小跟随内容
				nImageNum 图像数量
				cAlphaThreshold 透明色阈值
				lpImageResType 资源类型
			*/
			void LoadImagesFromResource(UINT nReourceID, bool bSizeToCont = true, int nImageNum = 4, BYTE cAlphaThreshold = 1, LPCTSTR lpImageResType = TEXT("PNG"));

			/*
				应用PNG图像
				wholeImage Png图像
				size2cont 大小跟随内容
				nImageNum 图像数量
				cAlphaThreshold 透明色阈值
			*/
			void ApplyPngImages(const CPng& wholeImage, CRect usedRect = CRect(), bool bSizeToCont = true, int nImageNum = 4, BYTE cAlphaThreshold = 1);

			BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT nImageID = NULL);
			virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
			afx_msg void OnMouseMove(UINT nFlags, CPoint point);
			afx_msg void OnMouseLeave();
			afx_msg void OnMouseHover(UINT nFlags, CPoint point);
			afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		};
	}
}

#endif // !_IMAGEBUTTON_H_