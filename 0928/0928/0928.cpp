// 0928.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0928.h"

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
    LoadStringW(hInstance, IDC_MY0928, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0928));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0928));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0928);
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

/// 나, 상대, 음식에 대한 자료형 선언
RECT g_me, g_you, g_food;       /// RECT 자료형은 겹침 처리가 편함

/// 객체의 크기를 상수 선언
#define RECT_SIZE           50

/// 게임 상태 정보를 보관할 변수 선언
int g_in_game = 1;      /// 0 - 게임 오바, 1 - 게임 중인 상태
/// 상대의 속도를 제어하기 위한 변수 선언
int g_speed = 1000;     /// 게임 시작 시, 1초에 한번 움직이도록 구성
/// 게임 점수를 누적하기 위한 변수 선언
int g_score = 0;
/// 게임 시간을 보관하기 위한 변수 선언
int g_gametime = 10;    /// 초 단위. 초기 값은 10초
/// 게임의 라이프를 관리할 변수 선언
int g_life = 3;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    /// 내 객체의 이동 처리
    case WM_KEYDOWN:
    {
        /// 게임 오버 상태 여부를 확인하는 코드
        if (0 == g_in_game)
            break;      /// 게임 오버라면, 키 입력을 처리하지 않는다.

        switch (wParam)
        {
        case VK_LEFT:
        {
            g_me.left -= 10;
            g_me.right -= 10;
        }
            break;
        case VK_RIGHT:
        {
            g_me.left += 10;
            g_me.right += 10;
        }
            break;
        case VK_UP:
        {
            g_me.top -= 10;
            g_me.bottom -= 10;
        }
            break;
        case VK_DOWN:
        {
            g_me.top += 10;
            g_me.bottom += 10;
        }
            break;
        }   /// switch( wParam )
        /// 사용자의 키 입력(좌표 변경)을 모두 처리한 상태

        /// 음식을 먹었는지 확인
        RECT tmp;
        if (IntersectRect(&tmp, &g_me, &g_food))
        {
            /// 점수 증가
            g_score += 100;
            /// 시간 변경
            g_gametime += 3;

            /// 참이라면 -> 음식을 먹었다! 상태
            g_food.left = rand() % 450;
            g_food.top = rand() % 450;
            g_food.right = g_food.left + RECT_SIZE;
            g_food.bottom = g_food.top + RECT_SIZE;
            /// 상대의 속도를 빠르게 변경
            KillTimer(hWnd, 1);                 /// 1번 ID 타이머를 세움

            /// 값을 보정해야 하는 경우의 처리
            /// 1. 일단 뺀다.
            g_speed -= 100;
            /// 2. 최소 값보다 적어졌는지 확인
            if (g_speed <= 100)
            {
                /// 최소 값보다 작아진 것이 참이라면~
                g_speed = 100;
            }
            SetTimer(hWnd, 1, g_speed, NULL);   /// 1번 타이머를 다시 시작
        }

        /// 화면을 다시 그리기를 OS에게 요청
        InvalidateRect(hWnd, NULL, TRUE);
    }
        break;

    case WM_TIMER:
    {
        /// ID 값 확인
        if (1 == wParam)
        {
            /// 상대방의 이동 코드 작성
            if (g_me.left < g_you.left)
            {
                /// 참이라면~ 나는 상대의 왼쪽에 위치
                g_you.left -= 10;
                g_you.right -= 10;
            }
            else
            {
                g_you.left += 10;
                g_you.right += 10;
            }

            if (g_me.top < g_you.top)
            {
                /// 참이라면~ 나는 상대보다 위에 위치
                g_you.top -= 10;
                g_you.bottom -= 10;
            }
            else
            {
                g_you.top += 10;
                g_you.bottom += 10;
            }
            /// 나를 기준으로 상대가 좌표를 변경 완료한 상태

            /// 나를 잡았는지 확인
            RECT tmp;
            if (IntersectRect(&tmp, &g_me, &g_you))
            {
                /// 라이프 개수의 감소
                g_life--;
                if (0 == g_life)
                {
                    /// 게임 오버를 설정
                    g_in_game = 0;
                    KillTimer(hWnd, 1);
                    KillTimer(hWnd, 2);
                }
                else
                {
                    g_you.left = 500;
                    g_you.top = 400;
                    g_you.right = g_you.left + RECT_SIZE;
                    g_you.bottom = g_you.top + RECT_SIZE;
                }
            }

            /// 화면 다시 그리기 요청
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (2 == wParam)
        {
            /// 게임 시간을 측정하기 위한 타이머가 도착한 경우
            g_gametime--;
            if (0 == g_gametime)
            {
                g_in_game = 0;
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                /// OS에게 화면 전체를 다시 그리도록 요청
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
    }
        break;

    /// 클래스의 생성자와 같이 프로그램 실행 시에 단 한번만 호출
    case WM_CREATE:
    {
        /// 랜덤 사용을 위한 시드 값 설정
        srand(time(NULL));

        /// 상대방을 움직이도록 만들기 위한 타이머 설정
        ///     -> WM_TIMER가 주기적으로 발생
        /// ID가 1이면 상대 좌표 변경을 위한 타이머
        SetTimer(hWnd, 1, 1000, NULL);
        /// ID가 2라면 게임 시간을 측정하기 위한 타이머
        SetTimer(hWnd, 2, 1000, NULL);

        /// 객체들의 초기화 수행
        g_me.left = 10;
        g_me.top = 10;
        g_me.right = g_me.left + RECT_SIZE;
        g_me.bottom = g_me.top + RECT_SIZE;

        g_you.left = 500;
        g_you.top = 400;
        g_you.right = g_you.left + RECT_SIZE;
        g_you.bottom = g_you.top + RECT_SIZE;

        g_food.left = 250;
        g_food.top = 250;
        g_food.right = g_food.left + RECT_SIZE;
        g_food.bottom = g_food.top + RECT_SIZE;
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
            WCHAR buf[128] = { 0, };        /// 문자열 버퍼 선언
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            HBRUSH myBrush, osBrush;

            /// 0. 그라운드 그리기
            Rectangle(hdc, 10, 10, 500, 500);
            /// 1. 나의 객체 그리기
            myBrush = CreateSolidBrush(RGB(10, 120, 200));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);
            /// 문자열 출력 API
            TextOut(hdc, g_me.left + 10, g_me.top + 20, L"ME", 2);
            
            /// 2. 상대의 객체 그리기
            myBrush = CreateSolidBrush(RGB(200, 120, 60));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);
            TextOut(hdc, g_you.left + 10, g_you.top + 20, L"YOU", 3);

            /// 3. 음식 객체 그리기
            myBrush = CreateSolidBrush(RGB(rand()%256, rand()%256, rand()%256));
            osBrush = (HBRUSH)SelectObject(hdc, myBrush);
            Ellipse(hdc, g_food.left, g_food.top, g_food.right, g_food.bottom);
            SelectObject(hdc, osBrush);
            DeleteObject(myBrush);

            /// 4. 게임 상태를 화면에 표시
            if (g_in_game == 1)
            {
                TextOut(hdc, 500, 20, L"게임 중", 4);
            }
            else
            {
                TextOut(hdc, 500, 20, L"게임 오바", 5);
            }

            /// 5. 게임에서 나의 점수를 출력 -> 숫자 값을 문자열로 변환하고 싶다..
            /// printf( ) -> 화면 출력 : printf( "x[ %d ]", ab );
            /// sprintf( ) -> 문자열에 출력 : sprintf( buf, "x[ %d ]", ab );
            wsprintf(buf, L"나으 현재 점수 : %d", g_score);
            TextOut(hdc, 500, 40, buf, lstrlen(buf));

            /// 6. 게임 시간 정보를 출력
            wsprintf(buf, L"게임 시간 : %d", g_gametime);
            TextOut(hdc, 500, 60, buf, lstrlen(buf));
            Rectangle(hdc, 500, 80, 500 + (g_gametime * 20), 100);

            /// 7. 게임의 라이프를 출력
            wsprintf(buf, L"나으 라이프 : %d", g_life);
            TextOut(hdc, 500, 100, buf, lstrlen(buf));

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
