#include "gui.h"

int pixelGrid[64][32]; // Represents the Chip-8 screen, 64x32 pixels
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

    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            if (pixelGrid[x][y] == 1) {
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

	/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, -1, 1); // Set up an orthographic projection
    glMatrixMode(GL_MODELVIEW);
	*/
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
	glutInitDisplayMode(GLUT_DOUBLE);	// Use double display buffer.
	glutInitWindowSize(640, 320);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CHIP-8 Uddivert");
	for (int i = 0; i < 64; i ++) {
		for (int j = 0; j < 32; j ++) {
			pixelGrid[i][j] = 1;
		} // for
	} // for 
	init();
	glutDisplayFunc(display);
	glutMainLoop();

} // guiInit

// TODO
// Timer function that updates at every clock pulse.
// pixel array that holds pixels need to be drawn`
// Draw pixels as quads