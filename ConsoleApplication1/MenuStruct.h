#ifndef MenuStruct
#define MenuStruct
typedef struct {
    int _menu_size;
    char _name[60];
    char** _sub_menu;
    int* _sub_menu_lenght;
    int _max_sub_lenght;
    int _menu_name_lenght;
    } _menu_item;
    
#endif // !
