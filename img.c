#include "bezier.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

float aid(float f) {
	while (f > 3.1415) {
	f-=6.2832;
	}
return f;
}

uint16_t *findscp(uint16_t scp, FILE *F) {
uint8_t nscp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t rscp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t tot = 0;
uint16_t *out;
fseek(F, 0, 2);
uint32_t begin;
uint32_t end = 
ftell(F);
fseek(F, 0, 0);
	while (ftell(F) < end) {
		nscp[0] = fgetc(F);
		if (nscp[0]=='p') { 
		fgetc(F);
		nscp[0] = fgetc(F);
			while (nscp[nscp[8]++]!=':') {
			nscp[nscp[8]] = fgetc(F);
			}
			nscp[8] = 0;
			if (atoi(nscp)==scp) {
			begin = ftell(F);
				while (atoi(nscp)==scp) {
				tot++;
				fgetc(F);
				fgetc(F);
				fgetc(F);
				fgetc(F);
				rscp[0] = fgetc(F);
					while (rscp[rscp[8]++]!='\n'&&ftell(F) < end) {
					rscp[rscp[8]] = fgetc(F);
					}
				rscp[8] = 0;
					while (nscp[0]!='p'&&ftell(F) < end) {
					nscp[0] = fgetc(F);
					}
				fgetc(F);
				nscp[0] = fgetc(F);
					while (nscp[nscp[8]++]!=':'&&ftell(F) < end) {
					nscp[nscp[8]] = fgetc(F);
					}
				nscp[8] = 0;
				}
			out = malloc(tot*2 + 2);
			fseek(F, begin, 0);
			out[tot] = 0;
				while (tot) {
				tot--;
				fgetc(F);
				fgetc(F);
				fgetc(F);
				fgetc(F);
				rscp[0] = fgetc(F);
					while (rscp[rscp[8]++]!='\n'&&ftell(F) < end) {
					rscp[rscp[8]] = fgetc(F);
					}
				rscp[8] = 0;
				nscp[0] = fgetc(F);
					while (nscp[0]!=':'&&ftell(F) < end) {
					nscp[0] = fgetc(F);
					}
				out[tot] = atoi(rscp);
				}
			return out;
			}
		}
	}
return 0;
}

uint16_t *uniq(uint16_t *references) {
	if (!references) {
	return 0;
	}
uint16_t *out;
uint16_t i, j, k;
i = 0;
	while (references[i]) {
	i++;
	}
uint16_t ncheck[i];
i = 0;
	while (references[i]) {
	ncheck[i] = references[i];
	i++;
	}
k = 0;
	while (k < i) { 
	j = k + 1;
		while (j < i) {
		references[j] = references[j] * (references[j]!=references[k]);
		j++;
		}
	k++;
	}
j = 0;
k = 0;
	while (j < i) {
	k+=!!references[j];
	j++;
	}
out = malloc(k*4 + 2);
out[0] = k;
j = 0;
k = 1;
	while (j < i) {
		if (references[j]) {
		out[k] = references[j];
		k++;
		}
	j++;
	}
j = 0;
	while (j < out[0]) {
	i = 0;
		while (ncheck[i]) {
		out[k]+=(out[j+1]==ncheck[i]);
		i++;
		}
	k++;
	j++;
	}
return out;
}

