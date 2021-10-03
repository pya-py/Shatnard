#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define EMPTY 0
#define S_1 1
#define E_1 2
#define H_1 3
#define C_1 4
#define Q_1 5
#define K_1 6
#define S_2 7
#define E_2 8
#define H_2 9
#define C_2 10
#define Q_2 11
#define K_2 12

#define COLOMNS 12
#define ROWS 2
#define CAPACITY 6
#define WRONG_MOVE -100
#define DONE 100
#define PIECES 15
#define TYPES 6
#define NO_MOVES -200
#define USED -300
#define savedGame "gameSave.dat"
#define savedGameNumber "numberOfSavedGames.dat"
#define SAVE_INTERRUPTED -1000
#define OUT -1
#define SHOW 100
#define HIDE -100

char *names[ COLOMNS + 1 ] = { { "    " }, { "S_1 " }, { "E_1 " }, { "H_1 " }, { "C_1 " }, { "Q_1 " }, { "K_1 " }, { "S_2 " }, { "E_2 " }, { "H_2 " }, { "C_2 " }, { "Q_2 " }, { "K_2 " } };
char *rooms[ COLOMNS * 2 + 1 ] = { "C1_1", "C1_2", "C1_3", "C1_4", "C1_5", "C1_6", "R1_1", "R1_2", "R1_3", "R1_4", "R1_5", "R1_6", "C2_1", "C2_2", "C2_3", "C2_4", "C2_5", "C2_6", "R2_1", "R2_2", "R2_3", "R2_4", "R2_5", "R2_6" ,"O"};
char *oNames[COLOMNS + 1] = { "   ", "O_S", "O_E", "O_H", "O_C", "O_Q", "O_K", "O_S", "O_E", "O_H", "O_C", "O_Q", "O_K" };

int board[2][COLOMNS][CAPACITY] = { EMPTY };
int playerOs[2][PIECES] = { EMPTY };
int point[2] = { 0 };
int holes[3][2] = { EMPTY }, coins[3][2] = { EMPTY };
int playerBanned[2] = { 0 };
int coinsCollected[2] = { 0 };
int newSaveNumber = 1;

void sendTpUpperCase(char*);
int loadBoard();
void updateBoard();
void obtainMove(char*, char*,int*);
int selectDice(int , int, int, int, int, int* , int*);
int movePlayer(int , int , int, int, int, int);
int freeThePiece(int,int,int);
int findFreePiece(int, int);
void checkForCoinRelease(int, int);
int checkResult(int);
void showMsg(int, int, int, int);
int isPlayerCastleFull(int);
int movePieceToO(int, int, int, int);
int anyMoveLeft(int,int);
void createHolesAndCoins();
int loadNames(int);
int save(int);

