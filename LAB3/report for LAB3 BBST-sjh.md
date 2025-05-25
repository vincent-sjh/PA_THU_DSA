# report for LAB3 BBST-sjh

### AVL Tree实现过程及复杂度分析

### 1. 结构体 `node`

```cpp
struct node {
    long long int height;
    long long int key;
    node* left;
    node* right;
    node* dad;

    node(const long long int& p) : height(0), key(p), left(nullptr), right(nullptr), dad(nullptr) {}
};
```

这个结构体表示 AVL 树的节点，包括节点高度 `height`、节点键值 `key`、左子树指针 `left`、右子树指针 `right`、父节点指针 `dad`。

### 2. 函数 `height(node* root)`

```cpp
int height(node* root) {
    if (root == nullptr)
        return -1;
    else
        return root->height;
}
```

该函数返回节点的高度。若节点为空，则返回 -1。我们使用这个函数可以避免繁琐的空指针特判。

### 3. 函数 `Search(node* root, long long int keyy)`

```cpp
node* Search(node* root, long long int keyy) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->key == keyy) {
        return root;
    }
    if (root->key < keyy) {
        if (root->right == nullptr)
            return root;
        else
            return Search(root->right, keyy);
    } else {
        if (root->left == nullptr)
            return root;
        else
            return Search(root->left, keyy);
    }
}
```

该函数在 AVL 树中搜索键值为 `keyy` 的节点，如果找到，则返回该节点；如果未找到，则返回应插入的位置的父节点。

### 4. 函数 `Searchunb(node* root)`

```cpp
node* Searchunb(node* root) {
    if (root == nullptr)
        return nullptr;
    node* roote = root;
    while ((height(roote->left) - height(roote->right) <= 1) && (height(roote->left) - height(roote->right) >= -1)) {
        roote = roote->dad;
        if (roote == nullptr)
            return nullptr;
    }
    return roote;
}
```

该函数在 AVL 树中从执行操作的点向上搜索不平衡的节点，即找到第一个不满足 AVL 树性质的节点。返回找到的不平衡节点。

### 5. 函数 `rift(node* root)`

```cpp
bool rift(node* root) {
    assert(root != nullptr);
    assert(root->dad != nullptr);

    if (root->dad->left == root)
        return true;
    else
        return false;
}
```

### 6. 函数 `regroup(node* root)`

```cpp
node* regroup(node* root) {
    node* t1;
    node* t2;
    node* v = root;
    node* s;
    node* g;

    //确定三代节点
    s = (height(v->left) > height(v->right)) ? v->left : v->right;
    if (height(s->left) > height(s->right))
        g = s->left;
    else if (height(s->left) < height(s->right))
        g = s->right;
    else
        g = (rift(s)) ? s->left : s->right;

    //根据三代节点的相互位置关系分类处理
    if (rift(s)) {
        if (rift(g)) {
            v->left = s->right;
            if (v->left != nullptr)
                s->right->dad = v;
            s->right = v;
            v->dad = s;
            v->height = (height(v->left) > height(v->right)) ? height(v->left) : height(v->right);
            v->height++;
            s->height = (height(s->left) > height(s->right)) ? height(s->left) : height(s->right);
            s->height++;
            return s;
        } else {
	    //“3+4”重组，避免繁琐的zig+zag
            t1 = g->left;
            t2 = g->right;
            s->right = t1;
            if (t1 != nullptr)
                t1->dad = s;
            v->left = t2;
            if (t2 != nullptr)
                t2->dad = v;
            g->left = s;
            g->right = v;
            v->dad = g;
            s->dad = g;
            v->height = (height(v->left) > height(v->right)) ? height(v->left) : height(v->right);
            v->height++;
            s->height = (height(s->left) > height(s->right)) ? height(s->left) : height(s->right);
            s->height++;
            g->height = (height(g->left) > height(g->right)) ? height(g->left) : height(g->right);
            g->height++;
            return g;
        }
    } else {
        if (rift(g)) {
           //...对称情况
        } else {
            //...对称情况
        }
    }
}
```

