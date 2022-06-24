#pragma warning(disable : 4996);
#include "VicMenuDLL.h"; //файл библиотеки меню
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include "ThreeStruct.h"

#define clearf() system("cls");


const char* Menu[] = { "Добавить новый элемент",
    "Загрузить из файла",
    "Записать все в файл ",
    "Вывод данных в списке",
    "Редактировать запись",
    "Отобразить структуру дерева",
    "Удалить элемент(любой)",
    "Анимация",
    "Картинка",
    "Очистить дерево",
    "Колличество элементов в дереве",
    "Отображение всех записей в которых нет ниодной тройки",
    "Среднее арифметическое по всем предметам",
    "Уровень элемента по ид",
    "Тест",
    "Выход" };

const int SizeStudent = sizeof(STudent);
const int MenuSize = 16;
HANDLE hConsole;
//------------------------------------------------------------------- Область функций ----------------------------------------------------------------
//-----------------------------------функции создания дерева----------------------------------------------
/// <summary>
/// Функция добавления элемента в дерево
/// </summary>
/// <param name="head">Указатель на корень лерева</param>
/// <param name="info">Информация которую необходимо сохранить</param>
void addToTree(Person** head, const STudent* info);
/// <summary>
/// Функция вывода меню добавления записи
/// </summary>
/// <param name="st">Указатель на корень дерева</param>
/// <returns>При успешном добавлении возвращает 1</returns>
int аddNewElement(Person** st);
/// <summary>
/// Функция реализующая ввод через форму
/// </summary>
/// <param name="d">Информационные поля</param>
/// <returns>Возвращает информационную структуру которую необходимо сохранить</returns>
STudent GetInfoFromCeyboard(STudent  d);
/// <summary>
/// Отображение структуры дерева
/// </summary>
/// <param name="top">Параметр корня дерева/текущей ноды</param>
/// <param name="otstup">Текущий отступ вывода</param>
void View(Person* top, int otstup);
//----------------------------------Функции работы с файлами----------------------------------------------
/// <summary>
/// Функция сохранения дерева в файл
/// </summary>
/// <param name="f">Указатель на файл</param>
/// <param name="St">Корень дерева</param>
/// <returns>В случае успеха вернет 1</returns>
int create_file(FILE* f, Person* St);
/// <summary>
/// Функция загрузки данных из файла в дерево
/// </summary>
/// <param name="f">Указатель на файл</param>
/// <returns>Возвращает указатель на корень дерева</returns>
Person* loadFromFile_new(FILE* f);
/// <summary>
/// Рекурсивная функция сохранения данных в файл
/// </summary>
/// <param name="f">Указатель на файл</param>
/// <param name="St">Корень дерева/текущая нода</param>
void printToFile(FILE* f, Person* St);
//------------------------------Функции работы с элементами дерева----------------------------------------
/// <summary>
/// Основное тело функции редактирования записи по ид
/// </summary>
/// <param name="st">Указатель на корень дерева</param>
/// <returns>В случае успеха - 1</returns>
int correctInfo(Person* st);
/// <summary>
/// Вспомогательная рекурсивная функция поиска необходимого листка/ноды
/// </summary>
/// <param name="root">Корень дерева или текущаяя нода</param>
/// <param name="indexToSerch">Ид который необходимо найти</param>
/// <returns>Возвращает указатель на элемент с соответствующим ид, иначе NULL</returns>
Person* getLeaf(Person* root, int indexToSerch);//поиск листка по ид
/// <summary>
/// Рекурсивная функция удаления дерева из памяти 
/// </summary>
/// <param name="st">Корень дерева/текущаяя нода</param>
/// <returns>Пустой указатель на структуру</returns>
Person* deleteThree(Person* st);
/// <summary>
/// Рекурсивная функция поиска и удаления ноды/листа из дерева по ид
/// </summary>
/// <param name="root">Корень дерева/текущая нода</param>
/// <param name="id">Значение которое ищем в дереве</param>
/// <returns>Корень дерева после изменения</returns>
Person* DeleteNode(Person* root, int id);
/// <summary>
/// Рекурсивная функция подсчета количества записей в дереве
/// </summary>
/// <param name="root">Корень дерева/текущаяя нода</param>
/// <param name="count">Используется для рекурсивной передачи данных</param>
/// <returns>Количество элементво в дереве</returns>
int getLeafCount(Person* root, int count);
/// <summary>
/// Рекурсивная функция подсчета среднего арефметического по предметам
/// </summary>
/// <param name="root">Корень дерева/текущая нода</param>
/// <param name="summ">Указатель на массив флоат в котором хранятся значения</param>
/// <returns>Указатель на массив из четырех элементов типа флоат где лежит сумма балов по предметам</returns>
float* getSerArefm(Person* root, float* summ);
//--------------------------------Функции вывода списка на экран------------------------------------------

