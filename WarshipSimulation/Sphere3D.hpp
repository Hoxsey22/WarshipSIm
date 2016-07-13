# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# define SPHERE3D_HPP

class Sphere3D {

private:

	int id;
	float boundingRadius;
	glm::mat4 rotationMatrix;
	glm::mat4 spinMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 initialMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 OrientationMatrix;
	glm::vec3 rotationAxis;
	glm::vec3 spinAxis;
	glm::vec3 translationVector;
	glm::vec3 color;
	glm::mat4 identity;
	float radians;
	bool orbital = true;

public:

	Sphere3D(float scale, float modelBR, glm::vec3 translationVector, float radians) {

		identity = glm::mat4();
		boundingRadius = modelBR;
		scaleMatrix = glm::scale(identity, glm::vec3((scale * 1.0f) / modelBR));	// vector to scale the objects
		rotationMatrix = identity;	// initializes the rotation matrix to the identity
		rotationAxis = glm::vec3(0, 1, 0);	// rotation axis vector so it rotates about the Y
		spinAxis = glm::vec3(1, 0, 0);	// spin axis vector so it rotates about the X
		spinMatrix = identity;	// initializes the spin matrix to the identity
		Sphere3D::translationVector = translationVector;	// initializes the translate vector

		translationMatrix = glm::translate(identity, translationVector);	// initializes the translation matrix for objects position
		Sphere3D::radians = radians;	// initializes the radians of the object
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

	float getBoundingRadius()	{
		return boundingRadius;
	}


	void setTranslationMatrix(glm::mat4 newTranslation, glm::vec3 newVector)	{
		translationMatrix = glm::translate(newTranslation, newVector);
	}

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
		return OrientationMatrix;	// rotational * translation matrix
	}

	// function that return the rotational matrix 
	//	returns glm:: mat4
	glm::mat4 getRotationMatrix()	{
		return rotationMatrix;
	}
	void setRadians(float radians)	{
		Sphere3D::radians = radians;
	}

	// function that return the radian 
	//	returns float
	float getRadians()	{
		return Sphere3D::radians;
	}

	glm::mat4 getModelMatrix() {
		if (orbital) // orbital rotation
			return(rotationMatrix * translationMatrix * scaleMatrix);
		else  // center rotation
			return(translationMatrix * rotationMatrix * scaleMatrix);
	}
	glm::mat4 getModelMatrix(glm::mat4 distanceMatrix) {
		return(distanceMatrix * rotationMatrix * translationMatrix * scaleMatrix);
	}

	void update() {
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		OrientationMatrix = rotationMatrix*translationMatrix;
		//spinMatrix = glm::rotate(spinMatrix, radians, spinAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};