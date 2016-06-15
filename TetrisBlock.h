#ifndef TETRISBLOCK_H
#define TETRISBLOCK_H
#include <windows.h>
#include "TetrisEnum.h"

class Tetris;
class TetrisBlock {
friend class Tetris;
private:
	static int cell[BLOCK_TYPE_COUNT][BLOCK_DIR_COUNT][cellsize][cellsize];
	int type;		//俄羅斯方塊的種類，用於cell的第一個index
	int direction;	//俄羅斯方塊的方向，用於cell的第二個index
	static HBRUSH* colorRush;	//宣告筆刷，搭配列舉好的顏色(白/紅/綠/藍)
	
	
public:
	TetrisBlock();
	void set(int _type, int _dir);
	int getDir();
	int getType();
	//設定 block 每個形狀每個方向的資料
	void CellInit();
	void rotate_clockwise ();
	void rotate_counterwise();
	void draw(HDC hdc, int ox, int oy);
};

#endif
