# PA Report for 4-4 Component

### 解题构思

- 最初只想到进行普通遍历即可，但是发现九成测之后有测试点超时，于是开始考虑如何优化算法。
- 后来想到可以维护一个并查集用于迅速确定两个点是否在一个连通分支。
- 同时维护一个可以快速合并的左式堆。
- 为了快速确定节点对应的左氏堆，建立一个表单，用于从并查集祖先到左式堆祖先的映射，这个表单需要更新。
- 由于查询的是连通分支中第k大的节点，所以左式堆中只需要维护最多k个节点，其余的可以舍去。
- 接收到两个点后只需验证二者是否在一个连通分支中，如果在则忽略。不在则更新左氏堆，更新并查集，更新祖先映射表单即可。
- 查询操作就检验左式堆大小，不足k个说明连通支节点数量不足k个，否则输出堆顶权值即可。
- 用存储空间换取计算复杂度的优化。

### 实现方法

### 1. 节点结构体定义

```cpp
struct node {
    int index;
    long long int weight;
    int subtreenum;
    int distance;
    node* left;
    node* right;
    node* father;
    int level;
};
```

定义了一个结构体 `node`，表示树的节点。每个节点包含权重 `weight`、子树节点数量 `subtreenum`、距离 `distance`、左右子节点指针、父节点指针 `father` （这里的父节点用于维护并查集，而非左氏堆）和层级规模 `level`（用于指示并查集中集合的大小，根据这个指标合并集合可以保证并查集相对平衡，深度相对会更低）。

### 2. 辅助变量

```cpp
node point[1200000];
node* tableset[1200000];
node* create;
node* crea;
node* process;
node* f1;
node* f2;
```

定义了一系列全局节点数组和指针，用于存储树节点、树集合、合并操作中的中间节点等。

### 3. Find 函数

```cpp
node* Find(node* point) {
    assert(point != nullptr);
    if (point->father == point) {
        return point;
    }
    point->father = Find(point->father);
    assert(point->father);
    return point->father;
}
```

实现了并查集的查找操作，用于找到节点所属的集合的根节点，同时在这个过程中还进行了路径压缩，尽可能降低了深度，使得搜索尽可能地迅速。

### 4. Union 函数

```cpp
void Union(node* point1, node* point2) {
    f1 = Find(point1);
    f2 = Find(point2);
    if (f1->level >= f2->level) {
        f2->father = f1;
        f1->level += f2->level;
    } else {
        f1->father = f2;
        f2->level += f1->level;
    }
}
```

实现了合并操作，用于将两个集合合并，通过按秩合并和路径压缩优化。

### 5. Merge 函数

```cpp
node* Merge(node* point1, node* point2) {
    // 实现节点合并
    // ...
}
```

实现了合并两个节点所在的左氏堆操作。

### 6. Merge_sizek 函数

```cpp
node* Merge_sizek(node* point1, node* point2) {
    create = Merge(point1, point2);
    length = create->subtreenum;
    if (length <= k) {
        return create;
    } else {
        process = create;
        // 修剪树，保留前 k 小的节点
        // ...
        return process;
    }
}
```

在 Merge 的基础上，保留了前 k 小的节点，通过修剪树的方式实现，这里的出堆操作不需要单独再写，只需要合并左右子树即可。

### 调试与微调&问题解决

- 由于左式堆和并查集都使用指针实现，所以开始出现了大量空指针引用的细节问题，调试后解决。
- 应该先用并查集寻找左式堆堆顶，更新左式堆之后再更新并查集，最后更新映射表单。开始的更新顺序出现了错误，发现逻辑问题后调试解决。

### 时间和空间复杂度估算

***空间复杂度大约为O(n)***

***时间复杂度大约为O((m+q)logn)***
