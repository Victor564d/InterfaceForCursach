#include "MenuStruct.h"
#include "ThreeStruct.h"

#ifndef VicMenuDLL
#define VicMenuDLL

typedef struct {
    int sort_f;
    int sort_t;
}sort_struct;

int* _get_curent_selection(char  c // Символ клавиатуры
    , int* position // Массив в котором хранятся x и y
    , int MaxY // Максимальный количество строк 
    , int Colums  // Количество столбцов, по умолчанию - 1
    ,int _flag_x_readonly
);

/// <summary>
/// Функция печати основного окна меню. Предназначена для создания внутренней области для работы с данными и реализует возможность 
/// работы с самим меню используя стрелки. 
/// </summary>
/// <param name="_menu"> структура данных содержащая в себе меню </param>
/// <param name="position">массив текущей позиции x,y</param>
/// <param name="_menu_size">Количество основных пунктов меню</param>
/// <param name="Colums">Количество кнопок меню</param>
/// <param name="_output_mas">Массив, необходимый для вывода информации</param>
/// <param name="_output_colcount">Количество данных в массиве вывода </param>
/// <param name="table">Структура хранящая информациию о таблице</param>
/// <param name="root">Корень дерева, предназначен для вызова функций из внутренностей меню</param>
/// <param name="sort">Структура, хранящая настройки сортировки</param>
/// <returns>Индекс выбранного элемента</returns>
int _print_menu(_menu_item* _menu, int* position, int _menu_size, int Colums, abonent_t* _output_mas,
    int _output_colcount, _tabel_metadata* table, abonent** root, sort_struct* sort);
/// <summary>
/// Анимация(над сделать)
/// </summary>
void animatedNeko();
/// <summary>
/// Рисует задний фон согласно параметрам, делая область для работы 
/// </summary>
/// <param name="_window_w">ширина окна</param>
/// <param name="_window_h">высота окна</param>
void _print_bakground(int _window_w, int _window_h);
/// <summary>
/// чистит экран
/// </summary>
void clear();
/// <summary>
/// Печать границ окна с очисткой в нутри
/// </summary>
/// <param name="_window_w">ширина окна</param>
/// <param name="_window_h">высота окна</param>
void _print_border(int _window_w, int _window_h);
/// <summary>
/// Возвращает текущий размер окна
/// </summary>
/// <returns>Двумерный массив, ширина и высота</returns>
int* _get_window_size(int* size);
/// <summary>
/// Функция отричовки окна подверждения с кастомным сообщением 
/// </summary>
/// <param name="msg">Указатель на строку сообщения, если NULL то выведет стандартное окно</param>
/// <returns>0 либо 1 в зависимости от того что выбрал пользователь</returns>
int _confirm_window(char *);
/// <summary>
/// Функция печати окна для работы в дальнейшем
/// </summary>
/// <param name="_window_w">Текущая ширина</param>
/// <param name="_window_h">Текущая высота </param>
/// <param name="title">Название окна </param>
void _window(int _window_w, int _window_h,char * title);
/// <summary>
/// Очистка только внутренней части окна
/// </summary>
void clear_for_info();
/// <summary>
/// Окно ввода данных. Считывать данные можно чем угодно вызвав после окна функцию считывания
/// </summary>
void _in_window();
/// <summary>
/// Окно сообщения. 
/// </summary>
/// <param name="message">Сообщение</param>
void _message_window( char* message);
/// <summary>
/// Табличная форма вывода данных с поддержкой управления в нутри таблицы
/// </summary>
/// <param name="table">структура данных, содержащая информацию о полях таблицы</param>
/// <param name="_output_mass">Массив данных для вывода</param>
/// <param name="_info_count">Количество данных в массиве для вывода</param>
/// <param name="page">Указатель, хранящий текущюю страницу</param>
/// <param name="_table_focus_flag">Флаг работы с таблицей</param>
/// <param name="root">Корень дерева</param>
/// <param name="sort">Структура сортировки </param>
/// <returns></returns>
int _table_window(_tabel_metadata* table, abonent_t* _output_mass, int* _info_count, int* page, int* _table_focus_flag,abonent ** root, sort_struct* sort);
/// <summary>
/// Большое окно для работы с данными 
/// </summary>
/// <param name="title">Название окна </param>
void _big_window(char* title);
/// <summary>
/// Окно ввода/редактирования информации
/// </summary>
/// <param name="table">Поля из таблицы, используются для отрисовки пунктов</param>
/// <param name="_output_mass">указатель на элемент данных, null для ввода</param>
/// <param name="flag">Флаг цикличного ввода. Для формирования дерева в цикле</param>
/// <returns>Возвращает элемент</returns>
abonent_t* _in_info_window(_tabel_metadata* table, abonent_t* _output_mass,int);  
/// <summary>
/// Печать информационной помощи
/// </summary>
/// <param name="help_message">Сообщение помощи. Поддерживает изменение цвета</param>
/// <returns></returns>
int print_help(char* help_message);
/// <summary>
///  Сортирует данные согласно указанной структуре
/// </summary>
/// <param name="">Указатель на данные (массив)</param>
/// <param name="">Количество данных в массиве</param>
/// <param name="">Структура хранящаяя текущюю сортировку</param>
/// <returns>Указатель на новые отсортированные данные</returns>
abonent_t* _sort_output(abonent_t*, int *,sort_struct *);
/// <summary>
/// Окно просмотра должников
/// </summary>
/// <param name="root">Корень дерева для получения информации</param>
void dolgiWindow(abonent** root);
#endif

#ifndef KEYCODE 
#define KEYCODE
/// <summary>
/// Енум хранящий коды кнопок 
/// </summary>
enum KeyboardCodes
{
    KEY_ARROW_UP = 72,
    KEY_ARROW_DOWN = 80,
    KEY_ARROW_LEFT = 75,
    KEY_ARROW_RIGHT = 77,
    KEY_TAB = 9,
    KEY_HOME = 71,
    KEY_END = 79,
    KEY_ENTER = 13,
    KEY_ESC = 27,
    KEY_DEL = 83,
    KEY_BACKSPACE = 8,
    KEY_PGUP = 73,
    KEY_PGDOWN = 81
};
#endif
/// <summary>
/// Енум , хранящий поля сортировок
/// </summary>
enum sort_fild { 
    DEF,
    FIO,
    AUTHOR,
    BOOK_NAME,
    IZD,
    DATE_OUT,
    COST,
    ZADANIE
};
enum sort_type {
    UP,
    DOWN
};