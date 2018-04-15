#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#pragma comment(lib,"Winmm.lib")

#include <SDL2/SDL.h>

#define MUS_PATH "medium.wav"

#define ESCAPE 27
bool fullscreen = false;
bool mouseDown = false;
GLint window;
GLint window2;
GLint Xsize=1000;
GLint Ysize=800;
float i,theta;
GLint nml=0,day=1;

int flagt=0;

int grass;
int flagg1=0;
int eflag=0;

int flagg = 0;

float red = 0;
float green = 0;
float blue = 0;
void my_audio_callback(void *userdata, Uint8 *stream, int len);

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play


char name3[]="PROJECT:  3D CAR  ANIMATION";

GLfloat xt=0.0,yt=0.0,zt=0.0,xw=0.0;   /* x,y,z translation */
GLfloat tx=295,ty=62;
GLfloat xs=1.0,ys=1.0,zs=1.0;
float xdiff = 0.0f;
float ydiff = 0.0f;
GLfloat xangle=0.0,yangle=0.0,zangle=0.0,angle=0.0;   /* axis angles */

GLfloat r=0,g=0,b=1;
GLint light=1;
int count=1,flg=1;
int view=1;
int flag1=0,aflag=1;            //to switch car driving mode
int flag2=0,wheelflag=0;   //to switch fog effect
GLUquadricObj *t;

static void SpecialKeyFunc( int Key, int x, int y );
void idle()
{
if (!mouseDown)
{
xangle += 0.05f;
yangle += 0.04f;
glutPostRedisplay();
}
}
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

int fun()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
      return 1;

  // local variables
  static Uint32 wav_length; //length of our sample
  static Uint8 *wav_buffer; //buffer containing our audio file
  static SDL_AudioSpec wav_spec; //the specs of our piece of music
  
  
  /* Load the WAV */
  // the specs, length and buffer of our wav are filled
  if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
    return 1;
  }
  // set the callback function
  wav_spec.callback = my_audio_callback;
  wav_spec.userdata = NULL;
  // set our global static variables
  audio_pos = wav_buffer; // copy sound buffer
  audio_len = wav_length; // copy file length
  
  /* Open the audio device */
  
    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
      fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
      exit(-1);
    }
    
    /* Start playing */
       SDL_PauseAudio(0);

    // wait until we're don't playing
    while ( audio_len > 0 ) {
      SDL_Delay(100); 
    }
    
    // shut everything down
    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);  
}

void draw_texture()					// Texture
{		
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,grass);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.1, 0.4,0.55);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(0.5, 0.48,0.55);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(0.5, 0.48,0.25);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.1,0.4,0.25);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

GLuint loadimage(const char *fileName)          // load image
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
	if (size == NULL)
		size = width * height * 3;
	if (dataPos == NULL)
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