/// <summary>
/// Рекурсивная функция печати информации из дерева на экран. Обход с лева на право.
/// </summary>
/// <param name="st">Корень дерева/текущая нода</param>
/// <returns>В случае успеха - 1</returns>
int PrintTreeData(Person* st);
/// <summary>
/// Форма вывода записи на экран
/// </summary>
/// <param name="d">Указатель на сируктуру с информационными полями</param>
/// <param name="index">Текущий номер элемента</param>

int PrintTreeDataNonThree(Person* root);
//-------------------------------Вспомогательные функции -------------------------------------------------
/// <summary>
/// Функция выбора необходимой операции. Использует для получения вызов библиотеки VicMenuDLL
/// </summary>
/// <param name="selector">Индекс выбранного элемента</param>
/// <param name="f">Указатель на файл</param>
void MenuSelect(int selector, FILE* f);
/// <summary>
/// Функция получения подтверждения от пользователя
/// </summary>
/// <returns>Если подвердили - вернет 1</returns>
int getResponse();
/// <summary>
/// Функция реализующая ввод через форму
/// </summary>
/// <param name="d">Информационные поля</param>
/// <returns>Возвращает информационную структуру которую необходимо сохранить</returns>
STudent GetInfoFromCeyboard(STudent  d);
/// <summary>
/// Заголовок таблицы
/// </summary>
int printTable();
/// <summary>
/// Горизонтальный разделитель таблицы
/// </summary>
int printBorder();
int correctInfo(Person* st);
/// <summary>
/// Вспомогательная рекурсивная функция поиска необходимого листка/ноды
/// </summary>
/// <param name="root">Корень дерева или текущаяя нода</param>
/// <param name="indexToSerch">Ид который необходимо найти</param>
/// <returns>Возвращает указатель на элемент с соответствующим ид, иначе NULL</returns>
Person* getLeaf(Person* root, int indexToSerch);//поиск листка по ид
/// <summary>
/// Рекурсивная функция удаления дерева из памяти 
/// </summary>
/// <param name="st">Корень дерева/текущаяя нода</param>
/// <returns>Пустой указатель на структуру</returns>
Person* deleteThree(Person* st);
/// <summary>
/// Рекурсивная функция поиска и удаления ноды/листа из дерева по ид
/// </summary>
/// <param name="root">Корень дерева/текущая нода</param>
/// <param name="id">Значение которое ищем в дереве</param>
/// <returns>Корень дерева после изменения</returns>
Person* DeleteNode(Person* root, int id);
/// <summary>
/// Рекурсивная функция подсчета количества записей в дереве
/// </summary>
/// <param name="root">Корень дерева/текущаяя нода</param>
/// <param name="count">Используется для рекурсивной передачи данных</param>
/// <returns>Количество элементво в дереве</returns>
int getLeafCount(Person* root, int count);
/// <summary>
/// Рекурсивная функция подсчета среднего арефметического по предметам
/// </summary>
/// <param name="root">Корень дерева/текущая нода</param>
/// <param name="summ">Указатель на массив флоат в котором хранятся значения</param>
/// <returns>Указатель на массив из четырех элементов типа флоат где лежит сумма балов по предметам</returns>
float* getSerArefm(Person* root, float* summ);

int GetLeafLevel(Person* root, int n, int serchID);

void printINFO(STudent* d, int index);
/// <summary>
/// Рекурсивная функция вывода информации о студентах без троек
/// </summary>
/// <param name="root">Корень дерева/текущая нода</param>
/// <returns>В случае успеха - значение > 1 </returns>
/// 
/// 
/// 
/// 
/// 
/// 
int* _get_window_size();
/// 
/// 
/// 
/// 
/// 
//------------------------------------------------------------------- Конец области функций ---------------------------------------------------------
Person* st = NULL;
Person* root = NULL;
int position[] = { 1,1 };
int* size;

