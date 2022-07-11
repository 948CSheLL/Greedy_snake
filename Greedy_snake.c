#include"Greedy_snake.h"

void initGame()
{
    /* 通常，tty 驱动程序会缓冲输入的字符，直到输入换行符或回车符。 cbreak
     * 例程禁用行缓冲和擦除/终止字符处理（中断和流控制字符不受影响），使用
     * 户键入的字符立即可供程序使用。 nocbreak 例程将终端返回到正常（熟）模
     * 式。
     * 最初终端可能处于也可能不处于 cbreak 模式，因为该模式是继承的； 因此，
     * 程序应该显式调用 cbreak 或 nocbreak。 大多数使用 curses 的交互式程序
     * 都设置了 cbreak 模式。
     */
    cbreak();                    //把终端的CBREAK模式打开
    noecho();                    //关闭回显
    curs_set(0);                //把光标置为不可见
    /* 键盘选项启用用户终端的键盘。 如果启用（bf 为 TRUE），用户可以按下功
     * 能键（例如箭头键），wgetch(3X) 返回表示功能键的单个值，如 KEY_LEFT。
     * 如果禁用（bf 为 FALSE），curses 不会专门处理功能键，程序必须自己解释
     * 转义序列。 如果终端中的键盘可以打开（发送）和关闭（本地工作），打开
     * 此选项会导致终端键盘在调用 wgetch(3X) 时打开。
     * 键盘的默认值为 FALSE。
     */
    keypad(stdscr, true);        //使用用户终端的键盘上的小键盘
    srand(time(0));                //设置随机数种子
    //初始化各项数据
    /* 游戏时间 */
    hour = minute = second = tTime = 0;
    /* 蛇的长度 */
    length = 1;
    /* 控制蛇前进的方向 */
    dir.cx = 1;
    dir.cy = 0;
    /* 输入的命令 */
    ch = 'A';
    /* 在初始化 curses 之后，LINES这个变量包含了屏幕的高度，即行数。*/
    /* 在初始化curses之后，COLS这个变量包含了屏幕的宽度，即列数。*/
    /* 食物的位置 */
    food.cx = rand() % COLS;
    /* -2 是因为需要去掉开始的信息栏和分割行 */
    food.cy = rand() % (LINES-2) + 2;
    //创建一个双向链表表示一条蛇
    creatLink();
    /* Q: 不是很懂，为什么时间参数是20 */
    setTicker(20);
}

