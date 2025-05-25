#include <iostream>
#include <cassert>
#include <utility>

using namespace std;

//取最大值函数，用于更新一个节点自身及所有弟弟的最大高度
int Max(int a, int b) {
    return (a > b) ? a : b;
}

//树的节点结构体，存有子树规模，高度，弟弟（含自身）最大高度，最小及最大后代地址，父节点地址，紧邻兄弟地址
struct TreeNode {
    int subtreenum;//子树规模
    int height;//高度
    int youngheight;//弟弟（含自身）最大高度

    TreeNode* mother;//父节点

    TreeNode* oldbrother;//临近哥哥
    TreeNode* youngbrother;//临近弟弟

    TreeNode* firstchild;//幼子
    TreeNode* lastchild;//长子

    TreeNode() :subtreenum(-1), height(-1), youngheight(-1), mother(nullptr), oldbrother(nullptr), youngbrother(nullptr), firstchild(nullptr), lastchild(nullptr) {}//初始化所有初始值

};

//从某个节点开始更新高度以及最大弟弟高度
void Updateheight(TreeNode* child) {
    int tempheight;
    TreeNode* stage = child;//存储初始位置用于判定
    while (1) {
        //持续循环
        if (child->youngbrother == nullptr) {
            tempheight = child->height;//如果没有弟弟，为幼子，最大弟弟高度为自己的高度
        } else {
            tempheight = Max(child->youngbrother->youngheight, child->height);//有弟弟时，最大弟弟高度为自身高度与近邻弟弟的最大弟弟高度中的更大值
        }
        if ((tempheight == child->youngheight) && (child != stage)) {
            return;//如果当前节点不是初始更新节点（初始节点最大弟弟高度没有变化也可能导致其祖先数据被更新），且最大弟弟高度根本没有变化，则停止进程
        } else {
            child->youngheight = tempheight;//否则更新最大弟弟高度
        }

        if (child->mother == nullptr) {
            return;//如果已经回溯至根部，终止进程
        }

        if (child->oldbrother == nullptr) {
            //更新至某节点长子
            child->mother->height = tempheight + 1;//将其父亲高度更新
            Updateheight(child->mother);//递归调用函数从其父亲开始继续更新
            return;
        }
        child = child->oldbrother;//以上情况均不成立则继续更新当前节点的哥哥
    }
    return;
}

//在某个位置插入一棵子树
void InsertAtPosition(int position, TreeNode* child, TreeNode* dad) {

    if ((dad->firstchild == nullptr) && (dad->lastchild == nullptr)) {
        //父亲之前没有任何后代时
        child->youngbrother = nullptr;
        child->oldbrother = nullptr;
        child->mother = dad;

        dad->firstchild = child;
        dad->lastchild = child;
    } else {
        if (position == 0) {
            //如果加在所有节点的最前面时
            dad->firstchild->youngbrother = child;
            child->oldbrother = dad->firstchild;
            child->youngbrother = nullptr;
            dad->firstchild = child;
            child->mother = dad;

        } else {
            //循环遍历获得其弟弟的位置
            TreeNode* current = dad->firstchild;
            for (int i = 1;i < position;i++) {
                current = current->oldbrother;
            }
            if (current->oldbrother != nullptr) {
                //这个弟弟没有其他哥哥，即他原来是长子时
                child->youngbrother = current;
                child->oldbrother = current->oldbrother;
                current->oldbrother->youngbrother = child;
                current->oldbrother = child;
                child->mother = dad;
            } else {
                //加入的位置在中间，即加入后有哥哥与弟弟；
                dad->lastchild->oldbrother = child;
                child->youngbrother = dad->lastchild;
                child->oldbrother = nullptr;
                dad->lastchild = child;
                child->mother = dad;
            }
        }
    }

    //循环找到所有祖先，更新其子树规模
    while (dad != nullptr) {
        dad->subtreenum += child->subtreenum;
        dad = dad->mother;
    }

    //调用高度更新函数，从当前加入的节点开始更新
    Updateheight(child);

    return;
}

