#include <cstdio>
#include <ctime>
#include <iostream>
#include "hashtable.h"
using namespace std;

int main() {

    freopen("out3.txt", "w", stdout);
    freopen("data3.txt", "r", stdin);

    int type;
    char buffer[1000];int data;

    hashtable table(600000, new normal_hashing(), new overflowzone_probe());

    double t1 = clock();
    while (true) {
        scanf("%d", &type);
        if (type == 0) {
            scanf("%s", buffer);
            scanf("%d", &data);
            table.insert(hash_entry(buffer, data));
        } else if (type == 1) {
            scanf("%s", buffer);
            cout << table.query(buffer) << endl;

        } else break;
    }

    double t2 = clock();
    double result = (t2 - t1) / CLOCKS_PER_SEC;//result里存的就是要测时间的代码段的运行时间，单位秒

    cout << "total_time: " << result << "s" << endl;

    return 0;
}