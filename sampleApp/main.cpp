
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

double x = -5;
double y = 5;
bool incrementingX = true, incrementingY = true;
double moveAmtY = 0.01, moveAmtX = 0.001;
float red = 1.0f, blue = 1.0f, green = 1.0f;
int frame = 0, itime, timebase = 0;
char s[50];
int h, w;
void *font = GLUT_STROKE_ROMAN;

static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;
	h = height;
	w = width;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void renderBitmapString(
                        float x,
                        float y,
                        float z,
                        void *font,
                        char *string) {
    
    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void renderStrokeFontString(
                            float x,
                            float y,
                            float z,
                            void *font,
                            char *string) {
    
    char *c;
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.002f, 0.002f, 0.002f);
    for (c = string; *c != '\0'; c++) {
        glutStrokeCharacter(font, *c);
    }
    glPopMatrix();
}

void restorePerspectiveProjection() {
    
	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();
    
	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {
    
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
    
	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
    
	// reset matrix
	glLoadIdentity();
    
	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);
    
	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

static void display(void)
{
    int r;
	int maxInt = 9;
    int minInt = maxInt * -1;
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	frame++;
	itime = glutGet(GLUT_ELAPSED_TIME);
	if (itime - timebase > 1000) {
		sprintf(s, "FPS:%4.2f", frame*1000.0 / (itime - timebase));
		timebase = itime;
		frame = 0;
	}
    
	glColor3d(1.0, 0.0, 0.0);
    
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	renderBitmapString(5, 30, 0, GLUT_BITMAP_TIMES_ROMAN_24, s);
	glPopMatrix();
	restorePerspectiveProjection();
    
    
    
	glColor3f(red, green, blue);
    
	glPushMatrix();
	
	if (incrementingX)
	{
		if (x < maxInt)
		{
			x += moveAmtX;
		}
		else
        {
            r = rand() % 10;
            r++;
            moveAmtX = (double)r / 10;
			incrementingX = false;
        }
	}
	else
	{
		if (x > minInt)
		{
			x -= moveAmtX;
		}
		else
		{
            r = rand() % 10;
            r++;
            moveAmtX = (double)r / 10;
            incrementingX = true;
        }
	}
    
    if (incrementingY)
	{
		if (y < maxInt)
		{
			y += moveAmtY;
		}
		else
        {
            r = rand() % 1000;
            r+=250;
            moveAmtY = (double)r / 1250;
			incrementingY = false;
        }
	}
	else
	{
		if (y > minInt)
		{
			y -= moveAmtY;
		}
		else
        {
            r = rand() % 1000;
            r+=250;
            moveAmtY = (double)r / 1250;
			incrementingY = true;
        }
	}
    
    
	glTranslated(x, y,-20);
	//glRotated(-20, 200.0, -5.0, 0.0);
	glutSolidSphere(3, 100, 100);
	glPopMatrix();
    
	/*glColor3d(1, 0, 0);
     glPushMatrix();
     
     glTranslated(0.0, -1.2, -6.0);
     glutWireTeapot(1);
     glPopMatrix();*/
	glutSwapBuffers();
    
}

const GLfloat light_ambient [] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse [] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular [] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position [] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient [] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse [] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular [] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess [] = { 100.0f };

void processSpecialKeys(int key, int x, int y) {
    
	switch (key) {
        case GLUT_KEY_F1:
            red = 1.0;
            green = 0.0;
            blue = 0.0; break;
        case GLUT_KEY_F2:
            red = 0.0;
            green = 1.0;
            blue = 0.0; break;
        case GLUT_KEY_F3:
            red = 0.0;
            green = 0.0;
            blue = 1.0; break;
	}
}

void processNormalKeys(unsigned char key, int x, int y) {
    
	if (key == 27)
		exit(0);
}

/* Program entry point */

void init() {
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(display);
    
	// here are the new entries
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
    
    
	glClearColor(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
    
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

int main(int argc, char *argv [])
{
    int r;
	glutInit(&argc, argv); 
	glutInitWindowSize(1024, 768);
     glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
    
	glutCreateWindow("Programming Techniques - 3D Teapots");
    
    srand(time(NULL));
    
    r = rand() % 100;
    r+=100;
    moveAmtX = (double)r / 200;
    
    r = rand() % 100;
    r+=100;
    moveAmtY = (double)r / 200;
    
	/*glutGameModeString("800x600:32");
	// enter full screen
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
		glutEnterGameMode();
	else {
		printf("The select mode is not available\n");
		exit(1);
	}*/
	init();
	
	/*while (1){*/
    glutMainLoop();
	/*}*/
    
    
	return EXIT_SUCCESS; 
} 

