import numpy as np
import time
import sys

a, b, c = 0, 0, 0

cubeWidth = 40
screenWidth, screenHeight = 100, 50
bufferLength = screenWidth * screenHeight

distanceFromCam = 100
k1 = 40

def calculateX(i, j, k, a, b, c):
    return np.cos(c)*(i*np.cos(b) - np.sin(b)*(k*np.cos(a) - j*np.sin(a)));

def calculateY(i, j, k, a, b, c):
    return np.cos(c)*(j*np.cos(a) + k*np.sin(a)) - np.sin(c)*(i*np.cos(b) - np.sin(b)*(k*np.cos(a) - j*np.sin(a)))

def calculateZ(i, j, k, a, b, c):
    return i*np.sin(b) + np.cos(b)*(k*np.cos(a) - j*np.sin(a))

def calculateProjection(i, j, k, symbol): 
    x = calculateX(i, j, k, a, b, c)
    y = calculateY(i, j, k, a, b, c)
    z = calculateZ(i, j, k, a, b, c) + distanceFromCam

    xp = int(screenWidth / 2 + k1 * x / z * 2)
    yp = int(screenHeight / 2 + k1 * y / z)

    idx = xp + yp * screenWidth

    if idx >= 0 and idx < bufferLength:
        if 1 / z > zBuffer[idx]:
            buffer[idx] = symbol
            zBuffer[idx] = 1 / z

print("\x1b[2J")

while 1:
    buffer = np.full(bufferLength, ' ')
    zBuffer = np.zeros(bufferLength, dtype=float)

    i, j = cubeWidth / 2, cubeWidth / 2  

    while i > -cubeWidth / 2:
        j = cubeWidth / 2
        while j > -cubeWidth / 2:
            calculateProjection(i, j, cubeWidth / 2, '😛') #face (+z)

            j -= 0.15
        i -= 0.15
    
    sys.stdout.write("\x1b[H")

    for k in range(bufferLength):
        if k != 0 and k % screenWidth == 0:
            sys.stdout.write('\n')
        else:
            sys.stdout.write(buffer[k])
    
    sys.stdout.flush()
    
    a += 0.15
    b += 0.15
    c += 0.1

    time.sleep(0.008)