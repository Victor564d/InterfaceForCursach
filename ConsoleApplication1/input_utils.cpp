#pragma warning(disable : 4996);
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>


#include "utf8.h"


#include "input_utils.h"

BOOL _get_con_info_local(CONSOLE_SCREEN_BUFFER_INFO* x)
{
    return GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), x);
}

BOOL _set_cur_to_pos_local(COORD cor) {
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}



char convert_u8_to_1251(int c) {
    wchar_t s[2];
    s[0] = (wchar_t)c; s[1] = '\0';
    char utf8[20];
    wchar_t wstr[20];
    char s1251[20];
    WideCharToMultiByte(CP_UTF8, 0, s, -1, utf8, 100, NULL, NULL);
    utf8[WideCharToMultiByte(CP_UTF8, 0, s, -1, utf8, 0, NULL, NULL)] = '\0';
    // Подготовили строку UTF8 дальше идет ее преобразование в 1251
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, 100);
    wstr[MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, 0)];
    WideCharToMultiByte(1251, 0, wstr, -1, s1251, 100, NULL, NULL);
    s1251[WideCharToMultiByte(1251, 0, wstr, -1, s1251, 0, NULL, NULL)] = '\0';
    return s1251[0];
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
    SetConsoleCP(65001);
    SetConsoleOutputCP(1251);
    CONSOLE_SCREEN_BUFFER_INFO con_inf; _get_con_info_local(&con_inf);
    int current_pos = 0;
    int in_sym_count = 0;
    char* old_info = (char*)calloc(buff_size, sizeof(char));
    strcpy(old_info, input_buff);
    //old_info = *input_buff;
    COORD positionCur = { 0,0 };
    COORD positionCur_start = { 0,0 };
    positionCur = con_inf.dwCursorPosition;
    positionCur_start = con_inf.dwCursorPosition;
    if (u8_strlen(input_buff) > 0) {
        int k = u8_strlen(input_buff);
        current_pos = k;
        in_sym_count = k;
       // positionCur.X = positionCur.X - k;
        _set_cur_to_pos_local(positionCur);
        printf("%s", input_buff);
        positionCur = con_inf.dwCursorPosition;
    }
    int c;
    while (1) {
        c = _getwch(); 
        //c = _getch();
        //if (c == 224)
         //   c = _getch();
        //else c = (char)c;
        switch (c)
        {
        case KEY_ENTER:
            if (in_sym_count > 0) {
                printf(" ");
                return KEY_ENTER;
            }
            break;
        case KEY_ESC:
            strcpy(input_buff, old_info);
            return KEY_ESC;
            break;
        case KEY_ARROW_UP:
            if (in_sym_count > 0) {
                strcpy(input_buff, old_info);
                _get_con_info_local(&con_inf);
                positionCur.X = con_inf.dwCursorPosition.X;
                positionCur.X--; _set_cur_to_pos_local(positionCur);
                printf(" ");
                _set_cur_to_pos_local(positionCur);
                return KEY_ARROW_UP;
            }
            break;
        case KEY_ARROW_DOWN:
            if (in_sym_count > 0) {
                strcpy(input_buff, old_info);
                _get_con_info_local(&con_inf);
                positionCur.X = con_inf.dwCursorPosition.X;
                positionCur.X--; _set_cur_to_pos_local(positionCur);
                printf(" ");
                _set_cur_to_pos_local(positionCur);
                return KEY_ARROW_DOWN;
            }
            break;
        case KEY_ARROW_LEFT:
        case KEY_ARROW_RIGHT:
            _get_con_info_local(&con_inf);
            positionCur.X = con_inf.dwCursorPosition.X;
            positionCur.X--; _set_cur_to_pos_local(positionCur);
            printf(" ");
            _set_cur_to_pos_local(positionCur);
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
            c = convert_u8_to_1251(c);
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
                if (c == ' ' && mode == PERSONAL)
                {
                    if (in_sym_count > 0) {
                        printf(" ");
                        return KEY_ENTER;
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

int in_date(int* d, int*m,int *y) {
    int n_count;
    char* correct_sym;
    int temp = 0;
    n_count = 10;
    char buff[10] = "";
    correct_sym = (char*)calloc(n_count, sizeof(char));
    for (int i = 48; i < 59; i++) {
        correct_sym[temp] = i;
        temp++;
    }
    SetConsoleCP(65001);
    SetConsoleOutputCP(1251);
    CONSOLE_SCREEN_BUFFER_INFO con_inf; _get_con_info_local(&con_inf);
    int current_pos = 0;
    int in_sym_count = 0;
    int cur_selector = 1;
    int old_d, old_m, old_y;
    if (d) old_d = *d;
    if(m)old_m = *m;
     if (y) old_y = *y;
    COORD positionCur = { 0,0 };
    COORD positionCur_start = { 0,0 };
    positionCur = con_inf.dwCursorPosition;
    printf("___________");
    _set_cur_to_pos_local(positionCur);
    positionCur_start = con_inf.dwCursorPosition;
    int c;
    while (1) 
    {
        c = _getwch();
      /*  if (c == 224) 
        {
            c = getch();
        }*/
        switch (c)
        {
        case KEY_ENTER:
            if (cur_selector > 3) {
                printf(" ");
                return KEY_ENTER;
            }
            break;
        case KEY_ESC:
            //  strcpy(input_buff, old_info);
            return KEY_ESC;
            break;
        case KEY_ARROW_UP:
            if (in_sym_count > 0) {
                _get_con_info_local(&con_inf);
                positionCur.X = con_inf.dwCursorPosition.X;
                positionCur.X--; _set_cur_to_pos_local(positionCur);
                printf(" ");
                _set_cur_to_pos_local(positionCur);
                return KEY_ARROW_UP;
            }
            break;
        case KEY_ARROW_DOWN:
            if (in_sym_count > 0) {
                _get_con_info_local(&con_inf);
                positionCur.X = con_inf.dwCursorPosition.X;
                positionCur.X--; _set_cur_to_pos_local(positionCur);
                printf(" ");
                _set_cur_to_pos_local(positionCur);
                return KEY_ARROW_DOWN;
            }
            break;
        case KEY_ARROW_LEFT:
        case KEY_ARROW_RIGHT:
            _get_con_info_local(&con_inf);
            positionCur.X = con_inf.dwCursorPosition.X;
            positionCur.X--; _set_cur_to_pos_local(positionCur);
            printf(" ");
            _set_cur_to_pos_local(positionCur);
            break;
        case KEY_BACKSPACE:
        {
            if (current_pos > 0 && in_sym_count > 0) {
                {
                    buff[current_pos] = '\0';
                    current_pos--;
                    in_sym_count--; int flag = 0;
                    if (current_pos <= 4 && current_pos > 2) {
                        if (cur_selector > 2) {
                            cur_selector--;
                            flag = 1;
                        }
                    }
                    else if (current_pos < 2) {
                        if (cur_selector > 1) {
                            cur_selector--;
                            flag = 1;
                        }
                    }
                    _get_con_info_local(&con_inf);
                    positionCur.X = con_inf.dwCursorPosition.X;
                    if (flag) {
                        positionCur.X--;  positionCur.X--; _set_cur_to_pos_local(positionCur);                
                        printf(" ");  printf(" ");
                        _set_cur_to_pos_local(positionCur);
                    }
                    else {
                        positionCur.X--;  _set_cur_to_pos_local(positionCur);
                        printf(" ");
                        _set_cur_to_pos_local(positionCur);
                    }

                }
            }
        }
        break;
        default: {
            c = convert_u8_to_1251(c);
            int flag = 0;
            for (int i = 0; i < n_count; i++)
            {
                if (c == correct_sym[i]) {
                    flag = 1;
                    break;
                }
            }
            if (flag) {
                if (cur_selector < 4) {
                    if (in_sym_count >= 2 && in_sym_count < 4) {
                        buff[in_sym_count-2] = c;
                    }
                    else if (in_sym_count >= 4){
                        buff[in_sym_count-4] = c;
                    }
                    else
                    buff[in_sym_count] = c;
                    printf("%c", c);
                    in_sym_count++;
                    if (in_sym_count == 2) {
                        *d = atoi(buff);
                        if (*d > 31) { 
                        *d = 31; 
                            _get_con_info_local(&con_inf);
                            positionCur.X = con_inf.dwCursorPosition.X;
                            positionCur.X--;  positionCur.X--; _set_cur_to_pos_local(positionCur);
                            printf(" ");  printf(" ");
                            _set_cur_to_pos_local(positionCur);
                            printf("%d", *d);
                        }
                        printf(".");
                        memset(buff, 0, strlen(buff));
                        cur_selector++;
                    }
                    else if (in_sym_count == 4) {
                        *m = atoi(buff);
                        if (*m > 12) { *m = 12; 
                        _get_con_info_local(&con_inf);
                        positionCur.X = con_inf.dwCursorPosition.X;
                        positionCur.X--;  positionCur.X--; _set_cur_to_pos_local(positionCur);
                        printf(" ");  printf(" ");
                        _set_cur_to_pos_local(positionCur);
                        printf("%d", *m);
                        }
                        printf(".");
                        memset(buff, 0, strlen(buff));
                        cur_selector++;
                    }
                    else if (in_sym_count == 8) {
                        // printf(".");
                        *y = atoi(buff);
                        time_t  now = time(0);
                        struct tm* ltm = localtime(&now);
                        if (*y > ltm->tm_year + 1900) { *y = ltm->tm_year + 1900; 
                        _get_con_info_local(&con_inf);
                        positionCur.X = con_inf.dwCursorPosition.X;
                        positionCur.X-=4; _set_cur_to_pos_local(positionCur);
                        printf("    ");
                        _set_cur_to_pos_local(positionCur);
                        printf("%d", *y);
                        }
                        return KEY_ENTER;
                    }
                    current_pos++;
                }
            }
        }
               break;
        }
      }
    }

