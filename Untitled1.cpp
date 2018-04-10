#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")



/* ASCII code for the escape key. */
#define ESCAPE 27

GLint window;
GLint window2;
GLint Xsize=1000;
GLint Ysize=800;
float i,theta;
GLint nml=0,day=1;

int grass;

int eflag=0;

int flagg = 0;

float red = 0;
float green = 0;
float blue = 0;

char name3[]="PROJECT:  3D CAR  ANIMATION";

GLfloat xt=0.0,yt=0.0,zt=0.0,xw=0.0;   /* x,y,z translation */
GLfloat tx=295,ty=62;
GLfloat xs=1.0,ys=1.0,zs=1.0;

GLfloat xangle=0.0,yangle=0.0,zangle=0.0,angle=0.0;   /* axis angles */

GLfloat r=0,g=0,b=1;
GLint light=1;
int count=1,flg=1;
int view=1;
int flag1=0,aflag=1;            //to switch car driving mode
int flag2=0,wheelflag=0;   //to switch fog effect
GLUquadricObj *t;

static void SpecialKeyFunc( int Key, int x, int y );

/* Simple  transformation routine */
GLvoid Transform(GLfloat Width, GLfloat Height)
{
  glViewport(0, 0, Width, Height);              /* Set the viewport */
  glMatrixMode(GL_PROJECTION);                  /* Select the projection matrix */
  glLoadIdentity();				/* Reset The Projection Matrix */
  gluPerspective(45.0,Width/Height,0.1,100.0);  /* Calculate The Aspect Ratio Of The Window */
  glMatrixMode(GL_MODELVIEW);                   /* Switch back to the modelview matrix */
}


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLfloat Width, GLfloat Height)
{

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glLineWidth(2.0);              /* Add line width,   ditto */
  Transform( Width, Height ); /* Perform the transformation */
  //newly added
  t=gluNewQuadric();
        gluQuadricDrawStyle(t, GLU_FILL);

glEnable(GL_LIGHTING);

glEnable(GL_LIGHT0);

// Create light components
GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 1.5f, 1.0f, 4.0f, 1.0f };

// Assign created components to GL_LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, position);

}

GLuint LoadBMP(const char *fileName)
{
	FILE *file;
	unsigned char header[54],*data;
	unsigned int dataPos,size,width, height;
	file = fopen(fileName, "rb");
	fread(header, 1, 54, file);				
	dataPos		= *(int*)&(header[0x0A]);	
	size		= *(int*)&(header[0x22]);	
	width		= *(int*)&(header[0x12]);	
	height		= *(int*)&(header[0x16]);	
//	if (size == )
		size = width * height * 3;
//	if (dataPos == NULL)
		dataPos = 54;
	data = new unsigned char[size];
	fread(data, 1, size, file);
	fclose(file);
	GLuint texture;
	glGenTextures(1, &texture);				
	glBindTexture(GL_TEXTURE_2D, texture);	



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data); 
	return texture;
}

/* The function called when our window is resized  */
GLvoid ReSizeGLScene(GLint Width, GLint Height)
{
  if (Height==0)     Height=1;                   /* Sanity checks */
  if (Width==0)      Width=1;
  Transform( Width, Height );                   /* Perform the transformation */
}

void init()
{
    glClearColor(0,0,0,0);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,900.0,0.0,600.0,50.0,-50.0);
	glutPostRedisplay(); 		// request redisplay
}

void display_string(int x, int y, char *string, int font)
{
    int len,i;
	glColor3f(0.8,0.52,1.0);
	glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
    if(font==1)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
	if(font==2)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	if(font==3)
	        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,string[i]);
	if(font==4)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,string[i]);
	}

}

void display1(void)
{

	glClearColor(1.0,1.0,0.1,1.0);
	glutPostRedisplay();
	glutSwapBuffers();

}

