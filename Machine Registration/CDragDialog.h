#pragma once

#include "CImageButton.h"


namespace thatboy
{
	namespace mfc
	{
		// CDragDialog 对话框
		/* BUG:
		 窗口创建时，（伪）标题栏的系统按钮位置大约左偏移了15个像素
		*/
		class CDragDialog : public CDialogEx
		{
			DECLARE_DYNAMIC(CDragDialog)

		public:
			CDragDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
			virtual ~CDragDialog();

			// 对话框数据
		//#ifdef AFX_DESIGN_TIME
		//	enum { IDD = IDD_DRAGDIALOG };
		//#endif

		protected:

			// 系统菜单按钮
			CImageButton bnSysTemMenuMinimize;
			CImageButton bnSysTemMenuMaximize;
			CImageButton bnSysTemMenuRestore;
			CImageButton bnSysTemMenuClose;

			// 菜单样式
			UINT nSystemMenuStyle = DS_DEFAULT;

			// 标题
			CStatic szTitle;

			// 图标
			CPng mIcon;

			CRect mClientRect;
			CRect mWindowRect;

		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

			DECLARE_MESSAGE_MAP()

			void UpdateSystemMenuItemPostion(BOOL menuChange = FALSE);

		public:

			// 系统菜单选择
			enum :UINT {
				// 可见
				DS_CLOSE = 0X0001
				, DS_MINIMIZE = 0X0002
				, DS_MAXIMIZE = 0X0004

				// 可用
				, DS_CLOSEABLE = 0X0010
				, DS_MINIMIZEABLE = 0X0020
				, DS_MAXIMIZEABLE = 0X0040

				// 其他
				, DS_CAPTIONTEXT = 0X0008	// 标题文字可见
				, DS_CAPTIONICON = 0X0080	// 标题图标可见

				, DS_ENTERCTRL = 0X0100		// 允许回车关闭
				, DS_ESCAPECTRL = 0X0200	// 允许ESC关闭
				, DS_DRAGBODY = 0X0400		// 允许客户区拖拽
				, DS_NCDBLCLK = 0X0800		// 允许对可拖动区域双击
				, DS_RESIZE = 0X1000		// 允许拖动边框改变窗口大小

				// 组合
				, DS_SYSMENUABLE = DS_CLOSEABLE | DS_MINIMIZEABLE | DS_MAXIMIZEABLE
				, DS_SYSMENUSHOW = DS_CLOSE | DS_MINIMIZE | DS_MAXIMIZE
				, DS_TIPBAR = DS_DRAGBODY | DS_SYSMENUABLE | DS_CLOSE | DS_ENTERCTRL | DS_ESCAPECTRL | DS_CAPTIONTEXT
				, DS_TOOLBAR = DS_DRAGBODY | DS_SYSMENUABLE | DS_CLOSE | DS_MINIMIZE | DS_ESCAPECTRL | DS_CAPTIONTEXT
				, DS_DEFAULT = DS_DRAGBODY | DS_SYSMENUABLE | DS_SYSMENUSHOW | DS_CAPTIONTEXT
			};

			// 系统菜单ID
			enum :UINT {
				ID_DRAG_MINIMIZE = 2345
				, ID_DRAG_MAXIMIZE
				, ID_DRAG_RESTORE
				, ID_DRAG_CLOSE
				, ID_DIALOGTITLETEXT = 3033
			};

			enum :int {
				CAPITIONWIDTH = 200
				, CAPITIONHEIGHT = 40
				, CAPITIONGAP = 10
				, MENUBUTTONWIDTH = 36
				, MENUBUTTONHEIGHT = 36
				, MENUBUTTONGAP = 4
				, MENUBUTTONRANGEWIDTH = MENUBUTTONWIDTH + MENUBUTTONGAP
				, MENUBUTTONRANGEHEIGHT = MENUBUTTONHEIGHT + MENUBUTTONGAP
				, DIALOGBORDERWIDTH = 10
			};

		public:
			UINT ModifyDragStyle(UINT nRemove, UINT nAdd);	// 修改系统菜单样式
			UINT ModifyDragStyle(UINT nStyle);				// 修改系统菜单样式
			UINT GetDragStyle()const;						// 获取系统菜单样式

			void SetWindowText(LPCTSTR lpszString);					// 修改标题
			void GetClientRect(LPRECT lpRect);						// 获得客户区域

			afx_msg LRESULT OnNcHitTest(CPoint point);				// 拖拽
			afx_msg void OnSystemMenuButtonClick(UINT nID);			// 处理系统菜单
			afx_msg void OnSize(UINT nType, int cx, int cy);		// 处理菜单位置移动
			afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 生成菜单按钮等
			afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);		// 去除任务栏覆盖
			afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
			afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
			afx_msg void OnMove(int x, int y);

			virtual BOOL PreTranslateMessage(MSG* pMsg);			// 拦截Enter Escape按键

			// 加载菜单按钮
			void LoadSystemMenuButtonFileImage(LPCTSTR pszFileName, UINT bnImageType = DS_CLOSE | DS_MINIMIZE | DS_MAXIMIZE, int nImageNum = 4, BYTE cAlphaThreshold = 1);
			void LoadSystemMenuButtonRCImage(LPCTSTR pszResourceName, UINT bnImageType = DS_CLOSE | DS_MINIMIZE | DS_MAXIMIZE, int nImageNum = 4, BYTE cAlphaThreshold = 1, LPCTSTR lpImageResType = TEXT("PNG"));
			void LoadSystemMenuButtonRCImage(UINT nReourceID, UINT bnImageType = DS_CLOSE | DS_MINIMIZE | DS_MAXIMIZE, int nImageNum = 4, BYTE cAlphaThreshold = 1, LPCTSTR lpImageResType = TEXT("PNG"));
			// 应用菜单图像
			void ApplySystemMenuImages(const CPng& wholeImage, UINT bnImageType = DS_CLOSE | DS_MINIMIZE | DS_MAXIMIZE, int nImageNum = 4, BYTE cAlphaThreshold = 1);

			int GetWindowWidth()const;
			int GetWindowHeight()const;
			const CRect& GetSelfRect()const;
			const CRect& GetWindowRect()const;
		};

	}
}