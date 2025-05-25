# PA Report for 3-2-2 Kidd

### 解题构思

- 最初只想到进行常规储存更新即可，但是发现有测试点超时，而且数据范围较大，于是开始考虑如何优化算法。
- 最原始的深搜遍历复杂度为O(m*n)次方，想到可以建立线段树Segment tree。
- 具体而言，一共最多200000次操作，导致最多有400000个位置索引，而这400000个索引之间的卡牌其实可以同时看作一张，因为他们是同步操作的，无论是查询还是更新都一定一致。
- 首先读入四十万个索引位置之后，进行排序和去重获得所需维护的位置信息，之后建树，实现更新和查询操作。
- 如果每次都更新到树叶则复杂度过高，所以设置懒惰标记，被查询到时再进行下放。

### 实现方法

这段代码实现了线段树的建立和查询功能，主要用于处理一系列操作，其中包括更新和查询。下面是代码的详细解释：

1. **全局变量的定义：**

   ```cpp
   long long int n, m;
   char a[300010];
   long long int rang[300010][2];
   long long int resort[600015];
   long long int nod[600015][3];
   TreeNode* root;
   ```

   - `n`和 `m`分别表示输入的两个整数。
   - `a`是一个字符数组，用于存储命令类型。
   - `rang`是一个二维数组，存储每个命令的两个范围参数。
   - `resort`是一个数组，用于存储排序后的索引。
   - `nod`是一个二维数组，记录每个新节点实际代表的范围。
   - `root`是线段树的根节点。
2. **线段树节点结构体 `TreeNode`：**

   ```cpp
   struct TreeNode {
       long long int sum;
       long long int min;
       long long int max;
       long long int rang;
       long long int lazy;
       TreeNode* left;
       TreeNode* right;

       TreeNode(const long long int& p) :sum(p), min(0), max(0), rang(0), lazy(0), left(nullptr), right(nullptr) {}
   };
   ```

   - `sum` 存储节点的总和。
   - `min` 存储节点范围的左边界。
   - `max` 存储节点范围的右边界。
   - `rang` 存储节点范围的大小。
   - `lazy` 存储懒惰标记。
   - `left` 和 `right` 是指向左右子节点的指针。
3. **建树函数 `BuildTree`：**

   ```cpp
   TreeNode* BuildTree(int start, int end) {
       //...
   }
   ```

   该函数用于递归建立线段树。根据给定的起始和结束索引，递归地构建树结构。
4. **更新函数 `update`：**

   ```cpp
   void update(long long int small, long long int big, TreeNode* node) {
       //...
        //如果正好目标范围就是节点范围则更新节点和，之后直接加懒惰标记
        if ((small == node->min) && (big == node->max)) {
            node->sum = node->sum + node->rang;
            node->lazy = node->lazy + 1;
            return;
        }
        //...
   }
   ```

   该函数用于更新线段树节点，根据范围参数进行相应的更新。如果正好目标范围就是节点范围则更新节点和，之后直接加懒惰标记,之后不再继续向下更新。
5. **查询函数 `search`：**

   ```cpp
   long long int search(long long int small, long long int big, TreeNode* node) {
       //...
       //向下查询，如果这个节点有懒惰标记，则将懒惰标记下放一层再进行查询
        if (node->lazy != 0) {

            if (node->left != nullptr) {
                node->left->lazy = node->left->lazy + node->lazy;
                node->left->sum = node->left->sum + node->lazy * node->left->rang;
            }
            if (node->right != nullptr) {
                node->right->lazy = node->right->lazy + node->lazy;
                node->right->sum = node->right->sum + node->lazy * node->right->rang;
        }
        node->lazy = 0;
        }
        //...
   }
   ```

   该函数用于查询线段树节点，根据范围参数返回相应的查询值。如果这个节点不是与目标范围完美重合，就要向下查询，如果这个节点有懒惰标记，则将懒惰标记下放一层再进行查询。

### 调试与微调&问题解决

- 定义类型初始都习惯性地设置为int,忽略了问题中数据值较大，数据超过类型范围导致报错，后续改为long long int后成功解决
- 最初没有设置懒惰标记功能，导致每次更新都需要更新到叶子节点，导致复杂度爆炸，增加懒惰标记后大大降低计算成本。
- 最初遇到需要下放懒惰标记的时候直接下放到底层，这也浪费了计算资源，实际上只需要下放一层，如果需要继续下放的时候再接着下放即可。
- 处理原始数据排序去重过程中的边界处理有很多细节，最开始有的地方没有注意到。

### 时间和空间复杂度估算

> 空间复杂度
>
>> 几个复杂度为m的存储数组
>> 一颗复杂度为m的平衡树
>> 其他的原始数据等（相比可忽略不计）
>>

***综上，空间复杂度大约为O(m)***

> 时间复杂度
>
>> 复杂度为m的读入数据和执行操作部分,以及建树
>> m个复杂度为O(logn)的排序，更新，搜索函数
>>

***综上，时间复杂度大约为O(mlogn)***
