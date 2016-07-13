# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define _MISSILE_

class Missile {

private:

	int id;
	int isLive = 1;
	int isHit = -1;
	int activeTimer = 0;
	int targetLockedOn = 0;
	int numberOfTargets = 0;
	int targetName = -1;
	int speed, pitch, yaw, roll;

	bool isCollided = false;

	const glm::mat4 identity = glm::mat4();
	const float detectionRadius = 2000;
	const float boundingRadius = 300;

	float radians;
	float targetBR[2];
	float lockedOnTargetBR;
	float modelBR;

	glm::mat4 target[2];
	glm::mat4 lockedOnTarget;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 detectionMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::vec3 rotationAxis;
	glm::vec3 speedVector;
	glm::vec3 translationVector;
	glm::vec3 detectionVector;
	glm::vec3 scale;
	
	

public:

	Missile(float mBR, glm::mat4 orientationMat, int spd) {
		isLive = 1;
		modelBR = (boundingRadius * 1.0f) / mBR;
		scale = glm::vec3(modelBR);
		detectionVector = glm::vec3((detectionRadius * 1.0f) / mBR);
		scaleMatrix = glm::scale(identity, scale);	// vector to scale the objects
		detectionMatrix = glm::scale(identity, detectionVector);
		rotationMatrix = identity;	// initializes the rotation matrix to the identity
		rotationAxis = glm::vec3(pitch, yaw, roll);	// rotation axis vector so it rotates about the Y

		translationMatrix = identity;	// initializes the translation matrix for objects position
		orientationMatrix = orientationMat;
		radians = 0.02f;	// initializes the radians of the object
		speed = spd;
		speedVector = glm::vec3(0, 0, -speed);
	}

	void getTargets(glm::mat4 tar[], float tarBR[], int numberOfTargets)	{
		Missile::numberOfTargets = numberOfTargets;
		for (int i = 0; i < numberOfTargets; i++)	{
			target[i] = tar[i];
			targetBR[i] = tarBR[i];
		}

	}

	int checkDetectionRadius(glm:: mat4 tar[], float tarBR[] , int nTargets)	{

		for (int i = 0; i < nTargets; i++)	{
			if (collision(tar[i], tarBR[i], 0))	{
				chase(tar[i]);
				printf("MISSILE FOUND SITE %d\n",i);
				return i;
			}
		}
		return -1;
		
	}

	// function tests for collision from cases
	// case 0: tests for collision if the missile is looking for a target in its detection radius
	// case 1: tests for collision if the missile has it's target and when it collides with the target
	// return int; 1 for collision, 0 no collision
	bool collision(glm::mat4 tar, float tarBR, int whichBR)	{
		float getDistance;
		float rad;
		switch (whichBR)	{
		case 0:
			getDistance = distance(getPosition(tar), getPosition(getDetectionMatrix()));
			rad = detectionRadius;
			break;
		case 1:	
			getDistance = distance(getPosition(tar),getPosition(getOrientationMatrix()));
			rad = tarBR;
			break;
		}
		if (getDistance < rad + modelBR)
			return true;
		return false;
		
	}

	void chase(glm::mat4 tar)	{
		float newRadian;
	//	showMat4("Chase target: ", tar);
		//get the foward vector of the missile
		glm::vec3 missileForwardVec = getIn(getOrientationMatrix());

		//get the distance vector between the missile and the target
		glm::vec3 distanceVec = getPosition(tar) - getMissilePosition();

		//normalizethe missile's forward and distance vector
		glm::vec3 targetNormal = glm::normalize(distanceVec);
		glm::vec3 missileNormal = glm::normalize(missileForwardVec);

		//showVec3("Target", targetNormal);
	//	showVec3("Missile", missileNormal);

		if (!(colinear(targetNormal, missileNormal, 0.1f)) )	{
			//showVec3("distance Vec Normal",targetNormal);
			//gets the cross product to find the axis of rotation
			//glm::vec3 axisOfRotation = glm::cross(targetNormal, missileNormal);
			glm::vec3 axisOfRotation = glm::cross(missileNormal, targetNormal);
			axisOfRotation = glm::normalize(axisOfRotation);

			//gets the axis of rotation's direction
			float axisOfRotationDirection = axisOfRotation.x + axisOfRotation.y + axisOfRotation.z;
			//printf("AOR-Direction: %f\n",axisOfRotationDirection);

			// get the dot product to find the rotation amount
			float totalRotation = glm::dot(missileNormal, targetNormal); 
			//float totalRotation = glm::dot(targetNormal, missileNormal);
			// get the length of the missile and target's normal
			float LOMN = glm::abs(distance(missileNormal, glm::vec3(0, 0, 0)));
			float LOTN = glm::abs(distance(targetNormal, glm::vec3(0, 0, 0)));


			//float totalRotation = glm::acos(glm::dot(targetNormal, missileNormal));
			//printf("Total Rotation: %f\n", totalRotation);
			//printf("AOR Direction: %f\n", axisOfRotationDirection);
			//check for the direction in which the missile is suppose to move in
			if (axisOfRotationDirection >= 0.01)
				newRadian = glm::acos(totalRotation);
			else
				newRadian = - glm::acos(totalRotation);

			rotationMatrix = glm::rotate(identity, (newRadian / 4.0f), axisOfRotation);
		}
		else
		{
			rotationMatrix = identity;
			//printf("colinear\n");
		}
			

	}

