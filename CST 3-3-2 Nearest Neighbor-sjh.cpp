#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

long long int buildaxis;//全局变量，保存建树时所用的分量位置
long long int besting;// 全局变量保存目前最好结果


//定义节点结构体
struct Point {
    int coordinates[5];
};

Point points[110000];
int compare(const void* a, const void* b) {
    return ((struct Point*)a)->coordinates[buildaxis] - ((struct Point*)b)->coordinates[buildaxis];
}

//定义KD树节点结构体
struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;
    int depth;
    int axis;

    KDNode(const Point& p) : point(p), left(nullptr), right(nullptr), depth(0), axis(0) {}
};



KDNode* buildKDTree(int start, int end, int depth, int dimension) {

    if (start > end) {
        return nullptr;
    }


    int axis = depth % dimension;
    buildaxis = axis;//

    // 使用qsort对学生数组按照分数进行排序
    qsort(&points[start], end - start + 1, sizeof(points[0]), compare);

    int median = (start + end) / 2;

    KDNode* node = new KDNode(points[median]);

    node->depth = depth;
    node->axis = axis;

    node->left = buildKDTree(start, median - 1, depth + 1, dimension);
    node->right = buildKDTree(median + 1, end, depth + 1, dimension);

    return node;
}

long long int distanceSquared(const Point& a, const Point& b, int dimension) {
    long long int dist = 0, diff = 0;
    for (int i = 0; i < dimension; ++i) {
        diff = (long long int)a.coordinates[i] - (long long int)b.coordinates[i];
        dist += diff * diff;
    }
    return dist;
}

Point findNearestNeighbor(KDNode* root, const Point& target, int depth, int dimension) {

    //获得比较的分量
    int axis = root->axis;

    KDNode* nextBranch = (target.coordinates[axis] < root->point.coordinates[axis]) ? root->left : root->right;
    KDNode* otherBranch = (nextBranch == root->left) ? root->right : root->left;

    //当前最优设置为根节点
    Point best = root->point;
    long long int current = distanceSquared(root->point, target, dimension);

    if (current < besting) {
        besting = current;
    }

    long long int turrenc;

    //查询目标点所在一侧的最优节点
    if (nextBranch != nullptr) {
        Point next = findNearestNeighbor(nextBranch, target, depth + 1, dimension);

        turrenc = distanceSquared(next, target, dimension);

        if (turrenc < current) {
            best = next;
            current = turrenc;
        }
    }

    // 检查另一侧是否有更近的点
    long long int delta = (long long int)target.coordinates[axis] - (long long int)root->point.coordinates[axis];
    if ((delta * delta < besting) && (otherBranch != nullptr)) {
        Point otherBest = findNearestNeighbor(otherBranch, target, depth + 1, dimension);
        if (distanceSquared(otherBest, target, dimension) < current) {
            best = otherBest;
        }
    }
    return best;
}


int main() {


    //节约输入输出时间
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, d;
    cin >> d >> n;

    // 输入n个d维向量
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < d; ++j) {
            cin >> points[i].coordinates[j];
        }
    }

    // 构建KD树
    KDNode* root = buildKDTree(0, n - 1, 0, d);

    int q;
    cin >> q;

    // 处理每个查询
    for (int i = 0; i < q; ++i) {
        Point query;
        for (int j = 0; j < d; ++j) {
            cin >> query.coordinates[j];
        }

        besting = 3000000000000000;
        // 查询最近邻点

        findNearestNeighbor(root, query, 0, d);

        // 输出结果
        cout << besting << '\n';
    }

    return 0;
}
