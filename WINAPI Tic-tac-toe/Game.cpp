
#include "Game.h"
#include <ctime>
#include <iostream>

Game::Game(Difficulty dif, bool userFirstMove) : mDifficulty(dif), userMovesFirst(userFirstMove)
{
	SetFieldEmpty();
	mGameState = TheGameIsOn;
	if (userFirstMove)
	{
		mMoveState = UserTurn;
	}
	else
	{
		mMoveState = ComputerTurn;
	}
	StartGame();
}

bool Game::SetEmpty(int x, int y)
{
	if (x < 0 || y < 0 || x > FIELD_WIDTH - 1 || y > FIELD_HEIGHT - 1 || field[x][y] != Empty)
	{
		return false;
	}
	else
	{
		field[x][y] = Empty;
		return true;
	}
}

void Game::RefreshState()
{
	mGameState = CheckState(field);
}

GameState Game::CheckState(SellState field[][3])
{
	//Cross Won Check
	//diagonal check
	if (field[0][0] == Cross && field[1][1] == Cross && field[2][2] == Cross ||
		field[0][2] == Cross && field[1][1] == Cross && field[2][0] == Cross)
	{
		return CrossWon;
	}
	//horizontal check
	if (field[0][0] == Cross && field[1][0] == Cross && field[2][0] == Cross ||
		field[0][1] == Cross && field[1][1] == Cross && field[2][1] == Cross ||
		field[0][2] == Cross && field[1][2] == Cross && field[2][2] == Cross)
	{
		return CrossWon;
	}
	//vertical check
	if (field[0][0] == Cross && field[0][1] == Cross && field[0][2] == Cross ||
		field[1][0] == Cross && field[1][1] == Cross && field[1][2] == Cross ||
		field[2][0] == Cross && field[2][1] == Cross && field[2][2] == Cross)
	{
		return CrossWon;
	}

	//Circle Won Check
	//diagonal check
	if (field[0][0] == Circle && field[1][1] == Circle && field[2][2] == Circle ||
		field[0][2] == Circle && field[1][1] == Circle && field[2][0] == Circle)
	{
		return CircleWon;
	}
	//horizontal check
	if (field[0][0] == Circle && field[1][0] == Circle && field[2][0] == Circle ||
		field[0][1] == Circle && field[1][1] == Circle && field[2][1] == Circle ||
		field[0][2] == Circle && field[1][2] == Circle && field[2][2] == Circle)
	{
		return CircleWon;
	}
	//vertical check
	if (field[0][0] == Circle && field[0][1] == Circle && field[0][2] == Circle ||
		field[1][0] == Circle && field[1][1] == Circle && field[1][2] == Circle ||
		field[2][0] == Circle && field[2][1] == Circle && field[2][2] == Circle)
	{
		return CircleWon;
	}
	//draw check
	bool draw = true;
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			if (field[i][j] == Empty)
			{
				draw = false;
			}
		}
	}
	if (draw)
	{
		return Draw;
	}
	return TheGameIsOn;
}

void Game::AIMove()
{
	switch (mDifficulty)
	{
	case Easy:
		EasyAIMove();
		break;
	case Hard:
		HardAIMove();
		break;
	}
}

int Game::GetRand(int seed)
{
	srand(time(0));
	return rand() % 3;
}

bool Game::FindNextEmptyButton(int& x, int& y)
{
	for(int i = 0; i < 9; i++)
	{
		if (field[x][y] == Empty && i > 0)
		{
			return true;
		}
		else
		{
			//00 10 20 01 11 21 02 12 22
			if (x == 0 && y == 0)
			{
				x = 1; y = 0;
				continue;
			}
			if (x == 1 && y == 0)
			{
				x = 2; y = 0;
				continue;
			}
			if (x == 2	 && y == 0)
			{
				x = 0; y = 1;
				continue;
			}
			if (x == 0 && y == 1)
			{
				x = 1; y = 1;
				continue;
			}
			if (x == 1 && y == 1)
			{
				x = 2; y = 1;
				continue;
			}
			if (x == 2 && y == 1)
			{
				x = 0; y = 2;
				continue;
			}
			if (x == 0 && y == 2)
			{
				x = 1; y = 2;
				continue;
			}
			if (x == 1 && y == 2)
			{
				x = 2; y = 2;
				continue;
			}
			if (x == 2 && y == 2)
			{
				x = 0; y = 0;
				continue;
			}
		}
	}
	return false;
}

