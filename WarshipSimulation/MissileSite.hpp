# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef _MISSILE_
# define _MISSILE_
# endif

# define _MISSILESITE_

class MissileSite {
private:

	int id;
	Missile * missile;
	glm::mat4 target;
	bool destoryed = false;
	float targetBR;
	const float detectionRadius = 2000;
	int activeTimer = 0;
	int missileCount = 10;
	int missileSpeed = 5;
	float missileModelBR;
	float modelBR;
	int missileActive = 0;
	glm::mat4 detectionMatrix;
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
	glm::vec3 detectionVector;
	float radians;
	int speed, pitch, yaw, roll;

public:

	MissileSite(float mBR, glm::vec3 translationVector, float radians) {
		MissileSite::translationVector = translationVector;
		modelBR = (30 * 1.0f) / mBR;// *1.0f) / mBR;
		MissileSite::radians = radians;
		identity = glm::mat4();
		scale = glm::vec3(modelBR);
		detectionVector = glm::vec3((detectionRadius * 1.0f) / mBR);
		detectionMatrix = glm::scale(identity, detectionVector);
		scaleMatrix = glm::scale(identity, scale);	// vector to scale the objects
		rotationMatrix = identity;	// initializes the rotation matrix to the identity
		rotationAxis = glm::vec3(0, 1, 0);	// rotation axis vector so it rotates about the Y

		translationMatrix = glm::translate(identity, translationVector);	// initializes the translation matrix for objects position
		orientationMatrix = translationMatrix;

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
		glm::mat4 tempTarget[1] = { target };
		float tempTargetBR[1] = { targetBR };
		missile->getTargets(tempTarget, tempTargetBR, 1);
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
		return MissileSite::missile;
	}

	// function set the warship's target in which it will become
	// the missile's target
	void setMissileTargets(glm::mat4 tar, float tarBR)	{
		target = tar;
		targetBR = tarBR;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool isCollided(glm::mat4 tar, float tarBR)	{
		float getDistance;
		float rad = tarBR;

		getDistance = distance(getPosition(tar), getPosition(getDetectionMatrix()));
		rad = detectionRadius;
		if (getDistance < rad + detectionRadius)
			return true;
		return false;

	}

	bool isDestoryed()	{
		return destoryed;
	}

	void destoryThis()	{
		destoryed = true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// function to set the scaleMatrix in order to adjust the size of the object
	void setScaleMatrix(glm::vec3 scaleVector)	{
		scaleMatrix = glm::scale(identity, scaleVector);
	}

	// function that return the scale matrix 
	//	returns glm:: mat4
	glm::mat4 getScaleMatrix()	{
		return scaleMatrix;
	}

	// function get the missile site's position by getting the orientation matrix
	// and using the function getPosition(glm::mat4);
	// return glm::vec3
	glm::vec3 getMissileSitePosition()	{
		return getPosition(orientationMatrix);
	}

	// function that sets the new translation
	void setTranslationMatrix(glm::vec3 newVector)	{
		orientationMatrix = glm::translate(getOrientationMatrix(), newVector);
	}

	// function transform the orientation matrix using the new vector given
	void transformPosition(glm::vec3 transformVec)	{
		orientationMatrix[3][0] += transformVec.x;
		orientationMatrix[3][1] += transformVec.y;
		orientationMatrix[3][2] += transformVec.z;

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
		MissileSite::radians = radians;
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

	glm::mat4 getDetectionMatrix() {
		return(orientationMatrix * detectionMatrix);
	}

	// function return the model matrix made of the orientation and scale matrix
	// return glm::mat4
	glm::mat4 getModelMatrix() {
		return(orientationMatrix * scaleMatrix);
	}

	void update() {
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(orientationMatrix, speedVector);
		orientationMatrix = rotationMatrix * translationMatrix;
		getDetectionMatrix();
		if (isMissileActive() > 0)	{
			missile->update();
			activeTimer++;
		}
		if (activeTimer == 200)	{
			missile->destroyMissile();
			missile = NULL;
			activeTimer = 0;
			missileActive = 0;
			printf("MS MISSILE IS DEAD!\n");
		}
		getDetectionMatrix();
	}
};