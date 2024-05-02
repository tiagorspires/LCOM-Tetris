#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <matrix.h>


int getColorIndex(char c, CharColorMap map[], int size) {
    if (c == '-') {
        return -1; 
    }
    for (int i = 0; i < size; i++) {
        if (map[i].key == c) {
            return map[i].colorIndex;
        }
    }
    return -1; 
}


