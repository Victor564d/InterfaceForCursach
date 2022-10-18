#include "MenuStruct.h"
#include "ThreeStruct.h"

#ifndef VicMenuDLL
#define VicMenuDLL

int* _get_curent_selection(char  c // Символ клавиатуры
    , int* position // Массив в котором хранятся x и y
    , int MaxY // Максимальный количество строк 
    , int Colums  // Количество столбцов, по умолчанию - 1
    ,int _flag_x_readonly
);


int _print_menu(_menu_item* _menu, int* position, int _menu_size, int Colums, abonent_t* _output_mas,
    int _output_colcount, _tabel_metadata* table, abonent** root);
void animatedNeko();
void _print_bakground(int _window_w, int _window_h);
void clear();
void _print_border(int _window_w, int _window_h);
int* _get_window_size();
int _confirm_window(char *);
void _window(int _window_w, int _window_h,char * title);
void clear_for_info();
void _in_window();
void _message_window( char* message);
int _table_window(_tabel_metadata* table, abonent_t* _output_mass, int* _info_count, int* page, int* _table_focus_flag,abonent ** root);
void _big_window(char* title);
abonent_t* _in_info_window(_tabel_metadata* table, abonent_t* _output_mass,int);                                                                    


#endif

#ifndef KEYCODE
#define KEYCODE
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
    KEY_BACKSPACE = 8
};
#endif
