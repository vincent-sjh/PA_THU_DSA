#include<iostream>
#include<cstdlib>
using namespace std;

//初始化变量
long long int n, m;

//舒适化代表命令种类的字符串数组
char a[300010];
//每个命令的两个范围参数
long long int rang[300010][2];

//重排之后的数组
long long int resort[600015];

//记录每个新节点实际代表的范围
long long int nod[600015][3];

//比较函数，传递给qsort
int compare(const void* a, const void* b) {
    // 将void指针转换为long long int指针，然后比较其值
    return (*(long long int*)a - *(long long int*)b);
}

//线段树结构体，每个节点存储总和，范围左右界和范围大小，两个子代地址，以及懒惰标记
struct TreeNode {
    long long int sum;//总和
    long long int min;//范围左界
    long long int max;//范围右界
    long long int rang;//范围大小
    long long int lazy;//懒惰标记

    TreeNode* left;//长子
    TreeNode* right;//次子

    TreeNode(const long long int& p) :sum(p), min(0), max(0), rang(0), lazy(0), left(nullptr), right(nullptr) {}//初始化所有初始值

};

//初始化线段树树根
TreeNode* root;

//建立线段树
TreeNode* BuildTree(int start, int end) {

    //特判特殊情况，范围无意义则返回空指针
    if (start > end) {
        return nullptr;
    }

    int median = (start + end) / 2;

    //初始化树节点
    TreeNode* node = new TreeNode(0);
    node->min = nod[start][0];
    node->max = nod[end][1];
    node->rang = node->max - node->min + 1;

    if (start == end) {
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

    //建立左右子树
    node->left = BuildTree(start, median);

    //建立左右子树
    node->right = BuildTree(median + 1, end);

    return node;
}

//更新线段树
void update(long long int small, long long int big, TreeNode* node) {

    if (node == nullptr)
        return;
    //如果正好目标范围就是节点范围则更新节点和，之后直接加懒惰标记
    if ((small == node->min) && (big == node->max)) {
        node->sum = node->sum + node->rang;
        node->lazy = node->lazy + 1;
        return;
    }

    long long int toleft, toright;
    //如果目标范围和节点范围无交集，直接返回,否则更新节点总和
    if (small > node->max || big < node->min) {
        return;
    }

    toleft = node->min;
    toright = node->max;
    if (small > toleft) {
        toleft = small;
    }
    if (big < toright) {
        toright = big;
    }
    node->sum = node->sum + (toright - toleft + 1);

    //如果左子树非空，且范围和目标范围有交集，则更新左子树
    if ((small <= node->left->max) && (node->left != nullptr)) {
        toleft = node->left->min;
        toright = node->left->max;
        if (small > toleft) {
            toleft = small;
        }
        if (big < toright) {
            toright = big;
        }
        update(toleft, toright, node->left);
    }

    //如果右子树非空，且范围和目标范围有交集，则更新右子树
    if ((big >= node->right->min) && (node->right != nullptr)) {
        toleft = node->right->min;
        toright = node->right->max;
        if (small > toleft) {
            toleft = small;
        }
        if (big < toright) {
            toright = big;
        }
        update(toleft, toright, node->right);
    }
}

//查询求值函数
long long int search(long long int small, long long int big, TreeNode* node) {

    if (node == nullptr)
        return 0;

    //如果目标范围就是节点范围直接返回
    if ((small == node->min) && (big == node->max)) {
        return node->sum;
    }

    //如果目标范围和节点范围无交集，直接返回0
    if (small > node->max || big < node->min) {
        return 0;
    }

    //否则就要向下查询，如果这个节点有懒惰标记，则将懒惰标记下放一层再进行查询
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

    long long int toleft, toright, ans = 0;


    //如果左子树非空，且范围和目标范围有交集，向左子树查询,更新返回值
    if ((small <= node->left->max) && (node->left != nullptr)) {
        toleft = node->left->min;
        toright = node->left->max;
        if (small > toleft) {
            toleft = small;
        }
        if (big < toright) {
            toright = big;
        }
        ans = ans + search(toleft, toright, node->left);
    }

    //如果右子树非空，且范围和目标范围有交集，向右子树查询,更新返回值
    if ((big >= node->right->min) && (node->right != nullptr)) {
        toleft = node->right->min;
        toright = node->right->max;
        if (small > toleft) {
            toleft = small;
        }
        if (big < toright) {
            toright = big;
        }
        ans = ans + search(toleft, toright, node->right);
    }

    return ans;

}

int main() {

    //节约输入输出时间
    ios::sync_with_stdio(false);
    cin.tie(0);

    //读入原始数据
    cin >> n >> m;
    for (int i = 1;i <= m;i++) {
        cin >> a[i] >> rang[i][0] >> rang[i][1];
        resort[2 * i - 1] = rang[i][0];
        resort[2 * i] = rang[i][1];
    }

    //将所有索引排序
    qsort(resort + 1, 2 * m, sizeof(long long int), compare);

    //去重
    long long int index = 1;

    for (int i = 1;i <= 2 * m;i++) {
        if (resort[i] != resort[i - 1]) {
            resort[index] = resort[i];
            index++;
        }
    }

    long long int len;
    len = index - 1;
    index = 1;

    //由去重后数组获得线段树节点范围信息并储存
    if (resort[1] > 1) {
        nod[index][0] = 1;
        nod[index][1] = resort[1] - 1;
        nod[index][2] = resort[1] - 1;
        index++;
    }

    for (int i = 1;i <= len;i++) {

        nod[index][0] = resort[i];
        nod[index][1] = resort[i];
        nod[index][2] = 1;
        index++;

        if (i == len) break;
        if (resort[i + 1] - resort[i] >= 2) {
            nod[index][0] = resort[i] + 1;
            nod[index][1] = resort[i + 1] - 1;
            nod[index][2] = resort[i + 1] - resort[i] - 1;
            index++;
        }
    }
    if (resort[len] + 1 <= n) {
        nod[index][0] = resort[len] + 1;
        nod[index][1] = n;
        nod[index][2] = n - resort[len];
        index++;
    }

    index = index - 1;

    //建树
    root = BuildTree(1, index);

    //依次执行各项操作
    for (int i = 1;i <= m;i++) {
        if (a[i] == 'H') {
            update(rang[i][0], rang[i][1], root);
        } else {
            cout << search(rang[i][0], rang[i][1], root) << '\n';
        }
    }

    return 0;


}