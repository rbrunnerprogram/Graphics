#include <windows.h>
#include <stdio.h>	
#include <gl/Gl.h>
#include <gl/GLU.h>
#include <gl/glut.h> 
#include <gl/GLAux.h>
//#include <math.h>

using namespace std;

/******************************** Prototypes ********************************/
void Initialize();
void Animate();
void Reshape(int width, int height);
void Display();
void DrawFrame();

/****************************************************************************
 LoadBMP()

 Create a function to load BMP images for textures
*****************************************************************************/
GLuint LoadBMP(const char *fileName)
{
	FILE *file;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int size;
	unsigned int width, height;
	unsigned char *data;
	

	file = fopen(fileName, "rb");

	dataPos		= *(int*)&(header[0x0A]);
	size		= *(int*)&(header[0x22]);
	width		= *(int*)&(header[0x12]);
	height		= *(int*)&(header[0x16]);

	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
		dataPos = 54;

	data = new unsigned char(size);

	fread(data, 1, size, file);

	fclose(file);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	return texture;
}

/********************************* Globals **********************************/

// how much to rotate the cube around an axis
GLfloat g_rotationAngle = 0.0;
GLfloat rotate_speed = 2.0;
GLfloat rotatingAngleFirstPlanet = 0.0;
GLfloat rotatingAngleSecondPlanet = 0.0;
GLfloat rotatingAngleThirdPlanet = 0.0;
GLfloat rotatingAngleFourthPlanet = 0.0;
GLfloat rotatingAngleFifthPlanet = 0.0;
GLfloat rotatingAngleEmoon1 = 0.0;
GLfloat rotatingAngleEmoon2 = 0.0;
GLfloat rotatingAngleEmoon3 = 0.0;
GLfloat rotatingAngleEmoon4 = 0.0;
GLfloat originRotation = 0.0;

//texture variables
GLuint sunTextureAttempt;

//initial rotating "speed"
GLfloat InitialrotatingAngleFirstPlanet = 2.0;
GLfloat InitialrotatingAngleSecondPlanet = 1.0;
GLfloat InitialrotatingAngleThirdPlanet = 0.5;
GLfloat InitialrotatingAngleFourthPlanet = 0.25;
GLfloat InitialrotatingAngleFifthPlanet = 0.1;
GLfloat InitialrotatingAngleEmoon1 = 2.0;
GLfloat InitialrotatingAngleEmoon2 = 3.0;
GLfloat InitialrotatingAngleEmoon3 = 2.5;
GLfloat InitialrotatingAngleEmoon4 = 1.3;

//allows change in rotating speed
GLfloat rotatingSpeedFirstPlanet = InitialrotatingAngleFirstPlanet/10;
GLfloat rotatingSpeedSecondPlanet = InitialrotatingAngleSecondPlanet/10;
GLfloat rotatingSpeedThirdPlanet = InitialrotatingAngleThirdPlanet/10;
GLfloat rotatingSpeedFourthPlanet = InitialrotatingAngleFourthPlanet/10;
GLfloat rotatingSpeedFifthPlanet = InitialrotatingAngleFifthPlanet/10;
GLfloat rotatingSpeedEmoon1 = InitialrotatingAngleEmoon1/10;
GLfloat rotatingSpeedEmoon2 = InitialrotatingAngleEmoon2/10;
GLfloat rotatingSpeedEmoon3 = InitialrotatingAngleEmoon3/10;
GLfloat rotatingSpeedEmoon4 = InitialrotatingAngleEmoon4/10;

GLfloat rollAmount = 0.0;

float sphereCenters[9][3];

//window dimensions
int windowWidth = 900;
int windowHeight = 900;

//gluLookAt reference points
float eyeLocation[3] = {0.0,0.0,60.0};
float referencePoint[3] = {0.0,0.0,0.0};
float upDirection[3] = {0.0,1.0,0.0};

//toggles the animation feature
bool toggleAnimate = false;

