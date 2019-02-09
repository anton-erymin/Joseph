#include "stdafx.h"
#include "Joseph.h"
#include "main.h"

HINSTANCE hInst;		
HWND hwnd;

LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)5;
	wc.lpszMenuName	= NULL;
	wc.lpszClassName = L"MyClass";
	


	RegisterClass(&wc);

	HWND hWnd;

    hInst = hInstance; 

    hWnd = CreateWindow(L"MyClass", L"Template Application",
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, 0, 1000, 800,
						NULL, NULL, 
						hInstance, NULL);

	hwnd = hWnd;

   //CreateWindowA("button", "Start", WS_VISIBLE | WS_CHILD, 100, 100, 850, 400, hWnd, 0, hInst, 0);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
	
   ShowCursor(0);


   MSG msg;

   while (true)
   {
	   if (PeekMessage(&msg, 0, 0, 0, 0))
	   {
		   if (!GetMessageA(&msg, 0, 0, 0)) break;
		   TranslateMessage(&msg);
		   DispatchMessage(&msg);
	   }
	   else DrawScene(hWnd);
	   
   }
   return (int) msg.wParam;
}





LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			InitOpenGL(hWnd);
			InitScene();
			break;

		case WM_DESTROY:
			ReleaseOpenGL();
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			keys[wParam] = true;
			break;

		case WM_KEYUP:
			keys[wParam] = false;
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}