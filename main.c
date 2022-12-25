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

struct data_save{        //利用儲存棋盤的變動來儲存檔案

    //DATA
    int xi;             //棋子在棋盤上的座標
    int yi; 
    int xo;
    int yo;
    int level_up;       //是否升變
    int revive;         //是否打入
    int red_or_blue;    //紅方藍方判斷
    int eat;            //是否有吃子
    int position;       //紀錄是在吃子欄位的地方第幾個拿的
    int eat_level_down; //被吃的子是否有升過級

    //左右指標
    struct data_save *left, *right;    //接下一個

};
typedef struct data_save data_save;

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
//data_save *list;       不使用這種方法儲存了。累死我也



void BoardReset();
void BoardPrint();
void ReceiveInput();
void MoveChess(int *red_or_blue,int xi, int yi,int xo, int yo);
int ValidCheck(int chess_number,int xi, int yi,int xo, int yo);
void ChangeChessPosition(int xi, int yi,int xo, int yo)  ;
void EatChess(int xi, int yi,int xo, int yo);
int EatOrChange(int xi, int yi,int xo, int yo);
void LoadingLine();
int LevelUpCheck(int xo, int yo);
void NicePic();
int LevelUpAsk();
void LevelDown(int x, int y);
void LevelDownColorNoChange(int x, int y);
int ReviveAsk();
int ReviveCheck(int position, int xo, int yo);
void ReviveMove(int *red_or_blue,int xi, int yi,int xo, int yo);
void LevelUp(int x, int y);

board_save *CreateSavedBoard(board_save *list_of_board);
void UpDateSavedBoard(board_save *list, int red_or_blue);
int RegretAsk();
board_save *UpDateBoard(board_save *list_of_board, int *red_or_blue);

//以下資料儲存被上方取代

data_save *Add(data_save *list, int xi, int yi, int xo, int yo, int level_up, int revive, int red_or_blue, int eat, int position, int eat_level_down);
int PreviousMove(data_save *list);




