// snake.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include<time.h>
#define MAX_LOADSTRING 100

#define IDM_UP            10000
#define IDM_RIGHT         10001
#define IDM_DOWN          10002
#define IDM_LEFT          10003
#define IDM_START_PAUSE   10004

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
int Matrix[40][32] = {0};
POINT Position[1280];
int n_body = 4;
int i,j;
int m,n;
int speed = 500;
int level =1;
int nn,mm=-1;//记录食物的位置
int score =0;
int direction = 1;
bool start_or_pause = true;
bool gameover = true;
RECT rt;
HBRUSH hbr;
HBITMAP hBmp,hBmpOld;
HWND  hwnd[6],hedit[4];	//	静态文本框和按钮的句柄
// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void                DrawUI(HWND);
unsigned int        random();
void                CheckFood();
void                CreateFood();
int                 GetScore();
bool                CheckOver(HWND,int,int);
void                CreateUI(HWND hWnd);
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SNAKE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SNAKE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SNAKE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_SNAKE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_MINIMIZEBOX | WS_SYSMENU ,
      200, 50, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc,hMemDC;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_UP:
					SendMessage(hWnd,WM_KEYDOWN,(WPARAM)VK_UP,0);
					break;
				case IDM_LEFT:
					SendMessage(hWnd,WM_KEYDOWN,(WPARAM)VK_LEFT,0);
					break;
				case IDM_RIGHT:
					SendMessage(hWnd,WM_KEYDOWN,(WPARAM)VK_RIGHT,0);
					break;
				case IDM_DOWN:
					SendMessage(hWnd,WM_KEYDOWN,(WPARAM)VK_DOWN,0);
					break;
				case IDM_START_PAUSE:
					SendMessage(hWnd,WM_KEYDOWN,(WPARAM)VK_SPACE,0);
					break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			m = 20;
			n = 15;
			for(i = 0; i < n_body; i++)
			{
				Position[i].x = m-i;
				Position[i].y = n;
				Matrix[Position[i].x][Position[i].y] = 1;
			}
			CreateFood();
			CreateUI(hWnd);
			SetTimer(hWnd,100,speed,NULL);
				break;
		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_UP:
				//KillTimer(hWnd,100);
				if(direction != 2 && n>0 && CheckOver(hWnd,m,n-1))
				{
					
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 0;
					}
					n--;
					CheckFood();
					for(i = n_body-1; i > 0; i--)
					{
						Position[i] = Position[i-1];
					}
					Position[0].y = n;
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 1;
					}
					CreateFood();
					direction = 0;
					DrawUI(hWnd);
				}
				else
					MessageBeep(0);
			//	SetTimer(hWnd,100,500,NULL);
				break;
			case VK_LEFT:
			//	KillTimer(hWnd,100);
				if(direction != 1 && m>0 && CheckOver(hWnd,m-1,n))
				{
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 0;
					}
					m--;
					CheckFood();
					for(i = n_body-1; i > 0; i--)
					{
						Position[i] = Position[i-1];
					}
					Position[0].x = m;
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 1;
					}	
					CreateFood();
					direction = 3;
					DrawUI(hWnd);
				}
				else
					MessageBeep(0);
			//	SetTimer(hWnd,100,500,NULL);
				break;
			case VK_RIGHT:
				//KillTimer(hWnd,100);
				if(direction != 3 && m<39 && CheckOver(hWnd,m+1,n))
				{
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 0;
					}
					m++;
					CheckFood();
					for(i = n_body-1; i > 0; i--)
					{
						Position[i] = Position[i-1];
					}
					Position[0].x = m;
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 1;
					}	
					CreateFood();
					direction = 1;
					DrawUI(hWnd);
				}
				else
					MessageBeep(0);
			//	SetTimer(hWnd,100,500,NULL);
				break;
			case VK_SPACE:
				if(start_or_pause)
				{
					KillTimer(hWnd,100);
					start_or_pause = false;
				}else
				{
					SetTimer(hWnd,100,speed,NULL);
					start_or_pause = true;
				}
				break;
			case VK_DOWN:
			//	KillTimer(hWnd,100);
				if(direction != 0 && n<31 && CheckOver(hWnd,m,n+1) )
				{
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 0;
					}
					n++;
					CheckFood();
					for(i = n_body-1; i > 0; i--)
					{
						Position[i] = Position[i-1];
					}
					Position[0].y = n;
					for(i = 0; i < n_body; i++)
					{
						Matrix[Position[i].x][Position[i].y] = 1;
					}
					CreateFood();
					direction = 2;
					DrawUI(hWnd);
				}
				else
					MessageBeep(0);
			//	SetTimer(hWnd,100,500,NULL);
				break;
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			
			//GetClientRect(hWnd, &rt);
			hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
			hBmp  =   LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			hMemDC  =  CreateCompatibleDC(hdc);   
			hBmpOld  =   (HBITMAP)SelectObject(hMemDC,hBmp);

			//Matrix[10][10] = 1;
			
			for(i = 0; i < 40; i++)
				for(j =0; j <32; j++)
				{
						rt.left = i * 16;
					rt.top = 24 + j *16;
					switch(Matrix[i][j])
					{
						case 0:
							{
								rt.right = rt.left + 16;
								rt.bottom = rt.top + 16 ;
								FillRect(hdc,&rt,hbr);
							}
							break;
						case 1:
							{
								hBmp  =   LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
								hMemDC  =  CreateCompatibleDC(hdc);   
								hBmpOld  =   (HBITMAP)SelectObject(hMemDC,hBmp);
								BitBlt(hdc,rt.left,rt.top,16,16,hMemDC,0,0,SRCCOPY);
								SelectObject(hMemDC, hBmpOld);   
								DeleteObject(hBmp);   
								DeleteObject(hMemDC); 
							}
							break;
						case 2:
							{
								hBmp  =   LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
								hMemDC  =  CreateCompatibleDC(hdc);   
								hBmpOld  =   (HBITMAP)SelectObject(hMemDC,hBmp);
								BitBlt(hdc,rt.left,rt.top,16,16,hMemDC,0,0,SRCCOPY);
								SelectObject(hMemDC, hBmpOld);   
								DeleteObject(hBmp);   
								DeleteObject(hMemDC); 
							}
							break;
					}
				}
			SelectObject(hMemDC, hBmpOld);   
			DeleteObject(hBmp);   
			DeleteObject(hMemDC); 
			EndPaint(hWnd, &ps);
			break;
		case WM_TIMER:
			//KillTimer(hWnd,100);
			switch(direction)
			{
			case 0://UP
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 0;
				}
				if(n>0)
				{
					n--;
				}
				else
				{
					//MessageBox(NULL,"Game Over!",NULL,NULL);
					KillTimer(hWnd,100);
					gameover = false;
				}
				CheckFood();
				for(i = n_body-1; i > 0; i--)
				{
					Position[i] = Position[i-1];
				}
				Position[0].y = n;
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 1;
				}
				CreateFood();
				DrawUI(hWnd);
				break;
			case 1://RIGHT
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 0;
				}
				if(m<39)
					m++;
				else
				{
					//MessageBox(NULL,"Game Over!",NULL,NULL);
					KillTimer(hWnd,100);
					gameover = false;
				}
				CheckFood();
				for(i = n_body-1; i > 0; i--)
				{
					Position[i] = Position[i-1];
				}
				Position[0].x = m;
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 1;
				}
				CreateFood();
				DrawUI(hWnd);
				break;
			case 2://DOWN
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 0;
				}
				if(n<31)
				  n++;
				else
				{
				//	MessageBox(NULL,"Game Over!",NULL,NULL);
					KillTimer(hWnd,100);
					gameover = false ;
				}
				CheckFood();
				for(i = n_body-1; i > 0; i--)
				{
					Position[i] = Position[i-1];
				}	
				Position[0].y = n;
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 1;
				}
				CreateFood();
				DrawUI(hWnd);
				break;
			case 3://LEFT
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 0;
				}
				if(m>0)
					m--;
				else
				{
					//MessageBox(NULL,"Game Over!",NULL,NULL);
					KillTimer(hWnd,100);
					gameover = false;
				}
				CheckFood();
				for(i = n_body-1; i > 0; i--)
				{
					Position[i] = Position[i-1];
				}
				Position[0].x = m;
				for(i = 0; i < n_body; i++)
				{
					Matrix[Position[i].x][Position[i].y] = 1;
				}
				CreateFood();
				DrawUI(hWnd);
				break;
			}
			//SetTimer(hWnd,100,500,NULL);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
