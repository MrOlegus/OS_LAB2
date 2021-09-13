#include <windows.h>
#include <ctime>

#define col 5
#define row 5
#define text 11
LPCWSTR values[text] = {L"Текст",
                        L"Маленький текст",
                        L"Еще один текст", 
                        L"Почему бы не сделать такой текст", 
                        L"Текст маленький незаурядный", 
                        L"Очень и очень и очень и очень и очень большой текст", 
                        L"Текст со знаками - препинания.", 
                        L"Т е к с т о т д е л ь н ы м и б у к в а м и", 
                        L"ОдноБольшоеСлово", 
                        L"Some English Text", 
                        L"Another English Text"
                       };
int random[col][row];

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR pCmdLine, int nCmdShow) {
    srand(time(0));
    for (int i = 0; i < col; i++)
        for (int j = 0; j < row; j++)
            random[i][j] = rand() % text; // случайным образом выбираем текст для ячеек

    TCHAR szClassName[] = L"Мой класс"; // строка с именем класса
    HWND hMainWnd; // создаём дескриптор будущего окошка
    MSG msg; // создём экземпляр структуры MSG для обработки сообщений

    WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
    wc.cbSize = sizeof(wc); // размер структуры (в байтах)
    wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
    wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
    wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
    wc.lpszClassName = szClassName; // указатель на имя класса
    wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
    wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // дескриптор кисти для закраски фона окна
    wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
    if (!RegisterClassEx(&wc)) {
        // в случае отсутствия регистрации класса:
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL; // возвращаем, следовательно, выходим из WinMain
    }

    // Функция, создающая окошко:
    hMainWnd = CreateWindow(
        szClassName, // имя класса
        L"Полноценная оконная процедура", // имя окошка (то что сверху)
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, // режимы отображения окошка
        CW_USEDEFAULT, // позиция окошка по оси х
        NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
        CW_USEDEFAULT, // ширина окошка
        NULL, // высота окошка (раз дефолт в ширине, то писать не нужно)
        (HWND)NULL, // дескриптор родительского окна
        NULL, // дескриптор меню
        HINSTANCE(hInst), // дескриптор экземпляра приложения
        NULL); // ничего не передаём из WndProc
    if (!hMainWnd) {
        // в случае некорректного создания окошка (неверные параметры и тп):
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }

    ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
    ShowScrollBar(hMainWnd, SB_BOTH, FALSE); // скрыть скроллбары
    UpdateWindow(hMainWnd); // обновляем окошко

    LPCWSTR lpString = L"!@#";
    GetWindowTextW(hMainWnd, NULL, 10);

    while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        TranslateMessage(&msg); // интерпретируем сообщения
        DispatchMessage(&msg); // передаём сообщения обратно ОС
    }

    return msg.wParam; // возвращаем код выхода из приложения
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        SelectObject(hDC, CreatePen(PS_SOLID, 3, RGB(0, 255, 0)));
        SelectObject(hDC, GetStockObject(NULL_BRUSH));

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int cellW = (clientRect.right - clientRect.left) / col;
        int sumH = 0;

        for (int i = 0; i < row; i++) {
            int maxRowH = 0;
            for (int j = 0; j < col; j++) {
                RECT cell = { j * cellW, sumH, (j+1) * cellW + 1, clientRect.bottom };
                LPCWSTR line = values[random[i][j]];
                int cellH = DrawText(hDC, line, -1, &cell, DT_WORDBREAK | DT_CENTER);
                Rectangle(hDC, cell.left, cell.top, cell.right, cell.bottom);
                if (cellH > maxRowH) maxRowH = cellH;
            }
            sumH += maxRowH;
        }

        SelectObject(hDC, GetStockObject(WHITE_BRUSH));
        Rectangle(hDC, 0, sumH, clientRect.right - clientRect.left, clientRect.bottom);

        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_SIZE:
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}