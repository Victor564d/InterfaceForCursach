#pragma warning(disable : 4996);
#include "VicMenuDLL.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

const int _otstup = 3;
const int _interval = 3;


int x, y; 

#define clearf() system("cls");
void clear() {
    COORD positionCur = { 10,10 }; //позиция x и y
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, positionCur);
}

/// <summary>
/// Функция получения новой позиции селектора на основе старой
/// </summary>
/// <param name="c">Код нажатого символа</param>
/// <param name="position">Указатель на массив из двух элементво (x,y)</param>
/// <param name="MaxY">Максимальная высота(глубина) списка(Макс смещение по вертикали)</param>
/// <param name="Colums">Ширина(максимальное смещение указателя по горизонтали)</param>
/// <returns>Указатель на двумерный массив, с измененными параметрами</returns>
int* GetCurentSelector(char  c // Символ клавиатуры
    ,int * position // Массив в котором хранятся x и y
    ,int MaxY // Максимальный количество строк 
    , int Colums  // Количество столбцов, по умолчанию - 1
) 
{
    int x = position[0]; int y = position[1];
    switch (c)
    {
    case 72://вверх
        if (y > 1) y--;
        break;
    case 80://вниз
        if (y < MaxY) y++;
        break;
    case 75://лево
        if (x > 1) x--;
        break;
    case 77://право
        if (x < Colums) x++;
        break;
    }
    position[0] = x; //запоминаем позицию по столбцу
    position[1] = y;//позиция по строке
    return position;
}
/// <summary>
/// Функция построения меню. Вызывается если нам не нужно вывести перед этим какую-то информацию.
/// </summary>
/// <param name="Menu">Указатель на массив из наименований пунктов меню</param>
/// <param name="position">Указатель на двумерный массив (x,y)</param>
/// <param name="MenuSize">Колличество элементов в массиве наименований</param>
/// <param name="Colums">Количество стобцов которое необходимо построить. Принимает значения 1,2,3</param>
/// <returns>Индекс выбранного пункта меню</returns>
int PrintMenu(char** Menu, int* position, int MenuSize, int Colums)
{
    return PrintMenuWithTable (Menu, position, MenuSize, Colums, NULL, NULL, -1);
}

