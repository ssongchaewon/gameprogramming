#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#define MAX_QUESTIONS 9

// 함수 선언 (프로토타입)
void outro_screen(void);
void start_game(void);
void show_unit_info(void);
void menu(void);

const char* ms_char[10] = { "센티미터", "미터", "킬로미터", "인치", "피트",
                            "야드", "마일", "자(척)", "간", "리" };

double msre[10];
int score = 0, level = 1;
int total_questions = 9;

HANDLE hConsole;

// 변환 계산
double calc(int basic, int transe, double measure) {
    return measure * (msre[transe] / msre[basic]);
}

// 변환 비율 계산
double ratio(int basic, int transe) {
    return msre[transe] / msre[basic];
}

// 초기화
void init_msre(void) {
    msre[0] = 392772;         // cm
    msre[1] = msre[0] / 100.; // m
    msre[2] = msre[1] / 1000.; // km
    msre[3] = msre[0] / 2.54; // inch
    msre[4] = msre[3] / 12.;  // feet
    msre[5] = msre[4] / 3.;   // yard
    msre[6] = msre[5] / 1760.; // mile
    msre[7] = 12960.;         // 자
    msre[8] = msre[7] / 6.;   // 간
    msre[9] = msre[8] / (6. * 36); // 리
}

// 인트로
void intro_screen(void) {
    system("cls");
    system("title 단위 계산기 - 로그라이크 게임");
    SetConsoleTextAttribute(hConsole, 11);

    printf("    ____                         \n");
    printf("   / ___| __ _ _ __ ___   ___   \n");
    printf("  | |  _ / _` | '_ ` _ \\ / _ \\  \n");
    printf("  | |_| | (_| | | | | | |  __/  \n");
    printf("   \\____|\\__,_|_| |_| |_|\\___|  \n");
    printf("            UNIT CONVERTER ROGUELIKE GAME             \n");
    printf("-------------------------------------------------------\n");

    SetConsoleTextAttribute(hConsole, 14);
    printf("로딩 중");
    for (int i = 0; i < 5; i++) {
        printf(".");
        fflush(stdout);
        Sleep(300);
    }
    SetConsoleTextAttribute(hConsole, 7);
    printf("\n\nEnter를 눌러 시작하세요...");
    (void)getchar(); // 경고 제거
    system("cls");
}

// 단위 목록
void print_distance_unit(void) {
    printf("+--------------------------------------------------+\n");
    printf("|                     단위 목록                    |\n");
    printf("+--------------------------------------------------+\n");
    for (int i = 0; i <= 9; i++) {
        if (i % 2 == 0) SetConsoleTextAttribute(hConsole, 14);
        else SetConsoleTextAttribute(hConsole, 10);
        printf("| %d : %-10s ", i, ms_char[i]);
        if (i % 3 == 2) printf("|\n");
    }
    int remainder = 10 % 3;
    if (remainder != 0) {
        for (int j = 0; j < 3 - remainder; j++)
            printf("| %-14s ", "");
        printf("|\n");
    }
    printf("+--------------------------------------------------+\n");
    SetConsoleTextAttribute(hConsole, 7);
}

// 진행도
void show_progress(int question_num) {
    char progress[MAX_QUESTIONS + 1];
    for (int i = 0; i < MAX_QUESTIONS; i++)
        progress[i] = (i < question_num) ? '#' : '-';
    progress[MAX_QUESTIONS] = '\0';

    SetConsoleTextAttribute(hConsole, 7);
    printf("+-----------------+\n");
    SetConsoleTextAttribute(hConsole, 9);
    printf("| 레벨:%-2d ", level);
    SetConsoleTextAttribute(hConsole, 14);
    printf("점수:%-3d|\n", score);
    SetConsoleTextAttribute(hConsole, 10);
    printf("| 진행도:%-9s|\n", progress);
    SetConsoleTextAttribute(hConsole, 7);
    printf("+-----------------+\n\n");
}

// 문제
void ask_question(int qnum) {
    int basic = rand() % 6, transe;
    do { transe = rand() % 6; } while (transe == basic);

    double measure = (rand() % 1000) / 10.0 + 1;
    show_progress(qnum);

    printf("문제 %d: %.2f %s를 %s로 변환하세요\n", qnum + 1, measure, ms_char[basic], ms_char[transe]);
    printf("정답 입력 > ");
    double answer;
    (void)scanf("%lf", &answer);

    double real_answer = calc(basic, transe, measure);

    if (fabs(real_answer - answer) < 1.0) {
        printf("정답! +10점\n\n");
        score += 10;
    }
    else {
        printf("오답! 정답은 %.5f %s 입니다. -5점\n\n", real_answer, ms_char[transe]);
        score -= 5;
    }

    Sleep(500);
}

// 아웃트로
void outro_screen(void) {
    system("cls");
    SetConsoleTextAttribute(hConsole, 11);
    printf("+------------------------------+\n");
    printf("|                              |\n");
    SetConsoleTextAttribute(hConsole, 14);
    printf("|  Thank you for playing!      |\n");
    SetConsoleTextAttribute(hConsole, 11);
    printf("|                              |\n");
    printf("+------------------------------+\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    printf("엔터 키를 눌러 종료하세요...");
    (void)getchar(); (void)getchar(); 
}

// 단위 비율 보기
void show_unit_info(void) {
    system("cls");
    printf("+---------------------- 단위 변환 비율 ----------------------+\n");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (i != j)
                printf("1 %-8s = %10.5f %-8s\n", ms_char[i], ratio(i, j), ms_char[j]);
        }
        printf("-------------------------------------------------------------\n");
    }
    printf("\nEnter를 눌러 메뉴로 돌아갑니다...");
    (void)getchar(); (void)getchar();
}

// 게임 시작
void start_game(void) {
    system("cls");
    score = 0;
    level = 1;
    for (int q = 0; q < total_questions; q++) ask_question(q);
    printf("게임 종료! 최종 점수: %d\n", score);
    outro_screen();
}

// 메뉴
void menu(void) {
    int choice;
    do {
        system("cls");
        SetConsoleTextAttribute(hConsole, 11);
        printf("+--------------------------------+\n");
        printf("|         UNIT GAME MENU         |\n");
        printf("+--------------------------------+\n");
        SetConsoleTextAttribute(hConsole, 14);
        printf("| 1. 게임 시작                   |\n");
        printf("| 2. 단위 비율 정보              |\n");
        printf("| 3. 종료                        |\n");
        SetConsoleTextAttribute(hConsole, 7);
        printf("+--------------------------------+\n");
        printf("선택 > ");
        (void)scanf("%d", &choice);

        switch (choice) {
        case 1: start_game(); break;
        case 2: show_unit_info(); break;
        case 3: outro_screen(); break;
        default:
            printf("잘못된 입력입니다.\n");
            Sleep(800);
        }
    } while (choice != 3);
}

// 메인
int main(void) {
    srand((unsigned int)time(NULL));
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    init_msre();

    intro_screen();
    menu();

    return 0;
}
