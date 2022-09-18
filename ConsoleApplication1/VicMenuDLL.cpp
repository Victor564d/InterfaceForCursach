#pragma warning(disable : 4996);
#include "VicMenuDLL.h"
#include "MenuStruct.h"
#include "utf8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>


const int _otstup = 3;
const int _interval = 3;
int* _window_size;
int _first_start = 1;
int x, y; 
COORD positionCur = {4,4};

#define clearf() system("cls");
#define _set_cursor_for_pos()

BOOL _get_con_info(CONSOLE_SCREEN_BUFFER_INFO* x)
{
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

BOOL _set_cur_to_pos(HANDLE h, COORD cor) {
    return SetConsoleCursorPosition(h, cor);
}


void clear() {
    positionCur.X = _otstup + 1;  positionCur.Y = _interval + 1;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* temp_window_size = NULL;
    temp_window_size = _get_window_size(temp_window_size);
    _set_cur_to_pos(hConsole, positionCur);
    _print_border(temp_window_size[0], temp_window_size[1]);
    positionCur.X = _otstup + 1;  positionCur.Y = _interval + 1;
        _set_cur_to_pos(hConsole, positionCur);
}

void clear_for_info() {
    positionCur.X = _otstup + 4;  positionCur.Y = _interval + 4;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    _set_cur_to_pos(hConsole, positionCur);
}

/// <summary>
/// Функция получения новой позиции селектора на основе старой
/// </summary>
/// <param name="c">Код нажатого символа</param>
/// <param name="position">Указатель на массив из двух элементво (x,y)</param>
/// <param name="MaxY">Максимальная высота(глубина) списка(Макс смещение по вертикали)</param>
/// <param name="Colums">Ширина(максимальное смещение указателя по горизонтали)</param>
/// <returns>Указатель на двумерный массив, с измененными параметрами</returns>
int* _get_curent_selection(char  c // Символ клавиатуры
    ,int * position // Массив в котором хранятся x и y
    ,int MaxY // Максимальный количество строк 
    , int Colums  // Количество столбцов, по умолчанию - 1
    ,int _flag_x_readonly
) 
{
    int x = position[0]; int y = position[1];
    switch (c)
    {
    case 72://вверх
        if (y > 1) y--;
        break;
    case 80://вниз
        if (y < MaxY) y++;
        break;
    case 75://лево
        if (x > 1) x--;
        break;
    case 77://право
        if (x < Colums) x++;
        break;
    }
    if (!_flag_x_readonly)
     position[0] = x; //запоминаем позицию по столбцу
    position[1] = y;//позиция по строке
    return position;
}
/// <summary>
/// Функция построения меню. Вызывается если нам не нужно вывести перед этим какую-то информацию.
/// </summary>
/// <param name="Menu">Указатель на массив из наименований пунктов меню</param>
/// <param name="position">Указатель на двумерный массив (x,y)</param>
/// <param name="MenuSize">Колличество элементов в массиве наименований</param>
/// <param name="Colums">Количество стобцов которое необходимо построить. Принимает значения 1,2,3</param>
/// <returns>Индекс выбранного пункта меню</returns>
int _print_menu(_menu_item * _menu, int* position, int _menu_size, int Colums,_tabel_metadata * table)
{
    return _print_menu_with_table (_menu, position, _menu_size, Colums, NULL, NULL, -1,table);
}

/// <summary>
/// Функция построения меню. Вызывается если нам  нужно вывести перед этим какую-то информацию.
/// </summary>
/// <param name="Menu">Указатель на массив из наименований пунктов меню</param>
/// <param name="position">Указатель на двумерный массив (x,y)</param>
/// <param name="MenuSize">Колличество элементов в массиве наименований</param>
/// <param name="Colums">Количество стобцов которое необходимо построить. Принимает значения 1,2,3</param>
/// <param name="funcptr">Указатель на функцию вывода данных экран</param>
/// <param name="Dataptr">Указатель на информационное поле</param>
/// <param name="num">Указатель на номер элемента при выводе</param>
/// <returns>Индекс выбранного пункта меню</returns>
int _print_menu_with_table(_menu_item* _menu //Массив объектов  меню
    , int* position //Массив текущей позиции x и y
    , int _menu_size  //Колличество элементов в массиве объектов меню
    , int _menu_buttons,//Количество кнопок меню  
    int (*funcptr) (void*, int), //резерв
    void* Dataptr, //резерв
    int num, //резерв
    _tabel_metadata * table
)
{
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
                _menu[i]._max_sub_lenght = _max_subm_lenght;
            }
        }
      }
    //printf("\x1b[43mHello\x1b[0m");
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
        //-----------------------------------------------------------------------------  
        //Пример управления цветом заднего фона и текста (задний фон красный)printf("\x1b[41mHello\x1b[0m");
                                                                          //printf("\x1b[43mHello\x1b[0m");
        _table_window(table,NULL,0,0);
                 
        if (funcptr != NULL && Dataptr != NULL) //Если у нас есть данные для вывода - выводим
            funcptr(Dataptr, num);

        char c = getch();
        if (c == 13) {
            if (_menu[position[0] - 1]._menu_size > 0) {
                position[1] = 1;
                while (1) {
                    positionCur.X = _menu[position[0] - 1]._menu_name_lenght + 2;
                    positionCur.Y = _interval + 2;
                    _set_cur_to_pos(hConsole, positionCur);
                    for (int i = 0; i <= _menu[position[0] - 1]._menu_size; i++) {
                        for (int j = 0; j <= _menu[position[0] - 1]._max_sub_lenght + 2; j++) {
                            if (j == 0) {
                                if (i == 0) {
                                    printf("┌");
                                }
                                else
                                    if (i == _menu[position[0] - 1]._menu_size) {
                                        printf("└");
                                    }
                                    else printf("├");
                            }
                            else
                                if (j == _menu[position[0] - 1]._max_sub_lenght + 2) {
                                    if (i == 0) {
                                        printf("┐");
                                    }
                                    else
                                        if (i == _menu[position[0] - 1]._menu_size) {
                                            printf("┘");
                                             _get_con_info(&info_x);
                                            positionCur.X = info_x.dwCursorPosition.X;
                                            positionCur.Y = info_x.dwCursorPosition.Y;
                                            positionCur.Y--; positionCur.X--;
                                            _set_cur_to_pos(hConsole, positionCur);
                                            printf("│");
                                            positionCur.Y++;
                                            _set_cur_to_pos(hConsole, positionCur);
                                        }
                                        else {
                                            printf("┤");
                                             _get_con_info(&info_x);
                                            positionCur.X = info_x.dwCursorPosition.X;
                                            positionCur.Y = info_x.dwCursorPosition.Y;
                                            positionCur.Y--; positionCur.X--;
                                            _set_cur_to_pos(hConsole, positionCur);
                                            printf("│");
                                            positionCur.Y++;
                                            _set_cur_to_pos(hConsole, positionCur);
                                        }
                                }
                                else (printf("─"));
                        }
                        positionCur.Y++; positionCur.X = _menu[position[0] - 1]._menu_name_lenght + 2;
                        _set_cur_to_pos(hConsole, positionCur);
                        if (i == _menu[position[0] - 1]._menu_size) break;
                        int _margin = _menu[position[0] - 1]._max_sub_lenght + 2 - _menu[position[0] - 1]._sub_menu_lenght[i];
                        _margin = _margin / 2;
                        printf("│");
                        for (int l = 0; l < _margin; l++) { printf(" "); }
                        if (position[1] - 1 == i) {
                            printf("\x1b[43m%s\x1b[0m", _menu[position[0] - 1]._sub_menu[i]);
                        }
                        else { printf("%s ", _menu[position[0] - 1]._sub_menu[i]); }
                        for (int l = 0; l < _margin; l++) { printf(" "); }
                        positionCur.Y++;
                        _set_cur_to_pos(hConsole, positionCur);
                    }
                    c = getch();
                    if (c == 13) {
                        int result = 0;
                        for (int l = 0; l < position[0] - 1; l++) {
                            result += _menu[l]._menu_size;
                        }
                        result += position[1];
                                     return result;
                                }
                    if (c == 27) { clear(); break; }
                    position = _get_curent_selection(c, position, _menu[position[0] - 1]._menu_size, _menu_buttons, 1);
                }
            }
            else return 16;          
            }
        else {
            position = _get_curent_selection(c, position, 1, _menu_buttons, 0);    
        }         
    }

    return EXIT_SUCCESS;
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
            f = fopen(url, "r"); char a[195]; ;
            while (fgets(a,195,f) != NULL)
            {
                printf("%s", a);
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

int _confirm_window() 
{
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int _window_h = _size_n[1];
    _window(_window_w, _window_h,"Подтверждение");
    int height = _window_h / 4; int width = _window_w / 4;
    COORD positionCur = { _otstup,_interval }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int _center_x = _window_w / 2;
    int _center_y = _window_h / 2;
    positionCur.X = _center_x - width / 2;
    positionCur.Y = _center_y - height / 2+ height/4;
    int margin = width - 21;
    margin = margin/2;
    positionCur.X += margin;
//-------------------------------------------- попытка сделать вывод рамки ----------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------- 
    _set_cur_to_pos(hConsole, positionCur);
    printf("Выполнить операцию ?");
    int _selection = 1;
    positionCur.Y = _center_y - height / 2 + height / 4 + height / 3;
    positionCur.X -= margin - 5;
    while (1) {
        _set_cur_to_pos(hConsole, positionCur);
        if (_selection) {           
            printf(" \x1b[43mДА\x1b[0m ");
            for (int i = positionCur.X; i < positionCur.X+width - 20; i++)
                printf(" ");
            printf(" НЕТ ");
        }
        else {
            printf(" ДА ");
            for (int i = positionCur.X; i < positionCur.X+width - 20; i++)
                printf(" ");
            printf(" \x1b[43mНЕТ\x1b[0m ");
        }
        char c = getch();
        switch(c) {
                case 75://лево
                    if (_selection != 1) _selection++;
                    break;
                case 77://право
                    if (_selection == 1) _selection--;
                    break;
                case 13://лево
                    return _selection;
                    break;
                case 27://право
                    return 0;
                    break;
                    }
    }

    return EXIT_SUCCESS;
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
    int _temp_ots;
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

int _table_window(_tabel_metadata * table, abonent_t * _output_mass, int _info_count, int * page) {
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
     int _window_w = _size_n[0]; int _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0; char buff[200];
    int _size_delta = 0;
    for (int i = 0; i < table->_col_count; i++) {
        _size_delta += table->_cols[i].size;
    }
    int height = _window_h - _interval * 2 - 15 ; int width = _window_w - _otstup * 2 - 4;
    _size_delta = (width -2) - _size_delta - table->_col_count*2-5 ;
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
         printf(" %s ", table->_cols[i].name);
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
    if (_output_mass) {
        
        
        int _col_inpage = height - positionCur.Y;
        int _diap[2] = { 0,0 };
        _diap[0] = ( * page) * _col_inpage;
        _diap[1] = _diap[0] + _col_inpage;
        printf("│");
        for (int j = _diap[0]; j < _diap[1]; j++)
        {
            if (_info_count < j) break;
            char buff[200];
            sprintf(buff, "%d", j);
        }



    }
    else {
        printf("│");
        for (int i = 0; i < (width-12)/2; i++)
            printf("-");
        printf(" Данных нет ");
        for (int i = 0; i < (width - 12) / 2 - 1; i++)
            printf("-");
        if ((width - 12) % 2 == 1) printf("-");
        printf("│");
        positionCur.Y++;
    }
    
       _set_cur_to_pos(hConsole, positionCur);
    for (int i = 0; i < width; i++) {
        if (i == 0) {
            printf("└");
        }
        if (i == width - 1) {
            printf("┘");
        }
        else
            printf("─");
    }
     return EXIT_SUCCESS;
}


void _big_window(char* title) {
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int  _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0; char buff[200];
    int height = _window_h / 2; int width = _window_w / 2;
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
     int _temp_ots;
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



abonent_t* _in_info_window(_tabel_metadata* table, abonent_t* _output_info) {
    CONSOLE_SCREEN_BUFFER_INFO info_x;  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int* _size_n = NULL;
    _size_n = _get_window_size(_size_n);
    int _window_w = _size_n[0]; int  _window_h = _size_n[1];
    int _padding, _new_padding; int _mn_size_flag = 0; int _size_temp = 0; char buff[200];
    int height = _window_h / 2; int width = _window_w / 2;
    int _center_x = _window_w / 2;
    int _center_y = _window_h / 2;
    _big_window("Окно ввода/редактирования записи");
    
    char c;
    int* _men_position = (int*)calloc(2, sizeof(int));
    _men_position[0] = 1; _men_position[1] = 1;
    while (1) {
        positionCur.X = _center_x - width / 2 + 4;
        positionCur.Y = _center_y - height / 2 + 4;
        _set_cur_to_pos(hConsole, positionCur);
        for (int i = 0; i < table->_col_count; i++) {
            if (_men_position[1] - 1 == i) {
                printf("\x1b[43m%s\x1b[0m", table->_cols[i].name); 
            }
            else {
                printf("%s ", table->_cols[i].name);
            }
            positionCur.Y++;
            _set_cur_to_pos(hConsole, positionCur);
        } 
        positionCur.Y = _center_y + height / 2 -2;
        _set_cur_to_pos(hConsole, positionCur);
        if (_men_position[1] == table->_col_count+1 ) {
            printf("\x1b[43mСохранить\x1b[0m");
        }
        else printf("Сохранить");
        positionCur.X = _center_x + (width / 2  - u8_strlen("Отмена") - 3);
        _set_cur_to_pos(hConsole, positionCur);
        if (_men_position[1] == table->_col_count+2) {
            printf("\x1b[43mОтмена\x1b[0m");
        }
        else printf("Отмена");
            c = getch();
            if (c == 13) {

                // return _output_info;
            }
            if (c == 27) {
                break;  return _output_info;
            }
            _men_position = _get_curent_selection(c, _men_position, table->_col_count + 2, 1, 1);
        }
    }



// 
//puts("┌──────┬─────┬────────────────────┬────────────┬───────────────┬──────┬──────────┬───────────┬───────┐");
//puts("│Индекс│Номер│ ФИО                │Год рождения│Год поступления│Физика│Математика│Информатика│История│");
//
//for (int i = 0; i < MaxIndex; i++)
//{
//    puts("├──────┼─────┼────────────────────┼────────────┼───────────────┼──────┼──────────┼───────────┼───────┤");
//    printf("│%6d│%5d│%-20s│%-20s│%-10.2f│%-13d│\n", st[i]._Index, st[i]._Number, st[i]._FIO, st[i]._God, st[i]._GodPos, st[i].marks.Fizika);
//}
//puts("└──────┴─────┴────────────────────┴────────────────────┴──────────┴─────────────┘");