int main()
{
	int turn = 0,saveResult = 0,dice1 = 0,dice2 = 0;
	char temp;
	int index = 0;
	int dices[2] = { 0 };
	char source[10], destination[10];
	int moveInfo[5];
	int result = 0;
	int moves = 2;
	int availableDices = 2;
	int matched = 0;
	srand(time(NULL));
	turn = loadBoard();
	while (1)
	{
		updateBoard();
		printf("\nPlayer %d, please press \'d\' to throw dices...", turn + 1);
		char input = _getch();
		if (input == 'd' || input == 'D')
		{
			if (playerBanned[turn] != 0)
			{
				playerBanned[turn]--;
				printf("\nPlayer %d is banned!\n", turn+1);
				result = 0;
				turn = (turn + 1) % 2;
				_getch();
				continue;
			}
			moves = 2;
			dice1 = dices[0] = rand() % 6 + 1;;
			dice2 = dices[1] = rand() % 6 + 1;
			availableDices = 2;
			if (dices[0] == dices[1])
				moves = 4;

			for (int i = 0; i < moves; i++)
			{
				updateBoard();
				matched = 0;
				printf("\nDices: %d , %d", dice1, dice2);
				if (i == 2 && dice1 == dice2 && dices[0] == USED && dices[1] == USED)
					dices[0] = dices[1] = dice1;
				for (int c = 0; c < 2 && !anyMoveLeft(turn, dices[c]); c++)
					availableDices--;
				if (availableDices == 0)
				{
					showMsg(turn + 1, NO_MOVES, i, moves);
					_getch();
					break;
				}
				showMsg(turn + 1, result, i, moves);
				result = 0;
				scanf("%s%s", source, destination);
				sendTpUpperCase(source);
				sendTpUpperCase(destination);
				obtainMove(source, destination, moveInfo);

				if (playerOs[turn][0] != EMPTY)
				{
					int pieceIndex;
					for (pieceIndex = 0; pieceIndex <= COLOMNS && strcmp(oNames[playerOs[turn][pieceIndex]], source); pieceIndex++);
					if (pieceIndex > COLOMNS)
					{
						result = WRONG_MOVE;
					}
					else
					{
						index = selectDice(turn, OUT, OUT, moveInfo[3], moveInfo[4], dices, &matched);
						if (matched == 0)
							result = WRONG_MOVE;
						else
							result = freeThePiece(turn, pieceIndex, dices[index]);
						if (result == DONE)
							dices[index] = USED;
					}
				}
				else if (!strcmp(destination, rooms[24]))
				{
					index = selectDice(turn, moveInfo[1], moveInfo[2], moveInfo[3], moveInfo[4], dices, &matched);
					result = movePieceToO(turn, moveInfo[1], moveInfo[2], dices[index]);
					if (result == DONE)
						dices[index] = USED;

				}
				else if (moveInfo[0] == 2)
				{
					index = selectDice(turn, moveInfo[1], moveInfo[2], moveInfo[3], moveInfo[4], dices, &matched);
					result = movePlayer(turn, dices[index], moveInfo[1], moveInfo[2], moveInfo[3], moveInfo[4]);
					if (result == DONE)
						dices[index] = USED;
				}
				else
				{
					result = WRONG_MOVE;
				}
				if (result == WRONG_MOVE)
					i--;
				if (i == moves - 1)
				{
					updateBoard();
					showMsg(turn + 1, result, moves, moves);
				}
			}

			result = 0;
			turn = (turn + 1) % 2;
			_getch();

		}
		printf("\ns. Save Game\to.w. Next Move ");
		if ((temp = _getch()) == 's' || temp == 'S')
		{
			saveResult = save(turn);
			if (saveResult == SAVE_INTERRUPTED)
				printf("\nSaving Failed !\n");
			else
				printf("\nShatnard%d Saved. ", newSaveNumber - 1);
			_getch();
		}
	}
}

