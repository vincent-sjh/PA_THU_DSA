#include <iostream>
#include <cassert>
#include <utility>
#include <cmath>
#include <cstdlib>
using namespace std;


long long int n, t;

//初始化储存p,q,每日确诊人数,计算范围下界数组
long long int p[200000], q[200000], weak[1100005], tompute[1100005];
int maxweak[1100005];

// 定义队列结构体
struct Queue {
    pair<long long int, long long  int> elements[1100005];
    int front, rear; // 前端和后端索引
};

// 初始化队列
void initQueue(Queue& q) {
    q.front = 0;
    q.rear = 0;
}

// 判断队列是否为空
bool isEmpty(const Queue& q) {
    return ((q.rear == 0) && (q.front == 0));
}


// 获取队列长度
int size(const Queue& q) {
    return q.rear - q.front + 1;
}

// 获取队头元素
pair<long long int, long long int> front(const Queue& q) {
    return q.elements[q.front];
}
//获取队尾元素
pair<long long int, long long int> rear(const Queue& q) {
    return q.elements[q.rear];
}

// 入队操作
void enqueue(Queue& q, long long int value) {

    // 如果队列为空，则设置front为1
    if (isEmpty(q)) {
        q.front = 1;
        q.rear = 1;
        q.elements[1] = { value, 1 };
        return;
    }

    //如果不为空，计算比当前入队值小的个数，全部替换为当前值
    long long int numew = 0;

    while ((rear(q).first <= value) && (size(q) > 0)) {
        numew += rear(q).second;
        q.rear--;
    }
    q.rear++;
    q.elements[q.rear] = { value,numew + 1 };
    return;

}

// 出队操作
void dequeue(Queue& q, int value) {

    long long int remaining = value;
    while ((remaining >= front(q).second) && (size(q) > 0)) {
        remaining -= front(q).second;
        q.front = q.front + 1;
    }
    if (size(q) > 0) {
        q.elements[q.front].second = front(q).second - remaining;
    } else {
        initQueue(q);
    }
    return;
}

Queue weakman;

//二分搜索用于确定风险人数
long long int BinarySearch(long long int value) {

    long long int start = 1, end = n, middle;
    if (value > maxweak[n]) return n;
    if (value <= maxweak[1]) return 0;
    while ((end - start) != 1) {

        middle = (start + end) / 2;
        if (maxweak[middle] < value) {
            start = middle;
        } else {
            end = middle;
        }
    }
    return start;
}

// 比较函数，用于传递给 qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {

    //解绑输入输出缓冲区，节省时间
    ios::sync_with_stdio(false);
    cin.tie(0);

    //读入数据
    cin >> n;
    for (int i = 1;i <= n;i++) {
        cin >> weak[i];
    }
    for (long long int i = 1;i <= n;i++) {
        cin >> tompute[i];
        if (i - tompute[i] < 1) tompute[i] = 1;
        else tompute[i] = i - tompute[i];
    }


    maxweak[1] = 0;

    //初始化队堆
    initQueue(weakman);


    for (long long int i = 2;i <= n;i++) {
        //先将上一天确诊人数入队
        enqueue(weakman, weak[i - 1]);
        //计算需要出队的人数并出队
        dequeue(weakman, tompute[i] - tompute[i - 1]);
        //存储最大单日确诊数
        maxweak[i] = front(weakman).first;
    }

    //给存储最大确诊数的数组排序
    qsort(maxweak + 1, n, sizeof(int), compare);

    //读入t  
    cin >> t;

    //输出每个pq对应的中低风险人数
    for (int i = 0;i < t;i++) {
        cin >> p[i] >> q[i];
        cout << BinarySearch(p[i]) << " " << BinarySearch(q[i]) - BinarySearch(p[i]) << "\n";
    }
    return 0;
}

