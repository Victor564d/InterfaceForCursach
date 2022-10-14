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
/// Удалить дерево или часть дерева. Также удалить все поддеревья, относящиеся к данному узлу.
/// </summary>
/// <param name="st">Корень дерева/текущаяя нода</param>
/// <returns>Пустой указатель на структуру</returns>
abonent* tree_delete(abonent* root);

/// <summary>
/// Удалить узел дерева по id. 
/// </summary>
/// <param name="root">Узел дерева, с которого начинать поиск.</param>
/// <param name="id">ID искомого узла.</param>
/// <returns>Корень дерева после изменения.</returns>
abonent* tree_deleteNodeById(abonent* root, const int id);

/// <summary>
/// Подсчитать количество узлов в дереве.
/// </summary>
/// <param name="root">Узел дерева, с которого начинать подсчёт.</param>
/// <param name="count">Стартовое значение счётчика.</param>
/// <returns>Количество элементво в дереве.</returns>
int tree_getNodeCount(const abonent* root, const int accum);


void printToFile(FILE* f, abonent* St);


void View(abonent* top, int otstup);


void addToTree(abonent** head, const abonent_t* info);


abonent_t* _get_output_info(abonent* root, abonent_t* _output_memory, int* index);

#endif // !TREE_OPERATION
