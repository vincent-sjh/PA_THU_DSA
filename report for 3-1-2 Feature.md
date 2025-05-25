# PA Report for 3-1-2 Feature

### 解题构思

- 最初只想到进行普通存储计算即可，但是发现测试点超时，于是开始考虑如何优化算法。
- 想到思路是建立两棵树，每棵树的节点都存储一个维度上的权重-特征值对，只不过区别是一个以权重大小为排序依据，另一个以权重绝对值大小为排序依据。
- 开始二分平衡地建立两棵树，第一种操作就搜索到对应节点的位置，将其旋转到最高位置，删除，再添加新节点，对另一颗树进行相同的操做；对操作二，搜索对应节点位置，然后进行更改，另一棵树进行相同操作。注意不要忘记更新需要维护的值，如子树规模，总和等等等。
- 最后的查询操作，将第二棵树旋转到指定位置，则根节点和右子树总和之和即为需要输出的值。

### 实现方法

### 初始化部分

```cpp
//定义全局变量，储存原始数据，即权重与样本值，之后需要的时候更新，其他时刻不变
int xin[510000];
int weight[510000];

//储存权重向量和样本向量的维数
int num;

//定义结构体，用于按照权重值或权重绝对值排序数据
struct couple {
    int weight;
    int xin;
    int order;
};

//定义节点，后续过程中进行维护，包括权重与样本值，数据位置，以及子树规模，子树输出总和，父节点，左右子节点
struct Node {
    int weight;
    int x;
    int order;
    long long int sum;
    int subtreenum;
    Node* father;
    Node* left;
    Node* right;

    //构造函数
    Node(const int& p) : weight(p), x(0), sum(0), subtreenum(0), father(nullptr), left(nullptr), right(nullptr) {}//初始化所有初始值
};

//初始化结构体数组
couple pair[510000];

//两颗树，即权重树，和权重绝对值树的根节点；
Node* rootnom;
Node* rootabs;


//在指定的范围上构造二叉搜索树
Node* BuildTree(int start, int end, Node* father) {

    //...
}
```

### 建树部分

```cpp
//在指定的范围上构造二叉搜索树
Node* BuildTree(int start, int end, Node* father) {

    //...

    node->left = BuildTree(start, median - 1, node);
    node->right = BuildTree(median + 1, end, node);

    //...
    return node;
}
```

这部分主要是 `BuildTree` 函数，该函数在指定的范围上构造二叉搜索树，采用递归的方式，每次选择中间元素作为树的根节点，并递归地构建左右子树，同时计算节点的输出总和和子树规模。

```cpp
//初始化函数，将权重向量和样本向量传入全局变量，然后按照权重值和权重绝对值排序并建树
void init(int n, const int* w, const int* x) {
    // nothing to init
    //读入数据

    //...
    qsort(pair + 1, n, sizeof(couple), compareCouples);
    rootnom = BuildTree(1, n, nullptr);
    qsort(pair + 1, n, sizeof(couple), compareCouplesabs);
    rootabs = BuildTree(1, n, nullptr);
    //...
}
```

这部分代码主要进行了初始化操作，包括定义全局变量、结构体、节点等，以及两个排序函数 `compareCouples` 和 `compareCouplesabs` 用于 `qsort` 排序。`BuildTree` 函数用于在指定范围上构造二叉搜索树，`init` 函数用于初始化权重向量和样本向量，然后按照权重值和权重绝对值排序，并建立两棵树。

### 修改树结构部分

