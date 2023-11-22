#include <math.h>
#include "gui.h"

unsigned char keys[16] = {
    '1', '2', '3', '4',
    'q', 'w', 'e', 'r',
    'a', 's', 'd', 'f',
    'z', 'x', 'c', 'v'
};
int keyPress[16] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

int pixelGrid[32][64]; // Represents the Chip-8 screen, 64x32 pixels
int counter = 0;
int _Atomic guiFlag = 0; // can be modified by two threads simultaneously
/**
 * @brief Displays the gui
 * 
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the drawing color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Define the size of each pixel square
    float pixelSizeX = 2.0f / 64; // Width of each pixel
    float pixelSizeY = 2.0f / 32; // Height of each pixel

    glBegin(GL_QUADS); // Begin drawing quads

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (pixelGrid[y][x] == 1) {
                // Pixel is lit, draw a white rectangle
                glVertex2f(-1.0f + x * pixelSizeX, 1.0f - y * pixelSizeY); // Top-left corner
                glVertex2f(-1.0f + (x + 1) * pixelSizeX, 1.0f - y * pixelSizeY); // Top-right corner
                glVertex2f(-1.0f + (x + 1) * pixelSizeX, 1.0f - (y + 1) * pixelSizeY); // Bottom-right corner
                glVertex2f(-1.0f + x * pixelSizeX, 1.0f - (y + 1) * pixelSizeY); // Bottom-left corner
            }
        }
    }

    glEnd(); // End drawing quads

    glFlush(); // Flush the rendering pipeline
    glutSwapBuffers();
}

// Initialize OpenGL
void init(void) 
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the clear color to black
    //set displayto be empty
    for (int i = 0; i < 32; i ++) {
        for (int j = 0; j < 64; j ++) {
            pixelGrid[i][j] = 0;
        } // for
    } // for 
}

// fills screen each instruction temporary
void screenFill(uint8_t grid[][64]) {
    for (int i = 0; i < 32; i ++) {
        for (int j = 0; j < 64; j ++) {
            pixelGrid[i][j] = grid[i][j];
        } // for
    } // for 
    counter ++;
}

/**
 * @brief Sets keyPress to 1 if they key is pressed
 * 
 * @param key Key Being pressed
 * @param x unused
 * @param y unused
 */
void keyBoard(unsigned char key, int x, int y)
{
    for (int i = 0; i < 16; i ++) {
        if (key == keys[i]) {
            printExtra("%c", key);
            keyPress[i] = 1;
        }
    }
}

/**
 * @brief Sets keyPress to 0 if key is no longer being pressed
 * 
 * @param key Key being pressed
 * @param x 
 * @param y 
 */
void keyBoardUp(unsigned char key, int x, int y) {
    for (int i = 0; i < 16; i ++) {
        if (key == keys[i]) {
            keyPress[i] = 0;
        }
    } 
}

void closeCallback(void) {
    quitDeb();
    glutLeaveMainLoop();
}

/**
 * @brief Handles all the startup of the gui
 * 
 * @param argc argc from shell
 * @param argv argv from shell
 */
void guiInit(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	// Use double display buffer.
    glutInitWindowSize(640, 320);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CHIP-8 Uddivert");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); // continue program if glut closes
    init();
    guiFlag = 1;
    glutDisplayFunc(display);
    glutKeyboardFunc(keyBoard);
    glutKeyboardUpFunc(keyBoardUp);
    glutCloseFunc(closeCallback);
    glutIdleFunc(display);
    glutMainLoop();
} // guiInit