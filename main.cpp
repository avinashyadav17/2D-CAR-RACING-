#include <windows.h>  // To use Windows-specific functionality for developing applications on the Windows platform.
#ifdef __APPLE__  // Check if the program is being compiled on an Apple (macOS) system.
#include <GLUT/glut.h>  // If on macOS, include the GLUT header for macOS.
#else  // If not on macOS (presumably Windows or Linux).
#include <GL/glut.h>  // Include the GLUT header for Windows/Linux for OpenGL utilities.
#endif
#include <iostream>  // Includes the standard I/O stream library to use input/output functions like std::cout.
#include <cstdio>    // For using functions like sprintf for formatted output to strings.
#include <cmath>     // To include mathematical functions like sin, cos, sqrt, etc.
 // For abs
 
int FPS = 30;  // The speed at which the game refreshes, controls game speed.
int start = 0;  // Indicates whether the game has started or not.
int gv = 0;     // Stores the game over status: 0 = not over, 1 = game over.
int level = 0;  // Stores the current level of the game.

int score = 0;  // Keeps track of the player's score.

int roadDivTopMost = 0, roadDivTop = 0, roadDivMd1 = 0, roadDivBtm = 0;  // Variables for road divider positions.
int lrIndex = 0;  // Represents the left-right position of the car.

int car1 = 0, lrIndex1 = 0;  // Variables for the first incoming car's position.
int car2 = 35, lrIndex2 = 0;  // Variables for the second incoming car's position.
int car3 = 70, lrIndex3 = 0;  // Variables for the third incoming car's position.

const void* font1 = GLUT_BITMAP_TIMES_ROMAN_24;  // Font for displaying large text (e.g., game over).
const void* font2 = GLUT_BITMAP_HELVETICA_18;    // Font for displaying medium text (e.g., instructions).
const void* font3 = GLUT_BITMAP_8_BY_13;         // Font for displaying small text (e.g., score, speed).

// Function to render text on the screen at a specified location (x, y) using the selected font.
void renderBitmapString(float x, float y, const void* font, const char* string) {
    const char* c;  // Pointer to the current character in the string.
    glRasterPos2f(x, y);  // Set the position for the text on the screen.
    for (c = string; *c != '\0'; c++) {  // Loop through each character of the string.
        glutBitmapCharacter((void*)font, *c);  // Render the current character.
    }
}

// Function to draw a tree at position (x, y).
void tree(int x, int y) {
    int newx = x, newy = y;  // Local variables for the tree's position.

    // Draw the tree trunk using a brown color.
    glColor3f(0.871, 0.722, 0.529);  
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55);  // First vertex of the trunk.
    glVertex2f(newx + 12, newy + 55 - 10);  // Second vertex of the trunk.
    glVertex2f(newx + 10, newy + 55 - 10);  // Third vertex of the trunk.
    glEnd();

    // Draw the tree leaves using a green color.
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx + 11, newy + 55 + 3);  // First vertex of the leaves.
    glVertex2f(newx + 12 + 3, newy + 55 - 3);  // Second vertex of the leaves.
    glVertex2f(newx + 10 - 3, newy + 55 - 3);  // Third vertex of the leaves.
    glEnd();
}

