#include "ThreeStruct.h"
#include <stdio.h>

#ifndef TREE_OPERATION
#define TREE_OPERATION

/// <summary>
/// ��������������� ����������� ������� ������ ������������ ������/����
/// </summary>
/// <param name="root">������ ������ ��� �������� ����</param>
/// <param name="indexToSerch">�� ������� ���������� �����</param>
/// <returns>���������� ��������� �� ������� � ��������������� ��, ����� NULL</returns>
abonent* tree_getLeafById(abonent* root, const int id);

/// <summary>
/// ������� ������ ��� ����� ������. ����� ������� ��� ����������, ����������� � ������� ����.
/// </summary>
/// <param name="st">������ ������/�������� ����</param>
/// <returns>������ ��������� �� ���������</returns>
abonent* tree_delete(abonent* root);

/// <summary>
/// ������� ���� ������ �� id. 
/// </summary>
/// <param name="root">���� ������, � �������� �������� �����.</param>
/// <param name="id">ID �������� ����.</param>
/// <returns>������ ������ ����� ���������.</returns>
abonent* tree_deleteNodeById(abonent* root, const int id);

/// <summary>
/// ���������� ���������� ����� � ������.
/// </summary>
/// <param name="root">���� ������, � �������� �������� �������.</param>
/// <param name="count">��������� �������� ��������.</param>
/// <returns>���������� ��������� � ������.</returns>
int tree_getNodeCount(const abonent* root, const int accum);

/// <summary>
/// �������� ������� � ������.
/// </summary>
/// <param name="head">��������� �� ��������� ������.</param>
/// <param name="info">����������� �������.</param>
void tree_add(abonent** head, const abonent_t* info);


void printToFile(FILE* f, abonent* St);

void View(abonent* top, int otstup);

abonent_t* _get_output_info(abonent* root, abonent_t* _output_memory, int* index);

#endif // !TREE_OPERATION
