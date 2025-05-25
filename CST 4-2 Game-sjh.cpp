#include<iostream>
#include<cassert>

using namespace std;

int innernum = 0;

struct node;
struct list {
    node* index;
    list* next;

    list(node* a) :index(a), next(nullptr) {}
};

struct node {
    int index;
    bool set;
    bool reach;

    long long int time;
    long long int mindistance;
    long long int pathnum;
    list* link;
};

node game[110000];

struct Heap {
    node reachdot[200000];
    int size;

    Heap() : size(0) {}

    bool exam(int i) {
        if (i >= size) {
            return true;
        }
        if ((2 * i + 1 < size) && (reachdot[i].mindistance > reachdot[2 * i + 1].mindistance))
            return false;

        if ((2 * i + 2 < size) && (reachdot[i].mindistance > reachdot[2 * i + 2].mindistance))
            return false;


        return exam(2 * i + 1) && exam(2 * i + 2);
    }

    void push(node a) {
        reachdot[size] = a;
        size++;
        int present = size - 1;

        while ((present > 0) && (reachdot[present].mindistance < reachdot[(present + 1) / 2 - 1].mindistance)) {
            node temp = reachdot[present];
            reachdot[present] = reachdot[(present + 1) / 2 - 1];
            reachdot[(present + 1) / 2 - 1] = temp;
            present = (present + 1) / 2 - 1;
        }
        // assert(exam(0));
        return;
    }



    node pop() {
        assert(size > 0);
        node ans = reachdot[0];
        reachdot[0] = reachdot[size - 1];
        size--;

        int present = 0;
        while ((2 * present + 1 < size)) {
            if (2 * present + 2 >= size) {
                if (reachdot[2 * present + 1].mindistance < reachdot[present].mindistance) {
                    node temp = reachdot[present];
                    reachdot[present] = reachdot[2 * present + 1];
                    reachdot[2 * present + 1] = temp;
                    present = 2 * present + 1;
                } else {
                    break;
                }
            } else {
                if ((reachdot[2 * present + 1].mindistance <= reachdot[2 * present + 2].mindistance) && (reachdot[2 * present + 1].mindistance < reachdot[present].mindistance)) {
                    node temp = reachdot[present];
                    reachdot[present] = reachdot[2 * present + 1];
                    reachdot[2 * present + 1] = temp;
                    present = 2 * present + 1;
                } else if ((reachdot[2 * present + 2].mindistance < reachdot[2 * present + 1].mindistance) && (reachdot[2 * present + 2].mindistance < reachdot[present].mindistance)) {
                    node temp = reachdot[present];
                    reachdot[present] = reachdot[2 * present + 2];
                    reachdot[2 * present + 2] = temp;
                    present = 2 * present + 2;
                } else {
                    break;
                }
            }
        }
        // assert(exam(0));
        return ans;
    }
};

Heap insect;

int main() {

    //节约输入输出时间
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    for (int i = 1;i <= n;i++) {
        cin >> game[i].time;
        game[i].index = i;
        game[i].set = false;
        game[i].reach = false;
        game[i].mindistance = 0;
        game[i].pathnum = 0;
        game[i].link = nullptr;
    }

    for (int i = 1;i <= m;i++) {
        int a, b;
        cin >> a >> b;
        list* toadd1 = new list(&game[b]);
        toadd1->next = game[a].link;
        game[a].link = toadd1;
        list* toadd2 = new list(&game[a]);
        toadd2->next = game[b].link;
        game[b].link = toadd2;
    }


    insect.size = 0;

    game[1].set = true;
    game[1].pathnum = 1;
    game[1].reach = true;

    list* templist = game[1].link;
    while (templist != nullptr) {


        templist->index->reach = true;
        templist->index->pathnum = game[1].pathnum;
        templist->index->mindistance = game[1].mindistance + game[1].time;
        insect.push(*templist->index);

        templist = templist->next;
    }

    while (1) {

        node temp = insect.pop();
        while (game[temp.index].set == true) {
            temp = insect.pop();
        }

        game[temp.index].set = true;
        bool found = false;
        int tofind = 1;



        //assert(game[tofind].mindistance == game[temp.index].mindistance);

        if (temp.index == n)
            break;

        templist = game[temp.index].link;

        while (templist != nullptr) {
            if (templist->index->set == true) {
                templist = templist->next;
                continue;
            }
            if (templist->index->reach == true) {
                if (templist->index->mindistance > game[temp.index].mindistance + game[temp.index].time) {
                    templist->index->mindistance = game[temp.index].mindistance + game[temp.index].time;
                    templist->index->pathnum = game[temp.index].pathnum;
                    insect.push(*templist->index);
                } else if (templist->index->mindistance == game[temp.index].mindistance + game[temp.index].time) {
                    templist->index->pathnum = (game[temp.index].pathnum + templist->index->pathnum) % 1000000007;

                } else {
                    templist = templist->next;
                    continue;
                }

            } else {
                templist->index->reach = true;
                templist->index->pathnum = game[temp.index].pathnum;
                templist->index->mindistance = game[temp.index].mindistance + game[temp.index].time;
                insect.push(*templist->index);
            }
            templist = templist->next;
        }

    }

    cout << game[n].mindistance + game[n].time << endl;
    cout << game[n].pathnum % 1000000007 << endl;

    return 0;
}