// Function to handle the game logic (drawing the road, car, and updating score and level).
void startGame() {
    // Draw the road background using gray color.
    glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);  // Left-bottom corner of the road.
    glVertex2f(20, 100);  // Left-top corner of the road.
    glVertex2f(80, 100);  // Right-top corner of the road.
    glVertex2f(80, 0);    // Right-bottom corner of the road.
    glEnd();

    // Draw the left border of the road using white color.
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(20, 0);   // Left-bottom corner.
    glVertex2f(20, 100);  // Left-top corner.
    glVertex2f(23, 100);  // Inner left-top corner.
    glVertex2f(23, 0);    // Inner left-bottom corner.
    glEnd();

    // Draw the right border of the road using white color.
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(77, 0);   // Right-bottom corner.
    glVertex2f(77, 100);  // Right-top corner.
    glVertex2f(80, 100);  // Outer right-top corner.
    glVertex2f(80, 0);    // Outer right-bottom corner.
    glEnd();

    // Draw the middle road divider (top part) using yellow color.
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivTop + 80);   // Left side of the top divider.
    glVertex2f(48, roadDivTop + 100);  // Right side of the top divider.
    glVertex2f(52, roadDivTop + 100);  // Right side of the top divider.
    glVertex2f(52, roadDivTop + 80);   // Left side of the top divider.
    glEnd();
    roadDivTop--;  // Move the divider downwards.
    if (roadDivTop < -100) {  // If the divider moves off-screen, reset it to the top.
        roadDivTop = 20;
        score++;  // Increase the score for crossing the divider.
    }

    // Similar logic applies to the middle and bottom road dividers:
    // Draw the middle road divider and update its position.
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivMd1 + 40);  // Left side of the middle divider.
    glVertex2f(48, roadDivMd1 + 60);  // Right side of the middle divider.
    glVertex2f(52, roadDivMd1 + 60);  // Right side of the middle divider.
    glVertex2f(52, roadDivMd1 + 40);  // Left side of the middle divider.
    glEnd();
    roadDivMd1--;
    if (roadDivMd1 < -60) {  // Reset when off-screen.
        roadDivMd1 = 60;
        score++;
    }

    // Similar logic applies to the bottom road divider:
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48, roadDivBtm + 0);    // Left side of the bottom divider.
    glVertex2f(48, roadDivBtm + 20);   // Right side of the bottom divider.
    glVertex2f(52, roadDivBtm + 20);   // Right side of the bottom divider.
    glVertex2f(52, roadDivBtm + 0);    // Left side of the bottom divider.
    glEnd();
    roadDivBtm--;
    if (roadDivBtm < -20) {  // Reset when off-screen.
        roadDivBtm = 100;
        score++;
    }

    // Draw the score board:
    glColor3f(0.000, 0.000, 0.000);  // Black background for the score.
    glBegin(GL_POLYGON);
    glVertex2f(80, 97);   // Upper left corner.
    glVertex2f(100, 97);  // Upper right corner.
    glVertex2f(100, 90);  // Lower right corner.
    glVertex2f(80, 90);   // Lower left corner.
    glEnd();

    // Display the current score:
    char buffer[50];
    sprintf(buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000);  // Green color for the score.
    renderBitmapString(83, 92, font1, buffer);

    // Display the current level (adjust based on score thresholds):
    if (score >= 5) {
        level = 1;  // Level 1 starts at score 5.
    }
    if (score >= 10) {
        level = 2;  // Level 2 starts at score 10.
    }
    char buffer2[50];
    sprintf(buffer2, "LEVEL: %d", level);
    glColor3f(0.000, 1.000, 0.000);  // Green color for the level.
    renderBitmapString(83, 95, font1, buffer2);

    // Increment FPS for speed control.
    FPS += 2;
}


    // Display level
    char level_buffer[50];
    sprintf(level_buffer, "LEVEL: %d", level);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(80.5, 89, font3, level_buffer);

    // Main car
    // Front
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 5);
    glVertex2f(lrIndex + 26 - 2, 7);
    glVertex2f(lrIndex + 30 + 2, 7);
    glVertex2f(lrIndex + 30 + 2, 5);
    glEnd();

    // Back
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26 - 2, 1);
    glVertex2f(lrIndex + 26 - 2, 3);
    glVertex2f(lrIndex + 30 + 2, 3);
    glVertex2f(lrIndex + 30 + 2, 1);
    glEnd();

    // Car body
    glColor3f(0.678, 1.000, 0.184);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex + 26, 1);
    glVertex2f(lrIndex + 26, 8);
    glVertex2f(lrIndex + 28, 10);
    glVertex2f(lrIndex + 30, 8);
    glVertex2f(lrIndex + 30, 1);
    glEnd();

    // Incoming car 1
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 4);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 6);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 4);
    glEnd();

    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100);
    glVertex2f(lrIndex1 + 26 - 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100 - 2);
    glVertex2f(lrIndex1 + 30 + 2, car1 + 100);
    glEnd();

    glColor3f(1.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex1 + 26, car1 + 100);
    glVertex2f(lrIndex1 + 26, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 28, car1 + 100 - 9);
    glVertex2f(lrIndex1 + 30, car1 + 100 - 7);
    glVertex2f(lrIndex1 + 30, car1 + 100);
    glEnd();
    car1--;
    if (car1 < -100) {
        car1 = 0;
        lrIndex1 = lrIndex;
    }

    // Check collision with car 1
    if ((abs(lrIndex - lrIndex1)) < 8 && (car1 + 100 < 10)) {
        start = 0;
        gv = 1;
    }

    // Incoming car 2
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 4);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 6);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 4);
    glEnd();

    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100);
    glVertex2f(lrIndex2 + 26 - 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100 - 2);
    glVertex2f(lrIndex2 + 30 + 2, car2 + 100);
    glEnd();

    glColor3f(0.294, 0.000, 0.510);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex2 + 26, car2 + 100);
    glVertex2f(lrIndex2 + 26, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 28, car2 + 100 - 9);
    glVertex2f(lrIndex2 + 30, car2 + 100 - 7);
    glVertex2f(lrIndex2 + 30, car2 + 100);
    glEnd();
    car2--;
    if (car2 < -100) {
        car2 = 0;
        lrIndex2 = lrIndex;
    }

    // Check collision with car 2
    if ((abs(lrIndex - lrIndex2)) < 8 && (car2 + 100 < 10)) {
        start = 0;
        gv = 1;
    }

    // Incoming car 3
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 4);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 6);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 4);
    glEnd();

    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100);
    glVertex2f(lrIndex3 + 26 - 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100 - 2);
    glVertex2f(lrIndex3 + 30 + 2, car3 + 100);
    glEnd();

    glColor3f(1.000, 0.271, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndex3 + 26, car3 + 100);
    glVertex2f(lrIndex3 + 26, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 28, car3 + 100 - 9);
    glVertex2f(lrIndex3 + 30, car3 + 100 - 7);
    glVertex2f(lrIndex3 + 30, car3 + 100);
    glEnd();
    car3--;
    if (car3 < -100) {
        car3 = 0;
        lrIndex3 = lrIndex;
    }

    // Check collision with car 3
    if ((abs(lrIndex - lrIndex3)) < 8 && (car3 + 100 < 10)) {
        start = 0;
        gv = 1;
    }
}

