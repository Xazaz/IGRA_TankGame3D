#include "stdafx.h"
#include "Projectile.h"

void Projectile::Update(float deltaTime)
{
	Draw();

	if (Projectile::currentStat == Projectile::PROECTILESTAT::FIRED)
	{

		////APPLY THE PHYSICS IN 2D
		localDistX += deltaTime * velocityX;
		localDistY += deltaTime * velocityY;

		if (velocityY > -maxVelocityY)
			velocityY -= 0.5 * deltaTime;
		else {
			velocityY = -maxVelocityY;
		}
	}
}

void Projectile::Draw() {
	if (globalPosY + localDistY > -1) {
		glPushMatrix();

		//ROTATE DIRECTION TO MOVE
		glRotatef(yawAngle - 90
			, 0, 1, 0);

		glTranslatef(globalPosX, globalPosY, globalPosZ);

		glTranslatef(localDistX, localDistY, 0);

		//ROTATE TO FACE
		glRotatef(90, 0, 1, 0);

		//ROTATE TO FACE UP DOWN
		glRotated(velocityY * -50, 1, 0, 0);

		DrawCylinder(0.3, 0.75, 10, 10);
		glPopMatrix();
	}
}

void Projectile::Fire() {
	velocityX = 1;
	velocityY = 1;

	currentStat = PROECTILESTAT::FIRED;
}

void Projectile::Spawn(float posX, float posY, float posZ) {
	globalPosX = posX;
	globalPosY = posY;
	globalPosZ = posZ;

}

void Projectile::DrawCylinder(double radius, double height, double slice, double stack)
{
	setMaterial(&yellowPlasticMaterial);
	quad = gluNewQuadric();

	gluCylinder(quad, radius, radius, height, slice, stack);
}

void Projectile::HandleKeyDown(WPARAM wParam, float YawAngle, float TiltAngle)
{
	if (Projectile::currentStat == Projectile::PROECTILESTAT::NOTFIRED)
	{
		if (wParam == VK_SPACE)
		{
			Projectile* p = this;
			Fire();
			yawAngle = YawAngle;
			tiltAngle = TiltAngle;

		}
	}
}

void Projectile::setMaterial(Material* m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
}

Projectile::Projectile()
{
	currentStat = PROECTILESTAT::NOTFIRED;

	velocityX = 1;
	velocityY = 1;
}

Projectile::~Projectile()
{

}

