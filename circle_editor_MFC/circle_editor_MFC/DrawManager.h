#pragma once
#include <vector>
#include <atlimage.h>
#include <atltypes.h>
#include "CircleUtils.h"

class DrawManager
{
public:
    DrawManager() noexcept;
    ~DrawManager() noexcept = default;

    void CreateImage(int nWidth, int nHeight, int nBpp = 8) noexcept;
    void UpdateBuffer(const std::vector<CPoint>& points) noexcept;

    CImage& GetImage() noexcept { return m_imgBuffer; }

    int GetPointRadius() const noexcept { return m_nPtRadius; }
    void SetPointRadius(int nRadius) noexcept { m_nPtRadius = nRadius; }

    int GetCircleThickness() const noexcept { return m_nThickness; }
    void SetCircleThickness(int nThick) noexcept { m_nThickness = nThick; }

private:
    void ClearBuffer() noexcept;
    void DrawPoints(BYTE* pBuf, int pitch, int width, int height, const std::vector<CPoint>& points) noexcept;
    void DrawCircleRing(
        BYTE* pBuf, int pitch, int width, int height,
        int x0, int y0, int r, int thickness, BYTE gray) noexcept;

private:
    int m_nPtRadius = 5;
    int m_nThickness = 2;
    CImage m_imgBuffer;
};
