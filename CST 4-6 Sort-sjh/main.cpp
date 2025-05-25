#include <iostream>
#include <random>

#include "sorting.cpp"

using namespace std;

int limit = 1e6;
int cmp_cnt = 0;

void compare(int a, int b, int c, int* max, int* min) {
    if (++cmp_cnt > limit) {
        printf("Wrong Answer --- exceeded limit\n");
        exit(0);
    }

    if (a > b) swap(a, b);
    *max = b > c ? b : c;
    *min = a < c ? a : c;
}

static int bbb[(size_t)1e6];

mt19937 engine(random_device{}());

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) bbb[i] = engine() % 1000000;
    sort(n, limit, bbb);
    cout << endl;
    // for (int i = 0; i < n; i++) cout << a[i] << ' ';
    return 0;
}