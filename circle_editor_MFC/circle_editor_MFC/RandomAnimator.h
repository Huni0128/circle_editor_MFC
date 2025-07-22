#pragma once

#include "PointManager.h"
#include "DrawManager.h"

#define WM_USER_RANDOM_UPDATE  (WM_USER + 1)
#define WM_USER_RANDOM_FINISH (WM_USER + 2)

class RandomAnimator
{
public:
    RandomAnimator(CWnd* pOwner, PointManager* pPtMgr, DrawManager* pDrawMgr);
    void Start();

private:
    static UINT ThreadProc(LPVOID lpParam);

    CWnd* m_pOwner;
    PointManager* m_pPtMgr;
    DrawManager* m_pDrawMgr;
};