int main(void) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCP(65001); // Задаем таблицу символов для консоли.
    SetConsoleOutputCP(65001);
    clearf();
    // system("color F0");
    size = _get_window_size();
    _print_bakground(size[0], size[1]);
    FILE* f = fopen("data.dat", "rb+");//Открытие существующего файла для чтения и записи в конец
    if (!f) {
        f = fopen("data.dat", "wb+"); //Создание нового файла для обновления
        if (!f) {
            puts("Не могу открыть (создать) файл\n");
            return 1;
        }
    }
    while (1) {//вывод меню и запуск соответствующих функций
        clear();
        MenuSelect(PrintMenu(Menu, position, MenuSize, 2), f);
    }
}
int Posid = 1;
void MenuSelect(int selector, FILE* f)
{
    switch (selector) {
    case 1:
        аddNewElement(&st);
        break;
    case 2:
        if (getResponse()) {
            st = loadFromFile_new(f);
        }
        break;
    case 3:
        create_file(f, st);
        break;
    case 4:
        clear();
        Posid = 1;
        PrintTreeData(st);
        puts("Нажмите любую клавишу");
        getch();
        break;
    case 5:
        correctInfo(st);
        break;
    case 6:
        clear();
        printf("---------------------------------------------- Структура дерева -----------------------------------------\n");
        View(st, 1);
        printf("-------------------------------------------- Конец струк. дерева ----------------------------------------\n");
        getch();
        break;
    case 7:
        if (getResponse()) {
            printf("\nВведите ИД записи -->"); int l; scanf("%d", &l);
            st = DeleteNode(st, l);
            puts("Ветка удалена ... Нажмите любую кнопку..."); getch();
        }
        clear();
        break;
    case 8:
        if (getResponse()) {
            animatedNeko();
        }
        break;
    case 9:
        if (getResponse()) {
            neko(34097);
            getch();
        }
        break;
    case 10:
        if (getResponse()) {
            st = deleteThree(st);
            printf("\n\n Дерево очищенно, нажмите любую кнопку...");
            getch();
        }
        break;
    case 11:
        if (getResponse()) {
            printf("\n\n Дерево содержит %d записей. Нажмите любую кнопку...", getLeafCount(st, 0));
            getch();
        }
        break;
    case 12:
        Posid = 1; printf("\n");
        if (PrintTreeDataNonThree(st) == 1)
            printf("\n\n Студентов без троек нет. Нажмите любую кнопку...");
        getch();

        break;
    case 13:
        if (getResponse()) {
            float  summ[] = { 0,0,0,0 };
            getSerArefm(st, &summ);
            int l = getLeafCount(st, 0);
            float temp = summ[0] / l;
            printf("\n Средняя оценка по Физике --> %.4f ", temp);
            temp = summ[1] / l;
            printf("\n Средняя оценка по Математика --> %.4f ", temp);
            temp = summ[2] / l;
            printf("\n Средняя оценка по Информатика --> %.4f ", temp);
            temp = summ[3] / l;
            printf("\n Средняя оценка по История --> %.4f ", temp);
            getch();
        }
        break;
    case 14:
        printf("\nВведите ИД записи -->"); int l; scanf("%d", &l); l = GetLeafLevel(st, 0, l);
        printf("Уровень на котором элемент --> %d", l);
        getch();
        break;
    case 15:

        size = _get_window_size();
        _print_bakground(size[0],size[1]);
        getch();
        break;
    case 16:
        if (getResponse()) {
            if (!st) {
                deleteThree(st);
            }
            exit(666);
        }
        break;
    }
}


int create_file(FILE* f, Person* root)
{
    if (!root) return 666;
    fclose(f); f = fopen("data.dat", "wb+");
    fseek(f, 0, SEEK_SET);
    printToFile(f, root);
    clear();
    puts("Данные сохранены. Нажмите любую кнопку ....");
    getch();
    return 0;
}

void printToFile(FILE* f, Person* root)
{
    if (root) {
        STudent te = root->info;
        fwrite(&te, SizeStudent, 1, f);
        if (root->left) {
            printToFile(f, root->left);
        }
        if (root->right) {
            printToFile(f, root->right);
        }

    }
}

