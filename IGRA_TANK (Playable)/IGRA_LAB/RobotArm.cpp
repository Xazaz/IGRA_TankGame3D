/*
__________________________________

**********************************
[000] Done by Joshua, P1651481 (Turret tilting and rotating)
[001] Done by Joshua, P1651481 (Basic Tank Movement and Rotation)
[003] Done by Zhen Xiong, P1646364 (Acceleration)
**********************************
__________________________________
*/
#include "stdafx.h"
#include "RobotArm.h"
#include "Clock.h"
//Basic cout
#include <iostream>
using namespace std;
Clock *clock = new Clock;
double currentVelo = 0;
double currentAcc = 0;
double currentDcc = 0;
double maxVelo = 360;
double t;
float acceVel;
double timeMaxStarts = 0;

RobotArm::RobotArm()
{
	gluQuadricNormals(joint, GLU_SMOOTH);
	gluQuadricNormals(base, GLU_SMOOTH);
	gluQuadricNormals(baseCover, GLU_SMOOTH);
	gluQuadricNormals(lowerArm, GLU_SMOOTH);
	gluQuadricNormals(upperArm, GLU_SMOOTH);
	gluQuadricTexture(base, GL_TRUE);// if you want to map a texture to it.
	CreateBaseTexture();
	
}

enum AccState
{
	Acc,
	Dcc,
	Max,
	SS,
	StandStill
};
AccState CurrentState = AccState::SS;
RobotArm::~RobotArm()
{
}

void RobotArm::SetMaterial(Material *m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
}

void RobotArm::CreateBaseTexture() {
	int nrOfCheckersOnRow = 8;
	float dim = 64.0 / nrOfCheckersOnRow;
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {

			// Calculate in which checkerboard
			//rectangle the pixel falls
			int row = (int)(i / dim);
			int col = (int)(j / dim);
			int c = 0;
			if (row % 2 == 0) { // Even rows start with black
				if (col % 2 == 0) {
					// All even column will be black
					red = green = blue = 0;
				}
				else {
					green = 255;
					red = 255;
					blue = 0;
				}
			}
			else {
				// Odd rows start with yellow
				if (col % 2 == 0) {
					// All even column will be yellow
					green = 255;
					red = 255;
					blue = 0;
				}
				else {
					red = green = blue = 0;
				}
			}
			image[i][j][0] = (GLubyte)red;
			image[i][j][1] = (GLubyte)green;
			image[i][j][2] = (GLubyte)blue;
		}
	}
}
void RobotArm::BuildTree() {
	// The root of the tree (represents the base of Robot Arm)
	tree = new TreeNode;
	tree->child = 0;
	tree->sibling = 0;
	tree->drawFunctionID = DRAW_BASE_FUNCTION_ID;
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);
	// The lower arm
	lowerArmNode = new TreeNode;
	lowerArmNode->child = 0;
	lowerArmNode->sibling = 0;
	lowerArmNode->drawFunctionID = DRAW_LOWERARM_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 0.25, 0);
	glRotatef(rotLowerArm, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerArmNode->matrix);
	// The upper arm
	upperArmNode = new TreeNode;
	upperArmNode->child = 0;
	upperArmNode->sibling = 0;
	upperArmNode->drawFunctionID = DRAW_UPPERARM_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 0.25, 0);
	glRotatef(rotUpperArm, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperArmNode->matrix);
	// The first joint
	jointNode = new TreeNode;
	jointNode->child = 0;
	jointNode->sibling = 0;
	jointNode->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 0, 1.5);
	glGetFloatv(GL_MODELVIEW_MATRIX, jointNode->matrix);
	// The second joint
	jointNode2 = new TreeNode;
	jointNode2->child = 0;
	jointNode2->sibling = 0;
	jointNode2->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glLoadIdentity();
	glTranslatef(0, 0, 1.5);
	glGetFloatv(GL_MODELVIEW_MATRIX, jointNode2->matrix);
	/*// Plugging the nodes in the tree together
	tree->child = lowerArmNode;
	//jointNode2->child = upperArmNode;
	lowerArmNode->sibling = upperArmNode;
	
	jointNode2->child = upperArmNode;
	//upperArmNode->sibling = lowerArmNode;
	//jointNode2->sibling = upperArmNode;
	//lowerArmNode->sibling = jointNode;
	//upperArmNode->child = jointNode2;*/

	tree->child = lowerArmNode;
	//jointNode2->child = upperArmNode;
	lowerArmNode->child = jointNode2;

	lowerArmNode->sibling= jointNode;
	jointNode2->child = upperArmNode;

	clock->Start();
}

