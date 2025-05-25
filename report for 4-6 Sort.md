# PA Report for 4-6 Sort

### 解题构思

- 最初只想到进行使用三元比较器进行快排或者三路归并即可，但是发现有测试点比较次数超过限制，于是开始考虑如何优化算法。
- 想到可以把原来的三路归并变为之后的四路归并，由于可以获得三个数中的最大值和最小值，每次归并之后可以把最大值那一路归并上的值排除，因为它一定不是最小值，这样我们和三路归并一样，还是平均调用一次比较器就可以确定一个值，从三路归并到四路归并的改变可以优化性能。

### 实现方法

我们逐个分析每个函数的功能和实现方法。

1. **`onemerge` 函数:**
   - 功能: 将单个子数组合并到最终排序数组中，用于二路归并到后期只剩下一路归并的情况，直接输出即可。
   - 实现: 遍历一个子数组（`mer[x1]`），将其元素复制到最终数组（`a`）中。

```cpp
void onemerge(int x1, int* a) {
    while (cur[x1] < sum[x1]) {
        *(a + start) = mer[x1][cur[x1]];
        cur[x1]++;
        start++;
    }
    return;
}
```

2. **`twomerge` 函数:**
   - 功能: 将两个子数组合并到最终排序数组中，用于三路归并到后期只剩下两路的情况。
   - 实现: 遍历两个子数组（`mer[x1]` 和 `mer[x2]`），使用 `compare` 函数比较元素。较小的元素被复制到最终数组（`a`）中，直到其中一个为空，判断剩下哪路归并之后转为一路归并。

```cpp
void twomerge(int x1, int x2, int* a) {

	//正常二路归并
    while ((cur[x1] < sum[x1]) && (cur[x2] < sum[x2])) {
        compare(mer[x1][cur[x1]], mer[x2][cur[x2]], mer[x2][cur[x2]], bin, a + start);

        if (*(a + start) == mer[x1][cur[x1]])
            cur[x1]++;
        else
            cur[x2]++;
        start++;
    }

	//转为一路归并
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
```

3. **`threemerge` 函数:**
   - 功能: 将三个子数组合并到最终排序数组中，用于四路归并后期只剩下三路的情况。
   - 实现: 类似于 `twomerge`，但处理三个子数组（`mer[x1]`，`mer[x2]`，和 `mer[x3]`），运行直到其中一个为空，判断剩下哪两路之后转为二路归并。

```cpp
void threemerge(int x1, int x2, int x3, int* a) {

	//正常三路归并
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

	//转为二路归并
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
```

4. **`merge` 函数:**
   - 功能: 将四个已排序的子数组合并为一个已排序数组。
   - 实现: 根据输入数组 `a` 初始化四个子数组（`mer[1]` 到 `mer[4]`）。然后使用 `compare` 函数确定三个子数组中最小的元素，并将其添加到最终数组（`a`）。由于上次排序过程中会得到最大值，从而排除一路的归并，故而每次使用一次比较器即可写入一个值，这个过程一直持续，直到一路为空，判断还剩下哪三路后转为三路归并。

```cpp
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

	//正常四路归并
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


	//转为三路归并
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
```

5. **`sortee` 函数:**
   - 功能: 通过调用 `merge` 函数递归地划分和合并输入数组。
   - 实现: 通过递归将数组划分为较小的子数组，然后使用 `merge` 函数合并它们。

```cpp
void sortee(int left, int right, int* a) {
  
	//特判区间较短的情况作为递归的终止，区间长度为1、2、3的情况
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

	//对于区间长度较长（>=3）的情况将区间用三个分界点分为四段，分别排序
    int n1 = (3 * left + right + 1) / 4;
    int n2 = (left + right + 1) / 2;
    int n3 = (left + 3 * right + 3) / 4;

    sortee(left, n1 - 1, a);
    sortee(n1, n2 - 1, a + n1 - left);
    sortee(n2, n3 - 1, a + n2 - left);
    sortee(n3, right, a + n3 - left);

	//对这四路排序好的数组进行四路归并
    merge(left, right, n1, n2, n3, a);
    return;

}
```

6. **`sort` 函数:**
   - 功能: 通过调用 `sortee` 函数启动排序过程。
   - 实现: 使用 `sortee` 函数调用初始参数。

```cpp
void sort(int n, int limit, int* a) {
    sortee(0, n - 1, a);
    return;
}
```

### 调试与微调&问题解决

- 初始考虑了很多种不必要的情况，比如四路归并转为二路归并，三路归并转为一路归并，但实际上四路归并只会退化为三路归并，三路归并只会退化为二路归并，二路归并只会退化为一路归并，因为每次只写入一个元素，不可能有两路同时由非空转为空。

### 时间和空间复杂度估算

> 空间复杂度
>
>> 四个规模为n/4的缓冲区数组
>> 其他的原始数据等（相比可忽略不计）
>>

***综上，空间复杂度大约为O(n)***

> 时间复杂度
>
>> 复杂度为O(nlogn)的四路归并
>>

***综上，空间复杂度大约为O(nlogn)***
