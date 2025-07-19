#include "pch.h"
#include "DrawManager.h"
#include <cstring>

DrawManager::DrawManager(PointManager* mgr) noexcept
    : m_pointMgr(mgr)
{
}

void DrawManager::CreateImage(int width, int height, int bpp) noexcept {
    m_image.Destroy();
    m_image.Create(width, -height, bpp);
    if (bpp == 8) {
        RGBQUAD pal[256];
        for (int i = 0; i < 256; ++i) {
            pal[i].rgbRed = pal[i].rgbGreen = pal[i].rgbBlue = static_cast<BYTE>(i);
            pal[i].rgbReserved = 0;
        }
        m_image.SetColorTable(0, 256, pal);
    }
}

void DrawManager::UpdateBuffer() noexcept {
    int width = m_image.GetWidth();
    int height = m_image.GetHeight();
    int pitch = m_image.GetPitch();
    BYTE* bits = static_cast<BYTE*>(m_image.GetBits());

    BYTE clearVal = (m_image.GetBPP() == 8 ? 0xFF : 0x00);
    std::memset(bits, clearVal, abs(pitch) * height);

    const auto& pts = m_pointMgr->GetPoints();
    for (const auto& pt : pts) {
        drawCircleBuffer(bits, pitch,
            pt.x - m_nPointRadius,
            pt.y - m_nPointRadius,
            m_nPointRadius,
            static_cast<BYTE>(0));
    }

    if (pts.size() == 3) {
        auto cir = CircleUtils::Calculate(pts[0], pts[1], pts[2]);
        if (!cir.bValid) return;
        int cx = static_cast<int>(cir.ptCenter.x) - static_cast<int>(cir.fRadius);
        int cy = static_cast<int>(cir.ptCenter.y) - static_cast<int>(cir.fRadius);
        int r = static_cast<int>(cir.fRadius);
        BYTE gray = 0;
        for (int t = 0; t < m_nCircleThickness; ++t) {
            drawCircleBuffer(bits, pitch,
                cx + t,
                cy + t,
                r - t,
                gray);
        }
    }
}