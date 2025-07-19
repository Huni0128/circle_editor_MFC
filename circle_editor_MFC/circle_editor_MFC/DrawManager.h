#pragma once
#include "PointManager.h"
#include "CircleUtils.h"
#include <afxwin.h>

class DrawManager {
private:
    PointManager* m_pointMgr;
    int m_nPointRadius = 5;
    int m_nCircleThickness = 2;

public:
    DrawManager(PointManager* mgr);

    void SetPointRadius(int nRadius) { m_nPointRadius = nRadius; }
    void SetCircleThickness(int nThickness) { m_nCircleThickness = nThickness; }

    void Draw(CDC* pDC);
};