int PrintTreeData(Person* root) {
    if (root) {
        if (root->left) {
            PrintTreeData(root->left);
        }
        printINFO(&root->info, Posid);
        Posid++;
        if (root->right) {
            PrintTreeData(root->right);
        }
    }
    return 0;
}

Person* deleteThree(Person* root) {
    if (root) {
        if (root->left) {
            deleteThree(root->left);
        }
        if (root->right) {
            deleteThree(root->right);
        }
        free(root);
    }
    return NULL;
}

void addToTree(Person** root, const STudent* info)
{
    if (*root == NULL)
    {
        *root = (Person*)calloc(sizeof(Person), 1);
        (*root)->info = *info;
    }
    else {
        if ((*root)->info.id > info->id)
            addToTree(&((*root)->left), info);
        else
            addToTree(&((*root)->right), info);
    }
}

int аddNewElement(Person** st)
{
    STudent d; d.id = NULL; d.birth_date = NULL;
    char** Menu[] = { "Добавить запись","Выход" };
    int position[] = { 1,1 }; int flag = 0;
    while (1) {
        clear();
        if (flag)
        {
            switch (PrintMenuWithTable(Menu, position, 2, 1, printINFO, &d, 1)) {
            case 1:
            {
                d.id = NULL; d.birth_date = NULL;
                d = GetInfoFromCeyboard(d);
                addToTree(st, &d); flag = 1;
            }
            break;
            case 2:
                return 1;
            }

        }
        else
            switch (PrintMenu(Menu, position, 2, 1)) {
            case 1:
            {

                d = GetInfoFromCeyboard(d);
                addToTree(st, &d); flag = 1;
            }
            break;
            case 2:
                return 1;
            }
    }
}

void View(Person* top, int otstup) {
    if (top) {
        otstup += 3; //отступ от края экрана
        View(top->right, otstup); //обход правого поддерева
        for (int i = 0; i < otstup; i++) printf(" ");
        printf("|%d\n", top->info.id);
        View(top->left, otstup); //обход левого поддерева
    }
}


Person* loadFromFile_new(FILE* f)
{
    STudent tmp;
    Person* head = NULL;
    int count = 1;
    fseek(f, 0, SEEK_SET);
    printf("\n");      // printTable();
    while (fread(&tmp, sizeof(STudent), 1, f))
    {
        addToTree(&head, &tmp);
        printINFO(&tmp, count);
        count++;
    }
    puts("Данные считаны. Нажмите любую кнопку ");
    getch();
    return head;
}

Person* DeleteNode(Person* root, int id) {
    if (root == NULL) return root; // выход если пустой узел
    if (root->info.id == id) { //найден удал. узел
        Person* tmp; // указатель
        if (root->right == NULL) tmp = root->left;
        else { // существует правое поддерево
            Person* ptr = root->right;
            if (ptr->left == NULL) { // у правого ПД отсутствует левое ПД
                ptr->left = root->left;
                tmp = ptr;
            }
            else {
                Person* pmin = ptr->left; // поиск самого левого
                while (pmin->left != NULL) {// узла в правом ПД
                    ptr = pmin;
                    pmin = ptr->left;
                } // найден самый левый узел правого ПП (pmin)
                ptr->left = pmin->right;
                pmin->left = root->left;
                pmin->right = root->right;
                tmp = pmin;
            }
        }
        free(root);
        return tmp;
    }
    else //бинарный поиск в левом или правом поддереве
        if (id < root->info.id)
            root->left = DeleteNode(root->left, id);
        else
            root->right = DeleteNode(root->right, id);
    return root;
}

int getLeafCount(Person* root, int count) {
    if (root) {
        if (root->left) {
            count = getLeafCount(root->left, count);
        }
        count++;
        if (root->right) {
            count = getLeafCount(root->right, count);
        }
    }
    return count;
}


int PrintTreeDataNonThree(Person* root) {
    if (root) {
        if (root->left) {
            PrintTreeDataNonThree(root->left);
        }
        if ((root->info.marks.fiz > 3) && (root->info.marks.math > 3) && (root->info.marks.it > 3) && (root->info.marks.history > 3))
        {
            printINFO(&root->info, Posid);
            Posid++;
        }
        if (root->right) {
            PrintTreeDataNonThree(root->right);
        }
    }
    return Posid;
}


