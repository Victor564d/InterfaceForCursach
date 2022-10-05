#include "ThreeStruct.h"
#include <stdio.h>
#ifndef TREE_OPERATION
#define TREE_OPERATION
abonent* getLeaf(abonent* root, int indexToSerch);//поиск листка по ид
abonent* deleteThree(abonent* st);
abonent* DeleteNode(abonent* root, int id);
int getLeafCount(abonent* root, int count);
void printToFile(FILE* f, abonent* St);
void View(abonent* top, int otstup);
void addToTree(abonent** head, const abonent_t* info);
abonent_t* _get_output_info(abonent* root, abonent_t* _output_memory, int* index);
#endif // !TREE_OPERATION
