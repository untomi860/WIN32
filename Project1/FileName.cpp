#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WindowProcess(HWND hWindow, UINT uMessage, WPARAM wParameter, LPARAM lParameter)
{
    HDC hDeviceContext;
    PAINTSTRUCT paintStruct;
    RECT rectPlace;
    HFONT hFont;

    static TCHAR text[50000];
    static int size = 0;
    static int sec = 0;
    static TCHAR workTime[10];

    switch (uMessage)
    {
    case WM_CREATE:
        ZeroMemory(text, sizeof(text));
        SetTimer(hWindow, 1, 1000, NULL);
        SetForegroundWindow(hWindow);
        break;
    case WM_PAINT:
        hDeviceContext = BeginPaint(hWindow, &paintStruct);
        GetClientRect(hWindow, &rectPlace);
        SetTextColor(hDeviceContext, RGB(0, 0, 0));
        hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY,
            VARIABLE_PITCH,
            TEXT("Arial"));
        SelectObject(hDeviceContext, hFont);
        if (size > 0)
        {
            DrawText(hDeviceContext, text, size, &rectPlace, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        EndPaint(hWindow, &paintStruct);
        break;
    case WM_CHAR:
        switch (wParameter)
        {
        case VK_RETURN:
            size = 0;
            break;
        case VK_BACK:
            if (size != 0)
                size--;
            break;
        default:
            if (size < sizeof(text) / sizeof(TCHAR))
            {
                text[size] = (TCHAR)wParameter;
                size++;
            }
            break;
        }
        InvalidateRect(hWindow, NULL, TRUE);
        break;
    case WM_TIMER:
        sec++;
        break;
    case WM_DESTROY:
        KillTimer(hWindow, 1);
        _itoa_s(sec, workTime, 10);
        MessageBox(NULL, workTime, _T("Время работы программы (сек.):"), MB_ICONASTERISK | MB_OK);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    TCHAR szClassName[] = TEXT("MyWindowClass");
    TCHAR szWindowCaption[] = TEXT("My Window");

    WNDCLASSEX wndClass;
    HWND hWnd;
    MSG msg;

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = 0;
    wndClass.lpfnWndProc = WindowProcess;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = szClassName;
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wndClass))
    {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hWnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        szClassName,
        szWindowCaption,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 480, 320,
        NULL, NULL, hInstance, NULL);

    if (hWnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}