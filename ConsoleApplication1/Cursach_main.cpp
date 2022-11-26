#pragma warning(disable : 4996);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <conio.h>
#include <Windows.h>

#include "utf8.h"

#include "include/data_utils.h"
#include "ThreeStruct.h"
#include "VicMenuDLL.h"
#include "MenuStruct.h"
#include "tree_operation.h"


#define clearf() system("cls");

const int size_abonent_t = sizeof(abonent_t);
const int menu_size = 5;
HANDLE hConsole;

int аddNewElement(abonent** st, _tabel_metadata *table);
int create_file_type(FILE* f, abonent* St);
abonent* loadFromFile_new_type(FILE* f);
abonent* loadFromFile_new_text(FILE* f);
int correctInfo(abonent* st);
void MenuSelect(int selector, FILE* f, _tabel_metadata* table);
int correctInfo(abonent* st);
_menu_item* _init_menu(_menu_item* menu);
_tabel_metadata* _init_table(_tabel_metadata* table);
abonent* abonents = NULL;
int position[] = { 1,1 };
int* size;

int main(void) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
    system("chcp 1251");
    SetConsoleCP(1251); // Задаем таблицу символов для консоли.
    SetConsoleOutputCP(65001);
    clearf();
    _menu_item* menu = NULL;
    _tabel_metadata* table = NULL;
    menu = _init_menu(menu);
    table = _init_table(table);
    abonent_t* _output_info;
    // system("color F0");
    FILE* f =  fopen("data.dat", "rb+");//Открытие существующего файла для чтения и записи в конец
    if (!f) {
        f = fopen("data.dat", "wb+"); //Создание нового файла для обновления
        if (!f) {
            puts("Не могу открыть (создать) файл\n");
            return 1;
        }
    }
    while (1) {//вывод меню и запуск соответствующих функций
        clear();
        int leafCount = tree_getNodeCount(abonents, 0);
        _output_info = (abonent_t*)calloc(leafCount, sizeof(abonent_t));
        int  temp = 0;
        _output_info =  _get_output_info(abonents, _output_info,&temp);
        if (leafCount == 0)
            _output_info = NULL;
        static sort_struct sort;
        sort.sort_f = DEF;
        sort.sort_t = UP;
        MenuSelect(_print_menu(menu, position, menu_size, 5,_output_info,leafCount,table, &abonents, &sort), f,table);
    }
}
int Posid = 1;

//"Добавить новый элемент", 1
//"Загрузить из файла", 2
//"Записать все в файл ", 3
//"Редактировать запись", 4
//"Удалить элемент(любой)", 5
//"Очистить дерево", 6
//"Уровень элемента по ид", 7 
//"Вывод данных в списке",  8 
//"Отобразить структуру дерева", 9 
//"Колличество элементов в дереве", 10 
//"Отобр. всех зап. в ко-ых нет ниодной тройки", 11
//"Ср. арифм. по всем предметам", 12
//"Анимация", 13
//"Картинка", 14 
//"Выход" 16 


