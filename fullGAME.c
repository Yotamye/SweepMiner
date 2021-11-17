// Project SweepMiners, part B. by Yotam Yehiely

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#define ROWS 100
#define COLS 100


char howMuchBombs(char boardReal[][COLS], int num1, int num2, int tRows, int tCols);
void printBoard(char board[][COLS], int tRows, int tCols);
bool scanValid(int num1, int num2);
void aimbot(int x, char boardReal[][COLS], char boardUser[][COLS], int tRows, int tCols);
void init_With(char board[][COLS], char c, int tRows, int tCols);
void init_Numbers(char board[][COLS], int tRows, int tCols);
void updateBoardSize(int BoardChoose, int* tRows, int* tCols);
void init_Bombs(char board[][COLS], int tRows, int tCols);
int floodFill(char boardReal[][COLS], char boardUser[][COLS], int tRows, int tCols, int curRow, int curCol);


void main() {

	int boardChoose, index1, index2, boxCount = 0;
	int tRows, tCols;
	char letter;

	bool alive = true, valid;

	char boardUser[ROWS][COLS]; //creating the boards
	char boardReal[ROWS][COLS];

	printf("Please choose board \n1. 8X8\n2. 12X12\n3. 15X15\n4. Free Choose\n0. Exit\n");
	scanf("%d", &boardChoose);

	while (boardChoose < 0 || boardChoose > 4)
	{
		printf("Please enter valid choice:\n");
		scanf("%d", &boardChoose);
	}
	if (boardChoose != 0) {
		updateBoardSize(boardChoose, &tRows, &tCols);

		init_Bombs(boardReal, tRows, tCols);		// initialize board with bombs
		init_With(boardUser, 'X', tRows, tCols);	// initialize  user board with X
		init_Numbers(boardReal, tRows, tCols);	// initialize board with numbers
		printf("\n");
		printBoard(boardUser, tRows, tCols);		// print the currect user board
		printf("\n");


		while (alive && boxCount < (tRows * tCols) - sqrt(tRows * tCols)) // are we still in the game?
		{
			valid = false;
			printf("Please enter dot, and O/F :\n");
			scanf("%d%d %c", &index1, &index2, &letter);

			while (!valid)
			{
				if (index1 >= 0 && index1 <= tRows - 1 && index2 >= 0 && index2 <= tCols - 1) // index are valid?
					valid = true;
				else if (index1 == -1 && index2 > 0) // index for cheat is valid?
					valid = true;
				else if (letter == 'o' || letter == 'O' || letter == 'f' || letter == 'F') // open or flag
					valid = true;

				else
				{
					printf("Please enter a valid dot:\n");
					scanf("%d%d %c", &index1, &index2, &letter);
				}
			}

			if (index1 == -1) // cares for the cheat
			{
				aimbot(index2, boardReal, boardUser, tRows, tCols);
				printBoard(boardUser, tRows, tCols);
				boxCount = boxCount + index2;
			}

			else				// valid indexes and not cheat
			{
				if (boardUser[index1][index2] != 'X' && boardUser[index1][index2] != 'F') // if we scan the same dot
				{
					printf("Alredy filled! \n");
				}
				else
				{
					if (letter == 'o' || letter == 'O') {

						boxCount += floodFill(boardReal, boardUser, tRows, tCols, index1, index2); // open dots and count opnening dots moves

						printBoard(boardUser, tRows, tCols);
						if (boardReal[index1][index2] == 'B')			 // if he landed on a bomb
							alive = false;

					}
					if (letter == 'f' || letter == 'F') {
						boardUser[index1][index2] = 'F';		// update user board with F
						printf("\n");
						printBoard(boardUser, tRows, tCols);
					}
				}

			}
		}

		if (!alive) {											// game finishes
			printf(" YOU ARE DEAD\n");// :(
			printBoard(boardReal, tRows, tCols);
		}
		else
			printf("\n TOTAHHHHH\n"); // :)
	}
}

/////////////////////////////////////////////////////////////////////////////////////