GLvoid DrawGLScene()
{
   GLfloat mat_specular1[] = { red, green, blue, 1.0 };
   GLfloat mat_shininess1[] = { 2.0 };
   GLfloat light_position1[] = { .3, 0.5, .6, 0.0 };
   glClearColor (1.0, 1.0, 1.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess1);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/* Clear The Screen And The Depth Buffer */
if(view==0)
{
init();
display1();
}
else
{
  if(count==1)
	InitGL(Xsize,Ysize);
  if(aflag==1)/* Initialize our window. */
  glClearColor(1,1,1,1);
  else
	  glClearColor(0.1,0.1,0.1,0);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(-1.0,0.0,-3.5);
  glRotatef(xangle,1.0,0.0,0.0);
  glRotatef(yangle,0.0,1.0,0.0);
  glRotatef(zangle,0.0,0.0,1.0);
  glTranslatef(xt,yt,zt);
  glScalef(xs,ys,zs);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  if(flag2==1)
  {
  GLfloat fogcolour[4]={1.0,1.0,1.0,1.0};

  glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
  glFogf(GL_FOG_DENSITY,0.1);                   /* How dense */
  glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
  glFogf(GL_FOG_START,3.0);                   /* Where wwe start fogging */
  glFogf(GL_FOG_END,100.0);                       /* end */
  glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
  glEnable(GL_FOG);/* ENABLE */
  }
  if(flag2==0)
  {
	  glDisable(GL_FOG);
  }

if(!aflag){
  glBegin(GL_POINTS);
  glColor3f(1,1,1);
  glPointSize(200.0);
  int ccount=0;
  float x=10,y=10;
  while(ccount<20)
  {
	  glVertex2f(x,y);
	  x+=10;
	  y+=10;
	  if(y>Ysize) y-=10;
	  if(x>Xsize) x-=10;
	  ccount++;
  }
  glEnd();}

  glColor3f(1.0,.75,0.0);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(0.1,0.3,0.3);
  glVertex3f(0.1,0.3,0.5);
  glEnd();
  glPointSize(200.0);

  glColor3f(1.0,0.0,0.0);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(1.9,0.3,0.5);
 glVertex3f(1.9,0.3,0.3); 
  glEnd();
  glPointSize(200.0);
 


  glBegin(GL_QUADS);                /* OBJECT MODULE*/

  /* top of cube*/
  //************************FRONT BODY****************************************
/*  glColor3f(r,g,b);
  glVertex3f( 0.1, 0.4,0.6);
  glVertex3f(0.2, 0.415,0.6);
  glVertex3f(0.2, 0.415,0.2);
  glVertex3f(0.1,0.4,0.2);
  */
  if(eflag==0)
  {
  glColor3f(0,1,0);
  glVertex3f(0.1, 0.4,0.55);
  glVertex3f(0.5, 0.48,0.55);
  glVertex3f(0.5, 0.48,0.25);
  glVertex3f(0.1,0.4,0.25);
  }
  else
  {
  	glColor3f(0,1,0);
    glVertex3f(0.1, 0.4,0.55);
    glVertex3f(0.25, 0.7,0.55);
    glVertex3f(0.25, 0.7,0.25);
    glVertex3f(0.1,0.4,0.25);
    
     glColor3f(0.2,0.2,0.2);
  glVertex3f(0.1, 0.4,0.55);
  glVertex3f(0.5, 0.48,0.55);
  glVertex3f(0.5, 0.48,0.25);
  glVertex3f(0.1,0.4,0.25);
  }
  
  glColor3f(r,g,b);
  glVertex3f(0.1, 0.4,0.2);
  glVertex3f(0.5, 0.48,0.2);
  glVertex3f(0.5, 0.48,0.25);
  glVertex3f(0.1,0.4,0.25);
  
  glVertex3f(0.1, 0.4,0.55);
  glVertex3f(0.5, 0.48,0.55);
  glVertex3f(0.5, 0.48,0.6);
  glVertex3f(0.1,0.4,0.6);

  glColor3f(r,g,b);
  glVertex3f(0.49, 0.415,0.6);
  glVertex3f(0.6, 0.5,0.6);
  glVertex3f(0.6, 0.5,0.2);
  glVertex3f(0.49,0.415,0.2);
  
  
  /* bottom of cube*/
  glVertex3f( 0.1,0.2,0.6);
  glVertex3f(0.6,0.2,0.6);
  glVertex3f(0.6,0.2,0.2);
  glVertex3f( 0.1,0.2,0.2);

  /* front of cube*/
  glVertex3f( 0.1,0.2,0.6);
  glVertex3f(0.1, 0.4,0.6);
  glVertex3f(0.1,0.4,0.2);
  glVertex3f( 0.1,0.2,0.2);

  /* back of cube.*/
  glVertex3f(0.6,0.2,0.6);
  glVertex3f(0.6,0.5,0.6);
  glVertex3f(0.6,0.5,0.2);
  glVertex3f( 0.6,0.2,0.2);

  /* left of cube*/
  glVertex3f(0.1,0.2,0.6);
  glVertex3f(0.6,0.2,0.6);
  glVertex3f(0.6,0.5,0.6);
  glVertex3f(0.1,0.4,0.6);

  /* Right of cube */
  glVertex3f(0.1,0.2,0.2);
  glVertex3f( 0.6,0.2,0.2);
  glVertex3f( 0.6,0.5,0.2);
  glVertex3f( 0.1,0.4,0.2);
//****************************************************************************
  glVertex3f(0.9,0.65,0.6);
  glVertex3f(0.9,0.65,0.2);
  glVertex3f(1.5,0.65,0.2);        //top cover
  glVertex3f(1.5,0.65,0.6);
//***************************back guard******************************
 /* glColor3f(r,g,b);            /* Set The Color To Blue*/
  glVertex3f( 1.8, 0.5,0.6);
  glVertex3f(1.8, 0.5,0.2);
  glVertex3f(1.9, 0.4, 0.2);
  glVertex3f(1.9,0.4,0.6);

  /* bottom of cube*/
  glVertex3f(1.9,0.2,0.6);
  glVertex3f(1.9,0.2,0.2);
  glVertex3f(1.8,0.2,0.6);
  glVertex3f( 1.8,0.2,0.6);

  /* back of cube.*/
  glVertex3f(1.9,0.4,0.6);
  glVertex3f(1.9,0.4,0.2);
  glVertex3f(1.9,0.2,0.2);
  glVertex3f(1.9,0.2,0.6);
  
  /* left of cube*/
  glVertex3f(1.8,0.2,0.2);
  glVertex3f(1.8,0.5,0.2);
  glVertex3f(1.9,0.4,0.2);
  glVertex3f(1.9,0.2,0.2);

  /* Right of cube */
  glVertex3f(1.8,0.2,0.6);
  glVertex3f(1.8,0.5,0.6);
  glVertex3f(1.9,0.4,0.6);
  glVertex3f(1.9,0.2,0.6);
//******************MIDDLE BODY************************************
  
  if(flagg==1)
  {
  glVertex3f( 0.6, 0.5,0.6);
  glVertex3f(0.6, 0.2,0.6);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8,0.5,0.6);
  }
  else
  {
 glVertex3f( 0.6, 0.5,0.6);
  glVertex3f(0.6, 0.2,0.6);
  glVertex3f(1.8, 0.2, 0.6);
  glVertex3f(1.8,0.5,0.6);
  }

  /* bottom of cube*/
  glVertex3f( 0.6,0.2,0.6);
  glVertex3f(0.6,0.2,0.2);
  glVertex3f(1.8,0.2,0.2);
  glVertex3f( 1.8,0.2,0.6);

  /* back of cube.*/
  glVertex3f(0.6,0.5,0.2);
  glVertex3f(0.6,0.2,0.2);
  glVertex3f(1.8,0.2,0.2);
  glVertex3f(1.8,0.5,0.2);
//*********************ENTER WINDOW**********************************
  glColor3f(.1,0.1,0.1);
  glVertex3f(0.93, 0.65,0.6);
  glVertex3f(0.63, 0.5,0.6);        //quad front window
  glVertex3f(1.2, 0.5, 0.6);
  glVertex3f(1.2,0.65,0.6);

  glVertex3f(1.22,0.5,0.6);
  glVertex3f(1.22,0.65,0.6);
  glVertex3f(1.48,0.65,0.6);        //quad back window
  glVertex3f(1.48,0.5,0.6);

  glColor3f(r,g,b);
  glVertex3f(0.6,0.5,0.6);
  glVertex3f(0.63,0.5,0.6);       //first separation
  glVertex3f(0.9,0.65,0.6);
  glVertex3f(0.93,0.65,0.6);

  glVertex3f(1.2,0.65,0.6);
  glVertex3f(1.2,0.5,0.6);       //second separation
  glVertex3f(1.22,0.5,0.6);
  glVertex3f(1.22,0.65,0.6);

  glVertex3f(1.48,0.65,0.6);
  glVertex3f(1.48,0.5,.6);     //3d separation
  glVertex3f(1.8,0.5,0.6);
  glVertex3f(1.5,0.65,0.6);

  glColor3f(.1,0.1,0.1);
  glVertex3f(0.93,0.65,0.2);
  glVertex3f(0.63,0.5,0.2);        //quad front window
  glVertex3f(1.2,0.5,0.2);
  glVertex3f(1.2,0.65,0.2);

  glVertex3f(1.22,0.5,0.2);
  glVertex3f(1.22,0.65,0.2);
  glVertex3f(1.48,0.65,0.2);        //quad back window
  glVertex3f(1.48,0.5,0.2);

  glColor3f(r,g,b);
  glVertex3f(0.6,0.5,0.2);
  glVertex3f(0.63,0.5,0.2);       //first separation
  glVertex3f(0.9,0.65,0.2);
  glVertex3f(0.93,0.65,0.2);

  glVertex3f(1.2,0.65,0.2);
  glVertex3f(1.2,0.5,0.2);       //second separation
  glVertex3f(1.22,0.5,0.2);
  glVertex3f(1.22,0.65,0.2);

  glVertex3f(1.48,0.65,0.2);
  glVertex3f(1.48,0.5,0.2);     //3d separation
  glVertex3f(1.8,0.5,0.2);
  glVertex3f(1.5,0.65,0.2);

 //**************************************************************
  glBegin(GL_QUADS);

  /* top of cube*/
  glColor3f(0.3,0.3,0.3);
  glVertex3f( 0.6, 0.5,0.6);
  glVertex3f(0.6, 0.5,0.2);        //quad front window
  glVertex3f(0.9, 0.65, 0.2);
  glVertex3f( 0.9,0.65,0.6);

  glVertex3f(1.5,0.65,.6);
  glVertex3f(1.5,0.65,0.2);        //quad back window
  glVertex3f(1.8,0.5,0.2);
  glVertex3f(1.8,0.5,0.6);


//*****************************road and surrounding development***********************************
  if(flag1)
  {
	glPushMatrix();
	glTranslatef(xw,0,0);
  glColor3f(0,1,0);
  glVertex3f(-100,0.1,-100);
  glVertex3f(-100,0.1,0);         //a green surroundings
  glVertex3f(100,0.1,0);
  glVertex3f(100,0.1,-100);

  glColor3f(0.7,0.7,0.7);
  glVertex3f(-100,0.1,0);
  glVertex3f(-100,0.1,0.45);         //a long road
  glVertex3f(100,0.1,0.45);
  glVertex3f(100,0.1,0);

  glColor3f(1.0,0.75,0.0);
  glVertex3f(-100,0.1,0.45);       //a median
  glVertex3f(-100,0.1,0.55);
  glVertex3f(100,0.1,0.55);
  glVertex3f(100,0.1,0.45);

  glColor3f(0.7,0.7,0.7);
  glVertex3f(-100,0.1,0.55);
  glVertex3f(-100,0.1,1);         //a long road
  glVertex3f(100,0.1,1);
  glVertex3f(100,0.1,0.55);

  glColor3f(0,1,0);
  glVertex3f(-100,0.1,1);
  glVertex3f(-100,0.1,100);         //a green surroundings
  glVertex3f(100,0.1,100);
  glVertex3f(100,0.1,1);
    glPopMatrix();
  }
  glEnd();

  if(wheelflag)
  {
	  glPushMatrix();
	  glTranslatef(xw,0,0);
	  glColor3f(0.5,.2,0.3);
	  glBegin(GL_QUADS);
	  for(i=0;i<200;i+=0.2)
	  {
		  glVertex3f(-100+i,0,1);
		  glVertex3f(-99.9+i,0,1);
		  glVertex3f(-99.9+i,0.2,1);
		  glVertex3f(-100+i,0.2,1);
		  i+=0.5;
	  }
	  for(i=0;i<200;i+=0.2)
	  {
		  glVertex3f(-100+i,0,0);
		  glVertex3f(-99.9+i,0,0);
		  glVertex3f(-99.9+i,0.2,0);
		  glVertex3f(-100+i,0.2,0);
		  i+=0.5;
	  }
	  glEnd();
	  glPopMatrix();
  }

//************IGNITION SYSTEM**********************************
glPushMatrix();
   glColor3f(0.3,0.3,0.3);
   glTranslatef(1.5,0.2,0.3);
   glRotatef(90.0,0,1,0);
   gluCylinder(t,0.02,0.03,.5,10,10);
glPopMatrix();
//********************WHEEL*********************************************

glColor3f(0.7,0.7,0.7);
glPushMatrix();
  glBegin(GL_LINE_STRIP);
	  for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(0.6,0.2,0.62);
  glVertex3f(0.6+(0.1*(cos(((theta+angle)*3.14)/180))),0.2+(0.1*(sin(((theta+angle)*3.14)/180))),0.62);
	  }