- **功能分析**:

  - `regroup` 函数用于对不平衡节点进行旋转操作，以保持 AVL 树平衡。
  - 根据不同情况的节点子树位置关系，进行“3+4”重组，避免zig+zag。

### 7. 函数 `Searchmin(node* root)`

```cpp
node* Search

min(node* root) {
    node* pro = root->right;
    while (pro->left != nullptr) {
        pro = pro->left;
    }
    return pro;
}
```

- **功能分析**:

  - `Searchmin` 函数用于在 AVL 树中找到大于当前结点的最小的节点。
  - 在 AVL 树中，先找到右孩子，然后一直沿着左子树走到底即可。

### 8. 函数 `Updateheight(node* here)`

```cpp
void Updateheight(node* here) {
    long long int heighting = 0;
    if (here->left != nullptr) {
        if (here->left->height + 1 > heighting)
            heighting = here->left->height + 1;
    }
    if (here->right != nullptr) {
        if (here->right->height + 1 > heighting)
            heighting = here->right->height + 1;
    }
    if (heighting == here->height)
        return;
    else {
        here->height = heighting;
        if (here->dad != nullptr)
            Updateheight(here->dad);
    }
    return;
}
```

- **功能分析**:

  - `Updateheight` 函数用于更新节点及其祖先的高度。
  - 计算当前节点的新高度，如果与旧高度相等则直接返回（这时所有祖先也必定不需要更新），否则更新节点的高度并递归更新其父节点的高度。

### 9. 函数 `add(node* root, long long int keyy)`

```cpp
void add(node* root, long long int keyy) {

    //插入新节点
    if (root == nullptr) {
        rootavl = new node(keyy);
        rootavl->dad = nullptr;
        return;
    } else {
        node* pos = Search(root, keyy);

        if (keyy < pos->key) {
            pos->left = new node(keyy);
            pos->left->dad = pos;
            Updateheight(pos);

        } else if (keyy > pos->key) {
            pos->right = new node(keyy);
            pos->right->dad = pos;
            Updateheight(pos);
        }

	//平衡维护阶段
        node* convert = Searchunb(pos);
        if (convert != nullptr) {
            node* father = convert->dad;
            if (father != nullptr) {
                bool legh = rift(convert);
                node* newtree = regroup(convert);
                newtree->dad = father;
                if (legh)
                    father->left = newtree;
                else
                    father->right = newtree;
            } else {
                node* newtree = regroup(convert);
                newtree->dad = nullptr;
                rootavl = newtree;
            }
        }
    }
    return;
}
```

- **功能分析**:

  - `add` 函数用于向 AVL 树中插入一个节点，其键值为 `keyy`。
  - 通过 `Search` 函数找到要插入的位置，并根据键值的大小插入到左子树或右子树。
  - 插入后调用 `Updateheight` 更新节点及其祖先的高度。
  - 最后，通过 `Searchunb` 找到不平衡的节点，如果存在则进行重组保持 AVL 树平衡。
  - 插入时最多只需要进行一次重组即可故而不需要循环上溯至树根。

### 10. 函数 `delet(node* root, long long int keyy)`

