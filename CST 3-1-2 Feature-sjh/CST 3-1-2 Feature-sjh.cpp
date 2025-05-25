#include "feature.h"
#include <cmath>
#include <cassert>
#include <cstdlib>

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
    Node(const int& p) :weight(p), x(0), sum(0), subtreenum(0), father(nullptr), left(nullptr), right(nullptr) {}//初始化所有初始值

};

//初始化结构体数组
couple pair[510000];

//两颗树，即权重树，和权重绝对值树的根节点；
Node* rootnom;
Node* rootabs;

//传递给qsort的比较依据函数，用于按照权重排序结构体数组
int compareCouples(const void* a, const void* b) {
    const couple* coupleA = static_cast<const couple*>(a);
    const couple* coupleB = static_cast<const couple*>(b);
    // 按照 weight 升序排序
    return coupleA->weight - coupleB->weight;
}

//传递给qsort的比较依据函数，用于按照权重绝对值排序结构体数组
int compareCouplesabs(const void* a, const void* b) {
    const couple* coupleA = static_cast<const couple*>(a);
    const couple* coupleB = static_cast<const couple*>(b);
    // 按照 weight 升序排序
    return abs(coupleA->weight) - abs(coupleB->weight);
}

//在指定的范围上构造二叉搜索树
Node* BuildTree(int start, int end, Node* father) {

    if (start > end) {
        return nullptr;
    }

    int median = (start + end) / 2;
    Node* node = new Node(pair[median].weight);
    node->order = pair[median].order;
    node->x = pair[median].xin;
    node->subtreenum = end - start + 1;
    node->father = father;

    node->left = BuildTree(start, median - 1, node);
    node->right = BuildTree(median + 1, end, node);

    node->sum = (long long int)(node->weight) * (long long int)(node->x);
    if (node->left != nullptr) {
        node->sum += node->left->sum;
    }
    if (node->right != nullptr) {
        node->sum += node->right->sum;
    }
    return node;
}

//初始化函数，将权重向量和样本向量传入全局变量，然后按照权重值和权重绝对值排序并建树
void init(int n, const int* w, const int* x) {
    // nothing to init
    for (int i = 1;i <= n;i++) {
        xin[i] = *(x + i);
        pair[i].weight = *(w + i);
        weight[i] = *(w + i);
        pair[i].xin = *(x + i);
        pair[i].order = i;
    }

    num = n;
    qsort(pair + 1, n, sizeof(couple), compareCouples);
    rootnom = BuildTree(1, n, nullptr);
    qsort(pair + 1, n, sizeof(couple), compareCouplesabs);
    rootabs = BuildTree(1, n, nullptr);
}


//zig函数,用于左旋节点，同时更新指针与子树规模，以及子树输出总和
void zig(Node* node) {

    if (node->right != nullptr) {
        node->right->father = node->father;
    }
    node->father->left = node->right;

    node->right = node->father;
    node->father = node->right->father;
    node->right->father = node;
    if (node->father != nullptr) {

        if (node->father->left == node->right) {
            node->father->left = node;
        } else {
            node->father->right = node;
        }
    }

    node->sum = node->right->sum;
    node->right->sum = node->sum - (long long int)node->weight * (long long int) node->x;

    node->subtreenum = node->right->subtreenum;
    node->right->subtreenum = node->subtreenum - 1;

    if (node->left != nullptr) {
        node->right->subtreenum = node->right->subtreenum - node->left->subtreenum;
        node->right->sum = node->right->sum - node->left->sum;
    }
    return;
}

//zag函数,用于右旋节点，同时更新指针与子树规模，以及子树输出总和
void zag(Node* node) {

    if (node->left != nullptr) {
        node->left->father = node->father;
    }
    node->father->right = node->left;

    node->left = node->father;
    node->father = node->left->father;
    node->left->father = node;
    if (node->father != nullptr) {

        if (node->father->right == node->left) {
            node->father->right = node;
        } else {
            node->father->left = node;
        }
    }

    node->subtreenum = node->left->subtreenum;
    node->left->subtreenum = node->subtreenum - 1;

    node->sum = node->left->sum;
    node->left->sum = node->sum - (long long int)node->weight * (long long int)node->x;
    if (node->right != nullptr) {
        node->left->subtreenum = node->left->subtreenum - node->right->subtreenum;
        node->left->sum = node->left->sum - node->right->sum;
    }
    return;
}

//Emerge函数，用于将节点上升到根节点，同时更新指针与子树规模，以及子树输出总和
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

//搜索函数，在权重树上搜索节点的权重，如果搜索不到则返回这个权值应当插入的位置
Node* Search(Node* root, int w) {
    if (root == nullptr)
        return nullptr;
    if (root->weight == w) {
        return root;
    } else if (w < root->weight) {
        if (root->left != nullptr)
            return Search(root->left, w);
        else
            return root;
    } else {
        if (root->right != nullptr)
            return Search(root->right, w);
        return root;
    }
}