int loadBoard()
{
	char t;
	int turn = 0,saveNumber = 0;
	newSaveNumber = loadNames(HIDE);
	while (1)
	{

		system("cls");
		printf("1. Continue \n2. New Game\n3. Load From Map File :");
		t = _getch() - '0';
		if (t == 2)
		{
			/* Player 1 */
			board[0][0][0] = K_2;
			board[0][0][1] = Q_2;

			board[0][5][0] = C_1;
			board[0][5][1] = S_1;
			board[0][5][2] = H_1;
			board[0][5][3] = S_1;
			board[0][5][4] = C_1;

			board[0][7][0] = S_1;
			board[0][7][1] = E_1;
			board[0][7][2] = S_1;

			board[0][11][0] = S_2;
			board[0][11][1] = H_2;
			board[0][11][2] = S_2;
			board[0][11][3] = S_2;
			board[0][11][4] = E_2;

			/* Player 2 */
			board[1][0][0] = K_1;
			board[1][0][1] = Q_1;

			board[1][5][0] = C_2;
			board[1][5][1] = S_2;
			board[1][5][2] = H_2;
			board[1][5][3] = S_2;
			board[1][5][4] = C_2;

			board[1][7][0] = S_2;
			board[1][7][1] = E_2;
			board[1][7][2] = S_2;

			board[1][11][0] = S_1;
			board[1][11][1] = H_1;
			board[1][11][2] = S_1;
			board[1][11][3] = S_1;
			board[1][11][4] = E_1;
			createHolesAndCoins();
			break;
		}
		else if (t == 1)
		{
			FILE *f = fopen(savedGame, "r");
			if (!f)
			{
				printf("\nNo saves found! Try again...");
				newSaveNumber = 1;
				_getch();
				continue;
			}
			newSaveNumber = loadNames(SHOW);
			do
			{
				printf("\nEnter the number of the save you want: ");
				scanf("%d", &saveNumber );
			} while (saveNumber <= 0 || saveNumber >= newSaveNumber);
			for (int i = 0; i < saveNumber; i++)
			{
				char temp[50];
				fscanf(f, "%s", temp);
				for (int i = 0; i < ROWS; i++)
				{
					for (int j = 0; j < COLOMNS; j++)
					{
						for (int k = 0; k < CAPACITY; k++)
						{
							fscanf(f, "%d", &board[i][j][k]);
						}
					}
				}

				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < PIECES; j++)
					{
						fscanf(f, "%d", &playerOs[i][j]);
					}
				}

				fscanf(f, "%d%d", &point[0], &point[1]);

				for (int i = 0; i < 3; i++)
					fscanf(f, "%d%d", &holes[i][0], &holes[i][1]);

				for (int i = 0; i < 3; i++)
					fscanf(f, "%d%d", &coins[i][0], &coins[i][1]);

				fscanf(f, "%d%d%d%d%d", &playerBanned[0], &playerBanned[1], &coinsCollected[0], &coinsCollected[1], &turn);
			}
			break;
		}
		else if (t == 3)
		{
			int c = 0;
			char address[100], temp[5] = { 0 };
			int realm = 1, room = 0, piece = 0;
			FILE *fMap;
			printf("\n\nMap File Address: ");
			scanf("%s", address);
			fMap = fopen(address, "r");
			if (!fMap)
			{
				printf("Error in openning the map file , Try again!\n");
				_getch();
				continue;
			}
			while (fgetc(fMap) != '\n');
			while (!feof(fMap))
			{
				
				int i = 0;
				int index = 0;
				temp[0] = temp[1] = temp[2] = temp[3] = temp[4] = 0;
				temp[3] = ' ';
				if (realm == 0 && piece < 0 && room == 0)
				{
					break;
				}
				for (i = 0; i < 3; i++)
				{
					temp[i] = fgetc(fMap);
					if (temp[i] == '|')
						temp[i] = fgetc(fMap);
					if (temp[i] == '\n')
					{
						if (realm == 1)
							piece++;
						else
							piece--;
						break;
					}
				}
				if (piece >= CAPACITY)
				{
					piece = CAPACITY - 1;
					realm = 0;
				}
				if (temp[i] == '\n')
				{
					board[realm][room][piece] = EMPTY;
					room = 0;
					temp[0] = temp[1] = temp[2] = temp[3] = temp[4] = 0;
				}
				else
				{
					for (; index < COLOMNS + 1 && strcmp(temp, names[index]); index++);
					board[realm][room][piece] = index;
					room = (room + 1) % COLOMNS;
				}
			}
			createHolesAndCoins();
			break;
		}
	}
	_fcloseall();
	return turn;
}

void updateBoard()
{
	int outCounter = 0;
	system("cls");
	printf("Player2's Castle\t\tRoad number 2\t\t\tPlayer1 Cell | Player2 Cell\n");
	printf(" 1   2   3   4   5   6  |");
	printf(" 1   2   3   4   5   6\t\t\t\t     |\n");
	for (int i = 0; i < CAPACITY; i++)
	{
		for (int j = 0; j < COLOMNS; j++)
		{
			printf("%s", names[ board[1][j][i] ]);
			if (j == 5)
				printf("|");
		}
		printf("\t\t\t%s  |  %s\n",oNames[playerOs[0][outCounter]], oNames[playerOs[1][outCounter]]);
		outCounter++;
	}

	for (int i = CAPACITY - 1; i >= 0; i--)
	{
		for (int j = 0; j < COLOMNS; j++)
		{
			printf("%s", names[board[0][j][i]]);
			if (j == 5)
				printf("|");
		}
		printf("\t\t\t%s  |  %s\n", oNames[playerOs[0][outCounter]], oNames[playerOs[1][outCounter]]);
		outCounter++;
	}
	printf(" 1   2   3   4   5   6  |");
	printf(" 1   2   3   4   5   6   ");
	printf("\t\t\t%s  |  %s\n", oNames[playerOs[0][outCounter]], oNames[playerOs[1][outCounter]]);
	printf("Player1's Castle\t\tRoad number 1\n");
}

