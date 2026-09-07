// 0907.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0907.h"

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
    LoadStringW(hInstance, IDC_MY0907, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0907));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0907));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0907);
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
/// 마우스의 이전 좌표 값
int px = 0, py = 0;

/// 플래그 변수
/// 프로그래밍에서 특정 상태나 조건이 충족되었는지 여부를 기록하고 확인하기 위해 사용하는 변수
/// BOOL 타입의 변수는 int로 사용하는 것이 더 좋다
/// 패딩 : 메모리 공간의 정렬 단위에 맞춰 빈 공간(바이트)을 끼워 넣는 것
/// 프로세스의 상태 정보 : 준비(1), 실행(2), 대기(3), 종료(4)
int g_flag = 0;     /// 0 - 마우스를 누르지 않은 상태, 1 - 누른 상태


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_RBUTTONDOWN:
    //case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);

        /// 자료형 선언
        HBRUSH myBrush, osBrush;
        /// 1. 생성
        myBrush = CreateSolidBrush(RGB(100, 100, 100));
        /// 2. 교체
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);
        /// 3. 사용
        Rectangle(hdc, 200, 300, x, y);
        /// 4. 복원
        SelectObject(hdc, osBrush);
        /// 5. 해제
        DeleteObject(myBrush);

        ReleaseDC(hWnd, hdc);
    }
        break;

    case WM_LBUTTONDOWN:
    {
        /// 사용자가 마우스를 누른 상태로 변경
        g_flag = 1;
        px = LOWORD(lParam);
        py = HIWORD(lParam);
    }
        break;

    case WM_LBUTTONUP:
    {
        /// 사용자가 마우스를 누른 상태를 뗀 상태
        g_flag = 0;
    }
        break;

    case WM_MOUSEMOVE:
    {
        if (0 == g_flag)
            break;

        /// 마우스의 위치 좌표 값을 획득
        /// A, B, C, D, ... , X,  Y,  Z
        /// 1, 2, 3, 4, ... , 24, 25, 26
        /// X(24) < Y(25) : 값이 낮기 때문에....
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        /// 화면 그리기를 하려면, 반드시 획득해야 하는 것!!! HDC
        HDC hdc = GetDC(hWnd);

        /// myPen : 내가 생성할 펜을 가리킬 핸들
        /// osPen : OS의 펜을 보관할 백업 용도의 핸들
        HPEN myPen, osPen;

        /// 1. 펜을 생성
        myPen = CreatePen(PS_SOLID, 7, RGB(0, 255, 0));
        /// 2. 펜을 교체
        /// HGDIOBJ가 상위 클래스, HPEN은 상속 받은 클래스
        osPen = (HPEN)SelectObject(hdc, myPen);

        /// 3. 선 그리기 : 사용
        MoveToEx(hdc, px, py, NULL);
        LineTo(hdc, x, y);

        /// 좌표 값 변경 : 현재 좌표는 다음 시점에 이전 좌표 값이 된다!
        px = x;
        py = y;

        /// 4. 사용 완료 ==> 원래 OS 펜으로 복원
        SelectObject(hdc, osPen);

        /// 5. 메모리 해제 요청
        DeleteObject(myPen);
        /*
        /// 1. 새로운 펜을 생성
        myPen = CreatePen(PS_DASHDOT, 7, RGB(66, 95, 200));
        /// 2. 교체
        osPen = (HPEN)SelectObject(hdc, myPen);
        /// 3. 사용
        MoveToEx(hdc, 1200, 700, NULL);
        LineTo(hdc, x, y);
        /// 4. 복원
        SelectObject(hdc, osPen);
        /// 5. 해제
        DeleteObject(myPen);
        */



        /// 디바이스 컨텍스트 해제
        ReleaseDC(hWnd, hdc);
    }
        break;


    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
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
            EndPaint(hWnd, &ps);
        }
        break;
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
