/*
__________________________________

**********************************
[000] Done by Zhen Xiong, P1646364 (Projectile)
**********************************
__________________________________
*/
#include "stdafx.h"
#include "Projectile.h"

//UPDATE, DRAW THE PROJECTILE AND IMPLEMENT PHYSICS
void Projectile::Update(float deltaTime)
{
	Draw();

	//IMPLEMENT PHYSICS
	if (Projectile::currentStat == Projectile::PROECTILESTAT::FIRED)
	{	
		double newPosY = tiltAngle * 0.1;
		double posY = globalPosY + localDistY;


 		if (posY > newPosY) {
			localDistX += deltaTime * velocityX;
			localDistY += deltaTime * velocityY;

			////APPLY THE PHYSICS IN 2D
			if (velocityY > -maxVelocityY)
				velocityY -= 0.5 * deltaTime;
			else {
				velocityY = -maxVelocityY;
			}
		}
		else {
			posY = posY;
			newPosY = newPosY;
		}
	}
}

//DRAW
void Projectile::Draw() {
	if (currentStat == PROECTILESTAT::FIRED) {
		glPushMatrix();

		glTranslatef(globalPosX, globalPosY, globalPosZ);

		//ROTATE DIRECTION TO MOVE
		glRotatef(yawAngle + 90, 0, 1, 0);

		//FIND BARREL
		glRotatef(-tiltAngle, 0, 0, 1);
		glTranslatef(2, 0, 0);
		
		glTranslatef(localDistX, localDistY, 0);

		//ROTATE TO FACE
		glRotatef(90, 0, 1, 0);

		if (velocityY * -50 < 90) {
			//ROTATE TO FACE UP DOWN
			glRotated(velocityY * -50, 1, 0, 0);
		}
		else {
			glRotated(90, 1, 0, 0);
		}

		DrawCylinder(0.3, 0.75, 10, 10);
		glPopMatrix();
	} 
}

//FIRE THE PROJECTILE
void Projectile::Fire() {
	velocityY = tiltAngle * velocityY;

	currentStat = PROECTILESTAT::FIRED;
}

//SPAWN THE PROJECTILE
void Projectile::Spawn(float posX, float posY, float posZ) {
	globalPosX = posX;
	globalPosY = posY;
	globalPosZ = posZ;
}

//ASSIGN THE VELOCITY
void Projectile::setVelocity(float veloX, float veloY) {
	velocityX = veloX;
	velocityY = veloY;
}

//CYLINDER
void Projectile::DrawCylinder(double radius, double height, double slice, double stack)
{
	setMaterial(&yellowPlasticMaterial);
	quad = gluNewQuadric();

	gluCylinder(quad, radius, radius, height, slice, stack);
}

//HANDLE INPUT
void Projectile::HandleKeyDown(WPARAM wParam, float YawAngle, float TiltAngle)
{
	if (Projectile::currentStat == Projectile::PROECTILESTAT::NOTFIRED)
	{
		//FIRE GUN
		if (wParam == VK_SPACE)
		{
			Projectile* p = this;
			Fire();
			yawAngle = YawAngle;
			tiltAngle = TiltAngle;
		}
	}
}

//ASSIGN MATERIAL
void Projectile::setMaterial(Material* m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
}

//SET WHEN INITIATED
Projectile::Projectile()
{
	currentStat = PROECTILESTAT::NOTFIRED;
}

Projectile::~Projectile()
{

}