int main()
{
    

    int red_or_blue = 1;           //-1代表換紅方1代表換藍方
    int initial_or_not = 1;         //判斷是否為一開始
    BoardReset();
    LoadingLine();
    board_save *list_of_board;       //儲存棋盤的list       記得不需要list_of_board = NULL; !!
    
    UpDateSavedBoard(list_of_board, red_or_blue);       //先儲存一個剛開始的棋盤
    list_of_board->left = NULL;
    
    //Game start!!
    while(win_condition == 0){
        BoardPrint();
        
        (red_or_blue == 1) ? printf("現在是藍方喔~\n") : printf("現在是紅方喔~\n");
        


        //悔棋判斷
        if(list_of_board->left != NULL) {       //如果左邊(上一步)還有就可以毀棋
            if(RegretAsk() == 1){
                list_of_board =  UpDateBoard(list_of_board, &red_or_blue);       //更新board並退回到上一個linklist
                continue;
            }
        }

        
        
        
        
        if(red_or_blue == 1){
            if(blue_catch[0].name != CHESSKUO(空)){
                if(ReviveAsk() == 1){
                    ReceiveInput();
                    ReviveMove(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');

                }else{
                    ReceiveInput();
                    MoveChess(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');
                }
            }else{
                ReceiveInput();
                MoveChess(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');
            }
                

        }else if(red_or_blue == -1){
            if(red_catch[0].name != CHESSKUO(空)){
                if(ReviveAsk() == 1){
                    ReceiveInput();
                    ReviveMove(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');
                }else{
                    ReceiveInput();
                    MoveChess(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');
                }
            }else{
                ReceiveInput();
                MoveChess(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');
            }
            
        }

        //判斷是否需要新加一個linklist(這次是否為正確輸入)
        if(reenter_or_not == 0){
            list_of_board = CreateSavedBoard(list_of_board);    //製造新的一格儲存棋盤的linklist
        }else{
            reenter_or_not = 0;     //判斷完把reenter_or_not歸零重新作為判斷依據
        }
        
        UpDateSavedBoard(list_of_board, red_or_blue * (-1));       //換下一人就儲存棋盤(這裡red_or_blue已經改了，所以要乘-1才是這次的值)
        
    }

    BoardPrint();               //最後的更新棋盤
    
    if( win_condition == 1){
        printf(" 阿蘭!!!!\n");
    }else if ( win_condition == -1){
        printf("阿紅!!!!\n");
    }


    //印出勝利圖片
    NicePic();

    return 0;
}




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

void BoardPrint()
{
    //system("clear");


    

    printf("⌓‿⌓ と⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓\n");
    int i, j =0;

    for(i = SIZE - 1;i >= 0;i--) {          //印出數字
        printf("%s",CHESSKUO(|));
        printf("%s", chinese_numbers[i]);
        
    }
    printf("\n");

    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n|");  
    
    for(i = 0;i < CATCH_SIZE;i++) {     //印出紅方吃的棋子
        printf("%s", red_catch[i].name);
         if(i % SIZE == SIZE -1 && i == CATCH_SIZE - 1) {   //印到第二行
            printf("|");  
            printf("%s", chinese_numbers[1]);
            printf("\n");
         }else if(i % SIZE == SIZE -1){     //印下一行
            printf("|");  
            printf("%s", chinese_numbers[0]);
            printf("\n");
            printf("|");            
         }else{
            printf(" ");   
         }

    }
    

    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n\n");

    for(i = SIZE - 1;i >= 0;i--) {          //印出數字
        printf("%s",CHESSKUO(|));
        printf("%s", chinese_numbers[i]);
        
    }
    printf("\n");

    for(i = 0;i < SIZE ;i++) {          //印棋盤
        
        for(j = 0;j <SIZE;j++) {        //棋盤橫線
            printf("———");
        }
        printf("\n");

        printf("|");            //第i行的第一條 '|' 

        for(j = 0;j <SIZE;j++) {        //印出棋子和格線
            printf("%s", board[i][j].name);
            printf("|");
        }
        printf("%s",chinese_numbers[i]);
        printf("\n");
    }
    for(j = 0;j <SIZE;j++) {        //棋盤最後的橫線
            printf("———");
    }
    printf("\n\n");

    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n");
    printf("|");  
    for(i = 0;i < CATCH_SIZE;i++) {     //印出藍方吃的棋子
        printf("%s", blue_catch[i].name);
        if(i % SIZE == SIZE -1 && i == CATCH_SIZE - 1) {
           printf("|");  
           printf("%s", chinese_numbers[1]);
           printf("\n");
        }else if(i % SIZE == SIZE -1){
           printf("|");  
           printf("%s", chinese_numbers[0]);
           printf("\n");
           printf("|");            
        }else{
           
           printf(" ");  
        }

    }
      
    
    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n");

    for(i = SIZE - 1;i >= 0;i--) {          //印出數字
        printf("%s",CHESSKUO(|));
        printf("%s", chinese_numbers[i]);
        
    }
    printf("\n");

}


//接收使用者輸入
void ReceiveInput()         
{
    
    
    printf("請輸入棋子所在位置行(段) : ");
    scanf("%s", chess_current_row);
    
    printf("請輸入棋子所在位置列(筋) : ");
    scanf("%s", chess_current_column);   

    printf("請輸入棋子所去位置行(段) : ");
    scanf("%s", chess_next_row); 

    printf("請輸入棋子所去位置列(筋) : ");
    scanf("%s", chess_next_column);   


}

void MoveChess(int *red_or_blue,int xi, int yi,int xo, int yo)
{
    //先座標轉換成陣列的座標
    xi--;
    xo--;
    yi = (yi - 9) * (-1) ;
    yo = (yo - 9) * (-1) ;
    
    
    if(board[xi][yi].name == CHESSKUO(空)){
        printf("輸入錯誤啦是空der!!\n");
        reenter_or_not = 1;
    }else if(*red_or_blue == 1){                 //blue
        if(board[xi][yi].name == CHESSGOOD(香)) {
            if(ValidCheck(1,xi, yi, xo, yo) == 1){              // 有 == 1 很重要 因為 如果ValidCheck == 2 也會跑到 if(ValidChenk) 裡面 好煩喔 搞好久
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo) ){
                    
                    //if(LevelUpAsk())board[xo][yo].name = CHESSGOOD(杏);
                    board[xo][yo].name = CHESSGOOD(杏);

                }   
                *red_or_blue = *red_or_blue * (-1); 
            }else if(ValidCheck(1,xi, yi, xo, yo) == 2 ) {      //如果ValidCheck回傳2就切換成EatChess
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo) ){
                    board[xo][yo].name = CHESSGOOD(杏);
                }   
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(桂) ) {
            if(ValidCheck(3,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo) ){
                    printf("i i ");
                    board[xo][yo].name = CHESSGOOD(圭);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(3,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(圭);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(銀) ) {
            if(ValidCheck(5,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(全);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(5,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(全);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("銀輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(玉) ) {
            if(ValidCheck(7,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(7,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("玉輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(金) || board[xi][yi].name == CHESSGOOD(杏) || board[xi][yi].name == CHESSGOOD(圭) || board[xi][yi].name == CHESSGOOD(全) || board[xi][yi].name == CHESSGOOD(と) ) {
            if(ValidCheck(9,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(9,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("金輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(角) ) {
            if(ValidCheck(11,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(馬);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(11,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(馬);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("角輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(飛) ) {
            if(ValidCheck(12,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(龍);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(12,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(龍);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("飛輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(步) ) {
            if(ValidCheck(13,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(と);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(13,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSGOOD(と);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("步輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(龍) ) {
            if(ValidCheck(7,xi, yi, xo, yo) == 1 || ValidCheck(11,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(7,xi, yi, xo, yo) == 2 || ValidCheck(11,xi, yi, xo, yo) == 2) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("龍輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSGOOD(馬) ) {
            if(ValidCheck(7,xi, yi, xo, yo) == 1 || ValidCheck(12,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(7,xi, yi, xo, yo) == 2 || ValidCheck(12,xi, yi, xo, yo) == 2) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("馬輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }
    } else if(*red_or_blue == -1){                //red
        if(board[xi][yi].name == CHESSBAD(香)) {
            if(ValidCheck(2,xi, yi, xo, yo) == 1){
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(杏);
                }   
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(2,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(杏);
                }   
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
            
        }else if(board[xi][yi].name == CHESSBAD(桂) ) {
            if(ValidCheck(4,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo) ){
                    printf("上面換");
                    board[xo][yo].name = CHESSBAD(圭);
                }   
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(4,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo) ){
                    printf("下面吃");
                    board[xo][yo].name = CHESSBAD(圭);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(銀) ) {
            if(ValidCheck(6,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(全);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(6,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(全);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(王) ) {
            if(ValidCheck(8,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(8,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("王輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(金) || board[xi][yi].name == CHESSBAD(杏) || board[xi][yi].name == CHESSBAD(圭) || board[xi][yi].name == CHESSBAD(全) || board[xi][yi].name == CHESSBAD(と)) {
            if(ValidCheck(10,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(10,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("金輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(角) ) {
            if(ValidCheck(11,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(馬);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(11,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(馬);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("角輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(飛) ) {
            if(ValidCheck(12,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo) ){
                    board[xo][yo].name = CHESSBAD(龍);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(12,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(龍);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("飛輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(步) ) {
            if(ValidCheck(14,xi, yi, xo, yo) == 1) {
                ChangeChessPosition(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(と);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(14,xi, yi, xo, yo) == 2 ) {
                EatChess(xi, yi, xo, yo);
                if(LevelUpCheck(xo, yo)){
                    board[xo][yo].name = CHESSBAD(と);
                }
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("步輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(龍) ) {
            if(ValidCheck(8,xi, yi, xo, yo) == 1 || ValidCheck(12,xi, yi, xo, yo) == 1) {               //王和飛的走法
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(8,xi, yi, xo, yo) == 2 ||ValidCheck(12,xi, yi, xo, yo) == 2) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("龍輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }else if(board[xi][yi].name == CHESSBAD(馬) ) {
            if(ValidCheck(11,xi, yi, xo, yo) == 1 || ValidCheck(12,xi, yi, xo, yo) == 1) {               //王和飛的走法
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else if(ValidCheck(11,xi, yi, xo, yo) == 2 ||ValidCheck(12,xi, yi, xo, yo) == 2) {
                EatChess(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("馬輸入錯誤請重新輸入喔喔喔喔喔!\n");
                reenter_or_not = 1;
            }
        }
    }else{
        printf("The coding guy is bad since he made some mistake\n");
    }

    printf("%d , %d , %d , %d, %s, %s, redorblue = %d\n", xi, yi, xo, yo, board[xi][yi].name, board[xo][yo].name, *red_or_blue);
    printf("%s, %s, validcheck = %d\n", red_catch[0].name, blue_catch[0].name, ValidCheck(2,xi, yi, xo, yo));
    scanf("%d", &xi);

}

int ValidCheck(int chess_number,int xi, int yi,int xo, int yo)           //如果沒有違規就回傳1，是吃子就回傳2
{
    int i, j;
    switch(chess_number){
        case 1:                                                     //藍_香
            if(xi > xo && yi == yo) {                               //是否向前(藍)
                for (i = xi - 1; i > xo;i--){                       //路上是否有棋子
                    if(board[i][yi].name != CHESSKUO(空)) {
                        return 0;
                    }
                }
                return EatOrChange(xi, yi, xo, yo);
                
            }else{                                                  //路徑不符合回傳0
                return 0;
            }            
            break;
        case 2:                                                     //紅_香
            if(xi < xo && yi == yo) {                               //是否向前(紅)
                for (i = xi + 1; i < xo;i++){
                    if(board[i][yi].name != CHESSKUO(空)) {
                        return 0;
                    }
                }
                return EatOrChange(xi, yi, xo, yo);
                
            }else{
                return 0;
            }            
            break;
        case 3:                                                     //藍_桂
            if(xi - 2 == xo && (max(yi, yo) - min(yi, yo)) == 1) {  //x向上2,y向左右1
                
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
            break;
        case 4:                                                     //紅_桂
            if(xi + 2 == xo && (max(yi, yo) - min(yi, yo)) == 1) {  //x向下2,y向左右1
                
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
            break;
        case 5:                                                     //藍_銀
            if( ((max(yi, yo) - min(yi, yo)) == 1 && (max(xi, xo) - min(xi, xo)) == 1) || (xi - 1 == xo && yi == yo)) {      //x和y都和原本差一(斜角) 或 x向上1 y不動 
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
            break;
        case 6:                                                     //紅_銀
            if( ((max(yi, yo) - min(yi, yo)) == 1 && (max(xi, xo) - min(xi, xo)) == 1) || (xi + 1 == xo && yi == yo)) {      //x和y都和原本差一(斜角) 或 x向下1 y不動 
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
            break;
        case 7:                                                     //藍_玉
            if( (max(yi, yo) - min(yi, yo)) == 1 || (max(xi, xo) - min(xi, xo)) == 1) {                 //斜角移動或是上下左右
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
            break;
        case 8:                                                     //紅_王
            if( (max(yi, yo) - min(yi, yo)) == 1 || (max(xi, xo) - min(xi, xo)) == 1) {                 //斜角移動或是上下左右
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
            break;
        case 9:                                                     //藍_金
            if ( ( (max(yi, yo) - min(yi, yo)) == 1 && xi == xo) || ( (max(xi, xo) - min(xi, xo)) == 1 && yi == yo) ) {     //十字動
                return EatOrChange(xi, yi, xo, yo);
            }else if ( xi - xo == 1 && (max(yi, yo) - min(yi, yo)) == 1) {                              //往前方的斜角動
                return EatOrChange(xi, yi, xo, yo);
            }else{
                return 0;
            }
            break;
        case 10:                                                    //紅_金
            if ( ( (max(yi, yo) - min(yi, yo)) == 1 && xi == xo) || ( (max(xi, xo) - min(xi, xo)) == 1 && yi == yo) ) {     //十字動
                return EatOrChange(xi, yi, xo, yo);
            }else if ( xo - xi == 1 && (max(yi, yo) - min(yi, yo)) == 1) {                              //往前方的斜角動
                return EatOrChange(xi, yi, xo, yo);
            }else{
                return 0;
            }
            break;
        case 11:                                                    //藍紅_角
            if( (max(yi, yo) - min(yi, yo)) == (max(xi, xo) - min(xi, xo)) ) {                          //斜角無限移動(xy的差值要一樣)
                if( xi < xo && yi < yo){                        //往左下
                    for(i = xi + 1, j = yi + 1;i < xo  ;i++,j++){   //斜的從小往大不等於空就return 0
                        if(board[i][j].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }else if( xi > xo && yi < yo){                       //往左上
                    for(i = xi - 1, j = yi +1;i > xo  ;i--,j++){   
                        if(board[i][j].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }else if( xi > xo && yi > yo){                       //往右上
                    for(i = xi - 1, j = yi - 1;i > xo  ;i--,j--){   
                        if(board[i][j].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }else if( xi < xo && yi > yo){                       //往右下
                    for(i = xi + 1, j = yi - 1;i < xo  ;i++,j--){   
                        if(board[i][j].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }

                return EatOrChange(xi, yi, xo, yo);
            }else{
                return 0;
            }
            break;
        // case 12:                                                    //紅_角             其實和藍_角一樣只是有case12比較對稱好理解
        //     if( (max(yi, yo) - min(yi, yo)) == (max(xi, xo) - min(xi, xo)) ) {                          //斜角無限移動(xy的差值要一樣)
        //         return EatOrChange(xi, yi, xo, yo);
        //     }else{
        //         return 0;
        //     }
        //     break;
        case 12:                                                    //藍紅_飛
            if( xi == xo || yi ==yo) {                          //其中一個xy不動，另一個隨便動，就會是十字
                if(xi < xo){
                    for (i = xi + 1; i < xo;i++){
                        if(board[i][yi].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }else if(xi > xo){
                    for (i = xo + 1; i < xi;i++){
                        if(board[i][yi].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }else if(yi > yo){
                    for (i = yo + 1; i < yi;i++){
                        if(board[xi][i].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }else if(yi < yo){
                    for (i = yi + 1; i < yo;i++){
                        if(board[xi][i].name != CHESSKUO(空)) {
                            return 0;
                        }
                    }
                }

                return EatOrChange(xi, yi, xo, yo);
            }else{
                return 0;
            }
            break;
        case 13:                                                        //藍_步
            if(xi == xo + 1 && yi == yo) {
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
        case 14:                                                        //紅_步
            if(xi == xo - 1 && yi == yo) {
                return EatOrChange(xi, yi, xo, yo);
            }else {
                return 0;
            }
        // case 14:                                                    //紅_飛         其實和藍_角一樣只是有case14比較對稱好理解
        //     if( xi == xo || yi ==yo) {                          //其中一個xy不動，另一個隨便動，就會是十字
        //         return EatOrChange(xi, yi, xo, yo);
        //     }else{
        //         return 0;
        //     }
        //     break;
        

        
    }
    return 0;
    

}


 //判斷是要吃子還是要移動還是自己撞到自己
int EatOrChange(int xi, int yi,int xo, int yo)                         
{
    

    if(board[xo][yo].color == board[xi][yi].color){                       //自己碰自己回傳0
        return 0;
    }else if(board[xo][yo].color == board[xi][yi].color * (-1)) {               //自己碰對方吃掉回傳2
        
        return 2;
    }else{
        return 1;                                           //單純移動到空地回傳1
    }


}

//把棋子換到下一個位置
void ChangeChessPosition(int xi, int yi,int xo, int yo)              
{   
    board[xo][yo] = board[xi][yi];
    board[xi][yi] = empty;

}

void EatChess(int xi, int yi,int xo, int yo)
{
    
    if(board[xo][yo].color == 1){               //藍被紅吃
         if(board[xo][yo].name == CHESSBAD(王)){
            win_condition = 1;                                          //紅色被吃藍色贏 win_condition 變 1
        }else if(board[xo][yo].name == CHESSGOOD(玉)){                   //藍色被吃紅色贏 win_condition 變 -1
            win_condition = -1;
        }
        LevelDown(xo, yo);                      //把升變的變回來，稅便變色
        red_catch[red_catch_num++] = board[xo][yo];
        board[xo][yo] = board[xi][yi];
        board[xi][yi] = empty;
        

    }else if(board[xo][yo].color == -1){               //紅被藍吃
         if(board[xo][yo].name == CHESSBAD(王)){
            win_condition = 1;                                          //紅色被吃藍色贏 win_condition 變 1
        }else if(board[xo][yo].name == CHESSGOOD(玉)){                   //藍色被吃紅色贏 win_condition 變 -1
            win_condition = -1;
        }
        LevelDown(xo, yo);                      //把升變的變回來，順便變色
        blue_catch[blue_catch_num++] = board[xo][yo];
        board[xo][yo] = board[xi][yi];
        board[xi][yi] = empty;
       
    }else {                                         //正常來說不會跑到這行
        printf("吃子還會ㄘ錯是怎樣 QQ\n");
    }
}

//跑條，純粹無聊用
void LoadingLine() 
{
    int i, j;
    for(i = 0;i <1500;i++) {
        system("clear");
        printf("|");
        for(j = 0;j < i;j+= 100) {
            printf("█");
        }
        // if ( i%4 == 0){
        //     printf("L o a d");
        // }else if( i%4 == 1){
        //     printf("l O a d\n");
        // }else if( i%4 == 2){
        //     printf("l o A d\n");
        // }else if( i%4 == 3){
        //     printf("l o a D\n");
        // }
        printf("LOADING ~");
        printf("|\n");
        
    }



}

//看看符不符和升級
int LevelUpCheck(int xo, int yo)
{
    if(board[xo][yo].color == 1){       //藍棋子靠近前3行變變變~
        if(xo <= 2 ){
            return LevelUpAsk();        //符合就問要不要
        }else{
            return 0;
        }
    }else if(board[xo][yo].color == -1) {//紅棋子靠近前3行變變變~
        if(xo >= 6)
            return LevelUpAsk();        //符合就問要不要
        else{
            return 0;
        }
    }else {
        return 0;                           //都沒滿足升變條件return 0
    }


}

void NicePic()          //印出圖片
{
    printf("⢸⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⡷⡄\n");
    printf("⢸⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⡇⠢⣀\n");
    printf("⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠈⠑⢦⡀      Are you winning son?\n");
    printf("⢸⠀⠀⠀⠀⢀⠖⠒⠒⠒⢤⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠙⢦⡀\n");
    printf("⢸⠀⠀⣀⢤⣼⣀⡠⠤⠤⠼⠤⡄⠀⠀ ⡇⠀⠀⠀⠀⠀⠀ ⠀⠙⢄\n");
    printf("⢸⠀⠀⠑⡤⠤⡒⠒⠒⡊⠙⡏⠀⢀⠀ ⡇            ⠑⠢⡄⠀\n");
    printf("⢸⠀⠀⠀⠇⠀⣀⣀⣀⣀⢀⠧⠟⠁⠀ ⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⡇⠀\n");
    printf("⢸⠀⠀⠀⠸⣀⠀⠀⠈⢉⠟⠓⠀⠀⠀⠀ ⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸\n");
    printf("⢸⠀⠀⠀⠀⠈⢱⡖⠋⠁⠀⠀⠀⠀⠀⠀ ⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸\n");
    printf("⢸⠀⠀⠀⠀⣠⢺⠧⢄⣀⠀⠀⣀⣀⠀⠀ ⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n");
    printf("⢸⠀⠀⠀⣠⠃⢸⠀⠀⠈⠉⡽⠿⠯⡆⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n");
    printf("⢸⠀⠀⣰⠁⠀⢸⠀⠀⠀⠀⠉⠉⠉⠀⠀ ⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n");
    printf("⢸⠀⠀⠣⠀⠀⢸⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸\n");
    printf("⢸⠀⠀⠀⠀⠀⢸⠀⢇⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸\n");
    printf("⢸⠀⠀⠀⠀⠀⡌⠀⠈⡆⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸\n");
    printf("⢸⠀⠀⠀⠀⢠⠃⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸\n");
    printf("⢸⠀⠀⠀⠀⢸⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠷\n");


}

//問是否升級
int LevelUpAsk()
{
    char input[3];
    printf("泥要升級泥的棋子嗎?[y/n]: ");
    scanf(" %c", input);
    if(input[0] == 'y'){
        return 1;
    }else if( input[0] == 'n') {
        return 0;
    }else {
        printf("輸入錯誤請重新輸入\n");
        return LevelUpAsk();
        
    }

}

//升變
void LevelUp(int x, int y)
{
    if(board[x][y].name == CHESSGOOD(香)){
        board[x][y].name = CHESSGOOD(杏);
    }else if(board[x][y].name == CHESSGOOD(桂)){
        board[x][y].name = CHESSGOOD(圭);
    }else if(board[x][y].name == CHESSGOOD(銀)){
        board[x][y].name = CHESSGOOD(全);
    }else if(board[x][y].name == CHESSGOOD(步)){
        board[x][y].name = CHESSGOOD(と);
    }else if(board[x][y].name == CHESSGOOD(飛)){
        board[x][y].name = CHESSGOOD(龍);
    }else if(board[x][y].name == CHESSGOOD(角)){
        board[x][y].name = CHESSGOOD(馬);
    }else if(board[x][y].name == CHESSBAD(香)){
        board[x][y].name = CHESSBAD(杏);
    }else if(board[x][y].name == CHESSBAD(桂)){
        board[x][y].name = CHESSBAD(圭);
    }else if(board[x][y].name == CHESSBAD(銀)){
        board[x][y].name = CHESSBAD(全);
    }else if(board[x][y].name == CHESSBAD(步)){
        board[x][y].name = CHESSBAD(と);
    }else if(board[x][y].name == CHESSBAD(飛)){
        board[x][y].name = CHESSBAD(龍);
    }else if(board[x][y].name == CHESSBAD(角)){
        board[x][y].name = CHESSBAD(馬);
    }

}

//可用來換顏色也可用來降級(但要注意顏色一定會換)
void LevelDown(int x, int y)
{

    //換顏色(要先換顏色再降級，不然降級玩還會換顏色)，如果是要降級的話換顏色不會觸發
    if(board[x][y].name == CHESSGOOD(香)){
        board[x][y].name = CHESSBAD(香);
    }else if(board[x][y].name == CHESSGOOD(桂)){
        board[x][y].name = CHESSBAD(桂);
    }else if(board[x][y].name == CHESSGOOD(銀)){
        board[x][y].name = CHESSBAD(銀);
    }else if(board[x][y].name == CHESSGOOD(步)){
        board[x][y].name = CHESSBAD(步);
    }else if(board[x][y].name == CHESSGOOD(飛)){
        board[x][y].name = CHESSBAD(飛);
    }else if(board[x][y].name == CHESSGOOD(角)){
        board[x][y].name = CHESSBAD(角);
    }else if(board[x][y].name == CHESSBAD(香)){
        board[x][y].name = CHESSGOOD(香);
    }else if(board[x][y].name == CHESSBAD(桂)){
        board[x][y].name = CHESSGOOD(桂);
    }else if(board[x][y].name == CHESSBAD(銀)){
        board[x][y].name = CHESSGOOD(銀);
    }else if(board[x][y].name == CHESSBAD(步)){
        board[x][y].name = CHESSGOOD(步);
    }else if(board[x][y].name == CHESSBAD(飛)){
        board[x][y].name = CHESSGOOD(飛);
    }else if(board[x][y].name == CHESSBAD(角)){
        board[x][y].name = CHESSGOOD(角);
    }

    //降級
    if(board[x][y].name == CHESSGOOD(杏)){
        board[x][y].name = CHESSBAD(香);
    }else if(board[x][y].name == CHESSGOOD(圭)){
        board[x][y].name = CHESSBAD(桂);
    }else if(board[x][y].name == CHESSGOOD(全)){
        board[x][y].name = CHESSBAD(銀);
    }else if(board[x][y].name == CHESSGOOD(と)){
        board[x][y].name = CHESSBAD(步);
    }else if(board[x][y].name == CHESSGOOD(龍)){
        board[x][y].name = CHESSBAD(飛);
    }else if(board[x][y].name == CHESSGOOD(馬)){
        board[x][y].name = CHESSBAD(角);
    }else if(board[x][y].name == CHESSBAD(杏)){
        board[x][y].name = CHESSGOOD(香);
    }else if(board[x][y].name == CHESSBAD(圭)){
        board[x][y].name = CHESSGOOD(桂);
    }else if(board[x][y].name == CHESSBAD(全)){
        board[x][y].name = CHESSGOOD(銀);
    }else if(board[x][y].name == CHESSBAD(と)){
        board[x][y].name = CHESSGOOD(步);
    }else if(board[x][y].name == CHESSBAD(龍)){
        board[x][y].name = CHESSGOOD(飛);
    }else if(board[x][y].name == CHESSBAD(馬)){
        board[x][y].name = CHESSGOOD(角);
    }

}

//不換顏色的降級(讀棋譜用，用LevelDown會有問題喔)
void LevelDownColorNoChange(int x, int y)
{
    //降級
    if(board[x][y].name == CHESSGOOD(杏)){
        board[x][y].name = CHESSGOOD(香);
    }else if(board[x][y].name == CHESSGOOD(圭)){
        board[x][y].name = CHESSGOOD(桂);
    }else if(board[x][y].name == CHESSGOOD(全)){
        board[x][y].name = CHESSGOOD(銀);
    }else if(board[x][y].name == CHESSGOOD(と)){
        board[x][y].name = CHESSGOOD(步);
    }else if(board[x][y].name == CHESSGOOD(龍)){
        board[x][y].name = CHESSGOOD(飛);
    }else if(board[x][y].name == CHESSGOOD(馬)){
        board[x][y].name = CHESSGOOD(角);
    }else if(board[x][y].name == CHESSBAD(杏)){
        board[x][y].name = CHESSBAD(香);
    }else if(board[x][y].name == CHESSBAD(圭)){
        board[x][y].name = CHESSBAD(桂);
    }else if(board[x][y].name == CHESSBAD(全)){
        board[x][y].name = CHESSBAD(銀);
    }else if(board[x][y].name == CHESSBAD(と)){
        board[x][y].name = CHESSBAD(步);
    }else if(board[x][y].name == CHESSBAD(龍)){
        board[x][y].name = CHESSBAD(飛);
    }else if(board[x][y].name == CHESSBAD(馬)){
        board[x][y].name = CHESSBAD(角);
    }


}

//詢問使用者的打入棋子並記錄
int ReviveAsk()
{
    char input[3];
    printf("想要拿取吃過的棋子嗎?[y/n]: ");
    scanf(" %c", input);
    if(input[0] == 'y'){    //回傳一表示要
        return 1;
    }else if( input[0] == 'n') {        //回傳0表示不要
        return 0;
    }else {
        printf("輸入錯誤請重新輸入\n");
        return ReviveAsk();
        
    }

}

//有沒有符合打入規則
int ReviveCheck(int position, int xo, int yo)       //檢查拿子合不合裡
{
    if(board[xo][yo].name != CHESSKUO(空)){
        return 0;
    }else if(blue_catch[position].name == CHESSGOOD(香)){  //藍_香不可以放到最後
        if(xo == 0){                                //不可以放到最後前面一排
            return 0;
        }else{
            return 1;
        }
    }else if(red_catch[position].name ==CHESSBAD(香)){
        if(xo == 8){
            return 0;
        }else{
            return 1;
        }
    }else if(blue_catch[position].name ==CHESSGOOD(桂)){
        if(xo == 0 || xo == 1){                 //桂不可放頂二段
            return 0;
        }else{
            return 1;
        }
    }else if(red_catch[position].name ==CHESSBAD(桂)){
        if(xo == 8 || xo == 7){
            return 0;
        }else{
            return 1;
        }
    }else if(blue_catch[position].name ==CHESSGOOD(步)){
        for(int i = 0;i < 8;i++){
            if(board[i][yo].name == CHESSGOOD(步)){     //那一列不能有步
                return 0;
            }
        }
        if(board[xo + 1][yo].name == CHESSBAD(王)){      //步不可瞬殺王
            return 0;
        }
        return 1;
    }else if(red_catch[position].name ==CHESSBAD(步)){
        for(int i = 0;i < 8;i++){
            if(board[i][yo].name == CHESSBAD(步)){     //那一列不能有步
                return 0;
            }
        }
        if(board[xo + 1][yo].name == CHESSGOOD(玉)){      //步不可瞬殺玉
            return 0;
        }
        return 1;
    }
    
    
}

//打入移動用函式
void ReviveMove(int *red_or_blue,int xi, int yi,int xo, int yo)
{
    //先座標轉換成陣列的座標
    xi--;
    xo--;
    yi = (yi - 9) * (-1) ;
    yo = (yo - 9) * (-1) ;
    
    int position = (xi) * 9 + yi;               //換算成一維正列的數字，給blue_catch和red_catch用
    int i;
    printf("%i\n", position);
    if(*red_or_blue == 1){       //藍
        if(( ReviveCheck(position, xo, yo) == 1 && position <= CATCH_SIZE) && blue_catch[position].name != CHESSKUO(空)){        //符合規則和不等於空和不超出位置就換
            blue_catch[position].color *= -1;               //把顏色參數也條一下，因為前面吃子時忘了調 嘿嘿
            board[xo][yo] = blue_catch[position];
            
            
            for( i = position;blue_catch[i].name == CHESSKUO(空) && i < CATCH_SIZE - 1;i++){         //把放吃子的地方排乾淨整齊(向左排)
                blue_catch[i] = blue_catch[i + 1];
            }
            blue_catch[i] = empty;            //把最後一個沒被覆蓋的吃子格蓋掉，不然會多一個子
            blue_catch_num--;                 //少了一個棋子blue_catch_num要倒退
            *red_or_blue *= -1;
        }else{
            printf("daru不符合規則喔\n");
            reenter_or_not = 1;
        }
    }else if(*red_or_blue == -1){
        if(( ReviveCheck(position, xo, yo) == 1 && position <= CATCH_SIZE) && red_catch[position].name != CHESSKUO(空)){        //符合規則就換
            red_catch[position].color *= -1;
            board[xo][yo] = red_catch[position];
            
            
            for( i = position;red_catch[i].name == CHESSKUO(空) && i < CATCH_SIZE - 1;i++){
                red_catch[i] = red_catch[i + 1];
            }
            red_catch[i] = empty;
            red_catch_num--;
            *red_or_blue *= -1;
        }else{
            printf("daru不符合規則喔\n");
            reenter_or_not = 1;
        }
    }



}


//新增一個儲存棋盤的陣列
board_save *CreateSavedBoard(board_save *list_of_board)
{
    board_save *tmp;
    tmp = (board_save*)malloc(sizeof(board_save));

    

    list_of_board->right = tmp;      //原本的接到tmp
    tmp->left =  list_of_board;      //左邊接到之前
    list_of_board = tmp;             //list_of_board 換到下一個

    return (list_of_board);
}

//儲存棋盤和換誰下棋
void UpDateSavedBoard(board_save *list, int red_or_blue)
{
    for(int i = 0; i < CATCH_SIZE;i++){
        list->saved_red_catch[i] = red_catch[i];
        list->saved_blue_catch[i] = blue_catch[i];
    }
    
    for(int i = 0; i < SIZE;i++){
        for(int j = 0; j < SIZE;j++){
            list->saved_board[i][j] = board[i][j];
        }
    }
    
    

    list->saved_red_or_blue = red_or_blue;
}

//問要不要悔棋
int RegretAsk()
{
    char input[3];
    printf("想要回到上一步嗎?[y/n]: ");
    scanf(" %c", input);
    if(input[0] == 'y'){    //回傳一表示要
        return 1;
    }else if( input[0] == 'n') {        //回傳0表示不要
        return 0;
    }else {
        printf("悔棋詢問輸入錯誤請重新輸入\n");
        return RegretAsk();
        
    }

}

//更新board並退回到上一個linklist
board_save *UpDateBoard(board_save *list_of_board, int *red_or_blue)
{
    board_save *tmp;   
    tmp = list_of_board;
    list_of_board = list_of_board->left;    //  退到上一個

    for(int i = 0; i < CATCH_SIZE;i++){
        red_catch[i] = list_of_board->saved_red_catch[i];
        blue_catch[i] = list_of_board->saved_blue_catch[i];
    }
    
    for(int i = 0; i < SIZE;i++){
        for(int j = 0; j < SIZE;j++){
            board[i][j] = list_of_board->saved_board[i][j];
        }
    }
    *red_or_blue = list_of_board->saved_red_or_blue;
    free(tmp);
    
    return (list_of_board);

}       

//===============以下拋棄因為直接存棋盤比儲存動作快===============



//每做一次動作就創造新的node來記錄(本來想一次紀錄但發現不是每個變數都是全域變數，所以暫時拋棄)
data_save *Add(data_save *list, int xi, int yi, int xo, int yo, int level_up, int revive, int red_or_blue, int eat, int position, int eat_level_down)
{
    data_save *tmp;
    tmp = (data_save*)malloc(sizeof(data_save));
    tmp->xi = xi;
    tmp->xo = xo;
    tmp->yi = yi;
    tmp->yo = yo;
    tmp->level_up = level_up;
    tmp->revive = revive;
    tmp->red_or_blue = red_or_blue;
    tmp->eat = eat;
    tmp->position  = position;
    tmp->eat_level_down = eat_level_down;
    //list 接來接去
    list->right = tmp;      //原本的接到tmp
    tmp->left =  list;      //左邊接到之前
    list = tmp;             //list 換到下一個
    return list;
}


//按照儲存的動作返回上一步
int PreviousMove(data_save *list)
{   
    

    if(list == NULL){
        printf("空了停止\n");
        return 0;               //回傳0表示終止
    }
    
    if(list->revive == 1){       //有復活
        if(list->red_or_blue == 1){     //藍方復活
            for(int i = blue_catch_num;i > list->position;i--){     //吃子欄向右移一格空出位置
                blue_catch[i] = blue_catch[i - 1];
            }
            blue_catch[list->position] = board[list->xo][list->yo];     //
            board[list->xo][list->yo] = empty;
        }else if(list->red_or_blue == -1){  //紅方復活
            for(int i = red_catch_num;i > list->position;i--){     //向右移一格
                red_catch[i] = red_catch[i - 1];
            }
            red_catch[list->position] = board[list->xo][list->yo];
            board[list->xo][list->yo] = empty;
        }
    }else if(list->revive == 0){
        if(list->level_up == 1){        //如果有升級先降級一下
            LevelDownColorNoChange(list->xo, list->yo);
        }

        if(list->eat == 0){         //沒有吃子單純換位置
            board[list->xi][list->yi] = board[list->xo][list->yo];
            board[list->xo][list->yo] = empty;
        }else if(list->eat == 1){   //有吃
            //先換
            board[list->xi][list->yi] = board[list->xo][list->yo];
            
            //在復活
            if(list->red_or_blue == 1){     //藍方復活
                board[list->xo][list->yo] = blue_catch[blue_catch_num - 1];
                if(list->eat_level_down == 1) {         //被吃的子有升級過
                    LevelUp(list->xo, list->yo);
                }
                blue_catch[blue_catch_num - 1] = empty;
                blue_catch_num--;
            }else if(list->red_or_blue == -1){  //紅方復活
                board[list->xo][list->yo] = red_catch[red_catch_num - 1];
                if(list->eat_level_down == 1) {         //被吃的子有升級過
                    LevelUp(list->xo, list->yo);
                }
                red_catch[red_catch_num - 1] = empty;
                red_catch_num--;
            }
        }

    }


}