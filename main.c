#include <stdio.h>
#include <stdlib.h>


#define SIZE 9              //鄭烈的大小， 可隨時更改
#define chessBAD(content) "\033[1;40;31m"#content"\033[0m"
#define chessGOOD(content) "\033[1;40;34m"#content"\033[0m"
#define chesskuo(content) "\033[8;40;37m"#content"\033[0m"      //8把字隱形happy 



char *board[SIZE][SIZE];

void boardReset() 
{
    int i , j = 0;

    for(i = 0;i < SIZE;i++) {
        for (j = 0;j < SIZE;j++) {
            board[i][j] = chesskuo(空);

        }
    }

    board[0][0] = board[0][8] = chessBAD(香);
    board[8][0] = board[8][8] = chessGOOD(香);   

    board[0][1] = board[0][7] = chessBAD(桂);
    board[8][1] = board[8][7] = chessGOOD(桂);   

    board[0][2] = board[0][6] = chessBAD(銀);
    board[8][2] = board[8][6] = chessGOOD(銀);

    board[0][3] = board[0][5] = chessBAD(金);
    board[8][3] = board[8][5] = chessGOOD(金);

    board[0][4] = chessBAD(玉);
    board[8][4] = chessGOOD(玉);

    board[1][7] = chessBAD(角);
    board[7][1] = chessGOOD(角);

    board[1][1] = chessBAD(飛);
    board[7][7] = chessGOOD(飛);

    for( i = 0;i < SIZE;i++) {
        board[2][i] = chessBAD(步);
        board[6][i] = chessGOOD(步);
    }

}

void boradPrint()
{
    system("clear");

    int i, j =0;

    for(i = 0;i < SIZE ;i++) {
        
        for(j = 0;j <SIZE;j++) {
            printf("———");
        }
        printf("\n");

        printf("|");

        for(j = 0;j <SIZE;j++) {
            printf("%s", board[i][j]);
            printf("|");
        }
        printf("\n");
    }

}


int main()
{


    boardReset();
    boradPrint();



    return 0;
}


