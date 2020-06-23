#include "GameDrawer.h"

Drawer::Drawer(int w, int h)
	: m_iWidth(w), m_iHeight(h), m_backHDC(NULL)
	, m_backBMP(CreateBitmap(w, h, 1, 32, NULL))
	, m_brush{ CreateSolidBrush(RGB(255, 255, 255)),
				CreateSolidBrush(RGB(255, 0, 0)),
				CreateSolidBrush(RGB(0, 255, 0)),
				CreateSolidBrush(RGB(0, 0, 255)) }
	, m_pen{ CreatePen(PS_SOLID, 1, RGB(0, 0, 0)),
			 CreatePen(PS_SOLID, 1, RGB(255, 255, 255)) }
{
	HDC screenDC = GetDC(NULL);
	if (screenDC)
	{
		m_backHDC = CreateCompatibleDC(screenDC);
		ReleaseDC(NULL, screenDC);
	}
	if (m_backHDC && m_backBMP)
	{
		SelectObject(m_backHDC, m_backBMP);
		ClearBackDC();
	}
}

Drawer::~Drawer()
{
	for (auto iter : m_pen)
	{
		if (iter)
			DeleteObject(iter);
	}
	for (auto iter : m_brush)
	{
		if (iter)
			DeleteObject(iter);
	}

	if (m_backBMP)
		DeleteObject(m_backBMP);
	if (m_backHDC)
		DeleteDC(m_backHDC);
}

BOOL Drawer::DrawRect(RECT rect, Color eColor)
{
	BOOL ret = FALSE;
	HBRUSH brush = SelectBrush(eColor);
	HPEN pen = m_pen[ePenBlack];
	if (m_backHDC && brush && pen)
	{
		SelectObject(m_backHDC, brush);
		SelectObject(m_backHDC, pen);
		ret = Rectangle(m_backHDC, rect.left, rect.top, rect.right, rect.bottom);
	}
	return ret;
}

BOOL Drawer::DrawText_(const std::string& text, POINT pos)
{
	BOOL ret = FALSE;
	HPEN pen = m_pen[ePenBlack];
	if (m_backHDC && pen)
	{
		SelectObject(m_backHDC, pen);
		ret = TextOutA(m_backHDC, pos.x, pos.y, text.c_str(), static_cast<int>(text.length()));
	}
	return ret;
}

BOOL Drawer::Update(HDC hdc)
{
	BitBlt(hdc, 0, 0, m_iWidth, m_iHeight, m_backHDC, 0, 0, SRCCOPY);
	ClearBackDC();
	return TRUE;
}

void Drawer::ClearBackDC()
{
	HBRUSH whiteBrush = SelectBrush(Color::WHITE);
	HPEN pen = m_pen[ePenWhite];
	if (m_backHDC && whiteBrush && pen)
	{
		SelectObject(m_backHDC, whiteBrush);
		SelectObject(m_backHDC, pen);
		Rectangle(m_backHDC, 0, 0, m_iWidth, m_iHeight);
	}
}

HBRUSH Drawer::SelectBrush(Color eColor)
{
	HBRUSH ret = NULL;
	switch (eColor)
	{
	case Color::WHITE:
		ret = m_brush[0];
		break;
	case Color::RED:
		ret = m_brush[1];
		break;
	case Color::GREEN:
		ret = m_brush[2];
		break;
	case Color::BLUE:
		ret = m_brush[3];
		break;
	}
	return ret;
}

//=========================
DrawRectCommand::DrawRectCommand(int x, int y, int w, int h, Drawer::Color color)
	: DrawCommandBase(), m_iX(x), m_iY(y), m_iWidth(w), m_iHeight(h)
	, m_iColor(color)
{
}

BOOL DrawRectCommand::Execute(Drawer& drawer)
{
	RECT rect;
	rect.left = m_iX;
	rect.top = m_iY;
	rect.right = m_iWidth;
	rect.bottom = m_iHeight;
	return drawer.DrawRect(rect, m_iColor);
}

//=========================
DrawTextCommand::DrawTextCommand(int x, int y, const std::string& text)
	: DrawCommandBase(), m_iX(x), m_iY(y)
	, m_sText(text)
{
}

BOOL DrawTextCommand::Execute(Drawer& drawer)
{
	return drawer.DrawText_(m_sText, { m_iX, m_iY });
}