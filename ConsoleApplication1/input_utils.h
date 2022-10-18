#ifndef INPUT_UTILS
#define INPUT_UTILS


/// <summary>
/// Ввод любых строковых данных
/// </summary>
/// <param name="input_buff">Буфер для ввода</param>
/// <param name="buff_size">Размер буфера</param>
/// <param name="mode">Режим работы, от которого зависят допустимые символы.  0 - разрешение на ввод всего, кроме спец символов, 1- для реализации ввода Фамилии\имени\Отчества\Издательства(не допускаются цифры и т.д),2 - Ввод инициалов</param>
/// <returns></returns>
int input_string(char* input_buff, int buff_size, int mode);



enum WorkingMode
{
    NORMAL = 0,
    PERSONAL = 1,
    INICIAL = 2
};

#endif // !input_utils


#ifndef KEYCODE
#define KEYCODE
enum KeyboardCodes
{
    KEY_ARROW_UP = 72,
    KEY_ARROW_DOWN = 80,
    KEY_ARROW_LEFT = 75,
    KEY_ARROW_RIGHT = 77,
    KEY_TAB = 9,
    KEY_HOME = 71,
    KEY_END = 79,
    KEY_ENTER = 13,
    KEY_ESC = 27,
    KEY_DEL = 83,
    KEY_BACKSPACE = 8
};
#endif // !1
