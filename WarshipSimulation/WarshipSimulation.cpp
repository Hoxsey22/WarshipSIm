/*

Author: Joseph Hoxsey


*/
# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"  

# include "Missile.hpp"
# include "Warship.hpp"
# include "MissileSite.hpp"
# include "Sphere3D.hpp"

char * bgFilename = "bgtexture.RAW";
GLuint loadCubemap();

GLuint skyboxTexture = loadCubemap();

int isTexture = 0;

GLfloat skyboxVertices[] = {
	// Positions          
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};
// Shapes
const int nSpheres = 5;	// Right now this includes the warship and the missile

// Objects
const int objectCount = 8;	// number of object that needs to be updated

// number of Missile Sites
const int nMissileSites = 2;

const int nWarships = 1;

int width = 1920, height = 1080;

Sphere3D * sphere[nSpheres];

// Warship
Warship * warship;

// Missile Site
MissileSite * missileSites[nMissileSites];

// Model for shapes
const int nModels = 8;
// name of tri model file
char * modelFile[nModels] = {
	"Ruber.tri",
	"Unum.tri", 
	"Duo.tri", 
	"Primus.tri", 
	"Secundus.tri", 
	"Warship.tri",
	"MissileBase.tri",
	"Missile.tri" 
}; 

const int nSphereVertices[nSpheres] = {
	264 * 3, // Ruber
	502 * 3, // Unum
	544 * 3, //Duo
	264 * 3, //Prim
	264 * 3 //Sec
};

const int warshipVertices = 1284 * 3;

const int missileSiteVertices = 2274 * 3;

const int missileVertices = 128 * 3;




float modelBR[nModels];  // modelFile's bounding radius
int Index = 0;  // global variable indexing into VBO arrays
int warpCounter = 0;

// display state and "state strings" for title display
// window title strings
char baseStr[50] = "Warship Simulator {v} : ";
char fpsStr[15], viewStr[15] = " front view";
char titleStr[100];

GLuint vao[nModels];  // VertexArrayObject
GLuint skyboxVAO;
GLuint skyboxBuffer;
GLuint buffer[nModels];
GLuint shaderProgram;
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";

char * skyboxVertexShaderFile = "JHvertexShader.glsl";//"simpleVertex.glsl";
char * skyboxFragmentShaderFile = "fragmentShader.glsl";//"simpleFragment.glsl";

GLuint MVP;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
GLuint Position[nModels], Color[nModels], Normal[nModels];   // vPosition, vColor, vNormal handles for models
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();


// vectors and values for lookAt
glm::vec3 eye, at, up;

// rotation variables
glm::mat4 identity();
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval;

// Vectors for the camera
glm::mat4 cameraViewUnum;
glm::mat4 cameraViewDuo;
glm::mat4 cameraViewWarship;
glm::mat4 cameraViewMissile;

// Index of the camera views
int cameraViewCounter = 0;

// Initialize the createSphere & createWarship function
void createSpheres();
void createWarship();
void createMissileSite();
void assignTargets();

// sphere sizes
const float sphereScales[nSpheres] = {
	2000.0f,	//Ruber
	200.0f, //Unum
	400.0f,	//Duo
	100.0f,	//Primus
	150.0f	//Secundus
};

bool wMissileActive = false;
bool msMissileActive[2] = {false, false};

int wMissileCount = 10;
int wMissileTimer = 0;
int wMissileTarget = 2;

int msMissileTimer[2] = { 0, 0 };
int msMissileTarget[2] = { 2, 2 };

Missile * wMissile;
Missile * msMissile[2];

void destroyPlayerMissile();

// Missile sizes
const float missileScales[2] = {
	30.0f, //Unum
	30.0f,	//Duo
};

// values for the time quantum
int timerMode = 0;
const int timeQuantum[4] = {
	40,	//ace
	100,	// pilot
	1000,	//250 trainee
	5000,	// 500 debug
};

// Warship's translation vector
const glm::vec3 warshipTranslationVector = glm::vec3(5000, 1000, 5000);

