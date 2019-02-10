#include <stdlib.h>
#include <stdio.h>

#include "../src/roll.h"

int main() {
    signed char buf[100] = { 0, };

    FILE *f = fopen("test44100.raw", "wb");
    librick_init(44100);
    for(int i=0; i<44100 * 30; i++) {
        librick_render(&buf, 1);
        fwrite((unsigned char *)&buf, 1, 1, f);
    }
    fclose(f);
    printf("\n");

    f = fopen("test8000.raw", "wb");
    librick_init(8000);
    for(int i=0; i<8000 * 30; i++) {
        librick_render(&buf, 1);
        fwrite((unsigned char *)&buf, 1, 1, f);
    }
    fclose(f);
    printf("\n");
}
