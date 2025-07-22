
// circle_editor_MFCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "circle_editor_MFC.h"
#include "circle_editor_MFCDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

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
	DDX_Control(pDX, IDC_EDIT_POINT1_POSE, m_editPt1Pose);
	DDX_Control(pDX, IDC_EDIT_POINT2_POSE, m_editPt2Pose);
	DDX_Control(pDX, IDC_EDIT_POINT3_POSE, m_editPt3Pose);
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
	ON_MESSAGE(WM_USER_RANDOM_UPDATE, &CcircleeditorMFCDlg::OnRandomUpdate)
	ON_MESSAGE(WM_USER_RANDOM_FINISH, &CcircleeditorMFCDlg::OnRandomFinish)
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

	m_editPt1Pose.SetReadOnly(TRUE);
	m_editPt2Pose.SetReadOnly(TRUE);
	m_editPt3Pose.SetReadOnly(TRUE);

	UpdatePointPoseDisplays();

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
        CPaintDC dc(this); // 아이콘 전용 DC 생성

        // 배경 지우기 메시지 전송
        SendMessage(WM_ICONERASEBKGND,
            reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형 가져와 아이콘 중앙 좌표 계산
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon); // 아이콘 그리기
    }
    else
    {
        CPaintDC dc(this);               // 일반 그리기용 DC 생성
        m_pDrawMgr->UpdateBuffer();      // 백버퍼 갱신
        m_pDrawMgr->GetImage().Draw(dc, 0, 0); // 이미지 출력
    }
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CcircleeditorMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 엔터 키 입력 처리: 점 크기 및 원 두께 설정
BOOL CcircleeditorMFCDlg::PreTranslateMessage(MSG* pMsg)
{
    // 엔터 키가 눌렸을 때
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        // 포커스가 있는 컨트롤의 ID 확인
        CWnd* pFocus = GetFocus();
        int id = pFocus ? pFocus->GetDlgCtrlID() : 0;

        // 점 크기 에디트에서 엔터 → 해당 버튼 핸들러 호출
        if (id == IDC_EDIT_POINT_SIZE)
        {
            OnBnClickedBtnPointSize();
            return TRUE;  // 메시지 소모
        }
        // 원 두께 에디트에서 엔터 → 해당 버튼 핸들러 호출
        else if (id == IDC_EDIT_THICKNESS_SIZE)
        {
            OnBnClickedBtnThickness();
            return TRUE;
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

// 다이얼로그 종료 시 자원 정리
void CcircleeditorMFCDlg::OnDestroy()
{
    delete m_pDrawMgr;         // 그리기 매니저 메모리 해제
    CDialogEx::OnDestroy();
}

// 마우스 왼쪽 버튼 클릭 처리: 드래그 시작 또는 점 추가
void CcircleeditorMFCDlg::OnLButtonDown(UINT nFlags, CPoint ptClick)
{
    // 클릭한 위치의 점 인덱스 확인
    m_nDragIndex = m_pointMgr.HitTest(ptClick, m_pDrawMgr->GetPointRadius());
    if (m_nDragIndex >= 0)
    {
        // 드래그 대상이 있으면 캡처 시작
        SetCapture();
    }
    else if (!m_pointMgr.IsFull(3))
    {
        // 점이 3개 미만이면 새 점 추가
        m_pointMgr.AddPoint(ptClick);
        m_pDrawMgr->UpdateBuffer();      // 버퍼 업데이트 후 화면 무효화
        Invalidate();
        UpdatePointPoseDisplays();      // 좌표 표시 갱신
    }

    CDialogEx::OnLButtonDown(nFlags, ptClick);
}

// 마우스 이동: 드래그 중이면 점 위치 업데이트
void CcircleeditorMFCDlg::OnMouseMove(UINT nFlags, CPoint ptCursor)
{
    if (m_nDragIndex >= 0)
    {
        // 점 이동
        m_pointMgr.MovePoint(m_nDragIndex, ptCursor);
        m_pDrawMgr->UpdateBuffer();                   // 버퍼 갱신
        Invalidate(FALSE);                            // 부분 무효화
        UpdatePointPoseDisplays();                    // 좌표 표시 갱신
    }
    CDialogEx::OnMouseMove(nFlags, ptCursor);
}

// 마우스 왼쪽 버튼 해제: 드래그 종료
void CcircleeditorMFCDlg::OnLButtonUp(UINT nFlags, CPoint ptClickUp)
{
    if (m_nDragIndex >= 0)
    {
        ReleaseCapture();  // 캡처 해제
        m_nDragIndex = -1; // 드래그 인덱스 초기화
    }
    CDialogEx::OnLButtonUp(nFlags, ptClickUp);
}

// [버튼] 점 크기 설정: 1~30 범위만 허용
void CcircleeditorMFCDlg::OnBnClickedBtnPointSize()
{
    CString str;
    m_editPointSize.GetWindowTextW(str);
    int nRadius = _ttoi(str);
    if (nRadius > 0 && nRadius <= 30)
    {
        if (m_pDrawMgr)
            m_pDrawMgr->SetPointRadius(nRadius); // 반경 업데이트
        Invalidate();                          // 화면 다시 그리기
    }
    else
    {
        AfxMessageBox(_T("1~30 사이의 숫자를 입력하세요."));
    }
}

// [버튼] 원 두께 설정: 1~30 범위만 허용
void CcircleeditorMFCDlg::OnBnClickedBtnThickness()
{
    CString strInput;
    m_editCircleThickness.GetWindowTextW(strInput);
    int nThickness = _ttoi(strInput);
    if (nThickness > 0 && nThickness <= 30)
    {
        if (m_pDrawMgr)
            m_pDrawMgr->SetCircleThickness(nThickness); // 두께 업데이트
        Invalidate();                                 // 화면 무효화
    }
    else
    {
        AfxMessageBox(_T("1~30 사이의 숫자를 입력하세요."));
    }
}

// [버튼] 리셋: 점 제거 전 사용자 확인
void CcircleeditorMFCDlg::OnBnClickedBtnReset()
{
    if (AfxMessageBox(_T("RESET하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        m_pointMgr.Clear();            // 모든 점 제거
        UpdatePointPoseDisplays();     // 좌표 표시 초기화
        Invalidate();                  // 화면 갱신
    }
}

// [버튼] 랜덤 애니메이션 시작: 3개 점 위치가 필요
void CcircleeditorMFCDlg::OnBnClickedBtnRandom()
{
    if (!m_pointMgr.IsFull(3))
    {
        AfxMessageBox(_T("3개의 점을 먼저 찍어주세요."));
        return;
    }

    // 버튼 비활성화
    GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_RESET)->EnableWindow(FALSE);

    // 별도 스레드에서 10회 랜덤 위치 이동 수행
    AfxBeginThread([](LPVOID lpParam) -> UINT
        {
            auto pDlg = static_cast<CcircleeditorMFCDlg*>(lpParam);
            auto tpStartTotal = std::chrono::high_resolution_clock::now();

            for (int nStep = 0; nStep < 10; ++nStep)
            {
                auto tpStartFrame = std::chrono::high_resolution_clock::now();

                // 클라이언트 영역 및 점 반경 가져오기
                CRect rcClient;
                pDlg->GetClientRect(&rcClient);
                int nWidth = rcClient.Width();
                int nHeight = rcClient.Height();
                int nRadius = pDlg->m_pDrawMgr->GetPointRadius();

                // 점 위치 랜덤 범위 계산
                int nMinX = nRadius, nMaxX = max(nRadius, nWidth - nRadius);
                int nMinY = nRadius, nMaxY = max(nRadius, nHeight - nRadius);

                // 3개 점 위치 랜덤 이동
                for (int nIdx = 0; nIdx < 3; ++nIdx)
                {
                    int nX = rand() % (nMaxX - nMinX + 1) + nMinX;
                    int nY = rand() % (nMaxY - nMinY + 1) + nMinY;
                    pDlg->m_pointMgr.MovePoint(nIdx, CPoint(nX, nY));
                }

                // 메인 스레드에 화면 갱신 메시지 전송
                pDlg->PostMessage(WM_USER_RANDOM_UPDATE);

                auto tpEndFrame = std::chrono::high_resolution_clock::now();
                double dElapsedSec = std::chrono::duration<double>(tpEndFrame - tpStartFrame).count();
                std::cout << "Random #" << (nStep + 1)
                    << " 실행 시간: " << std::fixed << std::setprecision(6)
                    << dElapsedSec << "초\n";

                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 프레임 간 지연
            }

            // 총 실행 시간 출력 후 완료 메시지 전송
            auto tpEndTotal = std::chrono::high_resolution_clock::now();
            double dTotalSec = std::chrono::duration<double>(tpEndTotal - tpStartTotal).count();
            std::cout << "총 실행 시간: " << std::fixed << std::setprecision(6)
                << dTotalSec << "초\n";

            ::PostMessage(pDlg->GetSafeHwnd(), WM_USER_RANDOM_FINISH, 0, 0);
            return 0;
        }, this);
}

// WM_USER_RANDOM_UPDATE 처리: 화면 및 좌표 표시 갱신
LRESULT CcircleeditorMFCDlg::OnRandomUpdate(WPARAM, LPARAM)
{
    m_pDrawMgr->UpdateBuffer();
    Invalidate(FALSE);
    UpdatePointPoseDisplays();
    return 0;
}

// WM_USER_RANDOM_FINISH 처리: 버튼 재활성화
LRESULT CcircleeditorMFCDlg::OnRandomFinish(WPARAM, LPARAM)
{
    GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_RESET)->EnableWindow(TRUE);
    return 0;
}

// 점 좌표 에디트 박스에 현재 위치 출력
void CcircleeditorMFCDlg::UpdatePointPoseDisplays()
{
    const auto& pts = m_pointMgr.GetPoints();
    CEdit* edits[3] = { &m_editPt1Pose, &m_editPt2Pose, &m_editPt3Pose };

    for (int i = 0; i < 3; ++i)
    {
        if (i < (int)pts.size())
        {
            CString str;
            str.Format(_T("(%d, %d)"), pts[i].x, pts[i].y);
            edits[i]->SetWindowTextW(str);
        }
        else
        {
            edits[i]->SetWindowTextW(_T("")); // 점 없으면 빈 문자열
        }
    }
}