# PA Report for 3-3-2 Nearest Neighbor

### 解题构思

- 最初只想到进行遍历即可，但是发现测试点超时，于是开始考虑如何优化算法。
- 最原始的遍历复杂度为O(n)次方，想到建立KD-tree，优化搜索过程。建树过程中需要更换每次作为二分依据的分量位置（维数），尽可能使得分类平衡。
- 可以给每个树节点建立高维盒更好的计算目标节点与其的最长距离，但发现优化效果不明显所以后续删除相关部分。

### 实现方法

### 数据点结构体 - `Point`：

```cpp
struct Point {
    int coordinates[5];
};
```

- **目的和功能：**
  - 存储多维度数据点的坐标。

### KD树节点结构体 - `KDNode`：

```cpp
struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;
    int depth;
    int axis;

    KDNode(const Point& p) : point(p), left(nullptr), right(nullptr), depth(0), axis(0) {}
};
```

- **目的和功能：**
  - 存储KD树的节点信息，包括数据点、左右子节点、深度、以及当前划分轴。
  - 构造函数用于初始化节点。

### KD树构建函数 - `buildKDTree`：

```cpp
KDNode* buildKDTree(int start, int end, int depth, int dimension) {
    //...
    //根据深度，更新这一层排序划分所要使用的维数
    int axis = depth % dimension;
    buildaxis = axis;

    // 使用qsort对学生数组按照分数进行排序
    qsort(&points[start], end - start + 1, sizeof(points[0]), compare);
    int median = (start + end) / 2;
    //...
}
```

- **目的和功能：**
  - 递归构建KD树。
  - 根据深度和轴对数据点进行排序，选择中位数作为当前节点，这样可以尽可能使得左右子树规模相同，使得树尽可能平衡。
  - 递归构建左右子树。

### 最近邻点查询函数 - `findNearestNeighbor`：

```cpp
Point findNearestNeighbor(KDNode* root, const Point& target, int depth, int dimension) {
    //...
    //找到所查询目标节点在分类轴的哪一侧，将这一侧的子树设为当前节点
    KDNode* nextBranch = (target.coordinates[axis] < root->point.coordinates[axis]) ? root->left : root->right;
    KDNode* otherBranch = (nextBranch == root->left) ? root->right : root->left;

    //...
    //查询目标点所在一侧的最优节点
    if (nextBranch != nullptr) {
        //...搜索这颗子树
    }

    // 检查另一侧是否有更近的点，只有满足目标节点这一维和划分轴最短距离小于当前最优结果才会查询
    long long int delta = (long long int)target.coordinates[axis] - (long long int)root->point.coordinates[axis];
    if ((delta * delta < besting) && (otherBranch != nullptr)) {
        //...搜索这颗子树
    }
    //...
}
```

- **目的和功能：**
  - 递归查询KD树中目标点的最近邻点。
  - 通过比较轴的分量选择搜索方向。找到所查询目标节点在分类轴的哪一侧，将这一侧的子树设为当前节点。检查另一侧是否有更近的点，只有满足目标节点这一维和划分轴最短距离小于当前最优结果才会查询。
  - 注意不要忘记这个节点本身对应的点也要进行距离计算和更新。
  - 使用全局变量 `besting` 记录当前最优距离。

### 全局变量 - `buildaxis` 和 `besting`：

- **目的和功能：**
  - `buildaxis` 保存KD树建树时所用的分量位置，用于比较函数中的轴选择。
  - `besting` 保存目前最好结果的距离平方。

### 调试与微调&问题解决

- 定义类型初始都习惯性地设置为int,忽略了问题中数据值较大，数据超过类型范围导致报错，后续都改为long long int后成功解决。
- 最初设置了包围盒进行优化查询，本以为可以进行更彻底的剪枝，但实际上反倒增加了计算成本，优化效果不明显，所以最终版本中删除了这个功能。
- 子算法的实现过程中的小问题，短暂调试后成功解决。

### 时间和空间复杂度估算

> 空间复杂度
>
>> 复杂度为O(n*d)的存储数组
>> 一颗复杂度为n的KD-tree树
>> 其他的原始数据等（相比可忽略不计）
>>

***综上，空间复杂度大约为O(n*d)***

> 时间复杂度
>
>> 复杂度为o(n*（logn*logn）)的建树
>>
>> m次均摊复杂度为O(n^(1-1/d)))的搜索最小值函数
>>

***综上，时间复杂度大约为O(mn^(1-1/d)+n*（logn***logn）)*
