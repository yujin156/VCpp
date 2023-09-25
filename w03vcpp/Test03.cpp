#include <stdio.h> // C 표준 입출력 라이브러리
#include <conio.h> // 콘솔 사용을 위한 라이브러리
#include <Windows.h> // Windows API 함수 및 데이터 유형을 사용하기 위한 라이브러리

// 특수 키 상수 정의
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

int main() {
    char input;
    int sequenceIndex = 0;
    int inputSequence[4] = { 0 };

    while (1) {
        input = _getch(); // 특수 키 입력 받기 위해

        // Esc 키 입력을 감지하면 루프를 종료합니다.
        if (input == 27) {
            break;
        }

        // 특수 키 처리
        if (input == 0 || input == -32) {
            input = _getch(); // 실제 특수 키 값을 얻기 위해 추가로 읽음
            switch (input) {
            case 72: // UP 키
                input = '^';
                break;
            case 80: // DOWN 키
                input = 'v';
                break;
            case 75: // LEFT 키
                input = '<';
                break;
            case 77: // RIGHT 키
                input = '>';
                break;
            default:
                // 다른 특수 키는 그대로 출력
                break;
            }
        }

        // 입력된 키를 화면에 출력합니다.
        printf("%c ", input);

        // 입력 시퀀스 업데이트
        for (int i = 0; i < 3; i++) {
            inputSequence[i] = inputSequence[i + 1];
        }
        inputSequence[3] = input;

        // 기술 1 실행 (왼쪽, 위, 오른쪽, 아래 순서)
        if (inputSequence[0] == '<' &&
            inputSequence[1] == '^' &&
            inputSequence[2] == '>' &&
            inputSequence[3] == 'v') {
            printf("\n＠＠＠＠＠＠＠＠＠＠＠＠");
            printf("\n＠요이키텐카이 무량공처＠\n"); // 기술 1 대사 출력
            printf("＠＠＠＠＠＠＠＠＠＠＠＠\n");
        }

        // 기술 2 실행 (위, 아래, 위, 아래 순서)
        if (inputSequence[0] == '^' &&
            inputSequence[1] == 'v' &&
            inputSequence[2] == '^' &&
            inputSequence[3] == 'v') {
            printf("\n자꾸 위아래로△▽");
            printf("\n＠ 흔들리는 나＠\n"); // 기술 2 대사 출력
            printf("와돈추노우 돈추노우 돈추\n");
        }

        // 기술 3 실행 (w, t, f, 위 순서)
        if (inputSequence[0] == 'w' &&
            inputSequence[1] == 't' &&
            inputSequence[2] == 'f' &&
            inputSequence[3] == '^') {
            printf("\n＠＠＠＠＠＠＠＠＠＠＠＠");
            printf("\n＠하..하얗게 불태웠다..＠\n"); // 기술 3 대사 출력
            printf("＠＠＠＠＠＠＠＠＠＠＠＠＠\n");
        }

        Sleep(300); // 300 밀리초 동안 프로그램을 일시 중지
    }

    return 0;
}
