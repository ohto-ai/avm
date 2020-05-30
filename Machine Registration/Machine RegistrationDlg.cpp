
// Machine RegistrationDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Machine Registration.h"
#include "Machine RegistrationDlg.h"
#include "afxdialogex.h"
#define MACHINE_REG_ADMINISTRATORS_CALL
#include "../MachineRegistration/MachineRegistration.h"
#include "../TBCpuInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMachineRegistrationDlg 对话框



CMachineRegistrationDlg::CMachineRegistrationDlg(CWnd* pParent /*=nullptr*/)
	: thatboy::mfc::CDragDialog(IDD_MACHINEREGISTRATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMachineRegistrationDlg::DoDataExchange(CDataExchange* pDX)
{
	thatboy::mfc::CDragDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMachineRegistrationDlg, thatboy::mfc::CDragDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GENERATE, &CMachineRegistrationDlg::OnBnClickedGenerateRegCode)
	ON_BN_CLICKED(IDC_FILLCODE, &CMachineRegistrationDlg::OnBnClickedFillcode)
	ON_BN_CLICKED(IDC_FILLMACHINID, &CMachineRegistrationDlg::OnBnClickedFillmachinid)
END_MESSAGE_MAP()


// CMachineRegistrationDlg 消息处理程序

BOOL CMachineRegistrationDlg::OnInitDialog()
{
	thatboy::mfc::CDragDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CDragDialog::ModifyDragStyle(DS_TOOLBAR | DS_ENTERCTRL);
	CDragDialog::SetWindowText(TEXT("机器注册"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMachineRegistrationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMachineRegistrationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMachineRegistrationDlg::OnBnClickedGenerateRegCode()
{
	char machineId[MAX_PATH];
	char envirment[MAX_PATH];
	char registrationCode[MAX_PATH];
	int key;

	GetDlgItemText(IDC_MACHINECODE, machineId, MAX_PATH);
	GetDlgItemText(IDC_MACHINEARGV, envirment, MAX_PATH);
	
	key = RegisterClient(machineId, envirment, registrationCode);

	SetDlgItemText(IDC_REGISTRATIONCODE, registrationCode);
	SetDlgItemInt(IDC_KEYVALUE, key);
}


void CMachineRegistrationDlg::OnBnClickedFillcode()
{
	SetDlgItemText(IDC_MACHINEARGV, thatboy::getCpuVendor().c_str());
}


void CMachineRegistrationDlg::OnBnClickedFillmachinid()
{
	SetDlgItemText(IDC_MACHINECODE, QueryMachineId());
}
