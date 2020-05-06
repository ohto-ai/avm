
// Machine RegistrationDlg.h: 头文件
//

#pragma once
#include "CDragDialog.h"

// CMachineRegistrationDlg 对话框
class CMachineRegistrationDlg : public thatboy::mfc::CDragDialog
{
// 构造
public:
	CMachineRegistrationDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MACHINEREGISTRATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGenerateRegCode();
	afx_msg void OnBnClickedFillcode();
	afx_msg void OnBnClickedFillmachinid();
};
