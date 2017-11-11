/*Ќаписать игру Ђ рестики-ноликиї, учитыва€ следующие требовани€:
игровое поле размером 3х3 должно состо€ть из кнопок;
при нажатии на кнопку, на ней должна отобразитьс€ картинка (крестик или нолик).
ƒл€ установки изображени€ на кнопку ей нужно отправить сообщение BM_SETIMAGE,
с которым в WPARAM передать значение IMAGE_BITMAP,
а в LPARAM Ц дескриптор растрового битового образа (и главное не забыть про свойство Bitmap!);
необходимо предотвращать попытку поставить крестик или нолик на зан€тую клетку;
предоставить пользователю право выбора первого хода, использу€ флажок;
предусмотреть возможность выбора уровн€ сложности, использу€ переключатели;
предусмотреть кнопку ЂЌачать новую игруї.*/

#include <windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include "resource.h"
#include "Game.h"
#include "consts.h"

HINSTANCE hInst;
HICON hMainIcon;	
HBITMAP hBM_Cross;
HBITMAP hBM_Circle;
HBITMAP hBM_Empty;
HWND hCB_FirstMove;
HWND hRB_Easy;
HWND hRB_Hard;
HWND buttons[3][3];

void SetBitmap(HWND sell, HBITMAP);
void InitGame();
void EnableField(bool val);
void GetButtonsHandles(HWND hWnd);
void PrintGameState();
void SellPressed(int x, int y);
void CheckGameState();

Game* curGame;

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	hInst = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	case WM_INITDIALOG:
		hMainIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
		hMainIcon = (HICON)SetClassLong(hWnd, GCL_HICON, LONG(hMainIcon));
		hBM_Circle = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(BM_CIRCLE));
		hBM_Cross = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(BM_CROSS));
		hBM_Empty = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(BM_EMPTY));

		hCB_FirstMove = GetDlgItem(hWnd, CB_FIRST_MOVE);
		hRB_Easy = GetDlgItem(hWnd, RB_EASY);
		hRB_Hard = GetDlgItem(hWnd, RB_HARD);

		SendMessage(hCB_FirstMove, BM_SETCHECK, BST_CHECKED, 1);
		SendMessage(hRB_Easy, BM_SETCHECK, BST_CHECKED, 1);

		GetButtonsHandles(hWnd);
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == BTN_NEW_GAME)
		{
			InitGame();
		}
		switch (LOWORD(wParam))
		{
			case BTN_0_0:
			SellPressed(0, 0);
			return TRUE;
		case BTN_0_1:
			SellPressed(0, 1);
			return TRUE;
		case BTN_0_2:
			SellPressed(0, 2);
			return TRUE;
		case BTN_1_0:
			SellPressed(1, 0);
			return TRUE;
		case BTN_1_1:
			SellPressed(1, 1);
			return TRUE;
		case BTN_1_2:
			SellPressed(1, 2);
			return TRUE;
		case BTN_2_0:
			SellPressed(2, 0);
			return TRUE;
		case BTN_2_1:
			SellPressed(2, 1);
			return TRUE;
		case BTN_2_2:
			SellPressed(2, 2);
			return TRUE;
		}
	}
	return FALSE;
}

void SetBitmap(HWND sell, HBITMAP bitmap)
{
	SendMessage(sell, BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(bitmap));
}

void GetButtonsHandles(HWND hWnd)
{
	buttons[0][0] = GetDlgItem(hWnd, BTN_0_0);
	buttons[0][1] = GetDlgItem(hWnd, BTN_0_1);
	buttons[0][2] = GetDlgItem(hWnd, BTN_0_2);
	buttons[1][0] = GetDlgItem(hWnd, BTN_1_0);
	buttons[1][1] = GetDlgItem(hWnd, BTN_1_1);
	buttons[1][2] = GetDlgItem(hWnd, BTN_1_2);
	buttons[2][0] = GetDlgItem(hWnd, BTN_2_0);
	buttons[2][1] = GetDlgItem(hWnd, BTN_2_1);
	buttons[2][2] = GetDlgItem(hWnd, BTN_2_2);
}

void PrintGameState()
{
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			switch (curGame->GetSellState(i,j))
			{
			case Empty:
				SetBitmap(buttons[i][j], hBM_Empty);
				break;
			case Cross:
				SetBitmap(buttons[i][j], hBM_Cross);
				break;
			case Circle:
				SetBitmap(buttons[i][j], hBM_Circle);
				break;
			}
			
		}
	}
}

void InitGame()
{
	if (curGame)
	{
		delete curGame;
	}

	Difficulty dif = Easy;
	if (Button_GetCheck(hRB_Easy) == BST_CHECKED)
	{
		dif = Easy;
	}
	else if (Button_GetCheck(hRB_Hard) == BST_CHECKED)
	{
		dif = Hard;
	}

	curGame = new Game(dif, Button_GetCheck(hCB_FirstMove) == BST_CHECKED);

	EnableField(true);
}

void EnableField(bool val)
{
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			EnableWindow(buttons[i][j], val);
		}
	}

	PrintGameState();
}

void SellPressed(int x, int y)
{
	if (curGame->UserMove(x, y))
	{
		PrintGameState();
		CheckGameState();
		if (curGame->GetGameState() == TheGameIsOn)
		{
			curGame->AIMove();
			PrintGameState();
			CheckGameState();
		}
	}	
}

void CheckGameState()
{
	switch (curGame->GetGameState())
	{
	case CrossWon:
		EnableField(false);
		MessageBox(0, TEXT("Cross won"), TEXT("Game is over"), MB_OK | MB_ICONINFORMATION);
		break;
	case CircleWon:
		EnableField(false);
		MessageBox(0, TEXT("Circle won"), TEXT("Game is over"), MB_OK | MB_ICONINFORMATION);
		break;
	case Draw:
		EnableField(false);
		MessageBox(0, TEXT("It's a draw"), TEXT("Game is over"), MB_OK | MB_ICONINFORMATION);
		break;
	default:
		break;
	}
}
