# report for LAB2 HashFun(含测试结果)

## 哈希函数及冲突处理策略实现方法

### 坏的哈希函数

为了将字符串不均匀地映射到哈希表中，且要求用到字符串中所有信息，想到可以将字符串中每个字符的值求和再对表长取模即可，这样大量字符串会被映射到中等大小的索引，较少的字符串会被映射到较大或者较小的索引，即“中间多两边少”。

```cpp

int normal_hashing::operator()(char* str, int N) {
    if (str == NULL) return 0;
    else {
        int hash = 0;
        for (int i = 0; str[i] != '\0'; i++) {
            hash = hash + str[i];
            hash = hash % N;
        }
        return hash;
    }
}
```

### 好的哈希函数

为了将字符串均匀地映射到哈希表中，想到采用同余多项式，把字符串的每一位字符的整数值视为多项式某一项的参数即可，这样每一位的权重都因为项的次数的不同而不同，从而可以尽可能地将字符串分散开达到均匀的效果。

```cpp
int perfect_hashing::operator()(char* str, int N) {
    if (str == NULL) return 0;
    else {
        int hash = 0;
        for (int i = 0; str[i] != '\0'; i++) {
            hash = hash * 2 + str[i];
            hash = hash % N;
        }
        return hash;
    }
}

```

### 冲突处理策略：双平方试探

双平方试探需要在冲突策略子类中设置两个成员变量，用于指示试探过程中相对初始位置的相对位置，一个指示方向，另一个指示距离，每次根据这两个量决定下一次试探的位置，并更新这两个成员变量。初始化函数负责将这两个成员变量回归初始状态。

```cpp
struct square_probe : public collision_strategy {
    bool right;
    int length;

    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
//...

int square_probe::operator()(hash_entry* Table, int table_size, int last_choice) {
    if (length == 0) {
        length = 1;
        right = true;
        return (last_choice + 1) % table_size;
    } else {
        if (right) {
            right = false;
            int delta = (2 * length * length) % table_size;
            return (last_choice - delta + table_size) % table_size;
        } else {
            right = true;
	    length++;
            int delta = (2 * length * length + 2 * length + 1) % table_size;
            return (last_choice + delta) % table_size;
        }
    }
}
void square_probe::init() {

    right = false;
    length = 0;
    return;
}

```

### 冲突处理策略：公共溢出区

哈希表大小为质数499883，挑选一个适宜大小的质数300017，从这个索引开始后面的哈希表区域作为公共溢出区。设置成员变量作为溢出区的首端。公共溢出区策略所对应的哈希函数的表长参数要相应地改成300017，使得哈希函数算出的索引不会落在溢出区内部。初始化时将成员变量改回溢出区首段，每次碰撞搜索时都从首端开始搜索即可。

```cpp
struct overflowzone_probe : public collision_strategy {

    int overflowzone_start = 300016;

    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};
//...
bool insert(hash_entry entry) {
        int last_choice = (*my_hashing)(entry.my_string, 300017);
	//...
    }

int query(char* query_string) {
        int last_choice = (*my_hashing)(query_string, 300017);
	//...
    }
//... 
int overflowzone_probe::operator()(hash_entry* Table, int table_size, int last_choice) {
    overflowzone_start++;
    return overflowzone_start;
}

void overflowzone_probe::init() {
    overflowzone_start = 300016;
    return;
}

```

## 测试结果

| 策略搭配（哈希函数+冲突策略）\数据集 | 数据1（20000操作） | 数据2（80000操作） | 数据3(400000操作) |
| ------------------------------------ | ------------------ | ------------------ | ----------------- |
| normal+linear                        | 0.671s             | 20.028s            | 1459.58s          |
| normal+square                        | 0.031s             | 0.198s             | 2.066s            |
| normal+overflowzone                  | 0.434s             | 7.345s             | 220.936s          |
| perfect+linear                       | 0.025s             | 1.005s             | 152.239s          |
| perfect+square                       | 0.037s             | 0.116s             | 1.273s            |
| perfect+overflowzone                 | 0.043s             | 1.468s             | 85.086S           |

## 数据集的构造方法和数据特征

我的数据集之间的差别主要体现在数据量方面，构造三个数据集操作总数分别为20000，80000，400000，同时控制变量法保证插入和查询操作比例恒为3：1，且都先插入再查询。构造方法为先重定向输入输出，再从poj.txt读入数据即可，程序支持改动输入操作及查询操作数量。

## 结果分析

* “好”和“坏”的哈希函数的性能比较情况如何？为什么是这样？
* 无论使用哪种冲突策略以及数据量的情况下，“好”的哈希函数性能都显著优于“坏”的哈希函数，这是因为“好的”哈希函数使得字符串每一位的权重都不同，可以尽可能均匀地将字符串映射到哈希表上，而“坏”的哈希函数（普通求和）每位权重相同，导致大量字符串被映射到中等大小的索引，导致大量哈希碰撞，从而时间更长。
* 线性试探和双向平方试探的性能比较情况如何？为什么是这样？
* 无论在使用哪种哈希函数以及数据量的情况下，双向平方试探的性能都显著优于线性试探，而且数据量越大，差距越大。且在使用“坏”的哈希函数的情况下，二者性能差距（三个数据集测试下分别：20倍，200倍，750倍）显著大于使用“好”的哈希函数的情况下的性能差距（三个数据集测试下分别：1倍，10倍，150倍）。我们可以看出，在哈希函数映射的密集程度（或叫做哈希冲突频率）越高时，双向平方试探优势越明显，这是因为双向平方试探可以更快地“逃离”密集冲突的区域，可以更快找到空的位置。
* 在测试数据中，开放散列(公共溢出区)和封闭散列(使用试探链)的比较情况如何，谁占优势？在你的测试结果中不占优势的处理策略，处理什么样的实际数据时会更适合使用？
* 在使用两种哈希函数以及三种不同数据量的情况下，性能排序大体呈现出双向平方试探>公共溢出区>线性探测的规律，这种规律数据量越大越明显。根据实验结果，相对不占优势的公共溢出区（开放散列）策略我想在处理数据量较小时相对更适用。
* 设计哈希函数时，我们往往假定字符串每个位置上出现字符集内每个字符的概率都是相等的，但实际的数据集往往并不满足这一点。这可能造成什么影响？
* 可能造成性能评估时和预期不同。字符串每个位置上出现字符集内每个字符的概率是不相等的，比如用户名开头字符大概率是字母而非数字。如果我们采用“好”的哈希策略，即同余多项式，这时位于首位的字符在多项式中次数最高，权重最大，如果首字符大量集中在某些字符（比如字母）之中，导致字符串大量映射到这些字符对应的部分中，哈希碰撞频率增加，导致性能相比每个位置等概率地出现字符更差。
* 实验框架的哈希表容量一定，如果希望哈希表能够做到动态扩容和缩容，你准备如何实现？(介绍思路即可)
* 可以监控负载因子： 哈希表中已存储元素的数量与哈希表容量的比率超过某个阈值时，我们就进行扩容操作，比如增加固定长度或者翻倍；当负载因子低于另一个阈值时，我们就进行缩容操作，比如缩短固定长度或者减半，这样是一种思路。