void MenuSelect(int selector, FILE* f,_tabel_metadata *table )
{
    char* a = (char*)calloc(200, sizeof(char));
    switch (selector) {
    
    case ADD_NEW_RECORD:
        аddNewElement(&abonents,table);
        break;
    
    case LOAD_FROM_FILE_TYPE:
        if (_confirm_window(NULL)) {
            f = fopen("data.dat", "rb+");
            abonents = loadFromFile_new_type(f);
        }
        break;
    
    case SAVE_TO_FILE_TYPE:
        create_file_type(f, abonents);
        break;
    case CLEAN_TREE:
        size = _get_window_size();
        if (_confirm_window(NULL)) {
            abonents = tree_delete(abonents);
            sprintf(a, "Дерево очищено");
            _message_window( a);
            getch();
        }
        break;
    case PRINT_TREE_STRUCT:
        clear();
        printf("---------------------------------------------- Структура дерева -----------------------------------------\n");
        View(abonents, 1);
        printf("-------------------------------------------- Конец струк. дерева ----------------------------------------\n");
        getch();
        break;   
    case TREE_SIZE:
        if (_confirm_window(NULL)) {
            sprintf(a, "Дерево содержит %d записей.",tree_getNodeCount(abonents, 0));
            _message_window(a);
            getch();
        }
        break;  
    case PRINT_HORRIBLE_ANIMATION:
        animatedNeko();
        break;
    case LOAD_FROM_FILE:
        if (_confirm_window(NULL)) {
            _in_window();
            char a[90]; scanf("%s", a);
            sprintf(a, "%s.txt", a);
            FILE* text = fopen(a, "rt");
            if (!text) break;
            abonents = loadFromFile_new_text(text);
            fclose(text);
        }       
        break;
    case SAVE_TO_FILE:
        if (_confirm_window(NULL)) {
            _in_window();
            char a[90]; scanf("%s", a);
            sprintf(a, "%s.txt", a);
            FILE* text = fopen(a, "wt");
            if (!text) break;
            printToFile_Text(text, abonents);
            fclose(text);
            
        }
            break;
    case DOLGNIKI_WINDWO:
        if (_confirm_window(NULL)) {
            dolgiWindow(&abonents);
            

        }
        break;
    case PROGRAM_EXIT:
        if (_confirm_window(NULL)) {
            if (!abonents) {
                tree_delete(abonents);
            }
            exit(666);
        }
        break;
        
    }
}

int create_file_type(FILE* f, abonent* root)
{
    if (!root)
        return 666;
    
    fclose(f);
    f = fopen("data.dat", "wb+");

    if (f == NULL)
    {
        _message_window("Не удалось открть файл для записи");
        Sleep(2000);
        return 666;
    }

    fseek(f, 0, SEEK_SET);
    
    printToFile(f, root);
    fclose(f);
    _message_window("Данные успешно сохранены...");
    Sleep(2000);
    
    return 0;
}

int аddNewElement(abonent** st,_tabel_metadata* table)
{
    abonent_t* d; 
        while (1) {
         d =   _in_info_window(table, NULL, 1);
         if (!d)   return EXIT_SUCCESS;
         d->id = util_hashCodeFromFio(&d->fio);
         tree_add(st, d);
              }
    return EXIT_SUCCESS;
}

abonent* loadFromFile_new_text(FILE* f)
{
    abonent_t tmp;
    abonent* head = NULL;
    int count = 1;
    while (1) {
        fscanf(f, "%s %s %s", tmp.fio.surname, tmp.fio.name, tmp.fio.secondname);
        //fscanf(f, "%s", tmp.book_name, "\n");
        fgets(tmp.book_name, 200, f);
        fscanf(f, "%s %s", tmp.autor.surname, tmp.autor.inicial);
        fgets(tmp.izd, 200, f);
        fscanf(f, "%d %d %d", &(tmp.date_out.d), &(tmp.date_out.m), &(tmp.date_out.y));
        fscanf(f, "%f", &(tmp.cost));
        if (feof(f)) break;
        for (int i = 0; i < strlen(tmp.book_name); i++) {
            if (tmp.book_name[i] == '\n') tmp.book_name[i] = '\0';
        }
        for (int i = 0; i < strlen(tmp.izd); i++) {
            if (tmp.izd[i] == '\n') tmp.izd[i] = '\0';
        }
        
        tmp.id = util_hashCodeFromFio(&tmp.fio);
        tree_add(&head, &tmp);
        count++;
        // fprintf(f, "%s %s %s\n%s\n%s %s\n%s\n%d %d %d %f\n", te.fio.surname, te.fio.name, te.fio.secondname, te.book_name, te.autor.surname, te.autor.inicial, te.izd, te.date_out.d, te.date_out.m, te.date_out.y, te.cost);
    }
    _message_window("Данные считаны");
    Sleep(2000);
    return head;
}

