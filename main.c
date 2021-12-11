#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "screen.h"


int main() {
    DWORD local = 0;
    Screen screen = createScreen();
    int ch;
    Point point;
    enum Status status;
    point.x = 0;
    point.y = 0;
    while (1){
        // 200毫米一刷
        if (GetTickCount() - local >= 200){
            local = GetTickCount();
            showScreen(&screen);
            status = moveSnake(&screen, point);
            if (status == GAME_WIN){
                puts("\nGAME WIN.");
                break;
            }else if(status == GAME_KILL){
                puts("\nKill ME.");
                break;
            }else if(status == GAME_END) {
                puts("\nGAME END.");
                break;
            }
        }
        // 获取键盘输入
        if (_kbhit()){
            ch = _getch();
            if (ch == 27) {
                closeScreen(&screen);
                break;
            };
            switch (ch) {
                case 119: {
                    point.x = -1;
                    point.y = 0;
                    break;
                }
                case 115: {
                    point.x = 1;
                    point.y = 0;
                    break;
                }
                case 97: {
                    point.x = 0;
                    point.y = -1;
                    break;
                }
                case 100: {
                    point.x = 0;
                    point.y = 1;
                    break;
                }
                default: break;
            }
        }
    }
}
