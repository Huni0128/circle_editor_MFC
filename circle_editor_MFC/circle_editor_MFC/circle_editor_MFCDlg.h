// circle_editor_MFCDlg.h: 헤더 파일
//

#pragma once

#include "PointManager.h"
#include "DrawManager.h"
#include "CircleUtils.h"
#include <afxwin.h>
#include <afxdlgs.h>

// CcircleeditorMFCDlg 대화 상자
class CcircleeditorMFCDlg : public CDialogEx
{
private:
	CEdit m_editPointSize;
	CEdit m_editCircleThickness;
	PointManager m_pointMgr;
	DrawManager* m_pDrawMgr = nullptr;

	int m_nDragIndex = -1;
	// 생성입니다.
public:
	CcircleeditorMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLE_EDITOR_MFC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnBnClickedBtnPointSize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint ptClick);
	afx_msg void OnBnClickedBtnThickness();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
