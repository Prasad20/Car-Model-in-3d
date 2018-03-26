#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
#define PI 3.14159265
struct Color {
  GLfloat r;
  GLfloat g;
  GLfloat b;
};

void ellipse1(int x,int y,int z,int start_a,int end_a,int a,int b,vector<pair<int,int> >&vec)
{
   for(int i=start_a;i<=end_a;i++)
     {
       int x1=x+a*cos(i*(PI/180.0));
       int y1=y+b*sin(i*(PI/180.0));
       vec.push_back({x1,y1});    
       
     }
    // cout<<vec[0].first<<"  "<<vec[0].second<<endl;
}
void setPixelColor(int x, int y, Color color) {
  glColor3f(color.r, color.g, color.b);
  glBegin(GL_POINTS);
  glVertex2i(x, y);
  glEnd();
  glFlush();
}

Color getPixelColor(GLint x, GLint y) {
  Color color;
  glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
  return color;
}
void floodFill(GLint x, GLint y, Color oldColor, Color newColor) {
  Color color;
  color = getPixelColor(x, y);

  if(color.r == oldColor.r && color.g == oldColor.g && color.b == oldColor.b)
  {
    setPixelColor(x, y, newColor);
    floodFill(x+1, y, oldColor, newColor);
    floodFill(x, y+1, oldColor, newColor);
    floodFill(x-1, y, oldColor, newColor);
    floodFill(x, y-1, oldColor, newColor);
  }
  return;
}

void ellipse(int x,int y,int z,int start_a,int end_a,int a,int b)
{
     vector<pair<int,int> >vec;
     ellipse1(x,y,z,start_a,end_a,a,b,vec);     
     
      
   for(int i=0;i<vec.size()-1;i++)
     {
       
         glBegin(GL_LINES);
         glVertex3d(vec[i].first,vec[i].second,z);
         glVertex3d(vec[i+1].first,vec[i+1].second,z);
          glEnd();
     }

    

}
void circle(int x,int y,int z,int start_a,int end_a,int a)
{
   glBegin(GL_LINE_STRIP);
   int xx,yy,flag=0;
   for(int i=start_a;i<=end_a;i++)
     {
       int x1=x+a*cos(i*(PI/180.0));
       int y1=y+a*sin(i*(PI/180.0));
       if(flag==0)
       {
         xx=x1;
         yy=y1;
       }
       flag=1;
       glVertex3d(x1,y1,z); 
     }
     cout<<xx<<" "<<yy<<endl;
     glEnd();
}
void line(int x0,int y0,int z0,int x1,int y1,int z1)
{
   glBegin(GL_LINES);
   glVertex3d(x0,y0,z0);
   glVertex3d(x1,y1,z1);
   glEnd();
   //cout<<"hii"<<endl; 
}
void onMouseClick(int button, int state, int x, int y)
{
  Color newColor = {0.7f,0.7f,0.7f};
  Color oldColor = {1.0f, 1.0f, 1.0f};

  floodFill( 300, 200, oldColor, newColor);
}

void displayCB(void)	
{
  glClear(GL_COLOR_BUFFER_BIT);		/* clear the display */
  glColor3f(0, 0, 0);		/* set current color to white */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //side views
  //wheel
   circle(100,100,0,0,180,100);
    circle(700,100,0,0,180,100);
  //connecting wheel line
   line(200,100,0,600,100,0);
   //bumper
   ellipse(800,200,0,-90,0,150,100);  
   line(950,200,0,950,350,0);
   line(950,350,0,910,350,0);
   //back window
   line(700,500,0,910,350,0);
   //top line
   line(700,500,0,300,500,0);

   ///left side views
  circle(300,490,0,90,170,10);
  line(290,491,0,175,350,0);
  line(175,350,0,-30,315,0);
  circle(-30,300-10,0,90,170,25);
  line(-54,294,0,-64,165,0);
  line(-64,165,0,-84+5,165,0);
  circle(-79,155,0,90,170,10);
  line(-88,156,0,-88,100,0);
  line(0,100,0,-88,100,0);

  //door
  line(225,125,0,575,125,0);
  line(225,125,0,225,300,0);
  ellipse(225+30,300,0,105,180,30,60);
  line(247,357,0,330,460,0);
  line(575-100,460,0,330,460,0);
  circle(575-100,460-190,0,40,90,190);
  line(670,330,0,620,392,0);
  line(670,330,0,700,227,0);
  circle(700,100,0,90,168,127);
  line(225,325,0,675,325,0);

  //top view 
 // line(-88,156,0,950,350,0);
  line(-88,156,200,950,350,200);
  line(-88,156,0,-88,156,200);      // white
  line(175,350,0,175,350,200);      // white
  line(330,460,0,330,460,200);      // black
  line(700,500,0,700,500,200);      // white
  line(910,350,0,910,350,200);      // black
  line(950,350,0,950,350,200);      // white
    
 // back side view
  circle(100,100,200,0,180,100);
    circle(700,100,200,0,180,100);
  //connecting wheel line
   line(200,100,200,600,100,200);
   //bumper
   ellipse(800,200,200,-90,0,150,100);  
   line(950,200,200,950,350,200);
   line(950,350,200,910,350,200);
   //back window
   line(700,500,200,910,350,200);
   //top line
   line(700,500,200,300,500,200);

   ///left side views
  circle(300,490,200,90,170,10);
  line(290,491,200,175,350,200);
  line(175,350,200,-30,315,200);
  circle(-30,300-10,200,90,170,25);
  line(-54,294,200,-64,165,200);
  line(-64,165,200,-84+5,165,200);
  circle(-79,155,200,90,170,10);
  line(-88,156,200,-88,100,200);
  line(0,100,200,-88,100,200);

  //door
  line(225,125,200,575,125,200);
  line(225,125,200,225,300,200);
  ellipse(225+30,300,200,105,180,30,60);
  line(247,357,200,330,460,200);
  line(575-100,460,200,330,460,200);
  circle(575-100,460-190,200,40,90,190);
  line(670,330,200,620,392,200);
  line(670,330,200,700,227,200);
  circle(700,100,200,90,168,127);
  line(225,325,200,675,325,200);

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
  glutInitWindowSize(1300,1300);		/* width=400pixels height=500pixels */
  win = glutCreateWindow("ellipse");	/* create window */

  /* from this point on the current window is win */
  
  glClearColor(1,1,1,0.0);	/* set background to black */
  gluOrtho2D(-100,1000,-100,1000);		/* how object is mapped to window */
  glutDisplayFunc(displayCB);		/* set window's display callback */
  glutMouseFunc(onMouseClick);
  glutKeyboardFunc(keyCB);		/* set window's key callback */

  glutMainLoop();			/* start processing events... */

  /* execution never reaches this point */

  return 0;
}


