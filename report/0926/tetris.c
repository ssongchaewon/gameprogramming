#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 20

int board[HEIGHT][WIDTH] = {0};
int x = 4, y = 0;      // 현재 블록 위치
int shape[4][4];       // 현재 블록
int blockType;         // 블록 종류

// 7가지 블록 (4x4)
int blocks[7][4][4] = {
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},   // I

    {{0,0,0,0},
     {0,1,1,0},
     {0,1,1,0},
     {0,0,0,0}},   // O

    {{0,0,0,0},
     {0,1,0,0},
     {1,1,1,0},
     {0,0,0,0}},   // T

    {{0,0,0,0},
     {0,1,1,0},
     {1,1,0,0},
     {0,0,0,0}},   // S

    {{0,0,0,0},
     {1,1,0,0},
     {0,1,1,0},
     {0,0,0,0}},   // Z

    {{0,0,0,0},
     {1,0,0,0},
     {1,1,1,0},
     {0,0,0,0}},   // J

    {{0,0,0,0},
     {0,0,1,0},
     {1,1,1,0},
     {0,0,0,0}}    // L
};

// 콘솔 커서 이동
void gotoxy(int x, int y) {
    COORD Pos = {x*2, y}; // *2: 가로 폭 맞춤
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

// 화면 출력
void drawBoard() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 1) printf("■");
            else printf(" .");
        }
        printf("\n");
    }
}

// 현재 블록 출력
void drawShape(int x, int y, int shape[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (shape[i][j] == 1) {
                gotoxy(x+j, y+i);
                printf("■");
            }
}

// 충돌 체크
int checkCollision(int nx, int ny, int s[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (s[i][j]) {
                int X = nx + j, Y = ny + i;
                if (X < 0 || X >= WIDTH || Y >= HEIGHT) return 1;
                if (Y >= 0 && board[Y][X]) return 1;
            }
    return 0;
}

// 블록 고정
void fixBlock(int x, int y, int s[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (s[i][j]) {
                board[y+i][x+j] = 1;
            }
}

// 줄 제거
void clearLines() {
    for (int i = HEIGHT-1; i >= 0; i--) {
        int full = 1;
        for (int j = 0; j < WIDTH; j++)
            if (!board[i][j]) { full = 0; break; }
        if (full) {
            for (int k = i; k > 0; k--)
                for (int j = 0; j < WIDTH; j++)
                    board[k][j] = board[k-1][j];
            for (int j = 0; j < WIDTH; j++) board[0][j] = 0;
            i++; // 같은 줄 다시 검사
        }
    }
}

// 블록 회전
void rotate(int s[4][4]) {
    int tmp[4][4];
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            tmp[j][3-i] = s[i][j];
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            s[i][j] = tmp[i][j];
}

// 새 블록 생성
void newBlock() {
    blockType = rand() % 7;
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            shape[i][j] = blocks[blockType][i][j];
    x = 3; y = 0;
}

int main() {
    srand(time(NULL));
    newBlock();

    while (1) {
        drawBoard();
        drawShape(x, y, shape);

        Sleep(300); // 자동 낙하 속도

        // 이동
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) key = _getch(); // 화살표
            if (key == 75 && !checkCollision(x-1,y,shape)) x--; // ←
            if (key == 77 && !checkCollision(x+1,y,shape)) x++; // →
            if (key == 80 && !checkCollision(x,y+1,shape)) y++; // ↓
            if (key == 72) { // ↑ 회전
                int temp[4][4];
                for (int i=0;i<4;i++) for (int j=0;j<4;j++) temp[i][j]=shape[i][j];
                rotate(shape);
                if (checkCollision(x,y,shape)) { // 충돌 시 복구
                    for (int i=0;i<4;i++) for (int j=0;j<4;j++) shape[i][j]=temp[i][j];
                }
            }
            if (key == 27) break; // ESC
        }

        // 자동 낙하
        if (!checkCollision(x,y+1,shape)) {
            y++;
        } else {
            fixBlock(x,y,shape);
            clearLines();
            newBlock();
            if (checkCollision(x,y,shape)) {
                system("cls");
                printf("Game Over!\n");
                break;
            }
        }
    }
    return 0;
}
