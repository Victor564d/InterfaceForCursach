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
    LOAD_FROM_FILE = 2,
    SAVE_TO_FILE = 3,
    EDIT_RECORD = 4,
    REMOVE_RECORD = 5,
    CLEAN_TREE = 6,
    RECORD_LEVEL = 7,
    PRINT_TREE_STRUCT = 8,
    TREE_SIZE = 10,
    PROCESS_1 = 11,
    PRINT_HORRIBLE_ANIMATION = 13,
    PROGRAM_EXIT = 16
};

#endif //! MenuStruct
