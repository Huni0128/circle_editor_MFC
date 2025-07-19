#pragma once
#include <vector>
#include <atltypes.h>

class PointManager 
{
private:
    std::vector<CPoint> m_vecPoints;

public:
    void AddPoint(const CPoint& pt);
    void MovePoint(int index, const CPoint& pt);
    const std::vector<CPoint>& GetPoints() const;
    int HitTest(CPoint ptTest, int nRadius = 10);
    void Clear();

    bool IsFull(int maxCount = 3) const 
    {
        return m_vecPoints.size() >= static_cast<size_t>(maxCount);
    }
};
