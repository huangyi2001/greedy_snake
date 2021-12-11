#include <stdio.h>
#include <time.h>
#include <conio.h>
#include "screen.h"


int main() {
    int count = 0;
    Screen screen = createScreen();
    int ch;
    Point point;
    point.x = 1;
    point.y = 0;
    while (1){
        count ++;
        if (count > 5000){
            count = 0;
            showScreen(&screen);
            if (!moveSnake(&screen, point)){
                puts("\nEnd.");
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