```cpp
void delet(node* root, long long int keyy) {
    node* todel = Search(root, keyy);
    if (todel == nullptr)
        return;
    if (todel->key != keyy)
        return;

    node* convert;

    if (todel->dad == nullptr) {
        if (todel->left == nullptr) {
            rootavl = todel->right;
            if (rootavl != nullptr)
                rootavl->dad = nullptr;
            return;
        } else if (todel->right == nullptr) {
            rootavl = todel->left;
            if (rootavl != nullptr)
                rootavl->dad = nullptr;
            return;
        } else {
            node* min = Searchmin(todel);
            todel->key = min->key;
            delet(min, min->key);
            convert = min->dad;
        }
    } else {
        bool index;
        if (todel->dad->left == todel)
            index = false;
        if (todel->dad->right == todel)
            index = true;

        if (todel->left == nullptr) {
            if (todel->right == nullptr) {
                if (index)
                    todel->dad->right = nullptr;
                else
                    todel->dad->left = nullptr;
                Updateheight(todel->dad);
                convert = todel->dad;
            } else {
                if (index) {
                    todel->dad->right = todel->right;
                    todel->right->dad = todel->dad;
                } else {
                    todel->dad->left = todel->right;
                    todel->right->dad = todel->dad;
                }
                Updateheight(todel->dad);
                convert = todel->dad;
            }
        } else {
            if (todel->right == nullptr) {
                if (index) {
                    todel->dad->right = todel->left;
                    todel->left->dad = todel->dad;
                } else {
                    todel->dad->left = todel->left;
                    todel->left->dad = todel->dad;
                }
                Updateheight(todel->dad);
                convert = todel->dad;
            } else {
                node* min = Searchmin(todel);
                todel->key = min->key;
                delet(min, min->key);
                convert = min->dad;
            }
        }
    }

    convert = Searchunb(convert);
    while (convert != nullptr) {
        node* father = convert->dad;
        if (father != nullptr) {
            bool legh = rift(convert);
            node* newtree = regroup(convert);
            newtree->dad = father;
            if (legh)
                father->left = newtree;
            else
                father->right = newtree;
        } else {
            node* newtree = regroup(convert);
            newtree->dad = nullptr;
            rootavl = newtree;
        }
        convert = Searchunb(convert);
    }
    return;
}
```

- **功能分析**:

  - `delet` 函数用于在 AVL 树中删除键值为 `keyy` 的节点。
  - 首先通过 `Search` 函数找到要删除的节点，如果不存在则直接返回。
  - 如果要删除的节点有两个子节点，则找到其右子树中的最小节点，将其键值赋给当前节点，然后递归删除最小节点。
  - 如果要删除的节点是根节点，分为三种情况处理：没有左子树、没有右子树和两个子节点。
  - 如果要删除的节点不是根节点，则根据其在父节点的位置，更新父节点的子节点指针。
  - 更新删除节点的父节点及祖先节点的高度。
  - 最后，通过 `Searchunb` 找到不平衡的节点，如果存在则进行旋转操作保持 AVL 树平衡。
  - 重组过后继续向上递归，因为删除操作可能导致失衡传播，直至上溯至根节点，过程中遇到不平衡节点就再次进行重组。

### 11. 函数 `ques(node* root, long long int keyy)`

```cpp
long long int ques(node* root, long long int keyy) {
    if (root == nullptr)
        return -1;


    if (root->key <= keyy) {
        if (root->right == nullptr)
            return root->key;
        else {
            int answer = ques(root->right, keyy);
            if (answer > root->key)
                return answer;
            else
                return root->key;
        }
    } else {
        if (root->left == nullptr)
            return -1;
        else {
            return ques(root->left, keyy);
        }
    }
}
```

- **功能分析**:

  - `ques` 函数用于在 AVL 树中寻找小于等于 `keyy` 的最大键值。
  - 递归搜索 AVL 树，根据当前节点的键值与 `keyy` 的比较决定继续在左子树或右子树中搜索。
  - 如果当前节点的键值小于等于 `keyy`，则在右子树中继续搜索，否则在左子树中继续搜索。
  - 如果右子树为空，则返回当前节点的键值；否则返回右子树中找到的最大值或当前节点的键值。

### 12. 时间复杂度

#### 1. `ques` 函数

- **时间复杂度分析**:
  - `ques` 函数在最坏情况下的时间复杂度为 O(log n)。
  - 主要消耗时间的是在 AVL 树中进行递归搜索，每次递归向左子树或右子树移动一步。
  - 由于 AVL 树是平衡二叉树，其高度为 O(log n)，因此搜索的时间复杂度为 O(log n)。

