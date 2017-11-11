#pragma once
#include "consts.h"

enum SellState {Empty = 0, Cross = 1, Circle = 2};
enum Difficulty {Easy , Hard};
enum GameState {TheGameIsOn, CrossWon, CircleWon, Draw};
enum MoveState {UserTurn, ComputerTurn};

class Game
{
public:
	Game(Difficulty dif, bool firstMove);

	bool UserMove(int x, int y);
	void AIMove();
	SellState GetSellState(int x, int y);
	GameState GetGameState();
private:
	SellState field[FIELD_HEIGHT][FIELD_WIDTH];
	bool userMovesFirst;
	Difficulty mDifficulty;
	GameState mGameState;
	MoveState mMoveState;

	void StartGame();
	bool MakeMove(int x, int y, SellState field[][3]);
	bool CrossMove(int x, int y, SellState field[][3]);
	bool CircleMove(int x, int y, SellState field[][3]);

	void SetFieldEmpty();
	bool SetEmpty(int x, int y);

	void RefreshState();
	GameState CheckState(SellState field[][3]);

	int GetRand(int seed);

	bool FindNextEmptyButton(int& x, int& y);

	bool SuchCrossMoveWillWinTheGame(int x, int y);
	bool SuchCircleMoveWillWinTheGame(int x, int y);

	void EasyAIMove();
	void HardAIMove();
	
	void ChangeTurn();
};

