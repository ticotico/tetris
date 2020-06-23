#ifndef __GAMEWINDOW_H__
#define __GAMEWINDOW_H__

#include <windows.h>
#include <memory>
#include <string>

#include "KeyOperation.h"
#include "GameDrawer.h"

constexpr int eWindowWidth = 350;
constexpr int eWindowHeight = 500;

// Initialize rand value and game object.
void gameInit();

// handle game logic operation
void gameWork();

// update game graphic
void gameDraw(HDC hdc);

// Deinitialize game object.
void gameDeinit();

// initialize game system
class Tetris
{
public:
	// constructure
	// Initialize all member variables
	Tetris();

	// Obtain darw information from internal data
	// resturn value: Draw information for class Drawer.
	std::list<std::unique_ptr<DrawCommandBase>> ExtractDrawCommand();

	// A function for key down is notified. Handle move down operation.
	void BlockMoveDown();

	// A function for key left is notified. Handle move left operation.
	void BlockMoveLeft();

	// A function for key right is notified. Handle move right operation.
	void BlockMoveRight();

	// A function for key rotate(space) is notified. Handle rotate operation.
	void BlockRotate();

	// Internal logic operation before check key state.
	void WorkBeforeCheckKeyState();

	// Internal logic operation after check key state.
	void WorkAfterCheckKeyState();
private:
	//information for draw objects

	static constexpr int eGridWidth = 10;
	static constexpr int eGridHeight = 20;
	static constexpr int eGridSizeForDraw = 20;
	static constexpr int eScorePosX = 5;
	static constexpr int eScorePosY = 5 + eGridHeight * eGridSizeForDraw;
	static constexpr int eGameOverPosX = eGridWidth * eGridSizeForDraw / 2;
	static constexpr int eGameOverPosY = eGridHeight * eGridSizeForDraw / 2;

	// Whem clear N line, increase score by index N.
	static constexpr int eScoreWhenClearLine[] = {0, 100, 300, 600, 1200};

	static constexpr int eMoveBlocIinitialPosX = 3;
	static constexpr int eMoveBlockInitialPosY = 0;
	static constexpr int ePeriodOfMoveDownWhenKeyDownFree = 5;
	enum class GridState : int
	{
		None = 0,
		Fixed = 1,
		Moving = 2
	};

	int m_iMoveBlockPosX, m_iMoveBlockPosY;
	int m_iMoveBlockShape, m_iMoveBlockDir;
	int m_iNextBlockShape, m_iNextBlockDir;
	std::size_t m_szScore;
	std::size_t m_szCounterOfMoveDownWhenKeyDownFree;
	bool m_bIsGameOver;

	GridState m_gridstate[eGridHeight][eGridWidth];

	// Try the moving block whether place at given position, shate and direction.
	// Return value: True when the moving block can place, otherwise return false.
	bool TestPlaceMovingBlock(int test_x, int test_y, int test_shape, int test_dir) const;

	// Assign GridState::Moving at accurate position by member variables which stores moving block information
	void UpdateMoveBlockArrayData();

	// When the moving block goes down failed, it means should be replace from GridState::Moving to GridState::Fixed
	void ChangeMoveBlockArrayDataToFixed();

	// If there are horizontal line which are all fixed, xlear the line.
	// Return value: Number of line cleared.
	int ClearHorizontalLineAllFixed();
};

// A class encapsulation all game components.
class GameSystem
{
public:
	GameSystem();

	// Initialize game system
	void init();

	// Detect key state and notify logic object.
	void work();

	// Draw game result on HDC.
	// param hdc: HDC to be drawn.
	void draw(HDC hdc);
private:
	std::shared_ptr<KeyObserver> spKeyObsLeft;
	std::shared_ptr<KeyObserver> spKeyObsRight;
	std::shared_ptr<KeyObserver> spKeyObsDown;
	std::shared_ptr<KeyObserver> spKeyObsSpace;

	std::unique_ptr<KeyState> upKeyStateLeft;
	std::unique_ptr<KeyState> upKeyStateRight;
	std::unique_ptr<KeyState> upKeyStateDown;
	std::unique_ptr<KeyState> upKeyStateSpace;

	std::unique_ptr<Drawer> upDrawer;
	std::unique_ptr<Tetris> upTetrris;
};

#endif