/// <summary>
/// Функция построения меню. Вызывается если нам  нужно вывести перед этим какую-то информацию.
/// </summary>
/// <param name="Menu">Указатель на массив из наименований пунктов меню</param>
/// <param name="position">Указатель на двумерный массив (x,y)</param>
/// <param name="MenuSize">Колличество элементов в массиве наименований</param>
/// <param name="Colums">Количество стобцов которое необходимо построить. Принимает значения 1,2,3</param>
/// <param name="funcptr">Указатель на функцию вывода данных экран</param>
/// <param name="Dataptr">Указатель на информационное поле</param>
/// <param name="num">Указатель на номер элемента при выводе</param>
/// <returns>Индекс выбранного пункта меню</returns>
int PrintMenuWithTable(char** Menu //Массив меню 
    , int* position //Массив текущей позиции x и y
    , int MenuSize  //Колличество элементов меню
    ,int Colums,//Количество столбцов 
    int (* funcptr ) (void*, int),
    void * Dataptr,
    int num
)  
{
    int Rows = MenuSize; int Override = 0;  int Console_Width = 180; 
    if (Colums > 3) Colums = 3; //Если ввели неверное значение строк (больше 3-х) то ставим 3
    if (Colums < 1) Colums = 1;// если строк меньше одной - ставим одну
    if (Colums > 1) { Rows = MenuSize / Colums; Override = MenuSize % Colums; if (Override > 0) Rows++; } //Если строк больше одной , вычисляем количество не полных строк 
    while (1) //цикл отрисовки меню 
     {
       int flag = 0; //флаг является показателем того что вывод строки окончен

        if (funcptr != NULL && Dataptr != NULL) //Если у нас есть данные для вывода - выводим
            funcptr (Dataptr, num);

     for (int i = 0; i < Console_Width; i++) //цикл вывода делителя
     {
        printf("-"); 
     }
     printf("\n"); //переход на следующую строку
      if (Colums == 1)  //если столбцов всего 1 
        {
        for (int i = 0; i < MenuSize; i++)  // до конца массива меню 
            {
                 if (position[1] - 1 == i) // если курсор на позици 
                      printf("| --> %-172s |\n", Menu[i]);//выводим с курсором 
                  else printf("| %-176s |\n" ,Menu[i]);// иначе без 
            }
        } else 
          if (Colums == 2)  // если столбцов 2 , то 
           {
              int PosOut = 0; //Переменная текущей позиции в массиве меню
              for (int i = 0; i < Rows; i++) { //цикл прохода по строкам
                  for (int j = 0; j < Colums; j++) // цикл по столбцам
                  {
                      printf("|"); // делитель 
                      if (i == Rows - 1 )  // если это последняя строка 
                      { 
                          if (Override) { // если она не полная , то 
                              if ((position[0] - 1 == j)&&(position[1] - 1 ==i)) printf(" --> %-83s", Menu[PosOut++]); //если курсор на текукщей позици, выведем с ним 
                              else printf(" %-86s ", Menu[PosOut++]); // иначе без
                              printf("|"); for (int l = 1; l < 89; l++) printf(" "); printf("|"); // печатаем делители 
                              flag = 1; // вывод строки меню завершен 
                              break; // остановить цикл
                          } else { //если все строки полные 
                              if ((position[0] - 1 == j) && (position[1] - 1 == i)) printf(" --> %-83s", Menu[PosOut++]); // если есть селектор - выводим с ним 
                              else printf(" %-86s ", Menu[PosOut++]); // иначе без 
                          }
                      }
                      else // если строка не последняя 
                      {
                          if ((position[0]- 1 == j) && (position[1] - 1 == i)) printf(" --> %-83s", Menu[PosOut++]); // если селектор стоит на текущей позиции выводим с ним
                          else printf(" %-86s ", Menu[PosOut++]); // иначе без
                      }
                  }
                  if (!flag) // если достигли конца , делаем просто переход без делителя 
                      printf("|\n"); else printf("\n");
              }
           }
          else  // если строк больше 2
          {
              int PosOut = 0;
              for (int i = 0; i < Rows; i++) {
                  for (int j = 0; j < Colums; j++)
                  {
                      printf("|");
                      if (i == Rows - 1)
                      {
                          if (Override == 1) {
                              if ((position[0] - 1 == j) && (position[1] - 1 == i)) printf(" --> %-54s", Menu[PosOut++]);
                              else printf(" %-57s ", Menu[PosOut++]);
                              printf("|"); for (int l = 1; l < 60; l++) printf(" "); printf("|"); 
                              for (int l = 1; l < 60; l++) printf(" "); printf("|");
                              flag = 1;
                              break;
                          }
                          else if (Override) {
                              if ((position[0] - 1 == j) && (position[1] - 1 == i)) printf(" --> %-54s", Menu[PosOut++]);
                              else printf(" %-57s ", Menu[PosOut++]);  printf("|"); j++; i++;
                              if ((position[0] - 1 == j) && (position[1] - 1 == i)) printf(" --> %-54s", Menu[PosOut++]);
                              else printf(" %-57s ", Menu[PosOut++]);
                              printf("|");
                              for (int l = 1; l < 60; l++) printf(" "); printf("|");
                              flag = 1;
                              break;
                                }
                          else {
                              if ((position[0] - 1 == j) && (position[1] - 1 == i)) printf(" --> %-54s", Menu[PosOut++]);
                              else printf(" %-57s ", Menu[PosOut++]);
                          }
                      }
                      else
                      {
                          if ((position[0] - 1 == j) && (position[1] - 1 == i)) printf(" --> %-54s", Menu[PosOut++]);
                          else printf(" %-57s ", Menu[PosOut++]);
                      }
                  }
                  if (!flag)
                      printf("|\n"); else printf("\n");
              }
          }
        
        for (int i = 0; i < Console_Width; i++) {
            printf("-");
        }

        char c = getch();
         if (c == 13) {
             if (Colums == 1) return position[1]; else if (Colums == 2)
                 return position[0] + (position[1] - 1) * 2; else return position[0] + (position[1] - 1) * 3;
                    }
         int MaxY = MenuSize / Colums; if (Override) MaxY++;
        position =  GetCurentSelector(c, position, MaxY, Colums);
        clear();
      }
}




