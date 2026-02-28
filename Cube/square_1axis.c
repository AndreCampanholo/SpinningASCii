#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

float calculateX(float i, float j, float theta) {
    return i * cos(theta) + j * sin(theta);
}

float calculateY(float i, float j, float theta) {
    return j * cos(theta) - i * sin(theta);
}

void calculateForPoint(float i, float j, float *x, float *y, float theta, char *buffer, int screenWidth, int screenHeight, int bufferLength) {
    *x = calculateX(i, j, theta);
    *y = calculateY(i, j, theta);

    int idx = (int)(screenWidth / 2 + *x * 1.8) + (int)(screenHeight / 2 + *y) * screenWidth;

    if(idx >= 0 && idx < bufferLength) {
        buffer[idx] = '@';
    }
}

int main() {
    printf("\x1b[2J");

    float x, y;
    float theta = 0;

    int screenHeight = 50, screenWidth = 100;
    int squareWidth = 30;

    int bufferLength = screenHeight * screenWidth;

    char buffer[bufferLength];

    while(1) {
        memset(buffer, ' ', bufferLength);

        for(float i = -squareWidth / 2; i < squareWidth / 2; i += 0.15) {
            for(float j = -squareWidth / 2; j < squareWidth / 2; j += 0.15) {
                calculateForPoint(i, j, &x, &y, theta, buffer, screenWidth, screenHeight, bufferLength);
            }
        }

        printf("\x1b[H");

        for(int k = 0; k < bufferLength; k++) {
            putchar(k % screenWidth ? buffer[k] : 10);
        }

        theta += 0.05;

        usleep(8000);
    }

    return 0;
}