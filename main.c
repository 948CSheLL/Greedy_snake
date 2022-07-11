//用方向键控制蛇的方向
#include"Greedy_snake.h"

int main()
{
    initscr();
    initGame();
    signal(SIGALRM, show);
    getOrder();
    /* 在退出 curses 之前，程序还必须为每个正在使用的终端调用 endwin。 如果
     * 为同一个终端多次调用 newterm，则引用的第一个终端必须是最后一个调用
     * endwin 的终端。
     */
    endwin();
    return 0;
}