//删去一棵子树
void DeleteAtPosition(TreeNode* child, TreeNode* dad) {

    //断言父节点必定有后代

    assert((dad->firstchild != nullptr) && (dad->lastchild != nullptr));
    if (dad->firstchild->oldbrother == nullptr) {
        //如果父亲只有这一个将要被删去的子节点
        dad->firstchild = nullptr;
        dad->lastchild = nullptr;
    } else {
        if (child->youngbrother == nullptr) {
            //如果删去的节点没有弟弟，即是父节点最年轻的后代
            dad->firstchild = child->oldbrother;
            dad->firstchild->youngbrother = nullptr;
        } else {
            if (child->oldbrother == nullptr) {
                //如果删去的节点没有哥哥，即是父节点的长子
                dad->lastchild = child->youngbrother;
                dad->lastchild->oldbrother = nullptr;
            } else {
                //删去的节点位于中间
                child->youngbrother->oldbrother = child->oldbrother;
                child->oldbrother->youngbrother = child->youngbrother;
            }
        }
    }

    //循环更新子树规模
    while (dad != nullptr) {
        dad->subtreenum -= child->subtreenum;
        dad = dad->mother;
    }


    if (child->oldbrother != nullptr) {
        //如果节点被删除前有哥哥，直接从这个哥哥开始更新
        Updateheight(child->oldbrother);
    } else if (child->youngbrother != nullptr) {
        //如果节点被删除前没有哥哥但有弟弟，直接更新父亲的告诉并从父亲开始更新
        child->mother->height = child->youngbrother->youngheight + 1;
        Updateheight(child->mother);
    } else {
        //如果被删除前没有哥哥也没有弟弟，将父亲高度归零后从父亲开始更新
        child->mother->height = 0;
        Updateheight(child->mother);
    }
    return;
}

TreeNode* FindLocation(TreeNode* root) {

    int length, index, remain, bin;
    TreeNode* current = root;

    TreeNode* temp;//实验品，确认实验品安全后才会赋值给当前节点
    cin >> length;
    remain = length;//计数器，回收多余输入
    for (int i = 1;i <= length;i++) {
        cin >> index;
        remain = remain - 1;
        if ((current->firstchild == nullptr) || (current->lastchild == nullptr)) {
            //当前节点无后代时，回收多于输入后直接输出
            for (int i = 0;i < remain;i++) {
                cin >> bin;
            }
            return current;
        }
        //用实验品进行查找
        temp = current->firstchild;
        for (int j = 0;j < index;j++) {
            //如果中途发现非法直接回收输入后返回当前节点
            if (temp->oldbrother == nullptr) {
                for (int i = 0;i < remain;i++) {
                    cin >> bin;
                }
                return current;
            }
            temp = temp->oldbrother;
        }

        //路径合法，用实验品给当前节点赋值
        current = temp;

        //当前节点无后代直接返回
        if ((current->firstchild == nullptr) || (current->lastchild == nullptr)) {
            break;
        }
    }
    //回收输入
    for (int i = 0;i < remain;i++) {
        cin >> bin;
    }
    return current;
}

// 定义结构体表示栈，模拟系统栈进程，节省栈空间
struct Stack {
    TreeNode* elements[1200000]; // 固定大小的数组
    int topIndex;// 栈顶索引

    // 构造函数，初始化栈顶索引
    Stack() : topIndex(0) {}

    // 入栈操作
    void push(TreeNode* p) {
        topIndex++;
        elements[topIndex] = p;
    }

    // 出栈操作
    TreeNode* pop() {
        topIndex--;
        return elements[topIndex + 1];
    }

    // 获取栈顶元素
    TreeNode* top() {
        return elements[topIndex];
    }

    //清空栈
    void clear() {
        topIndex = 0;
    }

    // 判断栈是否为空
    bool isEmpty() {
        return topIndex == 0;
    }

