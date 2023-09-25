#include <stdio.h> // C ǥ�� ����� ���̺귯��
#include <conio.h> // �ܼ� ����� ���� ���̺귯��
#include <Windows.h> // Windows API �Լ� �� ������ ������ ����ϱ� ���� ���̺귯��

// Ư�� Ű ��� ����
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

int main() {
    char input;
    int sequenceIndex = 0;
    int inputSequence[4] = { 0 };

    while (1) {
        input = _getch(); // Ư�� Ű �Է� �ޱ� ����

        // Esc Ű �Է��� �����ϸ� ������ �����մϴ�.
        if (input == 27) {
            break;
        }

        // Ư�� Ű ó��
        if (input == 0 || input == -32) {
            input = _getch(); // ���� Ư�� Ű ���� ��� ���� �߰��� ����
            switch (input) {
            case 72: // UP Ű
                input = '^';
                break;
            case 80: // DOWN Ű
                input = 'v';
                break;
            case 75: // LEFT Ű
                input = '<';
                break;
            case 77: // RIGHT Ű
                input = '>';
                break;
            default:
                // �ٸ� Ư�� Ű�� �״�� ���
                break;
            }
        }

        // �Էµ� Ű�� ȭ�鿡 ����մϴ�.
        printf("%c ", input);

        // �Է� ������ ������Ʈ
        for (int i = 0; i < 3; i++) {
            inputSequence[i] = inputSequence[i + 1];
        }
        inputSequence[3] = input;

        // ��� 1 ���� (����, ��, ������, �Ʒ� ����)
        if (inputSequence[0] == '<' &&
            inputSequence[1] == '^' &&
            inputSequence[2] == '>' &&
            inputSequence[3] == 'v') {
            printf("\n������������������������");
            printf("\n������Ű��ī�� ������ó��\n"); // ��� 1 ��� ���
            printf("������������������������\n");
        }

        // ��� 2 ���� (��, �Ʒ�, ��, �Ʒ� ����)
        if (inputSequence[0] == '^' &&
            inputSequence[1] == 'v' &&
            inputSequence[2] == '^' &&
            inputSequence[3] == 'v') {
            printf("\n�ڲ� ���Ʒ��Ρ��");
            printf("\n�� ��鸮�� ����\n"); // ��� 2 ��� ���
            printf("�͵��߳�� ���߳�� ����\n");
        }

        // ��� 3 ���� (w, t, f, �� ����)
        if (inputSequence[0] == 'w' &&
            inputSequence[1] == 't' &&
            inputSequence[2] == 'f' &&
            inputSequence[3] == '^') {
            printf("\n������������������������");
            printf("\n����..�Ͼ�� ���¿���..��\n"); // ��� 3 ��� ���
            printf("��������������������������\n");
        }

        Sleep(300); // 300 �и��� ���� ���α׷��� �Ͻ� ����
    }

    return 0;
}
