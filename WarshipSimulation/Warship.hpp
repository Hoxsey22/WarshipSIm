# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef _MISSILE_
# define _MISSILE_
# endif

#define _WARSHIP_

class Warship {
private:

	int id;
	Missile * missile;
	glm::mat4 target[2];
	float targetBR[2];
	int activeTimer = 0;
	int missileSpeed = 20;
	int missileCount = 10;
	int targetHit = -1;
	float missileModelBR;
	float modelBR;
	int missileActive = 0;
	glm::mat4 rotationMatrix;
	glm::mat4 spinMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 initialMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::vec3 rotationAxis;
	glm::vec3 spinAxis;
	glm::vec3 speedVector;
	glm::vec3 translationVector;
	glm::vec3 color;
	glm::mat4 identity;  
	glm::vec3 scale;
	float radians;
	int speed, pitch, yaw, roll;

public:

	Warship(float mBR, glm::vec3 translationVector) {
		Warship::translationVector = translationVector;
		modelBR = (100.0 * 1.0f) / mBR;
		pitch = 0;
		yaw = 0;
		roll = 0;
		identity = glm::mat4();
		scale = glm::vec3(modelBR);
		scaleMatrix = glm::scale(identity, scale);	// vector to scale the objects
		rotationMatrix = identity;	// initializes the rotation matrix to the identity
		rotationAxis = glm::vec3(pitch, yaw, roll);	// rotation axis vector so it rotates about the Y
		speedVector = glm::vec3(0, 0, 0);
		spinMatrix = identity;	// initializes the spin matrix to the identity
		//Sphere3D::translationVector = translationVector;	// initializes the translate vector

		translationMatrix = glm::translate(identity, translationVector);	// initializes the translation matrix for objects position
		orientationMatrix = translationMatrix;
		radians = 0.02f;	// initializes the radians of the object
		speed = 10;
		
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////											Missile															///////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// function creates the missile and assigns the targets that the missile needs to track
	// updates the missile count as well 
	void createMissile()	{
		if (missileActive == 0)	{
			if (missileCount > 0)
				missile = new Missile(missileModelBR, orientationMatrix, missileSpeed);
			missileCount--;
			missileActive = 1;
			assignTarget();
		}
	}
	
	// function assigns the targets in which the missile will track
	// get the mat4 and model bounding radius from the object
	void assignTarget()	{
		missile->getTargets(target, targetBR, 2);
	}
	
	// function to set the missile's bounding radius
	void setMissileBR(float modelBR)	{
		missileModelBR = modelBR;
	}

	// function to see if the missile is active or not
	// returns int missileActive
	int isMissileActive()	{
		return missileActive;
	}

	// function gets the missile in which the warship uses
	// return Missile Warship::missile
	Missile * getMissile()	{
		return Warship::missile;
	}

	// function set the warship's target in which it will become
	// the missile's target
	void setMissileTargets(glm::mat4 tar[], float mBR[])	{
		for (int i = 0; i < 2; i++)	{
			target[i] = tar[i];
			targetBR[i] = mBR[i];
		}
	}

	void destoryMissile()	{
		targetHit = missile->targetHit();
		missile->destroyMissile();
		missile = NULL;
		activeTimer = 0;
		missileActive = 0;
		printf("MISSILE IS DEAD!\n");
	}

	int getTargetHit()	{
		return targetHit;
	}

	bool isWarshipHit()	{

	}
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// function to set the scaleMatrix in order to adjust the size of the object
	void setScaleMatrix(glm::vec3 scaleVector)	{
		scaleMatrix = glm::scale(identity, scaleVector);
	}

	// function that return the scale matrix 
	//	returns glm:: mat4
	glm::mat4 getScaleMatrix()	{
		return scaleMatrix;
	}

	// function get the warship's position by getting the orientation matrix
	// and using the function getPosition(glm::mat4);
	// return glm::vec3
	glm::vec3 getWarshipPosition()	{
		return getPosition(orientationMatrix);
	}

	// function that sets the new translation
	void setTranslationMatrix(glm::vec3 newVector)	{
		orientationMatrix = glm::translate(getOrientationMatrix() , newVector);
	}

	// function transform the orientation matrix using the new vector given
	void transformPosition(glm::vec3 transformVec)	{
		orientationMatrix[3][0] += transformVec.x;
		orientationMatrix[3][1] += transformVec.y;
		orientationMatrix[3][2] += transformVec.z;

	}

	// function changes the speed of the warship
	void increaseSpeed()	{
		switch (speed)	{
		case 10: speed = 50; break;
		case 50: speed = 200; break;
		case 200: speed = 10; break;
		}
	}

	// function moves the warship to the new position by the given matrix
	void warp(glm::mat4 warpMatrix)	{
		translationMatrix = glm::translate(identity, getPosition(warpMatrix));
		rotationAxis = glm::vec3(1,0,0);
		rotationMatrix = glm::rotate(identity,3*PI/2,rotationAxis);
		orientationMatrix = translationMatrix * rotationMatrix;
	}

	// function changes the speed vector so that the warship moves forward
	void accelerate()	{
		speedVector = glm::vec3(0,0,-speed);
	}

	// function changes the speed vector so that the warship moves backward
	void reverse()	{
		speedVector = glm::vec3(0, 0, speed);
	}

	// function changes the yaw of the rotation axis to move to the left
	void turnLeft()	{
		yaw = 1;
	}

	// function changes the yaw of the rotation axis to move to the right
	void turnRight()	{
		yaw = -1;
	}

	// function changes the roll of the rotation axis to move to roll left
	void rollLeft()	{
		roll = -1;
	}

	// function changes the roll of the rotation axis to move to roll right
	void rollRight()	{
		roll = 1;
	}

	// function changes the pitch of the rotation axis to move the warship up
	void pitchUp()	{
		pitch = 1;
	}

	// function changes the pitch of the rotation axis to move the warship down
	void pitchDown()	{
		pitch = -1;
	}

	// funtion sets the new rotation matrix
	void setRotationMatrix(glm::mat4 newRotationMatrix, glm::vec3 newVector)	{
		rotationMatrix = glm::translate(newRotationMatrix, newVector);
	}

	// function return the tranlation matrix 
	// returns glm:: mat4
	glm::mat4 getTranslationMatrix()	{
		return translationMatrix;
	}

	// function return the orientation matrix 
	// returns glm:: mat4
	glm::mat4 getOrientationMatrix()	{
		return orientationMatrix;
	}

	// function return the rotational matrix 
	// returns glm:: mat4
	glm::mat4 getRotationMatrix()	{
		return rotationMatrix;
	}

	// function set the new radians
	void setRadians(float radians)	{
		Warship::radians = radians;
	}

	// function return the radian 
	// returns float
	float getRadians()	{
		return radians;
	}

	// function return the model bounding radius
	// returns float
	float getModelBR()	{
		return modelBR;
	}
	// function return the model matrix made of the orientation and scale matrix
	// return glm::mat4
	glm::mat4 getModelMatrix() {
		return(orientationMatrix * scaleMatrix);
	}

	void update() {
		if (yaw != 0 || pitch != 0 || roll != 0)	{
			rotationAxis = glm::vec3(pitch, yaw, roll);
			rotationMatrix = glm::rotate(identity, radians, rotationAxis);
		}
		else
		{
			rotationMatrix = identity;
		}
		translationMatrix = glm::translate(orientationMatrix, speedVector);
		orientationMatrix = translationMatrix*rotationMatrix;
		speedVector = glm::vec3(0,0,0);
		pitch = yaw = roll = 0;
		if (isMissileActive() > 0)	{
			missile->update();
			activeTimer++;
			printf("timer: %d\n",activeTimer );
			if (activeTimer == 2000 || missile->targetHit())	{
				missile->destroyMissile();
				missile = NULL;
				activeTimer = 0;
				missileActive = 0;
				printf("MS MISSILE IS DEAD!\n");
			}
		}
		
		//spinMatrix = glm::rotate(spinMatrix, radians, spinAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
		//radians = 0.0f;
	}
};