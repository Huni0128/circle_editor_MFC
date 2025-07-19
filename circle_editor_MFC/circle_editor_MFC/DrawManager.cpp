#include "pch.h"
#include "DrawManager.h"
#include <cmath>
#include <cstring>

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
        RGBQUAD rgb[256];
        for (int i = 0; i < 256; ++i) 
        {
            rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = (BYTE)i;
            rgb[i].rgbReserved = 0;
        }
        m_imgBuffer.SetColorTable(0, 256, rgb);
    }
}

void DrawManager::UpdateBuffer() noexcept 
{
    int nW = m_imgBuffer.GetWidth();
    int nH = m_imgBuffer.GetHeight();
    int nPitch = m_imgBuffer.GetPitch();
    BYTE* pBits = (BYTE*)m_imgBuffer.GetBits();
    BYTE rClear = (m_imgBuffer.GetBPP() == 8) ? 0xFF : 0x00;

    std::memset(pBits, rClear, abs(nPitch) * nH);

    const auto& vecPts = m_pPointMgr->GetPoints();
    for (const auto& pt : vecPts) 
    {
        DrawFilledCircle(pBits, nPitch, nW, nH, pt.x - m_nPtRadius, pt.y - m_nPtRadius, m_nPtRadius, 0);
    }

    if (vecPts.size() == 3) 
    {
        auto c = CircleUtils::Calculate(vecPts[0], vecPts[1], vecPts[2]);

        if (!c.bValid) 
            return;

        int nCX = (int)c.ptCenter.x - (int)c.fRadius;
        int nCY = (int)c.ptCenter.y - (int)c.fRadius;
        int nR = (int)c.fRadius;

        for (int i = 0; i < m_nThickness; ++i)
        {
            DrawFilledCircle(pBits, nPitch, nW, nH, nCX + i, nCY + i, nR - i, 0);
        }
    }
}

void DrawManager::DrawFilledCircle(
    BYTE* pBuf, int nPitch, int nW, int nH, int nX, int nY, int nR, BYTE rGray) noexcept 
{
    int nCX = nX + nR;
    int nCY = nY + nR;

    int y0 = max(nY, 0);
    int y1 = min(nY + 2 * nR - 1, nH - 1);

    for (int j = y0; j <= y1; ++j) 
    {
        int nDY = j - nCY;
        int nSq = nR * nR - nDY * nDY;

        if (nSq < 0) 
            continue;

        int nDX = (int)std::sqrt((double)nSq);
        int xStart = max(nCX - nDX, 0);
        int xEnd = min(nCX + nDX, nW - 1);

        BYTE* pRow = pBuf + j * nPitch;
        for (int i = xStart; i <= xEnd; ++i) 
        {
            pRow[i] = rGray;
        }
    }
}