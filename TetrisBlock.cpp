#include "TetrisBlock.h"

TetrisBlock::TetrisBlock() {
	type = 0;
	direction = 0;
}
void TetrisBlock::set(int _type, int _dir) {
	type = _type;
	direction = _dir;
}
int TetrisBlock::getDir() {
	return direction;
}
int TetrisBlock::getType() {
	return type;
}
//設定 block 每個形狀每個方向的資料
void TetrisBlock::CellInit()
{
	//先全部設為COLOR_NULL
	for (int a = 0; a < BLOCK_TYPE_COUNT; a++)
		for (int b = 0; b < BLOCK_DIR_COUNT; b++)
			for (int c = 0; c < cellsize; c++)
				for (int d = 0; d < cellsize; d++)
					cell[a][b][c][d] = COLOR_NULL;
	//設定 SQUARE_SHAPE
	//因為四個方向都一樣，就直接迴圈寫
	for (int b = 0; b < BLOCK_DIR_COUNT; b++)
		for (int c = 0; c < 2; c++)
			for (int d = 0; d < 2; d++)
				cell[SQUARE_SHAPE][b][c][d] = BLOCK_COLOR;
	//設定 LINE_SHAPE
	//設定 DIR_UP
	for (int c = 0; c < cellsize; c++)
		cell[LINE_SHAPE][DIR_UP][c][0] = BLOCK_COLOR;
	//設定 DIR_RIGHT
	for (int d = 0; d < cellsize; d++)
		cell[LINE_SHAPE][DIR_RIGHT][0][d] = BLOCK_COLOR;
	//設定 DIR_DOWN
	for (int c = 0; c < cellsize; c++)
		cell[LINE_SHAPE][DIR_DOWN][c][0] = BLOCK_COLOR;
	//設定 DIR_LEFT
	for (int d = 0; d < cellsize; d++)
		cell[LINE_SHAPE][DIR_LEFT][0][d] = BLOCK_COLOR;
	//設定 L_SHAPE
	//設定 DIR_UP
	cell[L_SHAPE][DIR_UP][1][0] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_UP][1][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_UP][1][2] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_UP][2][2] = BLOCK_COLOR;
	//設定 DIR_RIGHT
	cell[L_SHAPE][DIR_RIGHT][0][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_RIGHT][1][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_RIGHT][2][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_RIGHT][0][2] = BLOCK_COLOR;
	//設定 DIR_DOWN
	cell[L_SHAPE][DIR_DOWN][0][0] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_DOWN][1][0] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_DOWN][1][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_DOWN][1][2] = BLOCK_COLOR;
	//設定 DIR_LEFT
	cell[L_SHAPE][DIR_LEFT][2][0] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_LEFT][0][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_LEFT][1][1] = BLOCK_COLOR;
	cell[L_SHAPE][DIR_LEFT][2][1] = BLOCK_COLOR;
	//設定 T_SHAPE
	//設定 DIR_UP
	cell[T_SHAPE][DIR_UP][0][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_UP][1][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_UP][2][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_UP][1][2] = BLOCK_COLOR;
	//設定 DIR_RIGHT
	cell[T_SHAPE][DIR_RIGHT][1][0] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_RIGHT][0][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_RIGHT][1][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_RIGHT][1][2] = BLOCK_COLOR;
	//設定 DIR_DOWN
	cell[T_SHAPE][DIR_DOWN][1][0] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_DOWN][0][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_DOWN][1][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_DOWN][2][1] = BLOCK_COLOR;
	//設定 DIR_LEFT
	cell[T_SHAPE][DIR_LEFT][1][0] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_LEFT][1][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_LEFT][2][1] = BLOCK_COLOR;
	cell[T_SHAPE][DIR_LEFT][1][2] = BLOCK_COLOR;
	//設定 N_SHAPE
	//設定 DIR_UP
	cell[N_SHAPE][DIR_UP][0][0] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_UP][0][1] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_UP][1][1] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_UP][1][2] = BLOCK_COLOR;
	//設定 DIR_RIGHT
	cell[N_SHAPE][DIR_RIGHT][1][0] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_RIGHT][2][0] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_RIGHT][0][1] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_RIGHT][1][1] = BLOCK_COLOR;
	//設定 DIR_DOWN
	cell[N_SHAPE][DIR_DOWN][0][0] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_DOWN][0][1] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_DOWN][1][1] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_DOWN][1][2] = BLOCK_COLOR;
	//設定 DIR_LEFT
	cell[N_SHAPE][DIR_LEFT][1][0] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_LEFT][2][0] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_LEFT][0][1] = BLOCK_COLOR;
	cell[N_SHAPE][DIR_LEFT][1][1] = BLOCK_COLOR;
	//設定 L2_SHAPE
	//設定 DIR_UP
	cell[L2_SHAPE][DIR_UP][1][0] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_UP][1][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_UP][0][2] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_UP][1][2] = BLOCK_COLOR;
	//設定 DIR_RIGHT
	cell[L2_SHAPE][DIR_RIGHT][0][0] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_RIGHT][0][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_RIGHT][1][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_RIGHT][2][1] = BLOCK_COLOR;
	//設定 DIR_DOWN
	cell[L2_SHAPE][DIR_DOWN][1][0] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_DOWN][2][0] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_DOWN][1][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_DOWN][1][2] = BLOCK_COLOR;
	//設定 DIR_LEFT
	cell[L2_SHAPE][DIR_LEFT][0][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_LEFT][1][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_LEFT][2][1] = BLOCK_COLOR;
	cell[L2_SHAPE][DIR_LEFT][2][2] = BLOCK_COLOR;
	//設定 N2_SHAPE
	//設定 DIR_UP
	cell[N2_SHAPE][DIR_UP][1][0] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_UP][0][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_UP][1][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_UP][0][2] = BLOCK_COLOR;
	//設定 DIR_RIGHT
	cell[N2_SHAPE][DIR_RIGHT][0][0] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_RIGHT][1][0] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_RIGHT][1][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_RIGHT][2][1] = BLOCK_COLOR;
	//設定 DIR_DOWN
	cell[N2_SHAPE][DIR_DOWN][2][0] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_DOWN][1][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_DOWN][2][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_DOWN][1][2] = BLOCK_COLOR;
	//設定 DIR_LEFT
	cell[N2_SHAPE][DIR_LEFT][0][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_LEFT][1][1] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_LEFT][1][2] = BLOCK_COLOR;
	cell[N2_SHAPE][DIR_LEFT][2][2] = BLOCK_COLOR;
}
void TetrisBlock::rotate_clockwise() {
	direction = (direction + 1) % BLOCK_DIR_COUNT;
}
void TetrisBlock::rotate_counterwise() {
	direction = (direction + 3) % BLOCK_DIR_COUNT;
}
void TetrisBlock::draw(HDC hdc, int ox, int oy) {
	int cur_posx, cur_posy;
	for (int i = 0; i < cellsize; i++)
		for (int j = 0; j < cellsize; j++)
			if (cell[type][direction][i][j] != COLOR_NULL) {
				cur_posx = ox + i;
				cur_posy = oy + j;
				SelectObject(hdc, colorRush[cell[type][direction][i][j]]);
				Rectangle(hdc, cur_posx*cell_Side_Len, cur_posy*cell_Side_Len,
					(cur_posx + 1)*cell_Side_Len, (cur_posy + 1)*cell_Side_Len);
			}
}