void init_texture()
{
    grass = loadimage("h.bmp") ;
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

void display1(void)
{
	glClearColor(1.0,1.0,0.1,1.0);
	glutPostRedisplay();
	glutSwapBuffers();
}

GLvoid DrawGLScene()
{
  


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
  glRotatef(xangle,1.0,0.0,0.0);							// rotate
  glRotatef(yangle,0.0,1.0,0.0);						
  glRotatef(zangle,0.0,0.0,1.0);
  glTranslatef(xt,yt,zt);								// translate
  glScalef(xs,ys,zs);									// scale
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

if(flagt==0)								//headlights
{
  glColor3f(.4,.1,0.2);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(0.1,0.3,0.3);
  glVertex3f(0.1,0.3,0.5);
  glEnd();
  glPointSize(200.0);
}
else
{
 glColor3f(.4,1,1);
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(0.1,0.3,0.3);
  glVertex3f(0.1,0.3,0.5);
  glEnd();
  glPointSize(200.0);
}

  glColor3f(1.0,0.0,0.0);					// back light
  glPointSize(30.0);
  glBegin(GL_POINTS);
  glVertex3f(1.9,0.3,0.5);
 glVertex3f(1.9,0.3,0.3); 
  glEnd();
  glPointSize(200.0);


 GLfloat mat_specular1[] = { red, green, blue, 1.0 };			// Lightning
   GLfloat mat_shininess1[] = { 2.0 };
   GLfloat light_position1[] = { 0.1, 0.3, 0.3, 0. };
   glClearColor (1.0, 1.0, 1.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess1);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position1);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

 
  glBegin(GL_QUADS);                /* OBJECT MODULE*/

  /* top of cube*/
  //************************FRONT BODY****************************************
/*  glColor3f(r,g,b);
  glVertex3f( 0.1, 0.4,0.6);
  glVertex3f(0.2, 0.415,0.6);
  glVertex3f(0.2, 0.415,0.2);
  glVertex3f(0.1,0.4,0.2);
  */
  if(eflag==0)										// bonate of car
  {			
  glColor3f(0,1,0);
  glVertex3f(0.1, 0.4,0.55);
  glVertex3f(0.5, 0.48,0.55);
  glVertex3f(0.5, 0.48,0.25);
  glVertex3f(0.1,0.4,0.25);
	 glEnd();
  }
  else
  {
  	glColor3f(0,1,0);
    glVertex3f(0.1, 0.4,0.55);
    glVertex3f(0.25, 0.7,0.55);
    glVertex3f(0.25, 0.7,0.25);
    glVertex3f(0.1,0.4,0.25);
    

	glColor3f(1,1,1);
	glEnd();


	draw_texture();						// car engine texture
  }

 glBegin(GL_QUADS);
  
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

  if(!flagg1)
{
 glVertex3f(0.9,0.65,0.6);
  glVertex3f(0.9,0.65,0.2);
  glVertex3f(1.5,0.65,0.2);        //top cover
  glVertex3f(1.5,0.65,0.6);
}
else
{
  glVertex3f(1.1,0.9,0.6);
  glVertex3f(1.1,0.9,0.2);
  glVertex3f(1.5,0.65,0.2);       
  glVertex3f(1.5,0.65,0.6);
}

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
	
	case 'e'    :  eflag = 0;					//bonate close
     			   glutDestroyWindow(window);
     			   break;
					
	case 'E'    :  eflag = 1;					//bonate open
     			   glutDestroyWindow(window);
     			   break;	 					      			   

     case 'r'	 : red = 1.0;					//red light
     			   blue = 0.0;
     			   green = 0.0;
     			   glutDestroyWindow(window);
     			   break;
		
     case 'b'	 : red = 0.0;					//green light
     			   blue = 1.0;
     			   green = 0.0;
     			   glutDestroyWindow(window);
     			   break;
					
	 case 'g'	 : red = 0.0;				// green light
     			   blue = 0.0;
     			   green = 1.0;
     			   glutDestroyWindow(window);
     			   break;
									
	 case 'c'	 : red = 0.0;				//clear light
     			   blue = 0.0;
     			   green = 0.0;
     			   glutDestroyWindow(window);
     			   break;
      case 'p'   :
	               flagg1=1;
                   glutPostRedisplay();
                   break;
      case  'P'  :
                   flagg1=0;
                   glutPostRedisplay();			

     case 'l'    :  flagt=1;				// head lights on
					glutDestroyWindow(window);
     			   break;

	case 'L'    :  flagt=0;					// head lights off
				   glutDestroyWindow(window);
     			   break;

	case 'O'    :  wheelflag = 1;
				   glutDestroyWindow(window);
     			   break;
     			   			
    case 'o'    :  wheelflag = 0;
				   glutDestroyWindow(window);
     			   break;	   
     				
     case ESCAPE : printf("escape pressed. exit.\n");
	               glutDestroyWindow(window);	/* Kill our window */
	               exit(0);
                   break;
	
      case 'x': 	  xangle += 5.0;					// rotate x-angle in +ve 
                      fun();
                      glutPostRedisplay();
                      break;

       case 'X':	   xangle -= 5.0;					// rotate x-angle in -ve 
                        fun();
                       glutPostRedisplay();
                       break;

      case 'y':		yangle += 5.0;					// rotate y-angle in +ve 
                    fun();
        			glutPostRedisplay();
        			break;
				
     case 'Y':		yangle -= 5.0;					// rotate y-angle in -ve 
                     fun();
        			glutPostRedisplay();
        			break;

     case 'z':      zangle += 5.0;					// rotate z-angle in +ve 
      				glutPostRedisplay();
        			break;

     case 'Z':		zangle -= 5.0;					// rotate z-angle in -ve 
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

	  case 's':zs+=.2;					//Scale w.r.t z
		  glutPostRedisplay();
		  break;

	  case 'S':zs-=0.2;
		  glutPostRedisplay();
		  break;

	  case 'a':ys+=.2;					//Scale w.r.t y
		  glutPostRedisplay();
		  break;

	  case 'A':ys-=0.2;
		  glutPostRedisplay();
		  break;

	  case 'q':xs+=.2;					//Scale w.r.t x
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
void mouse(int button, int state, int x, int y)
{
if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
{
mouseDown = true;
 
xdiff = x - yangle;
ydiff = -y + xangle;
}
else
  mouseDown = false;
}
 
void mouseMotion(int x, int y)
{
if (mouseDown)
{
yangle = x - xdiff;
xangle = y + ydiff;
 
glutPostRedisplay();
}
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
init_texture();

  glutReshapeFunc(myreshape);
  glutDisplayFunc(DrawGLScene);        /* Function to do all our OpenGL drawing. */
  glutReshapeFunc(ReSizeGLScene);
  glutKeyboardFunc(NormalKey);         /*Normal key is pressed */
  glutSpecialFunc( SpecialKeyFunc );
  InitGL(Xsize,Ysize);
  int submenu=glutCreateMenu(colorMenu);
	 glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
  /* Now drop into the event loop from which we never return */

  glutMainLoop();                      /* Start Event Processing Engine. */
  return 1;
}

void my_audio_callback(void *userdata, Uint8 *stream, int len) {
  
  if (audio_len ==0)
    return;
  
  len = ( len > audio_len ? audio_len : len );
  //SDL_memcpy (stream, audio_pos, len);          // simply copy from one buffer into the other
  SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
  
  audio_pos += len;
  audio_len -= len;
}