void sendTpUpperCase(char *s)
{
	for (int i = 0; s[i]; i++)
		if (s[i] >= 'a' && s[i] <= 'z')
			s[i] += 'A' - 'a';
}

void createHolesAndCoins()
{
	int n = rand() % 3;
	for (int i = 0; i <= n; i++)
	{
		do
		{
			holes[i][0] = rand() % ROWS;
			holes[i][1] = rand() % (COLOMNS/2) + (COLOMNS / 2);
		}while(board[holes[i][0]][holes[i][1]][0] != EMPTY);
	}
	n = rand() % 3;
	for (int i = 0; i <= n; i++)
	{
		do
		{
			coins[i][0] = rand() % ROWS;
			coins[i][1] = rand() % COLOMNS;
		} while ( board[coins[i][0]][coins[i][1]][0] != EMPTY || (coins[i][0] == holes[i][0] && coins[i][1] == holes[i][1]));
	}
}

int anyMoveLeft(int player, int dice)
{
	int r1 = 0, r2 = 0, numberOfPiecesInCastle = 0;
	for (int j = 0; j < COLOMNS; j++)
	{
		if (player == 0 )
		{
			if (playerOs[0][0] != EMPTY)
			{
				if (board[1][dice - 1][CAPACITY - 1] <= K_1 && board[1][dice - 1][CAPACITY - 1] != EMPTY)
					return 0;
				else if (board[1][dice - 1][1] >= S_2)
					return 0;
			}
			if (board[0][j][0] <= K_1 && board[0][j][0] > EMPTY)
			{
				r1 = (j + 1 - dice);
				if (r1 == 0)
					return 1;
				if (r1 > 0)
				{
					if (board[0][j - dice][1] <= K_1 && board[0][j-dice][CAPACITY-1] == EMPTY)
						return 1;
				}
			}
			if (board[1][j][0] <= K_1 && board[1][j][0] >= EMPTY)
			{
				r1 = (j + dice) % COLOMNS;
				r2 = (j + dice) / COLOMNS;
				if (r2 == 0)
				{
					if (board[1][j + dice][1] <= K_1 && board[1][ j + dice ][CAPACITY - 1] )
						return 1;
				}
				else
				{
					if (board[0][COLOMNS - r1 - 1][1] <= K_1 && board[1][j + dice][CAPACITY - 1])
						return 1;
				}
			}
		}
		else
		{
			if (playerOs[1][0] != EMPTY)
			{
				if (board[0][dice - 1][1] >= S_2 && board[0][dice - 1][CAPACITY - 1] != EMPTY)
					return 0;
				else if (board[0][dice - 1][1] <= K_1 && board[0][dice - 1][1] != EMPTY)
					return 0;
			}
			if (board[1][j][0] >= S_2)
			{
				r1 = (j + 1 - dice);
				if (r1 == 0)
					return 1;
				if (r1 > 0)
				{
					if (board[0][j - dice][1] <= K_1)
						return 1;
				}
			}
			if (board[0][j][0] >= S_2 )
			{
				r1 = (j + dice) % COLOMNS;
				r2 = (j + dice) / COLOMNS;
				if (r2 == 0)
				{
					if (board[0][j + dice][1] <= K_1)
						return 1;
				}
				else
				{
					if (board[1][COLOMNS - r1 - 1][1] <= K_1)
						return 1;
				}
			}
		}
	}

	return isPlayerCastleFull(player) && dice >= 4 && ((player == 0 && board[player][3][0] <= K_1 && board[player][3][0] > EMPTY) || (player == 1 && board[player][3][0] >= S_2));

}

void obtainMove(char *s, char *d, int *r)
{
	r[0] = 0;
	for (int i = 0; i < COLOMNS*ROWS +1; i++)
	{
		if (!strcmp(s, rooms[i]))
		{
			r[1] = i / 12;
			r[2] = i % 12;
			r[0]++;
		}
		if (!strcmp(d, rooms[i]))
		{
			r[3] = i / 12;
			r[4] = i % 12;
			r[0]++;
		}
	}
}

