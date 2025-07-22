#include "pch.h"
#include "CircleUtils.h"
#include <cmath>

// �� �� A, B, C�� �̿��� ������ ���
CircleUtils::Circle CircleUtils::Calculate(const CPoint& A, const CPoint& B, const CPoint& C)
{
    Circle result;

    // ��ǥ�� double�� ��ȯ
    double dAx = A.x, dAy = A.y;
    double dBx = B.x, dBy = B.y;
    double dCx = C.x, dCy = C.y;

    // D: ������ �߽� ������ �и� (0�̸� ������)
    double D = 2 * (dAx * (dBy - dCy)
        + dBx * (dCy - dAy)
        + dCx * (dAy - dBy));

    if (D == 0.0)
        return result; // ��ȿ���� ���� ��� ��ȯ

    // ������ �߽� ��ǥ (Ux, Uy) ���
    double dUx = ((dAx * dAx + dAy * dAy) * (dBy - dCy)
        + (dBx * dBx + dBy * dBy) * (dCy - dAy)
        + (dCx * dCx + dCy * dCy) * (dAy - dBy))
        / D;

    double dUy = ((dAx * dAx + dAy * dAy) * (dCx - dBx)
        + (dBx * dBx + dBy * dBy) * (dAx - dCx)
        + (dCx * dCx + dCy * dCy) * (dBx - dAx))
        / D;

    // ����� �߽ɰ� ������ ����
    result.ptCenter = CPoint((int)dUx, (int)dUy);
    result.fRadius = static_cast<float>(
        std::sqrt((dUx - dAx) * (dUx - dAx)
            + (dUy - dAy) * (dUy - dAy))
        );
    result.bValid = true; // ��ȿ �÷��� ����

    return result;
}
