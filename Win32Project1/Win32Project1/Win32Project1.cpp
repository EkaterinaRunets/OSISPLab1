// Win32Project1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <CommDlg.h>
#include <stdio.h>
#include <string>
#include "Win32Project1.h"
using namespace std;
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc,hdc1;
	static POINT pt1,pt2,pt3;
	static BOOL draw=false;
	static HDC bufDC,saveDC;
	static HBITMAP bufBmp,saveBmp;
	static RECT rect;
	static HMENU  MainMenu = CreateMenu();
	static HMENU  SubMenuFigures = CreateMenu();
	static HMENU  SubMenuAction = CreateMenu();
	static HMENU  SubMenuFile = CreateMenu();		
	static HMENU  SubMenuColor = CreateMenu();
	static HMENU  SubMenuWidth = CreateMenu();
	static HANDLE oldbufBmp;
	int static choosen=3;
	static BOOL poly;
	static int pointcount;
	static POINT points[50];
	static HPEN Pen;
	static HBRUSH Brush;
	static COLORREF   crCustColor[16];
	static CHOOSECOLOR  ccPen,ccBrush;
	static int Width=1;
	static HANDLE oldPen,oldBrush;
	static string Text;
	static BOOL type;
	static char c;
	static char  fullpath[256],filename[256],dir[256];
	static HENHMETAFILE  hEnhMtf;
	static ENHMETAHEADER  emh;	
	static OPENFILENAME  ofn;
	static PRINTDLG  print;
	static HDC fileDC;
	static DOCINFO docinfo;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if(wParam>=1 && wParam<10) choosen=wParam;
		if(choosen==8 || choosen==9) 
		{
			pointcount=0;
			poly=true;

		}
		else poly=false;
		if(wParam==1)
		{
			Text="";
			type=true;

		}
		else
		{
			type=false;
		}
		if(wParam>=41 && wParam<46)
		{
			Width=wParam-40;
			DeleteObject(Pen);
			Pen=CreatePen(PS_SOLID, Width, ccPen.rgbResult);
			DeleteObject(SelectObject(hdc,Pen));
			DeleteObject(SelectObject(saveDC,Pen));			

		}		
		switch(wParam)
		{
		case 10:
			ofn.lStructSize=sizeof(OPENFILENAME);
			ofn.hwndOwner=hWnd;
			ofn.hInstance=hInst; 
			ofn.lpstrFilter="Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
			ofn.nFilterIndex=1;
			ofn.lpstrFile=fullpath;
			ofn.nMaxFile=sizeof(fullpath);
			ofn.lpstrFileTitle=filename;
			ofn.nMaxFileTitle=sizeof(filename);
			ofn.lpstrInitialDir=dir;
			ofn.lpstrTitle="Save file as...";
			ofn.Flags=OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_EXPLORER;
			if(GetSaveFileName(&ofn))
			{
				fileDC=CreateEnhMetaFile(NULL,filename,NULL,NULL);
				BitBlt(fileDC,0,0,rect.right,rect.bottom,hdc,0,0,SRCCOPY);
				hEnhMtf=CloseEnhMetaFile(fileDC);
				DeleteEnhMetaFile(hEnhMtf);
			}
			break;
		case 11:
			ofn.lStructSize=sizeof(OPENFILENAME);
			ofn.hwndOwner=hWnd;
			ofn.hInstance=hInst; 
			ofn.lpstrFilter="Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
			ofn.nFilterIndex=1;
			ofn.lpstrFile=fullpath;
    		ofn.nMaxFile=sizeof(fullpath);
			ofn.lpstrFileTitle=filename;
			ofn.nMaxFileTitle=sizeof(filename);
			ofn.lpstrInitialDir=dir;
			ofn.lpstrTitle="Open file...";
			ofn.Flags=OFN_EXPLORER|OFN_CREATEPROMPT|OFN_ALLOWMULTISELECT;
			if(GetOpenFileName(&ofn))
			{
				hEnhMtf=GetEnhMetaFile(fullpath);
				GetEnhMetaFileHeader(hEnhMtf,sizeof(ENHMETAHEADER),&emh);
				SetRect(&rect,emh.rclBounds.left,emh.rclBounds.top,emh.rclBounds.right,emh.rclBounds.bottom);
				PlayEnhMetaFile(saveDC,hEnhMtf,&rect);			
				InvalidateRect(hWnd,&rect,NULL);
				UpdateWindow(hWnd);
				DeleteEnhMetaFile(hEnhMtf);
			}
			break;
		case 12:
			DestroyWindow(hWnd);
			break;
		case 13:
			FillRect(saveDC,&rect,WHITE_BRUSH);
			InvalidateRect(hWnd,&rect,NULL);
			UpdateWindow(hWnd);
			break;
		case 14:
			ZeroMemory(&print, sizeof(print));
			print.lStructSize = sizeof(print);
			print.hwndOwner   = hWnd;
			print.hDevMode    = NULL; 
			print.hDevNames   = NULL; 
			print.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
			print.nCopies     = 1;
			print.nFromPage   = 0xFFFF;
			print.nToPage     = 0xFFFF;
			print.nMinPage    = 1;
			print.nMaxPage    = 0xFFFF;
			if (PrintDlg(&print)==TRUE)
			{
				int Rx = GetDeviceCaps(print.hDC, LOGPIXELSX);
				int Ry = GetDeviceCaps(print.hDC, LOGPIXELSY);
				docinfo.cbSize=sizeof(DOCINFO);
				docinfo.lpszDocName="Print Picture";
				docinfo.fwType=NULL;
				docinfo.lpszDatatype=NULL;
				docinfo.lpszOutput=NULL;
				StartDoc(print.hDC,&docinfo);
				StartPage(print.hDC);		
				GetClientRect(hWnd,&rect);
				int Rx1 = GetDeviceCaps(hdc, LOGPIXELSX);
				int Ry1 = GetDeviceCaps(hdc, LOGPIXELSY);
				StretchBlt(print.hDC, 0, 0,(int)((float)(0.91*rect.right*Rx/Rx1)), (int)((float)(0.91*rect.bottom*Ry/Ry1)),
				hdc, 0, 0, rect.right, rect.bottom, SRCCOPY);
				EndPage(print.hDC);
				EndDoc(print.hDC);
				DeleteDC(print.hDC);
			}
			break;
		case 30:
			ccPen.lStructSize = sizeof(CHOOSECOLOR);
			ccPen.hInstance = NULL;
			ccPen.hwndOwner = hWnd;
			ccPen.lpCustColors = crCustColor;
			ccPen.Flags = CC_RGBINIT|CC_FULLOPEN;
			ccPen.lCustData = 0L;
			ccPen.lpfnHook = NULL;
			ccPen.rgbResult = RGB(0x80, 0x80, 0x80);
			ccPen.lpTemplateName = NULL;	
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
			if(ChooseColor(&ccPen))
			{
				DeleteObject(Pen);
				Pen=CreatePen(PS_SOLID, Width, ccPen.rgbResult);
				DeleteObject(SelectObject(hdc,Pen));
				DeleteObject(SelectObject(saveDC,Pen));
			}
			break;
		case 31:
			ccBrush.lStructSize = sizeof(CHOOSECOLOR);
			ccBrush.hInstance = NULL;
			ccBrush.hwndOwner = hWnd;
			ccBrush.lpCustColors = crCustColor;
			ccBrush.Flags = CC_RGBINIT|CC_FULLOPEN;
			ccBrush.lCustData = 0L;
			ccBrush.lpfnHook = NULL;
			ccBrush.rgbResult = RGB(0x80, 0x80, 0x80);
			ccBrush.lpTemplateName = NULL;	
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
			if(ChooseColor(&ccBrush))
			{
				DeleteObject(Brush);
				Brush=CreateSolidBrush(ccBrush.rgbResult);
				DeleteObject(SelectObject(hdc,Brush));
				DeleteObject(SelectObject(saveDC,Brush));
			}
		    break;
		case 32:			
		    DeleteObject(Brush);
			Brush=(HBRUSH)GetStockObject(NULL_BRUSH);
			break;

		}
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
		break;
	case WM_CREATE:			
		ShowWindow(hWnd,SW_SHOWMAXIMIZED);
		hdc=GetDC(hWnd);
		GetClientRect(hWnd,&rect);
		saveDC=CreateCompatibleDC(hdc);
		saveBmp=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		SelectObject(saveDC,saveBmp);
		FillRect(saveDC,&rect,WHITE_BRUSH);
		draw=false;

		AppendMenu(MainMenu,MF_POPUP,(UINT_PTR)SubMenuFile,"File");
		AppendMenu(MainMenu,MF_POPUP,(UINT_PTR)SubMenuFigures,"Figures");
		AppendMenu(MainMenu,MF_POPUP,(UINT_PTR)SubMenuColor,"Color");
		AppendMenu(MainMenu,MF_POPUP,(UINT_PTR)SubMenuWidth,"Width");
		AppendMenu(MainMenu,MF_POPUP,(UINT_PTR)SubMenuAction,"Action");	

		AppendMenu(SubMenuFile, MF_STRING, 13, "New");
		AppendMenu(SubMenuFile, MF_STRING, 11, "Open");		
		AppendMenu(SubMenuFile, MF_STRING, 10, "Save");
		AppendMenu(SubMenuFile, MF_STRING, 14, "Print");
		AppendMenu(SubMenuFile, MF_STRING, 12, "Exit");

		AppendMenu(SubMenuAction, MF_STRING, 20, "Pan and Zoom");

		AppendMenu(SubMenuFigures, MF_STRING, 3, "Pen");
		AppendMenu(SubMenuFigures, MF_STRING, 4, "Line");	
		AppendMenu(SubMenuFigures, MF_STRING, 5, "Triangle");
		AppendMenu(SubMenuFigures, MF_STRING, 6, "Rectangle");
		AppendMenu(SubMenuFigures, MF_STRING, 7, "Ellipse");
		AppendMenu(SubMenuFigures, MF_STRING, 8, "Polyline");	
		AppendMenu(SubMenuFigures, MF_STRING, 9, "Polygon");			
		AppendMenu(SubMenuFigures, MF_STRING, 1, "Text");	

		AppendMenu(SubMenuColor, MF_STRING, 30, "PenColor");
		AppendMenu(SubMenuColor, MF_STRING, 31, "FillColor");
		AppendMenu(SubMenuColor, MF_STRING, 32, "TransparentFill");
		
		AppendMenu(SubMenuWidth, MF_STRING, 41, "1");
		AppendMenu(SubMenuWidth, MF_STRING, 42, "2");
		AppendMenu(SubMenuWidth, MF_STRING, 43, "3");
		AppendMenu(SubMenuWidth, MF_STRING, 44, "4");
		AppendMenu(SubMenuWidth, MF_STRING, 45, "5");	


		SetMenu(hWnd, MainMenu);

		break;
	case WM_LBUTTONDOWN:		
		pt1.x=LOWORD(lParam);
		pt1.y=HIWORD(lParam);
		pt3.x=LOWORD(lParam);
		pt3.y=HIWORD(lParam);
		draw=true;
		if(poly)
		{
			
			if(pointcount==0)
			{
				
				points[pointcount].x=LOWORD(lParam);
			    points[pointcount].y=HIWORD(lParam);
				pt1.x=LOWORD(lParam);
				pt1.y=HIWORD(lParam);
			}
			else
			{
				pt1=pt2;		
			}
			
		}
		break;
	case WM_MOUSEMOVE:
		
		
		if(draw)
		{
			pt2.x=LOWORD(lParam);
		    pt2.y=HIWORD(lParam);
			if(choosen>=4 && choosen<10)
			{

				   	GetClientRect(hWnd,&rect);
					bufDC=CreateCompatibleDC(hdc);
					bufBmp=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
					oldbufBmp=SelectObject(bufDC,bufBmp);
					FillRect(bufDC,&rect,WHITE_BRUSH);
					BitBlt(bufDC,0,0,rect.right,rect.bottom,saveDC,0,0,SRCCOPY);
					oldPen=SelectObject(bufDC,Pen);
					oldBrush=SelectObject(bufDC,Brush);
			}			
			
			switch(choosen)
			{
			
		       
			   case 3:
				 MoveToEx(hdc,pt2.x,pt2.y,NULL);
		         LineTo(hdc,pt3.x,pt3.y);
				 break;
			   case 4:
				  MoveToEx(bufDC,pt1.x,pt1.y,NULL);
		          LineTo(bufDC,pt2.x,pt2.y);
				  break;
				case 5:			
					POINT Points[3];
					Points[1].x=pt1.x-(pt1.x-pt2.x);
					Points[1].y=pt1.y;
					Points[2].x=pt1.x-(pt1.x-pt2.x)/2;
					Points[2].y=pt1.y-(pt1.y-pt2.y);
					Points[0].x=pt1.x;
					Points[0].y=pt1.y;
					Polygon(bufDC,Points,3);
					break;
				case 6:				
					Rectangle(bufDC,pt1.x-(pt1.x-pt2.x),pt1.y,pt1.x,pt1.y-(pt1.y-pt2.y));
					break;
				case 7:												
					Ellipse(bufDC,pt1.x,pt1.y,pt1.x-(pt1.x-pt2.x),pt1.y-(pt1.y-pt2.y));
					break;
				case 8:	
					MoveToEx(bufDC,pt1.x,pt1.y,NULL);
		            LineTo(bufDC,pt2.x,pt2.y);
					break;
				case 9:	
					MoveToEx(bufDC,pt1.x,pt1.y,NULL);
		            LineTo(bufDC,pt2.x,pt2.y);
					break;
				

			
			}
			if(choosen>=4 && choosen<10)
			{
					InvalidateRect(hWnd,&rect,NULL);
					UpdateWindow(hWnd);
				   	SelectObject(bufDC, oldPen);
					SelectObject(bufDC, oldBrush);
					SelectObject(bufDC, oldbufBmp);
					DeleteObject(bufBmp);
					DeleteDC(bufDC);
			}				
			
			pt3=pt2;
		}					
		break;
	case WM_LBUTTONUP:
		draw=false;
		
		if(poly)
		{
			pointcount++;
			points[pointcount].x=LOWORD(lParam);
			points[pointcount].y=HIWORD(lParam);
		}
		if(!type) BitBlt(saveDC,0,0,rect.right,rect.bottom,hdc,0,0,SRCCOPY);
		break;

	case WM_RBUTTONDOWN:
		if(poly)
		{
			if(choosen==9)
			{
				Polygon(saveDC,points,pointcount+1);
				
			}			
			pointcount=0;

		}
		
		InvalidateRect(hWnd,&rect,NULL);
		UpdateWindow(hWnd);		
		break;
	case  WM_CHAR:
		if(type)
		{
			char c=(char)wParam;
			if(c==VK_RETURN)
			{
				type=false;
				GetClientRect(hWnd,&rect);
				BitBlt(saveDC,0,0,rect.right,rect.bottom,hdc,0,0,SRCCOPY);
			}
			else 
				if(c==VK_BACK)
					Text.pop_back();
				else
					Text+=c;
			BitBlt(hdc,0,0,rect.right,rect.bottom,saveDC,0,0,SRCCOPY);
			TextOut(hdc,pt1.x,pt1.y,Text.c_str(),strlen(Text.c_str()));								
		}
		break;
	case WM_MOUSEWHEEL:

		break;
	case WM_PAINT:
		hdc1 = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd,&rect);
		if(draw) BitBlt(hdc1,0,0,rect.right,rect.bottom,bufDC,0,0,SRCCOPY);
		else BitBlt(hdc1,0,0,rect.right,rect.bottom,saveDC,0,0,SRCCOPY);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
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
