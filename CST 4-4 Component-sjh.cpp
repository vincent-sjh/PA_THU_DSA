#include<iostream>
#include<cassert>
using namespace std;

int k, length;

struct node {
    int index;
    long long int weight;
    int subtreenum;
    int distance;
    node* left;
    node* right;
    node* father;
    int level;
};

node point[1200000];
node* tableset[1200000];
node* create;
node* crea;
node* process;
node* f1;
node* f2;

node* Find(node* point) {
    assert(point != nullptr);
    if (point->father == point) {
        return point;
    }
    point->father = Find(point->father);
    assert(point->father);
    return point->father;
}

void Union(node* point1, node* point2) {
    f1 = Find(point1);
    f2 = Find(point2);
    if (f1->level >= f2->level) {
        f2->father = f1;
        f1->level += f2->level;
    } else {
        f1->father = f2;
        f2->level += f1->level;
    }
}
/**
 * @struct node
 * @brief Represents a node in a binary tree.
 * @param point1 dfsffsf
 * @return dffdsfs
 *
 * The `node` struct contains information about a node in a binary tree, including its weight, distance, subtreenum, left child, and right child.
 */
node* Merge(node* point1, node* point2) {
    node* top1 = point1;
    node* top2 = point2;
    assert(top1 != nullptr);
    assert(top2 != nullptr);
    if (top1 == nullptr) {
        return top2;
    }
    if (top2 == nullptr) {
        return top1;
    }
    if (top1->weight <= top2->weight) {
        if (top1->right == nullptr) {
            top1->right = top2;
            if (top1->left == nullptr) {
                swap(top1->right, top1->left);
            } else {
                if (top1->right->distance > top1->left->distance) {
                    swap(top1->right, top1->left);
                }
            }
            if (top1->right == nullptr) {
                top1->distance = 0;
            } else {
                top1->distance = top1->right->distance + 1;
            }
            top1->subtreenum += top2->subtreenum;
            return top1;
        } else {
            top1->subtreenum += top2->subtreenum;
            create = Merge(top1->right, top2);
            top1->right = create;
            if (top1->left == nullptr) {
                swap(top1->right, top1->left);
            } else {
                if (top1->right->distance > top1->left->distance) {
                    swap(top1->right, top1->left);
                }
            }
            if (top1->right == nullptr) {
                top1->distance = 0;
            } else {
                top1->distance = top1->right->distance + 1;
            }
            return top1;
        }
    } else {
        if (top2->right == nullptr) {
            top2->right = top1;
            if (top2->left == nullptr) {
                swap(top2->right, top2->left);
            } else {
                if (top2->right->distance > top2->left->distance) {
                    swap(top2->right, top2->left);
                }
            }
            if (top2->right == nullptr) {
                top2->distance = 0;
            } else {
                top2->distance = top2->right->distance + 1;
            }
            top2->subtreenum += top1->subtreenum;
            return top2;
        } else {
            top2->subtreenum += top1->subtreenum;
            create = Merge(top2->right, top1);
            top2->right = create;
            if (top2->left == nullptr) {
                swap(top2->right, top2->left);
            } else {
                if (top2->right->distance > top2->left->distance) {
                    swap(top2->right, top2->left);
                }
                if (top2->right == nullptr) {
                    top2->distance = 0;
                } else {
                    top2->distance = top2->right->distance + 1;
                }
            }
            return top2;
        }
    }
}

node* Merge_sizek(node* point1, node* point2) {

    create = Merge(point1, point2);
    length = create->subtreenum;
    if (length <= k) {
        return create;
    } else {
        process = create;
        assert(process != nullptr);
        for (int i = 0;i < length - k;i++) {
            assert((process->left != nullptr) || (process->right != nullptr));
            if ((process->left != nullptr) && (process->right != nullptr))
                process = Merge(process->left, process->right);
            else if (process->left == nullptr)
                process = process->right;
            else
                process = process->left;
        }
        assert(process != nullptr);
        return process;
    }
}

int main() {
    //解绑输入输出缓冲区，节省时间
    ios::sync_with_stdio(false);
    cin.tie(0);
    int m, n, q;
    cin >> n >> m >> k >> q;
    for (int i = 1;i <= n;i++) {
        cin >> point[i].weight;
        point[i].left = nullptr;
        point[i].right = nullptr;
        point[i].subtreenum = 1;
        point[i].distance = 0;
        point[i].index = i;
        tableset[i] = &point[i];
        point[i].father = &point[i];
        point[i].level = 1;
    }
    for (int i = 1;i <= m;i++) {
        int a, b;
        cin >> a >> b;
        if (Find(&point[a]) != Find(&point[b])) {
            node* root1 = tableset[Find(&point[a])->index];
            node* root2 = tableset[Find(&point[b])->index];
            crea = Merge_sizek(root1, root2);
            Union(&point[a], &point[b]);
            tableset[Find(&point[a])->index] = crea;
        }
    }
    for (int i = 0;i < q;i++) {
        int a;
        cin >> a;
        if (a == 1) {
            int c, d;
            cin >> c >> d;
            if (Find(&point[c]) != Find(&point[d])) {
                node* root1 = tableset[Find(&point[c])->index];
                node* root2 = tableset[Find(&point[d])->index];
                crea = Merge_sizek(root1, root2);
                Union(&point[c], &point[d]);
                tableset[Find(&point[c])->index] = crea;
            }
        } else {
            int e;
            cin >> e;
            if (tableset[Find(&point[e])->index]->subtreenum < k) {
                cout << -1 << '\n';
            } else {
                cout << tableset[Find(&point[e])->index]->weight << '\n';
            }
        }
    }

    return 0;
}