// Missile's scale and translation vector
const glm::vec3 missileScaleVector = glm::vec3(25, 25, 25);
const glm::vec3 missileTranslationVector = glm::vec3(4900, 1000, 4850);

int gravityToggle = 0;
const float gravity = 90000000.0f;
const float gravityVector[nSpheres] = {
	0.0f,	//Ruber
	5.63f, //Unum
	1.1f,	//Duo
	0.0f,	//Primus
	0.0f,	//Secundus
};

// sphere distance translation vectors
const glm::vec3 sphereTranslationVectors[nSpheres] = {
	glm::vec3(0, 0, 0),	//Ruber
	glm::vec3(4000, 0, 0), //Unum
	glm::vec3(-9000, 0, 0),	//Duo
	glm::vec3(-900, 0, 0),	//Primus
	glm::vec3(-1750, 0, 0)	//Secundus
};

// missile site distance translation vectors
const glm::vec3 missileSiteTranslationVectors[2] = {
	glm::vec3(4000, 230, 0),	//Site 1
	glm::vec3(-9000, 430, 0)	//Site 2
};

// distance translation vectors
const float sphereRadians[nSpheres] = {
	0.0f,	//Ruber
	0.004f, //Unum
	0.002f,	//Duo
	0.004f,	//Primus
	0.002f,	//Secundus
};

// distance translation vectors
const float missileSiteRadians[2] = {
	0.004f, //Site 1
	0.002f	//Site 2
};

GLuint loadCubemap()	{
	GLuint textureID;
	glGenTextures(1, &textureID);
	unsigned char * data;
	int sides = 6;
	FILE * file;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < sides; i++)	{
		data = (unsigned char *)malloc(width * height * 3);
		int readResult;
		file = fopen(bgFilename, "rb");
		readResult = fread(data, width * height * 3, 1, file);
		if (readResult != 1)	{
			printf("File %s was not read correctly\n");
			//return 0;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_FILTER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_FILTER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);

	return textureID;

}

/*
Initializes the shader programs, models, camera
*/
void init(void)	{
	///////////////////////LOADING SKYBOX//////////////////////////////////////////////////////////////////////
	//shaderProgram = loadShaders(skyboxVertexShaderFile,skyboxFragmentShaderFile);
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxBuffer);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(nModels, vao);
	glGenBuffers(nModels, buffer);

	// loaded sphere models into the buffers
	for (int i = 0; i < nSpheres; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nSphereVertices[i], vao[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		if (modelBR[i] == -1.0f) {
			printf("Sphere - loadTriModel error:  returned -1.0f \n");
			exit(1);
		}
		else
			printf("Sphere - loaded %s model with %7.2f bounding radius \n", modelFile, modelBR);
	}



	// loaded warship into the buffer
	modelBR[nSpheres] = loadModelBuffer(modelFile[nSpheres], warshipVertices,
		vao[nSpheres], buffer[nSpheres], shaderProgram, vPosition[nSpheres],
		vColor[nSpheres], vNormal[nSpheres], "vPosition", "vColor", "vNormal");
	if (modelBR[nSpheres] == -1.0f) {
		printf("Warship - loadTriModel error:  returned -1.0f \n");
		exit(1);
	}
	else
		printf("Warship - loaded %s model with %7.2f bounding radius \n", modelFile, modelBR);


	// loaded missile sites into the buffer
	int MSC = nSpheres + nWarships;	// easy counter to find where missile sites go
	modelBR[MSC] = loadModelBuffer(modelFile[MSC], missileSiteVertices, vao[MSC], buffer[MSC],
		shaderProgram, vPosition[MSC], vColor[MSC], vNormal[MSC], "vPosition", "vColor", "vNormal");
	if (modelBR[MSC] == -1.0f) {
		printf("Missile Site - loadTriModel error:  returned -1.0f \n");
		exit(1);
	}
	else
		printf("Missile Site - loaded %s model with %7.2f bounding radius \n", modelFile, modelBR);

	
	// loaded missile into the buffer
	int MC = MSC + 1;	// easy counter to find where missile go
	printf("%d", MC);

	modelBR[MC] = loadModelBuffer(modelFile[MC], missileVertices, vao[MC], buffer[MC],
			shaderProgram, vPosition[MC], vColor[MC], vNormal[MC], "vPosition", "vColor", "vNormal");
	if (modelBR[MC] == -1.0f) {
		printf("Missile - loadTriModel error:  returned -1.0f \n");
		int n;
		scanf("debug:", &n);
		exit(1);
	}
	else
		printf("Missile - loaded %s model with %7.2f bounding radius \n", modelFile, modelBR);


	// initially use a front view
	eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // eye is 1000 "out of screen" from origin
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
	up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
	viewMatrix = glm::lookAt(eye, at, up);
	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// create spheres
	createSpheres();

	// creates the warship
	createWarship();

	// creates the missile sites
	createMissileSite();

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time

}



/*
Gets the front camera view and updates the view matrix
*/
void updateFrontCameraView()	{
	eye = glm::vec3(0.0f, 10000.0f, 20000.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	if ((cameraViewCounter % 5) == 0)	{
		strcpy(viewStr, " Front view");
		viewMatrix = glm::lookAt(eye, at, up);
	}
}

/*
Gets the top camera view and updates the view matrix
*/
void updateTopCameraView()	{
	eye = glm::vec3(0.0f, 20000.0f, 0.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);   // looking at origin  
	up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's up is looking towards -Z vector
	if ((cameraViewCounter % 5) == 1)	{
		strcpy(viewStr, " Top view");
		viewMatrix = glm::lookAt(eye, at, up);
	}
}

/*
Gets Unum camera view and updates the view matrix
*/
void updateUnumCameraView()	{
	glm::vec3 unumPosition = getPosition(sphere[1]->getOrientationMatrix());
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), unumPosition);
	glm::mat4 translationMatrixtemp = glm::translate(translationMatrix, glm::vec3(0.0f, 4000.0f, 0.0f));

	cameraViewUnum = translationMatrixtemp;
	eye = getPosition(cameraViewUnum);   // eye is 4000 "out of screen" from Unum
	at = unumPosition;   // looking at Unum
	up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's Y vector 
	if ((cameraViewCounter % 5) == 3)	{
		strcpy(viewStr, " Unum view");
		viewMatrix = glm::lookAt(eye, at, up);
	}
}

