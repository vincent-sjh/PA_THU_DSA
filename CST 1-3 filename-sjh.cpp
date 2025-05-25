#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;

string s1, s2;
int m, n, k, len, stage1[501500], stage2[501500];//定义m,n,k变量,len最长公共子序列长度，交替存储的两个长一维数组



//评估函数，评价s1的前s位和s2的前t位的最长公共子序列长度len(s,t)是否有计算的必要（若是必然超过限制k则不需计算）
int ass(int s, int t) {

    return 2 * (min(n - s, m - t) + min(s, t)) - m - n + k;

}

int main() {

    int temp, left, right;
    string str;


    cin >> n >> m >> k;//读入m,n,k
    cin >> s1 >> s2;//读入两个字符串


    if (n < m) {
        temp = n;
        n = m;
        m = temp;
        str = s1;
        s1 = s2;
        s2 = str;
    }//确保s1不短于s2,否则交换二者长度和字符数据

    left = (m - n - k + 1) / 2;//确定需要计算的数据范围下界（上取整）
    right = (m + k - n) / 2;//确定需要计算的数据范围上界（下取整）


    if (abs(m - n) > k)
        cout << -1;//原始判断，若是初始长度之差大于k，则不必进行计算直接输出-1
    else {

        for (int i = 0;i <= n;i++) {
            if (i == 0) {
                for (int j = max(0, i + left);j <= min(m, i + right);j++) {

                    stage1[j] = 0;
                }//在上下界范围内进行遍历，第一行直接赋值为零
                continue;
            }
            if (i % 2 == 0) {
                stage1[0] = 0;//第一个元素赋值为零
                for (int j = max(1, i + left);j <= min(m, i + right);j++) {
                    if (int(s1[i - 1]) == int(s2[j - 1])) {
                        stage1[j] = stage2[j - 1] + 1;//若当前计算的位置有意义，斜下方位置必然也有意义，不需要用ass函数特判
                    } else {
                        if (ass(i - 1, j) < 0)
                            stage1[j] = stage1[j - 1];//若当前计算位置有意义，则左面元素和下方位置至少有一个有意义，若一个无意义直接选择另一个
                        else if (ass(i, j - 1) < 0)
                            stage1[j] = stage2[j];//若当前计算位置有意义，则左面位置和下方位置至少有一个有意义，若一个无意义直接选择另一个
                        else
                            stage1[j] = max(stage1[j - 1], stage2[j]);//若左方下方的两个位置都有意义则取更大者
                    }
                }//在上下界范围内进行遍历，i为偶数时数组stage2为上一行原始数据，要根据stage2计算下一行stage1
            } else {
                stage2[0] = 0;//第一个元素赋值为零
                for (int j = max(1, i + left);j <= min(m, i + right);j++) {
                    if (int(s1[i - 1]) == int(s2[j - 1])) {
                        stage2[j] = stage1[j - 1] + 1;//若当前计算的位置有意义，斜下方位置必然也有意义，不需要用ass函数特判
                    } else {
                        if (ass(i - 1, j) < 0)
                            stage2[j] = stage2[j - 1];//若当前计算位置有意义，则左面元素和下方位置至少有一个有意义，若一个无意义直接选择另一个
                        else if (ass(i, j - 1) < 0)
                            stage2[j] = stage1[j];//若当前计算位置有意义，则左面元素和下方位置至少有一个有意义，若一个无意义直接选择另一个
                        else
                            stage2[j] = max(stage2[j - 1], stage1[j]);//若左方下方的两个位置都有意义则取更大者
                    }
                }//在上下界范围内进行遍历，i为偶数时数组stage2为上一行原始数据，要根据stage2计算下一行stage1

            }

        }

        if (n % 2 == 0)//根据奇偶性确认两个数组中存储了len(n,m)的那个
            len = stage1[m];
        else
            len = stage2[m];


        if (m + n - 2 * len <= k)//判定是否可以在k步内完成转化，不可则按照题意输出-1
            cout << m + n - 2 * len;
        else
            cout << -1;

    }
    return 0;
}
