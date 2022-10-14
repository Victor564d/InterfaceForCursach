#include "ThreeStruct.h"
#include <stdio.h>

#ifndef TREE_OPERATION
#define TREE_OPERATION

/// <summary>
/// Вспомогательная рекурсивная функция поиска необходимого листка/ноды
/// </summary>
/// <param name="root">Корень дерева или текущаяя нода</param>
/// <param name="indexToSerch">Ид который необходимо найти</param>
/// <returns>Возвращает указатель на элемент с соответствующим ид, иначе NULL</returns>
abonent* tree_getLeafById(const abonent* root, const int id);

/// <summary>
/// Рекурсивная функция удаления дерева из памяти.
/// </summary>
/// <param name="st">Корень дерева/текущаяя нода</param>
/// <returns>Пустой указатель на структуру</returns>
abonent* deleteTree(abonent* st);

/// <summary>
/// Рекурсивная функция поиска и удаления ноды/листа из дерева по ид
/// </summary>
/// <param name="root">Корень дерева/текущая нода</param>
/// <param name="id">Значение которое ищем в дереве</param>
/// <returns>Корень дерева после изменения</returns>
abonent* deleteNode(abonent* root, int id);

/// <summary>
/// Рекурсивная функция подсчета количества записей в дереве
/// </summary>
/// <param name="root">Корень дерева/текущаяя нода</param>
/// <param name="count">Используется для рекурсивной передачи данных</param>
/// <returns>Количество элементво в дереве</returns>
int getLeafCount(abonent* root, int count);


void printToFile(FILE* f, abonent* St);


void View(abonent* top, int otstup);


void addToTree(abonent** head, const abonent_t* info);


abonent_t* _get_output_info(abonent* root, abonent_t* _output_memory, int* index);

#endif // !TREE_OPERATION