void DrawUI(HWND hWnd)
{
	if(gameover)
		KillTimer(hWnd,100);
	HDC hdc,hMemDC;
	hdc = GetDC(hWnd);
	hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
			
			for(i = 0; i < 40; i++)
				for(j =0; j <32; j++)
				{
					rt.left = i * 16;
					rt.top = 24 + j *16;
					switch(Matrix[i][j])
					{
						case 0:
							{
								rt.right = rt.left + 16;
								rt.bottom = rt.top + 16 ;
								FillRect(hdc,&rt,hbr);
							}
							break;
						case 1:
							{
								hBmp  =   LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
								hMemDC  =  CreateCompatibleDC(hdc);   
								hBmpOld  =   (HBITMAP)SelectObject(hMemDC,hBmp);
								BitBlt(hdc,rt.left,rt.top,16,16,hMemDC,0,0,SRCCOPY);
								SelectObject(hMemDC, hBmpOld);   
								DeleteObject(hBmp);   
								DeleteObject(hMemDC); 
							}
							break;
						case 2:
							{
								hBmp  =   LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
								hMemDC  =  CreateCompatibleDC(hdc);   
								hBmpOld  =   (HBITMAP)SelectObject(hMemDC,hBmp);
								BitBlt(hdc,rt.left,rt.top,16,16,hMemDC,0,0,SRCCOPY);
								SelectObject(hMemDC, hBmpOld);   
								DeleteObject(hBmp);   
								DeleteObject(hMemDC); 
							}
							break;
					}

				}
				char str[20];
		        memset(str,0,20);
				GetScore();
			level = score / 200 + 1;
			speed = 500 / level;
			wsprintf(str,"%d",level);
			SetWindowText(hedit[0],str);
			wsprintf(str,"%d",score);
			SetWindowText(hedit[2],str);
			ReleaseDC(hWnd,hdc);
			if(gameover)
				SetTimer(hWnd,100,speed,NULL);
}

