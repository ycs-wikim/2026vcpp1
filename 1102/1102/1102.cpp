// 1102.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1102.h"

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
    LoadStringW(hInstance, IDC_MY1102, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1102));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1102));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1102);
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


/// 스레드 프로그래밍 기초 : 생성, 제어까지 진행
/// 멀티 스레드 프로그래밍 : 다수의 핸들을 보관하고 관리
///     문제점 : 동시에 다수 개의 흐름이 발생
///     동기화(Synchronization) 문제 : 공유 자원으로 인한 문제
///     공유 자원 접근에 문제가 발생하는데,
///     이 접근 문제를 해결하는 것이 동기화 문제 해결
/// 동기화 문제의 코드 예)
/// 
/// 공유자원 A가 선언;
/// ....
/// 
/// * 현 위치에서 각 스레드/프로세스의 경쟁 상태(Race Condition)가 발생
/// /////////////// 공유자원에 접근하는 코드 시작 : 임계 영역
///                     Critical Section
/// 공유 자원 A를 사용;
/// 
/// A를 접근;
/// 
/// 이후로는 공유 자원을 더 이상 사용하지 않는다.
/// /////////////// 공유자원에 접근하는 코드 종료 

/// 위의 동기화 문제를 해결하기 위한 방법론
/// 1. Test & Set (Hardware)
///     메모리 공간에 대한 접근을 하드웨어 레벨에서 관리
///     특정 메모리 공간에 대해 하드웨어가 관리
///     ==> 모든 프로그램의 공유자원에 대한 대응이 어려움
///     ==> 특정 메모리를 구성하는데 비용이 너무 비쌈!
/// 2. 데커 알고리즘 (Software)
///     특정 자원에 대한 접근을 코드로 표현
/// 3. Semaphore (OS)
///     개념적인 표현. 실제로 동작을 이렇게 하면 된다~
///     P( ), V( ) 연산
///     P( ) : wait
///     V( ) : signal

/// 스레드의 인수로 사용될 구조체 선언
typedef struct thread_exec_information
{
    /// 마우스 좌표 값 보관 멤버 변수
    LPARAM              lParam;
    /// 윈도우 핸들
    HWND                hWnd;

} TEINFO, *PTEINFO;

/// OS가 제공하는 동기화
/// 1. OS는 프로그램이 동기화가 필요한지 알 수 없다.
/// 2. 프로그래머가 동기화가 필요하다면, OS에 준비를 요청
///     ==> OS에게 동기화 객체 생성을 요구 => 자료형
///     ==> 생성은 단 한번만하면 된다.
///     ==> WM_CREATE에서 생성
/// 3. 프로그래머가 동기화 객체를 사용하는 위치 지정
///     ==> 현재 위치부터 임계 영역 시작을 알림
/// 4. 프로그래머가 동기화 해제 위치를 지정
///     ==> 현재 위치부터 임계 영역이 아님을 알림
/// 5. 동기화 객체의 해제를 요청
///     ==> 더 이상 동기화가 필요 없거나, 프로그램 종료
///     ==> WM_DESTROY에서 해제

/// 자료형 : CRITICAL_SECTION
/// 생성 API : InitializeCriticalSection( )
/// 임계 영역 진입 API : EnterCiriticalSection( )
/// 임계 영역 떠나는 API : LeaveCriticalSection( )
/// 동기화 객체 해제 API : DeleteCriticalSection( )

/// 동기화 객체 자료형 선언
CRITICAL_SECTION g_cs;


/// 동기화 문제를 유발하는 공유자원
int g_x;    /// 화면에 그림 그릴때, 반드시 x좌표로 사용
/// 데커 알고리즘 구현을 위한 변수 선언
///     ==> 확인을 하기 위한 변수가 또다른 공유자원
bool g_use = false; /// false : 사용 안함. true 사용중

/// OS의 동기화 객체를 이용하여 동기화 문제 해결 스레드
DWORD WINAPI pig2(LPVOID param)      /// PTEINFO
{
    PTEINFO ptr = (PTEINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        EnterCriticalSection(&g_cs);
        //// 임계 영역 시작 위치
        g_x = x;
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        //// 임계 영역 종료 위치
        LeaveCriticalSection(&g_cs);
        Sleep(30);          /// CPU 양보
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);          /// 스레드의 자원을 스스로 정리
    return 0;
}



/// 소프트웨어적으로 동기화 문제를 해결하는 스레드
DWORD WINAPI pig1(LPVOID param)      /// PTEINFO
{
    PTEINFO ptr = (PTEINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        /// 상호배제 : 원자적으로 동작해야 한다.
        while (false != g_use) {}     /// 노크~ 사용중??

        /// 아무도 사용하지 않는 상태! => 문 닫기
        g_use = true;
        ///////////////////////////// 임계 영역 시작
        g_x = x;    /// 공유자원 = 로컬자원
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        ///////////////////////////// 임계 영역 끝
        g_use = false;      /// 문 열기

        Sleep(30);          /// CPU 양보
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);          /// 스레드의 자원을 스스로 정리
    return 0;
}




/// 멀티 스레드의 기초 스레드 함수
DWORD WINAPI pig(LPVOID param)      /// PTEINFO
{
    PTEINFO ptr = (PTEINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, i);
        Sleep(30);          /// CPU 양보
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);          /// 스레드의 자원을 스스로 정리
    return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /// 동기화 객체 생성 요청
        InitializeCriticalSection(&g_cs);
    }
        break;

    case WM_LBUTTONDOWN:
    {
        /// 스레드에 전달할 구조체 선언과 정보 설정
        TEINFO te;
        te.hWnd = hWnd;
        te.lParam = lParam;
        CreateThread(NULL, 0, pig2, &te, 0, NULL);
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
    {
        /// 동기화 객체의 자원 해제 요청
        DeleteCriticalSection(&g_cs);
        PostQuitMessage(0);
    }
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
