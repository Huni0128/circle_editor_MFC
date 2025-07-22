#include "pch.h"
#include "PointManager.h"

// 새 점 추가
void PointManager::AddPoint(const CPoint& pt)
{
    m_vecPoints.push_back(pt);  // 벡터 끝에 점 삽입
}

// 기존 점 이동: 인덱스가 유효할 때만 업데이트
void PointManager::MovePoint(int index, const CPoint& pt)
{
    if (index >= 0 && index < static_cast<int>(m_vecPoints.size()))
        m_vecPoints[index] = pt; // 지정된 위치로 점 이동
}

// 현재 점 목록 반환
const std::vector<CPoint>& PointManager::GetPoints() const
{
    return m_vecPoints;        // 참조 반환으로 복사 방지
}

// 히트 테스트: 클릭 위치가 점 반경 내에 있으면 해당 인덱스 반환
int PointManager::HitTest(CPoint ptTest, int nRadius)
{
    for (int i = 0; i < static_cast<int>(m_vecPoints.size()); ++i)
    {
        if (abs(m_vecPoints[i].x - ptTest.x) <= nRadius &&
            abs(m_vecPoints[i].y - ptTest.y) <= nRadius)
            return i;             // 반경 내 첫 번째 점 인덱스
    }
    return -1;                   // 일치하는 점 없음
}

// 모든 점 제거
void PointManager::Clear()
{
    m_vecPoints.clear();         // 벡터 비우기
}
