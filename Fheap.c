#include "Fheap.h"
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fHeap* fh_create()
{
    struct fHeap* heap = (struct fHeap*)malloc(sizeof(struct fHeap));
    heap->min = NULL;
    heap->size = 0;
    return heap;
}

struct Node* node_create(int key, int value)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->mark = false;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    return node;
}

struct Node* fh_insert(struct fHeap* heap, int key, int value)
{
    struct Node* node = node_create(key, value);

    fh_AddNodeToRootList(node, heap->min); /* Добавляем node в список корней heap */

    if (heap->min == NULL || node->key < heap->min->key)
    {
        heap->min = node;
    }
    heap->size = heap->size + 1;

    return node;
}

void fh_AddNodeToRootList(struct Node* node, struct Node* h)
{
    struct Node* lnode;
    if (h == NULL)
    {
        return;
    }
    if (h->left == h) /* Случай 1: список h содержит 1 корень */
    {
        h->left = node;
        h->right = node;
        node->right = h;
        node->left = h;
    }
    else /* Случай 2: список h содержит > 1 корня */
    {
        lnode = h->left;
        h->left = node;
        node->right = h;
        node->left = lnode;
        lnode->right = node;
    }
}

struct Node* fh_min(struct fHeap* heap)
{
    return heap->min;
}

struct fHeap* fh_Union(struct fHeap* heap1, struct fHeap* heap2)
{
    struct fHeap* heap = fh_create();
    if (heap1->min && heap2->min)
    {
        heap->min = heap1->min;
        heap->min->left->right = heap2->min->right;
        heap2->min->right->left = heap->min->left;
        heap->min->left = heap2->min;
        heap2->min->right = heap->min;
        if (heap2->min->key < heap1->min->key)
        {
            heap->min = heap2->min;
        }
    }
    else if (heap1->min != NULL)
    {
        heap->min = heap1->min;
    }
    else if (heap2->min != NULL)
    {
        heap->min = heap2->min;
    }
    heap->size = heap1->size + heap2->size;
    free(heap1);
    free(heap2);
    return heap;
}

void fh_LinkLists(struct Node* node1, struct Node* node2)
{
    node1->left->right = node1->right;
    node1->right->left = node1->left;
    if (node2->child)
    {
        node1->right = node2->child;
        node1->left = node2->child->left;
        node2->child->left->right = node1;
        node2->child->left = node1;
    }
    else
    {
        node2->child = node1;
        node1->left = node1;
        node1->right = node1;
    }
    node2->degree++;
    node1->mark = false;
    node1->parent = node2;
}

int fh_DeleteMin(struct fHeap* heap)
{
    int res;
    int i;
    struct Node* ptr;
    struct Node* ptr2;

    res = heap->min->key;
    ptr = heap->min->child;
    for (i = 0; i < heap->min->degree; i++)
    {
        ptr2 = ptr->right;
        ptr->right = heap->min;
        ptr->left = heap->min->left;
        heap->min->left->right = ptr;
        heap->min->left = ptr;
        ptr = ptr2;
    }

    ptr = heap->min;
    ptr->right->left = ptr->left;
    ptr->left->right = ptr->right;

    if (ptr == ptr->right)
    {
        heap->min = NULL;
    }
    else
    {
        heap->min = ptr->right;
        fh_Consolidate(heap);
    }
    heap->size = heap->size - 1;

    free(ptr);
    return res;
}

void fh_Consolidate(struct fHeap* heap)
{
    int i, d, D;
    struct Node* x;
    struct Node* w;
    struct Node* v;
    struct Node* y;
    struct Node* tmp;
    struct Node** A; // вспомогательный массив указателей (элементы расположены по степеням)

    D = ceil(log2((float)(heap->size))); // max степень корня
    A = (struct Node**)calloc(D * sizeof(struct Node*), sizeof(struct Node*));

    v = heap->min;
    w = heap->min;
    do
    {
        x = w;
        w = w->right;
        d = x->degree;
        while (A[d] != NULL)
        {
            y = A[d];
            if (x->key > y->key) // fh_swap
            {
                tmp = x;
                x = y;
                y = tmp;
            }
            fh_LinkLists(y, x); // y становится дочерним узлом x
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    } while (w != v); //цикл по всем корням в списке

    heap->min = NULL; //удаляем минимальный элемент из цикл. списка
    for (i = 0; i < D; i++)
    {
        if (A[i] != NULL)
        {
            if (heap->min == NULL)
            {
                heap->min = A[i];
                heap->min->left = heap->min;
                heap->min->right = heap->min;
            }
            else
            {
                A[i]->right = heap->min;
                A[i]->left = heap->min->left;
                heap->min->left->right = A[i];
                heap->min->left = A[i];
                if (A[i]->key < heap->min->key)
                {
                    heap->min = A[i];
                }
            }
        }
    }
    free(A);
    return;
}

void fh_DecreaseKey(struct fHeap* heap, struct Node* x, int newkey)
{
    if (newkey > x->key) /* Новый ключ больше текущего значения ключа */
    {
        return;
    }
    x->key = newkey;
    struct Node* y = x->parent;

    if (y != NULL && x->key < y->key)
    {
        /* Нарушены свойства min-heap: ключ родителя больше */
        /* Вырезаем x и переносим его в список корней */
        fh_Cut(heap, x, y);
        fh_CascadingCut(heap, y);
    }
    /* 	Корректируем указатель на минимальный узел 	*/
    if (x->key < heap->min->key)
    {
        heap->min = x;
    }
}

void fh_Cut(struct fHeap* heap, struct Node* x, struct Node* y) // x становится корнем
{
    y->degree--;

    if (x->right == x)
    {
        y->child = NULL;
    }
    else
    {
        y->child = x->right;
        x->right->left = x->left;
        x->left->right = x->right;
    }
    x->right = heap->min;
    x->left = heap->min->left;
    heap->min->left->right = x;
    heap->min->left = x;

    x->parent = NULL;
    x->mark = false;

    return;
}

void fh_CascadingCut(struct fHeap* heap, struct Node* y)
{
    struct Node* z = y->parent;

    if (z != NULL)
    {
        if (y->mark == false)
        {
            y->mark = true;
        }
        else
        {
            fh_Cut(heap, y, z);
            fh_CascadingCut(heap, z);
        }
    }
}

void fh_Delete(struct fHeap* heap, struct Node* x)
{
    fh_DecreaseKey(heap, x, INT_MIN);
    fh_DeleteMin(heap);
}