#### 2. `add` 函数

- **时间复杂度分析**:
  - `add` 函数在最坏情况下的时间复杂度为 O(log n)。
  - 主要消耗时间的是通过 `Search` 找到要插入的位置，这个操作的时间复杂度为 O(log n)。
  - 插入节点后可能导致不平衡，需要调用 `Searchunb` 找到不平衡的节点，然后调用 `regroup` 进行平衡调整，而 `regroup` 中的重组操作的时间复杂度为 O(1)。
  - 整体来说，`add` 函数的总体时间复杂度为 O(log n)。

#### 3. `delet` 函数

- **时间复杂度分析**:
  - `delet` 函数在最坏情况下的时间复杂度为 O(log n)。
  - 主要消耗时间的是通过 `Search` 找到要删除的节点，这个操作的时间复杂度为 O(log n)。
  - 删除节点后可能导致不平衡，需要调用 `Searchunb` 找到不平衡的节点，然后调用 `regroup` 进行平衡调整，而 `regroup` 中的旋转操作的时间复杂度为 O(1)，由于一直向上查找，重组次数最多为O(log n)次，故而总复杂度为O(log n)。
  - 整体来说，`delet` 函数的总体时间复杂度为 O(log n)。

### Splay Tree实现过程及复杂度分析

### 1. `Node` 结构体定义

```cpp
struct Node {
    int weight;
    Node* father;
    Node* left;
    Node* right;

    Node(const int& p) : weight(p), father(nullptr), left(nullptr), right(nullptr) {}
};
```

### 2. `zig` 函数

```cpp
void zig(Node* node) {
   //...调整指针
}
```

 `zig` 函数实现了 Splay 树的左旋操作。

### 3. `zag` 函数

```cpp
void zag(Node* node) {
   //...调整指针
}
```

 `zag` 函数实现了 Splay 树的右旋操作。

### 4. `Emerge` 函数

```cpp
void Emerge(Node* node) {
    while (node->father != nullptr) {
        if (node->father->father != nullptr) {
            if (node->father->father->left == node->father) {
                if (node->father->left == node) {
                    zig(node->father);
                    zig(node);
                } else {
                    zag(node);
                    zig(node);
                }
            } else {
                if (node->father->left == node) {
                    zig(node);
                    zag(node);
                } else {
                    zag(node->father);
                    zag(node);
                }
            }
            continue;
        }
        if (node->father != nullptr) {
            if (node->father->left == node) {
                zig(node);
            } else {
                assert(node->father->right == node);
                zag(node);
            }
            continue;
        }
    }
    return;
}
```

 `Emerge` 函数实现了将指定节点上升到根节点的过程，通过反复依据其父节点以及爷爷节点是否存在来进行对应的 `zig` 和 `zag` 操作，确保目标节点成为树的根节点。

### 5. `add` 函数

```cpp
void add(int w) {
    Node* nodein = new Node(w);
    nodein->left = nullptr;
    nodein->right = nullptr;

    if (rootnom == nullptr) {
        rootnom = nodein;
        return;
    }

    Node* position = Search(rootnom, w);
    if (position->weight > w)
        position->left = nodein;
    else
        position->right = nodein;
    nodein->father = position;

    Emerge(nodein);
    rootnom = nodein;

    return;
}
```

 `add` 函数实现了在 Splay 树中添加新节点的过程。首先找到插入位置，然后插入新节点并通过 `Emerge` 上升到根节点。

### 6. `delet` 函数

