#include "pch.h"
#include "DrawManager.h"

DrawManager::DrawManager(PointManager* mgr)
    : m_pointMgr(mgr)
{
}

void DrawManager::Draw(CDC* pDC)
{
    const auto& points = m_pointMgr->GetPoints();

    CBrush brush(RGB(0, 0, 0));
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    for (const auto& pt : points) {
        pDC->Ellipse(pt.x - m_nPointRadius, pt.y - m_nPointRadius,
            pt.x + m_nPointRadius, pt.y + m_nPointRadius);
    }

    pDC->SelectObject(pOldBrush);

    if (points.size() == 3) {
        CircleUtils::Circle circle = CircleUtils::Calculate(points[0], points[1], points[2]);
        if (!circle.bValid)
            return;

        CPen pen(PS_SOLID, m_nCircleThickness, RGB(255, 0, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush* pNullBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

        pDC->Ellipse((int)(circle.ptCenter.x - circle.fRadius),
            (int)(circle.ptCenter.y - circle.fRadius),
            (int)(circle.ptCenter.x + circle.fRadius),
            (int)(circle.ptCenter.y + circle.fRadius));

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pNullBrush);
    }
}
