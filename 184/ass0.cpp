// CS184 Simple OpenGL Example
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>

#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265

double iIncr = 1;

using namespace std;

//****************************************************
// Some Classes
//****************************************************
class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport    viewport;

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport(0,0,viewport.w,viewport.h);// sets the rectangle that will be the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                // loading the identity matrix for the screen

  //----------- setting the projection -------------------------
  // glOrtho sets left, right, bottom, top, zNear, zFar of the chord system


  // glOrtho(-1, 1 + (w-400)/200.0 , -1 -(h-400)/200.0, 1, 1, -1); // resize type = add
  // glOrtho(-w/400.0, w/400.0, -h/400.0, h/400.0, 1, -1); // resize type = center

  glOrtho(-1, 1, -1, 1, 1, -1);    // resize type = stretch

  //------------------------------------------------------------
}


//****************************************************
// sets the window up
//****************************************************
void initScene(){
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 

  myReshape(viewport.w,viewport.h);
}




//***************************************************
// function that does the actual drawing
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)

  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"

  //----------------------- code to draw objects --------------------------
  

	double pos_y = 1;
	double size = 0.2;
	double height = sqrt(3)/2 * size;
	boolean orientation = true; 
	double i = 1;
	double freq = 1/(8*PI);
	double r = sin(freq * i + 0)/5 + 0.8;
	double g = sin(freq * i + 2)/5 + 0.8;
	double b = sin(freq * i + 4)/5 + 0.8;
	
	for (double pos_y = -1.2; pos_y <= 1.1; pos_y += height) {
		for (double pos_x = -1; pos_x <= 1.1; pos_x += size/2) {
			i += iIncr;
			r = sin(freq * i + 0)/5 + 0.8;
			g = sin(freq * i + 2)/5 + 0.8;
			b = sin(freq * i + 4)/5 + 0.8;

			glColor3f(r, g, b);
			glBegin(GL_POLYGON);
			if (orientation) {
				glVertex2f(pos_x, pos_y);
				glVertex2f(pos_x - size/2, pos_y - height);
				glVertex2f(pos_x + size/2, pos_y - height);
			} else {
				glVertex2f(pos_x, pos_y - height);
				glVertex2f(pos_x - size/2, pos_y);
				glVertex2f(pos_x + size/2, pos_y);
			}
			glEnd();
			orientation = !orientation;
		}
	}

  glFlush();
  glutSwapBuffers();                           // swap buffers (we earlier set double buffer)
}




//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myFrameMove() {
  //nothing here for now
#ifdef _WIN32
  Sleep(10);                                   //give ~10ms back to OS (so as not to waste the CPU)
#endif
  glutPostRedisplay(); // forces glut to call the display function (myDisplay())
}





void keyPressed (unsigned char key, int x, int y) {
	if (key == ' ') {
		iIncr++;
	}
	glFlush();
}


//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = 400;
  viewport.h = 400;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("CS184!");

  initScene();                                 // quick function to set up scene

  glutDisplayFunc(myDisplay);                  // function to run when its time to draw something
  glutReshapeFunc(myReshape);                  // function to run when the window gets resized
  glutIdleFunc(myFrameMove);                   // function to run when not handling any other task
  glutKeyboardFunc(keyPressed);
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