glEnd();

glBegin(GL_LINE_STRIP);
	  for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(0.6,0.2,0.18);
  glVertex3f(0.6+(0.1*(cos(((theta+angle)*3.14)/180))),0.2+(0.1*(sin(((theta+angle)*3.14)/180))),0.18);
	  }
glEnd();

glBegin(GL_LINE_STRIP);
for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(1.7,0.2,0.18);
  glVertex3f(1.7+(0.1*(cos(((theta+angle)*3.14)/180))),0.2+(0.1*(sin(((theta+angle)*3.14)/180))),0.18);
	  }
glEnd();

glBegin(GL_LINE_STRIP);
for(theta=0;theta<360;theta=theta+20)
	  {
  glVertex3f(1.7,0.2,0.62);
 glVertex3f(1.7+(0.1*(cos(((theta+angle)*3.14)/180))),0.2+(0.1*(sin(((theta+angle)*3.14)/180))),0.62);
	  }
glEnd();
  glTranslatef(0.6,0.2,0.6);
  glColor3f(0,0,0);
  glutSolidTorus(0.04,0.1,10,40);

  glTranslatef(0,0,-0.4);
  glutSolidTorus(0.04,0.1,10,40);

  glTranslatef(1.1,0,0);
  glutSolidTorus(0.04,0.1,10,40);

  glTranslatef(0,0,0.4);
  glutSolidTorus(0.04,0.1,10,40);
  glPopMatrix();
