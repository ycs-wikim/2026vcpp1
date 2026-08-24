// 0824.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"          /// 자동으로 VS가 현재 시스템에 맞게 작성한
                                /// 헤더 파일들이 포함된 헤더 파일
#include "0824.h"               /// 자원 관련된 상수 정보

/// 상수 선언
#define MAX_LOADSTRING 100

/// 윈도우 자료형의 특징
///     1. 대문자 H로 시작하는 자료형은 핸들(HANDLE)을 나타낸다.
///         핸들 : 자원은 OS가 가지고 있고, 사용자가 직접 접근하지 못할때
///             해당 자원을 이용한 여러가지 처리를 요청할 때 필요한 정보(포인터)
///     2. 모두 대문자로 구성되어 있다.
///     3. WND == window
///     4. W가 붙어 있다면, 유니코드 지원 함수이다.
///     5. EX 또는 Ex가 붙어 있다면 확장(Extension)되어 있다는 의미


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
                    /// 나의 정보를 가리키는 핸들(this)
// char szT[100];
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/// Window 프로그램의 main 함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                    /// OS가 메모리에 잘 적재한 다음 해당 정보를
                    /// 나중에 사용할 수 있도록 내 프로그램에 전달
                     _In_opt_ HINSTANCE hPrevInstance,
                    /// 이전에 실행된 나와 같은 프로그램의 인스턴스 핸들
                    /// code/text 영역이 읽기 전용이기 때문에 공유를 위해.
                    /// 현재는 OS가 다 해주기 때문에 의미 없음. 호환성
                     _In_ LPWSTR    lpCmdLine,
                    /// 명령줄에 입력된 실행 인수 정보
                     _In_ int       nCmdShow
                    /// 프로그램 시작 시에 화면에 표시될지 여부
                    )
{
    /// 해당 인수가 사용되지 않거나 NULL이라면 인수를 제거
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY0824, szWindowClass, MAX_LOADSTRING);
    /// 내 윈도우 설정을 OS에 등록한다.
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0824));

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
    /// WND : 윈도우
    /// CLASS : 클래스
    /// EX : 확장
    /// W : 유니코드
    /// 윈도우를 구성하는 유니코드를 지원하는 확장된 클래스 변수 선언
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
                    /// Wnd : window        , Proc : Procedure(함수)
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0824));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0824);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    /// 설정된 윈도우 클래스의 내용을 OS에 등록한다.
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

   /// 오류 체크
   if (!hWnd)
   {
      return FALSE;
   }

   /// 만든 윈도우 화면에 표시해줘
   ShowWindow(hWnd, nCmdShow);
   /// 화면에 변경 사항이 있으면 최신 정보로 표시해줘
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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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