/****************************************************************************
 myKeyboard()

 Create a keyboard handler to zoom, pan, yaw, row, and roll
*****************************************************************************/
void myKeyboard( unsigned char key, int x, int y){ //keyboard keybindings
	switch(key){
		case 'w':  //pitch up
			referencePoint[1] += 1;
			Display();
			break;
		case 's': //pitch down
			referencePoint[1] -= 1;
			Display();
			break;
		case 'a': //pan left
			referencePoint[0] += 1;
			Display();
			break;
		case 'd': //pan right
			referencePoint[0] -= 1;
			Display();
			break;
		case 'z': //zoom in
			if(eyeLocation[2]>1){
				eyeLocation[2] -= 1;
			}
			Display();
			break;
		case 'x': //zoom out
			eyeLocation[2] += 1;
			Display();
			break;
		case 'c': //slow down rotations
			InitialrotatingAngleFirstPlanet -= rotatingSpeedFirstPlanet;
			InitialrotatingAngleSecondPlanet -= rotatingSpeedSecondPlanet;
			InitialrotatingAngleThirdPlanet -= rotatingSpeedThirdPlanet;
			InitialrotatingAngleFourthPlanet -= rotatingSpeedFourthPlanet;
			InitialrotatingAngleFifthPlanet -= rotatingSpeedFifthPlanet;
			InitialrotatingAngleEmoon1 -= rotatingSpeedEmoon1;
			InitialrotatingAngleEmoon2 -= rotatingSpeedEmoon2;
			InitialrotatingAngleEmoon3 -= rotatingSpeedEmoon3;
			InitialrotatingAngleEmoon4 -= rotatingSpeedEmoon4;
			break;
		case 'v': //speed up rotation
			InitialrotatingAngleFirstPlanet += rotatingSpeedFirstPlanet;
			InitialrotatingAngleSecondPlanet += rotatingSpeedSecondPlanet;
			InitialrotatingAngleThirdPlanet += rotatingSpeedThirdPlanet;
			InitialrotatingAngleFourthPlanet += rotatingSpeedFourthPlanet;
			InitialrotatingAngleFifthPlanet += rotatingSpeedFifthPlanet;
			InitialrotatingAngleEmoon1 += rotatingSpeedEmoon1;
			InitialrotatingAngleEmoon2 += rotatingSpeedEmoon2;
			InitialrotatingAngleEmoon3 += rotatingSpeedEmoon3;
			InitialrotatingAngleEmoon4 += rotatingSpeedEmoon4;
			break;
		case 'b': //toggle animation
			if(toggleAnimate){
				toggleAnimate = false;
				Display();
			}
			else{
				toggleAnimate = true;
				Display();
			}
			break;
		case 'q': //roll counterclockwise
			rollAmount +=2.0;
			Display();
			break;
		case 'e': //roll clockwise
			rollAmount -=2.0;
			Display();
			break;
	}
}


/****************************************************************************
 main()

 Setup GLUT and OpenGL, drop into the event loop
*****************************************************************************/
int main(int argc, char **argv)
{
  // Setup the basic GLUT stuff
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Create the window
  glutInitWindowSize(900, 900);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Solar System");
  //glutFullScreen();

  Initialize();

  // Register the event callback functions
  glutKeyboardFunc(myKeyboard);
  glutDisplayFunc(Display); 
  glutReshapeFunc(Reshape);
  glutIdleFunc(Animate);

  glutMainLoop();   
   
  return 0;
} // end main()


/****************************************************************************
 Initialize()

create a light, set the shading mode, and clear color.
*****************************************************************************/
void Initialize()
{
  // set the background color
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // set the shading model
  glShadeModel(GL_SMOOTH);

  // set up a single white light
  GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };

  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // make the modelview matrix active, and initialize it
  glMatrixMode(GL_MODELVIEW);
} // end Initialize()



/****************************************************************************
 Animate()
*****************************************************************************/
void Animate()
{
  glutPostRedisplay();
} // end Animate()


/****************************************************************************
 Reshape()

 Reset the viewport for window changes
*****************************************************************************/
void Reshape(int width, int height)
{
  if (height == 0)
    return;
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, width/height, 1.0, 1000.0);

  glMatrixMode(GL_MODELVIEW);
} // end Reshape


/****************************************************************************
 Display()

 Clear and redraw the scene.
*****************************************************************************/
void Display()
{
	//if animation is "on" ...
	if(toggleAnimate){
		//increase the rotating angles of all spheres
		rotatingAngleFirstPlanet += InitialrotatingAngleFirstPlanet;
		rotatingAngleSecondPlanet += InitialrotatingAngleSecondPlanet;
		rotatingAngleThirdPlanet += InitialrotatingAngleThirdPlanet;
		rotatingAngleFourthPlanet += InitialrotatingAngleFourthPlanet;
		rotatingAngleFifthPlanet += InitialrotatingAngleFifthPlanet;
		rotatingAngleEmoon1 += InitialrotatingAngleEmoon1;
		rotatingAngleEmoon2 += InitialrotatingAngleEmoon2;
		rotatingAngleEmoon3 += InitialrotatingAngleEmoon3;
		rotatingAngleEmoon4 += InitialrotatingAngleEmoon4;
		originRotation +=1.0;
	}

	// set up the view orientation looking at the origin from slightly above
	// and to the left
	glLoadIdentity();
	gluLookAt(eyeLocation[0], eyeLocation[1], eyeLocation[2],
				referencePoint[0], referencePoint[1], referencePoint[2],
				upDirection[0], upDirection[1], upDirection[2]);
	  // clear the screen
  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//initiallize sunlight
	GLfloat lightPos[4] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	DrawFrame();

	glFlush();
	glutSwapBuffers();
} // end Display()
 

