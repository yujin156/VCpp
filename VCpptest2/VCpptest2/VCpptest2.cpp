#include <iostream>
#include <conio.h> 

using namespace std;

int main() {
    cout << "♥★♥송유진의 행복한 VC++♥★♥\n" << endl;

    char command;
    char cursor = '_';

    while (true) {
        cout << "command>" << cursor;

        command = _getch(); 
        
        
        if (command == 27) { 
            cout << "\n프로그램 종료" << endl;
            break;
        }
      
        cout << command;

        while (true) {
            char enter = _getch();
            if (enter == '\r') 
                break;
        }
       


        switch (command) {
        case 'q':
            cout << " : qwer 화이팅" << endl;
            break;
        case 'w':
            cout << " : 과제 모르겠다" << endl;
            break;
        case 'e':
            cout << " : 담주 부터 3/1배" << endl;
            break;
        case 'r':
            cout << " : 햅피햅피햅피" << endl;
            break;
        default:
            cout << "\n오류: 알 수 없는 커맨드, 다시 입력하세요." << endl;
            break;
        }
    }

    return 0;
}