### Greedy_snake

---

#### 项目特点：

---

- 使用curse 库实现整个终端游戏界面；
- 使用`setitimer` 定时函数实现蛇的移动，以及随等级的提升速度也跟着提升；
- 使用首尾都是空节点的双向链表来表示蛇结构；
- 使用随机种子`srand` 函数，随机初始化蛇和食物的位置；
- 短小，只有200行。

#### 编译过程

---

`Makefile` 文件如下：

```makefile
all:	Gready_snake


Gready_snake:
	gcc -g  main.c Greedy_snake.c -l curses -o Gready_snake

clean:
	rm -rf *.o Gready_snake
```
