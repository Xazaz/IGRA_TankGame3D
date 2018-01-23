#pragma once
#define DRAW_BASE_FUNCTION_ID 0
#define DRAW_LOWERARM_FUNCTION_ID 1
#define DRAW_UPPERARM_FUNCTION_ID 2
#define DRAW_JOINT_FUNCTION_ID 3


class RobotArm
{
public:

	float yRotBase;
	float rotLowerArm;
	float rotUpperArm;
	float xTranslateBase;
	float zTranslateBase;
	float xPos;
	float yPos;
	float zPos;
	double yRotation;

	RobotArm();
	~RobotArm();
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void DrawJoint();
	void Draw();
	void DrawBase();
	void DrawLowerArm();
	void DrawUpperArm();

	void Rotate(double angle);
	void MoveForward(double dist);
	//void HandleKeyDown(WPARAM wParam);
	void HandleKeyWASD(WPARAM wParam);
	void MoveForward(double dist, WPARAM wParam);
	void CreateBaseTexture();
	void SetMaterial(Material *m);
	// Following Nodes need to available in HandleKeyDown
	TreeNode* tree;
	TreeNode* lowerArmNode;
	TreeNode* upperArmNode;

	Material  yellowPlasticMaterial = { { 0.0f ,0.0f ,0.0f ,1.0f},
										{ 0.5f ,0.5f ,0.0f ,1.0f},
										{ 0.6f ,0.6f ,0.5f ,1.0f},
										32.0f
	};
	Material  redPlasticMaterial =	{	{ 0.4f, 0.0f, 0.0f, 1.0f }, // Ambient
										{ 0.5f, 0.0f, 0.0f, 1.0f }, // Diffuse
										{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular
										32 // Shininess
	};
	Material  greenPlasticMaterial = {	{ 0.0f ,0.4f, 0.0f, 1.0f }, // Ambient
										{ 0.0f, 0.5f, 0.0f, 1.0f }, // Diffuse
										{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular
										32 // Shininess
	};
	Material  whitePlasticMaterial = {	{ 0.8f ,0.8f, 0.8f, 1.0f }, // Ambient
										{ 0.5f, 0.5f, 0.5f, 1.0f }, // Diffuse
										{ 0.6f ,0.6f ,0.5f ,1.0f }, // Specular
											32 // Shininess
	};
	GLUquadricObj *base = gluNewQuadric();
	GLUquadricObj *baseCover = gluNewQuadric();
	GLUquadricObj *lowerArm = gluNewQuadric();
	GLUquadricObj *upperArm = gluNewQuadric();
	GLUquadricObj *joint = gluNewQuadric();

	// Texture image
	GLubyte image[64][64][3];
};