int selectDice(int player , int sRealm, int sRoom, int dRealm, int dRoom,  int *dices , int *matched)
{
	int step = player == sRealm ? -1 : 1 , distance,flag;

	if (sRealm == OUT)
	{
		if (dRealm == (player + 1) % 2)
		{
			if (dRoom == dices[0] - 1)
			{
				*matched = 1;
				return 0;
			}
			if (dRoom == dices[1] - 1)
			{
				*matched = 1;
				return 1;
			}
		}
		*matched = 0;
		return 0;
	}

	if (dices[0] == dices[1])
		return 0;
	if (dices[0] == USED)
		return 1;
	if (dices[1] == USED)
		return 0;

	if (sRealm == dRealm)
	{
		if (dRoom == sRoom + step*dices[0])
		{
			*matched = 1;
			return 0;
		}
		if (dRoom == sRoom + step*dices[1])
		{
			*matched = 1;
			return 1;
		}
	}
	else if ( dRealm != 2 )
	{
		if (step != -1)
		{
			distance = (COLOMNS - sRoom) + (COLOMNS - dRoom) - 1;
			if (distance == dices[0])
			{
				*matched = 1;
				return 0;
			}
			if (distance == dices[1])
			{
				*matched = 1;
				return 1;
			}
		}
	}
	else if ( dRealm == 2 )
	{
		flag = isPlayerCastleFull(player);
		if ((sRealm == player && sRoom == dices[0] - 1) || (flag == 1 && sRoom == 3 && dices[0] >= 4))
		{
			*matched = 1;
			return 0;
		}
		if ((sRealm == player && sRoom == dices[1] - 1) || (flag == 1 && sRoom == 3 && dices[1] >= 4))
		{
			*matched = 1;
			return 1;
		}
	}
	*matched = 0;
	return 0;
}

