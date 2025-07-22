#include "pch.h"
#include "PointManager.h"

// �� �� �߰�
void PointManager::AddPoint(const CPoint& pt)
{
    m_vecPoints.push_back(pt);  // ���� ���� �� ����
}

// ���� �� �̵�: �ε����� ��ȿ�� ���� ������Ʈ
void PointManager::MovePoint(int index, const CPoint& pt)
{
    if (index >= 0 && index < static_cast<int>(m_vecPoints.size()))
        m_vecPoints[index] = pt; // ������ ��ġ�� �� �̵�
}

// ���� �� ��� ��ȯ
const std::vector<CPoint>& PointManager::GetPoints() const
{
    return m_vecPoints;        // ���� ��ȯ���� ���� ����
}

// ��Ʈ �׽�Ʈ: Ŭ�� ��ġ�� �� �ݰ� ���� ������ �ش� �ε��� ��ȯ
int PointManager::HitTest(CPoint ptTest, int nRadius)
{
    for (int i = 0; i < static_cast<int>(m_vecPoints.size()); ++i)
    {
        if (abs(m_vecPoints[i].x - ptTest.x) <= nRadius &&
            abs(m_vecPoints[i].y - ptTest.y) <= nRadius)
            return i;             // �ݰ� �� ù ��° �� �ε���
    }
    return -1;                   // ��ġ�ϴ� �� ����
}

// ��� �� ����
void PointManager::Clear()
{
    m_vecPoints.clear();         // ���� ����
}
