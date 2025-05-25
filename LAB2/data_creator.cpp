#include<cstdio>
#include<iostream>
#include<ctime>
#include<cstdlib>

using namespace std;

const int INSERT_SIZE = 15000; //另两组为60000，300000
const int QUERY_SIZE = 5000;//另两组为20000，100000

int generateRandomBinary() {
    // 设置种子值
    return rand() % 2;
}

int main() {
    freopen("data1.txt", "w", stdout);
    freopen("poj.txt", "r", stdin);

    string name;
    int index, order, score;

    //接下来，cin/cout/printf/scanf都会从b.txt尝试读取数据，并将输出打印到a.txt
    for (int i = 0;i < INSERT_SIZE;i++) {

        cin >> index >> name >> order >> score;
        cout << 0 << " " << name << " " << score << endl;
    }

    freopen("poj.txt", "r", stdin);

    for (int i = 0;i < QUERY_SIZE;i++) {

        cin >> index >> name >> order >> score;
        cout << 1 << " " << name << endl;
    }
    cout << 2;

    return 0;
}