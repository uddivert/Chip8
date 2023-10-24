#include <GL/glut.h>
#include <stdio.h>
#include <pthread.h>

void display1() {
    // Your OpenGL rendering code goes here
    glClear(GL_COLOR_BUFFER_BIT);
    // Add your drawing commands here
    glutSwapBuffers();
}

int main1(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GLuUT Thread Example");

    // Initialize OpenGL settings
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Register display callback
    glutDisplayFunc(display1);

    // Enter the GLUT main loop
    glutMainLoop();

    // This point will not be reached because glutMainLoop never returns
    return 0;
}
