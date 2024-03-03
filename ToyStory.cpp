#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>      // handle the window-managing operations
#else
#include <GL/glew.h>
#include <GL/freeglut.h>    // handle the window-managing operations
#endif
#include <stdlib.h>
#include <iostream>
//--------*******************varibles***************------------
using namespace std;
GLint win_width = 500,
win_hight = 500;
GLdouble PI = 3.14159265359;
int scene=0; // Initialize with the start scene
float cloudX = 0.0f; // Initial cloud position
float velocity_x = 0.0003f;
float carX = 0.0f; // Initial car position
float carspeed_x = 0.0003f;
float starX = 0.0f; // Initial star position
float velocityx = 0.0003f;
float velocity_b = 0.0001;
float boatX = 0.0;
int  deviceButton;
float transValuegy = 0;
float transValueby = 0;
float transValuegx = 0;
float transValuebx = 0;
float doorAngle = 0.0f;  // Initial door angle (0 degrees, closed)
float doorRotationSpeed = 0.02f;  // Adjust rotation speed as needed
float charAngle = 0.0f;  // Initial door angle (0 degrees, closed)
float charRotationSpeed = 0.008f;  // Adjust rotation speed as needed
// buffers
GLuint myTexture1;
char  image1Path[]= "C:/Users/vip/source/repos/project0/woodenTexture.bmp";
//-------------texture part-----------
//http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture
GLuint LoadTexture(const char* filename, int width, int height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;

    //The following code will read in our RAW file
    fopen_s(&file, filename, "rb");

    if (file == NULL)
    {
        cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
        exit(0);
        return 0;
    }

    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);

    fclose(file);


    // reorder the image colors to RGB not BGR
    for (int i = 0; i < width * height; ++i)
    {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

    }
    glGenTextures(1, &texture);            //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //set texture environment parameters

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //ex4
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    /////////////////////////////////////////

    free(data); //free the texture array

    if (glGetError() != GL_NO_ERROR)
        printf("GLError in genTexture()\n");

    return texture; //return whether it was successfull  
}
//----------------************drawing part*************--------------
/*
 * Function that handles the drawing of a circle using the triangle fan
 * method. This will create a filled circle.
 * Params:
 *    x (GLFloat) - the x position of the center point of the circle
 *    y (GLFloat) - the y position of the center point of the circle
 *    radius (GLFloat) - the radius that the painted circle will have
 *    function Source: https://gist.github.com/linusthe3rd/803118
 */
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}
void drawHalfCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 1.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}
void drawFilledCircleOval(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle

    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (0.05 * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int lineAmount = 100; //# of triangles used to draw circle
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;
    glBegin(GL_LINE_LOOP);
    for (i = 0; i <= lineAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / lineAmount)),
            y + (radius * sin(i * twicePi / lineAmount))
        );
    }
    glEnd();
}
void drawHalfupCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = -1.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}
void drawHalfdownCircle(GLfloat x, GLfloat y, GLfloat radius) {
    int i;
    int triangleAmount = 20; //# of triangles used to draw circle
    //GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 1.0f * PI;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}
void drawString(void* font, const char* string, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}
void Button(){
    glBegin(GL_QUADS);
    glColor4f(0.5, 0.5, 0.5, 0.8);
    glVertex2f(0.6, -1);
    glVertex2f(0.6, -0.6);
    glVertex2f(1, -0.6);
    glVertex2f(1, -1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor4f(1, 1, 1, 0.5);
    glVertex2f(0.7, -0.65);
    glVertex2f(0.7, -0.9);
    glVertex2f(0.9, -0.78);
    glEnd();
    glColor3f(0, 0, 0);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "NEXT", 0.7, -0.8);
}
void reshapeFun(GLint newWidth, GLint newHight)
{
    if (scene == 4) {
        glViewport(0, 0, newWidth, newHight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1, 1, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
    }
    else {
        glViewport(0, 0, newWidth, newHight);
        win_width = newWidth;
        win_hight = newHight;
    }
}
//---------------**************************** main characters ********************************------------------------------
void Jessie(){
    // drawing Jessie's hair - Brown
    glColor3f(0.65f, 0.16f, 0.16f);
    drawFilledCircle(0, 0, 0.3);
    drawFilledCircle(0.1, -0.27, 0.04);
    drawFilledCircle(0.17, -0.27, 0.04);
    drawFilledCircle(0.2, -0.27, 0.045);
    drawFilledCircle(0.26, -0.27, 0.044);
    drawFilledCircle(0.3, -0.28, 0.044);
    glColor3f(0.9f, 0.7f, 0.0f);
    drawFilledCircleOval(0.33, -0.27, 0.025);
    //    drawFilledCircle(0, -0.2, 0.25);
    glColor3f(1.0f, 0.85f, 0.7f);
    drawFilledCircle(0, 0, 0.25);
    // Drawing Jessie's Neck - Peach color
    glColor3f(1.0f, 0.85f, 0.7f); // Skin color
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, -0.15f);
    glVertex2f(0.05f, -0.15f);
    glVertex2f(0.05f, -0.4f);
    glVertex2f(-0.05f, -0.4f);
    glEnd();
    //  Arms
    glColor3f(1.0f, 0.85f, 0.7f);
    glBegin(GL_QUADS);
    // Left arm (
    glVertex2f(-0.175f, -0.35f);
    glVertex2f(-0.125f, -0.35f);
    glVertex2f(-0.125f, -0.6f);
    glVertex2f(-0.175f, -0.6f);
    // Right arm
    glVertex2f(0.175f, -0.35f);
    glVertex2f(0.125f, -0.35f);
    glVertex2f(0.125f, -0.6f);
    glVertex2f(0.175f, -0.6f);
    glEnd();
    // J BODY
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.125f, -0.35f);  // Top left
    glVertex2f(0.125f, -0.35f);   // Top right
    glVertex2f(0.125f, -0.65f);   // Bottom right
    glVertex2f(-0.125f, -0.65f);  // Bottom left
    glEnd();
    // Jessie's BLAZERRR
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.125f, -0.35f);  // Top left
    glVertex2f(0.125f, -0.35f);   // Top right
    glVertex2f(0.125f, -0.65f);   // Bottom right
    glVertex2f(-0.125f, -0.65f);  // Bottom left
    glEnd();
    //  Jessie's left sleeve
    glBegin(GL_QUADS);
    glVertex2f(-0.175f, -0.35f);
    glVertex2f(-0.125f, -0.35f);
    glVertex2f(-0.125f, -0.55f);
    glVertex2f(-0.175f, -0.55f);
    glEnd();
    //  right  sleeve
    glBegin(GL_QUADS);
    glVertex2f(0.175f, -0.35f);
    glVertex2f(0.125f, -0.35f);
    glVertex2f(0.125f, -0.55f);
    glVertex2f(0.175f, -0.55f);
    glEnd();
    //  golden belt
    glColor3f(1.0f, 0.843f, 0.0f);  // Golden color
    glBegin(GL_QUADS);
    glVertex2f(-0.125f, -0.45f);
    glVertex2f(0.125f, -0.45f);
    glVertex2f(0.125f, -0.48f);
    glVertex2f(-0.125f, -0.48f);
    glEnd();
    //  buckle
    glColor3f(0.753f, 0.753f, 0.753f);
    glBegin(GL_QUADS);
    glVertex2f(-0.025f, -0.44f);
    glVertex2f(0.025f, -0.44f);
    glVertex2f(0.025f, -0.49f);
    glVertex2f(-0.025f, -0.49f);
    glEnd();
    // Jessie's hat
    glColor3f(0.8f, 0.2f, 0.4f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.25f, 0.17f);  // Left bottom
    glVertex2f(0.25f, 0.17f);   // Right bottom
    glVertex2f(0.15f, 0.5f);    // Right top
    glVertex2f(-0.15f, 0.5f);   // Left top
    glEnd();
    // Hat's brim
    glColor3f(0.8f, 0.3f, 0.3f);
    glBegin(GL_QUAD_STRIP);
    glVertex2f(-0.3f, 0.25f);
    glVertex2f(0.3f, 0.25f);
    glVertex2f(-0.28f, 0.2f);
    glVertex2f(0.28f, 0.2f);
    glEnd();
    // drawing Jessie's left eye
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.075, 0.075, 0.05);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-0.075, 0.075, 0.025);
    //left eyebrow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glLineWidth(10.0f);
    glVertex2f(-0.11, 0.152);
    glVertex2f(-0.04, 0.152);
    glVertex2f(-0.04, 0.148);
    glVertex2f(-0.11, 0.148);
    glEnd();
    // drawing Jessie's right eye
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    drawFilledCircle(0.075, 0.075, 0.05);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the pupil
    drawFilledCircle(0.075, 0.075, 0.025);
    // Jessie's mouth
    glColor3f(0.8f, 0.4f, 0.4f);
    drawHalfCircle(0.0, -0.1, -0.075);
    // Jessie's right eyebrow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glLineWidth(10.0f);
    glVertex2f(0.04, 0.152);
    glVertex2f(0.11, 0.152);
    glVertex2f(0.11, 0.148);
    glVertex2f(0.04, 0.148);
    glEnd();
    //  legs // Left leg
    glColor3f(0.53f, 0.81f, 0.98f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.65f);
    glVertex2f(-0.05f, -0.65f);
    glVertex2f(-0.05f, -0.85f);
    glVertex2f(-0.1f, -0.85f);
    glEnd();
    // Right leg
    glColor3f(0.53f, 0.81f, 0.98f);
    glBegin(GL_QUADS);
    glVertex2f(0.1f, -0.65f);
    glVertex2f(0.05f, -0.65f);
    glVertex2f(0.05f, -0.85f);
    glVertex2f(0.1f, -0.85f);
    glEnd();
    // Jessie's shoes
    glColor3f(0.545f, 0.271f, 0.075f);
    // Left shoe
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.85f);
    glVertex2f(-0.05f, -0.85f);
    glVertex2f(-0.05f, -0.9f);
    glVertex2f(-0.1f, -0.9f);
    glEnd();
    // Right shoe
    glBegin(GL_QUADS);
    glVertex2f(0.1f, -0.85f);
    glVertex2f(0.05f, -0.85f);
    glVertex2f(0.05f, -0.9f);
    glVertex2f(0.1f, -0.9f);
    glEnd();
}
void woody() {
    //hair
    glColor3f(0.5f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.23f, 0.07f);
    glVertex2f(0.23f, 0.07f);
    glVertex2f(0.21f, -0.2f);
    glVertex2f(-0.21f, -0.2f);
    glEnd();
    //woody's hat //haed
    glColor3f(1.0f, 0.85f, 0.7f);
    drawFilledCircle(0, -0.1, 0.2);
    glColor3f(0.5f, 0.2f, 0.1f);
    drawHalfupCircle(0.03, 0.1, .09);
    glColor3f(0.8f, 0.5f, 0.1f);
    drawHalfCircle(0.0f, 0.17f, 0.13);
    //bottom
    glLineWidth(18);
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.5f, 0.1f);
    glVertex2f(0.25f, 0.1f);
    glVertex2f(-0.25f, 0.1f);
    glEnd();
    //tabe
    glLineWidth(12);
    glBegin(GL_LINES);
    glColor3f(0.6f, 0.3f, 0.0f);
    glVertex2f(0.2f, 0.15f);
    glVertex2f(-0.2f, 0.15f);
    glEnd();
    // Neck - Peach color
    glColor3f(1.0f, 0.85f, 0.7f); // Skin color
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, -0.27f);
    glVertex2f(0.05f, -0.27f);
    glVertex2f(0.05f, -0.4f);
    glVertex2f(-0.05f, -0.4f);
    glEnd();
    //  Arms
    glColor3f(1.0f, 0.85f, 0.7f);
    glBegin(GL_QUADS);
    // Left arm (
    glVertex2f(-0.15f, -0.35f);
    glVertex2f(-0.1f, -0.35f);
    glVertex2f(-0.21f, -0.6f);
    glVertex2f(-0.267f, -0.6f);
    glEnd();
    glBegin(GL_QUADS);
    // Right arm
    glVertex2f(0.15f, -0.35f);
    glVertex2f(0.1f, -0.35f);
    glVertex2f(0.21f, -0.6f);
    glVertex2f(0.267f, -0.6f);
    glEnd();
    // BODY
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.35f);  // Top left
    glVertex2f(0.1f, -0.35f);   // Top right
    glVertex2f(0.1f, -0.6f);   // Bottom right
    glVertex2f(-0.1f, -0.6f);  // Bottom left
    glEnd();
    //  BLAZERRR
    glColor3f(0.9f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.35f);  // Top left
    glVertex2f(0.1f, -0.35f);   // Top right
    glVertex2f(0.1f, -0.6f);   // Bottom right
    glVertex2f(-0.1f, -0.6f);  // Bottom left
    glEnd();
    //   left sleeve
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.35f);
    glVertex2f(-0.1f, -0.35f);
    glVertex2f(-0.19f, -0.55f);
    glVertex2f(-0.25f, -0.55f);
    glEnd();
    //  right  sleeve
    glBegin(GL_QUADS);
    glVertex2f(0.15f, -0.35f);
    glVertex2f(0.1f, -0.35f);
    glVertex2f(0.19f, -0.55f);
    glVertex2f(0.25f, -0.55f);
    glEnd();
    //potton
    glColor3f(1, 1, 1);
    drawFilledCircle(0.0, -0.48, 0.01);
    drawFilledCircle(0.0, -0.43, 0.01);
    drawFilledCircle(0.0, -0.38, 0.01);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, -0.35f);
    glVertex2f(-0.1f, -0.35f);
    glVertex2f(-0.1f, -0.5f);
    glVertex2f(-0.05f, -0.5f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.05f, -0.35f);
    glVertex2f(0.1f, -0.35f);
    glVertex2f(0.1f, -0.5f);
    glVertex2f(0.05f, -0.5f);
    glEnd();
    //  left eye
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(-0.075, -0.08, 0.05);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(-0.075, -0.08, 0.025);
    //left eyebrow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.11, 0.0);
    glVertex2f(-0.04, 0.0);
    glVertex2f(-0.04, -0.015);
    glVertex2f(-0.11, -0.015);
    glEnd();
    //  right eye
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    drawFilledCircle(0.075, -0.08, 0.05);
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the pupil
    drawFilledCircle(0.075, -0.08, 0.025);
    //  right eyebrow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.04, 0.0);
    glVertex2f(0.11, 0.0);
    glVertex2f(0.11, -0.015);
    glVertex2f(0.04, -0.015);
    glEnd();
    // mouth
    glColor3f(0.8f, 0.4f, 0.4f);
    drawHalfCircle(0.0, -0.15, -0.075);
    //pants
    glColor3f(0.3f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.6f);
    glVertex2f(0.1f, -0.6f);
    glVertex2f(0.1f, -0.65f);
    glVertex2f(-0.1f, -0.65f);
    glEnd();
    //  legs// Left leg
    glColor3f(0.3f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.65f);
    glVertex2f(-0.05f, -0.65f);
    glVertex2f(-0.05f, -0.85f);
    glVertex2f(-0.1f, -0.85f);
    glEnd();
    // Right leg
    glColor3f(0.3f, 0.5f, 1.f);
    glBegin(GL_QUADS);
    glVertex2f(0.1f, -0.65f);
    glVertex2f(0.05f, -0.65f);
    glVertex2f(0.05f, -0.85f);
    glVertex2f(0.1f, -0.85f);
    glEnd();
    // belt
    glColor3f(0.5f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.6f);
    glVertex2f(0.1f, -0.6f);
    glVertex2f(0.1f, -0.57f);
    glVertex2f(-0.1f, -0.57f);
    glEnd();
    //  buckle
    glColor3f(0.6f, 0.6f, 0.6f);
    drawFilledCircle(0.0, -0.59, 0.03);
    //  shoes
    glColor3f(0.545f, 0.271f, 0.075f);
    // Left shoe
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.85f);
    glVertex2f(-0.05f, -0.85f);
    glVertex2f(-0.05f, -0.9f);
    glVertex2f(-0.1f, -0.9f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, -0.9f);
    glVertex2f(-0.05f, -0.9f);
    glVertex2f(-0.1f, -0.85f);
    glEnd();
    // Right shoe
    glBegin(GL_QUADS);
    glVertex2f(0.1f, -0.85f);
    glVertex2f(0.05f, -0.85f);
    glVertex2f(0.05f, -0.9f);
    glVertex2f(0.1f, -0.9f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(0.15f, -0.9f);
    glVertex2f(0.05f, -0.9f);
    glVertex2f(0.1f, -0.85f);
    glEnd();
}
void bear(){
    glColor3f(0.5f, 0.0f, 0.2f);    //the bear body
    drawFilledCircle(0, -0.3, 0.3);
    glColor3f(0.9f, 0.7f, 0.7f);    //the bear inner body
    drawFilledCircle(0, -0.3, 0.2);
    glPushMatrix();
    glScalef(1.9, 1.0, 0.0);
    glColor3f(0.5f, 0.0f, 0.2f);    //the bear right hand
    drawFilledCircle(0.18, -0.2, 0.08);
    glPopMatrix();
    glPushMatrix();
    glScalef(1.9, 1.0, 0.0);
    glColor3f(0.5f, 0.0f, 0.2f);    //the bear left hand
    drawFilledCircle(-0.18, -0.2, 0.08);
    glPopMatrix();
    glColor3f(0.9f, 0.7f, 0.7f);    //the inner right hand
    drawFilledCircle(0.43, -0.2, 0.04);
    glColor3f(0.9f, 0.7f, 0.7f);    //the inner left hand
    drawFilledCircle(-0.43, -0.2, 0.04);
    glPushMatrix();
    glScalef(1., 1.7, 0.0);
    glColor3f(0.5f, 0.0f, 0.2f);    //the bear right foot
    drawFilledCircle(0.1, -0.37, 0.08);
    glPopMatrix();
    glPushMatrix();
    glScalef(1., 1.7, 0.0);
    glColor3f(0.5f, 0.0f, 0.2f);    //the bear left foot
    drawFilledCircle(-0.1, -0.37, 0.08);
    glPopMatrix();
    glColor3f(0.5f, 0.0f, 0.2f);   //the right ear
    drawFilledCircle(0.2, 0.3, 0.12);
    glColor3f(0.9f, 0.7f, 0.7f);   //the inner right ear
    drawFilledCircle(0.2, 0.3, 0.06);
    glColor3f(0.5f, 0.0f, 0.2f);    //the left ear
    drawFilledCircle(-0.2, 0.3, 0.12);
    glColor3f(0.9f, 0.7f, 0.7f); ;   //the inner left ear
    drawFilledCircle(-0.2, 0.3, 0.06);
    glColor3f(0.5f, 0.0f, 0.2f);    //the bear face
    drawFilledCircle(0, 0.1, 0.25);
    glColor3f(0.9f, 0.7f, 0.7f);    //the nose
    drawFilledCircle(0.0, 0.0, 0.1);
    glColor3f(1.0f, 1.0f, 1.0f);   //the right eye
    drawFilledCircle(0.09, 0.19, 0.06);
    glColor3f(1.0f, 1.0f, 1.0f);   //the left eye
    drawFilledCircle(-0.09, 0.19, 0.06);
    glColor3f(0.09f, 0.09f, 0.09f);   //the right eye color
    drawFilledCircle(0.09, 0.19, 0.03);
    glColor3f(0.09f, 0.09f, 0.09f);   //the left eye color
    drawFilledCircle(-0.09, 0.19, 0.03);
    glBegin(GL_QUADS);  //for the right eye
    glColor3f(0.5f, 0.0f, 0.2f);
    glVertex2f(0.0f, 0.23f);
    glVertex2f(0.0f, 0.28f);
    glVertex2f(0.2f, 0.23f);
    glVertex2f(0.2f, 0.28f);
    glEnd();
    glBegin(GL_QUADS);  //for the left eye
    glColor3f(0.5f, 0.0f, 0.2f);
    glVertex2f(0.0f, 0.23f);
    glVertex2f(0.0f, 0.28f);
    glVertex2f(-0.2f, 0.23f);
    glVertex2f(-0.2f, 0.28f);
    glEnd();
    glColor3f(0.3f, 0.0f, 0.0f);    //the little nose
    drawFilledCircle(0.0, 0.03, 0.05);
    glBegin(GL_LINES);                // the mouth line
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, -0.02f);
    glVertex2f(0.0f, -0.1f);
    glEnd();
}
void MrPootatoHead() {
    //ears 
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(1, 0.6, 0.9);
    drawFilledCircle(0.30, 0.06, 0.08);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(1, 0.6, 0.9);
    drawFilledCircle(-0.30, 0.06, 0.08);
    glPopMatrix();
    //earholes
    glLineWidth(2);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(1, 0.2, 1);
    drawHollowCircle(0.30, 0.06, 0.06);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(1, 0.2, 1);
    drawHollowCircle(-0.30, 0.06, 0.06);
    glPopMatrix();
    glColor3f(1, 0.2, 1);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawFilledCircle(0.26, 0.06, 0.016);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawFilledCircle(-0.26, 0.06, 0.016);
    glPopMatrix();
    //arms
    glLineWidth(5);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(0.23, -0.02);
    glVertex2f(0.3, -0.17);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(0.31, -0.17);
    glVertex2f(0.15, -0.25);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-0.23, -0.02);
    glVertex2f(-0.3, -0.17);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-0.31, -0.17);
    glVertex2f(-0.15, -0.25);
    glEnd();
    glPopMatrix();
    //hands and thumbs
    glColor3f(1, 1, 1);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawFilledCircle(0.18, -0.25, 0.04);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1, 0.7 * 2, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawFilledCircle(0.16, -0.125, 0.015);
    glPopMatrix();
    glLineWidth(1);
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1, 0.7 * 2, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawHollowCircle(0.16, -0.125, 0.015);
    glPopMatrix();
    glColor3f(1, 1, 1);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawFilledCircle(-0.18, -0.25, 0.04);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1, 0.7 * 2, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawFilledCircle(-0.16, -0.125, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1, 0.7 * 2, 1);
    glTranslatef(0 - 0.8, 0, 0);
    drawHollowCircle(-0.16, -0.125, 0.015);
    glPopMatrix();
    //mr potatohead's body
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0.7, 0.4, 0.2);
    drawFilledCircle(0, 0, 0.25);
    glPopMatrix();
    //face//eyebrows
    glLineWidth(5);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(0.03, 0.18);
    glVertex2f(0.12, 0.18);
    glVertex2f(-0.03, 0.18);
    glVertex2f(-0.12, 0.18);
    glEnd();
    glPopMatrix();
    //eyes
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(1, 1, 1);
    drawFilledCircle(0.08, 0.06, 0.06);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(1, 1, 1);
    drawFilledCircle(-0.08, 0.06, 0.06);
    glPopMatrix();
    //pupils
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0, 0, 0);
    drawFilledCircle(0.08, 0.06, 0.028);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0, 0, 0);
    drawFilledCircle(-0.08, 0.06, 0.028);
    glPopMatrix();
    //eyelids
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0.5, 0.3, 0.1);
    drawHalfCircle(0.08, 0.06, 0.06);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.3, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0.5, 0.3, 0.1);
    drawHalfCircle(-0.08, 0.06, 0.06);
    glPopMatrix();
    //mouth
    glPushMatrix();
    glColor3f(0.7, 0, 0);
    glRotatef(180, 1, 0, 0);
    glScalef(0.7 * 1.5, 0.7 * 1, 1.0);
    glTranslatef(0 - 0.533, 0.31, 0);
    drawHalfCircle(0, -0.17, 0.04);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glRotatef(180, 1, 0, 0);
    glScalef(0.7 * 2, 0.7 * 1, 1.0);
    glTranslatef(0 - 0.4, 0.31, 0);
    drawHalfCircle(0, -0.17, 0.02);
    glPopMatrix();
    //mustache
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.5, 0.7 * 1, 1.0);
    glTranslatef(0 - 0.533, 0, 0);
    drawHalfCircle(0, -0.135, 0.065);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-0.0974, -0.16);
    glVertex2f(-0.065, -0.12);
    glVertex2f(-0.065, -0.16);
    glVertex2f(-0.0974, -0.12);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-0.064, -0.16);
    glVertex2f(-0.0325, -0.12);
    glVertex2f(-0.0325, -0.16);
    glVertex2f(-0.064, -0.12);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(-0.0315, -0.16);
    glVertex2f(0, -0.12);
    glVertex2f(0, -0.16);
    glVertex2f(-0.0315, -0.12);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, -0.16);
    glVertex2f(0.0325, -0.12);
    glVertex2f(0.0325, -0.16);
    glVertex2f(0, -0.12);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0.0325, -0.16);
    glVertex2f(0.064, -0.12);
    glVertex2f(0.064, -0.16);
    glVertex2f(0.0325, -0.12);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0.065, -0.16);
    glVertex2f(0.0974, -0.12);
    glVertex2f(0.0974, -0.16);
    glVertex2f(0.065, -0.12);
    glEnd();
    glPopMatrix();
    //nose
    glPushMatrix();
    glColor3f(0.6, 0, 0);
    glScalef(0.7 * 1.5, 0.7 * 1, 1.0);
    glTranslatef(0 - 0.533, 0, 0);
    drawFilledCircle(0, -0.05, 0.048);
    glPopMatrix();
    //hat
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.2, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0, 0, 0);
    drawHalfCircle(0, 0.23, 0.13);
    glPopMatrix();
    glLineWidth(12);
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1, 1);
    glTranslatef(0 - 0.8, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-0.16, 0.28);
    glVertex2f(0.16, 0.28);
    glEnd();
    glPopMatrix();
    //shoes
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.2, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0, 0.5, 0.8);
    drawHalfCircle(0.08, -0.3, 0.08);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.7 * 1, 0.7 * 1.2, 1.0);
    glTranslatef(0 - 0.8, 0, 0);
    glColor3f(0, 0.5, 0.8);
    drawHalfCircle(-0.08, -0.3, 0.08);
    glPopMatrix();
    //left hand fingers
    glLineWidth(1);
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawFilledCircle(0.101, -0.23, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawHollowCircle(0.101, -0.23, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawFilledCircle(0.101, -0.28, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawHollowCircle(0.101, -0.28, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawFilledCircle(0.101, -0.255, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawHollowCircle(0.101, -0.255, 0.015);
    glPopMatrix();
    //right hand fingers
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawFilledCircle(-0.101, -0.23, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawHollowCircle(-0.101, -0.23, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawFilledCircle(-0.101, -0.28, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawHollowCircle(-0.101, -0.28, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(1, 1, 1);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawFilledCircle(-0.101, -0.255, 0.015);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glScalef(0.7 * 1.9, 0.7 * 1, 1);
    glTranslatef(0 - 0.421, 0, 0);
    drawHollowCircle(-0.101, -0.255, 0.015);
    glPopMatrix();
}
void alien()
{
    //head
    glColor3f(0.0f, 0.8f, 0.1f);
    drawHalfdownCircle(0, 0.2, .40);
    drawFilledCircle(0, 0.8, .09);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.0f, 0.8f);
    glEnd();
    //eyes 
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(0, 0.42, .07);
    drawFilledCircle(0.2, 0.42, .07);
    drawFilledCircle(-0.2, 0.42, .07);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawFilledCircle(0, 0.42, .04);
    drawFilledCircle(0.2, 0.42, .04);
    drawFilledCircle(-0.2, 0.42, .04);
    //mouth
    glColor3f(0.0f, 0.0f, 0.0f);
    drawHalfupCircle(0, 0.31, .09);
    //fingers
    glColor3f(0.0f, 0.8f, 0.1f);
    glLineWidth(10);
    glBegin(GL_LINES);
    glVertex2f(-0.6f, 0.05f);
    glVertex2f(-0.5f, -0.064f);//left 1
    glVertex2f(-0.65f, -0.09f);
    glVertex2f(-0.5f, -0.09f);//left 2
    glVertex2f(-0.6f, -0.20f);
    glVertex2f(-0.5f, -0.14f);//left 3
    glVertex2f(0.6f, 0.05f);
    glVertex2f(0.5f, -0.064f);//right 1
    glVertex2f(0.65f, -0.09f);
    glVertex2f(0.5f, -0.09f);//right 2
    glVertex2f(0.6f, -0.20f);
    glVertex2f(0.5f, -0.14f);//right 3
    glEnd();
    glColor3f(0.0f, 0.3f, 0.6f);
    //hands
    glLineWidth(30);
    glBegin(GL_LINES);
    glVertex2f(0.3f, 0.1f);
    glVertex2f(0.5f, -0.10f);//right
    glVertex2f(-0.3f, 0.1f);
    glVertex2f(-0.5f, -0.10f);//left
    glEnd();
    //suit
    glBegin(GL_QUADS);
    glVertex2f(0.4f, 0.21f);
    glVertex2f(-0.4f, 0.21f);
    glVertex2f(-0.4f, -0.75f);
    glVertex2f(0.4f, -0.75f);
    glEnd();
    //tie
    glColor3f(0.0f, 0.0f, 0.4f);
    glLineWidth(10);
    glBegin(GL_LINES);
    glVertex2f(0.4f, 0.19f);
    glVertex2f(-0.4f, 0.19f);
    glEnd();
    //foot
    glLineWidth(25);
    glBegin(GL_LINES);
    glVertex2f(0.5f, -0.7f);
    glVertex2f(-0.5, -0.7f);
    glEnd();
    //belt
    glBegin(GL_LINES);
    glLineWidth(0.5);
    glVertex2f(0.0f, -0.7f);
    glVertex2f(0.0f, -0.3f);
    glVertex2f(-0.4f, -0.3f);
    glVertex2f(0.4f, -0.3f);
    glEnd();
}
//-----------------***************design s0 Budur****************------------------------
void hat() {
    //up
    glColor3f(0.8f, 0.5f, 0.1f);
    drawHalfCircle(-0.5f, 0.23f, 0.1);
    //bottom
    glLineWidth(18);
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.5f, 0.1f);
    glVertex2f(-0.7f, 0.15f);
    glVertex2f(-0.3f, 0.15f);
    glEnd();
    //tabe
    glLineWidth(12);
    glBegin(GL_LINES);
    glColor3f(0.6f, 0.3f, 0.0f);
    glVertex2f(-0.6f, 0.21f);
    glVertex2f(-0.4f, 0.21f);
    glEnd();
}
void myClouds() {
    glColor3f(1.0f, 1.0f, 1.0f);
    // drawing the  right up cloud 
    drawFilledCircle(0.6 + cloudX, 0.7, 0.09);
    drawFilledCircle(0.5 + cloudX, 0.6, 0.09);
    drawFilledCircle(0.6 + cloudX, 0.6, 0.09);
    drawFilledCircle(0.45 + cloudX, 0.65, 0.09);
    drawFilledCircle(0.70 + cloudX, 0.65, 0.09);
    //corners left
    drawFilledCircle(-0.9, 0.9, 0.09);
    drawFilledCircle(-1.0, 0.9, 0.09);
    drawFilledCircle(-0.95, 0.95, 0.09);
    //corner right
    drawFilledCircle(1.0, -0.9, 0.09);
    drawFilledCircle(0.9, -1.0, 0.09);
    // drawing the  left up cloud 
    drawFilledCircle(-0.4 + cloudX, 0.7, 0.09);
    drawFilledCircle(-0.5 + cloudX, 0.6, 0.09);
    drawFilledCircle(-0.4 + cloudX, 0.6, 0.09);
    drawFilledCircle(-0.30 + cloudX, 0.65, 0.09);
    drawFilledCircle(-0.55 + cloudX, 0.65, 0.09);
    // drawing the  left down cloud 
    drawFilledCircle(-0.58 + cloudX, -0.6, 0.09);
    drawFilledCircle(-0.5 + cloudX, -0.5, 0.09);
    drawFilledCircle(-0.6 + cloudX, -0.5, 0.09);
    drawFilledCircle(-0.45 + cloudX, -0.60, 0.09);
    drawFilledCircle(-0.70 + cloudX, -0.60, 0.09);
    // drawing the  right down cloud 
    drawFilledCircle(0.50 + cloudX, -0.50, 0.09);
    drawFilledCircle(0.58 + cloudX, -0.54, 0.09);
    drawFilledCircle(0.60 + cloudX, -0.40, 0.09);
    drawFilledCircle(0.70 + cloudX, -0.50, 0.09);
    // drawing the  middle down cloud 
    drawFilledCircle(-0.1 + cloudX, -0.75, 0.09);
    drawFilledCircle(0.0 + cloudX, -0.65, 0.09);
    drawFilledCircle(-0.0 + cloudX, -0.65, 0.09);
    drawFilledCircle(0.1 + cloudX, -0.68, 0.09);
    drawFilledCircle(0.13 + cloudX, -0.73, 0.09);
    drawFilledCircle(0.0 + cloudX, -0.70, 0.09);
    drawFilledCircle(0.0 + cloudX, -0.75, 0.09);
    // drawing the right corner cloud 
    drawFilledCircle(0.5, 0.1, 0.09);
    drawFilledCircle(0.6, 0.1, 0.09);
    drawFilledCircle(0.5, 0.2, 0.09);
    drawFilledCircle(0.4, 0.1, 0.09);
    // drawing the left corner cloud 
    drawFilledCircle(-0.5, -0.1, 0.09);
    drawFilledCircle(-0.6, -0.1, 0.09);
    drawFilledCircle(-0.5, 0.0, 0.09);
    drawFilledCircle(-0.4, -0.1, 0.09);
}
void rectangle() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS); // Each set of 4 vertices form a quad
    glColor3f(1.0f, 0.0f, 0.0f); // Red, f = float
    glVertex2f(-0.5f, -0.10f); // x, y
    glVertex2f(0.5f, -0.10f);
    glVertex2f(0.5f, 0.15f);
    glVertex2f(-0.5f, 0.15f);
    glEnd();
}
void s0()
{
    myClouds();
    rectangle();
    // Render the text in the rectangle
    glColor3f(1.0f, 0.9f, 0.0f);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "you've got a friend in me", -0.49f, 0.0f);
    hat();
    //  click rectnagle 
    glColor4f(0.2, 0.2, 0.2, 0.8);
    glBegin(GL_QUADS);
    glVertex2f(0.32f, -0.95f); // Top-right vertex
    glVertex2f(0.32f, -0.8f); // Bottom-right vertex
    glVertex2f(-0.32f, -0.8f); // Bottom-left vertex
    glVertex2f(-0.32f, -0.95f); // Top-left vertex
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "Click to start:", -0.27, -0.9);
}
//-----------------***************design s1 **************-------------------------------
void Cloud() {
    glColor3f(1, 1, 1);
    drawFilledCircle(0.6, 0.8, 0.08);
    glColor3f(1, 1, 1);
    drawFilledCircle(0.5, 0.75, 0.08);
    glColor3f(1, 1, 1);
    drawFilledCircle(0.7, 0.75, 0.08);
    glColor3f(1, 1, 1);
    drawFilledCircle(0.65, 0.7, 0.08);
    glColor3f(1, 1, 1);
    drawFilledCircle(0.55, 0.7, 0.08);
}
void Room() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);
    glColor3f(0.6, 0.8, 1);
    glVertex2f(-1, 1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glEnd();
    //small clouds(first row)
    glPushMatrix();
    glTranslatef(0.05, 0.6, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.55, 0.6, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.45, 0.6, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.95, 0.6, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.35, 0.6, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    //big clouds(first row)
    Cloud();
    glPushMatrix();
    glTranslatef(0.5, 0, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5, 0, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1, 0, 0);
    Cloud();
    glPopMatrix();
    Cloud();
    glPushMatrix();
    glTranslatef(-1.5, 0, 0);
    Cloud();
    glPopMatrix();
    //small clouds(2nd row)
    glPushMatrix();
    glTranslatef(0.05, 0.1, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.55, 0.1, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.45, 0.1, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.95, 0.1, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.35, 0.1, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    //big clouds(2nd row)
    glPushMatrix();
    glTranslatef(0.5, -0.5, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -0.5, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.5, -0.5, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1, -0.5, 0);
    Cloud();
    glPopMatrix();
    Cloud();
    glPushMatrix();
    glTranslatef(-1.5, -0.5, 0);
    Cloud();
    glPopMatrix();
    //part of wall
    glBegin(GL_QUADS);
    glColor3f(0.9, 0.9, 0.8);
    glVertex2f(-1, 0.05);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 0.05);
    glEnd();
    glLineWidth(15);
    glBegin(GL_LINES);
    glColor3f(0.85, 0.85, 0.75);
    glVertex2f(-1, 0.05);
    glVertex2f(1, 0.05);
    glEnd();
    //wooden floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor3f(0.4, 0.3, 0.1);
    glTexCoord2f(0.0, 0.0); glVertex2f(-1, -0.1);
    glTexCoord2f(0.0, 1.0); glVertex2f(-1, -1.1);
    glTexCoord2f(1.0, 1.0); glVertex2f(1, -1.1);
    glTexCoord2f(1.0, 0.0); glVertex2f(1, -0.1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void s1() {
    Room();
    bear();
    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(0.95, -0.6);
    glVertex2f(0.95, -1.0);
    glEnd();
    glColor3f(1, 1, 1);
    drawString(GLUT_BITMAP_HELVETICA_12, "Hello, I am Lotso, today I will tell you", -0.60f, -0.7f);
    drawString(GLUT_BITMAP_HELVETICA_12, "a story that happened to me someday.", -0.60f, -0.8f);
    drawString(GLUT_BITMAP_HELVETICA_12, " Click next to start the story. ", -0.60f, -0.9f);
    Button();
}
//---------------****************design s2 Narden*******************----------------------
void Door() {

    // Door frame
    glBegin(GL_QUADS);
    glColor3f(0.9, 0.9, 0.9);
    glVertex2f(-0.25, -0.1);
    glVertex2f(0.30, -0.1);
    glVertex2f(0.30, 0.82);
    glVertex2f(-0.25, 0.82);
    glEnd();
    //External view of the room
    glBegin(GL_QUADS);
    glColor3f(0, 0, 0);
    glVertex2f(-0.21, -0.1);
    glVertex2f(0.15, -0.1);
    glVertex2f(0.15, 0.78);
    glVertex2f(-0.21, 0.78);
    glEnd();
    glPushMatrix();
    // The Door
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef(doorAngle, 0.0, 1.0, 0.0);  // Rotate the door around the x-axis
   glBegin(GL_QUADS);
    glColor3f(0.7, 0.7, 0.7);
    glVertex2f(-0.36, -0.1);
    glVertex2f(0.16, -0.1);
    glVertex2f(0.16, 0.78);
    glVertex2f(-0.36, 0.78);
    glEnd();
    glPopMatrix();

    ////The door hole
    //glColor3f(0.9, 0.9, 0.9);
    //drawFilledCircle(0.1, 0.4, 0.03);
}
void s2() {
    Room();
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(-0.21, -0.1);
    glVertex2f(0.16, -0.1);
    glVertex2f(0.16, 0.0);
    glVertex2f(-0.21, 0.0);
    glEnd();
    Door();
    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(0.95, -0.6);
    glVertex2f(0.95, -1.0);
    glEnd();

    // Render Mr. Potato Head last
    glPushMatrix();
    glScalef(0.8, 0.8, 1);
    glRotatef(doorAngle, 0.0, 0.0, 0.0);  // Rotate the door around the z-axis
    glTranslatef(0.0, -0.3, 1);
    MrPootatoHead();
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //woody
    glPushMatrix();
    glRotatef(doorAngle, 0.0, 0.0, 0.0);  // Rotate the door around the z-axis
    glTranslatef(0.4, 0.2, 1);
    glScalef(0.8, 0.8, 1); 
    woody();
    glPopMatrix();
    //alien
    glPushMatrix();
    glRotatef(doorAngle, 0.0, 0.0, 0.0);  // Rotate the door around the z-axis
    glTranslatef(0.7, -0.4, 1);
    glScalef(0.44, 0.44, 1);
    alien();
    glPopMatrix();
    //jessy
    glPushMatrix();
    glRotatef(doorAngle, 0.0, 0.0, 0.0);  // Rotate the door around the z-axis
    glTranslatef(-0.7+ doorAngle/90, -0.4, 0);
    glScalef(0.6, 0.6, 1);
    Jessie();
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(0.95, -0.6);
    glVertex2f(0.95, -1.0);
    glEnd();
    glColor3f(1, 1, 1);
    drawString(GLUT_BITMAP_HELVETICA_12, "Once upon a time, I got lost in the neighbour's house", -0.90f, -0.7f);
    drawString(GLUT_BITMAP_HELVETICA_12, "and my friends: Woody, Jessie, Alien,and Mr.PotatoHead", -0.90f, -0.8f);
    drawString(GLUT_BITMAP_HELVETICA_12, " were looking for me.   click the next button: ", -0.90f, -0.9f);
    Button();
}
//---------------****************design s3 Aseel*******************----------------------
void redDevice() {
    //red device
    glBegin(GL_QUADS);
    glColor3f(0.7, 0, 0);
    glVertex2f(-0.35, 0.15);
    glVertex2f(-0.35, -0.34);
    glVertex2f(0.35, -0.34);
    glVertex2f(0.35, 0.15);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.9, 0.9, 0.9);
    glVertex2f(-0.27, 0.09);
    glVertex2f(-0.27, -0.22);
    glVertex2f(0.27, -0.22);
    glVertex2f(0.27, 0.09);
    glEnd();
    glColor3f(0.8, 0.7, 0.6);
    drawFilledCircle(0.28, -0.277, 0.042);
    drawFilledCircle(-0.28, -0.277, 0.042);
}
void Sketch()
{
    //window sketch
    //inner window frame
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex2f(-0.08, 0);
    glVertex2f(-0.08, -0.11);
    glVertex2f(0.08, -0.11);
    glVertex2f(0.08, 0);
    glEnd();
    //outer window frame
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex2f(-0.095, 0.015);
    glVertex2f(-0.095, -0.11);
    glVertex2f(0.095, -0.11);
    glVertex2f(0.095, 0.015);
    glEnd();
    //window sill
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex2f(-0.108, -0.11);
    glVertex2f(-0.108, -0.13);
    glVertex2f(0.108, -0.13);
    glVertex2f(0.108, -0.11);
    glEnd();
    //horizontal lines
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(-0.08, -0.03);
    glVertex2f(0.08, -0.03);
    glVertex2f(-0.08, -0.06);
    glVertex2f(0.08, -0.06);
    glEnd();
    //vertical lines
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(0.04, -0.06);
    glVertex2f(0.04, 0);
    glVertex2f(0, -0.06);
    glVertex2f(0, 0);
    glVertex2f(-0.04, -0.06);
    glVertex2f(-0.04, 0);
    glEnd();
    //rope
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex2f(-0.05, -0.11);
    glVertex2f(-0.07, -0.22);
    glEnd();
}
void clearScreen()
{
    glBegin(GL_QUADS);
    glColor3f(0.9, 0.9, 0.9);
    glVertex2f(-0.27, 0.09);
    glVertex2f(-0.27, -0.22);
    glVertex2f(0.27, -0.22);
    glVertex2f(0.27, 0.09);
    glEnd();
}
void s3() {
    Room();
    glPushMatrix();
    glTranslatef(0, 0.1, 0);
    redDevice();
    glPopMatrix();
    //mouse interaction with the red device
    //first if-else for mouse clicks
    if (deviceButton == 2) {
        //window sketch appears
        glPushMatrix();
        glTranslatef(0, 0.1, 0);
        clearScreen();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.02, 0.1, 0);
        Sketch();
        glPopMatrix();
        drawString(GLUT_BITMAP_TIMES_ROMAN_10, "Plan A", -0.22, 0.13);
    }
    else if (deviceButton == 1) {
        //message to user
        glPushMatrix();
        glTranslatef(0, 0.1, 0);
        clearScreen();
        glPopMatrix();
        glColor3f(0, 0, 0);
        drawString(GLUT_BITMAP_TIMES_ROMAN_10, "click next to climb ", -0.22, 0.13);
        drawString(GLUT_BITMAP_TIMES_ROMAN_10, " out the window", -0.22, 0.08);
        glBegin(GL_QUADS);
        glColor4f(0.5, 0.5, 0.5, 0.8);
        glVertex2f(0.6, -0.95);
        glVertex2f(0.6, -0.6);
        glVertex2f(0.95, -0.6);
        glEnd();
    }
    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(0.95, -0.6);
    glVertex2f(0.95, -1.0);
    glEnd();
    Button();
    glColor3f(1, 1, 1);
    drawString(GLUT_BITMAP_HELVETICA_12, "So,they decided to set a plan to figure out my place.", -0.90f, -0.7f);
    drawString(GLUT_BITMAP_HELVETICA_12, "click the right device button to reveal the plan. ", -0.90f, -0.80f);
    drawString(GLUT_BITMAP_HELVETICA_12, "click the left device button to erase the plan.", -0.90f, -0.90f);
    //characters
    glPushMatrix();
    glScalef(0.7, 0.7, 1);
    glTranslatef(-1.2, 0.5, 0);
    woody();
    glPopMatrix();
    MrPootatoHead();
    glPushMatrix();
    glScalef(0.6, 0.6, 1);
    glTranslatef(1.3, 0.5, 0);
    Jessie();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.4, 0.4, 1);
    glTranslatef(1.3, 0, 0);
    alien();
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}
//---------------------**********design s4 Noran************----------------------------
void Scanse()
{
    glBegin(GL_QUADS);  //window frame
    glColor4f(0.7f, 0.8f, 0.9f, 1.0f);
    glVertex2f(-0.9f, -0.3f);
    glVertex2f(0.7f, -0.3f);
    glVertex2f(0.7f, 0.9f);
    glVertex2f(-0.9f, 0.9f);
    glEnd();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);   //wall
    glColor3f(0.4, 0.6, 0.8);
    glVertex2f(-0.85f, -0.25f);
    glVertex2f(0.63f, -0.25f);
    glVertex2f(0.63f, 0.85f);
    glVertex2f(-0.85f, 0.85f);
    glEnd();

    glPushMatrix();
    glTranslatef(-0.17, -0.15, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.7, -0.15, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.2, -0.15, 0);
    Cloud();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, 0, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.7, 0, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.17, -0.54, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.7, -0.54, 0);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.2, -0.54, 0);
    Cloud();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1, -0.4, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.7, -0.4, 0);
    glScalef(0.5, 0.5, 1);
    Cloud();
    glPopMatrix();


    glBegin(GL_QUADS);   //habipl
    glColor4f(0.4f, 0.2f, 0.0f, 1.0f);
    glVertex2f(-0.2f, -0.25f);
    glVertex2f(-0.15f, -0.25f);
    glVertex2f(-0.15f, -1.0f);
    glVertex2f(-0.2f, -1.0f);
    glEnd();

    glBegin(GL_QUADS);   //window glass
    glColor4f(1, 1, 1, 0.7);
    glVertex2f(-0.85f, 0.3f);
    glVertex2f(0.63f, 0.3f);
    glVertex2f(0.63f, 0.85f);
    glVertex2f(-0.85f, 0.85f);
    glEnd();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.0, -0.2, 0.0);
    glColor3f(0.5, 0.5, 0.5);
    glRectf(-1.0, -0.3, 1.0, -0.8);
    glPopMatrix();

    glColor3f(1.0, 1.0, 1.0); // White color for the lines
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glVertex2f(-0.8, -0.7); // Left line segment
    glVertex2f(-0.6, -0.7);
    glVertex2f(-0.4, -0.7); // Left line segment
    glVertex2f(-0.2, -0.7);
    glVertex2f(0.0, -0.7); // Middle line segment
    glVertex2f(0.2, -0.7);
    glVertex2f(0.4, -0.7); // Right line segment
    glVertex2f(0.6, -0.7);
    glEnd();
}
void drawCarWithDetails() {
    // Car Body
    glColor3f(0.7, 0.0, 0.0); // Red color
    glBegin(GL_QUADS);
    glVertex2f(-0.6, 0.2);
    glVertex2f(-0.6, 0.0);
    glVertex2f(0.6, 0.0);
    glVertex2f(0.6, 0.2);
    glEnd();

    // Roof
    glBegin(GL_QUADS);
    glVertex2f(-0.3, 0.2);
    glVertex2f(-0.3, 0.4);
    glVertex2f(0.3, 0.4);
    glVertex2f(0.3, 0.2);
    glEnd();

    // Windows
    glColor3f(0.2, 0.4, 0.8); // Blue color
    glBegin(GL_QUADS);
    glVertex2f(-0.28, 0.21);
    glVertex2f(-0.28, 0.39);
    glVertex2f(0.28, 0.39);
    glVertex2f(0.28, 0.21);
    glEnd();

    // Wheels
    glColor3f(0.2, 0.2, 0.2); // Grey color
    int numSpokes = 5;
    for (int i = -1; i <= 1; i += 2) {
        float x = 0.4 * i;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, -0.05); // Center of the wheel
        for (int j = 0; j <= numSpokes; j++) {
            float angle = 2.0f * PI * j / numSpokes;
            float wheelX = x + 0.08 * cos(angle);
            float wheelY = -0.05 + 0.08 * sin(angle);
            glVertex2f(wheelX, wheelY);
        }
        glEnd();
    }

    // Headlights
    glColor3f(1.0, 1.0, 0.8); // Yellow color
    glBegin(GL_QUADS);
    glVertex2f(0.55, 0.14);
    glVertex2f(0.55, 0.10);
    glVertex2f(0.58, 0.10);
    glVertex2f(0.58, 0.14);
    glEnd();

    // Taillights
    glBegin(GL_QUADS);
    glVertex2f(-0.55, 0.14);
    glVertex2f(-0.55, 0.10);
    glVertex2f(-0.58, 0.10);
    glVertex2f(-0.58, 0.14);
    glEnd();
}
void s4() {
    Scanse();    // NoranS
    MrPootatoHead();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.48, -0.02, 0.0);
    glScalef(0.31, 0.31, 1.0);
    alien();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(transValuegx, transValuegy, 0.0);
    glScalef(0.4, 0.4, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);
    Jessie();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(transValuebx, transValueby, 0.0);
    glScalef(0.45, 0.45, 0.0);
    glPushMatrix();
    glTranslatef(-0.5, 0.3, 0.0);
    woody();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(carX, -0.7, 0);
    glScalef(0.6, 0.6, 1);
    drawCarWithDetails();
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(0.95, -0.6);
    glVertex2f(0.95, -1.0);
    glEnd();
    glColor3f(1, 1, 1);
    drawString(GLUT_BITMAP_HELVETICA_12, "Can you help my friends to get down from the window? ", -0.90f, -0.7f);
    drawString(GLUT_BITMAP_HELVETICA_12, "Move Jessie by using the arrow keys ", -0.90f, -0.8f);
    drawString(GLUT_BITMAP_HELVETICA_12, "Move Woody by using the keys: r= up,d=left,f=right,c=down", -0.90f, -0.9f);
    Button();
}
//---------------------**********design s5 Budur************----------------------------
void line(float a, float b, float c, float d) {
    glBegin(GL_LINES);
    glVertex2f(a, b);
    glVertex2f(c, d);
    glEnd();
}
void circle(float a, float b, float r) {

    int i;
    int triangleAmount = 100;
    GLfloat twicePi = 2.0f * PI;
    GLfloat x = a; GLfloat y = b; GLfloat radius = r;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            x + (radius * cos(i * twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }   glEnd();
}
void triangle(float a, float b, float c, float d, float e, float f) {
    glBegin(GL_TRIANGLES);
    glVertex2f(a, b);
    glVertex2f(c, d);
    glVertex2f(e, f);
    glEnd();
}
void sky() {
    glBegin(GL_QUADS);
    glVertex2f(-1, 0);
    glVertex2f(1, 0);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
}
void mountain() {
    //right
    glColor3f(0.3, 0.6, 0);  //dark
    triangle(0.55, 0, 1.1, 0, 0.8, 0.22);
    glColor3f(0.3, 0.4, 0.1);//light
    triangle(0.55, 0, 0.65, 0, 0.8, 0.22);
    glBegin(GL_TRIANGLES);
    glColor3f(0.3, 0.6, 0);  //dark
    //left 1
    glVertex2f(-1.1, 0);
    glVertex2f(-0.75, 0);
    glVertex2f(-0.9, 0.12);
    glColor3f(0.3, 0.4, 0.1);//light
    glVertex2f(-1.1, 0);
    glVertex2f(-0.98, 0);
    glVertex2f(-0.9, 0.12);
    //left2
    glColor3f(0.3, 0.6, 0);  //dark
    glVertex2f(-0.8, 0);
    glVertex2f(-0.3, 0);
    glVertex2f(-0.55, 0.15);
    glColor3f(0.3, 0.4, 0.1);//light
    glVertex2f(-0.8, 0);
    glVertex2f(-0.7, 0);
    glVertex2f(-0.55, 0.15);
    //left big
    glColor3f(0.3, 0.6, 0);  //dark
    glVertex2f(-0.45, 0);
    glVertex2f(0.1, 0);
    glVertex2f(-0.2, .23);
    glColor3f(0.3, 0.4, 0.1);//light
    glVertex2f(-0.45, 0);
    glVertex2f(-0.35, 0);
    glVertex2f(-0.2, .23);
    ////mid
    glColor3f(0.3, 0.6, 0);  //dark
    glVertex2f(-.1, 0);
    glVertex2f(.3, 0);
    glVertex2f(.1, .12);
    glColor3f(0.3, 0.4, 0.1);//light
    glVertex2f(-.1, 0);
    glVertex2f(0, 0);
    glVertex2f(.1, .12);
    glEnd();
}
void house() {
    // house
    glScalef(2.0, 2.0, 0);
    glTranslatef(-0.3, 0.1, 0);
    glBegin(GL_QUADS);
    glColor3f(0.831, 0.804, 0.804);
    glVertex2f(0.4, -0.1);
    glVertex2f(0.4, -0.3);
    glVertex2f(0.7, -0.3);
    glVertex2f(0.7, -0.1);
    glEnd();
    //door
    glBegin(GL_QUADS);
    glColor3f(1, 0.7, 0);
    glVertex2f(0.52, -0.2);
    glVertex2f(0.52, -0.3);
    glVertex2f(0.58, -0.3);
    glVertex2f(0.58, -0.2);
    glVertex2f(0.43, -0.21);
    glVertex2f(0.43, -0.26);
    glVertex2f(0.49, -0.26);
    glVertex2f(0.49, -0.21);
    glVertex2f(.61, -.21);
    glVertex2f(.61, -.26);
    glVertex2f(.67, -.26);
    glVertex2f(.67, -.21);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.169, 0.125);
    glVertex2f(0.39, -0.05);
    glVertex2f(0.37, -0.18);
    glVertex2f(0.73, -0.18);
    glVertex2f(0.71, -0.05);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.169, 0.125);
    glVertex2f(0.39, -0.29);
    glVertex2f(0.39, -0.31);
    glVertex2f(0.71, -0.31);
    glVertex2f(0.71, -0.29);
    glEnd();
    glLoadIdentity();
}
void river() {
    glLoadIdentity();
    glBegin(GL_QUADS);
    glColor3f(0.588, 0.788, 0.149);
    glVertex2f(-1, -.00);
    glVertex2f(1, -.00);
    glVertex2f(1, -.01);
    glVertex2f(-1, -.01);
    glColor3ub(43, 155, 207);
    glVertex2f(-1, -.01);
    glVertex2f(1, -.01);
    glVertex2f(1, -.1);
    glVertex2f(-1, -.1);
    glEnd();

    glColor3ub(109, 170, 199);
    glLineWidth(.05);
    line(-0.85, -.06, -0.95, -.06);
    line(-0.5, -.035, -.56, -.035);
    line(0.0, -.06, -0.1, -.06);
    line(0.8, -.03, 0.9, -.03);


}
void longGrass() {
    /////////// long grass
    glScalef(0.24, .2, 0);
    glTranslated(0.8, .49, 0);
    glColor3ub(68, 173, 47);
    triangle(-.5, -1.0, -.45, -1.0, -.35, -.65);
    triangle(-.5, -1.0, -.45, -1.0, -.6, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.35, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.0, -.65);
    triangle(-.45, -1.0, -.4, -1.0, -.2, -.65);
    triangle(.15, -1.0, .2, -1.0, .35, -.65);
    triangle(.15, -1.0, .2, -1.0, .05, -.65);
    triangle(.45, -1.0, .5, -1.0, .35, -.65);
    triangle(.55, -1.0, .6, -1.0, .55, -.65);
    triangle(.6, -1.0, .65, -1.0, .8, -.65);
    triangle(.85, -1.0, .9, -1.0, 1.0, -.65);
    glLoadIdentity();
    glScalef(0.25, .2, 0);
    glTranslated(0.0, .49, 0);
    glColor3ub(68, 173, 47);
    triangle(-.5, -1.0, -.45, -1.0, -.35, -.65);
    triangle(-.5, -1.0, -.45, -1.0, -.6, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.35, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.0, -.65);
    triangle(-.45, -1.0, -.4, -1.0, -.2, -.65);
    triangle(.15, -1.0, .2, -1.0, .35, -.65);
    triangle(.15, -1.0, .2, -1.0, .05, -.65);
    triangle(.45, -1.0, .5, -1.0, .35, -.65);
    triangle(.55, -1.0, .6, -1.0, .55, -.65);
    triangle(.6, -1.0, .65, -1.0, .8, -.65);
    triangle(.85, -1.0, .9, -1.0, 1.0, -.65);
    glLoadIdentity();
    glScalef(0.25, .2, 0);
    glTranslated(-2.5, .49, 0);
    glColor3ub(68, 173, 47);
    triangle(-.5, -1.0, -.45, -1.0, -.35, -.65);
    triangle(-.5, -1.0, -.45, -1.0, -.6, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.35, -.65);
    triangle(-.2, -1.0, -.15, -1.0, -.0, -.65);
    triangle(-.45, -1.0, -.4, -1.0, -.2, -.65);
    triangle(.15, -1.0, .2, -1.0, .35, -.65);
    triangle(.15, -1.0, .2, -1.0, .05, -.65);
    triangle(.45, -1.0, .5, -1.0, .35, -.65);
    triangle(.55, -1.0, .6, -1.0, .55, -.65);
    triangle(.6, -1.0, .65, -1.0, .8, -.65);
    triangle(.85, -1.0, .9, -1.0, 1.0, -.65);
    glLoadIdentity();
}
void boat() {
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(0.5, 0.2, 0.0);
    glVertex2f(-0.01, -.04); glVertex2f(-0.01, 0.3);
    glVertex2f(-0.1, -.04); glVertex2f(-0.01, 0.28);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.8, 0.0, 0.0);
    glVertex2f(-0.01, 0.28);
    glVertex2f(-0.01, -0.0);
    glVertex2f(0.14, -0.0);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.5, 0.2, 0.0);
    glVertex2f(-0.12, -0.03);
    glVertex2f(-0.1, -0.06);
    glVertex2f(0.1, -0.06);
    glVertex2f(0.12, -0.03);
    glEnd();

}
void ground() {
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 0);
    glVertex2f(-1, 0);
    glEnd();
}
void field() {
    triangle(-0.5, -0.2, 1, -0.4, 1, -0.1);
    triangle(-1, -0.4, -1, -0.7, 0, -0.5);
}
void s5() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
    glEnable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ///////sky
    glColor3f(0.4, 0.6, 0.8);
    sky();
    /////mountain
    mountain();
    ///////sun
    glColor3f(1, 0.6, 0);
    circle(0.0, .75, .09);
    ////sun cloud
    glColor3f(1, 1, 1);
    circle(-.08, .7, .04);
    circle(-.00, .71, .07);
    circle(0.09, .71, .05);
    /////////ground
    glColor3f(0.5, 0.8, 0.1);
    ground();
    //field
    glColor3f(0.5, 0.7, 0.2);
    field();
    glColor3f(0.5, 0.7, 0.9);
    river();

    glPushMatrix();
    glTranslatef(boatX, 0.0f, 0.0f);
    boat();
    glPopMatrix();
    longGrass();
    
    glPushMatrix();
    glScalef(1.5, 1.5, 1);
    house();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(transValuegx, transValuegy, 0.0);
    glScalef(0.4, 0.4, 1.0);
    glPushMatrix();
    glTranslatef(-1, -0.9, 0.0);
    Jessie();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(transValuebx, transValueby, 0.0);
    glScalef(0.45, 0.45, 0.0);
    glPushMatrix();
    glTranslatef(-1, -0.9, 0.0);
    woody();
    glPopMatrix();
    glPopMatrix();

    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(0.95, -0.6);
    glVertex2f(0.95, -1.0);
    glEnd();
    glColor3f(1, 1, 1);
    drawString(GLUT_BITMAP_HELVETICA_12, "They were going to the neighbour's house to look for me.", -0.90f, -0.7f);
    drawString(GLUT_BITMAP_HELVETICA_12, "Can you help them to knock on the door to get inside?  ", -0.90f, -0.78f);
    drawString(GLUT_BITMAP_HELVETICA_12, "Move Jessie by using the arrow keys.", -0.90f, -0.86f);
    drawString(GLUT_BITMAP_HELVETICA_12, "Move Woody by using the keys: r= up,d=left,f=right,c=down", -0.90f, -0.94f);
    Button();
}
//---------------------**********design s6 Teif************----------------------------
void drawStar(float x, float y, float size) {
    // Vertical line of the star
    glBegin(GL_LINES);
    glVertex2f(x, y - size);
    glVertex2f(x, y + size);
    glEnd();
    // Horizontal line of the star
    glBegin(GL_LINES);
    glVertex2f(x - size, y);
    glVertex2f(x + size, y);
    glEnd();
    // Diagonal lines
    float offset = size * 0.7f;  // Adjust this value for different star looks
    glBegin(GL_LINES);
    glVertex2f(x - offset, y - offset);
    glVertex2f(x + offset, y + offset);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x - offset, y + offset);
    glVertex2f(x + offset, y - offset);
    glEnd();
}
void Background()
{
    glBegin(GL_QUADS);
    glColor3f(0.01f, 0.1f, 0.3f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    glBegin(GL_QUADS);
    glColor3f(0.4, 0.3, 0.1);
    glTexCoord2f(0.0, 0.0); glVertex2f(-1, 0);
    glTexCoord2f(0.0, 1.0); glVertex2f(-1, -1);
    glTexCoord2f(1.0, 1.0); glVertex2f(1, -1);
    glTexCoord2f(1.0, 0.0); glVertex2f(1, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void s6() {
    Background();
    //jessie 
    glPushMatrix();
    glScalef(0.5, 0.5, 1.0);
    Jessie();
    glPopMatrix();
    //woody
    glPushMatrix();
    glScalef(0.5, 0.5, 1.0);
    glTranslatef(0.9f, 0.0f, 0.0f);
    woody();
    glPopMatrix();
    //bear
    glPushMatrix();
    glScalef(0.5, 0.5, 1.0);
    glTranslatef(-0.9f, 0.0f, 0.0f);
    bear();
    glPopMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    // Drawing a few stars
    glLineWidth(0.7);
    drawStar(-0.8 + starX, 0.4f, 0.1f);
    drawStar(-0.3f + starX, 0.7f, 0.1f);
    drawStar(0.0f + starX, 0.4f, 0.1f);
    drawStar(0.3f + starX, 0.8f, 0.1f);
    drawStar(0.7f + starX, 0.5f, 0.1f);
    drawStar(-0.9f + starX, 0.8f, 0.1f);
    drawString(GLUT_BITMAP_TIMES_ROMAN_24, "THE END ",- 0.22, 0.30);
    glBegin(GL_QUADS);
    glColor4f(0.2, 0.2, 0.2, 0.5);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, -0.6);
    glVertex2f(1, -0.6);
    glVertex2f(1, -1.0);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    drawString(GLUT_BITMAP_HELVETICA_12, "In the end, my friends didn't give up on looking for me", -0.65f, -0.7f);
    drawString(GLUT_BITMAP_HELVETICA_12, "until they found me cause friendship bonds  ", -0.50f, -0.78f);
    drawString(GLUT_BITMAP_HELVETICA_12, "are stronger than any obstacles they faced. ", -0.50f, -0.86f);
}
//-----------------*************intractive******************------------------
void mouseClick(int button, int state, int x, int y) {
    // Convert mouse coordinates to window coordinates
    float mouseX = (static_cast<float>(x) / win_width) * 2 - 1;
    float mouseY = 1 - (static_cast<float>(y) / win_hight) * 2;
    if (scene == 0) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            // Check if the click is inside the gray rectangle
            if (mouseX >= -0.4f && mouseX <= 0.4f && mouseY >= -1.0f && mouseY <= -0.8f) {
                scene = 1;  // Move from scene 0 to 1
            }
        }
    }
    else if (scene == 1) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (mouseX >= 0.6f && mouseX <= 0.95f && mouseY >= -0.95f && mouseY <= -0.6f) {
                // Click is inside the "NEXT" button
                scene = 2;  // Move from scene 1 to 2
            }
        }
    }
    else if (scene == 2) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (mouseX >= 0.6f && mouseX <= 0.95f && mouseY >= -0.95f && mouseY <= -0.6f) {
                // Click is inside the "NEXT" button
                scene = 3;  // Move from scene 2 to 3
            }
        }
    }
    else if (scene == 3) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (mouseX >= 0.6f && mouseX <= 0.95f && mouseY >= -0.95f && mouseY <= -0.6f) {
                // Click is inside the "NEXT" button
                scene = 4;  // Move from scene 3 to 4
            }
        }
        if (button == GLUT_LEFT_BUTTON && state == GLUT_MIDDLE_BUTTON) {
            cout << "x: " << x << endl;
            cout << "y: " << y << endl;
            if (x >= 312 && x <= 328 && y >= 285 && y <= 305) {
                deviceButton = 1;
            }
            else if (x >= 170 && x <= 190 && y >= 285 && y <= 305) {
                deviceButton = 0;
            }
        }
        else if (button == GLUT_RIGHT_BUTTON && state == GLUT_MIDDLE_BUTTON) {
            if (x >= 312 && x <= 328 && y >= 285 && y <= 305) {
                deviceButton = 2;
            }
            else if (x >= 170 and x <= 190 and y >= 285 and y <= 305) {
                deviceButton = 0;
            }
        }
    }
    else if (scene == 3) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (mouseX >= 0.6f && mouseX <= 0.95f && mouseY >= -0.95f && mouseY <= -0.6f) {
                // Click is inside the "NEXT" button
                // Change the scene to another scene, e.g., scene = 3;
                scene = 4;
            }
        }
    }
    else if (scene == 4) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (mouseX >= 0.6f && mouseX <= 0.95f && mouseY >= -0.95f && mouseY <= -0.6f) {
                // Click is inside the "NEXT" button
                // Change the scene to another scene, e.g., scene = 3;
                scene = 5;
            }
        }
    }
    else if (scene == 5) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (mouseX >= 0.6f && mouseX <= 0.95f && mouseY >= -0.95f && mouseY <= -0.6f) {
                // Click is inside the "NEXT" button
                // Change the scene to another scene, e.g., scene = 3;
                scene = 6;
            }
        }
    }
    glutPostRedisplay(); // Request a window redisplay
}
//this function is called when keyboard button is pressed 
void KeyPress(unsigned char key, int x, int y) {
    switch (key) {
    case 'r':
        transValueby+= 0.05;
        break;
    case 'c':
        transValueby-= 0.05;
        break;
    case 'f':
        transValuebx += 0.05;
        break;
    case 'd':
        transValuebx -= 0.05;
        break;
    }
    glutPostRedisplay();
}
static void special(int key, int x, int y) {
    //handle special keys
    switch (key) {
    case GLUT_KEY_UP:
        transValuegy += 0.05;
        break;
    case GLUT_KEY_DOWN:
        transValuegy -= 0.05;
        break;
    case GLUT_KEY_LEFT:
        transValuegx -= 0.05;
        break;
    case GLUT_KEY_RIGHT:
        transValuegx += 0.05;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void idle() {
    if (scene == 0) {
        cloudX += velocity_x;
        if (cloudX > 0.8) {
            cloudX = -1.0;
        }
        glutPostRedisplay(); // Request a window redisplay
    }
    if (scene == 2) {  // Open the door
        // Adjust the door angle based on the time or any other factor
        doorAngle += doorRotationSpeed;
        // If the door is fully open or closed, reverse the rotation direction
        if (doorAngle >= 90.0 || doorAngle <= 0.0) {
            doorRotationSpeed = -doorRotationSpeed;
        }
        // Adjust the door angle based on the time or any other factor
        charAngle += charRotationSpeed;
        // If the door is fully open or closed, reverse the rotation direction
        if (charAngle >= 90.0 || charAngle <= 0.0) {
            charRotationSpeed = -charRotationSpeed;
        }
        glutPostRedisplay(); // Request a window redisplay
    }
    if (scene == 4) {
        carX += carspeed_x;
        if (carX > 0.8) {
            carX = -1.0;
        }
        glutPostRedisplay(); // Request a window redisplay
    }
    if (scene == 5) {
        boatX += velocity_b;
        if (boatX > 1.0) {
            boatX = -1.0; // Reset boat's position
        }
        glutPostRedisplay(); // Request a window redisplay
    }
    if (scene == 6) {
        starX += velocityx;
        if (starX > 0.8) {
            starX = -1.0;
        }
        glutPostRedisplay(); // Request a window redisplay
    }
}
//----------*******************implement part******************--------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (scene == 0) {
        s0();
    }
    else if (scene==1) {
        s1();
    }
    else if (scene == 2) {
        s2();
    }
    else if (scene == 3) {
       s3();
    }
    else if (scene == 4) {
        s4();
    }
    else if (scene == 5) {
        s5();
    }
    else if (scene == 6) {
        s6();
    }
    glFlush();
    glutSwapBuffers();
}
void init()
{
    glClearColor(0.4f, 0.8f, 1.0f, 0.6f); // Set background color to White and opaque
    glMatrixMode(GL_PROJECTION);          // set the projection parameters
    glMatrixMode(GL_MODELVIEW);
    //------- Texture ---------
    myTexture1 = LoadTexture(image1Path, 600, 600);
    if (myTexture1 == -1)
    {
        cout << "Error in loading the texture" << endl;
    }
    else
        cout << "The texture value is: " << myTexture1 << endl;
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_hight);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("project CG");
    init();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeFun);
    glutKeyboardFunc(KeyPress);
    glutMouseFunc(mouseClick);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
