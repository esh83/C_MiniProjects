/* TIC TAC TOE GAME DEVELOPED BY EHSAN SHAFIEE
GITHUB REPO : https://github.com/esh83/TicTacToe_C */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum
{
    CROSS,
    NAUGHT,
    EMPTY,
    BORDER
};
enum
{
    DRAW,
    WIN_CROSS,
    WIN_NAUGHT,
    CONTINUE
};
int convertTo25[10] = {0, 6, 7, 8, 11, 12, 13, 16, 17, 18};
int directions[4] = {1, 5, 4, 6};
char symbols[4] = {'X', 'O', '_', 'B'};
void printBoard(const int *);
void runGame(int *);
int getComputerMove(int *, int);
int foundInDir(int *, int, int, int);
int isThreeInRow(int *, int, int);
int hasEmpty(int *);
int gameStatus(int *, int, int);
int bestMove(int *);
int getWinningSquare(int *, int);
void resetBoard(int *);
int main()
{
    srand(time(NULL));
    int board[25];
    resetBoard(board);
    runGame(board);
    return 0;
}

void resetBoard(int *board)
{
    for (int i = 0; i < 25; i++)
    {
        board[i] = BORDER;
    }
    for (int i = 1; i <= 9; i++)
    {
        board[convertTo25[i]] = EMPTY;
    }
}
void printBoard(const int *board)
{
    printf("BOARD :\n\n");
    for (int i = 1; i <= 9; i++)
    {
        printf("%4c", symbols[board[convertTo25[i]]]);
        if (i % 3 == 0)
        {
            printf("\n\n");
        }
    }
}

int getWinningSquare(int *board, int player)
{
    for (int i = 1; i <= 9; i++)
    {
        if (board[convertTo25[i]] == EMPTY)
        {
            board[convertTo25[i]] = player;
            if (isThreeInRow(board, convertTo25[i], player))
            {
                board[convertTo25[i]] = EMPTY;
                return i;
            }
            board[convertTo25[i]] = EMPTY;
        }
    }
    return -1;
}
int bestMove(int *board)
{
    if (board[convertTo25[5]] == EMPTY)
        return 5;
    int corners[4] = {1, 3, 7, 9};
    for (int i = 0; i < 4; i++)
    {
        if (board[convertTo25[corners[i]]] == EMPTY)
        {
            return corners[i];
        }
    }
    return -1;
}

int getComputerMove(int *board, int intensity)
{
    if (intensity != 1)
    {
        // try to win
        int mv = getWinningSquare(board, NAUGHT);
        if (mv != -1)
            return mv;
        // try to block opponent from win
        mv = getWinningSquare(board, CROSS);
        if (mv != -1)
            return mv;
        mv = bestMove(board);
        if (mv != -1)
            return mv;
    }

    int emptySqaures[9];
    int emptyNum = 0;
    for (int i = 1; i <= 9; i++)
    {
        if (board[convertTo25[i]] == EMPTY)
        {
            emptySqaures[emptyNum++] = i;
        }
    }
    int randIndex = rand() % emptyNum;
    return emptySqaures[randIndex];
}

int foundInDir(int *board, int sqaure, int player, int dir)
{
    int found = 0;
    while (board[sqaure] == player)
    {
        found++;
        sqaure += dir;
    }
    return found;
}
int hasEmpty(int *board)
{
    int has = 0;
    for (int i = 1; i <= 9; i++)
    {
        if (board[convertTo25[i]] == EMPTY)
            return 1;
    }
    return 0;
}
int isThreeInRow(int *board, int sqaure, int player)
{
    int counts = 1;
    for (int i = 0; i < 4; i++)
    {
        int dir = directions[i];
        counts += foundInDir(board, sqaure + dir, player, dir);
        counts += foundInDir(board, sqaure - dir, player, -1 * dir);
        if (counts == 3)
        {
            return 1;
        }
        counts = 1;
    }
    return 0;
}
int gameStatus(int *board, int player, int square)
{
    if (isThreeInRow(board, square, player))
    {
        if (player == NAUGHT)
            return WIN_NAUGHT;
        else
            return WIN_CROSS;
    }
    if (!hasEmpty(board))
        return DRAW;
    return CONTINUE;
}
void runGame(int *board)
{
    int status = CONTINUE;
    int intensity = 0;
    char input[3];
    int gameExit = 0;
    do
    {
        resetBoard(board);
        do
        {
            printf("choose one:\n1:easy\n2:hard\n");
            fgets(input, 2, stdin);
            fflush(stdin);
            if (sscanf(input, "%d", &intensity) != 1)
            {
                printf("invalid input !\n");
                continue;
            }
            if (intensity != 1 && intensity != 2)
            {
                printf("invalid input !\n");
                continue;
            }
            break;

        } while (1);

        printf("GAME GUIDE:\nthe index of board square is as follows:\n1  2  3\n4  5  6\n7  8  9\nyou are \'%c\' and game is %s\n", symbols[CROSS], intensity == 2 ? "hard" : "easy");
        do
        {
            // GAME START
            //  HUMAN IS CROSS AND COMP IS NAUGHT
            puts("enter a number between 1-9:");
            fgets(input, 2, stdin);
            fflush(stdin);
            int move;
            if (sscanf(input, "%d", &move) != 1)
            {
                printf("invalid input !\n");
                continue;
            }
            if (move < 1 || move > 9)
            {
                printf("invalid input !\n");
                continue;
            }
            if (board[convertTo25[move]] != EMPTY)
            {
                printf("this square has already chosen !\n");
                continue;
            }
            // HUMAN MOVE
            board[convertTo25[move]] = CROSS;
            status = gameStatus(board, CROSS, convertTo25[move]);
            if (status != CONTINUE)
                break;

            // COMP MOVE
            int compMove = convertTo25[getComputerMove(board, intensity)];
            board[compMove] = NAUGHT;
            status = gameStatus(board, NAUGHT, compMove);
            if (status != CONTINUE)
                break;
            printBoard(board);

        } while (1);
        printBoard(board);
        switch (status)
        {
        case DRAW:
            printf("result : draw !\n");
            break;
        case WIN_NAUGHT:
            printf("result : \'O\' win !\n");
            break;
        case WIN_CROSS:
            printf("result : \'X\' win !\n");
            break;
        }
        char playAgain;
        do
        {
            printf("do you wanna play agin ? y/n\n");
            fgets(input, 2, stdin);
            fflush(stdin);
            if (sscanf(input, "%c", &playAgain) != 1)
            {
                printf("invalid input !\n");
                continue;
            }
            if (playAgain != 'y' && playAgain != 'n' && playAgain != 'Y' && playAgain != 'N')
            {
                printf("invalid input !\n");
                continue;
            }
            break;
        } while (1);
        if (playAgain == 'n' || playAgain == 'N')
            gameExit = 1;
    } while (!gameExit);
}