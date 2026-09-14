// 0914.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0914.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0914, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0914));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0914));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0914);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

/// 콜백 함수의 특징으로 인해 전역 변수를 통해 좌표를 보관
/// 마우스가 눌린 위치의 좌표 값을 보관하기 위한 전역 변수 선언
int g_x, g_y;

/// 그리기 정보를 보관할 전역 플래그 변수 선언 : 종류 구분
int g_draw = 0;     /// 0-안그림, 1-선그림, 2-사각형그림, 3-타원그림, 4-자유선그림

/// 선의 굵기 값을 보관할 전역 플래그 변수의 선언 : 값을 보관
int g_px = 1;

/// 내 캐릭터의 위치 좌표를 보관하는 자료형
RECT g_me;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 클래스의 생성자와 동일한 역할을 수행하는 윈도우 메시지
    /// 클래스가 메모리에 적재되어 인스턴스화될때, 단 한번 자동으로 호출되는 함수
    /// 프로그램 적재 후에 OS에 의해 단 한번만 호출되는 윈도우 메시지
    case WM_CREATE:
    {
        /// 내 캐릭터의 위치 좌표 초기화 설정
        g_me.left = 10;         /// 좌상단의 x 좌표
        g_me.top = 10;          /// 좌상단의 y 좌표
        g_me.right = 160;       /// 우하단의 x 좌표
        g_me.bottom = 160;      /// 우하단의 y 좌표
    }
        break;

    case WM_KEYDOWN:
    {
        /// 오른쪽 방향키를 눌렀는지 확인
        if (wParam == VK_RIGHT)
        {
            g_me.left += 10;
            g_me.right += 10;
        }

        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    /// 마우스의 왼쪽 버튼을 누른 시점
    case WM_LBUTTONDOWN:
    {
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;

    /// 마우스의 왼쪽 버튼을 땐 시점
    case WM_LBUTTONUP:
    {
        /// 현재 위치 좌료 획득
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);

        /// 선의 굵기 변경을 위한 코드 추가
        /// 0. 자료형 선언
        HPEN myPen, osPen;
        /// 1. 펜을 생성한다.
        myPen = CreatePen(PS_SOLID, g_px, RGB(0, 0, 0));
        /// 2. 펜을 교체한다.
        osPen = (HPEN)SelectObject(hdc, myPen);

        /// 3. 사용한다.
        if (1 == g_draw)
        {
            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);

            MoveToEx(hdc, x, g_y, NULL);
            LineTo(hdc, g_x, y);
        }
        else if (2 == g_draw)
        {
            Rectangle(hdc, g_x, g_y, x, y);
        }
        else if (3 == g_draw)
        {
            Ellipse(hdc, g_x, g_y, x, y);
        }

        /// 4. 복원한다.
        SelectObject(hdc, osPen);
        /// 5. 해제한다.
        DeleteObject(myPen);

        ReleaseDC(hWnd, hdc);
    }
        break;

    /// 메뉴에 관련된 사용자 입력 윈도우 메시지
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            /// visual studio 버그로 변경된 ID 값이 즉시 적용되지 않는다.
            /// 그리기 종류를 구분하기 위한 메뉴 선택 정보 확인 시작
            case ID_DRAW_LINE:
                g_draw = 1;
                break;
            case ID_DRAW_RECTANGLE:
                g_draw = 2;
                break;
            case ID_DRAW_ELLIPSE:
                g_draw = 3;
                break;
            case ID_DRAW_FREELINE:
                g_draw = 4;
                break;
            /// 그리기 종류를 구분하기 위한 메뉴 선택 정보 확인 끝
            /// 선의 두께를 설정하는 메뉴 값 설정 시작
            case ID_LINE_1PX:
                g_px = 1;
                break;
            case ID_LINE_5PX:
                g_px = 5;
                break;
            case ID_LINE_10PX:
                g_px = 10;
                break;
            /// 선의 두께를 설정하는 메뉴 값 설정 끝

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

            EndPaint(hWnd, &ps);
        }
        break;
    /// 소멸자와 동일한 역할을 수행하는 윈도우 메시지
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
