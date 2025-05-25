#include <iostream>
using namespace std;

// 定义结构体表示点
struct Point {
    long long int x;
    long long int y;
    long long int index;//栈中节点在dot数组中的编号
};

//初始化二维点集
Point dot[1100000];

//定义储存点类别（左右边界，上面，下面）的数组
long long int poly[1100000];

//定义储存扫描点顺序的数组
long long int addr[1100000];

//顶点数
long long int n;



//普通外积函数，参数为三个点的索引编号
long long int outerproduct(long long int a, long long int b, long long int c) {
    return (dot[b].y - dot[a].y) * (dot[c].x - dot[a].x) - (dot[b].x - dot[a].x) * (dot[c].y - dot[a].y);
}

//处理节点索引循环
long long int looper(long long int a) {

    if (a > n) return a - n;
    if (a < 1) return a + n;
    return a;
}

//保证内角被计算的外积函数，依据是上曲线还是下曲线上的点
long long int innerproduct(long long int a, long long int b, long long int c) {


    if (poly[a] == 1) {
        return outerproduct(a, b, c);
    } else {
        return -outerproduct(a, b, c);
    }

}



// 定义结构体表示栈
struct Stack {
    Point elements[1100000]; // 固定大小的数组
    long long int topIndex;// 栈顶索引

    // 构造函数，初始化栈顶索引
    Stack() : topIndex(0) {}

    // 入栈操作
    void push(const Point& p) {
        topIndex++;
        elements[topIndex] = p;
    }

    // 出栈操作
    Point pop() {
        topIndex--;
        return elements[topIndex + 1];
    }

    // 获取栈顶元素
    Point top() const {
        return elements[topIndex];
    }
    Point tip() const {
        return elements[topIndex - 1];
    }

    //清空栈
    void clear() {
        topIndex = 0;
    }

    // 判断栈是否为空
    bool isEmpty() const {
        return topIndex == 0;
    }

    // 获取栈的大小
    long long int size() const {
        return topIndex;
    }
};

//初始化栈
Stack myStack;


int main() {

    //加快输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(0);

    //初始化最小点索引，最大点索引，次小点索引
    long long int min = 1, max = 1, temp;

    //初始化暂时节点
    Point tempoint;

    //读入节点数量及数据，同时确定最大最小点索引
    cin >> n;
    for (long long int i = 1;i <= n;i++) {
        cin >> dot[i].x >> dot[i].y;
        dot[i].index = i;
        if (dot[i].x < dot[min].x) {
            min = i;
        }
        if (dot[i].x > dot[max].x) {
            max = i;
        }
    }

    //确定上链首个端点索引
    temp = looper(min + 1);
    if (outerproduct(min, looper(min + 1), looper(min - 1)) < 0) {
        temp = looper(min - 1);
    }


    bool setting;
    if (temp == looper(min + 1)) setting = true;
    else setting = false;


    //从最小点索引min开始向两侧归并，直至上下同时到达最大点索引max，获得一个扫描顺序数组addr(依照x坐标)
    long long int s = 1, t = 1, id = 2;
    addr[1] = min;
    while ((looper(min + s) != max) || (looper(min - t) != max)) {
        if (dot[looper(min + s)].x < dot[looper(min - t)].x) {
            addr[id] = looper(min + s);
            if (setting) poly[looper(min + s)] = 1;
            s++;
        } else {
            addr[id] = looper(min - t);
            if (!setting) poly[looper(min - t)] = 1;
            t++;
        }
        id++;
    }
    addr[n] = max;

    //类别数组值为2代表最小点或最大点
    poly[min] = 2;
    poly[max] = 2;


    //将最小点与次小点入栈
    myStack.push(dot[addr[1]]);
    myStack.push(dot[addr[2]]);

    //从左至右按照addr顺序开始扫描
    for (long long int i = 3;i <= n;i++) {

        //判定是否扫描到最大点
        if (poly[addr[i]] == 2) {

            //直接全部出栈并输出划分方案
            for (long long int j = 1;j < myStack.size();j++) {
                cout << dot[addr[i]].x << " " << dot[addr[i]].y;
                cout << " " << myStack.elements[j].x << " " << myStack.elements[j].y;
                cout << " " << myStack.elements[j + 1].x << " " << myStack.elements[j + 1].y << "\n";
            }

            //终止循环
            break;
        } else if (poly[addr[i]] == 1) {
            //扫描到一个上节点

            //如果栈顶是下节点，输出划分方案，清空全栈，将栈顶节点和当前节点入栈
            if (poly[myStack.top().index] == 0) {
                for (long long int j = 1;j < myStack.size();j++) {
                    cout << dot[addr[i]].x << " " << dot[addr[i]].y;
                    cout << " " << myStack.elements[j].x << " " << myStack.elements[j].y;
                    cout << " " << myStack.elements[j + 1].x << " " << myStack.elements[j + 1].y << "\n";
                }
                tempoint = myStack.top();
                myStack.clear();
                myStack.push(tempoint);
                myStack.push(dot[addr[i]]);

            } else {

                //如果栈顶也是上节点，一直出栈直至判定成功，然后将当前节点入栈
                while ((innerproduct(myStack.top().index, myStack.tip().index, addr[i]) < 0) && (myStack.size() >= 2)) {
                    cout << dot[addr[i]].x << " " << dot[addr[i]].y;
                    cout << " " << myStack.top().x << " " << myStack.top().y;
                    cout << " " << myStack.tip().x << " " << myStack.tip().y << "\n";
                    myStack.pop();
                }
                myStack.push(dot[addr[i]]);
            }
        } else {
            //扫描到一个下节点

            //如果栈顶是上节点，输出划分方案，清空全栈，将栈顶节点和当前节点入栈（注意栈中仅剩下一个点停止判定）
            if (poly[myStack.top().index] == 1) {
                for (long long int j = 1;j < myStack.size();j++) {
                    cout << dot[addr[i]].x << " " << dot[addr[i]].y;
                    cout << " " << myStack.elements[j].x << " " << myStack.elements[j].y;
                    cout << " " << myStack.elements[j + 1].x << " " << myStack.elements[j + 1].y << "\n";
                }
                tempoint = myStack.top();
                myStack.clear();
                myStack.push(tempoint);
                myStack.push(dot[addr[i]]);

            } else {

                //如果栈顶也是下节点，一直出栈直至判定成功，然后将当前节点入栈（注意栈中仅剩下一个点停止判定）
                while ((innerproduct(myStack.top().index, myStack.tip().index, addr[i]) < 0) && (myStack.size() >= 2)) {
                    cout << dot[addr[i]].x << " " << dot[addr[i]].y;
                    cout << " " << myStack.top().x << " " << myStack.top().y;
                    cout << " " << myStack.tip().x << " " << myStack.tip().y << "\n";
                    myStack.pop();
                }
                myStack.push(dot[addr[i]]);
            }
        }
    }

    return 0;
}
