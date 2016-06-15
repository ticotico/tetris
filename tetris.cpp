#include "Tetris.h"
#include <cassert>
#include <cmath> 

void Tetris::BlockHint::addLastInfo() {
	int tmp_type = rand() % BLOCK_TYPE_COUNT;
	int tmp_dir = rand() % BLOCK_DIR_COUNT;
	TetrisBlock tmp;
	tmp.set(tmp_type, tmp_dir);
	V_block_info::push_back(tmp);
}

void Tetris::BlockHint::popFirstInfo(TetrisBlock& output) {
	output = this->front();
	this->pop_front();
}

Tetris::Tetris() :
		posx(0), posy(0), score(0), combo(0),
		gameover(false), downspeed(0UL) {
	K_DOWN.SetKey(VK_DOWN);
	K_LEFT.SetKey(VK_LEFT);
	K_RIGHT.SetKey(VK_RIGHT);
	K_ROTATE.SetKey(VK_SPACE);
	for (int i = 0; i < window_W; i++)
	for (int j = 0; j < window_H; j++) {
		window[i][j] = COLOR_NULL;
	}
}

Tetris::Tetris(int down, int left, int right, int rotate)  : 
		posx(0), posy(0), score(0), combo(0),
		gameover(false), downspeed(0UL) {
	downspeed = 0UL;
	K_DOWN.SetKey(down);
	K_LEFT.SetKey(left);
	K_RIGHT.SetKey(right);
	K_ROTATE.SetKey(rotate);
	for (int i = 0; i < window_W; i++)
	for (int j = 0; j < window_H; j++) {
		window[i][j] = COLOR_NULL;
	}
}

void Tetris::init(int down, int left, int right, int rotate) {
	posx = 0;
	posy = 0;
	int tmp_type = rand() % BLOCK_TYPE_COUNT;
	int tmp_dir = rand() % BLOCK_DIR_COUNT;
	curBlock.set(tmp_type, tmp_dir);
	score = 0;
	combo = 0;
	gameover = false;
	downspeed = 0UL;
	K_DOWN.SetKey(down);
	K_LEFT.SetKey(left);
	K_RIGHT.SetKey(right);
	K_ROTATE.SetKey(rotate);
	//將window每個格子洗白
	for (int i = 0; i < window_W; i++)
	for (int j = 0; j < window_H; j++) {
		window[i][j] = COLOR_NULL;
	}
	curBlock.CellInit();
	//產生三個即將出現的方塊
	for (int i = 0; i < blcok_hint; i++) {
		blockHint.addLastInfo();
	}
}

bool Tetris::isOutWindow() {
	int cur_posx, cur_posy;
	for (int i = 0; i < cellsize; i++)
		for (int j = 0; j < cellsize; j++)
			if (curBlock.cell[curBlock.type][curBlock.direction][i][j] != COLOR_NULL)
	 {
		cur_posx = posx + i;
		cur_posy = posy + j;
		if (cur_posx >= window_W ||
			cur_posx < 0 ||
			cur_posy >= window_H)
			return TRUE;
	}
	return FALSE;
}

bool Tetris::isTouchWindowCell() {
	int cur_posx, cur_posy;
	for (int i = 0; i < cellsize; i++)
		for (int j = 0; j < cellsize; j++)
			if (curBlock.cell[curBlock.type][curBlock.direction][i][j] != COLOR_NULL)
	{
		cur_posx = posx + i;
		cur_posy = posy + j;
		if (window[cur_posx][cur_posy] != COLOR_NULL)
			return TRUE;
	}
	return FALSE;
}

void Tetris::fixToWindow() {
	if (gameover) return; //gameover就不執行 
	int cur_posx, cur_posy;
	for (int i = 0; i < cellsize; i++)
		for (int j = 0; j < cellsize; j++)
			if (curBlock.cell[curBlock.type][curBlock.direction][i][j] != COLOR_NULL)
			{
				cur_posx = posx + i;
				cur_posy = posy + j;
				window[cur_posx][cur_posy] = COLOR_RED;
			}
	return;
}
//產生新的block
void Tetris::generate() {
	posx = 0;
	posy = 0;
	blockHint.popFirstInfo(curBlock);
	blockHint.addLastInfo();
	return;
}
//順時針旋轉block
void Tetris::rotate() {
	curBlock.rotate_clockwise();
	if (isOutWindow() || isTouchWindowCell())
		curBlock.rotate_counterwise();
}
//block隨鍵盤的移動
void Tetris::move() {
	DWORD continous_t = KEY_RESPONCE_TIME;
	
	K_RIGHT.Test();
	if (K_RIGHT.isContinuous(continous_t) ||
		K_RIGHT.IsPressDownMoment())
		{
			posx++;
			if (isOutWindow() || isTouchWindowCell())
				posx--;
		}
	
	K_LEFT.Test();
	if (K_LEFT.isContinuous(continous_t) ||
		K_LEFT.IsPressDownMoment()) 
		{
			posx--;
			if (isOutWindow() || isTouchWindowCell())
				posx++;
		}

	K_DOWN.Test();
	if (K_DOWN.isContinuous(continous_t) ||
		K_DOWN.IsPressDownMoment())
		{
			posy++;
			if (isOutWindow() || isTouchWindowCell())
				posy--;
		}

	K_ROTATE.Test();
	if (K_ROTATE.IsPressDownMoment())
		{
			rotate();
		}
	return;
}
//block 隨時間往下移動
void Tetris::flow() {
	if (isGameover()) return; //gameover就不執行  
	posy++;
	if (isOutWindow() || isTouchWindowCell()) {
		posy--;
		fixToWindow();
		generate();
	}
	return;
}