```cpp
void delet(long long int keyy) {
    Node* toemerge;
    Node* node = Search(rootnom, keyy);

    if (node == nullptr)
        return;
    if (node->weight != keyy) {
        return;
    }

    Emerge(node);
    rootnom = node;
    node->father = nullptr;

    Node* right = node->right;
    Node* left = node->left;

    if (right != nullptr)
        right->father = nullptr;
    if (left != nullptr)
        left->father = nullptr;

    if (right == nullptr) {
        rootnom = left;
        if (left != nullptr)
            left->father = nullptr;
    } else {
        toemerge = Searchmin(right);
        Emerge(toemerge);
        rootnom = toemerge;
        toemerge->left = left;

        if (left != nullptr) {
            left->father = toemerge;
        }
    }
}
```

**功能分析**:

- **搜索节点：** 首先，通过 `Search` 函数找到权值等于 `keyy` 的节点，将其保存在变量 `node` 中。如果找不到对应节点，则直接返回。
- **将节点上升到根节点：** 使用 `Emerge` 函数将目标节点上升到根节点，确保该节点成为新的根节点。
- **更新树的根节点及子树连接：** 在树中删除节点 `node`，同时处理其左右子树。如果右子树为空，则直接以左子树为新树，更新根节点为左子树。如果右子树不为空，找到右子树中最小的节点 `toemerge`，再次通过 `Emerge` 函数将其上升到根节点，然后连接左子树。最终，更新树的根节点为 `toemerge`。
- **更新父节点指针：** 对新根节点 `toemerge` 的左子树进行父节点指针的更新，确保连接正确。

### 7. `ques` 函数：

```cpp
long long int ques(Node* root, long long int keyy) {
    if (root == nullptr)
        return -1;
    if (root->weight <= keyy) {
        if (root->right == nullptr)
            return root->weight;
        else {
            int answer = ques(root->right, keyy);
            if (answer > root->weight)
                return answer;
            else
                return root->weight;
        }
    } else {
        if (root->left == nullptr)
            return -1;
        else {
            return ques(root->left, keyy);
        }
    }
}
```

**功能分析**:

- `ques` 函数用于在 AVL 树中寻找小于等于 `keyy` 的最大键值。
- 递归搜索 AVL 树，根据当前节点的键值与 `keyy` 的比较决定继续在左子树或右子树中搜索。
- 如果当前节点的键值小于等于 `keyy`，则在右子树中继续搜索，否则在左子树中继续搜索。
- 如果右子树为空，则返回当前节点的键值；否则返回右子树中找到的最大值或当前节点的键值。

### 8. 时间复杂度

#### 1. `ques` 函数

- 时间复杂度： 主要涉及搜索和递归。递归次数复杂度为树的高度。因此，整体时间复杂度为 O(log n)。

#### 2. `add` 函数

- 时间复杂度： 主要涉及搜索、插入、上升操作。搜索的时间复杂度为树的高度O(log n)，插入的时间复杂度为O(1)，上升操作时间复杂度为树高O(log n)。因此，整体时间复杂度为 O(log n)，其中 n 是树的节点数。

#### 3. `delet` 函数

- 时间复杂度：要涉及搜索、上升、删除和连接操作。搜索、删除和上升的时间复杂度为树的高度，而连接操作的时间复杂度同样是树的高度。因此，整体时间复杂度为 O(log n)，其中 n 是树的节点数。

### 测例生成方式及设计思路

#### 1. 测例参数定义部分

```cpp
//三种操作总数
int totalnum = 200000;

//操作计数器
int numa = 0, numb = 0, numc = 0;

//数据局部性因子，因子越大，数据局部性越强，即相邻操作的操作数key值越接近。
double overbalance = 0.9;

//key值上界
int upper = 2000000;

//key值是否在树中
int inside[2100000];

// 三种操作的比例，通过调整这些概率值来控制生成的操作比例
double prob0 = 0.2;  // 添加操作比例
double prob1 = 0.1;  // 删除操作比例
double prob2 = 0.7;  // 查询操作比例
```

