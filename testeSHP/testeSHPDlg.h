// testeSHPDlg.h : header file
//

#if !defined(AFX_TESTESHPDLG_H__436E6D79_445C_4A7D_B09C_94C18D07347C__INCLUDED_)
#define AFX_TESTESHPDLG_H__436E6D79_445C_4A7D_B09C_94C18D07347C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTesteSHPDlg dialog

#include "GIS.MapControl.h"
using MapGIS::MapControl;

class CTesteSHPDlg : public CDialog
{
// Construction
public:
	CTesteSHPDlg(CWnd* pParent = NULL);	// standard constructor
	//MapControl	*m_Map;


// Dialog Data
	//{{AFX_DATA(CTesteSHPDlg)
	enum { IDD = IDD_TESTESHP_DIALOG };
	CStatic	m_st_Query;
	CListBox	m_Lst_Layers;
	CStatic	m_stCoord;
	MapControl	Map;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesteSHPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTesteSHPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton2();
	afx_msg void OnBtZoom();
	afx_msg void OnBtExtents();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	virtual void OnOK();
	afx_msg void OnBtAdd();
	afx_msg void OnBtMoveUp();
	afx_msg void OnBtProp();
	afx_msg void OnBtRemove();
	afx_msg void OnBtMoveDown();
	afx_msg void OnBtCenter();
	afx_msg void OnBtZmais();
	afx_msg void OnBtZoomObj();
	afx_msg void OnBtSetFields();
	afx_msg void OnSelchangeLstLayers();
	//}}AFX_MSG
	afx_msg LRESULT OnCoordinate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelected(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTESHPDLG_H__436E6D79_445C_4A7D_B09C_94C18D07347C__INCLUDED_)
