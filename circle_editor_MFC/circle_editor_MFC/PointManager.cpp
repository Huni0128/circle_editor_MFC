#include "pch.h"
#include "PointManager.h"

void PointManager::AddPoint(const CPoint& pt)
{
    m_vecPoints.push_back(pt);
}

void PointManager::MovePoint(int index, const CPoint& pt)
{
    if (index >= 0 && index < m_vecPoints.size())
        m_vecPoints[index] = pt;
}

const std::vector<CPoint>& PointManager::GetPoints() const
{
    return m_vecPoints;
}

int PointManager::HitTest(CPoint ptTest, int nRadius)
{
    for (int i = 0; i < m_vecPoints.size(); ++i) 
    {
        if (abs(m_vecPoints[i].x - ptTest.x) <= nRadius &&
            abs(m_vecPoints[i].y - ptTest.y) <= nRadius)
            return i;
    }
    return -1;
}

void PointManager::Clear()
{
    m_vecPoints.clear();
}
