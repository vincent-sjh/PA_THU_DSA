# PA Report for 4-2 Game

### 解题构思

- 最初只想到进行普通的Dijkstra，但是发现九成测之后有测试点超时，于是开始考虑如何优化算法。
- 普通算法中在寻找下一个需要更新处理的节点时，会遍历所有还未处理的节点来找寻这个点，这样导致搜索的时间复杂度太高，
- 想到维护一个小根堆来便于搜索下一个节点，堆中维护当前节点可以到达的（距离不是正无穷）的节点。
- 用存储空间换取计算复杂度的优化。

### 实现方法

### 1. 节点结构体定义

```cpp
struct node;
struct list {
    node* index;
    list* next;

    list(node* a) : index(a), next(nullptr) {}
};

struct node {
    // 成员变量
    int index;
    bool set;
    bool reach;
    long long int time;
    long long int mindistance;
    long long int pathnum;
    list* link;
};
```

定义两个结构体，`node` 表示图中的一个节点，而 `list` 用于创建邻接表，表示和其相连的节点。

### 2. 小根堆结构体定义

```cpp
node game[110000];
struct Heap {
    // 成员变量
    node reachdot[200000];
    int size;

    // 构造函数
    Heap() : size(0) {}

    // 方法: exam, push, pop
    // ...
};
Heap insect;
```

全局变量包括一个节点数组（`game`），一个 `Heap` 结构体的实例（`insect`），以及 `Heap` 结构体本身。`Heap` 结构体表示用于 Dijkstra 算法中过程节点的小根堆。

`Heap` 结构体有三个主要方法：

- **exam(int i):** 检查是否保持了堆的性质，主要用于调试检验小根堆的维护是否正确。
- **push(node a):** 将一个节点添加到堆中并保持堆的性质。
- **pop():** 从堆中移除并返回最小的元素，同时保持堆的性质。

### 3. 初始化堆

```cpp
insect.size = 0;
// 初始化堆中的第一个节点
game[1].set = true;
game[1].pathnum = 1;
game[1].reach = true;
// 将第一个节点的邻居添加到堆中
list* templist = game[1].link;
while (templist != nullptr) {
    // ...
}
```

我们的计算从序号为1的节点开始，先将和节点一1相邻的节点入堆

### 3. Dijkstra 算法：取出节点

```cpp
while (1) {
    node temp = insect.pop();
    while (game[temp.index].set == true) {
        temp = insect.pop();
    }

    // ...
}
```

实现了 Dijkstra 算法，过程中要取出接下来当前距离最小的未处理节点。如果我们的堆维护正确，只需要pop输出堆顶即可，但是这可能是历史数据。具体而言，同一节点可能会被多次更新最短距离，而更新最短距离时会被入堆，所以可能会被多次入堆，所以我们呢需要一直pop节点直至这个节点现在是未处理的状态。

### 4. 更新距离与路径

```cpp
templist = game[temp.index].link;
while (templist != nullptr) {
	if (templist->index->set == true) {
                templist = templist->next;
                continue;
            }
            if (templist->index->reach == true) {
                if (templist->index->mindistance > game[temp.index].mindistance + game[temp.index].time) {
                    templist->index->mindistance = game[temp.index].mindistance + game[temp.index].time;
                    templist->index->pathnum = game[temp.index].pathnum;
                    insect.push(*templist->index);
                } else if (templist->index->mindistance == game[temp.index].mindistance + game[temp.index].time) {
                    templist->index->pathnum = (game[temp.index].pathnum + templist->index->pathnum) % 1000000007;

                } else {
                    templist = templist->next;
                    continue;
                }

            } else {
                templist->index->reach = true;
                templist->index->pathnum = game[temp.index].pathnum;
                templist->index->mindistance = game[temp.index].mindistance + game[temp.index].time;
                insect.push(*templist->index);
            }
            templist = templist->next;
        }
}
```

如果需要的话，更新当前节点的每个邻居的距离和路径数量。注意只有在最小距离被更新或者是从不可达节点到可达节点时，我们会对这个点入堆。

### 调试与微调&问题解决

- 最初没有添加堆优化，时间复杂度太高，添加堆优化之后解决。
- 入堆的时候push函数的参数为node类型，而非一个地址，所以所传参数是当前节点情况的切片，所以需要在节点结构体中设置index指示其序号，然后检验是要使用这个序号作为索引，这样检验的才是当前的情况，否则检验的是入堆时的切片。

### 时间和空间复杂度估算

***空间复杂度大约为O(n+m)***

***时间复杂度大约为O(mlogm)***
