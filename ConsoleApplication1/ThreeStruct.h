#pragma once
typedef struct {
    int id;
    char name[40];
    char surname[40];
    int birth_date;
    int stud_date;
    struct {
        int fiz;
        int math;
        int it;
        int history;
    } marks;
} STudent;
/// <summary>
/// Структура ноды дерева
/// </summary>
typedef struct Persons {
    STudent info;
    struct Persons* right;
    struct Persons* left;
} Person;