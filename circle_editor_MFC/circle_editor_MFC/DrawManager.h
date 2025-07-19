#pragma once
#include "PointManager.h"
#include "CircleUtils.h"
#include <atlimage.h>

class DrawManager
{
public:
    explicit DrawManager(PointManager* pMgr) noexcept;
    ~DrawManager() noexcept = default;

    void CreateImage(int nWidth, int nHeight, int nBpp = 8) noexcept;
    void UpdateBuffer() noexcept;

    CImage& GetImage() noexcept { return m_imgBuffer; }

    int GetPointRadius() const noexcept { return m_nPtRadius; }
    void SetPointRadius(int nRadius) noexcept { m_nPtRadius = nRadius; }

    int GetCircleThickness() const noexcept { return m_nThickness; }
    void SetCircleThickness(int nThick) noexcept { m_nThickness = nThick; }

private:
    void DrawFilledCircle(
        BYTE* pBuf, int nPitch, int nW, int nH, int nX, int nY, int nR, BYTE rGray) noexcept;

private:
    PointManager* m_pPointMgr = nullptr;
    int m_nPtRadius = 5;
    int m_nThickness = 2;
    CImage m_imgBuffer;
};