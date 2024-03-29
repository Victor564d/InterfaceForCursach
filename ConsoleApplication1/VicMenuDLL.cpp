﻿#pragma warning(disable : 4996)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <conio.h>
#include <Windows.h>

#include "utf8.h"

#include "VicMenuDLL.h"
#include "MenuStruct.h"
#include "tree_operation.h"
#include "input_utils.h"

const int _otstup = 3; //Хранит отступ слева и справа в окне
const int _interval = 3; //хранит интервал сверху и снизу окна
int* _window_size; //указатель на массив размера 
int _first_start = 1; //флаг первого запуска.
COORD positionCur = {4,4}; //Хранит текущую позицию курсора 

#define clearf() system("cls"); // Полная очистка экрана
#define KEY_ENTER 13 // дефайны для кнопок 
#define KEY_ESC 27

int page = 1; // хранит текущую страницу.

BOOL _get_con_info(CONSOLE_SCREEN_BUFFER_INFO* x) //Внутренняя функция, получает информацию из буфера экрана. 
{
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

BOOL _set_cur_to_pos(HANDLE h, COORD cor) { //Устанавливает курсор на необходимую позицию 
    return SetConsoleCursorPosition(h, cor);
}


void clear() { //функция кастомной очистки , чистит только окно 
    positionCur.X = _otstup + 1;  positionCur.Y = _interval + 1; //установить стартовые позиции 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //получить хендлер консоли
    int* temp_window_size = NULL; //получить размеры окна
    temp_window_size = _get_window_size(temp_window_size);
    _set_cur_to_pos(hConsole, positionCur); // установить курсор в позицию
    _print_border(temp_window_size[0], temp_window_size[1]); // напечатать внутренний бордер
    positionCur.X = _otstup + 1;  positionCur.Y = _interval + 1; //данные курсора обнулить в исходное состояние
        _set_cur_to_pos(hConsole, positionCur); // установить курсор в исходное состояние
}



void clear_table() { //Внутренняя функция очистки таблицы 
    positionCur.X = _otstup + 2;  positionCur.Y = _interval + 3; //ставим курсор согласно параметрам положения таблицы
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //получаем хендлер консоли
    int* temp_window_size = NULL; // получаем размер кона
    temp_window_size = _get_window_size(temp_window_size);
    _set_cur_to_pos(hConsole, positionCur); int _window_h = temp_window_size[1];
    int _window_w = temp_window_size[0]; //запомнинаем рахмеры , ставим курсор в нужную позицию
    for (int y = _interval+3; y <= _window_h - _interval-3; y++) { // цикл очистки таблицы
        _set_cur_to_pos(hConsole, positionCur);
        for (int x = _otstup + 2; x < _window_w - _otstup-1; x++) {
             printf(" "); //_otstup+2,_interval+3
        }
        positionCur.Y++;
    }
}



void clear_for_info() { //установка курсора в позицию для вывода информации
    positionCur.X = _otstup + 4;  positionCur.Y = _interval + 4;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    _set_cur_to_pos(hConsole, positionCur);
}


int* _get_curent_selection(char  c // Символ клавиатуры
    ,int * position // Массив в котором хранятся x и y
    ,int MaxY // Максимальный количество строк 
    , int Colums  // Количество столбцов, по умолчанию - 1
    ,int _flag_x_readonly //можно ли менять x?
) 
{
    int x = position[0]; int y = position[1];
    switch (c)
    {
    case KEY_ARROW_UP://вверх
        if (y > 1) y--;
        break;
    case KEY_ARROW_DOWN://вниз
        if (y < MaxY) y++;
        break;
    case KEY_ARROW_LEFT://лево
        if (x > 1) x--;
        if(_flag_x_readonly) if (y > 1) y--;
        break;
    case KEY_ARROW_RIGHT://право
        if (x < Colums) x++;
        if (_flag_x_readonly) if (y < MaxY) y++;
        break;
    }
    if (!_flag_x_readonly)
     position[0] = x; //запоминаем позицию по столбцу
    position[1] = y;//позиция по строке
    return position;
}

int _print_menu(_menu_item* _menu //Массив объектов  меню
    , int* position //Массив текущей позиции x и y
    , int _menu_size  //Колличество элементов в массиве объектов меню
    , int _menu_buttons,//Количество кнопок меню  
    abonent_t * _output_mas, //массив для вывода информации
    int _output_colcount, // количество данных для вывода
    _tabel_metadata * table, //структура хранящая информацию о полях таблицы
    abonent** root, //указатель на указатель на корень дерева 
    sort_struct* sort // структура, хранящая текущую сортировку
)
{
    int  table_focus_flag = 0; // флаг работы с таблицей
    int _padding; //оступ 
    int _new_padding; //новый отступ
    int* _size_now = NULL; //текущий размер окна 
    if (_first_start) { // Если у нас первый запуск
        _window_size = _get_window_size(_window_size); //порлучаем размер окна 
        _print_bakground(_window_size[0], _window_size[1]); // вызываем отрисовку заднего фона
        _first_start = 0; // первый запуск закончен
    }
    COORD positionCur = { _otstup + 1, _interval + 1 }; //позиция x и y у курсора. Стартовый отступ 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //получаем хендлер консоли
    _set_cur_to_pos(hConsole, positionCur);// установим курсор в позици. 
    CONSOLE_SCREEN_BUFFER_INFO info_x; //необходимо для получения размера консоли 
    int _max_subm_lenght = 0;  //поиск максимальной длины записи в сабменю
    for (int i = 0; i < _menu_size; i++) { //цикл прохода по всем записям в меню
        if (_menu[i]._menu_size > 0) {
            _max_subm_lenght = _menu[i]._sub_menu_lenght[0];
            for (int j = 1; j < _menu[i]._menu_size; j++) {
                if (_menu[i]._sub_menu_lenght[j] > _max_subm_lenght) {
                    _max_subm_lenght = _menu[i]._sub_menu_lenght[j];
                }
                _menu[i]._max_sub_lenght = _max_subm_lenght; //получаем максимальный размер в сабменю, который будет использован для его вывода в дальнейшем
            }
        }
    }
      
    while (1) //цикл отрисовки меню 
    {
        _size_now = _get_window_size(_size_now); //получение текущего размера окна 
        if ((_size_now[0] != _window_size[0]) || (_size_now[1] != _window_size[1])) //ели текузий размер не совпал с стартовым
        {
            _window_size[0] = _size_now[0];  _window_size[1] = _size_now[1]; // запоминаем текущий размер 
            clearf(); // очистка экрана
            _print_bakground(_window_size[0], _window_size[1]); //по новой отрисовываем задний фон и рабочую область
            positionCur.X = _otstup + 1; positionCur.Y = _interval + 1; // задаем базовые стартовые параметры курсора 
            _set_cur_to_pos(hConsole, positionCur); //устанавливаем курсор в начальное положение
            page = 1;
        }
        positionCur.X = _otstup + 1; positionCur.Y = _interval + 1;   
        _set_cur_to_pos(hConsole, positionCur);//устанавливаем курсор в начальное положение
        for (int i = 0; i < _menu_size; i++) { //Пока у нас есть элементы в меню 
            _get_con_info(&info_x); // получаем текущее положение 
            _padding = info_x.dwCursorPosition.X - 1; //задаем текущий падинг 
            if (position[0] == i + 1) { //если у нас позиция курсора-указателя совпадает с текущей позицией
                printf("\x1b[43m %s \x1b[0m", _menu[i]._name); // выделяем цветом
            }
            else printf(" %s ", _menu[i]._name); // иначе просто выводим на экран
            printf("│");  //разделитель
            _get_con_info(&info_x);
            positionCur.X = info_x.dwCursorPosition.X - 1; // получаем текущую позицию курсора, сместив его на один символ назад
            _new_padding = info_x.dwCursorPosition.X - 1;  //запоминаем новый падинг(оступ)
            positionCur.Y -= 1; // подымаем курсор на 1 
            _set_cur_to_pos(hConsole, positionCur);  //Установили курсор
            printf("┬"); // выводим разделитель между двумя 
            _get_con_info(&info_x); //получаем снова информацию о курсоре
            positionCur.Y += 2; positionCur.X = _padding; //смещаем курсор вниз на две ячейки, установив его на оступ 
            _set_cur_to_pos(hConsole, positionCur);
            if (_padding == _otstup) // печатаем разделитель
            {
                printf("├"); positionCur.X++;
            }
            else {
                positionCur.X++; //иначе увеличели X
                _set_cur_to_pos(hConsole, positionCur);
            }
            for (int j = positionCur.X; j < _new_padding; j++) { //отрисовка нижнего отделителя
                printf("─"); 
                            }
            if (i == _menu_size - 1) {
                printf("┘"); // если элемент последний , отрисовать уголок
            }
            else
                printf("┴"); //иначе отрисовка отделителя
            positionCur.Y -= 1; positionCur.X = _new_padding + 1;
            _set_cur_to_pos(hConsole, positionCur);
        }
        if (!table_focus_flag)  //если у нас флаг не активен
        print_help("\x1b[45mESC\x1b[0m:Выход \x1b[45mENTER\x1b[0m:Ввод \x1b[45mСТРЕЛКИ\x1b[0m: Переключение селектора меню \x1b[45mTAB\x1b[0m:Переключить фокус на таблицу "); //печатаем один хелп 
        else  print_help("\x1b[45mESC\x1b[0m:Выход \x1b[45mENTER\x1b[0m:Редактировать \x1b[45mСТРЕЛКИ\x1b[0m:Навигация  \x1b[45mTAB\x1b[0m:Фокус на меню \x1b[45mDEL\x1b[0m: Удалить запись \x1b[45mHOME|END|PgUp|PgDown\x1b[0m:Сортировка");// иначе другой
        _table_window(table,_output_mas,&_output_colcount,&page,&table_focus_flag,root,sort);// вызываем печать окна таблицы     
        char c = getch(); // получаем селектор меню
        if (c == KEY_TAB) {   // если таб то переход на таблицу с проверкой
            if (_output_mas && _output_colcount != 0)
                table_focus_flag = 1; else {
                print_help("\x1b[41mДанных для вывода нет. Невозможно переключиться на таблицу.\x1b[0m");
                Sleep(1000);
            }
        } else 
        if (c == KEY_ENTER) { // если ввели enter 
            if (_menu[position[0] - 1]._menu_size > 0) { //если в текущем пункте меню есть сабменю , то 
                position[1] = 1; 
                while (1) {//бесконечный цикл работы с сабменю 
                    positionCur.X = _menu[position[0] - 1]._menu_name_lenght + 2;//устанавливаем текущие позиции для курсора 
                    positionCur.Y = _interval + 2;
                    _set_cur_to_pos(hConsole, positionCur); 
                    for (int i = 0; i <= _menu[position[0] - 1]._menu_size; i++) { //цикл отрисовки каждого окошка меню
                        for (int j = 0; j <= _menu[position[0] - 1]._max_sub_lenght + 2; j++) { //цикл отрисовки бордера
                            if (j == 0) { 
                                if (i == 0) {//если позиция 0 , то у нас верхние ограничитель
                                    printf("┌"); 
                                }
                                else
                                    if (i == _menu[position[0] - 1]._menu_size) {
                                        printf("└"); //если конечная позиция , то нижний
                                    }
                                    else printf("├"); // иначе соеденитель
                            }
                            else
                                if (j == _menu[position[0] - 1]._max_sub_lenght + 2) { 
                                    if (i == 0) {
                                        printf("┐"); //печать верхнего ограничителя
                                    }
                                    else
                                        if (i == _menu[position[0] - 1]._menu_size) {
                                            printf("┘"); //печать нижнего ограничителя , если достигли конца
                                             _get_con_info(&info_x);
                                            positionCur.X = info_x.dwCursorPosition.X; 
                                            positionCur.Y = info_x.dwCursorPosition.Y;
                                            positionCur.Y--; positionCur.X--;
                                            _set_cur_to_pos(hConsole, positionCur); // ставим курсор на один назад и на один вверх
                                            printf("│"); //печать разделителя 
                                            positionCur.Y++;
                                            _set_cur_to_pos(hConsole, positionCur);//возврат на текущую позциию
                                        }
                                        else {
                                            printf("┤"); //печать соеденителя
                                             _get_con_info(&info_x);
                                            positionCur.X = info_x.dwCursorPosition.X;
                                            positionCur.Y = info_x.dwCursorPosition.Y;
                                            positionCur.Y--; positionCur.X--;
                                            _set_cur_to_pos(hConsole, positionCur);// ставим курсор на один назад и на один вверх
                                            printf("│");// печать разделителя
                                            positionCur.Y++; 
                                            _set_cur_to_pos(hConsole, positionCur);//возврат на текущую позциию 
                                        }
                                }
                                else (printf("─")); //печать промежуточного разделителя
                        }
                        positionCur.Y++; positionCur.X = _menu[position[0] - 1]._menu_name_lenght + 2; //стартовые параметры для печати данных
                        _set_cur_to_pos(hConsole, positionCur); // установим курсор на данную позицию
                        if (i == _menu[position[0] - 1]._menu_size) break; //если текущий номер совпал с размером , то ничего не рисуем дальше
                        int _margin = _menu[position[0] - 1]._max_sub_lenght + 2 - _menu[position[0] - 1]._sub_menu_lenght[i]; //высчитываем отступ
                        _margin = _margin / 2; 
                        printf("│");//печатаем разделитель
                        for (int l = 0; l < _margin; l++) { printf(" "); } //делаем отступ слева
                        if (position[1] - 1 == i) {
                            printf("\x1b[43m%s\x1b[0m", _menu[position[0] - 1]._sub_menu[i]); // если текущий выбор сабменю совпал , то выделяем его
                        }
                        else { printf("%s ", _menu[position[0] - 1]._sub_menu[i]); } // иначе печатаем обычно
                        for (int l = 0; l < _margin; l++) { printf(" "); } // печать отступа справа 
                        positionCur.Y++;
                        _set_cur_to_pos(hConsole, positionCur); // увеличиваем y , и ставим курсор на след позицию
                    }
                    c = getch(); //получаем нажатую кнопку 
                    
                    if (c == KEY_ENTER) { // если у нас нажат enter 
                        int result = 0; //возвращаемое значение
                        for (int l = 0; l < position[0] - 1; l++) {
                            result += _menu[l]._menu_size; //до текущей позиции высчитываем результат выбора 
                        }
                        result += position[1]; // добавляем текущее смещение 
                                     return result; // вернем результат
                                }
                    if (c == KEY_ESC) { clear(); break; } // если escape , то очистим экран меню , выйдем с цикла 
                    position = _get_curent_selection(c, position, _menu[position[0] - 1]._menu_size, _menu_buttons, 1); //получение новой позиции курсора согласно c
                }
            }
            else return PROGRAM_EXIT;   //Вернте выход из программы если в текущем меню 0 сабменю        
            }
        else { //внутренний цикл работы будет получать новую позицию курсора 
            position = _get_curent_selection(c, position, 1, _menu_buttons, 0);    
        }         
    }

    return EXIT_SUCCESS; //вернет успешный выход
}
/// <summary>
/// Анимированная картинка в консоли
/// </summary>
void animatedNeko() {
    FILE* f; int count = 1;
    while (1) {
        for (int i = 0; i < 11; i++) {
            char url[256] = { 0 };
            sprintf(url, "bakemonogatari-monogatari/banner (%d).txt", i);
            f = fopen(url, "r"); char a[210]; 
            while (fgets(a,210,f) != NULL)
            {
                printf("         %s", a);
            }
            Sleep(30);
            COORD positionCur = { 0,0 }; //позиция x и y
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            _set_cur_to_pos(hConsole, positionCur);
        }
        count++; 
        if (count >= 20) return;
    }
}

void _print_bakground(int _window_w,int _window_h)
{
    clearf();
    char c =  rand() % (47 - 33 + 1) + 33;
    srand(time(NULL));
    for (int i = 0; i < _window_h; i++)
    {
        for (int j = 0; j < _window_w; j++) {
            printf("\x1b[44m%c\x1b[0m",c);
            c= rand() % (47 - 33 + 1) + 33;
        }
        printf("\n");
    }
    _print_border(_window_w, _window_h);
}

void _print_border(int _window_w, int _window_h) {
    COORD positionCur = { _otstup,_interval }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = _interval; y <= _window_h - _interval; y++) {
        _set_cur_to_pos(hConsole, positionCur);
        if(y == _interval)  printf("┌");
        else if (y == _window_h - _interval)
            printf("└");
        else 
        printf("│");
        for (int x = _otstup+1; x < _window_w - _otstup; x++) {
            if ((y == _interval) || (y == _window_h - _interval)) printf("─"); else  printf(" ");
        } 
        if (y == _interval)  printf("┐");
        else if (y == _window_h - _interval)
            printf("┘");
        else
            printf("│");
        positionCur.Y++;
    }
}