//畫出block
void Tetris::draw(HDC hdc) {
	//選筆刷
	SelectObject(hdc, colorRush[COLOR_NULL]);
	//畫出遊戲視窗
	Rectangle(hdc, 0, 0, cell_Side_Len*window_W, cell_Side_Len*window_H);
	//畫出視窗每一個cell的方塊顏色
	for (int i = 0; i < window_W; i++)
	for (int j = 0; j < window_H; j++) {
		if (window[i][j] != COLOR_NULL) {
			SelectObject(hdc, colorRush[window[i][j]]);
			Rectangle(hdc, i*cell_Side_Len, j*cell_Side_Len,
				(i + 1)*cell_Side_Len, (j + 1)*cell_Side_Len);
		}
	}
	//畫出目前運動中cell
	curBlock.draw(hdc, posx, posy);

	//畫出即將出現的cell
	BlockHint::reverse_iterator cur_hint = blockHint.rbegin();
	int hint_count = 0;
	while (cur_hint != blockHint.rend()) {
		int cur_ori_x = 1 + window_W;
		int cur_ori_y = hint_count * (cellsize + 1);
		cur_hint->draw(hdc, cur_ori_x, cur_ori_y);

		++hint_count;
		++cur_hint;
	}
	//畫出分數 
	char buf[32];
		sprintf(buf, "score: %d", score);
		TextOut(hdc, 2, cell_Side_Len * window_H, buf, strlen(buf));
	//畫出gameover 
	if (gameover) {
		strcpy(buf, "gameover");
		TextOut(hdc, cell_Side_Len*(window_W/2-2), cell_Side_Len*window_H/2,
			buf, strlen(buf));
	}
	//顯示speed 
	//sprintf(buf, "%d", downspeed);
	//TextOut(hdc, 0, 0, buf, strlen(buf));
}
//window的一行是否滿了
bool Tetris::isLineFull(int nth) {
	for (int i = 0; i < window_W; i++)
		if (window[i][nth] == COLOR_NULL)
			return FALSE;
	return TRUE;
}
//window的一行是否全為空
bool Tetris::isLineEmpty(int nth) {
	for (int i = 0; i < window_W; i++)
		if (window[i][nth] != COLOR_NULL)
			return FALSE;
	return TRUE;
}
//將window上面的一行移動到下面的一行
void Tetris::moveWindowLineToLower(int nth) {
	for (int i = 0; i < window_W; i++) {
		window[i][nth] = window[i][nth - 1];
		window[i][nth - 1] = COLOR_NULL;
	}
	return;
}
//檢查window是否其中一行滿了，若滿了就消除
void Tetris::checkWindowIfClearLine() {
	int combo_org = combo; //記下目前的combo 
	for (int i = window_H - 1; i >= 0; i--) {
		if (isLineEmpty(i)) break; //若其中一行全空，則上面的不用檢查
		if (isLineFull(i)) { //找到滿的一行
			combo++;
			for (int j = i; j > 0; j--) {
				if (isLineEmpty(j - 1)) break; //上面一行空的，就不需要再做搬移
				moveWindowLineToLower(j);
			}
			i++;//因為往下搬了一行，所以for迴圈判斷第幾行的引數要補回去 
		}
	}
	
	if (combo > combo_org) {
		//計算分數
		int combo_inc = combo - combo_org;
		score += 100 * combo_inc * combo_inc + 30 * downspeed * combo_inc;
		downspeed = sqrt(combo);
		if (downspeed > max_downspeed) downspeed = max_downspeed;
	}
	return;
}

//檢查是否gameover
bool Tetris::isGameover() {	
	for (int i = 0; i < cellsize; i++)
	for (int j = 0; j < cellsize; j++)
	if (curBlock.cell[curBlock.type][curBlock.direction][i][j] != COLOR_NULL)
	if (window[posx + i][posy + j] != COLOR_NULL) {
		gameover = true;
		return gameover;
	}
	return gameover;
}
//根據downspeed獲得往下一次(flow)所需的時間
DWORD Tetris::flowtime() {
	assert(downspeed >= 0 && downspeed <= max_downspeed);
	return 150 + 10 * (max_downspeed - downspeed);
}