abonent* loadFromFile_new_type(FILE* f)
{
    abonent_t tmp;
    abonent* head = NULL;
    int count = 1;
    fseek(f, 0, SEEK_SET);
    while (fread(&tmp, sizeof(abonent_t), 1, f))
    {
        tree_add(&head, &tmp);
        count++;
    }
    _message_window("Данные считаны");
    Sleep(2000);
    return head;
}

int PrintTreeDataNonThree(abonent* root) {
    if (root) {
        if (root->left) {
            PrintTreeDataNonThree(root->left);
        }
     //   if ((root->info.marks.fiz > 3) && (root->info.marks.math > 3) && (root->info.marks.it > 3) && (root->info.marks.history > 3))
        {
            //printINFO(&root->info, Posid);
            Posid++;
        }
        if (root->right) {
            PrintTreeDataNonThree(root->right);
        }
    }
    return Posid;
}

int correctInfo(abonent* st)
{
   return EXIT_SUCCESS;
}


_menu_item* _init_menu(_menu_item* menu) {
    menu = (_menu_item *) calloc(1, sizeof(_menu_item) * 5);
//--------------------------------------------------------------------------------------------------------- 
    strcpy(menu[0]._name, "Внести данные");
    menu[0]._menu_name_lenght = 14;
    menu[0]._menu_size = 3;
    menu[0]._sub_menu = (char**)calloc(menu[0]._menu_size, sizeof(char*) );
    for (int i = 0; i < menu[0]._menu_size; i++) {
        menu[0]._sub_menu[i] = (char*)calloc(90, sizeof(char));
                }
    menu[0]._sub_menu_lenght = (int*)calloc(3, sizeof(int));
        strcpy(menu[0]._sub_menu[0], "Добавить новый элемент c клавиатуры");  menu[0]._sub_menu_lenght[0] = 36;
        strcpy(menu[0]._sub_menu[1], "Загрузить из типизированного файла");      menu[0]._sub_menu_lenght[1] = 35;
        strcpy(menu[0]._sub_menu[2], "Загрузить из текстового файла");     menu[0]._sub_menu_lenght[2] = 30;
//--------------------------------------------------------------------------------------------------------- 
        strcpy(menu[1]._name, "Сохранить");
        menu[1]._menu_name_lenght = 10;
        menu[1]._menu_size = 2;
        menu[1]._sub_menu = (char**)calloc(menu[1]._menu_size, sizeof(char*) );
        for (int i = 0; i < menu[1]._menu_size; i++) {
            menu[1]._sub_menu[i] = (char*)calloc(90, sizeof(char)); 
        }
        menu[1]._sub_menu_lenght = (int*)calloc(4, sizeof(int));
        strcpy(menu[1]._sub_menu[0], "Сохранить в типизированный файл");        menu[1]._sub_menu_lenght[0] = 32;
        strcpy(menu[1]._sub_menu[1], "Сохранить в текстовый файл"); menu[1]._sub_menu_lenght[1] = 27;
//--------------------------------------------------------------------------------------------------------- 
        strcpy(menu[2]._name, "Работа с деревом");
        menu[2]._menu_name_lenght = 17;
        menu[2]._menu_size = 4;
        menu[2]._sub_menu = (char**)calloc(menu[2]._menu_size, sizeof(char*));
        for (int i = 0; i < menu[2]._menu_size; i++) {
            menu[2]._sub_menu[i] = (char*)calloc(90, sizeof(char));
           
        }
        menu[2]._sub_menu_lenght = (int*)calloc(5, sizeof(int));
        strcpy(menu[2]._sub_menu[0], "Отобразить структуру дерева");  menu[2]._sub_menu_lenght[0] = 28;
        strcpy(menu[2]._sub_menu[1], "Колличество элементов в дереве");         menu[2]._sub_menu_lenght[1] = 31;
        strcpy(menu[2]._sub_menu[2], "Очистить дерево");  menu[2]._sub_menu_lenght[2] = 16;
        strcpy(menu[2]._sub_menu[3], "Список должников");         menu[2]._sub_menu_lenght[3] = 14;
//--------------------------------------------------------------------------------------------------------- 
        strcpy(menu[3]._name, "Прочее");
        menu[3]._menu_name_lenght = 7;
        menu[3]._menu_size = 2;
        //"Анимация",
        //"Картинка",
        menu[3]._sub_menu = (char**)calloc(menu[3]._menu_size, sizeof(char*) );
        for (int i = 0; i < menu[3]._menu_size; i++) {
            menu[3]._sub_menu[i] = (char*)calloc(90, sizeof(char));
            
        }
        menu[3]._sub_menu_lenght = (int*)calloc(2, sizeof(int));
        strcpy(menu[3]._sub_menu[0], "Анимация"); menu[3]._sub_menu_lenght[0] = 9;
        strcpy(menu[3]._sub_menu[1], "Картинка"); menu[3]._sub_menu_lenght[1] = 9;
//--------------------------------------------------------------------------------------------------------- 
        strcpy(menu[4]._name, "Выход");
        menu[4]._menu_name_lenght = 6;
        menu[4]._menu_size = 0;
        //"Выход"
        menu[4]._sub_menu = NULL;
    return menu;
}

