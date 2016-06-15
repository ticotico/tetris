#include "game.h"

namespace {
	//宣告Tetris 物件
	Tetris player(VK_DOWN, VK_LEFT, VK_RIGHT, VK_SPACE);
	//宣告FPS 物件
	FPS fps;
	//宣告背景頁物件
	BackGroundPage bgp;
	//畫筆
	HBRUSH globalColorRush[] = {
		CreateSolidBrush(RGB(255,255,255)),
		CreateSolidBrush(RGB(255,0,0)),
		CreateSolidBrush(RGB(0,255,0)),
		CreateSolidBrush(RGB(0,0,255)) };
	//兩次 gamework的相隔時間 
	DWORD gameworkDur;
}



//以下是俄羅斯方塊靜態成員定義
//定義正在移動的方塊，用type和dir決定方塊 
int TetrisBlock::cell[BLOCK_TYPE_COUNT][BLOCK_DIR_COUNT][cellsize][cellsize];//定義俄羅斯方塊的基礎 cell
//定義筆刷，搭配列舉好的顏色(白/紅/綠/藍)
HBRUSH* TetrisBlock::colorRush = globalColorRush; 
HBRUSH* Tetris::colorRush = globalColorRush;

void gameInit()
{
	gameworkDur = 0;
	srand(GetTickCount());	//初始化亂數
	//準備背景頁
	bgp.init(1024, 768);
	//準備俄羅斯方塊初始值
	player.init(VK_DOWN, VK_LEFT, VK_RIGHT, VK_SPACE);
}

void gameWork()
{
	fps.counter();//計算fps
	gameworkDur += fps.getDur();//計算兩次gameWork過了幾秒
	if (gameworkDur >= player.flowtime()) {
		player.flow();
		gameworkDur -= player.flowtime();
		
	}
	player.move();
	player.checkWindowIfClearLine();
}

void gameDraw(HDC& hdc)
{
	//清空背景頁 
	bgp.clear();
	//畫出移動中的block
	player.draw(bgp.getbackDC());
	//畫出fps
	fps.draw(bgp.getbackDC(), 0, 0);
	
	bgp.draw(hdc);
}
