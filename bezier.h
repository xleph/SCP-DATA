#include <stdint.h>
#include <stdio.h>
extern void *rotate(void *ptr, void *ptr1, float angle, char type); // char/int return
extern uint8_t *italicize(uint8_t *font);
extern void drawcircle(uint32_t x, uint32_t y, uint32_t radius, uint32_t max, uint8_t r, uint8_t g, uint8_t b, uint8_t *rmap, uint8_t *gmap, uint8_t *bmap);
extern void drawcurve(uint32_t *x, uint32_t *y, uint8_t n, uint32_t width, uint32_t tif, uint32_t max, uint8_t r, uint8_t g, uint8_t b, uint8_t *rmap, uint8_t *gmap, uint8_t *bmap);
extern void drawstr(unsigned char *font, unsigned char *str, unsigned int x, unsigned int y, unsigned char dscl, char win);
extern float cosine(float f);
extern float sine(float f);