void showMsg(int player, int r, int i, int moves)
{
	char *moveOrders[4] = { "first", "second", "third", "forth" };
	if (r == WRONG_MOVE)
		printf("\nWrong move Player %d, please try again: ", player);
	else if (r == NO_MOVES)
		printf("\nplayer %d, No moves available", player);
	else if (r == 0)
		printf("\nPlayer %d, please enter your %s source and destination: ",player, moveOrders[i]);
	else if (i == moves)
		printf("\nNice move Player %d, end of your turn...", player);
	else
		printf("\nNice move Player %d , now please enter your %s source and destination: ", player, moveOrders[i]);
}
int movePlayer(int player ,int dice , int sRealm,int sRoom ,int dRealm,int dRoom)
{
	int piece, dFirstFree, distance , cellFirstFree;
	int nextPlayer = (player + 1) % 2;
	int step = player == sRealm ? -1 : 1 ;
	if (board[sRealm][sRoom][0] == EMPTY)
		return WRONG_MOVE;
	if ((player == 0 && board[sRealm][sRoom][0] >= S_2) || (player == 1 && board[sRealm][sRoom][0] <= K_1))
		return WRONG_MOVE;
	piece = findFreePiece(sRealm, sRoom);
	if (board[dRealm][dRoom][0] == EMPTY)
		dFirstFree = 0;
	else
		dFirstFree = findFreePiece(dRealm, dRoom) + 1;

	if (sRealm == dRealm)
	{
		if (dRoom == sRoom + step*dice)
		{
			if ((player == 0 && board[dRealm][dRoom][0] >= S_2) || (player == 1 && board[dRealm][dRoom][0] <= K_1 && board[dRealm][dRoom][0] > EMPTY))
			{
				
				if (board[dRealm][dRoom][1] != EMPTY)
					return WRONG_MOVE;
				else
				{
					for (cellFirstFree = 0; playerOs[nextPlayer][cellFirstFree] != EMPTY; cellFirstFree++);
					playerOs[nextPlayer][cellFirstFree] = board[dRealm][dRoom][0];
					board[dRealm][dRoom][0] = board[sRealm][sRoom][piece];
					board[sRealm][sRoom][piece] = EMPTY;
					checkForCoinRelease(nextPlayer,cellFirstFree);
				}
			}
			else if (board[dRealm][dRoom][CAPACITY - 1] != EMPTY)
				return WRONG_MOVE; 
			else
			{
				
				board[dRealm][dRoom][dFirstFree] = board[sRealm][sRoom][piece];
				board[sRealm][sRoom][piece] = EMPTY;
				
			}
		}
		else
			return WRONG_MOVE;
	}
	else
	{
		if (step == -1)
		{
			return WRONG_MOVE;
		}
		else
		{
			if ((player == 0 && board[dRealm][dRoom][0] >= S_2) || (player == 1 && board[dRealm][dRoom][0] <= K_1 && board[dRealm][dRoom][0] > EMPTY))
			{

				if (board[dRealm][dRoom][1] != EMPTY)
					return WRONG_MOVE;
				else
				{
					for (cellFirstFree = 0; playerOs[nextPlayer][cellFirstFree] != EMPTY; cellFirstFree++);
					playerOs[nextPlayer][cellFirstFree] = board[dRealm][dRoom][0];
					board[dRealm][dRoom][0] = board[sRealm][sRoom][piece];
					board[sRealm][sRoom][piece] = EMPTY;
					checkForCoinRelease(nextPlayer, cellFirstFree);
				}
			}
			else if (board[dRealm][dRoom][CAPACITY - 1] != EMPTY)
				return WRONG_MOVE;
			else
			{
				distance = (COLOMNS - sRoom) + (COLOMNS - dRoom) - 1;
				if (distance == dice)
				{
					board[dRealm][dRoom][dFirstFree] = board[sRealm][sRoom][piece];
					board[sRealm][sRoom][piece] = EMPTY;
				}
				else
					return WRONG_MOVE;
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (holes[i][0] == dRealm && holes[i][1] == dRoom)
		{
			playerBanned[player] = 2;
			printf("\nPlayer %d, There was a hole right there ! You are banned for 2 turns!\n", player+1);
			_getch();
		}
		else if (coins[i][0] == dRealm && coins[i][1] == dRoom)
		{
			coinsCollected[player]++;
			coins[i][0] = coins[i][1] = USED;
			printf("\n+1 coins collected! Player %d coins: %d\n", player+1, coinsCollected[player]);
			_getch();
		}
	}
	return DONE;
}

void checkForCoinRelease(int player,int cellFirstFree)
{
	if (coinsCollected[player] > 0)
	{
		int search;
		for (search = 0; search < COLOMNS / 2; search++)
			if (board[player][search][0] == EMPTY)
				break;
		if (search < COLOMNS / 2)
		{
			char c;
			printf("\n\nPlayer %d, do you want to use your coin? y. Yes o.w. No ", player);
			c = _getch();
			if (c == 'y' || c == 'Y')
			{
				coinsCollected[player]--;
				board[(player + 1) % 2][search][0] = playerOs[player][cellFirstFree];
				playerOs[player][cellFirstFree] = EMPTY;
			}
		}
	}
}

int findFreePiece(int realm, int room)
{
	int x = CAPACITY - 1;
	for (;x >= 0 && board[realm][room][x] == EMPTY; x--);
	return x;
}

int freeThePiece(int player , int pIndex , int dice)
{ 
	int nextPlayer = (player + 1) % 2, cellFirstFree;
	int dFirstFree;
	if (board[nextPlayer][dice - 1][0] == EMPTY)
		dFirstFree = 0;
	else
		dFirstFree = findFreePiece(nextPlayer, dice - 1) + 1;
	if (board[nextPlayer][dice - 1][CAPACITY - 1] != EMPTY)
		return WRONG_MOVE;
	if ((player == 0 && board[1][dice - 1][1] >= S_2) || (player == 1 && board[0][dice - 1][1] <= K_1 && board[0][dice - 1][1] > EMPTY))
		return WRONG_MOVE;
	if ((player == 0 && board[1][dice - 1][0] >= S_2) || (player == 1 && board[0][dice - 1][0] <= K_1 && board[0][dice - 1][0] > EMPTY))
	{
		for (cellFirstFree = 0; playerOs[nextPlayer][cellFirstFree] != EMPTY; cellFirstFree++);
		playerOs[nextPlayer][cellFirstFree] = board[nextPlayer][dice - 1][0];
		board[nextPlayer][dice - 1][0] = playerOs[player][pIndex] ;
		playerOs[player][pIndex] = EMPTY;
		checkForCoinRelease(nextPlayer, cellFirstFree);
	}
	else
	{
		board[nextPlayer][dice - 1][dFirstFree] = playerOs[player][pIndex];
		playerOs[player][pIndex] = EMPTY;
	}
	for (int i = pIndex; i < PIECES-1; i++)
	{
		playerOs[player][i] = playerOs[player][i + 1];

	}
	for (int i = 0; i < 3; i++)
	{
		if (holes[i][0] == nextPlayer && holes[i][1] == dice - 1)
		{
			playerBanned[player] = 2;
			printf("\nPlayer %d, There was a hole right there ! You are banned for 2 turns!\n", player);
		}
		else if (coins[i][0] == nextPlayer && coins[i][1] == dice - 1)
		{
			coinsCollected[player]++;
			coins[i][0] = coins[i][1] = USED;
			printf("\n+1 coins collected! Player %d coins: %d\n", player, coinsCollected[player]);
		}
	}
	return DONE;
}

int isPlayerCastleFull(int player)
{
	int numberOfPiecesInCastle = 0, flag = 0;
	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= CAPACITY && board[player][i][j] != EMPTY; j++)
		{
			if (player == 0 && board[player][i][j] > EMPTY && board[player][i][j] <= K_1)
				numberOfPiecesInCastle++;
			else if (player == 1 && board[player][i][j] >= S_2)
				numberOfPiecesInCastle++;
		}
	}
	if (numberOfPiecesInCastle == PIECES - point[player])
		flag = 1;
	return flag;
}

