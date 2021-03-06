/*
__________________________________

**********************************
[000] Done by Joshua, P1651481 (Basic Scene and Rendering)
[001] Done by Zhen Xiong, P1646364 (Integration of Tank & Projectile to function together)
**********************************
__________________________________
*/
#include "stdafx.h"
#include "IGRA_LAB.h"
#include "Clock.h"
#include "RobotArm.h"
#include "Projectile.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int width = 418;
int height = 460;

//DECLARATION OF OBJECTS
RobotArm *robot = new RobotArm;
vector<Projectile*>  projectileArray;	//PROJECTILE
Clock *clock1 = new Clock;

int veloValSelected = 1;

GLuint PixelFormat;
HDC hDC = NULL;
HWND hWnd = NULL;
HGLRC hRC = NULL;

static PIXELFORMATDESCRIPTOR pfd{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,// Version Number
	PFD_DRAW_TO_WINDOW | // Format Must Support Window
	PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
	PFD_DOUBLEBUFFER,// Must Support Double Buffering
	PFD_TYPE_RGBA, // Request An RGBA Format
	16, // Select Our Color Depth = 16
	0, 0, 0, 0, 0, 0, // Color Bits Ignored
	0,// No Alpha Buffer
	0,// Shift Bit Ignored
	0,// No Accumulation Buffer
	0, 0, 0, 0, // Accumulation Bits Ignored
	32, // 32 Bit Z-Buffer (Depth Buffer)
	0,// No Stencil Buffer
	0,// No Auxiliary Buffer
	PFD_MAIN_PLANE,// Main Drawing Layer
	0,// Reserved
	0, 0, 0// Layer Masks Ignored
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int InitOpenGL();
void DrawGLScene();
void ReSizeGLScene(GLsizei width, GLsizei height);

void SetupLight() {
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = { 1.0f, 1.0f,1.0f, 1.0f };
	GLfloat LightDiffuse[] = { 2.0f, 2.0f, 2.0f, 0.2f };
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightPosition[] = { 70.0f, 30.0f, 100.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos,
	float &xOpenGLPos, float &yOpenGLPos) {
	xOpenGLPos = xWinPos / 200.0 - 1;
	yOpenGLPos = -yWinPos / 200.0 + 1;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IGRALAB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGRALAB));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			DrawGLScene();
			SetTimer(hWnd, 0, 1, 0);
			SwapBuffers(hDC);
		}
	}

	return (int)msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGRALAB));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IGRALAB);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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
	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowW(szWindowClass,
		szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, width, height,
		nullptr, nullptr, hInstance, nullptr);


	InitOpenGL();

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	robot->BuildTree();
	clock1->Start();

	//PROJECTILE INITIATION, SPAWN FIRST PROJECTILE
	projectileArray.push_back(new Projectile);
	projectileArray.back()->Spawn(0, 0, 0);

	veloValSelected = 1;
	CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_SLOW, MF_CHECKED);
	CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_MEDIUM, MF_UNCHECKED);
	CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_FAST, MF_UNCHECKED);

	ReSizeGLScene(width, height);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_VELOCITIES_SLOW:
			veloValSelected = 1;
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_SLOW, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_MEDIUM, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_FAST, MF_UNCHECKED);
			break;
		case ID_VELOCITIES_MEDIUM:
			veloValSelected = 2;
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_SLOW, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_MEDIUM, MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_FAST, MF_UNCHECKED);
			break;

		case ID_VELOCITIES_FAST: 
			veloValSelected = 3;
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_SLOW, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_MEDIUM, MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd), ID_VELOCITIES_FAST, MF_CHECKED);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN: {
		//robot->HandleKeyDown(wParam);
		robot->HandleKeyWASD(wParam);

		//PROJECTILE HANDLING
		if (VK_SPACE) { //FIRE
			//FIRE CURRENT PROJECTILE
			projectileArray[projectileArray.size() - 1]->Spawn(robot->xPos, robot->yPos + 1.5, robot->zPos);
			projectileArray[projectileArray.size() - 1]->HandleKeyDown(wParam, robot->rotUpperArm + robot->yRotation, robot->rotLowerArm);

			//SPAWN NEW
			projectileArray.push_back(new Projectile);

			//CHECK SELECTED VELO AND SET SPEED THEN PUT THE BULLET SOMEWHERE
			if (veloValSelected == 1) {
				projectileArray[projectileArray.size() - 1]->setVelocity(1, 1);
			}
			else if (veloValSelected == 2) {
				projectileArray[projectileArray.size() - 1]->setVelocity(3, 3);
			}
			else {
				projectileArray[projectileArray.size() - 1]->setVelocity(5, 5);
			}

			
			projectileArray.back()->Spawn(0, 0, 0);
		}
	}
					 break;
	
	case WM_KEYUP: {
		robot->HandleKeyRelease(wParam);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_LBUTTONDOWN:
	{		
		break;
	}
	case WM_LBUTTONUP: {
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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



void ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)// Prevent A Divide By Zero By
		height = 1; // Making Height Equal One
					// Reset The Current Viewport
	glViewport(0, 0, width - 17, height - 60);
	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset The Projection Matrix
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	// 3D Perspective
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	//// 2D Orthographic
	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}
int InitOpenGL() {
	// Get Device Dontext
	if (!(hDC = GetDC(hWnd))) {
		MessageBox(NULL, L"Can't Create A GL Device Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Check if Windows can find a matching Pixel Format
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Try to set pixel format
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Get a Rendering context
	if (!(hRC = wglCreateContext(hDC))) {
		MessageBox(NULL, L"Can't Create A GL Rendering Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Activate Rendering Context
	if (!wglMakeCurrent(hDC, hRC)) {
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	SetupLight();
	
	return 1;
}


void Draw3D_AxisSystem() {

	glBegin(GL_POLYGON);
	// Draw positive x-axis as red
	glColor3f(1, 0., 0.0);
	glVertex3f(100, -5, -100);//4
	glVertex3f(-100, -5, -100);//3
	glVertex3f(-100, -5, 100);//2
	glVertex3f(100, -5, 100);//1
	glEnd();
	// Draw positive x-axis as green
	/*glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);*/
	// Draw positive z-axis as blue
	/*glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();*/



}

int firstframe = 0;
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(6, 9,8, // Camera's position
		0, 0, 0, // Camera’s target to look at
		0, 1, 0); // Orientation of camera

	glEnable(GL_DEPTH_TEST);

	double timePassedRelInSeconds = clock1->GetTimePassedSinceLastTime();

	//RENDER PROJECTILES
	for each (Projectile *projectile in projectileArray)
	{
		projectile->Update(timePassedRelInSeconds);
	}

	Draw3D_AxisSystem();
	robot->Draw();

	firstframe++;
	if (firstframe == 1)
	{
		robot->HandleKeyWASD(1);
	}
	
}