#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cassert>
#include<ctime>

using namespace std;

struct node {
    int height;
    int key;
    node* left;
    node* right;
    node* dad;

    node(const int& p) : height(0), key(p), left(nullptr), right(nullptr), dad(nullptr) {}
};



node* rootavl = nullptr;

int height(node* root) {
    if (root == nullptr)
        return -1;
    else
        return root->height;
}
node* Search(node* root, int keyy) {
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

bool rift(node* root) {
    assert(root != nullptr);
    assert(root->dad != nullptr);

    if (root->dad->left == root)
        return true;
    else
        return false;
}

node* regroup(node* root) {
    node* t1;
    node* t2;
    node* v = root;
    node* s;
    node* g;
    s = (height(v->left) > height(v->right)) ? v->left : v->right;
    if (height(s->left) > height(s->right))
        g = s->left;
    else if (height(s->left) < height(s->right))
        g = s->right;
    else
        g = (rift(s)) ? s->left : s->right;

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
            t1 = g->left;
            t2 = g->right;
            v->right = t1;
            if (t1 != nullptr)
                t1->dad = v;
            s->left = t2;
            if (t2 != nullptr)
                t2->dad = s;
            g->left = v;
            g->right = s;
            v->dad = g;
            s->dad = g;
            v->height = (height(v->left) > height(v->right)) ? height(v->left) : height(v->right);
            v->height++;
            s->height = (height(s->left) > height(s->right)) ? height(s->left) : height(s->right);
            s->height++;
            g->height = (height(g->left) > height(g->right)) ? height(g->left) : height(g->right);
            g->height++;
            return g;
        } else {
            v->right = s->left;
            if (v->right != nullptr)
                s->left->dad = v;
            s->left = v;
            v->dad = s;
            v->height = (height(v->left) > height(v->right)) ? height(v->left) : height(v->right);
            v->height++;
            s->height = (height(s->left) > height(s->right)) ? height(s->left) : height(s->right);
            s->height++;
            return s;
        }
    }
}

node* Searchmin(node* root) {

    node* pro = root->right;
    while (pro->left != nullptr) {
        pro = pro->left;
    }
    return pro;
}

void Updateheight(node* here) {
    int heighting = 0;
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

void add(node* root, int keyy) {
    if (root == nullptr) {
        rootavl = new node(keyy);
        rootavl->dad = nullptr;
        return;
    } else {
        node* pos = Search(root, keyy);

        //assert(pos->key != keyy);

        if (keyy < pos->key) {
            pos->left = new node(keyy);
            pos->left->dad = pos;
            Updateheight(pos);

        } else if (keyy > pos->key) {

            pos->right = new node(keyy);
            pos->right->dad = pos;
            Updateheight(pos);
        }
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

bool exam(node* root) {
    bool index = true;
    if (root == nullptr)
        return index;
    if (root->left != nullptr) {
        index = index && (root->left->key < root->key);
        index = index && exam(root->left);
    }
    if (root->right != nullptr) {
        index = index && (root->right->key > root->key);
        index = index && exam(root->right);
    }
    return index;
}

void delet(node* root, int keyy) {

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
            //assert(min->left == nullptr);
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
                //assert(min->left == nullptr);
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

int ques(node* root, int keyy) {
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


int main() {

    int n;
    char s;
    int keyy;

    //double t1 = clock();

    //freopen("avl.txt", "w", stdout);
    //freopen("1in.txt", "r", stdin);

    scanf("%d", &n);

    int answer;
    for (int i = 0;i < n;i++) {

        scanf("%c", &s);
        scanf("%d", &keyy);


        if (s == 'A') {
            add(rootavl, keyy);
            //assert(exam(rootavl));
        } else if (s == 'B') {
            delet(rootavl, keyy);
            //assert(exam(rootavl));
        } else {
            answer = ques(rootavl, keyy);
            printf("%d\n", answer);
        }
    }

    //double t2 = clock();
    //double result = (t2 - t1);

    //cout << "Toal time of AVL tree: " << result << "ms" << '\n';

    return 0;
}