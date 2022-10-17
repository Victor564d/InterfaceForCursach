#pragma once
typedef struct {
    char name[80];
    char surname[80];
    char secondname[80];
}fio_t;

typedef struct {
    unsigned int id;
    fio_t fio;
    struct {
        char surname[60];
        char inicial[20];
    }autor;
    char book_name[160];
    char izd[70];
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
const int size_abonent_t;