//俄羅斯方塊遊戲

/*
105/04/05: 畫圖，block隨鍵盤移動
105/04/06: window一行消除的函式
roatae有問題
105/04/07: roatte問題解決，原因為使用EACH_CELL巨集
該巨集的if判斷在rotate中並不需要
105/04/11:
使用陣列應放每種形狀的block
取而代之的變更旋轉的函式
加入隨時間移動的函式 block::flow()
105/04/13:
解決畫面閃爍問題(使用背景頁的概念)
105/05/07:
將俄羅斯方塊的所有內容搬到類別Tetris(方便之後的2P)
建立2P視窗
105/06/10
畫出接下來三個
*/
#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include "fps.h"
#include "keystate.h"
#include "BackGroundPage.h"
#include <list>
#include "Tetris.h"

void gameInit();
void gameWork();
void gameDraw(HDC& hdc);
#endif
