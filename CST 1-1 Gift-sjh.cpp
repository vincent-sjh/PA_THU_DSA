#include<iostream>
#include<cstdlib>
#include<cmath>

using namespace std;


long long int P, n, middle, num, sum, cost_1[45], cost_2[45];//定义数据存储变量
long long int save[2500000], enumerate[2500000], total, total_enum;//两个大数组分别存储遍历得到的前半和后半数据，两个存储遍历个数的变量


void saving(long long int k) {
    if (k == middle) {
        sum = sum + cost_1[k];

        save[num] = sum;
        sum = sum - cost_1[k];
        num++;

        sum = sum + cost_2[k];
        save[num] = sum;
        sum = sum - cost_2[k];
        num++;

    } else {
        sum = sum + cost_1[k];
        saving(k + 1);
        sum = sum - cost_1[k];

        sum = sum + cost_2[k];
        saving(k + 1);
        sum = sum - cost_2[k];

    }
}//遍历前半数据集并存入save数组

void saving_enum(long long int k) {
    if (k == n - middle) {
        sum = sum + cost_1[k + middle];

        enumerate[num] = sum;
        sum = sum - cost_1[k + middle];
        num++;

        sum = sum + cost_2[k + middle];
        enumerate[num] = sum;
        sum = sum - cost_2[k + middle];
        num++;

    } else {
        sum = sum + cost_1[k + middle];
        saving_enum(k + 1);
        sum = sum - cost_1[k + middle];

        sum = sum + cost_2[k + middle];
        saving_enum(k + 1);
        sum = sum - cost_2[k + middle];

    }
}//遍历后半数据集并存入enumerate数组

void QuickSort(long long int low, long long int high) {

    long long int temp;

    if (low < high) {

        long long int pivot = save[high];
        long long int i = low - 1;

        for (long long int j = low; j < high; j++) {
            if (save[j] <= pivot) {
                i++;
                temp = save[i];
                save[i] = save[j];
                save[j] = temp;

            }
        }

        temp = save[i + 1];
        save[i + 1] = save[high];
        save[high] = temp;

        QuickSort(low, i);
        QuickSort(i + 2, high);
    }

}//前半数据集save的快速排序算法

long long int findranking(long long int index) {
    long long int temp, left = 0, right = pow(2, middle) - 1;

    if (index >= save[right]) return right + 1;
    if (index < save[left]) return left;

    while (right - left > 1) {
        temp = (left + right) / 2;

        if (save[temp] <= index) {
            left = temp;
        } else {
            right = temp;
        }
    }

    return right;
}//前半数据数组save的二分搜索算法

int main() {

    long long int method_num = 0, method_sum = 0;//定义两个变量，前者为针对确定后半数据，可以与之匹配的前半数据量，后者为匹配方法总量


    cin >> n >> P;
    middle = n / 2;
    total = pow(2, middle);
    total_enum = pow(2, n - middle);
    for (long long int i = 1;i <= n;i++) {
        cin >> cost_1[i] >> cost_2[i];
    }//读入并存储原始数据

    sum = 0;
    num = 0;
    saving(1);//遍历并存储前半数据
    sum = 0;
    num = 0;
    saving_enum(1);//遍历并存储后半数据

    QuickSort(0, total - 1);//对前半数据的save数组进行快速排序

    for (long long int i = 0;i < total_enum;i++) {
        method_num = findranking(P - enumerate[i]);
        method_sum += method_num;
    }//对每个后半数据的可能情况enumerate[i]，P-enumerate[i]在save数组中的排名就是这种可能情况的方法数，遍历相加

    cout << method_sum;//输出结果
    return 0;
}