void printBoard(char board[][COLS], int tRows, int tCols) {

	// print a specific board

	int i;

	printf("    ");						// print the uppr line
	for (i = 0; i < tCols; i++)
		printf(" %2d ", i);

	printf("\n");

	for (int i = 0; i < tRows; i++) {		// print the board
		printf(" %2d |", i);
		for (int j = 0; j < tCols; j++) {
			printf(" %c  ", board[i][j]);

		}
		printf("\n");

	}
}
char howMuchBombs(char boardReal[][COLS], int num1, int num2,int tRows, int tCols) {

	// calculate for a single dot, how many bombs around her

	int count = 0;
	for (int i = num1 - 1; i <= num1 + 1; i++) {
		for (int j = num2 - 1; j <= num2 + 1; j++) {
			if (i >= 0 && i <= tRows && j >= 0 && j <= tCols)
				if (boardReal[i][j] == 'B')
					count++;
		}
	}
	if (count == 0)
		return ' ';

	return count + '0';
}
bool scanValid(int num1, int num2) {

	// check valiation of dot

	if (num1 < 0 || num1 > 7 || num2 < 0 || num2 > 7)
		return false;
	return true;
}
void aimbot(int x, char boardReal[][COLS], char boardUser[][COLS],int tRows, int tCols) {

	// expose board from existing point untill  x 

	int count = 0;
	for (int i = 0; i < tRows && count < x; i++) {
		for (int j = 0; j < tCols && count < x; j++) {
			if (boardReal[i][j] != 'B' && boardUser[i][j] == 'X') {
				count++;
				boardUser[i][j] = boardReal[i][j];
			}
		}
	}
}
void init_With(char board[][COLS], char c,int tRows, int tCols) {
	int i, j;

	for (i = 0; i < tRows; i++)
		for (j = 0; j < tCols; j++)
			board[i][j] = c;
}
void init_Numbers(char board[][COLS], int tRows, int tCols) {

	// initialize real board with numbers

	for (int i = 0; i < tRows; i++)
		for (int j = 0; j < tCols; j++) {
			if (board[i][j] != 'B')
				board[i][j] = howMuchBombs(board, i, j,tRows, tCols);
		}
}
void init_Bombs(char board[][COLS], int tRows, int tCols) {

	// initialize board with bombs

	int bombsNumber = sqrt(tRows * tCols);
	int num1, num2, i;
	srand(time(NULL));

	for (i = 0; i < bombsNumber;) {
		num1 = rand() % tRows;
		num2 = rand() % tCols;
		if (board[num1][num2] != 'B')
		{
			board[num1][num2] = 'B';
			i++;
		}
	}
}
void updateBoardSize(int BoardChoose, int* tRows, int* tCols) 
{
	if (BoardChoose == 1)
	{
		*tRows = 8;
		*tCols = 8;
	}
	else if (BoardChoose == 2)
	{
		*tRows = 12;
		*tCols = 12;
	}
	else if (BoardChoose == 3)
	{
		*tRows = 15;
		*tCols = 15;
	}
	else if (BoardChoose == 4)
	{
		printf("Please enter your board size by entering row number and than cols number(between 0-22, 0-22) :\n");
		scanf("%d%d", tRows, tCols);
	}
}
int floodFill(char boardReal[][COLS],char boardUser[][COLS], int tRows, int tCols, int curRow, int curCol) { // expose all the empty dots untill it reaches to a number
	int count = 0;

	if (boardReal[curRow][curCol] != ' ' && (boardUser[curRow][curCol] == 'X' || boardUser[curRow][curCol] == 'F')) 
	{
		boardUser[curRow][curCol] = boardReal[curRow][curCol];
		count++;
	}
	else if (boardReal[curRow][curCol] == ' ' && (boardUser[curRow][curCol] == 'X' || boardUser[curRow][curCol] == 'F'))
	{
		count++;
		for (int i = curRow - 1; i <= curRow + 1; i++) {
			for (int j = curCol - 1; j <= curCol + 1; j++)
			{
				if (i >= 0 && i <= tRows && j >= 0 && j <= tCols)
				{
					boardUser[curRow][curCol] = boardReal[curRow][curCol];
					count += floodFill(boardReal, boardUser, tRows, tCols, i, j);

				}

			}

		}
	}
	return count;
}