#include "MenuStruct.h"
#include "ThreeStruct.h"

#ifndef VicMenuDLL
#define VicMenuDLL
/// <summary>
/// Функция получения новой позиции селектора на основе старой
/// </summary>
/// <param name="c">Код нажатого символа</param>
/// <param name="position">Указатель на массив из двух элементво (x,y)</param>
/// <param name="MaxY">Максимальная высота(глубина) списка(Макс смещение по вертикали)</param>
/// <param name="Colums">Ширина(максимальное смещение указателя по горизонтали)</param>
/// <returns>Указатель на двумерный массив, с измененными параметрами</returns>
int* _get_curent_selection(char  c // Символ клавиатуры
    , int* position // Массив в котором хранятся x и y
    , int MaxY // Максимальный количество строк 
    , int Colums  // Количество столбцов, по умолчанию - 1
    ,int _flag_x_readonly
);

/// <summary>
/// Функция построения меню. Вызывается если нам не нужно вывести перед этим какую-то информацию.
/// </summary>
/// <param name="Menu">Указатель на массив из наименований пунктов меню</param>
/// <param name="position">Указатель на двумерный массив (x,y)</param>
/// <param name="MenuSize">Колличество элементов в массиве наименований</param>
/// <param name="Colums">Количество стобцов которое необходимо построить. Принимает значения 1,2,3</param>
/// <returns>Индекс выбранного пункта меню</returns>
int _print_menu(_menu_item* _menu, int* position, int _menu_size, int Colums, abonent_t* _output_mas,
    int _output_colcount, _tabel_metadata* table);


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
int _print_menu_with_table(_menu_item* _menu, int* position, int _menu_size, int Colums, abonent_t* _output_mas,
    int _output_colcount, _tabel_metadata* table);

// <summary>
/// Анимированная картинка в консоли
/// </summary>
void animatedNeko();
/// <summary>
/// Картинка в консоли
/// </summary>
/// <param name="Password">Пароль для вывода</param>





void _print_bakground(int _window_w, int _window_h);
void clear();
void _print_border(int _window_w, int _window_h);
int* _get_window_size();
int _confirm_window(char *);
void _window(int _window_w, int _window_h,char * title);
void clear_for_info();
void _in_window();
void _message_window( char* message);
int _table_window(_tabel_metadata* table, abonent_t* _output_mass, int _info_count, int* page, int* _table_focus_flag);
void _big_window(char* title);
abonent_t* _in_info_window(_tabel_metadata* table, abonent_t* _output_mass,int);                                                                    

enum KeyboardCodes
{
    KEY_ARROW_UP = 72,
    KEY_ARROW_DOWN = 80,
    KEY_ARROW_LEFT = 75,
    KEY_ARROW_RIGHT = 77,
    KEY_TAB = 9,
    KEY_HOME = 71,
    KEY_END  = 79,
    KEY_ENTER = 13,
    KEY_ESC  = 27
};

#endif