//*************************************************************
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glutPostRedisplay();
  glutSwapBuffers();
 }
}


void NormalKey(GLubyte key, GLint x, GLint y)
{	
    switch ( key )    {
     case 'k'	: grass=LoadBMP("pht.bmp");
     			   glutDestroyWindow(window);
     			   break;	 
    	
     case 'd'    : flagg = 0;
     			   glutDestroyWindow(window);
     			   break;	 
	
	case 'e'    :  eflag = 0;
     			   glutDestroyWindow(window);
     			   break;
					
	case 'E'    :  eflag = 1;
     			   glutDestroyWindow(window);
     			   break;	 					      			   

     case 'r'	 : red = 1.0;
     			   blue = 0.0;
     			   green = 0.0;
     			   glutDestroyWindow(window);
     			   break;
     			   
     case 'b'	 : red = 0.0;
     			   blue = 1.0;
     			   green = 0.0;
     			   glutDestroyWindow(window);
     			   break;
					
	 case 'g'	 : red = 0.0;
     			   blue = 0.0;
     			   green = 1.0;
     			   glutDestroyWindow(window);
     			   break;
									
	 case 'c'	 : red = 0.0;
     			   blue = 0.0;
     			   green = 0.0;
     			   glutDestroyWindow(window);
     			   break;							   
     				
     case ESCAPE : printf("escape pressed. exit.\n");
	               glutDestroyWindow(window);	/* Kill our window */
	               exit(0);
                   break;

      case 'x': xangle += 5.0;
                      glutPostRedisplay();
                      break;

       case 'X':xangle -= 5.0;
                       glutPostRedisplay();
                       break;

      case 'y':
        yangle += 5.0;
        glutPostRedisplay();
        break;

     case 'Y':
        yangle -= 5.0;
        glutPostRedisplay();
        break;

     case 'z':
        zangle += 5.0;
        glutPostRedisplay();
        break;

     case 'Z':
        zangle -= 5.0;
        glutPostRedisplay();
        break;

      case 'u':                          /* Move up */
        yt += 0.2;
        glutPostRedisplay();
        break;

      case 'U':
        yt -= 0.2;                      /* Move down */
        glutPostRedisplay();
        break;

      case 'f':                          /* Move forward */
        zt += 0.2;
        glutPostRedisplay();
        break;

      case 'F':
        zt -= 0.2;                      /* Move away */
        glutPostRedisplay();
        break;

	  case 's':zs+=.2;
		  glutPostRedisplay();
		  break;

	  case 'S':zs-=0.2;
		  glutPostRedisplay();
		  break;

	  case 'a':ys+=.2;
		  glutPostRedisplay();
		  break;

	  case 'A':ys-=0.2;
		  glutPostRedisplay();
		  break;

	  case 'q':xs+=.2;
		  glutPostRedisplay();
		  break;

	  case 'Q':xs-=0.2;
		  glutPostRedisplay();
		  break;


     default: DrawGLScene();
	           break;
    }

}

