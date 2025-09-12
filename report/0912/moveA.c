#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define X_MAX 79
#define Y_MAX 24

void move_arrow_key(int key, int *x, int *y, int x_b, int y_b);
void gotoxy(int x, int y);

int main(void)
{
    int x = 10, y = 5;  // 초기 위치
    int prev_x = x, prev_y = y;
    int key;

    // 첫 출력
    gotoxy(x, y);
    printf("A");

    do
    {
        key = getch();

        if (key == 0 || key == 0xE0) // 방향키 처리
        {
            key = getch();

            // 이전 위치 지우기
            gotoxy(x, y);
            printf(" ");
            fflush(stdout);  // 출력 버퍼 비우기 (안 하면 지연될 수 있음)

            // 위치 업데이트
            move_arrow_key(key, &x, &y, X_MAX, Y_MAX);

            // 새 위치에 A 출력
            gotoxy(x, y);
            printf("A");
            fflush(stdout);
        }

    } while (key != 27); // ESC 종료

    return 0;
}

void move_arrow_key(int key, int *x, int *y, int x_b, int y_b)
{
    switch (key)
    {
        case 72: // 위
            (*y)--;
            if (*y < 0) *y = 0;
            break;
        case 75: // 왼쪽
            (*x)--;
            if (*x < 0) *x = 0;
            break;
        case 77: // 오른쪽
            (*x)++;
            if (*x > x_b) *x = x_b;
            break;
        case 80: // 아래
            (*y)++;
            if (*y > y_b) *y = y_b;
            break;
    }
}

void gotoxy(int x, int y)
{
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