_tabel_metadata* _init_table(_tabel_metadata* table) {
    table = (_tabel_metadata*)calloc(sizeof(_tabel_metadata), 1);
    table->_col_count = 7;
    table->_cols = (_table_col*)calloc(sizeof(_table_col) , table->_col_count);
    //-------------------------------------------------------------------------------------------//
    table->_cols[0].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[0].name, "№");
    table->_cols[0].resizebl = 0;
    table->_cols[0].size = u8_strlen(table->_cols[0].name);
    //-------------------------------------------------------------------------------------------//
    table->_cols[1].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[1].name, "ФИО абонента");
    table->_cols[1].resizebl = 1;
    table->_cols[1].size = u8_strlen(table->_cols[1].name);
    //-------------------------------------------------------------------------------------------//
    table->_cols[2].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[2].name, "Автор книги");
    table->_cols[2].resizebl = 0; 
    table->_cols[2].size = u8_strlen(table->_cols[2].name); table->_cols[2].size =18;
    //-------------------------------------------------------------------------------------------//
    table->_cols[3].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[3].name, "Название книги");
    table->_cols[3].resizebl = 1;
    table->_cols[3].size = u8_strlen(table->_cols[3].name);
    //-------------------------------------------------------------------------------------------//
    table->_cols[4].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[4].name, "Издательство");
    table->_cols[4].resizebl = 0;
    table->_cols[4].size = u8_strlen(table->_cols[4].name);
    //-------------------------------------------------------------------------------------------//
    table->_cols[5].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[5].name, "Дата выдачи");
    table->_cols[5].resizebl = 0;
    table->_cols[5].size = u8_strlen(table->_cols[5].name);
    //-------------------------------------------------------------------------------------------//
    table->_cols[6].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[6].name, "Стоимость,р.");
    table->_cols[6].resizebl = 0;
    table->_cols[6].size = u8_strlen(table->_cols[6].name);
    //-------------------------------------------------------------------------------------------//
    return table;
}



//"Добавить новый элемент",
//"Загрузить из файла",
//"Записать все в файл ",
//"Редактировать запись",
//"Удалить элемент(любой)",
//"Очистить дерево",
//"Уровень элемента по ид",
//"Вывод данных в списке",  
//"Отобразить структуру дерева",
//"Колличество элементов в дереве",
//"Отобр. всех зап. в ко-ых нет ниодной тройки",
//"Ср. арифм. по всем предметам",
//"Анимация",
//"Картинка",
//"Выход"