/*
Gets Duo camera view and updates the view matrix
*/
void updateDuoCameraView()	{
	glm::vec3 duoPosition = getPosition(sphere[2]->getOrientationMatrix());
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), duoPosition);
	glm::mat4 translationMatrixtemp = glm::translate(translationMatrix, glm::vec3(0.0f, 4000.0f, 0.0f));

	cameraViewDuo = translationMatrixtemp;
	eye = getPosition(cameraViewDuo);   // eye is 4000 "out of screen" from Unum
	at = duoPosition;   // looking at Unum
	up = glm::vec3(0.0f, 0.0f, -1.0f);   // camera's Y vector 
	if ((cameraViewCounter % 5) == 4)	{
		strcpy(viewStr, " Duo view");
		viewMatrix = glm::lookAt(eye, at, up);
	}
}

/*
Gets second Warship camera view and updates the view matrix
*/
void updateWarshipCameraView()	{
	cameraViewWarship = warship->getOrientationMatrix()* glm::translate(glm::mat4(), glm::vec3(0.0f, 300.0f, 1000.0f));
	eye = getPosition(cameraViewWarship);   // eye is above the warship
	at = getPosition(warship->getOrientationMatrix()* glm::translate(glm::mat4(), glm::vec3(0.0f, 300.0f, 0.0f)));;   // looking at Warship
	up = getUp(cameraViewWarship);  // camera'a up vector
	if ((cameraViewCounter % 5) == 2)	{
		strcpy(viewStr, " Warship view");
		viewMatrix = glm::lookAt(eye, at, up);
	}
}
void updateMissileCameraView()	{
	if (wMissile != NULL)	{
		cameraViewMissile = wMissile->getOrientationMatrix()* glm::translate(glm::mat4(), glm::vec3(0.0f, 300.0f, 1000.0f));
		eye = getPosition(cameraViewMissile);   // eye is above the warship
		at = getPosition(wMissile->getOrientationMatrix()* glm::translate(glm::mat4(), glm::vec3(0.0f, 300.0f, 0.0f)));;   // looking at Warship
		up = getUp(cameraViewMissile);  // camera'a up vector
		if ((cameraViewCounter % 5) == 5)	{
			strcpy(viewStr, " Missile view");
			viewMatrix = glm::lookAt(eye, at, up);
		}
	}
}

