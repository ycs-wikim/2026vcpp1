// 0921.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0921.h"

#include <stdlib.h>
#include <time.h>


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
    LoadStringW(hInstance, IDC_MY0921, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0921));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0921));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0921);
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

/// 나의 객체, 적의 객체, 음식 객체 선언
RECT g_me, g_you, g_food;       /// 전역 변수 -> 자동 초기화 : 0으로

void userInputProcessor(WPARAM wParam)
{
    switch (wParam)
    {
    case VK_LEFT:           /// 왼쪽 방향키
    {
        g_me.left -= 10;
        g_me.right -= 10;
    }
    break;
    case VK_RIGHT:          /// 오른쪽 방향키
    {
        g_me.left += 10;
        g_me.right += 10;
    }
    break;
    case VK_UP:             /// 위쪽 방향키
    {
        g_me.top -= 10;
        g_me.bottom -= 10;
    }
    break;
    case VK_DOWN:           /// 아래쪽 방향키
    {
        g_me.top += 10;
        g_me.bottom += 10;
    }
    break;
    }       /// switch(wParam)
}


#define RECT_WIDTH  50


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
    {
        if (1 == wParam)         /// ID 값 확인
        {
            if (g_me.left < g_you.left)  /// you는 me의 오른쪽에 위치함
            {
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else     /// you는 me의 왼쪽에 위치함
            {
                g_you.left += 10;
                g_you.right += 10;
            }

            if (g_me.top < g_you.top)
            {
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }

            /// 적이 나를 잡았는지를 확인하는 코드
            RECT inc;
            if (IntersectRect(&inc, &g_me, &g_you))
            {
                KillTimer(hWnd, 1);
                MessageBox(hWnd, L"Game Over", L"잡혔다", MB_OK);
            }

            /// 적의 좌표 이동이 완료된 시점
            /// 화면 무효화를 통해 화면을 다시 그려야 한다!
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
        break;

    /// 생성자의 기능을 수행하는 윈도우 메시지
    case WM_CREATE:
    {
        /// 랜덤 시드 값을 현재 시간으로 항상 초기화
        srand(time(NULL));

        /// 타이머를 설정
        /// hWnd : 주기적인 메시지를 받을 윈도우
        /// ID : 타이머를 구분하기 위한 구분자. WM_TIMER 호출 시 wParam으로 전달
        /// Interval : 주기 설정. 숫자로 표현. 1000msec == 1초
        /// Func : 타이머를 받을 함수
        SetTimer(hWnd, 1, 50, NULL);

        /// 나의 객체 초기화
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = 60;
        g_me.bottom = 60;

        /// 적의 객체 초기화
        g_you.left = 500;
        g_you.top = 350;
        g_you.right = g_you.left + RECT_WIDTH;
        g_you.bottom = g_you.top + RECT_WIDTH;

        /// 음식의 객체 초기화
        g_food.left = 200;
        g_food.top = 300;
        g_food.right = g_food.left + RECT_WIDTH;
        g_food.bottom = g_food.top + RECT_WIDTH;
    }
        break;

    /// 사용자 입력을 통해 나의 객체 좌표 변경
    case WM_KEYDOWN:
    {
        userInputProcessor(wParam);
        /// 키보드 입력을 통해 객체의 좌표가 변경이 완료된 상황
        
        /// 게임 재시작
        if (VK_F3 == wParam)
        {
            SetTimer(hWnd, 1, 50, NULL);
        }

        /// 음식을 먹었는지 여부를 확인
        /// IntersectRect() : 2개의 RECT 자료형의 겹침이 있는지 확인
        ///         겹침이 있다면 TRUE가 반환, 없다면 FALSE가 반환
        /// Dst : 겹침이 있는 경우, 해당 겹침 좌표를 반환
        /// Src1, Src2 : 비교할 RECT 자료형
        RECT ir;
        if (IntersectRect(&ir, &g_me, &g_food))
        {
            /// 음식 위치를 랜덤으로 재배치
            g_food.left = rand() % 450;
            g_food.top = rand() % 450;
            g_food.right = g_food.left + RECT_WIDTH;
            g_food.bottom = g_food.top + RECT_WIDTH;
            //MessageBox(hWnd, L"먹었다", L"음식을", MB_OK);

            /*  /// 겹침이 발생한 경우, 해당 위치를 그려서 확인하는 코드
            HDC hdc = GetDC(hWnd);

            Rectangle(hdc, ir.left, ir.top, ir.right, ir.bottom);

            ReleaseDC(hWnd, hdc);
            MessageBox(hWnd, L"먹었다", L"음식을", MB_OK);
            */
        }


        /// 마지막 무효화 방법 : 프로그래머가 OS에게 요청하는 방법
        /// hWnd : 화면을 무효화할 윈도우
        /// NULL : RECT 자료형으로 무효화할 범위를 지정
        /// TRUE : 화면 무효화 시에 현재 상태를 유지(FALSE)하거나 지우는(TRUE) 설정
        InvalidateRect(hWnd, NULL, TRUE);
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
            /// 나의 객체를 화면에 그리는 코드
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            /// 음식 객체를 화면에 그리는 코드
            Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);
            /// 적의 객체를 화면에 그리는 코드
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);
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