static void SpecialKeyFunc( int Key, int x, int y )
{
	switch ( Key ) {
	case GLUT_KEY_RIGHT:
		if(!wheelflag)
		xt += 0.2;
		if(wheelflag)
		{
		angle+=5;
		xw+=0.2;
		}
	    glutPostRedisplay();
		break;

                    case GLUT_KEY_LEFT:
		if(!wheelflag)
		xt -= 0.2;
		if(wheelflag)
		{
		angle+=5;
		xw-=0.2;
		}
        glutPostRedisplay();
		break;
	}
}

void myMenu(int id)
{
		if (id==1)
	{
		flag1=0;
		wheelflag=0;
	glutPostRedisplay();

	}
	if(id ==2)
	{
		flag1=1;
		flag2=0;
		wheelflag=0;
		xangle += 5.0;
		glutPostRedisplay();
	}
	if(id==3)
	{
		flag2=1;
		wheelflag=0;
		xangle += 5.0;
		glutPostRedisplay();
	}
	if (id==4)
	{
		 wheelflag=1;
		 glutPostRedisplay();
	}
	if (id==5)
	{
               if(day)
                {

		if(light)
		{
			count++;
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			light=0;
		}
		else
		{
			count--;
			light=1;
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}
		glutPostRedisplay();
                }
               else
                {

                  if(nml==0 && flag2==2)
                     {
                       flag2=0;
                       nml=1;
                     }
                   else
                     {
                         flag2=2;
                 nml=0;

                 aflag=0;
                 day=0;

	         glClearColor(0.1,0.1,0.1,0);
	         GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};

                 glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
                 glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
                 glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
                         /* end */
                glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
                glEnable(GL_FOG);


	        glutPostRedisplay();
                 }
             }

	}


       if(id==12)
	{
	aflag=1;
        day=1;
	glClearColor(1,1,1,1);
	glDisable(GL_FOG);
	glutPostRedisplay();
	}

	if(id==13)
	{
	aflag=0;
        day=0;
	flag2=2;
	glClearColor(0.1,0.1,0.1,0);
	GLfloat fogcolour[4]={0.0,0.0,0.0,1.0};

  glFogfv(GL_FOG_COLOR,fogcolour);              /* Define the fog colour */
  glFogf(GL_FOG_DENSITY,0.5);                   /* How dense */
  glFogi(GL_FOG_MODE,GL_EXP);                   /* exponential decay */
                         /* end */
  glHint(GL_FOG_HINT, GL_FASTEST);              /* compute per vertex */
  glEnable(GL_FOG);

  glutPostRedisplay();
       }
}

