#include "tree_operation.h"
#include "VicMenuDLL.h"
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

abonent* getLeaf(abonent* root, int index)
{
    if (root == NULL)
        return NULL;
    else if (root->info.id == index)
        return root;
    else if (root->info.id < index)
        return getLeaf(root->right, index);
    else
        return getLeaf(root->left, index);
}

abonent* deleteNode(abonent* root, int id) {
    if (root == NULL) return root; // ����� ���� ������ ����
    if (root->info.id == id) { //������ ����. ����
        abonent* tmp; // ���������
        if (root->right == NULL) tmp = root->left;
        else { // ���������� ������ ���������
            abonent* ptr = root->right;
            if (ptr->left == NULL) { // � ������� �� ����������� ����� ��
                ptr->left = root->left;
                tmp = ptr;
            }
            else {
                abonent* pmin = ptr->left; // ����� ������ ������
                while (pmin->left != NULL) {// ���� � ������ ��
                    ptr = pmin;
                    pmin = ptr->left;
                } // ������ ����� ����� ���� ������� �� (pmin)
                ptr->left = pmin->right;
                pmin->left = root->left;
                pmin->right = root->right;
                tmp = pmin;
            }
        }
        free(root);
        root = NULL;
        return tmp;
    }
    else //�������� ����� � ����� ��� ������ ���������
        if (root->info.id >id)
            root->left = deleteNode(root->left, id);
        else
            root->right = deleteNode(root->right, id);
    return root;
}

int getLeafCount(abonent* root, int count) {
    if (root != NULL)
        return getLeafCount(root->left, count) + 1 + getLeafCount(root->right, count);
    else
        return 0;
}


void View(abonent* top, int otstup) {
    if (top) {
        otstup += 3; //������ �� ���� ������
        View(top->right, otstup); //����� ������� ���������
        for (int i = 0; i < otstup; i++) printf(" ");
        printf("|%d\n", top->info.id);
        View(top->left, otstup); //����� ������ ���������
    }
}

void addToTree(abonent** root, const abonent_t* info)
{
    if (*root == NULL)
    {
        *root = (abonent*)calloc(sizeof(abonent), 1);
        (*root)->info = *info;
    }
    else {
        if (((*root)->info.id == info->id)) {
            _message_window("������ � ����� id ��� ����������\0");
            Sleep(2000);
            return;
        }
        else if (((*root)->info.id > info->id))
            addToTree(&((*root)->left), info);
        else
            addToTree(&((*root)->right), info);
    }
}

abonent* deleteTree(abonent* root) {
    if (root) {
        if (root->left) {
            deleteTree(root->left);
        }
        if (root->right) {
            deleteTree(root->right);
        }
        free(root);
    }
    return NULL;
}

void printToFile(FILE* f, abonent* root)
{
    if (root) {
        abonent_t te = root->info;
        fwrite(&te, size_abonent_t, 1, f);
        if (root->left) {
            printToFile(f, root->left);
        }
        if (root->right) {
            printToFile(f, root->right);
        }

    }
}


abonent_t* _get_output_info(abonent* root, abonent_t* _output_memory, int* index) {
    {
        if (root) {
            if (root->left) {
                _get_output_info(root->left, _output_memory, index);
            }
            _output_memory[*index] = root->info;
            (*index)++;
            if (root->right) {
                _get_output_info(root->right, _output_memory, index);
            }
            return _output_memory;
        }
    }
}

