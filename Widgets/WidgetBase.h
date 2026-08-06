#pragma once

#include <Windows.h>
#include "../Graphics/Renderer.h"

class WidgetBase {
public:
	WidgetBase(HDC hdc, int x, int y, int width, int height) : m_hdc(hdc), m_graphics(Renderer(m_hdc)) {

		m_bounds.left = x;
		m_bounds.top = y;
		m_bounds.right = x + width;
		m_bounds.bottom = y + height;
	};

	virtual ~WidgetBase() = default;

	virtual void update() = 0;

	virtual void draw() = 0;

	RECT getBounds() const;

protected:
	RECT m_bounds;
	HDC m_hdc;

	Renderer m_graphics;
};