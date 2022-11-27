#pragma once
typedef struct { //Структура для хранения фио
    char name[80]; //Имя 
    char surname[80];//Фамилия 
    char secondname[80];//отчество
}fio_t;

typedef struct { // основная структура информационного поля 
    unsigned int id; //идентификатор поля
    fio_t fio; //поле структуры фио
    struct { // структура данных для автора книги 
        char surname[60];
        char inicial[20];
    }autor;
    char book_name[160]; //поле для названия книги
    char izd[70]; // поле для издания 
    struct { // структура, хранит дату выдачи книги
        int d;
        int m;
        int y;
    }date_out;
    float cost; // цена книги
} abonent_t;

typedef struct { // структура для хранения должников  
    unsigned int id;  //идентификатор
    fio_t fio; // поле фио
    int count_dolg_books; // количество книг в задолжности
}dolgi_pers_t;

typedef struct { // структура-обертка для хранения данных должников
    dolgi_pers_t* info_mass; // массив должников
    int count; // количество должников
}dolgi_pers_t_obr;


/// <summary>
/// Структура ноды дерева
/// </summary>
typedef struct abonent_l {
    abonent_t info; // информационное поле
    struct abonent_l* right; //правая нода
    struct abonent_l* left; // левая нода 
} abonent;
const int size_abonent_t; //константа, хранит размер информационного поля 