bool Game::SuchCrossMoveWillWinTheGame(int x, int y)
{
	SellState tempField[3][3];
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			tempField[i][j] = field[i][j];
		}
	}

	CrossMove(x, y, tempField);

	if (CheckState(tempField) == CrossWon)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Game::SuchCircleMoveWillWinTheGame(int x, int y)
{
	SellState tempField[3][3];
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			tempField[i][j] = field[i][j];
		}
	}

	CircleMove(x, y, tempField);

	if (CheckState(tempField) == CircleWon)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Game::EasyAIMove()
{
	bool moveDone = false;
	while (!moveDone)
	{
		int x = GetRand(0);
		int y = GetRand(1);
		FindNextEmptyButton(x, y);
		moveDone = MakeMove(x, y, field);
	}
	
}

void Game::HardAIMove()
{
	//Setting a starting point
	int x = GetRand(0);
	int y = GetRand(1);
	//Trying to make a move in the center of the field
	if (field[1][1] == Empty)
	{
		MakeMove(1, 1, field);
		return;
	}
	//looking for possibility to win
	for (int i = 0; i < 9; i++)
	{
		FindNextEmptyButton(x, y);
		if (SuchCircleMoveWillWinTheGame(x, y) && userMovesFirst 
			|| SuchCrossMoveWillWinTheGame(x,y) && !userMovesFirst)
		{
			MakeMove(x, y, field);
			return;
		}
	}
	//looking for possibility to stop user from wining
	for (int i = 0; i < 9; i++)
	{
		FindNextEmptyButton(x, y);
		if (SuchCircleMoveWillWinTheGame(x, y) && !userMovesFirst
			|| SuchCrossMoveWillWinTheGame(x, y) && userMovesFirst)
		{
			MakeMove(x, y, field);
			return;
		}
	}
	//Putting the move to ome of the corners
	if (field[0][0] == Empty || field[2][0] == Empty || field[0][2] == Empty || field[2][2] == Empty)
	{
		while (2 + 2 == 4)
		{
			FindNextEmptyButton(x, y);
			if (x == 0 && y == 0 || x == 2 && y == 0 || x == 0 && y == 2 || x == 2 && y == 2)
			{
				MakeMove(x, y, field);
				return;
			}
		}
	}
	EasyAIMove();
}

void Game::ChangeTurn()
{
	if (mMoveState == UserTurn)
	{
		mMoveState = ComputerTurn;
	}
	else if(mMoveState == ComputerTurn)
	{
		mMoveState = UserTurn;
	}
}

bool Game::UserMove(int x, int y)
{
	if (mMoveState == UserTurn)
	{
		if (field[x][y] == Empty)
		{
			MakeMove(x, y, field);
			return true;
		}
	}
	return false;
}

void Game::StartGame()
{
	if (!userMovesFirst)
	{
		AIMove();
	}
}

bool Game::MakeMove(int x, int y, SellState field[][3])
{
	if (x < 0 || y < 0 || x > FIELD_WIDTH - 1 || y > FIELD_HEIGHT - 1 || field[x][y] != Empty)
	{
		return false;
	}
	else
	{
		if (userMovesFirst && mMoveState == UserTurn || !userMovesFirst && mMoveState == ComputerTurn)
		{
			CrossMove(x,y,field);
			ChangeTurn();
			RefreshState();
			return true;
		}
		else if(userMovesFirst && mMoveState == ComputerTurn || !userMovesFirst && mMoveState == UserTurn)
		{
			CircleMove(x,y,field);
			ChangeTurn();
			RefreshState();
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Game::CrossMove(int x, int y, SellState field[][3])
{
	field[x][y] = Cross;
	return true;
}

bool Game::CircleMove(int x, int y, SellState field[][3])
{
	field[x][y] = Circle;
	return true;
}


void Game::SetFieldEmpty()
{
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			field[i][j] = Empty;
		}
	}
}

SellState Game::GetSellState(int x, int y)
{
	if (x < 0 || y < 0 || x > FIELD_WIDTH - 1 || y > FIELD_HEIGHT - 1)
	{
		return Empty;
	}
	else
	{
		return field[x][y];
	}
}

GameState Game::GetGameState()
{
	return mGameState;
}
