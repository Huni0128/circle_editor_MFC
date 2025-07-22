#include "pch.h"
#include "RandomAnimator.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef max
#undef max
#endif

// 다이얼로그 윈도우, 점 관리자, 그리기 관리자를 초기화
RandomAnimator::RandomAnimator(CWnd* pOwner, PointManager* pPtMgr, DrawManager* pDrawMgr)
    : m_pOwner(pOwner)
    , m_pPtMgr(pPtMgr)
    , m_pDrawMgr(pDrawMgr)
{
}

// 백그라운드 스레드 생성
void RandomAnimator::Start()
{
    AfxBeginThread(&RandomAnimator::ThreadProc, this);
}

// 10회 반복하며 500ms 간격으로 점 위치를 랜덤 이동
UINT RandomAnimator::ThreadProc(LPVOID lpParam)
{
    auto* pThis = static_cast<RandomAnimator*>(lpParam);
    // 전체 실행 시간 측정 시작
    auto tpStartTotal = std::chrono::high_resolution_clock::now();

    // 10번 반복 애니메이션 수행
    for (int nStep = 0; nStep < 10; ++nStep)
    {
        // 한 프레임 실행 시간 측정 시작
        auto tpStartFrame = std::chrono::high_resolution_clock::now();

        // 클라이언트 영역 크기와 점 반경으로 랜덤 범위 계산
        CRect rcClient;
        pThis->m_pOwner->GetClientRect(&rcClient);
        int width = rcClient.Width();
        int height = rcClient.Height();
        int radius = pThis->m_pDrawMgr->GetPointRadius();

        int minX = radius;
        int maxX = std::max(radius, width - radius);
        int minY = radius;
        int maxY = std::max(radius, height - radius);

        // 세 개 점을 새 랜덤 좌표로 이동
        for (int i = 0; i < 3; ++i)
        {
            int x = rand() % (maxX - minX + 1) + minX;
            int y = rand() % (maxY - minY + 1) + minY;
            pThis->m_pPtMgr->MovePoint(i, CPoint(x, y));
        }

        // 화면 갱신 요청 메시지 전송
        pThis->m_pOwner->PostMessage(WM_USER_RANDOM_UPDATE);

        // 프레임 실행 시간 계산 및 로그 출력
        auto tpEndFrame = std::chrono::high_resolution_clock::now();
        double frameSec = std::chrono::duration<double>(tpEndFrame - tpStartFrame).count();
        std::cout << "랜덤 애니메이션 #" << (nStep + 1)
            << " 실행 시간: " << std::fixed << std::setprecision(6) << frameSec << "초\n";

        // 다음 프레임 전 500ms 대기
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // 전체 실행 시간 측정 종료 및 로그 출력
    auto tpEndTotal = std::chrono::high_resolution_clock::now();
    double totalSec = std::chrono::duration<double>(tpEndTotal - tpStartTotal).count();
    std::cout << "총 실행 시간: " << std::fixed << std::setprecision(6) << totalSec << "초\n";

    // 애니메이션 종료 처리 메시지 전송
    pThis->m_pOwner->PostMessage(WM_USER_RANDOM_FINISH);
    return 0;
}
