#include <cmath>
#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>
#include <iostream>
#include<ctime>

using namespace std;

#define debug(...) fprintf(stderr, __VA_ARGS__)


//储存权重向量和样本向量的维数
int num;


//定义节点，后续过程中进行维护，包括权重与样本值，数据位置，以及子树规模，子树输出总和，父节点，左右子节点
struct Node {
    int weight;

    Node* father;
    Node* left;
    Node* right;

    //构造函数
    Node(const int& p) :weight(p), father(nullptr), left(nullptr), right(nullptr) {}//初始化所有初始值

};

Node* rootnom;


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


//搜索函数，在权重树和权重绝对值树上搜索“最小”节点，在权重树上返回的是最小权值点，在绝对值树上返回的是最小绝对值点
Node* Searchmin(Node* root) {
    if (root == nullptr)
        return nullptr;
    if (root->left == nullptr)
        return root;
    else
        return Searchmin(root->left);
}


//修改函数，将目标节点上升到根节点，删除它，重新连接树，然后将新节点插入适当的位置，再把这个新节点上升到根节点
void add(int w) {

    //初始化新节点
    Node* nodein = new Node(w);

    nodein->left = nullptr;
    nodein->right = nullptr;

    if (rootnom == nullptr) {
        rootnom = nodein;
        return;
    }

    //找到插入位置
    Node* position = Search(rootnom, w);
    if (position->weight > w)
        position->left = nodein;
    else
        position->right = nodein;
    nodein->father = position;

    //将新节点升到最高位置
    Emerge(nodein);
    rootnom = nodein;

    return;
}

void delet(long long int keyy) {
    // nothing to modify
    //读入要修改的节点的权重，作为搜索依据

    //在全局变量中修改节点的绝对值    
    Node* toemerge;
    //搜索到需要修改的节点的位置
    Node* node = Search(rootnom, keyy);

    if (node == nullptr)
        return;
    //搜索到的节点的权重必须与keyy相等
    if (node->weight != keyy) {
        return;
    }

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
        if (left != nullptr)
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
        }
    }
}

long long int ques(Node* root, long long int keyy) {


    if (root == nullptr)
        return -1;
    if (root->weight <= keyy) {
        if (root->right == nullptr)
            return root->weight;
        else {

            int answer = ques(root->right, keyy);
            if (answer > root->weight)
                return answer;
            else
                return root->weight;
        }
    } else {
        if (root->left == nullptr)
            return -1;
        else {
            return ques(root->left, keyy);
        }
    }
}

int main() {


    //解绑输入输出缓冲区，节省时间
    ios::sync_with_stdio(false);
    cin.tie(0);

    freopen("splay.txt", "w", stdout);
    freopen("1in.txt", "r", stdin);

    int n, keyy;
    char s;

    double t1 = clock();

    cin >> n;

    for (long long int i = 0;i < n;i++) {
        cin >> s >> keyy;
        if (s == 'A') {
            add(keyy);

        } else if (s == 'B') {
            delet(keyy);

        } else {
            Node* toemer = Search(rootnom, keyy);
            if (toemer == nullptr) {
                cout << -1 << '\n';
            } else {
                Emerge(toemer);
                rootnom = toemer;
                cout << ques(rootnom, keyy) << '\n';
            }
        }
    }

    double t2 = clock();
    double result = (t2 - t1);
    cout << "Total time of Splay TREE: " << result << "ms" << '\n';

    return 0;
}
