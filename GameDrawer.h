#ifndef __GAMEDRAWER_H__
#define __GAMEDRAWER_H__

#include <windows.h>
#include <string>

class Drawer
{
public:
	//rectangle background color
	enum class Color : int
	{
		WHITE = 0,
		RED = 1,
		GREEN = 2,
		BLUE = 3
	};
	static constexpr int eCOLORCOUNT = 4;
public:

	//constructure
	//param w: width of canvas
	//param h: height of canvas
	explicit Drawer(int w, int h);

	~Drawer();

	//Draw rectangle internally
	//param rect: information of position
	//param eColor: background color
	BOOL DrawRect(RECT rect, Color eColor);

	//Draw text internally
	//param text: text content
	//param pos: position of text will be drawn
	BOOL DrawText_(const std::string& text, POINT pos);

	//Copy draw result form internal HDC
	//param hdc: destination for draw result
	BOOL Update(HDC hdc);
private:

	//reset internal HDC draw result
	void ClearBackDC();

	//select draw object by input color
	//param eColor: selected color
	HBRUSH SelectBrush(Color eColor);
private:
	int m_iWidth, m_iHeight;
	HDC m_backHDC;
	HBITMAP m_backBMP;
	HBRUSH m_brush[eCOLORCOUNT];
	static int constexpr ePenBlack = 0;
	static int constexpr ePenWhite = 1;
	HPEN m_pen[2];
};

//An interface class
//Derived class store draw information at construction and do draw when Execute() called.
class DrawCommandBase
{
public:
	DrawCommandBase() = default;
	virtual ~DrawCommandBase() = default;
	virtual BOOL Execute(Drawer& drawer) = 0;
};

//A class for draw rectangle on Drawer class
class DrawRectCommand : public DrawCommandBase
{
public:
	explicit DrawRectCommand(int x, int y, int w, int h, Drawer::Color color);
	BOOL Execute(Drawer& drawer) override;
private:
	int m_iX, m_iY, m_iWidth, m_iHeight;
	Drawer::Color m_iColor;
};

//A class for draw text on Drawer class
class DrawTextCommand : public DrawCommandBase
{
public:
	explicit DrawTextCommand(int x, int y, const std::string& text);
	BOOL Execute(Drawer& drawer) override;
private:
	int m_iX, m_iY;
	std::string m_sText;
};

#endif