void RobotArm::DrawTree(TreeNode* root) {
	if (root == 0)
		return;

	glPushMatrix();
		glMultMatrixf(root->matrix);
		DrawFunction(root->drawFunctionID);
		if (root->child != 0)
			DrawTree(root->child);
	glPopMatrix();

	if (root->sibling != 0)
		DrawTree(root->sibling);
}

void RobotArm::DrawFunction(int id) {
	switch (id) {
	case DRAW_BASE_FUNCTION_ID:
		DrawBase();
		break;
	case DRAW_LOWERARM_FUNCTION_ID:
		DrawLowerArm();
		break;
	case DRAW_UPPERARM_FUNCTION_ID:
		DrawUpperArm();
		break;
	case DRAW_JOINT_FUNCTION_ID:
		DrawJoint();
		break;
	}
}
void RobotArm::DrawBase() {
	GLfloat blue[] = { 0, 0, 1, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);

	glBegin(GL_POLYGON);
	glColor3f(.0f, .0f, 0);
	glVertex3f(-1, -1, -1);//5
	glVertex3f(-1, 1, -1);//4
	glVertex3f(-1, 1, 1);//3
	glVertex3f(-1, -1, 1);//2
	glEnd();

	// Y-
	glBegin(GL_POLYGON);
	glColor3f(.0f, .0f, 0);
	glVertex3f(1, -1, -1);//4
	glVertex3f(-1, -1, -1);//3
	glVertex3f(-1, -1, 1);//2
	glVertex3f(1, -1, 1);//1
	glEnd();

	// Z-
	glBegin(GL_POLYGON);
	glColor3f(.0f, .0f, 0);
	glVertex3f(1, -1, -1); // 7
	glVertex3f(1, 1, -1); // 6
	glVertex3f(-1, 1, -1); // 5
	glVertex3f(-1, -1, -1); // 4
	glEnd();

	// X+
	glBegin(GL_POLYGON);
	glColor3f(.0f, .0f, 0);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, -1, -1);
	glEnd();

	// Y+
	glBegin(GL_POLYGON);
	glColor3f(.0f, .0f, 0);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	glEnd();

}
void RobotArm::DrawLowerArm() {
	glPushMatrix();
	glRotatef(-90, 0, 0, 0);
	//glEnable(GL_LIGHTING);
	glColor3f(0.3, 0.3, 0.2);
	//SetMaterial(&redPlasticMaterial);
	gluCylinder(lowerArm, 0.35, 0.35, 1.95, 64, 64);
	//glDisable(GL_LIGHTING);
	glPopMatrix();
}
void RobotArm::DrawUpperArm() {
	glPushMatrix();
	DrawJoint();
	//glEnable(GL_LIGHTING);
	//SetMaterial(&redPlasticMaterial);
	glColor3f(0.1, 0.5, 0.2);
	gluCylinder(upperArm, 0.18, 0.18, 2, 64, 64);
	//glDisable(GL_LIGHTING);
	glPopMatrix();
}

void RobotArm::DrawJoint() {
	glPushMatrix();
	//glEnable(GL_LIGHTING);
	SetMaterial(&yellowPlasticMaterial);
	glColor3f(0.3, 0.3, 0.2);
	gluSphere(joint, 0.5, 64, 64);
	//glDisable(GL_LIGHTING);
	glPopMatrix();
}

float degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise!
	return (float)(degAngle / 180.0 * pi);
}

void RobotArm::MoveForward(WPARAM wParam) {
	switch (wParam)
	{
	case VK_UP:
		if (currentVelo > 0.1) {
			CurrentState = AccState::Max;
		}
		else {
			CurrentState = AccState::Dcc;
		}
		break;
	case VK_DOWN:
		if (currentVelo < -0.1) {
			CurrentState = AccState::Max;
		}
		else {
			CurrentState = AccState::Acc;
		}
		break;
	}
}

void RobotArm::HandleKeyWASD(WPARAM wParam) 
{
	// Handle the various key-presses
	if (wParam == VK_LEFT) //Body left 
	{
		Rotate(5);
		//yRotBase -= 5;
		//xTranslateBase -= 5;
	}
	if (wParam == VK_RIGHT) //Body right
	{
		Rotate(-5);
		//yRotBase += 5; 
		//xTranslateBase += 5;
	}
	if (wParam == VK_UP)
	{
		MoveForward(VK_UP);
	}
	if (wParam == VK_DOWN)
	{
		MoveForward(VK_DOWN);
	}
	if (wParam == 'W') //Turret up 
	{
		if(rotLowerArm > -45)
		rotLowerArm -= 3;
	}
	if (wParam == 'S') //Turret down 
	{
		if (rotLowerArm < 0)
		rotLowerArm += 3;
	}
	if (wParam == 'D')//Turret right
	{
		rotUpperArm -= 3;
	}
	if (wParam == 'A')//Turret left 
	{
		rotUpperArm += 3;
	}
	// Update the rotation of the upper adrm (Right and left)
	glLoadIdentity();
	//glLoadIdentity();
	//glTranslatef(xPos, yPos, zPos);
	
	//glMultMatrixf(upperArmNode->matrix);
	glRotatef(rotUpperArm, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperArmNode->matrix);// get & stores transform
	

	
	glTranslatef(0, 1.5, 0);
	glRotatef(rotLowerArm, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperArmNode->matrix);// get & stores transform
	
	
}
void RobotArm::HandleKeyRelease(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
		CurrentState = AccState::SS;
	case VK_DOWN:
		CurrentState = AccState::SS;
		break;
	}
}

void RobotArm::Rotate(double angle) {
	// Update the rotation (yaw)
	yRotation = yRotation + angle;

	glPushMatrix();
	// Update the position & rotation of the complete robot arm
	glLoadIdentity();
	glTranslatef(xPos, yPos + 0.075, zPos);
	glRotatef(yRotation + 180, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform *
	glPopMatrix();
}



void RobotArm::Draw() {
	DrawTree(tree);

	double timePassedAbsInSeconds = clock->GetTimePassedSinceStart();
	t = timePassedAbsInSeconds * 0.001;

	switch (CurrentState)
	{
	case Acc:
		if (currentVelo > 0.1) {
			CurrentState = AccState::Max;
		}
		else {
			currentVelo += t;
		}
		break;
	case Dcc:
		if (currentVelo < -0.1) {
			CurrentState = AccState::Max;
		}
		else {
			currentVelo -= t;
		}
		break;
	case Max:
		break;	
	
	case StandStill:
		currentVelo = 0;
			break;
	case SS:
		if (currentVelo > 0) {
			currentVelo -= t * 0.3;
		}
		else if (currentVelo < 0)
		{
			currentVelo += t * 0.3;
		}
		break;
	}

	// Calculate translation as based on current yRotation angle
	deltaX = sin(degToRad(yRotation)) * currentVelo;
	deltaZ = cos(degToRad(yRotation)) * currentVelo;
	// Update the position
	xPos = (xPos + deltaX);
	zPos = (zPos + deltaZ);

	glPushMatrix();
	// Update the position & rotation of the complete robot arm
	glLoadIdentity();
	glTranslatef(xPos, yPos + 0.075, zPos);
	glRotatef(yRotation + 180, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, tree->matrix);// get & stores transform *
	glPopMatrix();
}