const char* BMP1[] = {
"................~J?!~~~~!!7?JYYYY55555PPPPPPPPPPPPPPPPPPGPPPPPPGBBBBBBBB#&&#B##BGB#BGB#BGGGGGGBGGGGGGGGGGGGGG5^75GGGB#&B7:^!7!777JGPB######B7......................................."   ,
"..................!JJ7~^^^:::^^~^::^~^^^~~~!!~!7?JYY55PGPPPPPGGGGPPPPG##&&#BGGBGGGBGGGGGGGGGGGGGGGGGGGGGGGGGGY. .^?PGB#&#GJ!!!777PGG#######BG7......................................"   ,
"         ...........^?J?!~~~^:::......      .    .:^~JGPPPPPPGPPPPPPGB###BGPPPPPPPPPPGGGGPJ?5GGGGGGGGGGGGGG5!?J!.~55GB###&#P7!775GPG#B######BP7:.................................   "   ,
"              ....   .:?Y7!~~~^:....::..            ~J5PPPP55555555GBB##BPPPPPPPPPPPPPPGGG7^:!YPGGGGGGGGGGY^~!~JPPGGPB#####&BJ75GPPB&##B###BGG57........................          .."   ,
"..........           ...:7YJ?77~^:.......          .:7P5P555555555PBPPGBP55PPPPPPPPPPPPPPPJ.. .^57JGGYYY!:...:7PGPPPPG#####&&#BGPPG&####GB##GGG57...............          .........."   ,
"....           ...........:!Y5Y7~^^:::::.         .:^Y??5555555555BP!PGG5555PPPPPPPPPPPPPGJ.    .7.:~YP???::^!5GPPPPPPG#####&#BGGGB##B##BGGB#GGGY!.....           .................."   ,
".      ......  ..............~JY?~::::.....      .:^P7^Y55555555YGP.~5BP55555PPPPPPPPPPPGGG!     .:   ~PJYGPGGGGGGGGGGGB####B#BGGGBGGGGGBBGGB#GGPJ~      ..........................."   ,
".      .      .................^JP5Y!:::.        ::PJ~5YY555555Y5G: 7YP?75555PPPPPPPPP5Y!~5G^          .YGGGGGGGGGGGBBBB#####BB##BGGGGGGGGGGGGBGG5?^................................"   ,
"....       ......................^?5PJ!:..      .:5P?5Y!JJ~~YYY!5~  77J7.:::55?J7J?5PPYY?JPGG:..........:?GGGGGGGGGGGBGGGGGGGGGGB##BBBBGGGGGGGGGGGJ?................................"   ,
"....................................~JP5!:......^YG5P555Y~.:^^!!7  .7!??. .:YP!7?55PGGGGBGGB#P..........~7JGBBBBBBBGGGGGGGGGGGGGGGBBBBBBBBBBBBBBGBP?!..............................."   ,
".................................... .:!YP5J?~:7YGPBG5555JYYYY^~.  .7~?Y7Y55PPPGGGGGGGGGBBB##&Y.::.:::^^.!??G#BGGGBGGGGGGGGGGGGGGGGBBGGGGGGGGBBGGBGY?~.............................."   ,
"....................................... .:^7JJJPPPGBBP5555555Y..   .7~75BBPPPPPPGGGGGGGGB####&G!^^^~!^^~::~^!PBGGGGGGGGGGGGGGGGGGGGGB#BGGGGGGGGGGGGPJ?.............................."   ,
"..........................................  ..~GGGBBBG5555555?     .7~?5BBBBBGGGGGGBBBBB##BB#&5~~^~7~:::::::^~YBBBGGGGGGGGGGGGGGGGGGGB#BBBGGGBBBBBBGP57............................."   ,
"..............................................YBBB##BGPPPPPPP!:. ...!!75B#####BBB##BBBBB##BBB#Y:.:^!?~^^^^:::~^?BBBBBBBBGGGGGGGGGGGGGGBBB#BBBBBBBBBBGP5^............................"   ,
"............................................ ~##GGGGBGGGGGGGP77::..:~7^7B#####BBB##BBBBB##G#B#?:  :~!:.:^^^^^^7:!G###BBBBBBBGBBBBBBBBBBBBBB###BBBBBBBGPY............................"   ,
"........................................... .B&BPPPPPGGGGGGGG!7.:::^~!::PBBBGBBBBBBBBBBB##5###!    :7!  .~!~~~JJ~^JG###BBBB#BBBBBBBBBBBBBB#BBB####BBBBPG! .........................."   ,
"...........................................:Y#&BPGGPGGGGGGGGP:!..:.!~~: 7#BBBBBBBBBBBBBB#BYB#G^     :?.   :!!~!~7!^~YB####BBB####BBBBBBBBBB###BBB###BBB55..........................."   ,
".......................................:::!GG##GGGGGGGGGGGGGP:7::^^7~^.  P#BBBBBBBBBBBBBBPJG#J.      ^:     ^!!:.:~!~!YG##&&#########BBBBBBBB####BB##B#G57.........................."   ,
".................................:..:::::!PPG&BPGBGGGGGGGGBBG^?^^!~^~^   .G#BBBBBBBBBGGGGY?PB.        .      :?.   .^~?7?5B&&@&&&&&&&&&&#####B##########55:........................."   ,
".......................    ....... .^^:!5PPPB#GGGBGGGGBBBBBBG^?~~?!.::    ^BBBBBGGGGGGPPP??GJ        ..:~7J5PPBG5Y?^.:5&GJ?YG#&&&&&&&&&&&&&&&&&&##&&&###B5J........................."   ,
"......................    ....    .::^?PPPPPBBGGBBGBBBBBBB#B5:JJ7Y!.:^     ^PBGGGGPPGGPPY?75~     .~?Y5B&&&&&&&&@@@@&7:#@#BG5YPB&&&####&&&&&&&&&&&&&&####P5!........................"   ,
"....................     ...     ^:^77^^~!~!5GGB#BBBBBBBBB&#Y^?7J5~  .      :!PGPPP55P55J?^!^   .7BP7:.P&&#BGBGGB#B#@: 7@&####BBBB######BBB#############BPPP^ ......................"   ,
".....................    ::....:!Y?~.  .^^!5GG#&&BBBBBBBB#&#J~!^^J~           .JP555YYYY?7.::   :J!.   :7YJJY#J7!~!G!  .&&##############################G5GPG: ....................."   ,
".....................    . .^!?YY~.   :^7YPPG##&#BBB##BB#&&B7~~:  :             ~??JJ?7?7:      .. .    Y~:^:....^~:   .#&##############################BBBBGG: ...................."   ,
"......................  .     ..    .:~YGGGB#B#&BBBB####&&&G~!^   :7Y55GGBGP5!.   :~777!!.              :?:.   ...     ~#&######&#&&############GB##########BGG: ..................."   ,
"...................... .   .      .:^^!Y5PGGPG##BBB####&&&#Y~!  7B#GJ7##BBBGBBY.    :~77~                .:.           Y########&#&&###########BGG###########BBB~ .................."   ,
".......................           .      .:^~^JBBB#&#&&@@&B!7#!~#@7   ^JB57YY!?~      .:^                             !G&####&##&#B&&############BB###########B#B7.................."   ,
".......................               ...:^^^^GBB#&&&&@@&#5~B@@5.:~:.  7P..:: .~                                     !P&B###&##&&#B&&############################BJ................."   ,
"....:..................               .?PPG5^YBB#&&&@@@@&B!Y@&&&P       .!:. ...                                   :!75G&&&&#B#&#B#&&&#B############&########BBB#&#5^..............."   ,
"....:...................               :B##GJBB#&&@@@&&&&Y7&&&#&#G:       ..                                     ...:YPB###BB#&##B&&&&&BBB##B#######&&##BB##BBBBB#&#G?:............."   ,
".....:...................               J###BB&&&@@@&&&&BJ&@&&#BBBG:                                               ^?JY5GBB#&&&#B#&#&&&#BB##BBBB##B#&&&##BB##BBBB###&#P7:. ........."   ,
".....:^::......::::^~~7?5~              .P&#&&&&&&@&&&&#G&@&&&#BBBGP:                                            .~!7?5G##&&&&#B#&&#&&####&&##BB##B&&&&&&##B###BBB#BGB##GJ~........."   ,
".......:^^~?YYYYYY55PGGGGG~              ~B&&&&&@@&&&&&&&@@&&##BBBGGP~.                                       .:^~!?YGG#&&&&BPB#&&BB&&B#&#&&##BB###&&&&&&&##BB#######GPGBGP5?~:....."   ,
".........::~?Y555PPGGGBBBBB.             :~B&&@@@&&&&@@@&&@&&&#BBBGGGG?~:.                                    .....^JJG&&&B55GG55PGB&&B#BB#&&&#B###&#&&&&&&###BB####&##G5J7!7???7!~^"   ,
"..............^~?YPGBBGGGGG?             .^?&@@&&&&&@@&&&&&@@&&#BBBGG5?^                                          :7JB&#G5Y5YJ?????JYPG#BB####&&###BB#&&&&&5?GBBBB####&&#P5J7~^:...."   ,
"...........:~?YPGGP555YY55Y5^.           .^?&@@@@&&&&&&&&&&&&&&&#BBGP5YJ!^.                                     .!5P##PYJJ??7777777!!?G#BB#BGB&&##BBG&&&&&&&^.!YPPGB##B##&#BPYJ?7!^:"   ,
".........^?5PGGPYJJJ?~..:!JJ~^^:.     .::^!B@&@&#B&&&&&&&&&&&&&&&&#BBP5Y?77!!~^:.                             :^~5GG5J???777!~~~!7!~^7PBGGBGYG##BBBBB&&&&&&&#. .:~!?J55PPPGGGG5YJJJJ"   ,
"......:^^JGP5YJ??JJ~       ..:^~~~^^^~~~!!7B&@&GG&@&&&&&&&&&#&&&&&&&&##BGG5JY!:                            .:^^^~!~7J7!!~^^^~~!7?7!!7?GGGGBPJGBGGBBB#&&##&&&&B: ......::^~~~!!7777!!"   ,
".....^^~!YJ7!7??J?.             .:^~!!~7!.Y#@#PP&@&&&@#&@&&#G#@@&BBGPPGGGP5YYYJ?!:.                     ..:^::::::~?!^::^~~~^^^^~7??JJGGGGB?YBGGGBBB&&&#BB#&&@#!. ......   ........."   ,
"...:^~7?7!!!!!77~.              ....:^!~.~G&#55#@&&&@&B&@&#GPPBGY?7!!~~~!!!77??????7!~:.             ..:::::...:^~!:.  ...  ......:~?5GGGB5!PGGGGBGB&&&&BBB#&&&&G^.   ....^J^ ......"   ,
"..:^!?7!!!!!!!!.               ..:::::::^YG#55P&&&&@&B#@&BBPJ??7!~^^::::::::^~~^^~~!!777!!^:.    ..::::::.....:^^:.                ~YGGGGP7?GGGGBBB&&&&&B#####&&&&GY?7?JY5PJ:::~!..."   ,
".:~J?!!!!!!~~^.                ...::^^^^^?PP55#@&@@@&G#@&GGGPJ7~^^^^^~~^:.  ..^^^^^^^^~~~~!!^^:::::::^^..   .:.                   ^YPGGG!!?5BGGGBB&@&&&#BB##&&&&&&&&&&#BGPY7777^...."   ,
":!J7!!!!~^:..                   ...::^^::PP555&@@@&@&GB@&PPG?!~~~!!7~:.       .::^^:::::....:^:.    ::     ..                    :J5GGY~:7JGGGGBB?7B@@&#BBB#&&&&&&&&&&#BBGY7~:......"   ,
"!J!!!~:....                     ...::^^.:5P55P&&@&&@&GG&&PPJ!!!!!7~.                        .       .                           ~YPPY!^^~J5GGBBP!::~&@@&&&#####&&&&&&&&&&&&G?^. ...."   ,
"?!!^......                      ...::^^.:!P555&@&B&@@#PB@#Y~~~~!7^                                                            ^?J?^..:^^?5BBGY7~~?P&@@&&&&&######&&&&&&&&&&@@&#J:..."   ,
"7!:........                     ...::^::..YP55&@GB@&@&BP#B^:^~!7~.   .                                                    .:~!~:....:^:!55YJ?!~^^~5&@@&&&&&&&&&&&&&&&&&&&&&&&&&@&5:."   ,
"7~.........~7:                  ...::^:: .YGP5G&5G&&&@@BY~^!!!7!^..:.              .                           .               ..:::^^^!!~~~77?G##&&&&&@@@@@@&&&&&&&&&&&&&&&&&&&&#P!"   ,
"J^........^5#BP!.              ...::::.::?PPPPPB5P&@@&@G7!?7^^!^:::.              ..                           .:.. ....     .:^!!!!~~~~~~~~^~!J#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&BGG5"   ,
"^:.......^5BGGGGPY!:           ..::::.7JY5YJY5PGP5B@&&@@&J^:^7^::^:        ...   .:   .                        .:^~~^^~^^:::::^^~77?7!~^^^^^^^^^!#&&&&&&&&&&&&&&&&&&&&&&&&#B&&&#P5PP"   
, NULL};
/// <summary>
/// Анимированная картинка в консоли
/// </summary>
void animatedNeko() {
    FILE* f; int count = 1;
    while (1) {
        for (int i = 0; i < 11; i++) {
            char url[256] = { 0 };
            sprintf(url, "bakemonogatari-monogatari/banner (%d).txt", i);
            f = fopen(url, "r"); char a[195]; ;
            while (fgets(a,195,f) != NULL)
            {
                printf("%s", a);
            }
            Sleep(30);
            COORD positionCur = { 0,0 }; //позиция x и y
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleCursorPosition(hConsole, positionCur);
        }
        count++; 
        if (count >= 20) return;
    }
}
/// <summary>
/// Картинка в консоли
/// </summary>
/// <param name="Password">Пароль для вывода</param>
void neko(int Password) {
    if (Password == 34097) {
        clear();
        for (int i = 0; BMP1[i] != NULL; i++) {
            printf("%s\n", BMP1[i]);
        }
    }
}

