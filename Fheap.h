#pragma once

struct Node
{
    int value;
    int key;             // ключ
    struct Node* parent; // указатель на родительский узел
    struct Node* child;  // указатель на один из дочерних узлов
    struct Node* left;   // указатель на левый узел того же предка
    struct Node* right;  // указатель на правый узел того же предка
    int degree;          // степень вершины
    int mark;            // был ли удален в процессе изменения ключа ребенок этой вершины)
};

struct fHeap
{
    int size;         // текущее количество узлов
    struct Node* min; // указатель на корень дерева с минимальным ключом
};

struct fHeap* fh_create();

struct Node* fh_insert(struct fHeap* heap, int key, int value);

void fh_AddNodeToRootList(struct Node* node, struct Node* h);

struct Node* fh_min(struct fHeap* heap);

struct fHeap* fh_Union(struct fHeap* heap1, struct fHeap* heap2);

void fh_LinkLists(struct Node* node1, struct Node* node2);

int fh_DeleteMin(struct fHeap* heap);

void fh_Consolidate(struct fHeap* heap);

void fh_DecreaseKey(struct fHeap* heap, struct Node* x, int newkey);

void fh_Cut(struct fHeap* heap, struct Node* x, struct Node* y);

void fh_CascadingCut(struct fHeap* heap, struct Node* y);

void fh_Delete(struct fHeap* heap, struct Node* x);
