
// circle_editor_MFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "circle_editor_MFC.h"
#include "circle_editor_MFCDlg.h"
#include "afxdialogex.h"
#include <atltime.h>
#include <chrono>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CcircleeditorMFCDlg 대화 상자

CcircleeditorMFCDlg::CcircleeditorMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLE_EDITOR_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcircleeditorMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_POINT_SIZE, m_editPointSize);
	DDX_Control(pDX, IDC_EDIT_THICKNESS_SIZE, m_editCircleThickness);
}

BEGIN_MESSAGE_MAP(CcircleeditorMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_POINT_SIZE, &CcircleeditorMFCDlg::OnBnClickedBtnPointSize)
	ON_BN_CLICKED(IDC_BTN_THICKNESS, &CcircleeditorMFCDlg::OnBnClickedBtnThickness)
	ON_BN_CLICKED(IDC_BTN_RESET, &CcircleeditorMFCDlg::OnBnClickedBtnReset)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CcircleeditorMFCDlg::OnBnClickedBtnRandom)
END_MESSAGE_MAP()


// CcircleeditorMFCDlg 메시지 처리기

BOOL CcircleeditorMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	srand(static_cast<unsigned>(time(nullptr)));

	m_pDrawMgr = new DrawManager(&m_pointMgr);

	CRect rc;
	GetClientRect(&rc);
	m_pDrawMgr->CreateImage(rc.Width(), rc.Height(), 8);
	m_pDrawMgr->UpdateBuffer();

	Invalidate();

	CString tmp;
	tmp.Format(_T("%d"), m_pDrawMgr->GetPointRadius());
	m_editPointSize.SetWindowTextW(tmp);
	tmp.Format(_T("%d"), m_pDrawMgr->GetCircleThickness());
	m_editCircleThickness.SetWindowTextW(tmp);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CcircleeditorMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CcircleeditorMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		m_pDrawMgr->UpdateBuffer();
		m_pDrawMgr->GetImage().Draw(dc, 0, 0);
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CcircleeditorMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CcircleeditorMFCDlg::OnLButtonDown(UINT nFlags, CPoint ptClick)
{
	m_nDragIndex = m_pointMgr.HitTest(ptClick, m_pDrawMgr->GetPointRadius());
	if (m_nDragIndex >= 0)
	{
		SetCapture();
	}
	else if (!m_pointMgr.IsFull(3))
	{
		m_pointMgr.AddPoint(ptClick);
		m_pDrawMgr->UpdateBuffer();
		Invalidate();
	}

	CDialogEx::OnLButtonDown(nFlags, ptClick);
}

void CcircleeditorMFCDlg::OnBnClickedBtnPointSize()
{
	CString str;
	m_editPointSize.GetWindowTextW(str);
	int nRadius = _ttoi(str);
	if (nRadius > 0 && nRadius <= 30)
	{
		if (m_pDrawMgr)
			m_pDrawMgr->SetPointRadius(nRadius);
		Invalidate();
	}
	else
	{
		AfxMessageBox(_T("1~30 사이의 숫자를 입력하세요."));
	}
}

void CcircleeditorMFCDlg::OnBnClickedBtnThickness()
{
	CString str;
	m_editCircleThickness.GetWindowTextW(str);
	int nThick = _ttoi(str);
	if (nThick > 0 && nThick <= 30)
	{
		if (m_pDrawMgr)
			m_pDrawMgr->SetCircleThickness(nThick);
		Invalidate();
	}
	else
	{
		AfxMessageBox(_T("1~30 사이의 숫자를 입력하세요."));
	}
}

void CcircleeditorMFCDlg::OnBnClickedBtnReset()
{
	if (AfxMessageBox(_T("RESET하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		m_pointMgr.Clear();
		Invalidate();
	}
}

void CcircleeditorMFCDlg::OnLButtonUp(UINT nFlags, CPoint ptClickUp)
{
	if (m_nDragIndex >= 0)
	{
		ReleaseCapture();
		m_nDragIndex = -1;
	}
	CDialogEx::OnLButtonUp(nFlags, ptClickUp);
}

void CcircleeditorMFCDlg::OnMouseMove(UINT nFlags, CPoint ptCursor)
{
	if (m_nDragIndex >= 0)
	{
		m_pointMgr.MovePoint(m_nDragIndex, ptCursor);
		m_pDrawMgr->UpdateBuffer();
		Invalidate(FALSE);
	}
	CDialogEx::OnMouseMove(nFlags, ptCursor);
}

void CcircleeditorMFCDlg::OnBnClickedBtnRandom()
{
	if (!m_pointMgr.IsFull(3))
	{
		AfxMessageBox(_T("3개의 점을 모두 찍은 후에 랜덤 이동을 실행하세요."));
		return;
	}

	auto t_start = std::chrono::high_resolution_clock::now();

	CRect rcClient;
	GetClientRect(&rcClient);
	int width = rcClient.Width();
	int height = rcClient.Height();
	int r = m_pDrawMgr->GetPointRadius();

	int minX = r, maxX = max(r, width - r);
	int minY = r, maxY = max(r, height - r);

	for (int nIdx = 0; nIdx < 3; ++nIdx)
	{
		int nX = rand() % (maxX - minX + 1) + minX;
		int nY = rand() % (maxY - minY + 1) + minY;
		m_pointMgr.MovePoint(nIdx, CPoint(nX, nY));
	}

	m_pDrawMgr->UpdateBuffer();
	Invalidate();

	auto t_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = t_end - t_start;

	std::cout << "OnBnClickedBtnRandom 실행 소요 시간: "
		<< elapsed.count() << "초\n";
}


void CcircleeditorMFCDlg::OnDestroy()
{
	delete m_pDrawMgr;
	CDialogEx::OnDestroy();
}
