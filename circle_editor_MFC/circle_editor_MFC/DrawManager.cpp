#include "pch.h"
#include "DrawManager.h"
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

// �� v�� [lo, hi] ������ ����
static inline int Clamp(int v, int lo, int hi) noexcept
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// �ݰ� r���� y �Ÿ� dy�� �����ϴ� x �Ÿ� ���
static inline int ComputeDx(int r, int dy) noexcept
{
    int sq = r * r - dy * dy;
    return (sq < 0 ? -1 : static_cast<int>(sqrt((double)sq)));
}

// ������: ���� ��ü ���� (PointManager ���� ����)
DrawManager::DrawManager() noexcept
{
}

// �̹��� ���� ����: ũ��, ��Ʈ ���� ���� �� 8bpp �ȷ�Ʈ �ʱ�ȭ
void DrawManager::CreateImage(int nWidth, int nHeight, int nBpp) noexcept
{
    m_imgBuffer.Destroy();
    m_imgBuffer.Create(nWidth, -nHeight, nBpp);
    if (nBpp == 8)
    {
        // �׷��̽����� �ȷ�Ʈ
        RGBQUAD pal[256];
        for (int i = 0; i < 256; ++i)
        {
            pal[i].rgbRed = pal[i].rgbGreen = pal[i].rgbBlue = static_cast<BYTE>(i);
            pal[i].rgbReserved = 0;
        }
        m_imgBuffer.SetColorTable(0, 256, pal);
    }
}

// ���� ������Ʈ: ��� ����� �� �� ������ �׸���
void DrawManager::UpdateBuffer(const std::vector<CPoint>& points) noexcept
{
    ClearBuffer();  // ���� ���� �ʱ�ȭ

    int width = m_imgBuffer.GetWidth();
    int height = m_imgBuffer.GetHeight();
    int pitch = m_imgBuffer.GetPitch();
    BYTE* pBuf = static_cast<BYTE*>(m_imgBuffer.GetBits());

    DrawPoints(pBuf, pitch, width, height, points);

    // ���� 3���� ���� �� ��� �� �� �׸���
    if (points.size() == 3)
    {
        auto circ = CircleUtils::Calculate(points[0], points[1], points[2]);
        if (circ.bValid)
        {
            int r = static_cast<int>(circ.fRadius);
            int x0 = circ.ptCenter.x - r;
            int y0 = circ.ptCenter.y - r;
            DrawCircleRing(pBuf, pitch, width, height, x0, y0, r, m_nThickness, 128);
        }
    }
}

// ���� Ŭ����: ��Ʈ ���̿� ���� �⺻������ �޸� ����
void DrawManager::ClearBuffer() noexcept
{
    BYTE* pBits = static_cast<BYTE*>(m_imgBuffer.GetBits());
    int pitch = m_imgBuffer.GetPitch();
    int height = abs(m_imgBuffer.GetHeight());
    BYTE fill = (m_imgBuffer.GetBPP() == 8 ? 0xFF : 0x00);
    memset(pBits, fill, static_cast<size_t>(pitch) * height);
}

// �� �׸���: �� ���� ������ ���� ������ ä��
void DrawManager::DrawPoints(
    BYTE* pBuf, int pitch, int width, int height,
    const std::vector<CPoint>& points) noexcept
{
    for (const auto& pt : points)
    {
        int r = m_nPtRadius;
        int cx = pt.x, cy = pt.y;
        int y0 = Clamp(cy - r, 0, height - 1);
        int y1 = Clamp(cy + r, 0, height - 1);

        BYTE* row = pBuf + y0 * pitch;
        for (int y = y0; y <= y1; ++y, row += pitch)
        {
            int dx = ComputeDx(r, y - cy);
            if (dx < 0) continue;
            int xs = Clamp(cx - dx, 0, width - 1);
            int xe = Clamp(cx + dx, 0, width - 1);
            for (int x = xs; x <= xe; ++x)
                row[x] = 0;
        }
    }
}

// �� �׵θ� �׸���: �β�, ����/�ܺ� �ݰ濡 ���� �ȼ� ����
void DrawManager::DrawCircleRing(
    BYTE* pBuf, int pitch, int width, int height,
    int x0, int y0, int r, int thickness, BYTE gray) noexcept
{
    int halfIn = thickness / 2;
    int halfOut = thickness - halfIn;
    int outerR = r + halfOut;
    int innerR = max(0, r - halfIn);
    int cx = x0 + r, cy = y0 + r;

    int yStart = Clamp(y0 - halfOut, 0, height - 1);
    int yEnd = Clamp(y0 + 2 * outerR - 1, 0, height - 1);

    BYTE* row = pBuf + yStart * pitch;
    for (int y = yStart; y <= yEnd; ++y, row += pitch)
    {
        int dxOut = ComputeDx(outerR, y - cy);
        if (dxOut < 0) continue;
        int sOut = Clamp(cx - dxOut, 0, width - 1);
        int eOut = Clamp(cx + dxOut, 0, width - 1);

        int sIn = 0, eIn = -1;
        if (innerR > 0)
        {
            int dxIn = ComputeDx(innerR, y - cy);
            if (dxIn >= 0)
            {
                sIn = Clamp(cx - dxIn, 0, width - 1);
                eIn = Clamp(cx + dxIn, 0, width - 1);
            }
        }

        if (innerR == 0 || sIn > eOut || eIn < sOut)
        {
            for (int x = sOut; x <= eOut; ++x)
                row[x] = gray;
        }
        else
        {
            for (int x = sOut; x < sIn; ++x) row[x] = gray;
            for (int x = eIn; x <= eOut; ++x) row[x] = gray;
        }
    }
}
