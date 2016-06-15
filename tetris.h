#ifndef TETRIS_H
#define TETRIS_H
#include "TetrisEnum.h"
#include "TetrisBlock.h"
#include "keystate.h"
#include <cstdio>
#include <cstring>
#include <list>

class Tetris {
private:
	int posx, posy;	//方塊在遊戲視窗的座標
	KeyContinuous K_RIGHT, K_LEFT, K_DOWN, K_ROTATE;	//紀錄各個按鍵的狀態
	static HBRUSH* colorRush;	//宣告筆刷，搭配列舉好的顏色(白/紅/綠/藍)
	int window[window_W][window_H];		//宣告視窗陣列，用來存方方塊
	TetrisBlock curBlock;	//現在正在移動的方塊
	int score;				//目前的分數
	int combo;				//消除了幾行
	bool gameover;			//是否遊戲結束
	DWORD downspeed;		//俄羅斯方塊隨時間自動往下一格的速度 
	
	
	typedef std::list<TetrisBlock> V_block_info;
	class BlockHint : public V_block_info {
	public:
		void addLastInfo();
		void popFirstInfo(TetrisBlock& output);
	};
	BlockHint blockHint;

public:
	//建構式
	Tetris();
	//建構式
	Tetris(int down, int left, int right, int rotate);
	//初始化 
	void init(int down, int left, int right, int rotate);
	//block是否超出螢幕
	bool isOutWindow();
	//block是否碰到window已有方塊
	bool isTouchWindowCell();
	//將block 的每一個cell複製到window的絕對位置
	void fixToWindow();
	//產生新的block
	void generate();
	//順時針旋轉block
	void rotate();
	//block隨鍵盤的移動
	void move();
	//block 隨時間往下移動
	void flow();

	//畫出block
	void draw(HDC hdc);
	//window的一行是否全為空
	bool isLineEmpty(int nth);
	//window的一行是否滿了
	bool isLineFull(int nth);
	//將window上面的一行移動到下面的一行
	void moveWindowLineToLower(int nth);
	//檢查window是否其中一行滿了，若滿了就消除
	void checkWindowIfClearLine();
	//檢查是否gameover
	bool isGameover();
	//根據downspeed獲得往下一次(flow)所需的時間
	DWORD flowtime();
};

#endif
