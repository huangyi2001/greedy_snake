//
// Created by HuangYi on 2021/12/11.
//

#ifndef GREEDY_SNAKE_SCREEN_H
#define GREEDY_SNAKE_SCREEN_H
#endif //GREEDY_SNAKE_SCREEN_H

#define ROWS 10
#define COLS 50
#define True 1
#define False 0


typedef struct point{
    int x;
    int y;
} Point;


typedef struct snake{
    // 贪吃蛇身体的位置
    Point pos;
    // 贪吃蛇身体下一次移动的方向
    Point mov;
    // 贪吃蛇身体下一个身体
    struct snake *next;
} Snake;


typedef struct movePoint{
    // 记录用户按下键盘时贪吃蛇第一个*的位置
    Point headPos;
    // 记录当次用户按下键盘移动的方向
    Point mov;
    // 已经移动了多少次了(当移动完了的时候删除)
    int count;
    // 是否已经移动完成
    _Bool is_kill;
    // 记录下一个
    struct movePoint *next;
} MovePoint;


typedef struct screen{
    char tiles[ROWS][COLS];
    int rows;
    int cols;
    // 记录贪吃蛇的位置
    Snake *snake;
    // 贪吃蛇的长度
    int snakeLength;
    // 记录用户按下的方向
    MovePoint *moveArray;
} Screen;

/**
 *创建一个屏幕大小为Weight, Height的屏幕
 */
Screen createScreen();

/**
 * 在命令行界面上打印屏幕
 */
void showScreen(Screen *screen);


// 关闭屏幕, 并释放内存
void closeScreen(Screen *screen);


// 移动屏幕上的蛇
_Bool  moveSnake(Screen *screen, Point point);
