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
abonent* getLeaf(abonent* root, int indexToSerch);

/// <summary>
/// ����������� ������� �������� ������ �� ������.
/// </summary>
/// <param name="st">������ ������/�������� ����</param>
/// <returns>������ ��������� �� ���������</returns>
abonent* deleteTree(abonent* st);

/// <summary>
/// ����������� ������� ������ � �������� ����/����� �� ������ �� ��
/// </summary>
/// <param name="root">������ ������/������� ����</param>
/// <param name="id">�������� ������� ���� � ������</param>
/// <returns>������ ������ ����� ���������</returns>
abonent* deleteNode(abonent* root, int id);

/// <summary>
/// ����������� ������� �������� ���������� ������� � ������
/// </summary>
/// <param name="root">������ ������/�������� ����</param>
/// <param name="count">������������ ��� ����������� �������� ������</param>
/// <returns>���������� ��������� � ������</returns>
int getLeafCount(abonent* root, int count);


void printToFile(FILE* f, abonent* St);


void View(abonent* top, int otstup);


void addToTree(abonent** head, const abonent_t* info);


abonent_t* _get_output_info(abonent* root, abonent_t* _output_memory, int* index);

#endif // !TREE_OPERATION