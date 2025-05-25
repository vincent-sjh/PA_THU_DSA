#include"sort.h"
#include <cassert>
int mer[5][500000];
int start;
int sum[5], cur[5];
int s, t;
int* elim = &s;
int* bin = &t;

void onemerge(int x1, int* a) {
    while (cur[x1] < sum[x1]) {
        *(a + start) = mer[x1][cur[x1]];
        cur[x1]++;
        start++;
    }
    return;
}

void twomerge(int x1, int x2, int* a) {

    while ((cur[x1] < sum[x1]) && (cur[x2] < sum[x2])) {

        compare(mer[x1][cur[x1]], mer[x2][cur[x2]], mer[x2][cur[x2]], bin, a + start);
        if (*(a + start) == mer[x1][cur[x1]])
            cur[x1]++;
        else
            cur[x2]++;
        start++;
    }

    if (cur[x1] < sum[x1]) {
        onemerge(x1, a);
        return;
    }
    if (cur[x2] < sum[x2]) {
        onemerge(x2, a);
        return;
    }
    return;
}

void threemerge(int x1, int x2, int x3, int* a) {
    while ((cur[x1] < sum[x1]) && (cur[x2] < sum[x2]) && (cur[x3] < sum[x3])) {

        compare(mer[x1][cur[x1]], mer[x2][cur[x2]], mer[x3][cur[x3]], bin, a + start);
        if (*(a + start) == mer[x1][cur[x1]])
            cur[x1]++;
        else if (*(a + start) == mer[x2][cur[x2]])
            cur[x2]++;
        else
            cur[x3]++;

        start++;
    }

    if ((cur[x1] < sum[x1]) && (cur[x2] < sum[x2])) {
        twomerge(x1, x2, a);
        return;
    }
    if ((cur[x1] < sum[x1]) && (cur[x3] < sum[x3])) {
        twomerge(x1, x3, a);
        return;
    }
    if ((cur[x2] < sum[x2]) && (cur[x3] < sum[x3])) {
        twomerge(x2, x3, a);
        return;
    }

    return;
}

void merge(int left, int right, int n1, int n2, int n3, int* a) {

    sum[1] = n1 - left;
    sum[2] = n2 - n1;
    sum[3] = n3 - n2;
    sum[4] = right - n3 + 1;

    cur[1] = 0;
    cur[2] = 0;
    cur[3] = 0;
    cur[4] = 0;
    start = 0;

    for (int i = 0; i < n1 - left; i++)
        mer[1][i] = *(a + i);
    for (int j = 0; j < n2 - n1; j++)
        mer[2][j] = *(a + n1 - left + j);
    for (int i = 0; i < n3 - n2; i++)
        mer[3][i] = *(a + n2 - left + i);
    for (int j = 0; j < right - n3 + 1; j++)
        mer[4][j] = *(a + n3 - left + j);

    compare(mer[1][0], mer[2][0], mer[3][0], elim, bin);
    compare(mer[4][0], mer[4][0], *bin, bin, a);

    if (*a == mer[1][0])
        cur[1]++;
    else if (*a == mer[2][0])
        cur[2]++;
    else if (*a == mer[3][0])
        cur[3]++;
    else
        cur[4]++;
    start++;

    while ((cur[1] < sum[1]) && (cur[2] < sum[2]) && (cur[3] < sum[3]) && (cur[4] < sum[4])) {

        if (*elim == mer[1][cur[1]])
            compare(mer[2][cur[2]], mer[3][cur[3]], mer[4][cur[4]], elim, a + start);
        else if (*elim == mer[2][cur[2]])
            compare(mer[1][cur[1]], mer[3][cur[3]], mer[4][cur[4]], elim, a + start);
        else if (*elim == mer[3][cur[3]])
            compare(mer[1][cur[1]], mer[2][cur[2]], mer[4][cur[4]], elim, a + start);
        else
            compare(mer[1][cur[1]], mer[2][cur[2]], mer[3][cur[3]], elim, a + start);

        if (*(a + start) == mer[1][cur[1]])
            cur[1]++;
        else if (*(a + start) == mer[2][cur[2]])
            cur[2]++;
        else if (*(a + start) == mer[3][cur[3]])
            cur[3]++;
        else
            cur[4]++;

        start++;
    }



    if ((cur[1] < sum[1]) && (cur[2] < sum[2]) && (cur[3] < sum[3])) {
        threemerge(1, 2, 3, a);
        return;
    }
    if ((cur[1] < sum[1]) && (cur[2] < sum[2]) && (cur[4] < sum[4])) {
        threemerge(1, 2, 4, a);
        return;
    }
    if ((cur[1] < sum[1]) && (cur[3] < sum[3]) && (cur[4] < sum[4])) {
        threemerge(1, 3, 4, a);
        return;
    }
    if ((cur[2] < sum[2]) && (cur[3] < sum[3]) && (cur[4] < sum[4])) {
        threemerge(2, 3, 4, a);
        return;
    }

    return;
}


void sortee(int left, int right, int* a) {

    if (right - left < 0) {
        return;
    }

    if (right - left == 0) {
        *a = left;
        return;
    }

    if (right - left == 1) {
        compare(left, right, right, a + 1, a);
        return;
    }
    if (right - left == 2) {

        compare(left, left + 1, left + 2, bin, a);
        if (*a == left) {
            compare(left + 1, left + 1, left + 2, a + 2, a + 1);
        } else if (*a == left + 1) {
            compare(left, left + 2, left + 2, a + 2, a + 1);
        } else {
            compare(left, left + 1, left + 1, a + 2, a + 1);
        }
        return;
    }

    int n1 = (3 * left + right + 1) / 4;
    int n2 = (left + right + 1) / 2;
    int n3 = (left + 3 * right + 3) / 4;

    sortee(left, n1 - 1, a);
    sortee(n1, n2 - 1, a + n1 - left);
    sortee(n2, n3 - 1, a + n2 - left);
    sortee(n3, right, a + n3 - left);

    merge(left, right, n1, n2, n3, a);
    return;

}

void sort(int n, int limit, int* a) {

    sortee(0, n - 1, a);
    return;
}