void colorMenu(int id)
{
		if (id==6)
	{
		r=g=0;
		b=1;
	glutPostRedisplay();

	}
	if(id ==7)
	{
		r=0.8;
		b=g=0;
		glutPostRedisplay();
	}
	if(id==8)
	{
	    g=1;
		r=b=0;
		glutPostRedisplay();
	}
	if (id==9)
	{
		 r=b=g=0;
		 glutPostRedisplay();
	}
	if(id==10)
	{
		b=0;
		r=g=1;
        glutPostRedisplay();
	}
	if(id==11)
	{
		b=r=g=.7;
        glutPostRedisplay();
	}

}

void myreshape(int w,int h)
{
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(w<=h)
                glOrtho(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,2.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
        else
                glOrtho(-2.0*(GLfloat)w/(GLfloat)h,2.0*(GLfloat)w/(GLfloat)h,-2.0,2.0,-10.0,10.0);
        glMatrixMode(GL_MODELVIEW);
        glutPostRedisplay();
}


//*************************** Main ***************************************************************

int main(int argc, char **argv)
{

/* Initialisation and window creation */

  glutInit(&argc, argv);               /* Initialize GLUT state. */

  glutInitDisplayMode(GLUT_RGBA |      /* RGB and Alpha */
                      GLUT_DOUBLE|     /* double buffer */
                      GLUT_DEPTH);     /* Z buffer (depth) */

  glutInitWindowSize(Xsize,Ysize);     /* set initial window size. */
  glutInitWindowPosition(0,0);         /* upper left corner of the screen. */

  glutCreateWindow("3D CAR ANIMATION"); /* Open a window with a title. */

/* Now register the various callback functions */

  glutReshapeFunc(myreshape);
  glutDisplayFunc(DrawGLScene);        /* Function to do all our OpenGL drawing. */
  glutReshapeFunc(ReSizeGLScene);
  glutKeyboardFunc(NormalKey);         /*Normal key is pressed */
  glutSpecialFunc( SpecialKeyFunc );
  InitGL(Xsize,Ysize);
  int submenu=glutCreateMenu(colorMenu);
	 glutAttachMenu(GLUT_RIGHT_BUTTON);

  /* Now drop into the event loop from which we never return */

  glutMainLoop();                      /* Start Event Processing Engine. */
  return 1;
}



