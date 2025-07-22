#include "pch.h"
#include "CircleUtils.h"
#include <cmath>

// 세 점 A, B, C를 이용한 외접원 계산
CircleUtils::Circle CircleUtils::Calculate(const CPoint& A, const CPoint& B, const CPoint& C)
{
    Circle result;

    // 좌표를 double로 변환
    double dAx = A.x, dAy = A.y;
    double dBx = B.x, dBy = B.y;
    double dCx = C.x, dCy = C.y;

    // D: 외접원 중심 계산식의 분모 (0이면 일직선)
    double D = 2 * (dAx * (dBy - dCy)
        + dBx * (dCy - dAy)
        + dCx * (dAy - dBy));

    if (D == 0.0)
        return result; // 유효하지 않은 결과 반환

    // 외접원 중심 좌표 (Ux, Uy) 계산
    double dUx = ((dAx * dAx + dAy * dAy) * (dBy - dCy)
        + (dBx * dBx + dBy * dBy) * (dCy - dAy)
        + (dCx * dCx + dCy * dCy) * (dAy - dBy))
        / D;

    double dUy = ((dAx * dAx + dAy * dAy) * (dCx - dBx)
        + (dBx * dBx + dBy * dBy) * (dAx - dCx)
        + (dCx * dCx + dCy * dCy) * (dBx - dAx))
        / D;

    // 결과에 중심과 반지름 설정
    result.ptCenter = CPoint((int)dUx, (int)dUy);
    result.fRadius = static_cast<float>(
        std::sqrt((dUx - dAx) * (dUx - dAx)
            + (dUy - dAy) * (dUy - dAy))
        );
    result.bValid = true; // 유효 플래그 설정

    return result;
}