//设置计时器
int setTicker(int n_msecs)
{
    struct itimerval new_timeset;
    long    n_sec, n_usecs;

    n_sec = n_msecs / 1000 ;
    n_usecs = ( n_msecs % 1000 ) * 1000L ;
    new_timeset.it_interval.tv_sec  = n_sec;       
    new_timeset.it_interval.tv_usec = n_usecs;     
    n_msecs = 1;
    n_sec = n_msecs / 1000 ;
    n_usecs = ( n_msecs % 1000 ) * 1000L ;
    new_timeset.it_value.tv_sec     = n_sec  ;     
    new_timeset.it_value.tv_usec    = n_usecs ;    
    /* setitimer函数为设置定时器（闹钟），可替代alarm函数，比alarm函数精确。
     * ITIMER_REAL 计时实际时间，到达时间，产生SIGALRM信号度更高，精度为微
     * 秒，可以实现周期定时。
     * struct itimerval {
     *    struct timeval it_interval; 
     *    struct timeval it_value; 
     * };
     * setitimer函数，第一个参数指定使用哪一个定时器，第二个参数就是传递进
     * 去的时间结构体，第三个参数为NULL。这样就设定好了一个定时器了。它就可
     * 以开始工作了。
     * it_value为计时时长，it_interval为定时器的间隔，即第一次计时it_value
     * 时长发送信号，再往后的信号每隔一个it_interval发送一次。
     */
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void showInformation()
{
    tTime++;
    if(tTime >= 1000000)               
        tTime = 0;
    /* 上面的show 函数中，setitimer 定时器，定时每0.02s 发送一次SIGALRM 信
     * 号，因此正好发送50次，就是一秒，下面的信息就是一秒显示一次。
     */
    if(1 != tTime % 50)
        return;
    /* int move(int y, int x);这些例程将与窗口关联的光标移动到第 y 行和第 x 列。
     * 在调用 refresh(3X) 之前，此例程不会移动终端的物理光标。 指定的位置相
     * 对于窗口的左上角，即 (0,0)。
     */
    move(0, 3);   
    //显示时间
    /* printw、wprintw、mvprintw 和 mvwprintw 例程类似于 printf [参见
     * printf(3)]。 实际上，将由 printf 输出的字符串改为输出，就好像在给定
     * 窗口上使用了 waddstr。
     * vwprintw 和 wv_printw 例程类似于 vprintf [参见 printf(3)] 并使用可变
     * 参数列表执行 wprintw。 第三个参数是 va_list，一个指向参数列表的指针，
     * 在 <stdarg.h> 中定义。
     */
    printw("time: %d:%d:%d %c", hour, minute, second);
    second++;
    /* 下面的NUM 是符号常量，表示60 */
    if(second > NUM)
    {
        second = 0;
        minute++;
    }
    if(minute > NUM)
    {
        minute = 0;
        hour++;
    }
    //显示长度，等级
    move(1, 0);
    int i;
    /* 分割行，上面是数据显示区，下面是游戏区 */
    for(i=0;i<COLS;i++)
        addstr("-");
    /* 打印蛇的相关信息 */
    move(0, COLS/2-5);
    printw("length: %d", length);
    move(0, COLS-10);
    level = length / 3 + 1;
    printw("level: %d", level);
}

//蛇的表示是用一个带头尾结点的双向链表来表示的，
//蛇的每一次前进，都是在链表的头部增加一个节点，在尾部删除一个节点
//如果蛇吃了一个食物，那就不用删除节点了
void showSnake()
{
    /* 等级越高，蛇的速度也就越快 */
    if(1 != tTime % (30-level))
        return;
    //判断蛇的长度有没有改变
    bool lenChange = false;
    //显示食物
    move(food.cy, food.cx);
    printw("@");
    //如果蛇碰到墙，则游戏结束
    if((COLS-1==head->next->cx && 1==dir.cx)
        || (0==head->next->cx && -1==dir.cx)
        || (LINES-1==head->next->cy && 1==dir.cy)
        || (2==head->next->cy && -1==dir.cy))
    {
        over(1);
        return;
    }
    //如果蛇头砬到自己的身体，则游戏结束
    /* mvinch是获得对应位置的字符 */
    if('*' == mvinch(head->next->cy+dir.cy, head->next->cx+dir.cx) )
    {
        over(2);
        return;
    }
    /* 往蛇头插入节点，模拟蛇向前走的过程 */
    insertNode(head->next->cx+dir.cx, head->next->cy+dir.cy);
    //蛇吃了一个“食物”
    if(head->next->cx==food.cx && head->next->cy==food.cy)
    {
        lenChange = true;
        length++;
        //恭喜你，通关了
        /* 如果长度超过了50 就通关了 */
        if(length >= 50)
        {
            over(3);
            return;
        }
        //重新设置食物的位置
        food.cx = rand() % COLS;
        food.cy = rand() % (LINES-2) + 2;
    }
    /* 如果蛇没有吃到食物，就需要将最后一个节点删掉 */
    if(!lenChange)
    {
        move(tail->back->cy, tail->back->cx);
        printw(" ");
        deleteNode();
    }
    /* 将光标移动到新的开头，打印新的蛇头 */
    move(head->next->cy, head->next->cx);
    printw("*");
}

void show()
{
    /* signal 需要反复设置信号处理函数，不然调用一次就会恢复到默认的处理方
     * 式。
     */
    signal(SIGALRM, show);        //设置中断信号
    /* 显示相关的信息 */
    showInformation();
    /* 显示蛇 */
    showSnake();
    refresh();                    //刷新真实屏幕
}

void getOrder()
{
    //建立一个死循环，来读取来自键盘的命令
    while(1)
    {
        ch = getch();
        /* 下面的KEY_LEFT 是curses.h 中的符号常量 表示左键头 */
        /* 左键头需要更改方向 */
        if(KEY_LEFT == ch)
        {
            dir.cx = -1;
            dir.cy = 0;
        }
        else if(KEY_UP == ch)
        {
            dir.cx = 0;
            dir.cy = -1;
        }
        else if(KEY_RIGHT == ch)
        {
            dir.cx = 1;
            dir.cy = 0;
        }
        else if(KEY_DOWN == ch)
        {
            dir.cx = 0;
            dir.cy = 1;
        }
        setTicker(20);
    }
}

void over(int i)
{
    //显示结束原因
    /* 将光标移动到坐标(0, 0)，打印相关信息 */
    move(0, 0);
    int j;
    for(j=0;j<COLS;j++)
        /* 在给定窗口上写入（以空结尾的）字符串 str。 它类似于为字符串中的
         * 每个字符调用一次 waddch。
         */
        addstr(" ");
    move(0, 2);
    if(1 == i)
        addstr("Crash the wall. Game over");
    else if(2 == i)
        addstr("Crash itself. Game over");
    else if(3 == i)
        addstr("Mission Complete");
    setTicker(0);                //关闭计时器
    deleteLink();                //释放链表的空间
}

//创建一个双向链表
/* 此链表首尾都是空节点 */
void creatLink()
{
    node *temp = (node *)malloc( sizeof(node) );
    head = (node *)malloc( sizeof(node) );
    tail = (node *)malloc( sizeof(node) );
    /* 一开始蛇只有一个节点，这个temp 表示他的坐标 */
    temp->cx = 5;
    temp->cy = 10;
    head->back = tail->next = NULL;
    head->next = temp;
    temp->next = tail;
    tail->back = temp;
    temp->back = head;
}

//在链表的头部（非头结点）插入一个结点
/* 根据坐标在蛇的头部插入一个节点 */
void insertNode(int x, int y)
{
    node *temp = (node *)malloc( sizeof(node) );
    temp->cx = x;
    temp->cy = y;
    temp->next = head->next;
    head->next = temp;
    temp->back = head;
    temp->next->back = temp;
}

//删除链表的（非尾结点的）最后一个结点
void deleteNode()
{
    node *temp = tail->back;
    node *bTemp = temp->back;
    bTemp->next = tail;
    tail->back = bTemp;
    temp->next = temp->back = NULL;
    free(temp);
    temp = NULL;
}

//删除整个链表
void deleteLink()
{
    /* 从最后一个节点开始删除 */
    while(head->next != tail)
        deleteNode();
    head->next = tail->back = NULL;
    free(head);
    free(tail);
}
