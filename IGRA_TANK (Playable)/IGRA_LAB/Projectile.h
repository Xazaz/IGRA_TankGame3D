#pragma once
#include "stdafx.h"

const static float gravity = 9.81;

class Projectile {
public:
	GLUquadric * quad;

	float globalPosX, globalPosY, globalPosZ;
	float localDistX = 0, localDistY = 0;

	float velocityX, velocityY, maxVelocityX = 3, maxVelocityY = 3;
	float yawAngle, tiltAngle;

	Material yellowPlasticMaterial;

	enum PROECTILESTAT { NOTFIRED, FIRED ,LANDED};
	PROECTILESTAT currentStat = NOTFIRED;

	void Update(float deltaTime);
	void Draw();
	void Spawn(float posX, float posY, float posZ);
	void Fire();

	void setMaterial(Material* m);

	void DrawCylinder(double radius, double height, double slice, double stack);

	void HandleKeyDown(WPARAM wParam, float YawAngle, float TiltAngle);

	Projectile();
	~Projectile();
};