/****************************************************************************
 DrawFrame()

 Draws the rotating cube
*****************************************************************************/
void DrawFrame()
{
	//Create quadric spheres
	GLUquadric *sunObject;
	GLUquadric *firstPlanet;
	/*GLUquadric *secondPlanet;
	GLUquadric *thirdPlanet;
	GLUquadric *fourthPlanet;
	GLUquadric *fifthPlanet;*/

	sunTextureAttempt = LoadBMP("sunText.bmp"); //load sunTexture bitmap
	glEnable(GL_TEXTURE_2D); //enable 2D textures

	//Create sun sphere
	//Normals will be pointing outwards to create light emitting effect
	sunObject = gluNewQuadric();
	gluQuadricDrawStyle(sunObject, GLU_FILL);
	gluQuadricNormals(sunObject, GLU_SMOOTH);
	gluQuadricOrientation(sunObject, GLU_INSIDE);  //reverses normal direction
	gluQuadricTexture(sunObject, GL_TRUE); //allows texture mapping
	
	//Create planet/moon sphere
	//normals will point normally
	firstPlanet = gluNewQuadric();
	gluQuadricDrawStyle(firstPlanet, GLU_FILL);
	gluQuadricNormals(firstPlanet, GLU_SMOOTH);
	gluQuadricOrientation(firstPlanet, GLU_OUTSIDE); //points normals outward
	gluQuadricTexture(firstPlanet, GL_TRUE);

	//Allows rolling
	glRotatef(rollAmount, 0.0,0.0,1.0);

	//Sun
	glPushMatrix();
		glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
		gluSphere(sunObject, 5.0, 30.0, 30.0);
	glPopMatrix();

	//First Planet from sun
	glPushMatrix();
		glRotatef(rotatingAngleFirstPlanet,0.08,1.0,0.0); //spin about origin
		glTranslatef(20.0, 0.0, 0.0);
		glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
		gluSphere(firstPlanet, 2.0, 30.0, 30.0);
	glPopMatrix();

	//Second Planet from sun
	glPushMatrix();
		glRotatef(rotatingAngleSecondPlanet,0.08,1.0,0.0); //spin about origin
		glTranslatef(25.0, 0.0, 0.0);
		glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
		gluSphere(firstPlanet, 1.0, 30.0, 30.0);
	glPopMatrix();

	//Third Planet from sun
	glPushMatrix();
		glRotatef(rotatingAngleThirdPlanet,0.08,1.0,0.0); //spin about origin
		glTranslatef(29.0, 0.0, 0.0);
		glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
		gluSphere(firstPlanet, 1.0, 30.0, 30.0);
	glPopMatrix();

	//Fourth Planet from sun
	glPushMatrix();
		glRotatef(rotatingAngleFourthPlanet,0.08,1.0,0.0); //spin about origin
		glTranslatef(35.0, 0.0, 0.0);
		glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
		gluSphere(firstPlanet, 1.2, 30.0, 30.0);

		//First Moon
		glPushMatrix();
			glRotatef(rotatingAngleEmoon1,0.08,1.0,0.0); //spin about planet
			glTranslatef(3, -0.4, 0.0);
			glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
			gluSphere(firstPlanet, 0.2, 30.0, 30.0);
		glPopMatrix();

		//Second Moon
		glPushMatrix();
			glRotatef(rotatingAngleEmoon2,0.08,1.0,0.0); //spin about planet
			glTranslatef(4.0, 1.0, 0.0);
			glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
			gluSphere(firstPlanet, 0.2, 30.0, 30.0);
		glPopMatrix();

		//Third Moon
		glPushMatrix();
			glRotatef(rotatingAngleEmoon3,0.08,1.0,0.0); //spin about planet
			glTranslatef(3.0, 0.0, 0.0);
			glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
			gluSphere(firstPlanet, 0.2, 30.0, 30.0);
		glPopMatrix();

	glPopMatrix();

	//Fifth Planet from sun
	glPushMatrix();
		glRotatef(rotatingAngleFifthPlanet,0.08,1.0,0.0); //spin about origin
		glTranslatef(42.0, 0.0, 0.0);
		glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
		gluSphere(firstPlanet, 1.0, 30.0, 30.0);

		//Moon
		glPushMatrix();
			glRotatef(rotatingAngleEmoon4,0.08,1.0,0.0); //spin about planet
			glTranslatef(3.0, 0.0, 0.0);
			glRotatef(originRotation,0.0,1.0,0.0); //spin about perpendicular axis
			gluSphere(firstPlanet, 0.2, 30.0, 30.0);
		glPopMatrix();

	glPopMatrix();

	//Delete Spheres
	gluDeleteQuadric(sunObject);
	gluDeleteQuadric(firstPlanet);
	/*gluDeleteQuadric(secondPlanet);
	gluDeleteQuadric(thirdPlanet);
	gluDeleteQuadric(fourthPlanet);
	gluDeleteQuadric(fifthPlanet);*/
} // end DrawFrame()