/*
Updates the camera and updates view Matrix based on what camera index it is
*/
void updateCameras()	{
	updateFrontCameraView();
	updateTopCameraView();
	updateMissileCameraView();
	updateWarshipCameraView();
	updateUnumCameraView();
	updateDuoCameraView();

}

/*
Creates all the sphere objects with it's scale, translation, and radians to
pass through the sphere3D class
*/
void createSpheres()	{
	for (int i = 0; i < nSpheres; i++)	{
		sphere[i] = new Sphere3D(sphereScales[i], modelBR[i],
			sphereTranslationVectors[i],
			sphereRadians[i]
			);
	}
	printf("%d Shapes created \n", nSpheres);
}


void createMissileSite()	{
	for (int i = 0; i < 2; i++)	{
		missileSites[i] = new MissileSite(modelBR[5],
			missileSiteTranslationVectors[i],
			missileSiteRadians[i]
			);
	}
	printf("%d Missile Sites created \n", nSpheres);
}

void createWarship()	{
	warship = new Warship(modelBR[6],
		warshipTranslationVector);
}

void worldGravity()	{
	if ( (gravityToggle % 2) == 0)	{
		glm::vec3 gravityVec = glm::vec3(0, 0, 0);
		// gets the length between warship and Ruber
		float length = distance(getPosition(warship->getOrientationMatrix()), getPosition(sphere[0]->getOrientationMatrix()));

		//get the gravity vector
		if (length != 0)	{

			//get the change in gravity due to length
			float deltaGravity = (gravity / (length * length));

			gravityVec = -glm::normalize(getPosition(warship->getOrientationMatrix())) * deltaGravity;
		}
		//subtracts the gravity vector from the warships position
		warship->transformPosition(gravityVec);
	}
}
void firePlayerMissile()	{
	if (!wMissileActive)	{
		if (wMissileCount > 0)
			wMissile = new Missile(modelBR[7], warship->getOrientationMatrix(), 20);
		wMissileCount--;
		wMissileActive = true;
	}
}
void updatePlayerMissile(MissileSite * ms, int x)	{
	// checks missiles active timer
	glm::mat4 tempTarMat[1];
	float tempTarBR[1];
	if (wMissileTimer > 20)	{
		if (ms != NULL)	{
			tempTarMat[0] = ms->getOrientationMatrix();
			tempTarBR[0] = ms->getModelBR();

			// checks if the player's missile has a target locked on
			// if it does it checks collision between the two objects
			// if no collision then missile continues to chase target
			if (wMissile->checkDetectionRadius(tempTarMat, tempTarBR, 2) == 0)	{
				wMissileTarget = x;
			}

			if (wMissileTarget == x)	{
				bool wmCollision = wMissile->collision(ms->getOrientationMatrix(), ms->getModelBR(), 1);
				// check to see if the locked on target and missile have collided
				// if not, missile continues to chase
				if (!wmCollision)	{
					wMissile->chase(ms->getOrientationMatrix());
					showMat4("Missile Site: ", ms->getOrientationMatrix());
					
				}
				else	{
					printf("PLAYER MISSILE HAS HIT IT'S TARGET \n");
					destroyPlayerMissile();
					ms->destoryThis();
					ms = NULL;
					

				}
			}
		}

	}
}

