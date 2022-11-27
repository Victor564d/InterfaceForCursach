#ifndef MenuStruct
#define MenuStruct

typedef struct { // структура одного элемента меню
    int _menu_size; //размер меню 
    char _name[80]; //наименование пункта меню
    char** _sub_menu; //указатель на массив сабменю
    int* _sub_menu_lenght; //количество элементов сабменю
    int _max_sub_lenght; //длина самой большого элемента в сабменю
    int _menu_name_lenght; //длина имени меню
    } _menu_item;

typedef struct { // структура хранящая данные столбца таблицы
    char* name; //имя столбца
    int size; //его размер 
    int resizebl; //флаг, можно ли изменять ее размер ? 
}_table_col;

typedef struct { // структура для хранения метаданных таблицы
    _table_col* _cols; //массив столбцов
    int _col_count; //количество столбцов
}_tabel_metadata;

enum MenuItemCodes //Коды для обработки вызовов меню
{
    ADD_NEW_RECORD = 1,
    LOAD_FROM_FILE_TYPE = 2,
    LOAD_FROM_FILE = 3,
    SAVE_TO_FILE_TYPE = 4,
    SAVE_TO_FILE = 5,
    TREE_SIZE = 6,
    PRINT_TREE_STRUCT = 7,
    CLEAN_TREE = 8,
    DOLGNIKI_WINDWO = 9,
    PRINT_HORRIBLE_ANIMATION = 10,
    PROGRAM_EXIT = 11
};

#endif //! MenuStruct
