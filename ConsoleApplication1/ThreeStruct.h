#pragma once
typedef struct {
    char name[60];
    char surname[60];
    char secondname[60];
}fio_t;

typedef struct {
    unsigned int id;
    fio_t fio;
    struct {
        char surname[40];
        char inicial[3];
    }autor;
    char book_name[140];
    char izd[50];
    struct {
        int d;
        int m;
        int y;
    }date_out;
    float cost;
} abonent_t;
/// <summary>
/// Структура ноды дерева
/// </summary>
typedef struct abonent_l {
    abonent_t info;
    struct abonent_l* right;
    struct abonent_l* left;
} abonent;