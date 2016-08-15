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
  

  // drawing the circle
  double cir_x, cir_y;
  double cir_angle = 0.0;
  double cir_change = 2*PI / 40;
  static double cir_radius = 0.15;
  static double cir_rad_change = 0.001;
  if (cir_radius >= 0.3) {
    cir_rad_change = -0.001;
  }
  if (cir_radius < 0.15) {
    cir_rad_change = 0.001;
  }
  cir_radius += cir_rad_change;


  // drawing the movement
  double radius = 0.5;
  double pos_change = 2*PI / 180;
  static double pos_x = 0.0;
  static double pos_y = radius;
  static double pos_angle = 0.0;

  pos_x = radius * sin(pos_angle);
  pos_y = radius * cos(pos_angle);
  pos_angle += pos_change;

  glColor3ub(185, 162, 179);
    glBegin(GL_POLYGON);
    while (cir_angle <= 2*PI) {
    cir_x = cir_radius * sin(cir_angle);
    cir_y = cir_radius * cos(cir_angle);
    glVertex2f(cir_x + pos_x, cir_y + pos_y);
    cir_angle += cir_change;
    }
    glEnd();
    
    cir_angle = 0.0;
    glBegin(GL_POLYGON);
    while (cir_angle <= 2*PI) {
    cir_x = cir_radius * sin(cir_angle);
    cir_y = cir_radius * cos(cir_angle);
    glVertex2f(-(cir_x + pos_x), -(cir_y + pos_y));
    cir_angle += cir_change;
    }
    glEnd();


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
  glutMainLoop();                              // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}








