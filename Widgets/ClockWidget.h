#pragma once

#include <Windows.h>

#include "WidgetBase.h"

class ClockWidget : public WidgetBase {
public:
	ClockWidget(HDC hdc, int x, int y, int width, int height) : WidgetBase(hdc, x, y, width, height) {};

	void draw() override{
		POINT start = { m_bounds.left, m_bounds.top };
		POINT end = { m_bounds.right, m_bounds.bottom };

		m_graphics.drawLine(start, end, RGB(255, 0, 0), 1);
	};

	void update() override {
		// Update logic for the clock widget (e.g., updating the time)
	};
};