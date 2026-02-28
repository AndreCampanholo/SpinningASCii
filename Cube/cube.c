#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

float calculateX(float i, float j, float k, float A, float B, float C) {
    return j*cos(C)*sin(B)*sin(A) - k*cos(C)*sin(B)*cos(A) + j*sin(C)*cos(A) + k*sin(C)*sin(A) + i*cos(C)*cos(B);
}

float calculateY(float i, float j, float k, float A, float B, float C) {
    return j*cos(C)*cos(A) + k*cos(C)*sin(A) - j*sin(C)*sin(B)*sin(A) + k*sin(C)*sin(B)*cos(A) - i*sin(C)*cos(B);
}

float calculateZ(float i, float j, float k, float A, float B, float C) {
    return k*cos(B)*cos(A) - j*cos(B)*sin(A) + i*sin(B);
}

float calculateForPoint(float i, float j, float k, char symbol, float *x, float *y, float *z, float distanceFromCam, 
    float A, float B, float C, int *xp, int *yp, int screenWidth, int screenHeight, float k1, float *zbuffer, char *buffer) {
    *x = calculateX(i, j, k, A, B, C);
    *y = calculateY(i, j, k, A, B, C);
    *z = calculateZ(i, j, k, A, B, C) + distanceFromCam;

    *xp = (int)(screenWidth / 2 + *x * k1 / *z * 2);
    *yp = (int)(screenHeight / 2 + *y * k1 / *z);

    int idx = (*xp) + (*yp) *screenWidth;
    if(idx >= 0 && idx < screenWidth * screenHeight) {
        if(1 / *z > zbuffer[idx]) {
            zbuffer[idx] = 1 / *z;
            buffer[idx] = symbol;
        }
    }
}

int main() {
    float A = 0, B = 0, C = 0; //angles
    const int cubeWidth = 15; 
    const int screenWidth = 40, screenHeight = 40;

    int bufferLength = screenHeight * screenWidth; //buffer stores the amount pixels/char_placeholders according to screen size

    float zBuffer[bufferLength]; //stores z value of each point of the cube
    char buffer[bufferLength]; //stores which character should be printed on the screen for each point

    const char backgroundChar = ' '; //empty space as default background character

    const int distanceFromCam = 100; //view is set on the z axis
    const float k1 = 40; //distance of the 
    float x, y, z; //xyz coordinates of each point
    //float ooz; //"one over z": used to turn 2 divisions into 1 division and two multiplications (cheaper computationaly)
    int xp, yp; //x and y projections on the screen, for lighting and char superposition purposes

    printf("\x1b[2J");

    while(1) { //loop maintains cube spinning until interruption 
        memset(buffer, backgroundChar, bufferLength); //resets buffer/background characters to ' '
        memset(zBuffer, 0, screenHeight * screenWidth * 4); //resets z value of each point to 0 (represents a point at infinity)

        // Front face (z = +cubeWidth/2)
        for(float i = -cubeWidth / 2; i < cubeWidth / 2; i += 0.15) {
            for(float j = -cubeWidth / 2; j < cubeWidth / 2; j += 0.15) {
                calculateForPoint(i, j, -cubeWidth / 2, '@', &x, &y, &z, distanceFromCam,
                    A, B, C, &xp, &yp, screenWidth, screenHeight, k1, zBuffer, buffer); //face (z-)
                calculateForPoint(-i, j, cubeWidth / 2, '#', &x, &y, &z, distanceFromCam,
                    A, B, C, &xp, &yp, screenWidth, screenHeight, k1, zBuffer, buffer); //face (z+)  
                calculateForPoint(-cubeWidth / 2, i, -j, '~', &x, &y, &z, distanceFromCam,
                    A, B, C, &xp, &yp, screenWidth, screenHeight, k1, zBuffer, buffer); //face (x-) 
                calculateForPoint(cubeWidth / 2, i, j, '$', &x, &y, &z, distanceFromCam,
                    A, B, C, &xp, &yp, screenWidth, screenHeight, k1, zBuffer, buffer); //face (x+) 
                calculateForPoint(i, -cubeWidth / 2, j, ';', &x, &y, &z, distanceFromCam,
                    A, B, C, &xp, &yp, screenWidth, screenHeight, k1, zBuffer, buffer); //face (y-) 
                calculateForPoint(i, cubeWidth / 2, j, '+', &x, &y, &z, distanceFromCam,
                    A, B, C, &xp, &yp, screenWidth, screenHeight, k1, zBuffer, buffer); //face (y+)   
            }
        }

        printf("\x1b[H"); //sets the cursor to the home position

        for(int k = 0; k < screenWidth * screenHeight; k++) {
            putchar(k % screenWidth ? buffer[k] : 10); //if k isn't at the end of a row prints buffer[k], else prints '\n' (ascii code 10)
        }
        A += 0.1;
        B += 0.1;
        C += 0.1;
        usleep(8000); // ~120fps cap, adjust for desired speed
    } 

    return 0;
}