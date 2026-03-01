#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

float calculateX(float phi, int distanceFromCenter) {
    return distanceFromCenter * cos(phi);
}

float calculateY(float phi, int distanceFromCenter) {
    return distanceFromCenter * sin(phi);
}

void calculateForPoint(float i, float j, float *x, float *y, float theta, float phi, char *buffer,
    int bufferLength, int screenWidth, int screenHeight, int distanceFromCenter) {
        *x = calculateX(phi, distanceFromCenter);
        *y = calculateY(phi, distanceFromCenter);

        int idx = (int)(screenWidth / 2 + (i + *x) * 1.8) + (int)(screenHeight / 2 + (j + *y)) * screenWidth;

        if(idx >= 0 && idx < bufferLength) {
            buffer[idx] = '@';
        }
    }

int main() {
    printf("\x1b[2J");

    float omega = 0, phi = 0;

    float x, y;

    int circleRadius = 8, distanceFromCenter = 15;
    int screenWidth = 110, screenHeight = 50;
    int bufferLength = screenHeight * screenWidth;

    char buffer[bufferLength];

    while(1) {
        memset(buffer, ' ', bufferLength);

        for(float theta = 0; theta < 2 * 3.1415; theta += 0.05) {
            float i = circleRadius * cos(theta);
            float j = circleRadius * sin(theta);

            calculateForPoint(i, j, &x, &y, theta, phi, buffer, bufferLength, screenWidth, screenHeight, distanceFromCenter);
        }

        printf("\x1b[H");

        for(int k = 0; k < bufferLength; k++) {
            putchar(k % screenWidth ? buffer[k] : 10);
        }

        // omega += 0.5;
        phi += 0.02;

        usleep(8000);
    }

    return 0;
}