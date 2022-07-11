#ifndef __GREEDY_SNAKE_H
#define __GREEDY_SNAKE_H
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#define NUM 60

struct direct                //用来表示方向的
{
    int cx;
    int cy;
};
typedef struct node            //链表的结点
{
    int cx;
    int cy;
    struct node *back;
    struct node *next;
}node;

void initGame();            //初始化游戏
int setTicker(int);            //设置计时器
void show();                //显示整个画面
void showInformation();        //显示游戏信息（前两行）
void showSnake();            //显示蛇的身体
void getOrder();            //从键盘中获取命令
void over(int i);            //完成游戏结束后的提示信息

void creatLink();                //（带头尾结点）双向链表以及它的操作
void insertNode(int x, int y);   
void deleteNode();
void deleteLink();

int ch;                                //输入的命令
int hour, minute, second;            //时分秒
int length, tTime, level;            //（蛇的）长度，计时器，（游戏）等级
struct direct dir, food;            //蛇的前进方向，食物的位置
node *head, *tail;                    //链表的头尾结点

#endif

