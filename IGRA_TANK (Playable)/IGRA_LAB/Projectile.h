#pragma once
#include "stdafx.h"

const static float gravity = 9.81;

class Projectile {
public:
	GLUquadric * quad;

	float globalPosX, globalPosY, globalPosZ;	//GLOBAL START POS
	float localDistX = 0, localDistY = 0; //RELATIVE DISTANCE

	float velocityX, velocityY, maxVelocityX = 3, maxVelocityY = 3;
	float yawAngle, tiltAngle; //YAW, UP, TILT, RIGHT

	Material yellowPlasticMaterial; 

	enum PROECTILESTAT { NOTFIRED, FIRED ,LANDED};
	PROECTILESTAT currentStat = NOTFIRED; 

	void Update(float deltaTime);	//UPDATE, HOLD DRAW
	void Draw();	//DRAW AND SIMULATE PHYSICS
	void Spawn(float posX, float posY, float posZ); //SET NEW POSITION
	void Fire(); //FIRE, MOVE THE PROJECTILE

	void setMaterial(Material* m);	//SET MATERIAL
	void setVelocity(float veloX, float veloY); //SET NEW VELOCITY

	void DrawCylinder(double radius, double height, double slice, double stack); //DRAW CYLINDER

	void HandleKeyDown(WPARAM wParam, float YawAngle, float TiltAngle);	//EVENT HANDLER

	Projectile();
	~Projectile();
};