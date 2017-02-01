// testeSHPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testeSHP.h"
#include "testeSHPDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesteSHPDlg dialog

CTesteSHPDlg::CTesteSHPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesteSHPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTesteSHPDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesteSHPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTesteSHPDlg)
	DDX_Control(pDX, IDC_ST_QUERY, m_st_Query);
	DDX_Control(pDX, IDC_LST_LAYERS, m_Lst_Layers);
	DDX_Control(pDX, IDC_ST_COORD, m_stCoord);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTesteSHPDlg, CDialog)
	//{{AFX_MSG_MAP(CTesteSHPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BT_ZOOM, OnBtZoom)
	ON_BN_CLICKED(IDC_BT_EXTENTS, OnBtExtents)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BT_ADD, OnBtAdd)
	ON_BN_CLICKED(IDC_BT_MOVE_UP, OnBtMoveUp)
	ON_BN_CLICKED(IDC_BT_PROP, OnBtProp)
	ON_BN_CLICKED(IDC_BT_REMOVE, OnBtRemove)
	ON_BN_CLICKED(IDC_BT_MOVE_DOWN, OnBtMoveDown)
	ON_BN_CLICKED(IDC_BT_CENTER, OnBtCenter)
	ON_BN_CLICKED(IDC_BT_ZMAIS, OnBtZmais)
	ON_BN_CLICKED(IDC_BT_ZOOM_OBJ, OnBtZoomObj)
	ON_BN_CLICKED(IDC_BT_SET_FIELDS, OnBtSetFields)
	ON_MESSAGE(WMU_COORDINATE, OnCoordinate)
	ON_MESSAGE(WMU_QUERYRESULT, OnSelected)
	ON_LBN_SELCHANGE(IDC_LST_LAYERS, OnSelchangeLstLayers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesteSHPDlg message handlers

BOOL CTesteSHPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	Map.Attach(this,IDC_MAP);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTesteSHPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTesteSHPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTesteSHPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTesteSHPDlg::OnButton2() 
{
	CString strTexto;
	((CEdit*)GetDlgItem(IDC_ED_REG))->GetWindowText(strTexto);
	strTexto = Map.QueryObject(atol(strTexto));
	
	if(strTexto.GetLength() > 0)
		AfxMessageBox(strTexto);
}

void CTesteSHPDlg::OnBtZoom() 
{
	Map.Command((MapGIS::PointerMode)5);
}
void CTesteSHPDlg::OnBtExtents() 
{
	Map.ZoomExtents();
}

void CTesteSHPDlg::OnButton3() 
{
	CString strTexto;
	((CEdit*)GetDlgItem(IDC_ED_REG))->GetWindowText(strTexto);
	Map.SelectObject(atol(strTexto));	
}

void CTesteSHPDlg::OnButton4() 
{
	Map.ClearSelected();
}

LRESULT CTesteSHPDlg::OnCoordinate(WPARAM wParam, LPARAM lParam)
{
	double *x = (double*)wParam;
	double *y = (double*)lParam;

	CString strTexto;
	strTexto.Format("%f - %f",*x,*y);

	m_stCoord.SetWindowText(strTexto);

	return 0L;
}

LRESULT CTesteSHPDlg::OnSelected(WPARAM wParam, LPARAM lParam)
{
	CString *sTexto = (CString*)lParam;

	if(sTexto->GetLength() > 0)
		AfxMessageBox(*sTexto);

	return 0L;
}

void CTesteSHPDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CTesteSHPDlg::OnBtAdd() 
{
	CFileDialog dlg(TRUE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.shp") ;
	if(dlg.DoModal() == IDOK)
	{
		Map.AddLayer(dlg.GetPathName());
		m_Lst_Layers.AddString(dlg.GetPathName());
		Map.ZoomExtents();
	}
}

void CTesteSHPDlg::OnBtMoveUp() 
{
	// TODO: Add your control notification handler code here
	
}

void CTesteSHPDlg::OnBtProp() 
{
	// TODO: Add your control notification handler code here
	
}

void CTesteSHPDlg::OnBtRemove() 
{
	int iSelected = m_Lst_Layers.GetCurSel();
	if(iSelected != -1)
	{
		Map.RemoveLayer(iSelected);
		m_Lst_Layers.DeleteString(iSelected);
	}
}

void CTesteSHPDlg::OnBtMoveDown() 
{
	// TODO: Add your control notification handler code here
	
}

void CTesteSHPDlg::OnBtCenter() 
{
	Map.Command((MapGIS::PointerMode)4);
}

void CTesteSHPDlg::OnBtZmais() 
{
	Map.Command((MapGIS::PointerMode)2);
}


void CTesteSHPDlg::OnBtZoomObj() 
{
	CString strTexto;
	((CEdit*)GetDlgItem(IDC_ED_REG))->GetWindowText(strTexto);
	Map.ZoomObject(atol(strTexto));	
}

void CTesteSHPDlg::OnBtSetFields() 
{
	Map.SetQueryFields("TERCDLOTE|TERCDZONA");
}

void CTesteSHPDlg::OnSelchangeLstLayers() 
{
	long iSelecionado = -1;
	iSelecionado = m_Lst_Layers.GetCurSel();
	
	if(iSelecionado >= 0) 
		Map.SetLayerActive(iSelecionado);
}