void _print_bakground(int _window_w,int _window_h)
{
    clearf(); char c =  rand() % (47 - 33 + 1) + 33;
    srand(time(NULL));
    for (int i = 0; i < _window_h; i++)
    {
        for (int j = 0; j < _window_w; j++) {
            printf("%c", c);
            c= rand() % (47 - 33 + 1) + 33;
        }
        printf("\n");
    }
    _print_border(_window_w, _window_h);
}

void _print_border(int _window_w, int _window_h) {
    COORD positionCur = { _otstup,_interval }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int y = _interval; y <= _window_h - _interval; y++) {
        SetConsoleCursorPosition(hConsole, positionCur);
        if(y == _interval)  printf("┌");
        else if (y == _window_h - _interval)
            printf("└");
        else 
        printf("│");
       
        for (int x = _otstup+1; x < _window_w - _otstup; x++) {
            if ((y == _interval) || (y == _window_h - _interval)) printf("─"); else  printf(" ");
        } 
        if (y == _interval)  printf("┐");
        else if (y == _window_h - _interval)
            printf("┘");
        else
            printf("│");
        positionCur.Y++;
    }
}


//
//puts("┌──────┬─────┬────────────────────┬────────────┬───────────────┬──────┬──────────┬───────────┬───────┐");
//puts("│Индекс│Номер│ ФИО                │Год рождения│Год поступления│Физика│Математика│Информатика│История│");
//
//for (int i = 0; i < MaxIndex; i++)
//{
//    puts("├──────┼─────┼────────────────────┼────────────┼───────────────┼──────┼──────────┼───────────┼───────┤");
//    printf("│%6d│%5d│%-20s│%-20s│%-10.2f│%-13d│\n", st[i]._Index, st[i]._Number, st[i]._FIO, st[i]._God, st[i]._GodPos, st[i].marks.Fizika);
//}
//puts("└──────┴─────┴────────────────────┴────────────────────┴──────────┴─────────────┘");