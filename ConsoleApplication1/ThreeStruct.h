#pragma once
typedef struct {
    char name[40];
    char surname[40];
    char secondname[40];
}fio_t;

typedef struct {
    char id[6];
    fio_t fio;
    struct {
        char surname[40];
        char inicial[3];
    }autor;
    char book_name[100];
    char izd[30];
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