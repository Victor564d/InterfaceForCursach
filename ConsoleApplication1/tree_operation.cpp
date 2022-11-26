#include "tree_operation.h"
#include "VicMenuDLL.h"
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

abonent* tree_getLeafById(abonent* root, const int id)
{
    if (root == NULL)
        return NULL;
    else if (root->info.id == id)
        return root;
    else if (root->info.id < id)
        return tree_getLeafById(root->right, id);
    else
        return tree_getLeafById(root->left, id);
}

void tree_deleteNodeById(abonent** root, const int id)
{
    if (root == NULL || *root == NULL)
        return; // выход если пустой узел

    if ((*root)->info.id == id)
    {
        if ((*root)->right == NULL)
        {
            abonent* t = *root;
            *root = (*root)->left;
            free(t);
        }
        else
        {
            if ((*root)->right->left == NULL)
            {   
                abonent* tmp = (*root)->right;
                (*root)->right->left = (*root)->left;
                free(*root);
                *root = tmp;
            }
            else
            {
                abonent** candidate_prev = &((*root)->right);
                for (; (*candidate_prev)->left->left != NULL; candidate_prev = &((*candidate_prev)->left));

                abonent* candidate = (*candidate_prev)->left;
                (*candidate_prev)->left = candidate->right;
                candidate->left = (*root)->left;
                candidate->right = (*root)->right;
                free(*root);
                *root = candidate;
            }
        }
    }
    else
    {
        //бинарный поиск в левом или правом поддереве
        tree_deleteNodeById
        (
            (*root)->info.id > id ?
            &((*root)->left) :
            &((*root)->right),
            id
        );
    }
}



int tree_getNodeCount(const abonent* root, const int accum) {
   if (root != NULL)
   return tree_getNodeCount(root->left, accum) + 1 + tree_getNodeCount(root->right, accum);
    else
        return 0;
}


void View(abonent* top, int offset) {
    if (top)
    {
        offset += 3; //отступ от края экрана
        View(top->right, offset); //обход правого поддерева
        for (int i = 0; i < offset; i++) printf(" ");
        printf("|%d\n", top->info.id);
        View(top->left, offset); //обход левого поддерева
    }
}

void tree_add(abonent** root, const abonent_t* info)
{
    if (root == NULL || info == NULL)
    {
        return;
    }

    if (*root == NULL)
    {
        *root = (abonent*)calloc(sizeof(abonent), 1);

        if (*root != NULL)
        {
            (*root)->info = *info;
        }
    }
    else
    {
        /*if (((*root)->info.id == info->id)) {
            _message_window("Запись с таким id уже существует\0");
            Sleep(2000);
            return;
        }
        else */
        if (((*root)->info.id > info->id))
            tree_add(&((*root)->left), info);
        else
            tree_add(&((*root)->right), info);
    }
}

abonent* tree_delete(abonent* root) {
    if (root) {
        if (root->left) {
            tree_delete(root->left);
        }
        if (root->right) {
            tree_delete(root->right);
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

void printToFile_Text(FILE* f, abonent* root)
{
    if (root) {
        abonent_t te = root->info;
        fprintf(f, "%s %s %s %s\n%s %s %s\n%d %d %d %f\n", te.fio.surname, te.fio.name, te.fio.secondname, te.book_name, te.autor.surname, te.autor.inicial, te.izd, te.date_out.d, te.date_out.m, te.date_out.y, te.cost);
        if (root->left) {
            printToFile_Text(f, root->left);
        }
        if (root->right) {
            printToFile_Text(f, root->right);
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




dolgi_pers_t_obr* _get_dolgi_info(abonent* root, dolgi_pers_t_obr* _output_memory, int d, int m, int y) {
    {
        if (root) {
            if (root->left) {
                _get_dolgi_info(root->left, _output_memory, d, m, y);
            }
            if ((((y * 365) + (m * 31) + d) - ((root->info.date_out.y) * 365) - ((root->info.date_out.m) * 31) - (root->info.date_out.d)) > 90 ){
                if (!_output_memory) {
                   
                    _output_memory->info_mass = (dolgi_pers_t*)calloc(sizeof(dolgi_pers_t), 1);
                    _output_memory->info_mass[0].id = root->info.id;
                    _output_memory->info_mass[0].fio = root->info.fio;
                    _output_memory->info_mass[0].count_dolg_books++;
                    _output_memory->count++;
                }
                else {
                    int flag = 0;
                    for (int i = 0; i < _output_memory->count; i++ ) {
                        if (_output_memory->info_mass[i].id == root->info.id) {
                            _output_memory->info_mass[i].count_dolg_books++;
                            flag = 1;
                        }
                    }
                    if (!flag) {
                        _output_memory->info_mass = (dolgi_pers_t*)realloc(_output_memory->info_mass, sizeof(dolgi_pers_t) * ((_output_memory->count) + 1));
                        _output_memory->count++;
                        _output_memory->info_mass[(_output_memory->count) - 1].count_dolg_books = 1;
                        _output_memory->info_mass[(_output_memory->count) - 1].fio = root->info.fio;
                        _output_memory->info_mass[(_output_memory->count) - 1].id = root->info.id;
                    }
                }
            }        
            if (root->right) {
                _get_dolgi_info(root->right, _output_memory, d,m,y);
            }
            //return _output_memory;
        }
        return _output_memory;
    }
}

