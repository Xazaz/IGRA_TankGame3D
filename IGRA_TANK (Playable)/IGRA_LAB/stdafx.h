// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
//#include <time.h>       /* time */

#include <math.h>
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include <atlstr.h>   //for the CString data

#include <vector>
using namespace std;

// TODO: reference additional headers your program requires here
// Typedef
typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
} Material;

typedef struct TreeNode {
	GLfloat matrix[16]; // Stores transformation of object
	GLfloat matrix1[16]; //For test1
	int drawFunctionID; // ID refers to a Draw function
	struct TreeNode * sibling;
	struct TreeNode * child;
} TreeNode;
