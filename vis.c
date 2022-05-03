#include "bezier.h"
#include <ssw.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void redraw(uint16_t r, uint32_t i) {
struct cache c;
RegionFill(0, 0, r*2, r*2, 0, 0);
float ptraceval = 3.14/i;
int32_t x[3], y[3];
uint32_t base;
char t = 0;
x[1] = r;
y[1] = r;
float p = 0;
	while (p < 3.14) {
	base = rand()%i;
        x[0] = r - cosine(base*ptraceval)*r;
        y[0] = r - sine(base*ptraceval)*r;
        x[2] = r - cosine(p)*r;
        y[2] = r - sine(p)*r;
        RegionFill(x[2], y[2], 1, 1, RGB(255 - t, 0, 0), 0);
	drawcurve(RGB(255, 255, 255), x, y, 3, 4, 1004, 0);
        p+=ptraceval;
	t++;
	Eve(&c, 0);
        }
p = 0;
	while (p < 3.14) {
	base = rand()%i;
	printf("%d\n", base);
	x[0] = cosine(base*ptraceval)*r + r;
	y[0] = sine(base*ptraceval)*r + r;
	x[2] = cosine(p)*r + r;
	y[2] = sine(p)*r + r;
	RegionFill(x[2], y[2], 1, 1, RGB(255 - t, 0, 0), 0);
	drawcurve(RGB(255, 255, 255), x, y, 3, 4, 1004, 0);
	p+=ptraceval;
	t++;
	Eve(&c, 0);
	}
}

uint32_t main (uint32_t argc, uint8_t **argv) {
if (argc < 2) {
return 1;
}
uint16_t origin[2];
uint16_t radius = atoi(argv[1]);
float pt = 3.14/atoi(argv[2]);
srand((uint32_t)&pt);
struct cache c;
XI(0, "", "", radius*2, radius*2, 0, 0, 0);
Eve(&c, 0);
RegionFill(0, 0, radius*2, radius*2, 0, 0);
	while (1) {
	Eve(&c, 0);
		if (c.t==1) {
		printf("%d\n", c.txt);
		}
		if (c.t==2) {
		printf("%d %d\n", c.x, c.y);
		}
		if (c.t==3) {
		Clean(0);
		XI(0, "", "", radius*2, radius*2, 0, 0, 0);
		Eve(&c, 0);
		RegionFill(0, 0, radius*2, radius*2, 0, 0);
		}
	redraw(radius, atoi(argv[2]));
	}

Clean(0);
}

//uint8_t *map;
//if (argc==3) {
//map = malloc(3*8000*8000);
//free(map);
//}
