#include "GameSystem.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <array>

static constexpr int eMovingBlockShapeCount = 7;
static constexpr int eMovingBlockDirCount = 4;
static constexpr int eMoveBlockGridCount = 4;

//expression of the specified shape and dirction of a moving block
//0x0f00  = 0000,1111,0000,0000
//0000
//1111
//0000
//0000
static constexpr int eMovingBlockDigitData[eMovingBlockShapeCount][eMovingBlockDirCount] =
{
    {0x0f00, 0x2222, 0x00f0, 0x4444},
    {0x8e00, 0x6440, 0x0e20, 0x44c0},
    {0x2e00, 0x4460, 0x0e80, 0xc440},
    {0xcc00, 0xcc00, 0xcc00, 0xcc00},
    {0x6c00, 0x4620, 0x06c0, 0x8c40},
    {0x4e00, 0x4640, 0x0e40, 0x4c40},
    {0xc600, 0x2640, 0x0c60, 0x4c80}
};

static inline auto GenerateMovingBlockDigitData()
{
    std::array<std::array<std::array<std::array<int, eMoveBlockGridCount>, eMoveBlockGridCount>, eMovingBlockDirCount>, eMovingBlockShapeCount> res{};

    for (int a = 0; a != eMovingBlockShapeCount; ++a) {
        for (int b = 0; b != eMovingBlockDirCount; ++b) {
            for (int c = 0; c != eMoveBlockGridCount; ++c) {
                for (int d = 0; d != eMoveBlockGridCount; ++d) {
                    res[a][b][c][d] = (eMovingBlockDigitData[a][b] >> (eMoveBlockGridCount*eMoveBlockGridCount - 1 - eMoveBlockGridCount * c - d)) & 0x1;
                }
            }
        }
    }

    return res;
}
static const auto eMovingBlockData = GenerateMovingBlockDigitData();
GameSystem* pGameSystem = nullptr;

//=========================
void gameInit()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	pGameSystem = new GameSystem();
	pGameSystem->init();
}

void gameWork()
{
	pGameSystem->work();
}

void gameDraw(HDC hdc)
{
	pGameSystem->draw(hdc);
}

void gameDeinit()
{
	if(pGameSystem)
	{
		delete pGameSystem;
		pGameSystem = nullptr;
	}
}

//=========================
Tetris::Tetris()
	: m_iMoveBlockPosX(eMoveBlocIinitialPosX), m_iMoveBlockPosY(eMoveBlockInitialPosY)
	, m_iMoveBlockShape(0), m_iMoveBlockDir(0)
	, m_iNextBlockShape(0), m_iNextBlockDir(0)
	, m_szScore(0) , m_szCounterOfMoveDownWhenKeyDownFree(0)
	, m_bIsGameOver(false)
	, m_gridstate{ {GridState::None} }
{
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
	static_assert((sizeof(eScoreWhenClearLine) / sizeof(int)) > eMoveBlockGridCount);
#endif

	m_iMoveBlockShape = std::rand() % eMovingBlockShapeCount;
	m_iMoveBlockDir = std::rand() % eMovingBlockDirCount;
	m_iNextBlockShape = std::rand() % eMovingBlockShapeCount;
	m_iNextBlockDir = std::rand() % eMovingBlockDirCount;
	UpdateMoveBlockArrayData();
}

std::list<std::unique_ptr<DrawCommandBase>> Tetris::ExtractDrawCommand()
{
	std::list<std::unique_ptr<DrawCommandBase>> ret;

	//draw a rest form (0,0) to (eGridWidth*eGridSizeForDraw, eGridHeight*eGridSizeForDraw)
	std::unique_ptr<DrawCommandBase> upDraeGameArea(new DrawRectCommand(0, 0, eGridWidth*eGridSizeForDraw, eGridHeight*eGridSizeForDraw, Drawer::Color::WHITE));
	ret.push_back(std::move(upDraeGameArea));

	//draw rectangles for which are moving or fixed.
	int x, y;
	for(int i = 0; i < eGridHeight; ++i)
	{
		for(int j =0; j < eGridWidth; ++j)
		{
			x = j * eGridSizeForDraw;
			y = i * eGridSizeForDraw;
			std::unique_ptr<DrawCommandBase> upDrawRect;
			switch(m_gridstate[i][j])
			{
				case GridState::Fixed:
					upDrawRect.reset(new DrawRectCommand(x, y, x+eGridSizeForDraw, y+eGridSizeForDraw, Drawer::Color::RED));
					ret.push_back(std::move(upDrawRect));
					break;
				case GridState::Moving:
					upDrawRect.reset(new DrawRectCommand(x, y, x+eGridSizeForDraw, y+eGridSizeForDraw, Drawer::Color::GREEN));
					ret.push_back(std::move(upDrawRect));
					break;
				default:
					break;
			}
		}
	}

	//draw next move block
	for(int i = 0; i < eMoveBlockGridCount; ++i)
	{
		for(int j = 0; j < eMoveBlockGridCount; ++j)
		{
			if(eMovingBlockData[m_iNextBlockShape][m_iNextBlockDir][i][j] == 1)
			{
				x = (j + 1 + eGridWidth) * eGridSizeForDraw;
				y = (i + 1) * eGridSizeForDraw;
				std::unique_ptr<DrawCommandBase> upDrawRect;
				upDrawRect.reset(new DrawRectCommand(x, y, x+eGridSizeForDraw, y+eGridSizeForDraw, Drawer::Color::GREEN));
				ret.push_back(std::move(upDrawRect));
			}
		}
	}

	//draw score text
	std::string sScore = "score: " + std::to_string(m_szScore);
	std::unique_ptr<DrawCommandBase> upDrawScore(new DrawTextCommand(eScorePosX, eScorePosY, sScore));
	ret.push_back(std::move(upDrawScore));

	//draw gameover text
	if(m_bIsGameOver)
	{
		std::string sGameOver = "Game Over";
		std::unique_ptr<DrawCommandBase> upDrawGameOver(new DrawTextCommand(eGameOverPosX, eGameOverPosY, sGameOver));
		ret.push_back(std::move(upDrawGameOver));
	}

	return ret;
}

