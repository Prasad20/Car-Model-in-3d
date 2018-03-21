#include<bits/stdc++.h>
#include <GL/glut.h>
#include<stdio.h>
using namespace std;
#define PI 3.14159265
void ellipse(int x,int y,int start_a,int end_a,int a,int b)
{
      glBegin(GL_POINTS);
   for(int i=start_a;i<=end_a;i++)
     {
       int x1=x+a*cos(i*(PI/180.0));
       int y1=y+b*sin(i*(PI/180.0)); 
       glVertex2f(x1,y1); 
     }
}
void line(int x0,int y0,int x1,int y1)
{
   glBegin(GL_LINE_STRIP);
   glVertex2f(x0,y0);
   glVertex2f(x1,y1); 
}
void displayCB(void)	
{
  glClear(GL_COLOR_BUFFER_BIT);		/* clear the display */
  glColor3f(1, 1, 0);		/* set current color to white */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
 ellipse(380,127,20,152,130,35);
        line(490,109,560,142);

  glEnd();
    glFlush();				/* Complete any pending operations */
}

void keyCB(unsigned char key, int x, int y)	/* called on key press */
{
  if( key == 'q' ) exit(0);
}


int main(int argc, char *argv[])
{
  int win;

  glutInit(&argc, argv);		/* initialize GLUT system */
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowSize(400,500);		/* width=400pixels height=500pixels */
  win = glutCreateWindow("ellipse");	/* create window */

  /* from this point on the current window is win */
  
  glClearColor(0.7,0.7,0.7,0.0);	/* set background to black */
  gluOrtho2D(-100,1000,-100,1000);		/* how object is mapped to window */
  glutDisplayFunc(displayCB);		/* set window's display callback */
  glutKeyboardFunc(keyCB);		/* set window's key callback */

  glutMainLoop();			/* start processing events... */

  /* execution never reaches this point */

  return 0;
}


