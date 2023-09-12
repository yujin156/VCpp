#include <iostream>
#include <conio.h> 

using namespace std;

int main() {
    cout << "♥★♥송유진의 행복한 VC++♥★♥";

    char command;
    char cursor = '_';

    while (true) {
        cout << "\ncommand>" << cursor;
       
        command = _getch();
        cout << "\b";
        cin >> command;

        switch (command) {
        case 'q':
            cout << "qwer 화이팅";
            break;
        case 'w':
            cout << "과제 모르겠다";
            break;
        case 'e':
            cout << "담주 부터 3/1배";
            break;
        case 'r':
            cout << "햅피햅피햅피";
            break;
        case 27:
            cout << "프로그램 종료" << endl;
            return 0;
        default:
            cout << "오류: 알 수 없는 커맨드, 다시 입력하세요.";
            break;
        }
    }
    return 0;
}