void Tetris::BlockMoveDown()
{
	//forbid move when game over
	if(m_bIsGameOver)
		return;

	if(TestPlaceMovingBlock(m_iMoveBlockPosX, m_iMoveBlockPosY+1, m_iMoveBlockShape, m_iMoveBlockDir))
	{
		++m_iMoveBlockPosY;
		m_szCounterOfMoveDownWhenKeyDownFree = 0;
		UpdateMoveBlockArrayData();
	}
	else
	{
		m_iMoveBlockPosX = eMoveBlocIinitialPosX;
		m_iMoveBlockPosY = eMoveBlockInitialPosY;
		m_iMoveBlockShape = m_iNextBlockShape;
		m_iMoveBlockDir = m_iNextBlockDir;
		m_iNextBlockShape = std::rand() % eMovingBlockShapeCount;
		m_iMoveBlockDir = std::rand() % eMovingBlockDirCount;
		ChangeMoveBlockArrayDataToFixed();
		auto iClearLines = ClearHorizontalLineAllFixed();
		if(0 < iClearLines && iClearLines <= eMoveBlockGridCount)
			m_szScore += eScoreWhenClearLine[iClearLines];
		//If test place move block fail at this time. It means game over.
		if(false == TestPlaceMovingBlock(m_iMoveBlockPosX, m_iMoveBlockPosY, m_iMoveBlockShape, m_iMoveBlockDir))
			m_bIsGameOver = true;
		UpdateMoveBlockArrayData();
	}
}

void Tetris::BlockMoveLeft()
{
	//forbid move when game over
	if(!m_bIsGameOver)
	{
		if (TestPlaceMovingBlock(m_iMoveBlockPosX-1, m_iMoveBlockPosY, m_iMoveBlockShape, m_iMoveBlockDir))
		{
			--m_iMoveBlockPosX;
			UpdateMoveBlockArrayData();
		}
	}
}

void Tetris::BlockMoveRight()
{
	//forbid move when game over
	if(!m_bIsGameOver)
	{
		if (TestPlaceMovingBlock(m_iMoveBlockPosX + 1, m_iMoveBlockPosY, m_iMoveBlockShape, m_iMoveBlockDir))
		{
			++m_iMoveBlockPosX;
			UpdateMoveBlockArrayData();
		}
	}
}

void Tetris::BlockRotate()
{
	//forbid move when game over
	if(!m_bIsGameOver)
	{
		auto new_dir = (m_iMoveBlockDir==eMovingBlockDirCount-1) ? (0) : (m_iMoveBlockDir+1);
		if (TestPlaceMovingBlock(m_iMoveBlockPosX, m_iMoveBlockPosY, m_iMoveBlockShape, new_dir))
		{
			m_iMoveBlockDir = new_dir;
			UpdateMoveBlockArrayData();
		}
	}
}

void Tetris::WorkBeforeCheckKeyState()
{
	++m_szCounterOfMoveDownWhenKeyDownFree;
}

void Tetris::WorkAfterCheckKeyState()
{
	if(m_szCounterOfMoveDownWhenKeyDownFree > ePeriodOfMoveDownWhenKeyDownFree)
		BlockMoveDown();
}