```cpp
//zig函数,用于左旋节点，同时更新指针与子树规模，以及子树输出总和
void zig(Node* node) {
    // ...
}

//zag函数,用于右旋节点，同时更新指针与子树规模，以及子树输出总和
void zag(Node* node) {
    // ...
}

子操作即左旋操作与优选操作，主要在进行旋转操作的同时维护指针和字数规模以及总和值，用于后续构建上浮函数。

```cpp
void Emerge(Node* node) {
    while (node->father != nullptr) {
        if (node->father->father != nullptr) {
            if (node->father->father->left == node->father) {
                if (node->father->left == node) {
                    zig(node->father);
                    zig(node);
                } else {
                    zag(node);
                    zig(node);
                }
            } else {
                if (node->father->left == node) {
                    zig(node);
                    zag(node);
                } else {
                    zag(node->father);
                    zag(node);
                }
            }
            continue;
        }
        if (node->father != nullptr) {
            if (node->father->left == node) {
                zig(node);
            } else {
                assert(node->father->right == node);
                zag(node);
            }
            continue;
        }
    }
    return;
}
```

这段代码实现了Emerge函数，该函数的主要作用是将一个指定的节点上升到根节点，同时更新指针与子树规模以及子树输出总和。具体实现是通过不断地进行zig和zag操作，将当前节点上升到树的根节点。`zig` 和 `zag` 函数是左旋和右旋的操作，用于调整树的结构。之所以设置这么多分支是因为需要按照特定的分类情况和顺序执行对应的左旋和右旋操作才可以最大程度上保持树的平衡性。

### modify_weight函数

```cpp
void modify_weight(int i, int w) {
    int target = *(weight + i);
    *(weight + i) = w;

    Node* toemerge;
    Node* node = Search(rootnom, target);
    Emerge(node);
    rootnom = node;
    node->father = nullptr;
    Node* right = node->right;
    Node* left = node->left;

    if (right != nullptr)
        right->father = nullptr;
    if (left != nullptr)
        left->father = nullptr;

    if (right == nullptr) {
        rootnom = left;
        left->father = nullptr;
    } else {
        toemerge = Searchmin(right);
        Emerge(toemerge);
        rootnom = toemerge;
        toemerge->left = left;

        if (left != nullptr) {
            left->father = toemerge;
            toemerge->subtreenum = left->subtreenum + toemerge->subtreenum;
            toemerge->sum = left->sum + toemerge->sum;
        }
    }

    long long int delta;
    Node* start;
    Node* nodein = new Node(w);
    nodein->left = nullptr;
    nodein->right = nullptr;
    nodein->subtreenum = 0;
    nodein->sum = 0;
    delta = (long long int)w * (long long int)xin[i];
    nodein->x = xin[i];
    nodein->order = i;

    Node* position = Search(rootnom, w);

    if (position->weight > w)
        position->left = nodein;
    else
        position->right = nodein;

    nodein->father = position;
    start = nodein;

    do {
        start->subtreenum = start->subtreenum + 1;
        start->sum = start->sum + delta;
        start = start->father;
    } while (start != nullptr);

    Emerge(nodein);
    rootnom = nodein;

    node = Searchabs(rootabs, target);
    Emerge(node);
    rootabs = node;
    node->father = nullptr;
    right = node->right;
    left = node->left;

    if (right != nullptr)
        right->father = nullptr;
    if (left != nullptr)
        left->father = nullptr;

    if (right == nullptr) {
        rootabs = left;
        left->father = nullptr;
    } else {
        toemerge = Searchmin(right);
        Emerge(toemerge);
        rootabs = toemerge;
        toemerge->left = left;

        if (left != nullptr) {
            left->father = toemerge;
            toemerge->subtreenum = left->subtreenum + toemerge->subtreenum;
            toemerge->sum = left->sum + toemerge->sum;
        }
    }

    Node* nodeinabs = new Node(w);
    nodeinabs->left = nullptr;
    nodeinabs->right = nullptr;
    nodeinabs->subtreenum = 0;
    nodeinabs->sum = 0;
    nodeinabs->x = xin[i];
    nodeinabs->order = i;

    position = Searchabs(rootabs, w);
    if (abs(position->weight) > abs(w))
        position->left = nodeinabs;
    else
        position->right = nodeinabs;

    nodeinabs->father = position;
    start = nodeinabs;

    do {
        start->subtreenum = start->subtreenum + 1;
        start->sum = start->sum + delta;
        start = start->father;
    } while (start != nullptr);

    Emerge(nodeinabs);
    rootabs = nodeinabs;

    return;
}
```

这段代码实现了modify_weight函数，用于修改节点的权重。首先，通过Search函数找到需要修改的节点的位置，然后通过Emerge函数将该节点上升到根节点，接着根据不同情况调整树的结构，并插入新的节点，最后再将新的节点上升到根节点。整个过程涉及权重树和权重绝对值树的修改，以及对应的维护工作。

### modify_value函数

```cpp
void modify_value(int k, int x) {
    long long int form;
    long long int delta;
    Node* node = Searchorder(rootnom, num - k + 1);

    long long int weight = node->weight;
    form = (long long int)node->x;
    delta = (long long int)(x - form) * (long long int)node->weight;

    node->x = x;
    xin[node->order] = x;

    do {
        node->sum = node->sum + delta;
        node = node->father;
    } while (node != nullptr);

    node = Searchabs(rootabs, weight);
    node->x = x;

    do {
        node->sum = node->sum + delta;
        node = node->father;
    } while (node != nullptr);

    return;
}
```

这段代码实现了modify_value函数，用于修改节点的样本值。通过Searchorder函数找到需要修改的节点的位置，然后计算总和差值，并更新节点的样本值。通过循环更新所有受影响的节点的输出总和。

### calculate函数

```cpp
long long calculate(int k) {
    long long int ans;
    Node* node = Searchorder(rootabs, num - k + 1);
    Emerge(node);
    rootabs = node;
    ans = (long long int)rootabs->weight * (long long int)rootabs->x;

    if (k > 1)
        ans = ans + rootabs->right->sum;
    return ans;
}
```

这段代码实现了calculate函数，用于计算排名为第k个的节点的输出。首先，通过Searchorder函数找到指定位置的节点，然后通过Emerge函数将该节点上升到根节点，最后根据要求计算输出。

### 调试与微调&问题解决

- 定义类型初始都习惯性地设置为int,忽略了问题中数据值较大，数据超过类型范围导致报错，后续都改为long long int后成功解决。
- 进行树的改动与维护过程中发生了许多空指针没有特判的情况，导致报错，在调试过程中完成修改。
- 最后输出值的时候，如果k=1,应该只需要输出根节点乘积即可，k>1时才需要加上调整后右节点的总和，这里需要进行特判。
- 子算法的实现过程中的小问题，短暂调试后成功解决。

### 时间和空间复杂度估算

> 空间复杂度
>
>> 复杂度为O(n)的存储数组
>> 两颗复杂度为n的树
>> 其他的原始数据等（相比可忽略不计）
>>

***综上，空间复杂度大约为O(n)***

> 时间复杂度
>
>> m次均摊复杂度为O(logn)的搜索，更新，查询函数
>>

***综上，时间复杂度大约为O(mlogn)***
