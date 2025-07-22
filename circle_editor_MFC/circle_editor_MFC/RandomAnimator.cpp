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

// ���̾�α� ������, �� ������, �׸��� �����ڸ� �ʱ�ȭ
RandomAnimator::RandomAnimator(CWnd* pOwner, PointManager* pPtMgr, DrawManager* pDrawMgr)
    : m_pOwner(pOwner)
    , m_pPtMgr(pPtMgr)
    , m_pDrawMgr(pDrawMgr)
{
}

// ��׶��� ������ ����
void RandomAnimator::Start()
{
    AfxBeginThread(&RandomAnimator::ThreadProc, this);
}

// 10ȸ �ݺ��ϸ� 500ms �������� �� ��ġ�� ���� �̵�
UINT RandomAnimator::ThreadProc(LPVOID lpParam)
{
    auto* pThis = static_cast<RandomAnimator*>(lpParam);
    // ��ü ���� �ð� ���� ����
    auto tpStartTotal = std::chrono::high_resolution_clock::now();

    // 10�� �ݺ� �ִϸ��̼� ����
    for (int nStep = 0; nStep < 10; ++nStep)
    {
        // �� ������ ���� �ð� ���� ����
        auto tpStartFrame = std::chrono::high_resolution_clock::now();

        // Ŭ���̾�Ʈ ���� ũ��� �� �ݰ����� ���� ���� ���
        CRect rcClient;
        pThis->m_pOwner->GetClientRect(&rcClient);
        int width = rcClient.Width();
        int height = rcClient.Height();
        int radius = pThis->m_pDrawMgr->GetPointRadius();

        int minX = radius;
        int maxX = std::max(radius, width - radius);
        int minY = radius;
        int maxY = std::max(radius, height - radius);

        // �� �� ���� �� ���� ��ǥ�� �̵�
        for (int i = 0; i < 3; ++i)
        {
            int x = rand() % (maxX - minX + 1) + minX;
            int y = rand() % (maxY - minY + 1) + minY;
            pThis->m_pPtMgr->MovePoint(i, CPoint(x, y));
        }

        // ȭ�� ���� ��û �޽��� ����
        pThis->m_pOwner->PostMessage(WM_USER_RANDOM_UPDATE);

        // ������ ���� �ð� ��� �� �α� ���
        auto tpEndFrame = std::chrono::high_resolution_clock::now();
        double frameSec = std::chrono::duration<double>(tpEndFrame - tpStartFrame).count();
        std::cout << "���� �ִϸ��̼� #" << (nStep + 1)
            << " ���� �ð�: " << std::fixed << std::setprecision(6) << frameSec << "��\n";

        // ���� ������ �� 500ms ���
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // ��ü ���� �ð� ���� ���� �� �α� ���
    auto tpEndTotal = std::chrono::high_resolution_clock::now();
    double totalSec = std::chrono::duration<double>(tpEndTotal - tpStartTotal).count();
    std::cout << "�� ���� �ð�: " << std::fixed << std::setprecision(6) << totalSec << "��\n";

    // �ִϸ��̼� ���� ó�� �޽��� ����
    pThis->m_pOwner->PostMessage(WM_USER_RANDOM_FINISH);
    return 0;
}
