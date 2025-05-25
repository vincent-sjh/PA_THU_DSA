#include <iostream>
#include <random>
#include <cmath>
#include <cstdlib>

using namespace std;

//操作总数
int totalnum = 400000;

//操作计数器
int numa = 0, numb = 0, numc = 0;

//数据局部性因子
double overbalance = 0.85;

//key值上界
int upper = 200000;


//数据是否在树中
int inside[2100000];

// 通过调整这些概率值来控制生成的操作比例
double prob0 = 0.2;  // 添加操作比例
double prob1 = 0.2;  // 删除操作比例
double prob2 = 0.6;  // 查询操作比例


//普通随机数生成器
int NormalRandom(int k) {

    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(0, k);

    return distribution(generator);
}


//比例操作生成器
int operation(double probability0, double probability1, double probability2) {
    // 使用随机数引擎和分布器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // 生成一个介于0和1之间的随机概率值
    double randomProbability = dis(gen);

    // 根据概率生成相应的数字
    if (randomProbability < probability0) {
        return 0;
    } else if (randomProbability < probability0 + probability1) {
        return 1;
    } else {
        return 2;
    }
}

//局部性因子随机数生成器
int InequalityRandom(int n) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // 生成一个介于0和1之间的随机概率值

    double index = dis(gen);
    int left, right;

    if (index > overbalance) {
        return NormalRandom(upper);
    } else {
        left = int(n - n * (1 - overbalance));
        right = int(n + (upper - n) * (1 - overbalance));
        return left + NormalRandom(right - left);
    }

}


int main() {


    //解绑输入输出缓冲区，节省时间
    ios::sync_with_stdio(false);
    cin.tie(0);

    freopen("1in.txt", "w", stdout);


    cout << totalnum << endl;

    int key = upper / 2;

    // 生成一些随机数并输出
    for (int i = 0; i < totalnum; ++i) {
        int ope = operation(prob0, prob1, prob2);
        if (ope == 0) {
            do {
                key = InequalityRandom(key);
            } while (inside[key] == 1);
            inside[key] = 1;

            cout << "A " << key << '\n';
            numa++;
        } else if (ope == 1) {
            key = InequalityRandom(key);
            if (inside[key] == 1)
                inside[key] = 0;

            cout << "B " << key << '\n';
            numb++;
        } else {

            key = InequalityRandom(key);
            cout << "C " << key << '\n';
            numc++;
        }
    }
    cout << "number of ABC: " << totalnum << endl;
    cout << "number of A: " << numa << endl;
    cout << "number of B: " << numb << endl;
    cout << "number of C: " << numc << endl;
    cout << "index of overbalance: " << overbalance << endl;

    return 0;
}