void destroyPlayerMissile()	{
	wMissile = NULL;
	wMissileTarget = 2;
	wMissileTimer = 0;
	wMissileActive = false;
}

void checkEnemieMissile()	{
	for (int i = 0; i < 2; i++)	{
		if (!msMissileActive[i])	{
			if (missileSites[i]->isCollided(warship->getOrientationMatrix(), warship->getModelBR()))	{
				printf("player collided with missile site %d radius\n", i);
				msMissile[i] = new Missile(modelBR[7], missileSites[i]->getOrientationMatrix(), 20.0);
				msMissileActive[i] = true;
			}
		}
	}
}

void destroyEnemyMissile(Missile * m, int i)	{
	printf("enemy destroy\n");
	m = NULL;
	msMissileTarget[i] = 2;
	msMissileTimer[i] = 0;
	msMissileActive[i] = false;
}

void updateEnemyMissile(Missile * m, int i)	{
	// checks missiles active timer
		if (msMissileTimer[i] > 20)	{
			glm::mat4 tempTarMat[1] = { warship->getOrientationMatrix() };

			float tempTarBR[1] = { warship->getModelBR() };

			// checks if the player's missile has a target locked on
			// if it does it checks collision between the two objects
			// if no collision then missile continues to chase target
			if (msMissileTarget[i] > 1)	{
				msMissileTarget[i] = m->checkDetectionRadius(tempTarMat, tempTarBR, 2);
			}
			else {
				bool msCollision = m->collision(warship->getOrientationMatrix(), warship->getModelBR(), 1);
				// check to see if the locked on target and missile have collided
				// if not, missile continues to chase
				if (!msCollision)	{
					m->chase(warship->getOrientationMatrix());
					//showMat4("Missile Site: ", missileSites[wMissileTarget]->getOrientationMatrix());
				}
				else	{
					printf("ENEMY MISSILE HAS HIT IT'S TARGET \n");
					destroyEnemyMissile(m,i);
					exit(EXIT_SUCCESS);
					warship = NULL;
				}
		}
	}
}





void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 1.0f, 300000.0f);
}

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, baseStr);
	strcat(titleStr, viewStr);
	strcat(titleStr, fpsStr);
	glutSetWindowTitle(titleStr);
}