int* _get_window_size(int* size) {
    HANDLE hWndConsole;
    size = (int*)calloc(2, sizeof(int));
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



void _window(int _window_w, int _window_h, char* title) {
    int height = _window_h / 4; int width = _window_w / 4;
    COORD positionCur = { _otstup,_interval }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int _center_x, _center_y;
    _center_x = _window_w / 2;
    _center_y = _window_h / 2;
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y - height / 2;
    for (int y = 0; y < height; y++) {
        _set_cur_to_pos(hConsole, positionCur);
        for (int x = 0; x < width; x++) {
            if (y == 0) {
                if (x == 0) {
                    printf("┌");
                }
                else if (x == width - 1) { printf("┐"); }
                else   printf("─");

            }
            else
                if (y == height - 1)
                {
                    if (x == 0) {
                        printf("└");
                    }
                    else if (x == width - 1) {
                        printf("┘");
                    }
                    else printf("─");
                }
                else {
                    if (x == 0) {
                        printf("│");
                    }
                    else if (x == width - 1) { printf("│"); }
                    else printf(" ");
                }
        }
        positionCur.Y++;
    }
    positionCur.X = _center_x - width / 2 + 1;
    positionCur.Y = _center_y - height / 2 + 1;
    _set_cur_to_pos(hConsole, positionCur);
    int _temp_ots=0;
    if (title) _temp_ots = (width - 2 - u8_strlen(title)) / 2;
    for (int j = 0; j < _temp_ots; j++) {
        printf(" ");
    }
    if (title)  printf("%s", title);
    for (int j = 0; j < _temp_ots; j++) {
        printf(" ");
    }
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y - height / 2 + 2;
    _set_cur_to_pos(hConsole, positionCur);
    for (int j = 0; j < width; j++) {
        if (j == 0) printf("├");
        else if (j == width - 1) {
            printf("┤");
        }
        else printf("─");
    }
}

int _confirm_window(char * message)
{
    SetConsoleOutputCP(65001);
    if (!message)  message = "Выполнить операцию ?";
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int _window_h = _size_n[1];
    _window(_window_w, _window_h, "Подтверждение");
    CONSOLE_SCREEN_BUFFER_INFO info_x;
    int height = _window_h / 4; int width = _window_w / 4;
    COORD positionCur = { _otstup,_interval }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int _center_x = _window_w / 2;
    int _center_y = _window_h / 2;
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y - height / 2 + height / 4+2;
    int m_lenght = u8_strlen(message);
    int margin = 0;
    /*margin = width - 21;
    margin = margin / 2;
    positionCur.X += margin;
    _set_cur_to_pos(hConsole, positionCur);
    printf("%s");*/

    if (m_lenght < width - 2) {
        int margin = width - m_lenght;
        margin = margin / 2;
        positionCur.X += margin;
        _set_cur_to_pos(hConsole, positionCur);
        printf("%s", message);
    }
    else {
        positionCur.X++;
        _set_cur_to_pos(hConsole, positionCur);
        for (int i = 0; i < (width - 1) * 2; i++) {
            printf("%c", message[i]);
            _get_con_info(&info_x);
            if (info_x.dwCursorPosition.X - positionCur.X >= width - 5) {
                printf("..."); break;
            }
        }
    }
    int _selection = 1;
    positionCur.Y = _center_y - height / 2 + height / 4 + height / 3+2;
    while (1) {
        positionCur.X = _center_x - width / 2 + 3;
        _set_cur_to_pos(hConsole, positionCur);
        if (_selection) {
            printf(" \x1b[43mДА\x1b[0m ");
            positionCur.X = _center_x + width / 2-8;
            _set_cur_to_pos(hConsole, positionCur);
            printf(" НЕТ ");
            
        }
        else {
            printf(" ДА ");
            positionCur.X = _center_x + width / 2 - 8;
            _set_cur_to_pos(hConsole, positionCur);
            printf(" \x1b[43mНЕТ\x1b[0m ");
            
        }
        char c = getch();
        switch (c) {
        case 75://лево
            if (_selection != 1) _selection++;
            break;
        case 77://право
            if (_selection == 1) _selection--;
            break;
        case KEY_ENTER:
            return _selection;
            break;
        case KEY_ESC:
            return 0;
            break;
        }
    }

    return EXIT_SUCCESS;
}

void _in_window() {
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int _window_h = _size_n[1];
    _window(_window_w, _window_h,"Окно ввода");
    int height = _window_h / 4; int width = _window_w / 4;
    COORD positionCur = { _otstup,_interval }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int _center_x = _window_w / 2;
    int _center_y = _window_h / 2;
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y;
    int margin = width - 21;
    margin = margin / 2;
    positionCur.X += margin;
    _set_cur_to_pos(hConsole, positionCur);
    printf("Введите данные -> ");
}

void _message_window(char* message) {
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int _window_h = _size_n[1];
    _window(_window_w, _window_h,"Сообщение");
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int m_lenght = u8_strlen(message);
    int height = _window_h / 4; int width = _window_w / 4;
    COORD positionCur = { _otstup,_interval }; //позиция x и y
   
    int _center_x = _window_w / 2;
    int _center_y = _window_h / 2;
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y;
    if (m_lenght < width - 2) {
        int margin = width - m_lenght;
        margin = margin / 2;
        positionCur.X += margin;
        _set_cur_to_pos(hConsole, positionCur);
        printf("%s", message);
    }
    else {
        positionCur.X++;
        _set_cur_to_pos(hConsole, positionCur);
        for (int i = 0; i < (width - 1)*2; i++) {
            printf("%c",message[i]);
             _get_con_info(&info_x);
            if (info_x.dwCursorPosition.X - positionCur.X >= width - 5) {
                printf("..."); break;
            }
        }
      }
        

}

int _table_window(_tabel_metadata * table, abonent_t * _output_mass, int * _info_count, int*  page, int * _table_focus_flag, abonent** root, sort_struct* sort) {
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
    static int row_selection[] = {1,1};
    int* _size_n = NULL; 
    _size_n = _get_window_size(_size_n);
     int _window_w = _size_n[0]; int _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0;
    int _size_delta = 0; static int last_sel = 0;
    for (int i = 0; i < table->_col_count; i++) {
        _size_delta += table->_cols[i].size;
    }
    int height = _window_h - _interval * 2 - 10 ; int width = _window_w - _otstup * 2 - 4;
    _size_delta = (width -2) - _size_delta - table->_col_count*2-5 ;
    do {
    COORD positionCur = { _otstup+2,_interval+3}; //позиция x и y 
    _set_cur_to_pos(hConsole, positionCur);
    for (int i = 0; i < width; i++) {
        if (i == 0) {
            printf("┌");
        }
        if (i == width - 1) {
            printf("┐");
        } else 
        printf("─");
    }
    positionCur.Y++; //positionCur.X++;
    _set_cur_to_pos(hConsole, positionCur);
    printf("│");
    for (int i = 0; i < table->_col_count; i++) {
         _get_con_info(&info_x);
        _padding = info_x.dwCursorPosition.X - 1;
        if (i == sort->sort_f && i!=0)
           if (sort->sort_t == UP)
              printf(" \x1b[44m%s\x1b[0m ", table->_cols[i].name);
           else printf(" \x1b[43m%s\x1b[0m ", table->_cols[i].name);
        else printf(" %s ", table->_cols[i].name);
        if (table->_cols[i].resizebl) {
            if (!_mn_size_flag) {
                int _tmp_padding = _size_delta / 3;
                _size_delta -= _tmp_padding;
                _mn_size_flag = 1;
                if ((u8_strlen(table->_cols[i].name) + _tmp_padding) != table->_cols[i].size) {
                    table->_cols[i].size += _tmp_padding;
                }
            }
            else {
                if ((u8_strlen(table->_cols[i].name) + _size_delta) != table->_cols[i].size) {
                    table->_cols[i].size += _size_delta;
                }
            }
        }

        if (u8_strlen(table->_cols[i].name) != table->_cols[i].size) {
            int _sim_padding = table->_cols[i].size - u8_strlen(table->_cols[i].name);
            for (int i = 0; i < _sim_padding; i++) {
                printf(" ");
            }
        }

        printf("│");
         _get_con_info(&info_x);
        positionCur.X = info_x.dwCursorPosition.X - 1;
        _new_padding = info_x.dwCursorPosition.X - 1;
        positionCur.Y -= 1;
        _set_cur_to_pos(hConsole, positionCur);
        if (i == table->_col_count - 1) {
            printf("┐");
        }
        else  
            printf("┬");
         _get_con_info(&info_x);
        positionCur.Y += 2; positionCur.X = _padding;
        _set_cur_to_pos(hConsole, positionCur);
        if (_padding == _otstup+2)
        {
            printf("├"); positionCur.X++;
        }
        else {
            positionCur.X++;
            _set_cur_to_pos(hConsole, positionCur);
        }
        for (int j = positionCur.X; j < _new_padding; j++) {
            printf("─");
        }
        if (i == table->_col_count - 1) {
            printf("┤");
        }
        else
            printf("┼");
        positionCur.Y -= 1; positionCur.X = _new_padding + 1;
        _set_cur_to_pos(hConsole, positionCur);
    }
    positionCur.Y += 2; 
    positionCur.X = _otstup + 2;
    _set_cur_to_pos(hConsole, positionCur);
   
        if (_output_mass && *_info_count!=0) {

            int _col_inpage = height;
            int _diap[2] = { 0,0 };
            _diap[0] = ((*page) - 1) * _col_inpage;
            _diap[1] = _diap[0] + _col_inpage;
          //  if (row_selection[1]== last_sel)
            for (int j = _diap[0]; j < _diap[1]; j++)
            {
                if (*_info_count <= j) break;
                _set_cur_to_pos(hConsole, positionCur);
                if (*_table_focus_flag)
                    if (j == ((*page) - 1) * _col_inpage + row_selection[1]-1)
                        printf("\x1b[42m");
                printf("│");
                char buff[400] = { "" };
                sprintf(buff, "%d", j+1); 
                SetConsoleOutputCP(1251); //-------
                printf("%s", buff);
                if (u8_strlen(buff) < 3)
                {
                    for (int l = 0; l < 3 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");

                sprintf(buff, "%s %s %s", _output_mass[j].fio.surname, _output_mass[j].fio.name, _output_mass[j].fio.secondname);
                if (u8_strlen(buff) > table->_cols[1].size + 2) {
                    sprintf(buff, "%s %c.%c", _output_mass[j].fio.surname, _output_mass[j].fio.name[0], _output_mass[j].fio.secondname[0]);
                }
                SetConsoleOutputCP(1251); //-------
                printf("%s", buff);
                if (u8_strlen(buff) < table->_cols[1].size + 2)
                {
                    for (int l = 0; l < table->_cols[1].size + 2 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");
                SetConsoleOutputCP(1251); //-------
                sprintf(buff, "%s %s", _output_mass[j].autor.surname, _output_mass[j].autor.inicial);
                if (u8_strlen(buff) > table->_cols[2].size + 2) {
                    for (int l = 0; l < table->_cols[2].size - 1; l++) {
                        printf("%c", buff[l]);
                    }
                    printf("...");
                }
                else
                    printf("%s", buff);
                if (u8_strlen(buff) < table->_cols[2].size + 2)
                {
                    for (int l = 0; l < table->_cols[2].size + 2 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");
                SetConsoleOutputCP(1251); //-------
                sprintf(buff, "%s", _output_mass[j].book_name);
                if (u8_strlen(buff) > table->_cols[3].size + 2) {
                    for (int l = 0; l < table->_cols[3].size - 1; l++) {
                        printf("%c", _output_mass[j].book_name[l]);
                    }
                    printf("...");
                }
                else
                    printf("%s", buff);
                if (u8_strlen(buff) < table->_cols[3].size + 2)
                {
                    for (int l = 0; l < table->_cols[3].size + 2 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");
                SetConsoleOutputCP(1251); //-------
                sprintf(buff, "%s", _output_mass[j].izd);
                if (u8_strlen(buff) > table->_cols[4].size + 2) {
                    for (int l = 0; l < table->_cols[4].size - 1; l++) {
                        printf("%c", _output_mass[j].izd[l]);
                    }
                    printf("...");
                }
                else
                    printf("%s", buff);
                if (u8_strlen(buff) < table->_cols[4].size + 2)
                {
                    for (int l = 0; l < table->_cols[4].size + 2 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");
                SetConsoleOutputCP(1251); //-------
                sprintf(buff, "%d.%d.%d", _output_mass[j].date_out.d, _output_mass[j].date_out.m, _output_mass[j].date_out.y);
                if (u8_strlen(buff) > table->_cols[5].size + 2) {
                    for (int l = 0; l < table->_cols[5].size - 1; l++) {
                        printf("%c", buff[l]);
                    }
                    printf("...");
                }
                else
                printf("%s", buff);
                if (u8_strlen(buff) < table->_cols[5].size + 2)
                {
                    for (int l = 0; l < table->_cols[5].size + 2 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");
                SetConsoleOutputCP(1251); //-------
                sprintf(buff, "%.2f", _output_mass[j].cost);
                if (u8_strlen(buff) > table->_cols[6].size + 2) {
                    for (int l = 0; l < table->_cols[6].size - 1; l++) {
                        printf("%c", buff[l]);
                    }
                    printf("...");
                }
                else
                    printf("%s", buff);
                if (u8_strlen(buff) < table->_cols[6].size + 2)
                {
                    for (int l = 0; l < table->_cols[6].size + 2 - u8_strlen(buff); l++)
                        printf(" ");
                }
                SetConsoleOutputCP(65001); //-------
                printf("│");
                if (j == ((*page) - 1) * _col_inpage + row_selection[1] - 1)
                    printf("\x1b[0m");
                positionCur.Y++;
            }
        }
        else {
        printf("│");
        for (int i = 0; i < (width - 12) / 2; i++)
            printf("-");
        SetConsoleOutputCP(65001); //-------
        printf(" Данных нет ");
        SetConsoleOutputCP(65001); //-------
        for (int i = 0; i < (width - 12) / 2 - 1; i++)
            printf("-");
        if ((width - 12) % 2 == 1) printf("-");
        printf("│");
        positionCur.Y++;
        *_table_focus_flag = 0;
        }

        _set_cur_to_pos(hConsole, positionCur);
        int _row_num = 0; int _padd_border = table->_cols[_row_num].size + 2;
        for (int i = 0; i < width; i++) {
            if (i == 0) {
                printf("└");
            }
            if (i == width - 1) {
                printf("┘");
            }
            else
                if (i == _padd_border) {
                    _row_num++;
                    if (_row_num < table->_col_count)
                        _padd_border += table->_cols[_row_num].size + 3;
                    printf("┴");
                }
                else
                    printf("─");
        }
        COORD temp_cord = positionCur;
        temp_cord.Y++;
        _set_cur_to_pos(hConsole,temp_cord);
        char message[200] = "";
        //if(( * _info_count) != 0)
        sprintf(message, "Страница %d из %d",*page,(*_info_count/height)+1);
        printf(message);


        if (*_table_focus_flag)
        {
            char c = getch();

            if (c == KEY_ENTER) {
                _output_mass[((*page) - 1) * height + row_selection[1] - 1] = *_in_info_window(table, &_output_mass[((*page) - 1) * height + row_selection[1] - 1], 0);
                tree_deleteNodeById(root, _output_mass[((*page) - 1) * height + row_selection[1] - 1].id);
                _output_mass[((*page) - 1) * height + row_selection[1] - 1].id = util_hashCodeFromFio(&_output_mass[((*page) - 1) * height + row_selection[1] - 1].fio);
                tree_add(root, &_output_mass[((*page) - 1) * height + row_selection[1] - 1]);
                clear_table();
            }


            if (c == KEY_ESC || c == KEY_TAB)
            {
                *_table_focus_flag = 0;
                break;
            }

            if (c == KEY_DEL) {
                if (_confirm_window("Вы действительно хотите удалить запись ?")) {
                 // root =  tree_deleteNodeById(root, _output_mass[((*page) - 1) * height + row_selection[1] - 1].id);
                    tree_deleteNodeById(root, _output_mass[((*page) - 1) * height + row_selection[1] - 1].id);
                    _message_window("Запись успешно удалена");
                    int leafcount = tree_getNodeCount(*root, 0);
                    int  temp = 0;
                    _output_mass = _get_output_info(*root, _output_mass, &temp);
                    if (leafcount == 0)
                        _output_mass = NULL;
                    *_info_count = leafcount;
                    if (*_info_count < ((*page) * height)) {
                        clear_table();
                        if ((((*page) - 1) * height + row_selection[1] - 1) > *_info_count) {
                            row_selection[1] = *_info_count - ((*page) - 1) * height;
                        }
                        if (((*_info_count) * (*page)) % height == 0){
                            if (*page > 1) { (*page)--; clear_table(); }
}
                    
                 }
                }
              }

            if (c == KEY_HOME)
            {
                if (sort->sort_f > 0) {
                    sort->sort_f--;
                    _output_mass = _sort_output(_output_mass, _info_count, sort);
                }
            }
            if (c == KEY_END)
            {
                if (sort->sort_f < ZADANIE-1) {
                    sort->sort_f++;
                    _output_mass = _sort_output(_output_mass, _info_count, sort);
                }
            }
            if (c == KEY_PGUP)
            {
                sort->sort_t = UP;
                    _output_mass = _sort_output(_output_mass, _info_count, sort);
            }
            if (c == KEY_PGDOWN)
            {
                 sort->sort_t = DOWN;
                    _output_mass = _sort_output(_output_mass, _info_count, sort);
                
            }
            if (c == KEY_ARROW_LEFT)
            {
                if (*page > 1) { (*page)--; clear_table(); }

            }
            if (c == KEY_ARROW_RIGHT)
            {
                if (*_info_count > ((*page) * height)) {
                    (*page)++;
                    clear_table();
                    if ((((*page) - 1) * height + row_selection[1] - 1) > *_info_count) {
                        row_selection[1] = *_info_count - ((*page) - 1) * height;
                    }
                }
            }
            int* temp = (int*)calloc(2, sizeof(int));
            temp = _get_curent_selection(c, row_selection, height, 1, 0);           
            row_selection[1] = temp[1];
            if ((((*page) - 1) * height + row_selection[1] - 1) == *_info_count) {
                row_selection[1] = row_selection[1] - 1;
            }
        }
    }while (*_table_focus_flag);
     return EXIT_SUCCESS;
}


void _big_window(char* title) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int  _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0; char buff[200];
    int height = _window_h / 2;  
    int width = _window_w / 2;
    int _center_x = _window_w / 2;
    int _center_y = _window_h / 2;
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y - height / 2;
    for (int y = 0; y < height; y++) {
        _set_cur_to_pos(hConsole, positionCur);
        for (int x = 0; x < width; x++) {
            if (y == 0) {
                if (x == 0) {
                    printf("┌");
                }
                else if (x == width - 1) { printf("┐"); }
                else   printf("─");

            }
            else
                if (y == height - 1)
                {
                    if (x == 0) {
                        printf("└");
                    }
                    else if (x == width - 1) {
                        printf("┘");
                    }
                    else printf("─");
                }
                else {
                    if (x == 0) {
                        printf("│");
                    }
                    else if (x == width - 1) { printf("│"); }
                    else printf(" ");
                }
        }
        positionCur.Y++;
    }

    positionCur.X = _center_x - width / 2 + 1;
    positionCur.Y = _center_y - height / 2 + 1;
    _set_cur_to_pos(hConsole, positionCur);
     int _temp_ots = 0;
     if (title) _temp_ots = (width - 2 - u8_strlen(title)) / 2;
    for (int j = 0; j < _temp_ots; j++) {
        printf(" ");
    }
    if(title)  printf("%s", title);
    for (int j = 0; j < _temp_ots; j++) {
        printf(" ");
    }
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y - height / 2 + 2;
    _set_cur_to_pos(hConsole, positionCur);
    for (int j = 0; j < width; j++) {
        if (j == 0) printf("├");
        else if (j == width - 1) {
            printf("┤");
        }
        else printf("─");
    }
}



abonent_t* _in_info_window(_tabel_metadata* table, abonent_t *_output_info,int _cycle_in_flag) {
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* _size_n = NULL; abonent_t *  _temp_info;
    if (_output_info ) {
        _temp_info = (abonent_t*)calloc(1, sizeof(abonent_t));
        *_temp_info = *_output_info;
    } 
    else _temp_info = (abonent_t*)calloc(1, sizeof(abonent_t));
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int  _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0; char buff[200];
    int height = _window_h / 2; int width = _window_w / 2;
    int _center_x = _window_w / 2; int flag_clear = 0;
    int _center_y = _window_h / 2; char* title = NULL;
start:
    if (_cycle_in_flag) title = "Окно ввода информации"; else title = "Окно редактирования информации";
    _big_window(title);
    int max_lenght = 0; int y_modifire = 1;
    if (height >= 20) y_modifire = 2;
    for (int i = 1; i < table->_col_count; i++) {
        if (u8_strlen(table->_cols[i].name) > max_lenght)
            max_lenght = u8_strlen(table->_cols[i].name);
    }
    
    char c;
    int* _men_position = (int*)calloc(2, sizeof(int));
    _men_position[0] = 1; _men_position[1] = 1;
    while (1) {
        if (flag_clear) { _big_window(title); flag_clear = 0; }
        positionCur.X = _center_x - width / 2 + 4;
        positionCur.Y = _center_y - height / 2 + 4;
        _set_cur_to_pos(hConsole, positionCur);
        for (int i = 1; i < table->_col_count; i++) {
            if (_men_position[1] == i) {
                // printf("\x1b[43m%s\x1b[0m", table->_cols[i].name); 
                printf("%s", table->_cols[i].name);
            }
            else {
                printf("%s", table->_cols[i].name);
            }
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
        }
        positionCur.X = _center_x - width / 2 + 4 + max_lenght + 1;
        positionCur.Y = _center_y - height / 2 + 4;
        _set_cur_to_pos(hConsole, positionCur);
        for (int i = 1; i < table->_col_count; i++) {
            if (_men_position[1] == i) {
               if(!_cycle_in_flag) printf("\x1b[43m --> \x1b[0m"); else printf(" --> ");
            }
            else {
                printf(" --> ");
            }
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
        }
        _get_con_info(&info_x);
        int padding = info_x.dwCursorPosition.X; // получаем текущую позицию курсора, сместив его на один символ назад
        padding -= _center_x;
        positionCur.X = _center_x - width / 2 + 4 + max_lenght + 6;
        positionCur.Y = _center_y - height / 2 + 4;
        _set_cur_to_pos(hConsole, positionCur);
        for (int i = 1; i < table->_col_count; i++) {
            for (int j = 0; j < (width / 2 - padding - 8); j++) {
                printf("_");
            }
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
        }
        positionCur.X = _center_x - width / 2 + 4 + max_lenght + 6;
        positionCur.Y = _center_y - height / 2 + 4;
        _set_cur_to_pos(hConsole, positionCur);
        if (!_cycle_in_flag) {
            COORD fild_cords[6];
            SetConsoleOutputCP(1251); //-------
            if (u8_strlen(_temp_info->fio.surname) > 0)
                printf(" %s ", _temp_info->fio.surname);
            if (u8_strlen(_temp_info->fio.name) > 0)
                printf(" %s ", _temp_info->fio.name);
            if (u8_strlen(_temp_info->fio.secondname) > 0)
                printf(" %s ", _temp_info->fio.secondname);
            fild_cords[0] = positionCur;
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
            if (u8_strlen(_temp_info->autor.surname) > 0)
                printf(" %s ", _temp_info->autor.surname);
            if (u8_strlen(_temp_info->autor.inicial) > 0)
                printf(" %s ", _temp_info->autor.inicial);
            fild_cords[1] = positionCur;
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
            if (u8_strlen(_temp_info->book_name) > 0) {
                printf(" %s ", _temp_info->book_name);
            }
            fild_cords[2] = positionCur;
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
            if (u8_strlen(_temp_info->izd) > 0) {
                printf(" %s ", _temp_info->izd);
            }
            fild_cords[3] = positionCur;
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
            if (_temp_info->date_out.d > 0) {
                printf(" %d.", _temp_info->date_out.d);
                printf("%d.", _temp_info->date_out.m);
                printf("%d ", _temp_info->date_out.y);
            }
            fild_cords[4] = positionCur;
            positionCur.Y += y_modifire;
            _set_cur_to_pos(hConsole, positionCur);
            if (_temp_info->cost > 0) {
                printf(" %f ", _temp_info->cost);
            }
            fild_cords[5] = positionCur;
            positionCur.X = _center_x - width / 2 + 4;
            positionCur.Y = _center_y + height / 2 - 2;
            SetConsoleOutputCP(65001); //-------
            _set_cur_to_pos(hConsole, positionCur);
            if (_men_position[1] == table->_col_count) {
                printf("\x1b[43mСохранить\x1b[0m");
            }
            else printf("Сохранить");
            positionCur.X = _center_x + (width / 2 - u8_strlen("Отмена") - 3);
            _set_cur_to_pos(hConsole, positionCur);
            if (_men_position[1] == table->_col_count + 1) {
                printf("\x1b[43mОтмена\x1b[0m");
            }
            else printf("Отмена");
           
            c = getch();

            if (c == KEY_ENTER)
            {
                if (_men_position[1] == table->_col_count + 1)
                {
                    if (_confirm_window("Отменить операцию ?"))
                    {
                        return _output_info;
                    }
                    else
                    {
                        flag_clear = 1;
                    }
                }else if (_men_position[1] == table->_col_count)
                {
                    if (_confirm_window("Сохранить данные ?"))
                    {
                        return _temp_info;
                    }
                    else
                    {
                        flag_clear = 1;
                    }
                }
                else 
                {
                    switch (_men_position[1])
                    {
                    case 1: {
                        _set_cur_to_pos(hConsole, fild_cords[0]);
                        for (int i = 0; i < width / 2 - padding - 8; i++) { printf("_"); }
                        _set_cur_to_pos(hConsole, fild_cords[0]);
                        input_string(_temp_info->fio.surname, 40, PERSONAL);
                        input_string(_temp_info->fio.name, 40, PERSONAL);
                        input_string(_temp_info->fio.secondname, 40, PERSONAL);
                        SetConsoleOutputCP(65001); //-------
                        break;
                    }
                    case 2: {
                        _set_cur_to_pos(hConsole, fild_cords[1]);
                        for (int i = 0; i < width / 2 - padding - 8; i++) { printf("_"); }
                        _set_cur_to_pos(hConsole, fild_cords[1]);
                        input_string(_temp_info->autor.surname, 40, PERSONAL);
                        input_string(_temp_info->autor.inicial, 3, INICIAL);
                        SetConsoleOutputCP(65001); //-------
                        break;
                    }
                    case 3: {
                        _set_cur_to_pos(hConsole, fild_cords[2]);
                        for (int i = 0; i < width / 2 - padding - 8; i++) { printf("_"); }
                        _set_cur_to_pos(hConsole, fild_cords[2]);
                        input_string(_temp_info->book_name, width / 2 - padding - 8, NORMAL);
                        SetConsoleOutputCP(65001); //-------
                        break;
                    }
                    case 4: {
                        _set_cur_to_pos(hConsole, fild_cords[3]);
                        for (int i = 0; i < width / 2 - padding - 8; i++) { printf("_"); }
                        _set_cur_to_pos(hConsole, fild_cords[3]);
                        input_string(_temp_info->izd, width / 2 - padding - 8, NORMAL);
                        SetConsoleOutputCP(65001); //-------
                        break;
                    }
                    case 5: {
                        _set_cur_to_pos(hConsole, fild_cords[4]);
                        for (int i = 0; i < width / 2 - padding - 8; i++) { printf("_"); }
                        _set_cur_to_pos(hConsole, fild_cords[4]);
                        in_date(&_temp_info->date_out.d, &_temp_info->date_out.m, &_temp_info->date_out.y);
                        SetConsoleOutputCP(65001); //-------
                        break;
                    }
                    case 6: {
                        _set_cur_to_pos(hConsole, fild_cords[5]);
                        for (int i = 0; i < width / 2 - padding - 8; i++) { printf("_"); }
                        _set_cur_to_pos(hConsole, fild_cords[5]);
                        input_float(&_temp_info->cost);
                        //scanf("%f", &_temp_info->cost);
                        SetConsoleOutputCP(65001); //-------
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
            if (c == KEY_ESC)
            {
                return _output_info;
                break;
            }

            _men_position = _get_curent_selection(c, _men_position, table->_col_count + 1, 1, 1);
        }
        else {
          // 
            char mes[] = "\x1b[45mESC\x1b[0m:Выход  \x1b[45mENTER\x1b[0m:Ввод  \x1b[45mСТРЕЛКИ\x1b[0m: Переключение поля"; //51
            positionCur.Y = _center_y + height / 2 - 2;
            positionCur.X = _center_x - 50/2;
            _set_cur_to_pos(hConsole, positionCur);
            printf("%s", mes);    
            int cur_step = 1; int cur_key = 0; int step_compl = 0;
            CONSOLE_SCREEN_BUFFER_INFO con_inf; 
            CONSOLE_CURSOR_INFO structCursorInfo;
            GetConsoleCursorInfo(hConsole, &structCursorInfo);
            structCursorInfo.bVisible = TRUE;
            SetConsoleCursorInfo(hConsole, &structCursorInfo);
            positionCur.X = _center_x - width / 2 + 4 + max_lenght + 6;
            positionCur.Y = _center_y - height / 2 + 4;
            _set_cur_to_pos(hConsole, positionCur);
            COORD lastcord = {0,0};
            SetConsoleOutputCP(1251); //-------
            while (cur_step <= 9) {
                _get_con_info(&con_inf);
                lastcord = con_inf.dwCursorPosition;
                switch (cur_step)
                {
                case 1: {
                    cur_key = input_string(_temp_info->fio.surname, 40, PERSONAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++; 

                        break;
                    case KEY_ARROW_UP:
                        _set_cur_to_pos(hConsole, positionCur);
                        for (int j = 0; j < (width / 2 - padding - 8); j++) {
                            printf("_");
                        }
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++;
                            printf(" ");
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                  break;
                }
                case 2: {
                    cur_key = input_string(_temp_info->fio.name, 40, PERSONAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step=1;
                        }
                        _set_cur_to_pos(hConsole, positionCur);
                        for (int j = 0; j < (width / 2 - padding - 8); j++) {
                            printf("_");
                        }
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++;
                            printf(" ");
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case 3: {
                    cur_key = input_string(_temp_info->fio.secondname, 40, PERSONAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        positionCur.Y += y_modifire;
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step = 1;
                        }
                        _set_cur_to_pos(hConsole, positionCur);
                        for (int j = 0; j < (width / 2 - padding - 8); j++) {
                            printf("_");
                        }
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++;
                            printf(" ");
                            positionCur.Y += y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                   
                    break;
                }
                case 4: {
                    cur_key = input_string(_temp_info->autor.surname, 40, PERSONAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step=1; 
                            positionCur.Y -= y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                            for (int j = 0; j < (width / 2 - padding - 8); j++) {
                                printf("_");
                            }
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            printf(" ");
                            cur_step++;
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                   break;
                }
                case 5: {
                    cur_key = input_string(_temp_info->autor.inicial, 3, INICIAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        positionCur.Y += y_modifire;
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step=4;
                            _set_cur_to_pos(hConsole, positionCur);
                            for (int j = 0; j < (width / 2 - padding - 8); j++) {
                                printf("_");
                            }
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++; printf(" ");
                            positionCur.Y += y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case 6: {
                    cur_key = input_string(_temp_info->book_name, 100, NORMAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        _get_con_info(&con_inf);
                        positionCur.Y += y_modifire;
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step=4;
                            positionCur.Y -= y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                            for (int j = 0; j < (width / 2 - padding - 8); j++) {
                                printf("_");
                            }
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++;
                            positionCur.Y += y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case 7: {
                    cur_key = input_string(_temp_info->izd, 60, NORMAL);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        _get_con_info(&con_inf);
                        positionCur.Y += y_modifire;
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step--;
                            positionCur.Y -= y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++;
                            positionCur.Y += y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case 8: {
                    cur_key = in_date(&_temp_info->date_out.d, &_temp_info->date_out.m, &_temp_info->date_out.y);
                    switch (cur_key)
                    {
                    case KEY_ENTER:
                        cur_step++;
                        step_compl++;
                        _get_con_info(&con_inf);
                        positionCur.Y += y_modifire;
                        _set_cur_to_pos(hConsole, positionCur);
                        break;
                    case KEY_ARROW_UP:
                        if (cur_step > 1) {
                            cur_step--;
                            positionCur.Y -= y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        break;
                    case KEY_ARROW_DOWN:
                        if (cur_step < step_compl) {
                            cur_step++;
                            positionCur.Y += y_modifire;
                            _set_cur_to_pos(hConsole, positionCur);
                        }
                        else {
                            _set_cur_to_pos(hConsole, lastcord);
                        }
                        break;
                    case KEY_ESC:
                        if (_confirm_window("Вы действительно хотите отменить ввод?")) {
                            SetConsoleOutputCP(65001); //-------
                            return NULL;
                        }
                        else { SetConsoleOutputCP(65001); goto start; }
                        break;
                    default:
                        break;
                    }
                    break;
                }
                case 9: {
                    cur_step++;
                    step_compl++;
                    input_float(&_temp_info->cost);
                    break;
                }
                default:
                    break;
                }
            }
            SetConsoleOutputCP(65001);
            _message_window("Запись успешно добавлена");
            Sleep(2000);
            structCursorInfo.bVisible = FALSE;
            SetConsoleCursorInfo(hConsole, &structCursorInfo);
            return _temp_info;
        }
        }
    }



int print_help(char * help_message) {
    int* _size_now = NULL; //текущий размер окна 
    _size_now = _get_window_size(_size_now);
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    _get_con_info(&info_x);
    COORD positionCur = { _otstup,_size_now[1] - _interval - 2 };
    _set_cur_to_pos(hConsole, positionCur);
    printf("├");
    for (int i = 0; i < _size_now[0] - _otstup*2 - 1; i++) {
        printf("─");
    }
    printf("┤");
    positionCur.Y++; positionCur.X++;
    _set_cur_to_pos(hConsole, positionCur);
    for (int i = 0; i < _size_now[0] - _otstup * 2 - 1; i++)
        printf(" ");
    _set_cur_to_pos(hConsole, positionCur);
    printf("%s", help_message);
    return 0;
} 

abonent_t* _sort_output(abonent_t* _output_mass, int* filds_count, sort_struct* sorts)
{
    if (!sorts) return _output_mass;
    switch (sorts->sort_f)
    {
    case DEF:
        return _output_mass;
        break;
    case FIO:
                if (sorts->sort_t == UP){
                    for (int write = 0; write < *filds_count; write++) {
                        for (int sort = 0; sort < *filds_count - 1; sort++) {
                            if (_output_mass[sort].id > _output_mass[sort + 1].id) {
                                abonent_t temp = _output_mass[sort + 1];
                                _output_mass[sort + 1] = _output_mass[sort];
                                _output_mass[sort] = temp;
                            }
                        }
                    }
                  }
                 else {
                    for (int write = 0; write < *filds_count; write++) {
                        for (int sort = 0; sort < *filds_count - 1; sort++) {
                            if (_output_mass[sort].id < _output_mass[sort + 1].id) {
                                abonent_t temp = _output_mass[sort + 1];
                                _output_mass[sort + 1] = _output_mass[sort];
                                _output_mass[sort] = temp;
                            }
                        }
                    }
                }
        break;
    case AUTHOR:
        if (sorts->sort_t == UP) {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (strcmp(_output_mass[sort].autor.surname, _output_mass[sort + 1].autor.surname) == 1) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        else {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (strcmp(_output_mass[sort].autor.surname, _output_mass[sort + 1].autor.surname) == -1) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        break;
    case BOOK_NAME:
        if (sorts->sort_t == UP) {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (strcmp(_output_mass[sort].book_name, _output_mass[sort + 1].book_name) == 1) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        else {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (strcmp(_output_mass[sort].book_name, _output_mass[sort + 1].book_name) == -1) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        break;
    case DATE_OUT:
        if (sorts->sort_t == UP) {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (_output_mass[sort].date_out.y > _output_mass[sort + 1].date_out.y) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                    else if (_output_mass[sort].date_out.m > _output_mass[sort + 1].date_out.m && _output_mass[sort].date_out.y == _output_mass[sort + 1].date_out.y)
                    {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                    else if (_output_mass[sort].date_out.m == _output_mass[sort + 1].date_out.m && _output_mass[sort].date_out.y == _output_mass[sort + 1].date_out.y && _output_mass[sort].date_out.d > _output_mass[sort + 1].date_out.d) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        else {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (_output_mass[sort].date_out.y < _output_mass[sort + 1].date_out.y) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                    else if (_output_mass[sort].date_out.m < _output_mass[sort + 1].date_out.m && _output_mass[sort].date_out.y == _output_mass[sort + 1].date_out.y)
                    {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                    else if (_output_mass[sort].date_out.m == _output_mass[sort + 1].date_out.m && _output_mass[sort].date_out.y == _output_mass[sort + 1].date_out.y && _output_mass[sort].date_out.d < _output_mass[sort + 1].date_out.d) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        break;

    case IZD:
        if (sorts->sort_t == UP) {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (strcmp(_output_mass[sort].izd, _output_mass[sort + 1].izd) == 1) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        else {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (strcmp(_output_mass[sort].izd, _output_mass[sort + 1].izd) == -1) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        break;
    case COST:
        if (sorts->sort_t == UP) {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (_output_mass[sort].cost > _output_mass[sort + 1].cost) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        else {
            for (int write = 0; write < *filds_count; write++) {
                for (int sort = 0; sort < *filds_count - 1; sort++) {
                    if (_output_mass[sort].cost < _output_mass[sort + 1].cost) {
                        abonent_t temp = _output_mass[sort + 1];
                        _output_mass[sort + 1] = _output_mass[sort];
                        _output_mass[sort] = temp;
                    }
                }
            }
        }
        break;
    case ZADANIE:
        if (sorts->sort_t == DEF) return _output_mass;
        break;
    default:
        break;
    }
    return _output_mass;
}



void dolgiWindow(abonent ** root) {
    _big_window("Должники");
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int  _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0; char buff[200];
    int height = _window_h / 2; int width = _window_w / 2;
    int _center_x = _window_w / 2; int flag_clear = 0;
    int _center_y = _window_h / 2;
    int refresh_flag = 1;
    time_t  now = time(0);
    struct tm* ltm = localtime(&now);
    int d, y, m;
    y = ltm->tm_year + 1900;
    m = ltm->tm_mon+1;
    d = ltm->tm_mday;
    dolgi_pers_t_obr* dolgniki = (dolgi_pers_t_obr*)calloc(sizeof(dolgi_pers_t_obr), 1);
    dolgniki->count = 0;
    dolgniki = _get_dolgi_info(*root, dolgniki, d, m, y);
    COORD positionCur; 
    positionCur.X = _center_x - width / 2 + 4;
    positionCur.Y = _center_y - height / 2 + 4;
    int* _men_position = (int*)calloc(2, sizeof(int));
    _men_position[0] = 1; _men_position[1] = 1;
    _set_cur_to_pos(hConsole, positionCur);    
    _tabel_metadata * table = (_tabel_metadata*)calloc(sizeof(_tabel_metadata), 1);
    table->_col_count = 2;
    table->_cols = (_table_col*)calloc(sizeof(_table_col), table->_col_count);
    //-------------------------------------------------------------------------------------------//
    table->_cols[0].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[0].name, "ФИО абонента-должника");
    table->_cols[0].resizebl = 1;
    table->_cols[0].size = u8_strlen(table->_cols[0].name);
    //-------------------------------------------------------------------------------------------//
    table->_cols[1].name = (char*)calloc(sizeof(char), 60);
    strcpy(table->_cols[1].name, "Кол-во книг");
    table->_cols[1].resizebl = 0;
    table->_cols[1].size = u8_strlen(table->_cols[1].name);
    //-------------------------------------------------------------------------------------------//
    COORD PosCur = { _center_x - width / 2 + 4,_center_y - height / 2 + 3 }; //позиция x и y 
    int _size_delta = 0;
    for (int i = 0; i < table->_col_count; i++) {
        _size_delta += table->_cols[i].size;
    }
    _size_delta = (width - 2) - _size_delta - ((table->_col_count )* 2) - 10;
    _set_cur_to_pos(hConsole, PosCur);
    int size_w = 0;
    for (int i = 0; i < table->_col_count; i++) {
        size_w += table->_cols[i].size;
    }



    for (int i = 0; i < size_w+ (table->_col_count)+3 + _size_delta; i++) {
        if (i == 0) {
            printf("┌");
        }
        if (i == width - 1) {
            printf("┐");
        }
        else
            printf("─");
    }
    PosCur.Y++; //positionCur.X++;
    _set_cur_to_pos(hConsole, PosCur);
    printf("│");
    for (int i = 0; i < table->_col_count; i++) {
        _get_con_info(&info_x);
        _padding = info_x.dwCursorPosition.X - 1;
        printf(" %s ", table->_cols[i].name);
        if (table->_cols[i].resizebl) {
            {
                if ((u8_strlen(table->_cols[i].name) + _size_delta) != table->_cols[i].size) {
                    table->_cols[i].size += _size_delta;
                }
            }
        }
        if (u8_strlen(table->_cols[i].name) != table->_cols[i].size) {
            int _sim_padding = table->_cols[i].size - u8_strlen(table->_cols[i].name);
            for (int i = 0; i < _sim_padding; i++) {
                printf(" ");
            }
        }

        printf("│");
        _get_con_info(&info_x);
        PosCur.X = info_x.dwCursorPosition.X - 1;
        _new_padding = info_x.dwCursorPosition.X - 1;
        PosCur.Y -= 1;
        _set_cur_to_pos(hConsole, PosCur);
        if (i == table->_col_count - 1) {
            printf("┐");
        }
        else
            printf("┬");
        _get_con_info(&info_x);
        PosCur.Y += 2; PosCur.X = _padding;
        _set_cur_to_pos(hConsole, PosCur);
        if (_padding == _center_x - width / 2 + 4 )
        {
            printf("├"); PosCur.X++;
        }
        else {
            PosCur.X++;
            _set_cur_to_pos(hConsole, PosCur);
        }
        for (int j = PosCur.X; j < _new_padding; j++) {
            printf("─");
        }
        if (i == table->_col_count - 1) {
            printf("┤");
        }
        else
            printf("┼");
        PosCur.Y -= 1; PosCur.X = _new_padding + 1;
        _set_cur_to_pos(hConsole, PosCur);
    }
    PosCur.Y += 2;
    PosCur.X = _center_x - width / 2 + 4;
    int page = 1;
    int cor_X, cor_Y; cor_X = PosCur.X; cor_Y = PosCur.Y;
        SetConsoleOutputCP(65001); //-------
        while (1) {
            PosCur.X = cor_X; PosCur.Y = cor_Y;
            if (refresh_flag) {
                if ((dolgniki->count)) {
                    int _col_inpage = height - 10;
                    int _diap[2] = { 0,0 };
                    _diap[0] = ((page)-1) * _col_inpage;
                    _diap[1] = _diap[0] + _col_inpage;
                    for (int j = _diap[0]; j < _diap[1]; j++)
                    {
                        if (dolgniki->count <= j) {
                            break;
                        }
                        _set_cur_to_pos(hConsole, PosCur);
                        char buff[400] = { "" };
                        SetConsoleOutputCP(65001); //-------
                        printf("│");

                        sprintf(buff, "%s %s %s", dolgniki->info_mass[j].fio.surname, dolgniki->info_mass[j].fio.name, dolgniki->info_mass[j].fio.secondname);
                        if (u8_strlen(buff) > table->_cols[0].size + 2) {
                            sprintf(buff, "%s %c.%c", dolgniki->info_mass[j].fio.surname, dolgniki->info_mass[j].fio.name[0], dolgniki->info_mass[j].fio.secondname[0]);
                        }
                        SetConsoleOutputCP(1251); //-------
                        printf("%s", buff);
                        if (u8_strlen(buff) < table->_cols[0].size + 2)
                        {
                            for (int l = 0; l < table->_cols[0].size + 2 - u8_strlen(buff); l++)
                                printf(" ");
                        }
                        SetConsoleOutputCP(65001); //-------
                        printf("│");
                        SetConsoleOutputCP(1251); //-------
                        sprintf(buff, "%d", dolgniki->info_mass[j].count_dolg_books);
                        if (u8_strlen(buff) > table->_cols[1].size + 2) {
                            for (int l = 0; l < table->_cols[1].size - 1; l++) {
                                printf("%c", buff[l]);
                            }
                            printf("...");
                        }
                        else
                            printf("%s", buff);
                        if (u8_strlen(buff) < table->_cols[1].size + 2)
                        {
                            for (int l = 0; l < table->_cols[1].size + 2 - u8_strlen(buff); l++)
                                printf(" ");
                        }
                        SetConsoleOutputCP(65001); //-------
                        printf("│");
                        SetConsoleOutputCP(1251); //-------
                        PosCur.Y++;
                    }
                    refresh_flag = 0;
                }
                else
                {
                    _set_cur_to_pos(hConsole, PosCur);
                    printf("│");
                    for (int i = 0; i < (width - 21) / 2; i++)
                        printf("-");
                    SetConsoleOutputCP(65001); //-------
                    printf(" Данных нет ");
                    SetConsoleOutputCP(65001); //-------
                    for (int i = 0; i < (width - 21) / 2 - 1; i++)
                        printf("-");
                    if ((width - 6) % 2 == 1) printf("-");
                    printf("│");
                    PosCur.Y++;
                    refresh_flag = 0;
                }
                SetConsoleOutputCP(65001); //-------
                _set_cur_to_pos(hConsole, PosCur);
                int _row_num = 0; int _padd_border = table->_cols[_row_num].size + 2;
                for (int i = 0; i < width - 10; i++) {
                    if (i == 0) {
                        printf("└");
                    }
                    if (i == width - 11) {
                        printf("┘");
                    }
                    else
                        if (i == _padd_border) {
                            _row_num++;
                            if (_row_num < table->_col_count)
                                _padd_border += table->_cols[_row_num].size + 3;
                            printf("┴");
                        }
                        else
                            printf("─");
                }
                if (_center_y + height / 2 - 2 - PosCur.Y > 1) {
                    for (; _center_y + height / 2 - 2 - PosCur.Y > 1;) {
                        PosCur.Y++; _set_cur_to_pos(hConsole, PosCur);
                        for (int i = PosCur.X; i < _center_x + width / 2 - 4; i++) {
                            printf(" ");
                        }
                    }
               }
            }
            
            positionCur.X = _center_x - width / 2 + 4;
            positionCur.Y = _center_y + height / 2 - 2;
            _set_cur_to_pos(hConsole, positionCur);
            if (_men_position[0] == 1) {
                printf("\x1b[43mПред.страница\x1b[0m");
            }
            else printf("Пред.страница");
            positionCur.X = 3+  width / 2 + 4 + u8_strlen("Пред.страница");
            _set_cur_to_pos(hConsole, positionCur);
            if (_men_position[0] == 2) {
                printf("\x1b[43mСлед.страница\x1b[0m");
            }
            else printf("След.страница");
            positionCur.X = _center_x + (width / 2 - u8_strlen("Выход") - 3);
            _set_cur_to_pos(hConsole, positionCur);
            if (_men_position[0] == 3) {
                printf("\x1b[43mВыход\x1b[0m");
            }
            else printf("Выход");
            char c = getch();

            if (c == KEY_ENTER)
            {
                if (_men_position[0] == 1)
                {
                    if (page > 1) { page--; refresh_flag = 1; }
                }
                if (_men_position[0] == 2)
                {
                    if (dolgniki->count>((page)  * (height - 10))) {
                        page++;
                        refresh_flag = 1;
                    }
                }
                if (_men_position[0] == 3)
                {
                    return;
                }
            }
            if (c == KEY_ESC)
            
                return;
                _men_position = _get_curent_selection(c, _men_position, 2, 3, 0);
            
        }
}


