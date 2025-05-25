#include "hashtable.h"
int naive_hashing::operator()(char* str, int N) {
    if (str == NULL) return 0;
    else return (str[0] + N) % N;
}

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


int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice) {
    return (last_choice + 1) % table_size;
}

void linear_probe::init() {
    return;// do nothing
}

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

int overflowzone_probe::operator()(hash_entry* Table, int table_size, int last_choice) {
    overflowzone_start++;
    return overflowzone_start;
}

void overflowzone_probe::init() {
    overflowzone_start = 300016;
    return;
}