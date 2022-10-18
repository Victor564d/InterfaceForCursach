#ifndef INPUT_UTILS
#define INPUT_UTILS


/// <summary>
/// ���� ����� ��������� ������
/// </summary>
/// <param name="input_buff">����� ��� �����</param>
/// <param name="buff_size">������ ������</param>
/// <param name="mode">����� ������, �� �������� ������� ���������� �������.  0 - ���������� �� ���� �����, ����� ���� ��������, 1- ��� ���������� ����� �������\�����\��������\������������(�� ����������� ����� � �.�),2 - ���� ���������</param>
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
