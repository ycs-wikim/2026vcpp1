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

    /// OS가 전달하는 메시지를 받아내기 위한 변수 선언
    MSG msg;

/*
typedef struct tagMSG {
    HWND        hwnd;       /// 어떤 윈도우가 이 메시지를 받아야 하는가?
    UINT        message;    /// 사용자 입력의 종류
    WPARAM      wParam;     /// Word Parameter (== char == 1byte)
                            /// 문자 정보가 전달 : 키보드 입력 정보
    LPARAM      lParam;     /// Long Parameter (== int == 2byte)
                            /// 숫자 정보가 전달 : 마우스 입력 정보
    DWORD       time;       /// 입력 발생 시간 정보
    POINT       pt;         /// 좌표 값
#ifdef _MAC
    DWORD       lPrivate;
#endif
} MSG
*/

    // 기본 메시지 루프입니다:
    /// GetMessage( ) --> 인터럽트 방식
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        /// 간단한 해석
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            /// 세부 내용을 전체 해석
            TranslateMessage(&msg);
            /// 실행 해줘~ OS에게 요청
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
/// CALLBACK 함수는 OS 또는 외부에서 호출할 수 있는 함수
/// HWND : 사용자의 입력을 받을 대상 윈도우
/// UINT : 사용자의 입력 정보(키보드, 마우스 ...)
/// WPARAM/LPARAM : 사용자 정보 처리를 위한 부가
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    /// 사용자의 입력 확인
    switch (message)
    {
    /// WM_ : Window Message
    case WM_COMMAND:            /// 메뉴 클릭 시 발생하는 윈도우 메시지
        {
            /// 정보 획득 매크로
            /// LOWORD( ) 매크로는 하위 절반 값을 숫자로 바꿔서 반환
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                /// 메시지 박스 API
                MessageBox(hWnd, L"자 정보를 알려줘~", L"힘내자", MB_OK);
                /// 정보 창을 생성해서 화면에 출력하는 API
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                /// 모달 형태의 윈도우 :
                /// GUI 프로그램에서 메시지 박스처럼 동작을 멈추는 형태
                /// 모달리스 형태의 윈도우 : 찾기 다이얼로그 형태
                MessageBox(hWnd, L"프로그램 종료해줘~", L"힘 그만내", MB_OK);
                /// 윈도우를 파괴해줘~ => 프로그램 종료
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    /// 항상 화면에 표시되어야 하는 내용을 작성하는 윈도우 메시지
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            /// HDC : 핸들 Device(모니터 화면) Context(정보체)
            /// Context Switching (문맥 교환)
            /// Begin/EndPaint( ) - 지금부터 그리기 시작/종료
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            /// 1. 선 그리기 : 2단계로 나뉘어 있음
            ///     1) 시작 위치를 설정한다.
            MoveToEx(hdc, 0, 0, NULL);
            /// 2) 끝 위치까지 선을 그린다.
            LineTo(hdc, 600, 267);

            /// 그리기를 종료
            EndPaint(hWnd, &ps);
        }
        break;

    /// 프로그램이 종료될거야~ 그러니! 종료전에 필요한 작업 진행해~
    case WM_DESTROY:
        MessageBox(hWnd, L"곧 프로그램 종료될 예정입니다.", L"힘 그만내!!!!!!!!!!!", MB_OK);
        /// GetMessage( )를 실패시킨다!
        PostQuitMessage(0);
        break;
    default:
        /// Default Windows Procedure
        /// 처리되지 않은 윈도우 메시지 처리 요청
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