float* getSerArefm(Person* root, float* summ) {
    if (root) {
        if (root->left) {
            summ = getSerArefm(root->left, summ);
        }
        summ[0] = summ[0] + root->info.marks.fiz;
        summ[1] = summ[1] + root->info.marks.math;
        summ[2] = summ[2] + root->info.marks.it;
        summ[3] = summ[3] + root->info.marks.history;
        if (root->right) {
            summ = getSerArefm(root->right, summ);
        }
    }
    return summ;
}


int getResponse() {
    printf("\n Вы уверены что хотите выполнить данную команду ? [Y | Any]  "); char c = getch();
    if ((c == 'y') || (c == 'Y')) return 1; else return 0;
}

STudent GetInfoFromCeyboard(STudent  d)
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char del = '|'; int index = 1;
    if ((d.id != NULL) || (d.birth_date != NULL))
    {
        int* position = (int*)malloc(2 * sizeof(int));
        position[0] = 1; position[1] = 1; int tempPosition = 1; int flag = 0;
        char c;
        while (1) {
            clear();   printTable();
            switch (position[0]) {
            case 1:
            case 2: {
                printf("%c %-3d %c->%-5d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 3: {
                printf("%c %-3d %c %-6d %c->%-31s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 4: {
                printf("%c %-3d %c %-6d %c %-32s %c->%-29s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 5: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c->%-13d %c %-17d %c %-8d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 6: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c->%-16d %c %-8d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 7: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c->%-7d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 8: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c->%-11d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 9: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c->%-17d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 10: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c->%-8d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\nВыход из редактирования\n");
                break;
            }
            case 11: {
                printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c %-9d %c\n", del, index, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del, d.marks.history, del);
                printBorder();
                printf("\n--> Выход из редактирования\n");
                break;
            }
            default: { }
            }
            COORD positionCur = { 50,3 }; //позиция x и y


            c = getch();
            if (c != 13) {
                position = GetCurentSelector(c, position, 2, 11);
                if ((position[1] == 2) && (!flag)) {
                    tempPosition = position[0];
                    position[0] = 11;
                    flag = 1;
                }
                else { if (flag) { position[0] = tempPosition; flag = 0; } }
            }
            else {
                switch (position[0]) {
                case 1:
                case 2: {
                    positionCur.X = 7; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("       ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.id);
                    break;
                }
                case 3: {
                    positionCur.X = 16; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("                               ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%s", &d.surname);
                    break;
                }
                case 4: {
                    positionCur.X = 51; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("                             ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%s", &d.name);
                    break;
                }
                case 5: {
                    positionCur.X = 84; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("             ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.birth_date);
                    break;
                }
                case 6: {
                    positionCur.X = 101; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("                ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.stud_date);
                    break;
                }
                case 7: {
                    positionCur.X = 121; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("       ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.marks.fiz);
                    break;
                }
                case 8: {
                    positionCur.X = 132; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("           ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.marks.math);
                    break;
                }
                case 9: {
                    positionCur.X = 147; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("                 ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.marks.it);
                    break;
                }
                case 10: {
                    positionCur.X = 168; //x 
                    SetConsoleCursorPosition(hConsole, positionCur);
                    printf("        ");
                    SetConsoleCursorPosition(hConsole, positionCur);
                    scanf("%d", &d.marks.history);
                    break;
                }
                case 11: {
                    return d;
                    break;
                }
                }
            }

        }
    }
    else {
        printf("\n");
        printTable();
        printf("|     | "); scanf("%d", &d.id); clear();
        printTable(); printf("|     | %-6d | ", d.id);
        scanf("%s", &d.surname); clear(); char del = '|'; int l = 1;
        printTable(); printf("%c %-3d %c %-6d %c %-32s %c", del, l, del, d.id, del, d.surname, del);
        scanf("%s", &d.name);
        clear(); printTable();
        printf("%c %-3d %c %-6d %c %-32s %c %-30s %c", del, l, del, d.id, del, d.surname, del, d.name, del);
        scanf("%d", &d.birth_date);
        clear(); printTable();
        printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c", del, l, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del);
        scanf("%d", &d.stud_date);
        clear(); printTable();
        printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c", del, l, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del);
        scanf("%d", &d.marks.fiz);
        clear(); printTable();
        printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c", del, l, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del);
        scanf("%d", &d.marks.math);
        clear(); printTable();
        printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c", del, l, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del);
        scanf("%d", &d.marks.it);
        clear(); printTable();
        printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c", del, l, del, d.id, del, d.surname, del, d.name, del, d.birth_date, del, d.stud_date, del, d.marks.fiz, del, d.marks.math, del, d.marks.it, del);
        scanf("%d", &d.marks.history);
    }
    return d;
}

int printBorder() {
    for (int i = 0; i < 180; i++)
        printf("-");
}

int printTable()
{
    char* TableColsName[] = { " № "," id "," Фамилия "," Имя "," Год рождения "," Год поступления "," Физика "," Математика "," Программирование "," История " };
    printBorder(); printf("\n"); char del = '|';
    printf("%c %s %c  %s  %-13c %-8s %12c %18s %13c %s %c %s %c %s %c %s %c %s %c %s %c\n", del, TableColsName[0], del, TableColsName[1], del, TableColsName[2], del, TableColsName[3], del, TableColsName[4], del, TableColsName[5], del, TableColsName[6], del, TableColsName[7], del, TableColsName[8], del, TableColsName[9], del);
    printBorder(); printf("\n");
}

void printINFO(STudent* d, int index)
{
    if (index == 1 || index == 0) printTable(); //заголовок таблицы
    char del = '|';
    printf("%c %-3d %c %-6d %c %-32s %c %-30s %c %-14d %c %-17d %c %-8d %c %-12d %c %-18d %c %-9d %c", del, index, del, d->id, del, d->surname, del, d->name, del, d->birth_date, del, d->stud_date, del, d->marks.fiz, del, d->marks.math, del, d->marks.it, del, d->marks.history, del);
    printf("\n");
    printBorder();
    printf("\n");
}

int correctInfo(Person* st)
{
    char** Menu1[] = { "Найти по номеру","Выйти в главное меню", "Редактировать","Сохранить" }; //меню если элемент для редактирования активен
    char** Menu2[] = { "Найти по номеру","Выйти в главное меню" }; //меню если элемента нет 
    int position[] = { 1,1 }; //массив позиции для меню
    STudent d, old;  d.id = NULL; d.birth_date = NULL;  Person* temp = NULL;
    while (1)
    {
        int select; int Posid = 1; clear();
        if ((d.id != NULL) || d.birth_date != NULL) //если запись с которо работаем   - есть
        {
            select = PrintMenuWithTable(Menu1, position, 4, 1, printINFO, &d, 1); // вызов меню с чермя параметрами 
        }
        else select = PrintMenu(Menu2, position, 2, 1); // если записи нет - меню с двумя параметрами 
        switch (select)
        {
        case 3:
        {

            d = GetInfoFromCeyboard(d); // получение данных с клавиатуры
            break;
        }
        case 4:
        {
            clear();
            printf("\n--------------------------------------------------------------------------------- Новая запись ------------------------------------------------------------------------------------- \n");
            printINFO(&d, 1); // вывод информации
            printf("\n-------------------------------------------------------------------------------- Старая запись ------------------------------------------------------------------------------------- \n");
            printINFO(&old, 1); // вывод информации
            printf("\n");
            if (getResponse()) {
                temp->info = d;
                puts("Данные сохранены . Нажмите любую кнопку");
                getch();
            }
        }
        break;
        case 1: {
            int SerchID; printf("\nВведите идентификационный номер -->"); scanf("%d", &SerchID);
            int flag = 0;  if (!st) break;
            temp = getLeaf(st, SerchID);
            if (temp) {
                d = temp->info;
                old = temp->info;
            }
            if (!temp) {
                printf("Не найден студент с таким идентификатором .... Нажмите любую кнопку"); getch();
                d.id = NULL; d.birth_date = NULL;
            }
            break; }
        case 2: return 0;
        }
    }
}

Person* getLeaf(Person* root, int index)
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




int GetLeafLevel(Person* root, int n, int serchID) {
    if (root == NULL)
        return 0;
    else if (root->info.id == serchID)
        return n;
    else if (root->info.id < serchID) {
        n++;
        return n = GetLeafLevel(root->right, n, serchID);
    }
    else {
        n++;
        return n = GetLeafLevel(root->left, n, serchID);
    }

}



int* _get_window_size() {
    HANDLE hWndConsole;
    int size[]={ 0,0 };
    if (hWndConsole = GetStdHandle(-12))
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
        {
            size[0] = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
            size[1] = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
        }
        else
            printf("Error: %d\n", GetLastError());
    }
    return size;
}