bool Tetris::TestPlaceMovingBlock(int test_x, int test_y, int test_shape, int test_dir) const
{
	bool ret(false);
	if(test_shape < 0 || test_shape > eMovingBlockShapeCount)
		ret = false;
	else if(test_dir < 0 || test_dir > eMovingBlockDirCount)
		ret = false;
	else
	{
		int moveBolckCode(eMovingBlockDigitData[test_shape][test_dir]);
		int terrisCode(0);
		bool orOPeration(false);
		int cur_x(0), cur_y(0);
		for(int y = 0; y < eMoveBlockGridCount; ++y)
		{
			for(int x = 0; x < eMoveBlockGridCount; ++x)
			{
				cur_x = test_x + x;
				cur_y = test_y + y;
				if(cur_x < 0 || cur_x >= eGridWidth || cur_y < 0 || cur_y >= eGridHeight)
					orOPeration = true;
				else if(m_gridstate[cur_y][cur_x] == GridState::Fixed && eMovingBlockData[test_shape][test_dir][y][x] == 1)
					orOPeration = true;
				else
					orOPeration = false;

				if(orOPeration)
					terrisCode |= (1<<(eMoveBlockGridCount*eMoveBlockGridCount - 1 - eMoveBlockGridCount * y - x));
			}
		}
		ret = ((moveBolckCode & terrisCode) == 0);
	}
	return ret;
}

void Tetris::UpdateMoveBlockArrayData()
{
	for(auto& line : m_gridstate)
	{
		for(auto& cell : line)
		{
			if(cell == GridState::Moving)
				cell = GridState::None;
		}
	}
	for(int y = 0; y < eMoveBlockGridCount; ++y)
	{
		for(int x = 0; x < eMoveBlockGridCount; ++x)
		{
			if(eMovingBlockData[m_iMoveBlockShape][m_iMoveBlockDir][y][x] == 1)
				m_gridstate[m_iMoveBlockPosY+y][m_iMoveBlockPosX+x] = GridState::Moving;
		}
	}
}

void Tetris::ChangeMoveBlockArrayDataToFixed()
{
	for(auto& line : m_gridstate)
	{
		for(auto& cell : line)
		{
			if(cell == GridState::Moving)
				cell = GridState::Fixed;
		}
	}
}

int Tetris::ClearHorizontalLineAllFixed()
{
	int clearLineCnt = 0;
	int y_src = eGridHeight - 1;
	int y_dest = eGridHeight - 1;
	for(; y_src >= 0; --y_src, --y_dest)
	{
		while((y_src >= 0) && std::all_of(m_gridstate[y_src], m_gridstate[y_src] + eGridWidth,
			[](GridState state) {return state == GridState::Fixed; } ))
		{
			--y_src;
			++clearLineCnt;
		}
		if(y_src != y_dest)
			std::copy(m_gridstate[y_src], m_gridstate[y_src] + eGridWidth, m_gridstate[y_dest]);
	}
	while(y_dest >= 0)
	{
		std::fill(m_gridstate[y_dest], m_gridstate[y_dest] + eGridWidth, GridState::None);
		--y_dest;
	}
	return clearLineCnt;
}

//=========================
GameSystem::GameSystem()
	: spKeyObsLeft(nullptr)
	, spKeyObsRight(nullptr)
	, spKeyObsDown(nullptr)
	, spKeyObsSpace(nullptr)
	, upKeyStateLeft(new KeyState(VK_LEFT))
	, upKeyStateRight(new KeyState(VK_RIGHT))
	, upKeyStateDown(new KeyState(VK_DOWN))
	, upKeyStateSpace(new KeyState(VK_SPACE))
	, upDrawer(new Drawer(eWindowWidth, eWindowHeight))
	, upTetrris(new Tetris())
{
	spKeyObsLeft.reset(new KeyObserver(std::bind(&Tetris::BlockMoveLeft, upTetrris.get())));
	spKeyObsRight.reset(new KeyObserver(std::bind(&Tetris::BlockMoveRight, upTetrris.get())));
	spKeyObsDown.reset(new KeyObserver(std::bind(&Tetris::BlockMoveDown, upTetrris.get())));
	spKeyObsSpace.reset(new KeyObserver(std::bind(&Tetris::BlockRotate, upTetrris.get())));

	upKeyStateLeft->AddObserver(spKeyObsLeft);
	upKeyStateRight->AddObserver(spKeyObsRight);
	upKeyStateDown->AddObserver(spKeyObsDown);
	upKeyStateSpace->AddObserver(spKeyObsSpace);
}

void GameSystem::init()
{
}

void GameSystem::work()
{
	upTetrris->WorkBeforeCheckKeyState();
	upKeyStateLeft->Check();
	upKeyStateRight->Check();
	upKeyStateDown->Check();
	upKeyStateSpace->Check();
	upTetrris->WorkAfterCheckKeyState();
}

void GameSystem::draw(HDC hdc)
{
	auto cmds = upTetrris->ExtractDrawCommand();
	for (auto iter = cmds.begin(); iter != cmds.end(); ++iter)
		(*iter)->Execute(*upDrawer);

	upDrawer->Update(hdc);
}
