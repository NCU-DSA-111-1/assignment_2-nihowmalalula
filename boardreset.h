

#include <stdio.h>
#include <stdlib.h>

#define SIZE 9              //鄭烈的大小， 可隨時更改
#define CATCH_SIZE 18      //抓到棋子的陣列大小
#define CHESSBAD(content) "\033[1;40;31m"#content"\033[0m"
#define CHESSGOOD(content) "\033[1;40;34m"#content"\033[0m"
#define CHESSKUO(content) "\033[8;40;37m"#content"\033[0m"      //8把字隱形happy 
#define CHINESE(content) "\033[1;40;35m"#content"\033[0m"
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


typedef struct {
    char *name;     //棋子的名稱
    int color;      //藍棋是1紅棋是-1
} board_property;




struct board_save{        //直接儲存棋盤來儲存檔案
    //把棋盤和兩方吃子的地方存起來
    board_property saved_red_catch[CATCH_SIZE];
    board_property saved_board[SIZE][SIZE];
    board_property saved_blue_catch[CATCH_SIZE];

    //把輪到誰下棋儲存起來
    int saved_red_or_blue;


    //左右指標
    struct board_save *left, *right;    //接下一個

};
typedef struct board_save board_save;





board_property empty = {
    .name = CHESSKUO(空),
    .color = 0
};

board_property board[SIZE][SIZE];        //棋盤上棋子的位置
board_property red_catch[CATCH_SIZE];             //紅方抓到的棋子
int red_catch_num;                  //紅方抓到棋子的數量
board_property blue_catch[CATCH_SIZE];           //藍方抓到的棋子
int blue_catch_num;                 //藍方抓到棋子的數量
char *chinese_numbers[SIZE];             //一二
char chess_current_row[10];                   //使用者輸入
char chess_current_column[10];
char chess_next_row[10];
char chess_next_column[10];
int win_condition = 0;              //判定勝負的變數 0是對局進行 1是藍贏了 -1 是紅贏了
int reenter_or_not = 0;             //判定此次是否為無效動作，如果是無效(輸入不和規則)就是1，反之正確就是0。



void BoardReset() 
{
    int i , j = 0;

    red_catch_num = blue_catch_num = 0;

    for(i = 0;i < CATCH_SIZE;i++) {        //清空抓到ㄉ棋子嘿嘿
        

        red_catch[i] = empty;
        blue_catch[i] = empty;
    }


    for(i = 0;i < SIZE;i++) {           //清空棋盤
        for (j = 0;j < SIZE;j++) {
            board[i][j] = empty;
        }
    }

    //放棋子
    board[0][0].name = board[0][8].name = CHESSBAD(香);
    board[8][0].name = board[8][8].name = CHESSGOOD(香);   

    board[0][1].name = board[0][7].name = CHESSBAD(桂);
    board[8][1].name = board[8][7].name = CHESSGOOD(桂);   

    board[0][2].name = board[0][6].name = CHESSBAD(銀);
    board[8][2].name = board[8][6].name = CHESSGOOD(銀);

    board[0][3].name = board[0][5].name = CHESSBAD(金);
    board[8][3].name = board[8][5].name = CHESSGOOD(金);

    board[0][4].name = CHESSBAD(王);
    board[8][4].name = CHESSGOOD(玉);

    board[1][7].name = CHESSBAD(角);
    board[7][1].name = CHESSGOOD(角);

    board[1][1].name = CHESSBAD(飛);
    board[7][7].name = CHESSGOOD(飛);

    for( i = 0;i < SIZE;i++) {
        board[2][i].name = CHESSBAD(步);
        board[6][i].name = CHESSGOOD(步);
    }


    //分配顏色
    board[0][0].color = board[0][8].color = -1;
    board[8][0].color = board[8][8].color = 1;   

    board[0][1].color = board[0][7].color = -1;
    board[8][1].color = board[8][7].color = 1;   

    board[0][2].color = board[0][6].color = -1;
    board[8][2].color = board[8][6].color = 1;

    board[0][3].color = board[0][5].color = -1;
    board[8][3].color = board[8][5].color = 1;

    board[0][4].color = -1;
    board[8][4].color = 1;

    board[1][7].color = -1;
    board[7][1].color = 1;

    board[1][1].color = -1;
    board[7][7].color = 1;

    for( i = 0;i < SIZE;i++) {
        board[2][i].color = -1;
        board[6][i].color = 1;
    }

    chinese_numbers[0] = CHINESE(一);
    chinese_numbers[1] = CHINESE(二);
    chinese_numbers[2] = CHINESE(三);
    chinese_numbers[3] = CHINESE(四);
    chinese_numbers[4] = CHINESE(五);
    chinese_numbers[5] = CHINESE(六);
    chinese_numbers[6] = CHINESE(七);
    chinese_numbers[7] = CHINESE(八);
    chinese_numbers[8] = CHINESE(九);
    

    
}


