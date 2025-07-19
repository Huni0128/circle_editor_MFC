#pragma once
#include <atltypes.h>

class CircleUtils {
public:
    struct Circle {
        CPoint ptCenter;
        float fRadius = 0.f;
        bool bValid = false;
    };

    static Circle Calculate(const CPoint& ptA, const CPoint& ptB, const CPoint& ptC);
};