	// function to set the scaleMatrix in order to adjust the size of the object
	void setScaleMatrix(glm::vec3 scaleVector)	{
		scaleMatrix = glm::scale(identity, scaleVector);
	}

	// function that return the scale matrix 
	//	returns glm:: mat4
	glm::mat4 getScaleMatrix()	{
		return scaleMatrix;
	}

	glm::vec3 getMissilePosition()	{
		return getPosition(orientationMatrix);
	}

	// function that sets the new translation
	// glm::mat4 newTranslation & glm::vec3 newVector
	void setTranslationMatrix(glm::vec3 newVector)	{
		translationMatrix = glm::translate(getOrientationMatrix(), newVector);
	}
	void increaseSpeed()	{
		switch (speed)	{
		case 10: speed = 50; break;
		case 50: speed = 200; break;
		case 200: speed = 10; break;
		}
	}
	void warp(glm::mat4 warpMatrix)	{
		translationMatrix = glm::translate(identity, getPosition(warpMatrix));
		rotationAxis = glm::vec3(1, 0, 0);
		rotationMatrix = glm::rotate(identity, PI / 2, rotationAxis);
		orientationMatrix = translationMatrix * rotationMatrix;
	}

	void accelerate()	{
		speedVector = glm::vec3(0, 0, -speed);
	}

	void reverse()	{
		speedVector = glm::vec3(0, 0, speed);
	}
	void turnLeft()	{
		yaw = 1;
	}
	void turnRight()	{
		yaw = -1;
	}
	void rollLeft()	{
		roll = -1;
	}
	void rollRight()	{
		roll = 1;
	}
	void pitchUp()	{
		pitch = 1;
	}
	void pitchDown()	{
		pitch = -1;
	}

	// funtion that sets the new rotation matrix
	// glm::mat4 newRotationMatrix & glm::vec3 newVector
	void setRotationMatrix(glm::mat4 newRotationMatrix, glm::vec3 newVector)	{
		rotationMatrix = glm::translate(newRotationMatrix, newVector);
	}

	// function that return the tranlation matrix 
	//	returns glm:: mat4
	glm::mat4 getTranslationMatrix()	{
		return translationMatrix;
	}

	// function that return the orientation matrix 
	//	returns glm:: mat4
	glm::mat4 getOrientationMatrix()	{
		return orientationMatrix;	// rotational * translation matrix
	}

	// function that return the rotational matrix 
	//	returns glm:: mat4
	glm::mat4 getRotationMatrix()	{
		return rotationMatrix;
	}

	// function that set the new Radians
	// float radians
	void setRadians(float radians)	{
		Missile::radians = radians;
	}

	// function that return the radian 
	//	returns float
	float getRadians()	{
		return radians;
	}

	void destroyMissile()	{
		lockedOnTarget = identity;
		targetLockedOn = 0;
	}

	int targetHit()	{
		return isCollided;
	}

	glm::mat4 getDetectionMatrix() {
		return(orientationMatrix * detectionMatrix);
	}

	glm::mat4 getModelMatrix() {
		return(orientationMatrix * scaleMatrix);
	}

	void update() {
		/*
		activeTimer++;
		if (activeTimer == 200)
			printf("MISSILE ACTIVE!");
		if (activeTimer > 200)
			detectionSystem();
		*/
		translationMatrix = glm::translate(identity, speedVector);
		orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;
		
	}
};