    // 获取栈的大小
    int size() const {
        return topIndex;
    }
};

//初始化栈
Stack myStack;

//初始化树节点
TreeNode node[1200000];

int main() {

    //节约输入输出时间
    ios::sync_with_stdio(false);
    cin.tie(0);

    //读入并初始化节点地址数据
    int n, m, num, sub, tem;
    cin >> n >> m;
    for (int i = 1;i <= n;i++) {
        cin >> num;
        if (num == 1) {
            cin >> sub;
            node[sub].mother = &node[i];
            node[i].firstchild = &node[sub];
            node[sub].youngbrother = nullptr;
            node[i].lastchild = &node[sub];
            node[sub].oldbrother = nullptr;
        } else {
            for (int j = 1;j <= num;j++) {
                cin >> sub;
                if (j == 1) {
                    node[sub].mother = &node[i];
                    node[i].firstchild = &node[sub];
                    node[sub].youngbrother = nullptr;
                } else {
                    if (j == num) {
                        node[i].lastchild = &node[sub];
                        node[sub].oldbrother = nullptr;
                        node[tem].oldbrother = &node[sub];
                        node[sub].youngbrother = &node[tem];
                        node[sub].mother = &node[i];
                    } else {
                        node[sub].youngbrother = &node[tem];
                        node[tem].oldbrother = &node[sub];
                        node[sub].mother = &node[i];
                    }
                }
                tem = sub;
            }
        }
    }


    TreeNode* temp;
    TreeNode* roll;
    int sum;
    myStack.push(&node[1]);

    //用人工栈模拟进程，同时初始化子树规模与高度
    while (!myStack.isEmpty()) {
        //如果栈顶元素是叶节点没有后代，直接完成数据更新后将其出栈
        if (myStack.top()->lastchild == nullptr) {
            temp = myStack.top();
            temp->height = 0;
            temp->subtreenum = 1;
            if (temp->youngbrother == nullptr) {
                temp->youngheight = 0;
            } else {
                temp->youngheight = temp->youngbrother->youngheight;
            }
            assert(!myStack.isEmpty());
            myStack.pop();
        } else {
            //如果顶元素长子已经更新过且不是叶节点，直接依据其数据更新栈顶元素，并将其出栈
            if (myStack.top()->lastchild->height != -1) {
                temp = myStack.top();
                temp->height = temp->lastchild->youngheight + 1;
                if (temp->youngbrother == nullptr) {
                    temp->youngheight = temp->height;
                } else {
                    temp->youngheight = Max(temp->height, temp->youngbrother->youngheight);
                }
                roll = temp->lastchild;
                sum = 0;
                while (roll != nullptr) {
                    sum = sum + roll->subtreenum;
                    roll = roll->youngbrother;
                }
                temp->subtreenum = sum + 1;
                assert(!myStack.isEmpty());
                myStack.pop();

            } else {
                //如果栈顶元素长子没有被更新过，直接从长至幼将其所有后代入栈
                temp = myStack.top()->lastchild;
                while (temp != nullptr) {
                    myStack.push(temp);
                    temp = temp->youngbrother;
                }
            }
        }

    }

    int justify, sition;
    TreeNode* noddee;
    TreeNode* nodeed;

    //依次读入命令开始进行操作
    for (int i = 0;i < m;i++) {
        cin >> justify;
        if (justify == 0) {

            //找到指定位置，并删除这颗子树
            noddee = FindLocation(&node[1]);
            DeleteAtPosition(noddee, noddee->mother);

            //找到指定位置，并插入这颗子树
            nodeed = FindLocation(&node[1]);
            cin >> sition;
            InsertAtPosition(sition, noddee, nodeed);

        } else {
            //确定位置输出指定内容
            noddee = FindLocation(&node[1]);
            if (justify == 1) {
                cout << noddee->height << "\n";
            } else {
                cout << noddee->subtreenum << "\n";
            }
        }
    }

    return 0;
}
