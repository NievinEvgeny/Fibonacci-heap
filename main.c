#include "Fheap.h"
#include <stdio.h>

int main()
{
    struct fHeap *heap, *hp;
    struct Node *n, *m;

    heap = fh_create(); //создание кучи

    fh_insert(heap, 10, 100);     //
    fh_insert(heap, 15, 100);     //
    fh_insert(heap, 7, 100);      //
    fh_insert(heap, 24, 100);     //
    fh_insert(heap, 23, 100);     //
    n = fh_insert(heap, 96, 100); //   вставляем элементы
    fh_insert(heap, 43, 100);     //
    m = fh_insert(heap, 80, 100); //
    fh_insert(heap, 26, 100);     //
    fh_insert(heap, 35, 100);     //
    fh_insert(heap, 46, 100);     //

    printf("Извлекаем минимум: %d\n", fh_DeleteMin(heap));

    printf("\nПонижение 80 -> 55\n");
    fh_DecreaseKey(heap, m, 55);
    printf("%d\n", m->key);
    fh_Delete(heap, n);

    hp = fh_create(); //создание второй кучи
    fh_insert(hp, 100, 100);
    fh_insert(hp, 200, 100);
    fh_insert(hp, 300, 100);
    printf("\nИзвлекаем минимум второй кучи: %d\n", fh_DeleteMin(hp));

    hp = fh_Union(heap, hp); // объединяем кучи

    printf("\nНаходим минимальный элемент после объединения: %d\n", hp->min->key);
}