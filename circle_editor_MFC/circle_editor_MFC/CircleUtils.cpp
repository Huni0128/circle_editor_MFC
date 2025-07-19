#include "pch.h"
#include "CircleUtils.h"
#include <cmath>

CircleUtils::Circle CircleUtils::Calculate(const CPoint& A, const CPoint& B, const CPoint& C)
{
    Circle result;

    double dAx = A.x, dAy = A.y;
    double dBx = B.x, dBy = B.y;
    double dCx = C.x, dCy = C.y;

    double D = 2 * (dAx * (dBy - dCy) + dBx * (dCy - dAy) + dCx * (dAy - dBy));
    if (D == 0.0)
        return result;

    double dUx = ((dAx * dAx + dAy * dAy) * (dBy - dCy) +
        (dBx * dBx + dBy * dBy) * (dCy - dAy) +
        (dCx * dCx + dCy * dCy) * (dAy - dBy)) / D;

    double dUy = ((dAx * dAx + dAy * dAy) * (dCx - dBx) +
        (dBx * dBx + dBy * dBy) * (dAx - dCx) +
        (dCx * dCx + dCy * dCy) * (dBx - dAx)) / D;

    result.ptCenter = CPoint((int)dUx, (int)dUy);
    result.fRadius = (float)sqrt((dUx - dAx) * (dUx - dAx) + (dUy - dAy) * (dUy - dAy));
    result.bValid = true;

    return result;
}
