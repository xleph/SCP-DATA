#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define u_char unsigned char

float cosine(float x) {
return (40320 - x*x*20160 + x*x*x*x*1680 - x*x*x*x*x*x*56 + x*x*x*x*x*x*x*x)/40320;
}

float sine(float x) {
return (x*5040 - x*x*x*840 + x*x*x*x*x*42 - x*x*x*x*x*x*x)/5040;
}

void *rotate(void *ptr, void *ptr1, float angle, char type) {
float co, si;
co = cosine(angle);
si = sine(angle);
	if (type) {
	uint32_t *out = malloc(8);
	uint32_t *o = ptr;
	uint32_t *r = ptr1;
	out[0] = co*(r[0] - o[0]) - si*(r[1] - o[1])+o[0];
	out[1] = si*(r[0] - o[0]) + co*(r[1] - o[1])+o[1];
	return (void *)out;
	} else {
	u_char *out = malloc(2);
	u_char *o = ptr;
	u_char *r = ptr1;
	out[0] = co*(r[0] - o[0]) - si*(r[1] - o[1])+o[0];
	out[1] = si*(r[0] - o[0]) + co*(r[1] - o[1])+o[1];
	return (void *)out;
	}
}

u_char *italicize(char *font) { // why is input font type not same as out?
int i = 0;
u_char origin[2];
u_char *xy;
origin[0] = 5;
origin[1] = 200;
while (1-(font[i]==3)) {
i++;
}
u_char *out = malloc(i+1); // or take in font size as argument
i = 0;
	while (1 - (font[i - !!i]==3)) {
		if (font[i + 2]==2) {
		out[i] = font[i];
		i++;
		out[i] = font[i];
		i++;
		out[i] = 2;
		i++;
		} else {
		xy = (char *)rotate((void *)origin, (void *)(font + i), 3.14/16, 0);
		out[i] = xy[0];
		i++;
		out[i] = xy[1] - 8;
		i++;
		out[i] = font[i];
		i++;
		}
	}
return out;
}

int adjpow(uint32_t n) {
char i = 31;
	while ((n&((~0)<<i))==0) {
	i--;
	}
i--;
return ((n>>i)+((n>>i)&1))<<i;
}

void drawcircle(uint32_t x, 
		uint32_t y, 
		uint32_t radius, 
		uint32_t max, 
		uint8_t r, 
		uint8_t g, 
		uint8_t b, 
		uint8_t *rmap, 
		uint8_t *gmap, 
		uint8_t *bmap) {
int32_t rt = 0;
uint32_t ry;
int32_t i = 0;
	while (rt < radius) {
	ry = 0;
		while (ry*ry < radius*radius-rt*rt) {
		i = 0 - rt;
			while (i < rt) {
			rmap[(y-ry)*max+x+i] = r;
			gmap[(y-ry)*max+x+i] = g;
			bmap[(y-ry)*max+x+i] = b;
			rmap[(y+ry)*max+x+i] = r;
			gmap[(y+ry)*max+x+i] = g;
			bmap[(y+ry)*max+x+i] = b;
			i++;
			}
		ry++;
		}
	rt++;
	}
}

void drawcurve( uint32_t *x,
		uint32_t *y, 
		u_char n,
		uint32_t width,  
		uint32_t tif, 
		uint32_t max, 
		u_char r,
		u_char g,
		u_char b,
		u_char *rmap, 
		u_char *gmap, 
		u_char *bmap) {
u_char i, j;
uint32_t slice[20];
uint32_t xi, yi;
double t, tc, tr, tg, tb, snbl, totx, toty;
tc = (double)1.0/adjpow(tif);
snbl = 1;

slice[0] = 1;
	for (i = 1; i < (n + 1)/2; i++) {
	slice[i] = slice[i - 1] * (n - i)/i;
	}

	for (i = n - 1; i > (n - 1)/2; i--) {
	slice[i] = slice[n - i - 1];
	}

	for (t = 0; (uint)t==0; t+=tc+tc*(t>0.25||t>0.75)) {
		totx=0;
		toty=0;
		i  = 0;
		while (i < n) { // n-1 times (for each term)
		snbl = 1;
			for (j = 0; j < n - i - 1; j++) {
			snbl*=(1 - t);
			}
			for (j = 0; j < i; j++) {
			snbl*=t;
			}

		snbl*=slice[i];

		totx+=x[i]*snbl;
		toty+=y[i]*snbl;
		i++;
		}
	xi = (uint32_t)totx;
	yi = (uint32_t)toty;
		if (width > 1) {
		tr = 255 - r + r * t;
		tg = 255 - g + g * t;
		tb = 255 - b + b * t;
		drawcircle(xi, yi, width, max, (uint8_t)tr, (uint8_t)tg, (uint8_t)tb, rmap, gmap, bmap);
		}
	//rmap[yi*max+xi] = r;
	//gmap[yi*max+xi] = g;
	//bmap[yi*max+xi] = b;
	}
}

void drawstr(u_char *font, u_char *str, uint32_t x, uint32_t y, u_char dscl) {
uint32_t i, j, f, o;
char size, pc;

i = 0;
	while (str[i]) {
	j = 0;
		while (font[j-!!j]!=3) {
			if (str[i]==font[j]&&font[j+2]==2) {
			j+=3;
			break;
			}
		j+=3;
		}
		if (font[j-1]==3) {
		//RegionFill(x+i*128/dscl+32/dscl, y+32/dscl, 64/dscl, 1, RGB(255, 255, 255), win);
		//RegionFill(x+(i+1)*128/dscl-32/dscl, y+32/dscl, 1, 168/dscl, RGB(255, 255, 255), win);
		//RegionFill(x+    i*128/dscl+32/dscl, y+32/dscl, 1, 168/dscl, RGB(255, 255, 255), win);
		//RegionFill(x+i*128/dscl+32/dscl, y+192/dscl, 64/dscl, 1, RGB(255, 255, 255), win);
		} else {
		o = j + 3;
			while (1 - (font[o-1]==3||font[o+2]==2)) {
			o = j + 3;
				while (font[o-1]!=1&&font[o-1]!=3&&font[o+2]!=2) {
				o+=3;
				}
			uint32_t cx[(o - j)/3];
			uint32_t cy[(o - j)/3];
			pc = 0;
			f = j;
			while (1-(j==o)) {
			cx[pc] = x + i*128/dscl + font[j]/dscl;
			cy[pc++] = y + font[j + 1]/dscl;
			j+=3;
			}							// was (o - f)*WH(win)/20
			//drawcurve(/*color*/0, cx, cy, (o - f)/3, 4, (o - f)*400);
			}
		}
	i++;
	}
}
