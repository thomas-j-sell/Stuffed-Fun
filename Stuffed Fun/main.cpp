/*
 *  main.cpp
 *  Stuffed Fun
 *
 *  Created by Thomas Sell on 5/31/11.
 */

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.141592654

// window dimensions
float window_height, window_width;

// fps info
int frame, fps_time, timebase = 0;
char fps[30];
int * font = (int *) GLUT_BITMAP_TIMES_ROMAN_24;
int bitmapHeight = 24;

//angle of rotation
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

float cRadius = 10.0f; // our radius distance from our character

float lastx, lasty;

//positions of the cubes
float positionz[100];
float positionx[100];

void cubepositions (void) { //set the positions of the cubes
    
    for (int i=0;i<100;i++)
    {
        positionz[i] = rand()%10 + 1;
        positionx[i] = rand()%10 + 1;
    }
}

//draw the cube
void cube (void) {
    for (int i=0;i<100 - 1;i++)
    {
        glPushMatrix();
        glTranslated(-positionx[i + 1] * 10, 0, -positionz[i + 1] * 
                     10); //translate the cube
        glutSolidCube(2); //draw the cube
        glPopMatrix();
    }
}

// change projecttion 
void setOrthographicProjection() {
    
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, window_width, 0, window_height);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -window_height, 0);
	glMatrixMode(GL_MODELVIEW);
}

// reset perspective
void resetPerspectiveProjection() {
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

// render string
void renderBitmapString(float x, float y, void *font, char *string)
{
    
    char *c;
    // set position to start drawing fonts
    glRasterPos2f(x, y);
    // loop all the characters in the string
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void init (void) {
    cubepositions();
}

void enable (void) {
    glEnable (GL_DEPTH_TEST); //enable the depth testing
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable (GL_COLOR_MATERIAL);
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader
    
}

void display (void) {
    glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    
    //clear the color buffer and the depth buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    //enable();
    
    glLoadIdentity(); 
    
    glTranslatef(0.0f, 0.0f, -cRadius);
    glRotatef(xrot,1.0,0.0,0.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(2); //Our character to follow
    
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,0.0f,-zpos); //translate the screen to the position of our camera
    glColor3f(1.0f, 1.0f, 1.0f);
    cube(); //call the cube drawing function
    
    // display fps info
    frame++;
	fps_time = glutGet(GLUT_ELAPSED_TIME);
	if (fps_time - timebase > 1000) {
		sprintf(fps,"FPS:%4.2f",frame*1000.0/(fps_time-timebase));
		timebase = fps_time;		
		frame = 0;
	}
    
    glClearColor(1, 1, 1, 1);
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30, 20, (void *)font, "GLUT Tutorial @ 3D Tech"); 
	renderBitmapString(30, 40, (void *)font, fps);
	renderBitmapString(30, 60, (void *)font, "Esc - Quit");
	glPopMatrix();
	resetPerspectiveProjection();
    
    glutSwapBuffers(); //swap the buffers
    angle++; //increase the angle
}

void reshape (int w, int h) {
    
    window_width = w;
    window_height = h;
    
    
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    
    glLoadIdentity ();
    
    glViewport (0, 0, w, h); //set the viewport to the current window specifications
    
    //set the perspective (angle of sight, width, height, , depth)
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 100.0); 
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
    
}

void keyboard (unsigned char key, int x, int y) {
    if (key=='q')
    {
        xrot += 1;
        if (xrot >360) xrot -= 360;
    }
    
    if (key=='z')
    {
        xrot -= 1;
        if (xrot < -360) xrot += 360;
    }
    
    if (key=='w')
    {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * PI);
        xrotrad = (xrot / 180 * PI); 
        xpos += float(sin(yrotrad));
        zpos -= float(cos(yrotrad));
        ypos -= float(sin(xrotrad));
    }
    
    if (key=='s')
    {
        float xrotrad, yrotrad;
        yrotrad = (yrot / 180 * PI);
        xrotrad = (xrot / 180 * PI); 
        xpos -= float(sin(yrotrad));
        zpos += float(cos(yrotrad));
        ypos += float(sin(xrotrad));
    }
    
    if (key=='d')
    {
        float yrotrad;
        yrotrad = (yrot / 180 * PI);
        xpos += float(cos(yrotrad)) * 0.2;
        zpos += float(sin(yrotrad)) * 0.2;
    }
    
    if (key=='a')
    {
        float yrotrad;
        yrotrad = (yrot / 180 * PI);
        xpos -= float(cos(yrotrad)) * 0.2;
        zpos -= float(sin(yrotrad)) * 0.2;
    }
    
    if (key==27)
    {
        exit(0);
    }
}

void mouseMovement(int x, int y) {
    int diffx = x-lastx; //check the difference between the current x and the last x position
    int diffy = y-lasty; //check the difference between the current y and the last y position
    
    //    if(diffx == 0 && diffy == 0)
    //        glutWarpPointer(window_width/2, window_height/2);
    
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the additionof the difference in the y position
    yrot += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA); 
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("A basic OpenGL Window"); 
    
    init (); 
    
    glutDisplayFunc (display); 
    glutIdleFunc (display);
    glutReshapeFunc (reshape);
    
    glutPassiveMotionFunc(mouseMovement); //check for mouse movement
    
    glutKeyboardFunc (keyboard); 
    
    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);
    
    glutMainLoop (); 
    return 0;
}