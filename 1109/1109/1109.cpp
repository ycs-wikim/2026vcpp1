// 1109.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "1109.h"

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
    /// Mutex 객체로 생성된 이름이 있는지 확인
    HANDLE mux = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"유효한이름");

    if (NULL == mux)
    {
        /// mux == NULL : 못 찾았다. / 없다.
        //MessageBox(NULL, L"못 찾았다", L"뮤텍스", MB_OK);
        CreateMutex(NULL, TRUE, L"유효한이름");
    }
    else
    {
        /// mux != NULL : 찾았다! / 있다!
        //MessageBox(NULL, L"찾았다23487462823468234652348723478", L"뮤텍스", MB_OK);
        return 0;
    }


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1109, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1109));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1109));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1109);
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


/// 공유 자원 선언
int g_x;

/// 1. 윈도우 OS가 제공하는 동기화 객체 선언
///     ==> 무한대의 동시 작업 스레드가 공유 자원 접근을 제한
CRITICAL_SECTION g_cs;
/// 2. 윈도우 OS가 제공하는 동기화 객체 선언 : 세마포어
///     ==> 임계 영역에 진입하는 작업(스레드)의 개수 제한
HANDLE g_sem;
/// 3. 윈도우 OS가 제공하는 동기화 객체 선언 : 뮤텍스
///     ==> 글로벌 이름 공간에 등록된 이름을 바탕으로 다양한 작업 가능
HANDLE g_mux;


/// 공통 : 스레드로 전달할 구조체 선언
typedef struct thread_info
{
    LPARAM          lParam;
    HWND            hWnd;
} TINFO, *PTINFO;


/// 뮤텍스 스레드 함수
DWORD WINAPI pig_mux(LPVOID param)
{
    PTINFO ptr = (PTINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        WaitForSingleObject(g_mux, INFINITE);
        /////////////////////////// 임계 영역 시작
        g_x = x;        /// 공유 자원 접근 시작
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /////////////////////////// 임계 영역 종료
        ReleaseMutex(g_mux);
        Sleep(30);      /// CPU 양보 ==> 문맥 교환
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);  /// OS로부터 할당받은 자원을 해제하고 종료
    return 0;       /// 함수의 반환 값
}


/// 세마포어 위치 설정을 위한 변수 선언
int g_pos = 15;

/// 스레드 함수
DWORD WINAPI pig_sem(LPVOID param)
{
    PTINFO ptr = (PTINFO)param;
    int pos = g_pos;            /// pos = 15
    g_pos += 15;                /// g_pos = 15 + 15 = 30
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);
    WCHAR buf[128] = { 0, };

    HDC hdc = GetDC(ptr->hWnd);

    WaitForSingleObject(g_sem, INFINITE);
    /////////////////////////// 임계 영역 시작
    for (int i = 0; i < y; i++)
    {
        /*
        wsprintf(buf, L"%d%% : ", i + 1);
        for (int j = 0; j < i; j++)
        {
            lstrcat(buf, L"|");
        }
        TextOut(hdc, 10, pos, buf, lstrlen(buf));
        */
        g_x = x;        /// 공유 자원 접근 시작
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        Sleep(30);      /// CPU 양보 ==> 문맥 교환
    }
    /////////////////////////// 임계 영역 종료
    ReleaseSemaphore(g_sem, 1, NULL);

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);  /// OS로부터 할당받은 자원을 해제하고 종료
    return 0;       /// 함수의 반환 값
}


/// 스레드 함수
DWORD WINAPI pig_cs(LPVOID param)
{
    PTINFO ptr = (PTINFO)param;
    int x = LOWORD(ptr->lParam);
    int y = HIWORD(ptr->lParam);

    HDC hdc = GetDC(ptr->hWnd);

    for (int i = 0; i < y; i++)
    {
        EnterCriticalSection(&g_cs);
        /////////////////////////// 임계 영역 시작
        g_x = x;        /// 공유 자원 접근 시작
        MoveToEx(hdc, g_x, 0, NULL);
        LineTo(hdc, g_x, i);
        /////////////////////////// 임계 영역 종료
        LeaveCriticalSection(&g_cs);
        Sleep(30);      /// CPU 양보 ==> 문맥 교환
    }

    ReleaseDC(ptr->hWnd, hdc);

    ExitThread(0);  /// OS로부터 할당받은 자원을 해제하고 종료
    return 0;       /// 함수의 반환 값
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        /// 스레드에게 전달할 구조체 변수 선언
        TINFO ti = { 0, };
        /// 스레드 동작에 필요한 정보 입력
        ti.hWnd = hWnd;
        ti.lParam = lParam;
        /// 1. Critical Section 스레드 생성
        //CreateThread(NULL, 0, pig_cs, (LPVOID)&ti, 0, NULL);
        /// 2. Semaphore 스레드 생성
        CreateThread(NULL, 0, pig_sem, (LPVOID)&ti, 0, NULL);
        /// 3. Mutex 스레드 생성
        //CreateThread(NULL, 0, pig_mux, (LPVOID)&ti, 0, NULL);
    }
        break;

    case WM_CREATE:
    {
        /// 1. 임계 영역의 초기화
        InitializeCriticalSection(&g_cs);
        /// 2. 세마포어 객체 초기화
        /// 1번째 인수 : 보안 속성
        /// 2번째 인수 : 초기 값 설정 - 사용가능한 자원의 수
        /// 3번째 인수 : 최대 값 설정 - 최대 자원의 개수
        /// 4번째 인수 : 이름
        ///     ==> 내 프로세스 내부에서만! 유효한 이름
        g_sem = CreateSemaphore(NULL, 5, 5, NULL);
        /// 3. 뮤텍스 객체 초기화
        /// 1번째 인수 : 보안 속성
        /// 2번째 인수 : 소유자 설정
        /// 3번째 인수 : 이름
        ///     ==> 내 시스템 내부에서 유효한 이름
        g_mux = CreateMutex(NULL, FALSE, NULL);
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
        /// 1. 임계 영역의 자원 해제
        DeleteCriticalSection(&g_cs);
        /// 2. 세마포어 자원 해제
        CloseHandle(g_sem);
        /// 3. 뮤텍스 자원 해제
        CloseHandle(g_mux);

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
