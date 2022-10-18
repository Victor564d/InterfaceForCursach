#pragma warning(disable : 4996);
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include "utf8.h"

#include "input_utils.h"

BOOL _get_con_info_local(CONSOLE_SCREEN_BUFFER_INFO* x)
{
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

BOOL _set_cur_to_pos_local(COORD cor) {
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}


/// <summary>
/// Ввод любых строковых данных
/// </summary>
/// <param name="input_buff">Буфер для ввода</param>
/// <param name="buff_size">Размер буфера</param>
/// <param name="mode">Режим работы, от которого зависят допустимые символы.  0 - разрешение на ввод всего, кроме спец символов, 1- для реализации ввода Фамилии\имени\Отчества\Издательства(не допускаются цифры и т.д),2 - Ввод инициалов</param>
/// <returns></returns>
int input_string(char* input_buff, int buff_size, int mode)
{
    int n_count;
    char* invalid_sym;
    int temp = 0;
    switch (mode)
    {
    case NORMAL:
    {
        n_count = 5;
        invalid_sym = (char*)calloc(n_count, sizeof(char));
        for (int i = 35; i < 40; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        break;
    }
    case PERSONAL: {
        n_count = 42;
        invalid_sym = (char*)calloc(n_count, sizeof(char));
        for (int i = 32; i < 65; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        for (int i = 91; i < 97; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        for (int i = 123; i < 127; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        break; }
    case INICIAL: {
        n_count = 42;
        invalid_sym = (char*)calloc(n_count, sizeof(char));
        for (int i = 32; i < 65; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        for (int i = 91; i < 97; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        for (int i = 123; i < 127; i++) {
            invalid_sym[temp] = i;
            temp++;
        }
        break; }

        default:
        {
            n_count = 5;
            invalid_sym = (char*)calloc(n_count, sizeof(char));
            for (int i = 35; i < 40; i++) {
                invalid_sym[temp] = i;
                temp++;
            }
            break;

        }
    }
    SetConsoleCP(1251);
    CONSOLE_SCREEN_BUFFER_INFO con_inf; _get_con_info_local(&con_inf);
    int current_pos = 0;
    int in_sym_count = 0;
    char* old_info = (char*)calloc(buff_size, sizeof(char));
    old_info = *input_buff;
    COORD positionCur = { 0,0 };
    COORD positionCur_start = { 0,0 };
    positionCur = con_inf.dwCursorPosition;
    positionCur_start = con_inf.dwCursorPosition;
    if (u8_strlen(input_buff) > 0) {
        int k = u8_strlen(input_buff);
        current_pos = k;
        in_sym_count = k;
        printf("%s", input_buff);
        positionCur = con_inf.dwCursorPosition;
    }
    int c;
    while (1) {
       // flushall();
        c = _getch(); 
        if (c == 244)
            c = _getch();
        else c = (char)c;
        switch (c)
        {
        case KEY_ENTER:
            if (in_sym_count > 0) {
                printf(" ");
                return KEY_ENTER;
            }
            break;
        case KEY_ESC:
            input_buff = old_info;
            return KEY_ESC;
            break;
        case KEY_ARROW_UP:
            input_buff = old_info;
            return KEY_ARROW_UP;
            break;
        case KEY_ARROW_DOWN:
            input_buff = old_info;
            return KEY_ARROW_DOWN;
            break;
        case KEY_ARROW_LEFT:
        case KEY_ARROW_RIGHT:
            break;
        case KEY_BACKSPACE:
            if (mode != INICIAL) {
                if (current_pos > 0 && in_sym_count > 0)
                {
                    input_buff[current_pos] = '\0';
                    current_pos--;  in_sym_count--;
                    _get_con_info_local(&con_inf);
                    positionCur.X = con_inf.dwCursorPosition.X;
                    positionCur.X--; _set_cur_to_pos_local(positionCur);
                    printf(" ");
                    _set_cur_to_pos_local(positionCur);
                }
            }
            else
                {
                    if (current_pos > 0 && in_sym_count > 0) {
                        if (current_pos>2)
                        {
                            input_buff[current_pos] = '\0';
                            current_pos--;  in_sym_count--;
                            _get_con_info_local(&con_inf);
                            positionCur.X = con_inf.dwCursorPosition.X;
                            positionCur.X--; _set_cur_to_pos_local(positionCur);
                            printf(" ");
                            _set_cur_to_pos_local(positionCur);
                        }
                        else {
                            input_buff[current_pos] = '\0';
                            current_pos--; 
                            input_buff[current_pos] = '\0';
                            current_pos--;
                            in_sym_count--; in_sym_count--;
                            _get_con_info_local(&con_inf);
                            positionCur.X = con_inf.dwCursorPosition.X;
                            positionCur.X--;  positionCur.X--; _set_cur_to_pos_local(positionCur);
                            printf(" ");  printf(" ");
                            _set_cur_to_pos_local(positionCur);
                        }
                    }
                }
            break;
        default:
            if (mode == INICIAL) {
                int flag = 0;
                for (int i = 0; i < n_count; i++)
                {
                    if (c == invalid_sym[i]) {
                        flag = 1;
                        break;
                    }
                }
                if (!flag) {
                    if (in_sym_count < buff_size) {
                        input_buff[in_sym_count] = c;
                        
                        if (in_sym_count == 0) {
                            in_sym_count++;
                            current_pos++;
                            input_buff[in_sym_count] = '.';
                            printf("%c.", c);
                        }
                        else  printf("%c", c);
                        in_sym_count++;
                        current_pos++;
                    }
                }
            }
            else {
                int flag = 0;
                for (int i = 0; i < n_count; i++)
                {
                    if (c == invalid_sym[i]) {
                        flag = 1;
                        break;
                    }
                }
                if (!flag) {
                    if (in_sym_count < buff_size) {
                        input_buff[in_sym_count] = c;
                        printf("%c", c);
                        in_sym_count++;
                        current_pos++;
                    }
                }
            }
            break;
        }
    }
}