unsigned int random()//产生随机数
{
	static unsigned int seed = (unsigned int )time(NULL);
	seed = (25273 * seed + 12849)%65535;
	return seed;
}
void CheckFood()
{
	if(Matrix[m][n] == 2) 
	{
		n_body++;
		mm = -1;
	}
}
void CreateFood()
{
	if(mm == -1)
	{
		do
		{
			mm = random() %40;
			nn = random() %32;
		}while(Matrix[mm][nn] == 1);
		Matrix[mm][nn] = 2;
	}
}
int GetScore()
{
	score = (n_body - 4) * 10;
	return score;
}
bool CheckOver(HWND hWnd,int a,int b)
{
	if(Matrix[a][b] == 1)
	{
		KillTimer(hWnd,100);
		MessageBox(NULL,"Game Over!",NULL,NULL);
		return false;
	}
	else
		return true;
}

void CreateUI(HWND hWnd)//初始化游戏界面
{
	int i;
	for(i=0;i<4;i++)
	{
		hwnd[i] = CreateWindow("STATIC","级别 :",
			WS_CHILD | WS_VISIBLE  |SS_LEFT,650,24 + i*30,48,20,hWnd,NULL,hInst,NULL);
	}
	SetWindowText(hwnd[1],"满行 :");
	SetWindowText(hwnd[2],"得分 :");
	SetWindowText(hwnd[3],"最高分");

	hwnd[4] = CreateWindow("STATIC","空格键开始/暂停",
			WS_CHILD | WS_VISIBLE  |SS_LEFT|WS_DISABLED,0,530,150,45,hWnd,NULL,hInst,NULL);

	CreateWindow("STATIC","|",
			WS_CHILD | WS_VISIBLE  |SS_LEFT|WS_DISABLED,160,530,2,40,hWnd,NULL,hInst,NULL);

	hwnd[5] = CreateWindow("STATIC","方向键上,左,右,下键控制移动",
			WS_CHILD | WS_VISIBLE  |SS_LEFT|WS_DISABLED,170,530,235,25,hWnd,NULL,hInst,NULL);

	/**
	创建编辑框
	*/
	for(i=0;i<4;i++)
	{
		hedit[i] = CreateWindow("EDIT",NULL,
			WS_CHILD | WS_VISIBLE  |ES_LEFT |ES_READONLY |WS_BORDER,
			700,24 + i*30,60,20,hWnd,NULL,hInst,NULL);
	}
//	SetWindowText(hedit[0],"1");

	//创建UP键
		CreateWindow(
		TEXT("button"),
		"U",
		WS_CHILD|WS_VISIBLE,
		695,295,
		30,30,
		hWnd,
		(HMENU)IDM_UP,
		hInst,
		NULL);

	//创建RIGHT键
		CreateWindow(
		TEXT("button"),
		"R",
		WS_CHILD|WS_VISIBLE,
		725,325,
		30,30,
		hWnd,
		(HMENU)IDM_RIGHT,
		hInst,
		NULL);

		//创建DOWN键
		CreateWindow(
		TEXT("button"),
		"D",
		WS_CHILD|WS_VISIBLE,
		695,355,
		30,30,
		hWnd,
		(HMENU)IDM_DOWN,
		hInst,
		NULL);

		//创建LEFT键
		CreateWindow(
		TEXT("button"),
		"L",
		WS_CHILD|WS_VISIBLE,
		665,325,
		30,30,
		hWnd,
		(HMENU)IDM_LEFT,
		hInst,
		NULL);

		//创建播放/暂停键
		CreateWindow(
		TEXT("button"),
		"S/P",
		WS_CHILD|WS_VISIBLE,
		695,325,
		30,30,
		hWnd,
		(HMENU)IDM_START_PAUSE,
		hInst,
		NULL);

			//工具栏
	/*CreateToolbarEx(
		hWnd,  //hwnd 父窗口句柄
		WS_VISIBLE|WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|CCS_NODIVIDER,  //ws 工具栏风格
		IDR_TOOLBAR1,  //wID 工具栏的子窗口ID
		6,  //nBitmaps 位图中包含多少个按钮小图标
		hInst,  //hBMInst 包含位图资源的模块句柄
		IDB_BITMAP2,  //wBMID 位图资源ID
		tbButtons,  //lpButtons
		8,  //iNumButtons 按钮总数
		16,  //dxButton 单个按钮宽度
		16,  //dyButton 单个按钮高度
		16,  //dxBitmap 单个按钮小图标的宽度
		16,  //dyBitmap 单个按钮小图标的高度
		sizeof(TBBUTTON));  //uStructSize
		*/
	//工具栏下的黑线
	CreateWindow(
		TEXT("static"),  //lpClassName 类名
		NULL,  //lpWindowName 标题
		WS_CHILD|WS_VISIBLE|SS_BLACKFRAME,
		1,12,
		800,1,
		hWnd,  //hWndParent
		NULL,  //hMenu
		hInst,
		NULL);  //lpParam  额外的参数Radio Buttons
}