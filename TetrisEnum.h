#ifndef TETRISENUM_H
#define TETRISENUM_H

enum { cell_Side_Len = 20 };	//每個方塊(cell)的邊長
enum { window_W = 10, window_H = 20 };
enum { KEY_RESPONCE_TIME = 300 };
enum { max_downspeed = 7};	//方塊隨侍間往下移動的速度為大值 
//列舉block_type(方形/線形/L形/T形/N形)
enum BLOCK_TYPE { SQUARE_SHAPE, LINE_SHAPE, L_SHAPE, T_SHAPE, N_SHAPE, L2_SHAPE, N2_SHAPE, BLOCK_TYPE_COUNT };
//列舉block_direction(上/右/下/左)
enum BLOCK_DIR { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT, BLOCK_DIR_COUNT };
//列舉每個顏色
enum ENUM_COLOR { COLOR_NULL, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_COUNT };
enum { cellsize = 4 };	//block.cell[type][dir][][]的size
enum { BLOCK_COLOR = COLOR_GREEN };	//決定 block 使用顏色
enum { blcok_hint = 3 };

#endif
