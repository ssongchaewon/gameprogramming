#include <stdio.h>

void draw_square(int size);

int main(void)
{
    int n;
    printf("정사각형 그리기\n\n");
    printf("정사각형의 길이(최대 37)를\n");
    printf("입력하고 Enter> ");
    scanf("%d", &n);
    draw_square(n);
    return 0;
}

void draw_square(int size)
{
    int i, j;
    unsigned char a = 0xA6;
    unsigned char b[7];

    for (i = 1; i < 7; i++)
        b[i] = 0xA0 + i;

    int width = size;           // 가로 크기
    int height = size / 2;      // 세로 크기 (비율 보정)

    // 윗면
    printf("%c%c", a, b[3]);           // ┌
    for (i = 0; i < width; i++)
        printf("%c%c", a, b[1]);       // ─
    printf("%c%c\n", a, b[4]);         // ┐

    // 옆면
    for (i = 0; i < height; i++) {
        printf("%c%c", a, b[2]);       // │
        for (j = 0; j < width; j++) {
            printf(" ");              // ← 공백도 2칸(스페이스 2개)
        }
        printf("%c%c\n", a, b[2]);     // │
    }

    // 아랫면
    printf("%c%c", a, b[6]);           // └
    for (i = 0; i < width; i++)
        printf("%c%c", a, b[1]);       // ─
    printf("%c%c\n", a, b[5]);         // ┘
}
