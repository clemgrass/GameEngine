// optimizations.c
#include <stddef.h>

typedef struct {
    int a;
    int b;
    double c;
} Data;

// 1. Loop summing an array
int sum_array(const int *arr, size_t n) {
    int sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

// 2. Struct copy
Data copy_struct(Data d) {
    Data copy = d;
    return copy;
}

// 3a. Normal function
int multiply_normal(int x) {
    return x * 2;
}

// 3b. Inline function
static inline int multiply_inline(int x) {
    return x * 2;
}

int test_inline(int x) {
    return multiply_inline(x);
}

int test_normal(int x) {
    return multiply_normal(x);
}

// 4. Branch that compiler can simplify
int simplified_branch(void) {
    if (1) {
        return 42;
    } else {
        return 0;
    }
}

// 5. Pointer dereference chain
typedef struct Node {
    struct Node *next;
    int value;
} Node;

int dereference_chain(Node *n) {
    return n->next->next->value;
}