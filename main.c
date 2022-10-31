#include <stdio.h>
#include <stdlib.h>


#define SIZE 9              //鄭烈的大小， 可隨時更改
#define CATCH_SIZE 18      //抓到棋子的陣列大小
#define CHESSBAD(content) "\033[1;40;31m"#content"\033[0m"
#define CHESSGOOD(content) "\033[1;40;34m"#content"\033[0m"
#define CHESSKUO(content) "\033[8;40;37m"#content"\033[0m"      //8把字隱形happy 
#define CHINESE(content) "\033[1;40;35m"#content"\033[0m"



char *board[SIZE][SIZE];        //棋盤上棋子的位置
char *red_catch[CATCH_SIZE];             //紅方抓到的棋子
int red_catch_num = 0;                  //紅方抓到棋子的數量
char *blue_catch[CATCH_SIZE];           //藍方抓到的棋子
int blue_catch_num = 0;                 //藍方抓到棋子的數量
char *chinese_numbers[SIZE];             //一二
char chess_current_row[10];                   //使用者輸入
char chess_current_column[10];
char chess_next_row[10];
char chess_next_column[10];

void BoardReset();
void BoardPrint();
void ReceiveInput();
void MoveChess(int *red_or_blue,int xi, int yi,int xo, int yo);
int ValidCheck(int chess_number,int xi, int yi,int xo, int yo);
void ChangeChessPosition(int xi, int yi,int xo, int yo)  ;




void BoardReset() 
{
    int i , j = 0;

    for(i = 0;i < CATCH_SIZE;i++) {        //清空抓到ㄉ棋子嘿嘿
        //red_catch[i] = CHESSBAD(空);
        //blue_catch[i] = CHESSGOOD(空);

        red_catch[i] = CHESSKUO(空);
        blue_catch[i] = CHESSKUO(空);
    }


    for(i = 0;i < SIZE;i++) {           //清空棋盤
        for (j = 0;j < SIZE;j++) {
            board[i][j] = CHESSKUO(空);

        }
    }

    //放棋子
    board[0][0] = board[0][8] = CHESSBAD(香);
    board[8][0] = board[8][8] = CHESSGOOD(香);   

    board[0][1] = board[0][7] = CHESSBAD(桂);
    board[8][1] = board[8][7] = CHESSGOOD(桂);   

    board[0][2] = board[0][6] = CHESSBAD(銀);
    board[8][2] = board[8][6] = CHESSGOOD(銀);

    board[0][3] = board[0][5] = CHESSBAD(金);
    board[8][3] = board[8][5] = CHESSGOOD(金);

    board[0][4] = CHESSBAD(玉);
    board[8][4] = CHESSGOOD(玉);

    board[1][7] = CHESSBAD(角);
    board[7][1] = CHESSGOOD(角);

    board[1][1] = CHESSBAD(飛);
    board[7][7] = CHESSGOOD(飛);

    for( i = 0;i < SIZE;i++) {
        board[2][i] = CHESSBAD(步);
        board[6][i] = CHESSGOOD(步);
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
    system("clear");

    printf("⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓ ⌓‿⌓\n");
    int i, j =0;
    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n|");  
    for(i = 0;i < CATCH_SIZE;i++) {     //印出紅方吃的棋子
        printf("%s", red_catch[i]);
         if(i % SIZE == SIZE -1 && i == CATCH_SIZE - 1) {
            printf("|");  
            printf("\n");
         }else if(i % SIZE == SIZE -1){
            printf("|");  
            printf("\n");
            printf("|");            
         }else{
            printf("%s",CHESSKUO(|));   
         }

    }
    

    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n\n");

    for(i = SIZE - 1;i >= 0;i--) {
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
            printf("%s", board[i][j]);
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
        printf("%s", blue_catch[i]);
        if(i % SIZE == SIZE -1 && i == CATCH_SIZE - 1) {
           printf("|");  
           printf("\n");
        }else if(i % SIZE == SIZE -1){
           printf("|");  
           printf("\n");
           printf("|");            
        }else{
           printf("%s",CHESSKUO(|));  
        }

    }
      
    
    for(j = 0;j <SIZE;j++) {        //棋盤橫線
        printf("———");
    }
    printf("\n");
}

//scanf("%s", red_catch[red_chatch_num++]);

/*void ReceiveInputTest()
{
    char input_order[20];
    scanf("%s", input_order);
    if(input_order[0] == 'G')
        printf("溪小媽\n");
    printf("input_order is : %s\n", input_order);


}
*/

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
    
    
    if(board[xi][yi] == CHESSKUO(空)){
        printf("輸入錯誤啦是空der!!\n");
    }else if(*red_or_blue == 1){                 //blue
        if(board[xi][yi] == CHESSGOOD(香)) {
            if(ValidCheck(1,xi, yi, xo, yo) ){
                ChangeChessPosition(xi, yi, xo, yo);   
                *red_or_blue = *red_or_blue * (-1); 
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
            }
        }else if(board[xi][yi] == CHESSGOOD(桂) ) {
            if(ValidCheck(3,xi, yi, xo, yo)) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
            }
        }
    }else if(*red_or_blue == -1){                //red
        if(board[xi][yi] == CHESSBAD(香)) {
            if(ValidCheck(2,xi, yi, xo, yo) ){
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
            }
            
        }else if(board[xi][yi] == CHESSBAD(桂) ) {
            if(ValidCheck(4,xi, yi, xo, yo)) {
                ChangeChessPosition(xi, yi, xo, yo);
                *red_or_blue = *red_or_blue * (-1);
            }else {
                printf("輸入錯誤請重新輸入喔喔喔喔喔!\n");
            }
        }
    }else{
        printf("some thing goes roung in MoveChess\n");
    }

    printf("%d , %d , %d , %d, %s, %s, redorblue = %d\n", xi, yi, xo, yo, board[xi][yi], board[xo][yo], *red_or_blue);
    scanf("%d", &xi);

}

int ValidCheck(int chess_number,int xi, int yi,int xo, int yo)           //如果沒有違規就回傳1，是吃子就回傳2
{
    int i;
    switch(chess_number){
        case 1:
            if(xi > xo && yi == yo) {                               //是否向前(藍)
                for (i = xi - 1; i > xo;i--){                       //路上是否有棋子
                    if(board[i][yi] != CHESSKUO(空)) {
                        return 0;
                    }
                }
                
                return 1;
            }else{
                return 0;
            }            
            break;
        case 2:
            if(xi < xo && yi == yo) {                               //是否向前(紅)
                for (i = xi + 1; i < xo;i++){
                    if(board[i][yi] != CHESSKUO(空)) {
                        return 0;
                    }
                }
                return 1;
            }else{
                return 0;
            }            
            break;
        
        
    }
    return 0;
    

}

void ChangeChessPosition(int xi, int yi,int xo, int yo)              //把棋子換到下一個位置
{   
    board[xo][yo] = board[xi][yi];
    board[xi][yi] = CHESSKUO(空);
}


int main()
{
    int red_or_blue = 1;           //-1代表換紅方1代表換藍方
    BoardReset();
    while(1){

        BoardPrint();
        ReceiveInput();
        MoveChess(&red_or_blue,chess_current_row[0] - '0', chess_current_column[0] - '0', chess_next_row[0] - '0', chess_next_column[0] - '0');      //把使用者輸入的座標轉換成int 丟進去
        
        if(chess_current_row[0] == 'w'){
            break;
        }

        
    }
    
    //ReceiveInputTest();
    printf("ZA WALUDO!!!!!!\n");


    return 0;
}


