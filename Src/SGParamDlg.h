#if !defined(AFX_SGPARAMDLG_H__4385612C_9C2E_4423_98EF_0B55BB623E8A__INCLUDED_)
#define AFX_SGPARAMDLG_H__4385612C_9C2E_4423_98EF_0B55BB623E8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SGParamDlg.h : header file
//

#include "GrabberRegularForSG.h"
#include "SimulateGrabber.h"

/////////////////////////////////////////////////////////////////////////////
// CSGParamDlg dialog

class CSGParamDlg : public CDialog
{
// Construction
public:
	CSGParamDlg(CWnd* pParent = NULL);   // standard constructor
	void SetOwner(CSimulateGrabber *pGrabber);

	void UpdateDataFromSG();// 从SG对象中更新数据到对话框 // [2009-12-28 by jzj]: add

// Dialog Data
	//{{AFX_DATA(CSGParamDlg)
	enum { IDD = IDD_DIALOG_PARAM };
	BOOL	m_bIsPlaneRGB;
	int		m_nGrabSpeed;
	CString	m_strImagesPath;
	int		m_nChannelNum;
	int		m_nImageHeight;
	int		m_nImageWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSGParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSimulateGrabber *m_pGrabber;

	// Generated message map functions
	//{{AFX_MSG(CSGParamDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStartGrab();
	afx_msg void OnButtonStopGrab();
	afx_msg void OnButtonSaveToFile();
	afx_msg void OnButtonSnapshot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SGPARAMDLG_H__4385612C_9C2E_4423_98EF_0B55BB623E8A__INCLUDED_)