int movePieceToO(int player, int sRealm, int sRoom, int dice)
{
	int flag = 0;
	int piece = findFreePiece(sRealm, sRoom), numberOfPiecesInCastle = 0;
	if (board[sRealm][sRoom][0] == EMPTY)
		return WRONG_MOVE;
	if ((player == 0 && board[sRealm][sRoom][0] >= S_2) || (player == 1 && board[sRealm][sRoom][0] <= K_1))
		return WRONG_MOVE;

	flag = isPlayerCastleFull(player);
	if ( (sRealm == player && sRoom == dice - 1) || (flag == 1 && sRoom == 3 && dice >= 4) )
	{
		point[player]++;
		board[sRealm][sRoom][piece] = EMPTY;
		if (point[player] == PIECES)
		{
			printf("\n*! Player %d Won !* ");
			_getch();
			exit(0);
		}
	}
	else
		return WRONG_MOVE;
	return DONE;
}

int loadNames(int readMode)
{
	FILE *fSavedNames = fopen(savedGameNumber, "r");
	newSaveNumber = 1;
	if (!fSavedNames)
		return 1;
	if (readMode == SHOW)
		printf("\n\nSaves:\n");
	fscanf(fSavedNames, "%d", &newSaveNumber);
	newSaveNumber++;
	if (readMode == SHOW)
	{
		for (int i = 1; i < newSaveNumber; i++)
		{
			printf("%d. Shatnard%d\n",i, i);
		}
	}
	return newSaveNumber;
	fclose(fSavedNames);
}

int save(int turn)
{
	FILE *f = fopen(savedGame, "a");
	FILE *fSaveNames = fopen(savedGameNumber, "w");
	if (!f)
	{
		f = fopen(savedGame, "w");
		newSaveNumber = 1;
		if (!f || !fSaveNames)
			return SAVE_INTERRUPTED;
	}

	fprintf(f, "Shatnard%d\n", newSaveNumber);
	fprintf(fSaveNames, "%d\n", newSaveNumber);
	fclose(fSaveNames);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLOMNS; j++)
		{
			for (int k = 0; k < CAPACITY; k++)
			{
				fprintf(f, "%d\n", board[i][j][k]);
			}
		}
	}
	
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < PIECES; j++)
		{
			fprintf(f, "%d\n", playerOs[i][j]);
		}
	}

	fprintf(f, "%d\n%d\n", point[0], point[1]);

	for (int i = 0; i < 3; i++)
		fprintf(f, "%d\n%d\n", holes[i][0], holes[i][1]);

	for (int i = 0; i < 3; i++)
		fprintf(f, "%d\n%d\n", coins[i][0], coins[i][1]);

	fprintf(f, "%d\n%d\n%d\n%d\n%d\n", playerBanned[0], playerBanned[1],coinsCollected[0],coinsCollected[1],turn);
	fclose(f);
	newSaveNumber++;
	return DONE;
}