void display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	updateCameras();
	isTexture = glGetUniformLocation(shaderProgram, "isTexture");
	glUniform1ui(isTexture, false);
	// update model matrix, set MVP, draw
	for (int i = 0; i < nSpheres; i++) {
		// switch statement to update the rotational matrix
		switch (i)	{
		case 3: modelMatrix = sphere[i]->getModelMatrix(sphere[2]->getOrientationMatrix()); break;
		case 4: modelMatrix = sphere[i]->getModelMatrix(sphere[2]->getOrientationMatrix()); break;
		default: modelMatrix = sphere[i]->getModelMatrix(); break;
		}
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(vao[i]);
		glDrawArrays(GL_TRIANGLES, 0, nSphereVertices[i]);
	}

	// displays the warship
	modelMatrix = warship->getModelMatrix();
	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
	glBindVertexArray(vao[nSpheres]);
	glDrawArrays(GL_TRIANGLES, 0, warshipVertices);
	
	

	// display missile site
	for (int i = 0; i < nMissileSites; i++)	{
		if (!missileSites[i]->isDestoryed())	{
			modelMatrix = missileSites[i]->getModelMatrix();
			ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
			glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
			glBindVertexArray(vao[nSpheres + nWarships]);
			glDrawArrays(GL_TRIANGLES, 0, missileSiteVertices);
		}
	}

	// missile
	// player missile draw
	if (wMissileActive)	{
		modelMatrix = wMissile->getModelMatrix();
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(vao[nSpheres + nWarships + 1]);
		glDrawArrays(GL_TRIANGLES, 0, missileVertices);
	}

	for (int i = 0; i < nMissileSites; i++)	{
		if (msMissileActive[i])	{
			modelMatrix = msMissile[i]->getModelMatrix();
			ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
			glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
			glBindVertexArray(vao[nSpheres + nWarships + 1]);
			glDrawArrays(GL_TRIANGLES, 0, missileVertices);
		}
	}
	// draw skybox
	glUniform1ui(isTexture,true);

	//glDepthFunc(GL_LEQUAL);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
	glDrawArrays(GL_TEXTURE_CUBE_MAP, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glBindVertexArray(0);
	//glDepthFunc(GL_LESS);

	glUniform1ui(isTexture, false);

	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;

	if (timeInterval >= 1000) {
		sprintf(fpsStr, " fps %4d", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}


// Animate scene objects by updating their transformation matrices
void update(int i) {
	glutTimerFunc(timeQuantum[(timerMode%4)], update, 1);

	if (wMissileCount == 0)
		exit(EXIT_SUCCESS);

	if (wMissileActive)	{
		wMissileTimer++;
		for (int i = 0; i < 2; i++)	{
			if (missileSites[i] != NULL)	{
				updatePlayerMissile(missileSites[i],i);
			}
		}
		if (wMissileTimer > 2000)
			destroyPlayerMissile();
		//printf("Missile Timer: %d", wMissileTimer);
	}
	if (wMissile != NULL)	{
		wMissile->update();
	}

	
	checkEnemieMissile();

	for (int i = 0; i < 2; i++)	{
		if (msMissileActive[i])	{
			msMissileTimer[i]++;
			updateEnemyMissile(msMissile[i], i);
			if (msMissileTimer[i] > 2000)
				destroyEnemyMissile(msMissile[i],i);
		}
		if (msMissile[i] != NULL)	{
			msMissile[i]->update();
		}
	}

	for (int i = 0; i < nSpheres; i++)
		sphere[i]->update();
	for (int i = 0; i < 2; i++)	{
		if (!missileSites[i]->isDestoryed())
			missileSites[i]->update();
	}
	if (gravityToggle == 0)
		worldGravity();
	warship->update();
	
	
}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	case 't': case 'T':  // time quantum
		timerMode++; break;
	case 's': case 'S':  // top view
		warship->increaseSpeed(); break;
	case 'g': case 'G':  // top view
		gravityToggle++; break;
	case 'k': case 'K':  // top view
		destroyPlayerMissile(); break;
	case 'w': case 'W':  // top view
		if ((warpCounter % 2) == 0)	{
			warship->warp(cameraViewUnum);
			warpCounter++; printf("Unum Warp %d\n", (warpCounter % 2));
		}
		else{
			warship->warp(cameraViewDuo);
			warpCounter++; printf("Duo Warp\n");
		} break;
	case 'v': case 'V':
		cameraViewCounter++; break;
	case 'f': case 'F':
		firePlayerMissile(); break;
	case 'z': case 'Z':
		showVec3("Missile Site",glm::normalize(getPosition(sphere[5]->getOrientationMatrix()))); break;
	}
	viewMatrix = glm::lookAt(eye, at, up);
	updateTitle();
}
int useControlFunction(int key)	{
	switch (key)	{
	case GLUT_KEY_UP:
		warship->pitchUp(); break;
	case GLUT_KEY_DOWN:
		warship->pitchDown(); break;
	case GLUT_KEY_LEFT:
		warship->rollLeft(); break;
	case GLUT_KEY_RIGHT:
		warship->rollRight(); break;
	}
	return 1;
}
void warshipKeyboard(int key, int x, int y) {
	int mod;
	mod = glutGetModifiers();

	if (mod == GLUT_ACTIVE_CTRL)
		useControlFunction(key);
	else	{
		switch (key) {
		case GLUT_KEY_LEFT:
			warship->turnLeft(); break;
		case GLUT_KEY_RIGHT:
			warship->turnRight(); break;
		case GLUT_KEY_DOWN:
			warship->reverse(); break;
		case GLUT_KEY_UP:
			warship->accelerate(); break;
		}
	}
}



int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Warhip Simulator : front view");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(warshipKeyboard);
	glutTimerFunc(timerDelay, update, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}


