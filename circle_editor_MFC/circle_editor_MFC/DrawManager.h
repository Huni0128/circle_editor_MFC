#pragma once
#include "PointManager.h"
#include "CircleUtils.h"
#include <atlimage.h>

class DrawManager {
private:
    PointManager* m_pointMgr;
    int           m_nPointRadius = 5;
    int           m_nCircleThickness = 2;
    CImage        m_image;

    static void drawCircleBuffer(BYTE* buffer, int pitch, int x, int y, int r, BYTE gray) noexcept {
        int cx = x + r;
        int cy = y + r;
        for (int j = y; j < y + 2 * r; ++j) {
            for (int i = x; i < x + 2 * r; ++i) {
                int dx = i - cx;
                int dy = j - cy;
                if (dx * dx + dy * dy < r * r) {
                    buffer[j * pitch + i] = gray;
                }
            }
        }
    }

public:
    explicit DrawManager(PointManager* mgr) noexcept;
    ~DrawManager() noexcept = default;

    void CreateImage(int width, int height, int bpp = 8) noexcept;

    CImage& GetImage() noexcept { return m_image; }

    int GetPointRadius() const noexcept { return m_nPointRadius; }
    int GetCircleThickness() const noexcept { return m_nCircleThickness; }
    void SetPointRadius(int r) noexcept { m_nPointRadius = r; }
    void SetCircleThickness(int t) noexcept { m_nCircleThickness = t; }

    void UpdateBuffer() noexcept;
};