void draw(uint16_t **scps, uint16_t maxref, uint16_t r, uint16_t max, uint32_t i, uint8_t *rmap, uint8_t *gmap, uint8_t *bmap, FILE *F) {
uint16_t t, tdata;
uint8_t clr[7][3] = {255, 0, 0,
		120, 120, 0,
		130, 20, 0,
		0, 255, 255,
		255, 0, 255,
		33, 0, 33,
		0, 0, 255};

float ptraceval = 6.2832/(i - 1);
uint16_t *data;
int32_t x[3], y[3];
float dis = 6.2832 * r / i;
uint32_t maxw = dis - 2*(dis > 1);
t = 2;
x[1] = max;
y[1] = max;
float p = 0;
	while (t < i + 1) {
        x[0] = max - cosine(aid(p))*r;
        y[0] = max - sine(aid(p))*r;
	data = scps[t - 2];
		if (data) {
		tdata = 0;
			while (tdata < data[0]) {
        		x[2] = max - cosine(aid((data[tdata+1]-2)*ptraceval))*r;
       			y[2] = max - sine(aid((data[tdata+1]-2)*ptraceval))*r;
				if (data[tdata+1]!=t&&data[tdata+1] < i + 1) {
				drawcurve(x, y, 3, 2 + data[tdata+data[0]+1]*maxw/maxref/2, max*2, max*2, clr[t%7][0], clr[t%7][1], clr[t%7][2], rmap, gmap, bmap);
				} else {
				//printf("%d %d %d\n", maxw, maxref, data[tdata+data[0]+1]*maxw/maxref/2);
				drawcircle(x[2], y[2], 2 + data[tdata+data[0]+1]*maxw/maxref/2, max*2, 255-clr[t%7][0], 255-clr[t%7][1], 255-clr[t%7][2], rmap, gmap, bmap);
				}
			tdata++;
			}
		}
        p+=ptraceval;
	t++;
        }
}

char *genfile(uint32_t scp) {
uint16_t pow = 1;
uint8_t i = 0;
char *filename;
	while (scp/pow) {
	pow*=10;
	i++;
	}
filename = malloc(i+4);
i = 0;
pow/=10;
	while (pow) {
	filename[i] = (scp/pow)%10 + 48;
	i++;
	pow/=10;
	}
filename[i++] = '.';
filename[i++] = 'p';
filename[i++] = 'b';
filename[i++] = 'm';
filename[i] = 0;
return filename;
}

uint32_t main (uint32_t argc, uint8_t **argv) {
if (argc < 5) {
return 1;
}
FILE *F = fopen("data", "r");
FILE *BMP;
uint32_t i, j;
uint16_t origin[2];
uint16_t max = atoi(argv[1]);
//		    argv[2] is height
uint16_t radius = atoi(argv[3]);
uint16_t ref = atoi(argv[4]);
uint8_t *rmap = malloc(max*max);
uint8_t *gmap = malloc(max*max);
uint8_t *bmap = malloc(max*max);

uint16_t t, high, tdata;
high = 1;
t = 0;

uint16_t **scps = malloc(8*(ref - 1)); // I have a bad feeling about this
printf("%d\n", sizeof(scps));
	while (t < ref - 1) {
	scps[t] = uniq(findscp(t + 2, F));
		if (scps[t]) {
		tdata = scps[t][0] + 1;
			if (tdata==2) {
			high = high*(high > scps[t][2]) + scps[t][2]*(high < scps[t][2]);
			}
			while (tdata < scps[t][0]*2) {
			high = high*(high > scps[t][tdata]) + scps[t][tdata]*(high < scps[t][tdata]);
			tdata++;
			}
		}
	t++;
	}
if (argv[5][0]=='r') {
j = 2;
	while (j < ref) {
	BMP = fopen(genfile(j), "w");
	fprintf(BMP, "P3 %s %s 255\n", argv[1], argv[2]);
	i = 0;
		while (i < max * max) {
		rmap[i] = 0;
		gmap[i] = 0;
		bmap[i] = 0;
		i++;
		}

	draw(scps, high, radius, max/2, j, rmap, gmap, bmap, F);
	i = 0;
		while (i < max * max) {
		fprintf(BMP, "%d %d %d ", rmap[i], gmap[i], bmap[i]);
		i++;
		}
	fclose(BMP);
	j++;
	}
} else {
BMP = fopen(genfile(ref), "w");
fprintf(BMP, "P3 %s %s 255\n", argv[1], argv[2]);
i = 0;
	while (i < max * max) {
	rmap[i] = 0;
	gmap[i] = 0;
	bmap[i] = 0;
	i++;
	}
draw(scps, high, radius, max/2, ref, rmap, gmap, bmap, F);
i = 0;
	while (i < max * max) {
	fprintf(BMP, "%d %d %d ", rmap[i], gmap[i], bmap[i]);
	i++;
	}
fclose(BMP);
}
free(rmap);
free(gmap);
free(bmap);
fclose(F);
}