//搜索函数，在绝对值树上搜索节点的绝对值，如果搜索不到则返回这个权重应当插入的位置
Node* Searchabs(Node* root, int w) {
    if (root == nullptr)
        return nullptr;
    if (abs(root->weight) == abs(w)) {
        return root;
    } else if (abs(w) < abs(root->weight)) {
        if (root->left != nullptr)
            return Searchabs(root->left, w);
        else
            return root;
    } else {
        if (root->right != nullptr)
            return Searchabs(root->right, w);
        return root;
    }
}

//搜索函数，在权重树和权重绝对值树上搜索“最小”节点，在权重树上返回的是最小权值点，在绝对值树上返回的是最小绝对值点
Node* Searchmin(Node* root) {
    if (root == nullptr)
        return nullptr;
    if (root->left == nullptr)
        return root;
    else
        return Searchmin(root->left);
}

//搜索函数，在顺序树上搜索排名为第k个的节点
Node* Searchorder(Node* root, int k) {
    if (root == nullptr)
        return nullptr;
    if (root->left == nullptr) {
        if (k == 1)
            return root;
        else
            return Searchorder(root->right, k - 1);
    } else {
        if (k <= root->left->subtreenum)
            return Searchorder(root->left, k);
        else if (k == root->left->subtreenum + 1)
            return root;
        else
            return Searchorder(root->right, k - root->left->subtreenum - 1);
    }
}

//修改函数，将目标节点上升到根节点，删除它，重新连接树，然后将新节点插入适当的位置，再把这个新节点上升到根节点
void modify_weight(int i, int w) {
    // nothing to modify
    //读入要修改的节点的权重，作为搜索依据
    int target = *(weight + i);
    //在全局变量中修改节点的权重
    *(weight + i) = w;
    //在全局变量中修改节点的绝对值    
    Node* toemerge;
    //搜索到需要修改的节点的位置
    Node* node = Search(rootnom, target);
    //将其升到根位置
    Emerge(node);

    //更新权重树根节点
    rootnom = node;
    node->father = nullptr;

    //找到新根节点的左右子树    
    Node* right = node->right;
    Node* left = node->left;

    //删除原本的根节点
    if (right != nullptr)
        right->father = nullptr;
    if (left != nullptr)
        left->father = nullptr;

    //如果右侧为空树，直接以左子树为新树
    if (right == nullptr) {
        rootnom = left;
        left->father = nullptr;

    } else {
        //否则在右子树中找到最小值
        toemerge = Searchmin(right);

        //将其升到根位置
        Emerge(toemerge);
        rootnom = toemerge;
        toemerge->left = left;

        //更新权重树总和与字数规模
        if (left != nullptr) {
            left->father = toemerge;
            toemerge->subtreenum = left->subtreenum + toemerge->subtreenum;
            toemerge->sum = left->sum + toemerge->sum;
        }
    }

    long long int delta;
    Node* start;

    //初始化新节点
    Node* nodein = new Node(w);
    nodein->left = nullptr;
    nodein->right = nullptr;
    nodein->subtreenum = 0;
    nodein->sum = 0;
    //设置需要更新的总和值
    delta = (long long int) w * (long long int)xin[i];

    nodein->x = xin[i];
    nodein->order = i;

    //找到插入位置
    Node* position = Search(rootnom, w);
    if (position->weight > w)
        position->left = nodein;
    else
        position->right = nodein;
    nodein->father = position;
    //更新树根地址，权重树总和与字数规模
    start = nodein;

    do {
        start->subtreenum = start->subtreenum + 1;
        start->sum = start->sum + delta;
        start = start->father;

    } while (start != nullptr);

    //将新节点升到最高位置
    Emerge(nodein);
    rootnom = nodein;

    //对权重绝对值树进行完全相同的操作与维护
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

void modify_value(int k, int x) {
    // nothing to modify
    long long int form;
    long long int delta;

    //在权重树中搜索所要修改的点的位置
    Node* node = Searchorder(rootnom, num - k + 1);

    //找到所需修改点的总和差值
    long long int weight = node->weight;
    form = (long long int)node->x;
    delta = (long long int)(x - form) * (long long int)node->weight;

    //更新数据
    node->x = x;
    xin[node->order] = x;
    do {
        node->sum = node->sum + delta;
        node = node->father;
    } while (node != nullptr);
    //在权重树中搜索所要修改的点的位置
    node = Searchabs(rootabs, weight);
    node->x = x;
    do {
        node->sum = node->sum + delta;
        node = node->father;
    } while (node != nullptr);
    return;
}

long long calculate(int k) {

    long long int ans;
    //在权重树中搜索绝对值大小排在指定位置的点的位置
    Node* node = Searchorder(rootabs, num - k + 1);

    //将其升到根节点
    Emerge(node);
    rootabs = node;

    //计算答案
    ans = (long long int)rootabs->weight * (long long int)rootabs->x;

    //特判如果只需计算一个值则不需要要添加右节点输出总和
    if (k > 1)
        ans = ans + rootabs->right->sum;
    return ans;
}
