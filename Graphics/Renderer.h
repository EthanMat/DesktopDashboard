#pragma once

#include <Windows.h>
#include <string>

class Renderer {
	public:
		Renderer(HDC hdc) {
			m_hdc = hdc;
		};

		void drawRect(RECT rect, HBRUSH brush) {
			
		};
		void drawText(RECT rect, const std::string & text, COLORREF color);
		void drawLine(POINT start, POINT end, COLORREF color, int thickness) {

			HPEN hNewPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			HBRUSH hNewBrush = CreateSolidBrush(RGB(0, 0, 255)); // Blue Fill

			// 3. Select tools into DC and save old default system stock tools
			HPEN hOldPen = (HPEN)SelectObject(m_hdc, hNewPen);
			HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hdc, hNewBrush);

			MoveToEx(m_hdc, start.x, start.y, NULL);
			LineTo(m_hdc, end.x, end.y);

			SelectObject(m_hdc, hOldBrush);
			SelectObject(m_hdc, hOldPen);

			DeleteObject(hNewPen);
			DeleteObject(hNewBrush);
		};
		void drawCircle(POINT center, int radius, COLORREF color, int thickness);

	private:
		HDC m_hdc;
};