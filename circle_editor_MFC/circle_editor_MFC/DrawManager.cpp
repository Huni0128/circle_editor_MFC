#include "pch.h"
#include "DrawManager.h"
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

static inline int Clamp(int v, int lo, int hi) noexcept
{
    return v < lo ? lo : v > hi ? hi : v;
}
static inline int ComputeDx(int r, int dy) noexcept
{
    int sq = r * r - dy * dy;
    return sq < 0 ? -1 : static_cast<int>(sqrt((double)sq));
}

DrawManager::DrawManager(PointManager* pMgr) noexcept
    : m_pPointMgr(pMgr)
{
}

void DrawManager::CreateImage(int nWidth, int nHeight, int nBpp) noexcept
{
    m_imgBuffer.Destroy();
    m_imgBuffer.Create(nWidth, -nHeight, nBpp);
    if (nBpp == 8)
    {
        RGBQUAD pal[256];
        for (int nI = 0; nI < 256; ++nI)
        {
            pal[nI].rgbRed =
                pal[nI].rgbGreen =
                pal[nI].rgbBlue = static_cast<BYTE>(nI);
            pal[nI].rgbReserved = 0;
        }
        m_imgBuffer.SetColorTable(0, 256, pal);
    }
}

void DrawManager::UpdateBuffer() noexcept
{
    ClearBuffer();

    int nW = m_imgBuffer.GetWidth();
    int nH = m_imgBuffer.GetHeight();
    int nPitch = m_imgBuffer.GetPitch();
    BYTE* pBuf = static_cast<BYTE*>(m_imgBuffer.GetBits());

    DrawPoints(pBuf, nPitch, nW, nH);

    const auto& pts = m_pPointMgr->GetPoints();
    if (pts.size() != 3)
        return;

    auto circ = CircleUtils::Calculate(pts[0], pts[1], pts[2]);
    if (!circ.bValid)
        return;

    int nR = static_cast<int>(circ.fRadius);
    int nX0 = static_cast<int>(circ.ptCenter.x) - nR;
    int nY0 = static_cast<int>(circ.ptCenter.y) - nR;

    DrawCircleRing(
        pBuf, nPitch, nW, nH, nX0, nY0, nR, m_nThickness,static_cast<BYTE>(128));
}

void DrawManager::ClearBuffer() noexcept
{
    BYTE* pBits = static_cast<BYTE*>(m_imgBuffer.GetBits());
    int nPitch = m_imgBuffer.GetPitch();
    int nH = m_imgBuffer.GetHeight();
    BYTE byClear = (m_imgBuffer.GetBPP() == 8 ? 0xFF : 0x00);
    memset(pBits, byClear, static_cast<size_t>(abs(nPitch)) * nH);
}

void DrawManager::DrawPoints(BYTE* pBuf, int nPitch, int nW, int nH) noexcept
{
    const auto& pts = m_pPointMgr->GetPoints();
    for (const auto& pt : pts)
    {
        int nR = m_nPtRadius;
        int nCX = pt.x;
        int nCY = pt.y;
        int nY0 = Clamp(nCY - nR, 0, nH - 1);
        int nY1 = Clamp(nCY + nR, 0, nH - 1);

        BYTE* pRow = pBuf + nY0 * nPitch;
        for (int nRow = nY0; nRow <= nY1; ++nRow, pRow += nPitch)
        {
            int nDX = ComputeDx(nR, nRow - nCY);
            if (nDX < 0) continue;

            int nXS = Clamp(nCX - nDX, 0, nW - 1);
            int nXE = Clamp(nCX + nDX, 0, nW - 1);
            for (int nCol = nXS; nCol <= nXE; ++nCol)
                pRow[nCol] = 0;
        }
    }
}

void DrawManager::DrawCircleRing(
    BYTE* pBuf, int nPitch, int nW, int nH,
    int nX0, int nY0, int nR, int nThickness, BYTE rGray) noexcept
{
    int nHalfIn = nThickness / 2;
    int nHalfOut = nThickness - nHalfIn;
    int nOuterR = nR + nHalfOut;
    int nInnerR = max(0, nR - nHalfIn);
    int nCX = nX0 + nR;
    int nCY = nY0 + nR;
    int nYStart = Clamp(nY0 - nHalfOut, 0, nH - 1);
    int nYEnd = Clamp(nY0 + 2 * nOuterR - 1, 0, nH - 1);

    BYTE* pRow = pBuf + nYStart * nPitch;
    for (int nRow = nYStart; nRow <= nYEnd; ++nRow, pRow += nPitch)
    {
        int nDXOut = ComputeDx(nOuterR, nRow - nCY);
        if (nDXOut < 0) continue;

        int nSOut = Clamp(nCX - nDXOut, 0, nW - 1);
        int nEOut = Clamp(nCX + nDXOut, 0, nW - 1);

        int nSIn = 0, nEIn = -1;
        if (nInnerR > 0)
        {
            int nDXIn = ComputeDx(nInnerR, nRow - nCY);
            if (nDXIn >= 0)
            {
                nSIn = Clamp(nCX - nDXIn, 0, nW - 1);
                nEIn = Clamp(nCX + nDXIn, 0, nW - 1);
            }
        }

        if (nInnerR == 0 || nSIn > nEOut || nEIn < nSOut)
        {
            for (int nCol = nSOut; nCol <= nEOut; ++nCol)
                pRow[nCol] = rGray;
        }
        else
        {
            for (int nCol = nSOut; nCol < nSIn; ++nCol)
                pRow[nCol] = rGray;
            for (int nCol = nEIn; nCol <= nEOut; ++nCol)
                pRow[nCol] = rGray;
        }
    }
}
