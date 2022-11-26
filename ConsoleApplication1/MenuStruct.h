#ifndef MenuStruct
#define MenuStruct

typedef struct {
    int _menu_size;
    char _name[80];
    char** _sub_menu;
    int* _sub_menu_lenght;
    int _max_sub_lenght;
    int _menu_name_lenght;
    } _menu_item;

typedef struct {
    char* name;
    int size;
    int resizebl;
}_table_col;

typedef struct {
    _table_col* _cols;
    int _col_count;
}_tabel_metadata;

enum MenuItemCodes
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
