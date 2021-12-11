//
// Created by HuangYi on 2021/12/11.
//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "screen.h"


// 随机食物o的位置
enum Status randomFood(Screen *screen, _Bool first){
    srand(time(NULL));
    // 图个方便, 如果5000次没有随机到一个食物位置, 就直接游戏胜利了
    for (int i = 0; i < 5000; ++i) {
        int x = rand() % ROWS;
        int y = rand() % COLS;
        if (screen->tiles[x][y] == ' '){
            screen->tiles[x][y] = 'o';
            screen->food.x = x;
            screen->food.y = y;
            return GAME_RUN;
        }
    }
    return GAME_WIN;
}


// 分配一个MovePoint结构
MovePoint * createMovePoint(){
    MovePoint *move = (MovePoint *) malloc(sizeof(MovePoint));
    move->count = 0;
    move->headPos.x = 0;
    move->headPos.y = 0;
    move->mov.x = 0;
    move->mov.y = 0;
    move->next = NULL;
    move->is_kill = False;
    return move;
}


// 分配一个贪吃蛇身体的结构
Snake * createSnakeBody(Screen *screen, int x, int y){
    Snake *snake = (Snake *) malloc(sizeof(Snake));
    snake->mov.x = 0;
    snake->mov.y = 0;
    snake->pos.x = x;
    snake->pos.y = y;
    snake->next = NULL;
    screen->snakeLength ++;
    return snake;
}


// 添加一个拐弯位置
void addMovePoint(Screen *screen, Point point){
    if (point.x == 0 && point.y == 0) return;
    MovePoint *head = screen->moveArray;
    if (head == NULL){
        screen->moveArray = createMovePoint();
        head = screen->moveArray;
    } else{
        while (head->next != NULL){
            head = head->next;
        }
        head->next = createMovePoint();
        head = head->next;
    }
    head->headPos.x = screen->snake->pos.x;
    head->headPos.y = screen->snake->pos.y;
    head->mov.x = point.x;
    head->mov.y = point.y;
}


// 在屏幕后面添加一个蛇*
void addSnakeBody(Screen *screen){
    Snake *snake = screen->snake;
    while (snake->next != NULL){
        snake = snake->next;
    }
    Point local = snake->pos;
    Snake *new = createSnakeBody(screen, local.x - snake->mov.x, local.y - snake->mov.y);
    new->mov.x = snake->mov.x;
    new->mov.y = snake->mov.y;
    screen->tiles[new->pos.x][new->pos.y] = '*';
    snake->next = new;
}

// 移动贪吃蛇的身体
enum Status moveSnakeBody(Screen *screen, Snake *snake){
    screen->tiles[snake->pos.x][snake->pos.y] = ' ';
    snake->pos.x += snake->mov.x;
    snake->pos.y += snake->mov.y;
    _Bool is_add_food = False;
    if (screen->tiles[snake->pos.x][snake->pos.y] == 'o'){
        addSnakeBody(screen);
        is_add_food = True;
    }
    screen->tiles[snake->pos.x][snake->pos.y] = '*';
    if (is_add_food){
        return randomFood(screen, False);
    }
    return GAME_RUN;
}



// 管理贪吃蛇的移动
void manageMoveSnakeBody(Screen *screen){
    Snake *snake_head = screen->snake;
    MovePoint *move_head = screen->moveArray;
    while (snake_head != NULL){
        MovePoint *move_temp = move_head;
        while (move_temp != NULL){
            if (move_temp->is_kill){
                move_temp = move_temp->next;
                continue;
            }
            // 检查这个点*是否到了要拐弯的位置
            if (snake_head->pos.x == move_temp->headPos.x && snake_head->pos.y == move_temp->headPos.y){
                snake_head->mov.x = move_temp->mov.x;
                snake_head->mov.y = move_temp->mov.y;
                move_temp->count++;
            }
            // 当所有都拐弯拐完了之后, 标记释放
            if (move_temp->count == screen->snakeLength){
                move_temp->is_kill = True;
            }
            move_temp = move_temp->next;
        }
        snake_head = snake_head->next;
    }
    // 删除已经移动完了的拐弯位置
    MovePoint *move_temp = move_head;
    if (move_temp->next != NULL && move_temp->is_kill){
        MovePoint *temp = move_temp->next;
        free(move_temp);
        move_temp = temp;
    }
    screen->moveArray = move_temp;
}


// 蛇头撞到墙了游戏结束
_Bool isGameEnd(Screen *screen, Point point){
    Snake *head = screen->snake;
    return screen->tiles[head->pos.x + point.x][head->pos.y + point.y] == '#';
}


// 键盘按下了相反的方向
_Bool isBackMove(Screen *screen, Point point){
    Snake *head = screen->snake;
    int next_x = head->pos.x + point.x;
    int next_y = head->pos.y + point.y;
    return head->next->pos.x == next_x && head->next->pos.y == next_y;
}


// 贪吃蛇撞到了自己的身体
_Bool isKillMe(Screen *screen){
    Snake *head = screen->snake;
    return screen->tiles[head->pos.x + head->mov.x][head->pos.y + head->mov.y] == '*';
}


enum Status moveSnake(Screen *screen, Point point){
    enum Status resp = GAME_RUN;
    if (point.x == 0 && point.y == 0){
        return GAME_RUN;
    }
    Snake *head = screen->snake;
    // 检查是否撞到墙, 游戏结束
    if (isGameEnd(screen, point)){
        return GAME_END;
    }
    // 检查是否自己撞到自己
    if (isKillMe(screen)){
        return GAME_KILL;
    }
    // 检查是否向同一方向的背面移动
    if (isBackMove(screen, point)){
        point.x = 0;
        point.y = 0;
        resp = GAME_BACK_RUN;
    }
    // 记录这次移动方向的位置
    addMovePoint(screen, point);
    manageMoveSnakeBody(screen);
    while (head != NULL){
        if (moveSnakeBody(screen, head) == GAME_WIN){
            return GAME_WIN;
        }
        head = head->next;
    }
    return resp;
}


// 初始化贪吃蛇的位置
void initSnakePosition(Screen *screen){
    Snake *snake = createSnakeBody(screen, 2, 6);
    snake->mov.x = 0;
    snake->mov.y = 1;
    screen->snake = snake;
    screen->tiles[2][6] = '*';
    // 初始长度 3
    for (int i = 0; i < 2; ++i) {
        addSnakeBody(screen);
    }
    randomFood(screen, True);
}


// 创建一个屏幕结构
Screen createScreen(){
    Screen screen;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (i == 0 || i == ROWS - 1 || j == 0 || j == COLS - 1) {
                screen.tiles[i][j] = '#';
            } else{
                screen.tiles[i][j] = ' ';
            }
        }
    }
    screen.rows = ROWS;
    screen.cols = COLS;
    screen.snakeLength = 0;
    screen.moveArray = NULL;
    initSnakePosition(&screen);
    return screen;
}


void showScreen(Screen *screen){
    // 打印前先清空屏幕
    system("cls");
    for (int i = 0; i < screen->rows; ++i) {
        for (int j = 0; j < screen->cols; ++j) {
            putchar(screen->tiles[i][j]);
        }
        putchar('\n');
    }
}


// 结束游戏, free释放蛇的身体和拐弯位置内存
void closeScreen(Screen *screen){
    Snake *head = screen->snake;
    MovePoint *move_head = screen->moveArray;
    while (head != NULL){
        Snake * next = head->next;
        free(head);
        head = next;
    }
    while (move_head != NULL){
        MovePoint * next = move_head->next;
        free(move_head);
        move_head = next;
    }
}