// Function to draw the initial game screen
void firstDesign() {
    // Background
    glColor3f(0.000, 0.392, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glColor3f(0.604, 0.804, 0.196);
    glVertex2f(100, 0);
    glVertex2f(0, 0);
    glEnd();

    // Road sky
    glColor3f(0.000, 0.749, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(100, 100);
    glVertex2f(0, 100);
    glColor3f(0.686, 0.933, 0.933);
    glVertex2f(0, 55);
    glVertex2f(100, 55);
    glEnd();

    // Trees
    tree(5, -15);
    tree(9, 5);
    tree(95, 9);
    tree(75, -5);

    // Game over screen
    if (gv == 1) {
        glColor3f(1.000, 0.000, 0.000);
        renderBitmapString(35, 60, font1, "GAME OVER");
        char buffer2[50];
        sprintf(buffer2, "Your score is: %d", score);
        renderBitmapString(33, 55, font1, buffer2);
    }

    // Game title and instructions
    glColor3f(1.000, 1.000, 0.000);
    renderBitmapString(30, 80, font1, "2D Car Racing Game");

    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(30, 50, font2, "Press SPACE to START");
    renderBitmapString(30, 45, font2, "Press ESC to Exit");

    glColor3f(1.000, 1.000, 1.000);
    renderBitmapString(30, 40, font3, "Press UP to increase speed");
    renderBitmapString(30, 37, font3, "Press DOWN to decrease speed");
    renderBitmapString(30, 34, font3, "Press RIGHT to turn right");
    renderBitmapString(30, 31, font3, "Press LEFT to turn left");
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (start == 1) {
        glClearColor(0.000, 0.392, 0.000, 1);
        startGame();
    } else {
        firstDesign();
    }

    glFlush();
    glutSwapBuffers();
}

// Special key input handling
void spe_key(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            if (FPS > (50 + (level * 2))) FPS -= 2;
            break;
        case GLUT_KEY_UP:
            FPS += 2;
            break;
        case GLUT_KEY_LEFT:
            if (lrIndex >= 0) {
                lrIndex -= (FPS / 10);
                if (lrIndex < 0) lrIndex = 0;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (lrIndex <= 44) {
                lrIndex += (FPS / 10);
                if (lrIndex > 44) lrIndex = 44;
            }
            break;
        default:
            break;
    }
}

// Regular key input handling
void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            if (start == 0) {
                start = 1;
                gv = 0;
                FPS = 50;
                roadDivTopMost = 0;
                roadDivTop = 0;
                roadDivMd1 = 0;
                roadDivBtm = 0;
                lrIndex = 0;
                car1 = 0;
                lrIndex1 = 0;
                car2 = 35;
                lrIndex2 = 0;
                car3 = 70;
                lrIndex3 = 0;
                score = 0;
                level = 0;
            }
            break;
        case 27: // ESC key 27 is the ASCII code for the ESC (Escape) key.
            exit(0);
            break;
        default:
            break;
    }
}

// Timer function for game updates
void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

// Main function
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 650);
    glutInitWindowPosition(200, 20);
    glutCreateWindow("Car Racing Game");

    glutDisplayFunc(display);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(processKeys);

    glOrtho(0, 100, 0, 100, -1, 1);
    glClearColor(0.184, 0.310, 0.310, 1);

    glutTimerFunc(1000, timer, 0);
    glutMainLoop();

    return 0;
}
