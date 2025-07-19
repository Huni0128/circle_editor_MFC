
// circle_editor_MFCDlg.h: 헤더 파일
//

#pragma once
#include <vector>

// CcircleeditorMFCDlg 대화 상자
class CcircleeditorMFCDlg : public CDialogEx
{
private:
	CEdit m_editPointSize;
	int m_nPointRadius = 5;
	std::vector<CPoint> m_vecClickPoints;
	int m_nMaxPoints = 3;
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
	afx_msg void OnEnChangeEditPointSize();
	afx_msg void OnBnClickedBtnPointSize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void AddClickPoint(const CPoint& pt);
	BOOL IsClickLimitReached() const;
	void DrawClickPoints(CDC* pDC);
};
