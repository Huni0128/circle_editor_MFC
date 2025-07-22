#include "pch.h"
#include "DrawManager.h"
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;

// 값 v를 [lo, hi] 범위로 제한
static inline int Clamp(int v, int lo, int hi) noexcept
{
    return v < lo ? lo : (v > hi ? hi : v);
}

// 반경 r에서 y 거리 dy에 대응하는 x 거리 계산
static inline int ComputeDx(int r, int dy) noexcept
{
    int sq = r * r - dy * dy;
    return (sq < 0 ? -1 : static_cast<int>(sqrt((double)sq)));
}

// 생성자: 의존 객체 제거 (PointManager 참조 안함)
DrawManager::DrawManager() noexcept
{
}

// 이미지 버퍼 생성: 크기, 비트 깊이 설정 및 8bpp 팔레트 초기화
void DrawManager::CreateImage(int nWidth, int nHeight, int nBpp) noexcept
{
    m_imgBuffer.Destroy();
    m_imgBuffer.Create(nWidth, -nHeight, nBpp);
    if (nBpp == 8)
    {
        // 그레이스케일 팔레트
        RGBQUAD pal[256];
        for (int i = 0; i < 256; ++i)
        {
            pal[i].rgbRed = pal[i].rgbGreen = pal[i].rgbBlue = static_cast<BYTE>(i);
            pal[i].rgbReserved = 0;
        }
        m_imgBuffer.SetColorTable(0, 256, pal);
    }
}

// 버퍼 업데이트: 배경 지우고 점 및 외접원 그리기
void DrawManager::UpdateBuffer(const std::vector<CPoint>& points) noexcept
{
    ClearBuffer();  // 기존 내용 초기화

    int width = m_imgBuffer.GetWidth();
    int height = m_imgBuffer.GetHeight();
    int pitch = m_imgBuffer.GetPitch();
    BYTE* pBuf = static_cast<BYTE*>(m_imgBuffer.GetBits());

    DrawPoints(pBuf, pitch, width, height, points);

    // 점이 3개일 때만 원 계산 및 링 그리기
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

// 버퍼 클리어: 비트 깊이에 따른 기본값으로 메모리 세팅
void DrawManager::ClearBuffer() noexcept
{
    BYTE* pBits = static_cast<BYTE*>(m_imgBuffer.GetBits());
    int pitch = m_imgBuffer.GetPitch();
    int height = abs(m_imgBuffer.GetHeight());
    BYTE fill = (m_imgBuffer.GetBPP() == 8 ? 0xFF : 0x00);
    memset(pBits, fill, static_cast<size_t>(pitch) * height);
}

// 점 그리기: 각 점을 반지름 범위 내에서 채움
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

// 원 테두리 그리기: 두께, 내부/외부 반경에 따라 픽셀 설정
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