这一部分定义了一些全局变量，包括操作总数、操作计数器、数据局部性因子、key值上界、一个数组 `inside` 用于表示数据是否在树中，以及控制不同操作比例的概率变量 `prob0`、`prob1` 和 `prob2`。其中，`inside` 数组的大小是2100000，可能需要根据需求调整。

#### 2. 普通随机数生成器

```cpp
//普通随机数生成器
int NormalRandom(int k) {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(0, k);

    return distribution(generator);
}
```

这个函数实现了一个普通的均匀随机数生成器，返回一个介于0和 `k`之间的整数。

#### 3. 比例操作生成器

```cpp
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
```

这个函数用于生成比例操作，即根据输入的概率生成0、1、2三种操作之一。

#### 4. 局部性因子随机数生成器

```cpp
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
```

这个函数用于生成具有局部性（更靠近参数n的数据被生成的概率更大）的随机数据。它根据一个随机数是否大于局部性因子，来决定是生成普通随机数（超过 `overbalance`），还是在一个局部范围内生成局部性随机数，这个设计是为了使得即使在局部性因子很大的情况下，远离n的数据依然有可能被生成。在后者的情况下，使用局部性因子计算出一个靠近n的左右边界，使用 `NormalRandom` 函数生成介于左右边界之间的随机数以达到使得生成的数更靠近n，增强局部性的效果。

#### 5. 测例生成思路

依据以上功能，就测例而言，我们拥有三个自变量即：操作总量，操作比例，局部性因子。对于后两个自变量我们设置三个等级构造具体数据，操作总数固定为400000。

- 操作比例（插入：删除：查询）：6：2：2，2：6：2，2：2：6
- 局部性因子：0.15，0.5，0.85.

生成共9个不同的测例，对两种树共进行18次测试得到。

### 不同测例下的性能测试结果及原因分析

AVL Tree测试结果如下表。

| 操作比例\局部性因子 | 0.15  | 0.5   | 0.85  |
| ------------------- | ----- | ----- | ----- |
| 6：2：2             | 626ms | 648ms | 594ms |
| 2：6：2             | 346ms | 350ms | 349ms |
| 2：2：6             | 318ms | 301ms | 346ms |

Splay Tree测试结果如下表。

| 操作比例\局部性因子 | 0.15  | 0.5   | 0.85  |
| ------------------- | ----- | ----- | ----- |
| 6：2：2             | 861ms | 768ms | 546ms |
| 2：6：2             | 384ms | 345ms | 279ms |
| 2：2：6             | 300ms | 318ms | 256ms |

- 性能描述：首先对于两种树而言都是在插入操作占较大比例的情况下耗时较长，这比较容易理解，因为插入操作越多树的规模越大，高度越高，单次操作耗时越长。AVL树随着数据局部性的改变，耗时基本维持不变；Splay树随着数据局部性因子增大，耗时显著减小。在数据局部性因子较小时，AVL树性能优于Splay树；在数据局部性因子较大时，Splay树性能反超AVL树。
- 原因描述：AVL树和Splay树在数据局部性变化时表现不同的原因主要源于它们的结构差异和平衡机制的不同。无论数据局部性如何，AVL树始终保持平衡的结构，通过旋转操作来实现平衡。这种平衡过程涉及更多的结构调整，并严格控制树的高度。Splay树是自调整的二叉搜索树，通过将频繁访问的节点移到靠近根的位置来提高访问效率，将访问的节点移到根，树会变得更加“上重”，因为频繁访问的节点移向根部。
  由于AVL树保持相对平衡的结构，树的高度相对稳定，数据局部性的变化可能会影响树内实际数据的分布，但树的平衡机制倾向于减轻对整体性能的影响，最终结果是数据局部性的改变对性能实际影响不大。
  Splay树对数据局部性变化非常敏感。当数据呈现高局部性（对某些节点进行频繁访问）时，这些节点被伸展到根部，从而减少访问时间，故而随着数据局部性因子的增加，树明显变得更加高效。
