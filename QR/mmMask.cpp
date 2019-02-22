#include "stdafx.h"

#if HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "mQrencode.h"
#include "mmQrspec.h"
#include "mmMask.h"

__STATIC void MMask_writeFormatInformation(int version, int width, unsigned char *frame, int mask, QRecLevel level)
{
	unsigned int format;
	unsigned char v;
	int i;

	format = mmQrspec::MQRspec_getFormatInfo(mask, version, level);

	for(i=0; i<8; i++) {
		v = 0x84 | (format & 1);
		frame[width * (i + 1) + 8] = v;
		format = format >> 1;
	}
	for(i=0; i<7; i++) {
		v = 0x84 | (format & 1);
		frame[width * 8 + 7 - i] = v;
		format = format >> 1;
	}
}

#define MASKMAKER(__exp__) \
	int x, y;\
\
	for(y=0; y<width; y++) {\
		for(x=0; x<width; x++) {\
			if(*s & 0x80) {\
				*d = *s;\
			} else {\
				*d = *s ^ ((__exp__) == 0);\
			}\
			s++; d++;\
		}\
	}

static void Mask_mask0(int width, const unsigned char *s, unsigned char *d)
{
	MASKMAKER(y&1)
}

static void Mask_mask1(int width, const unsigned char *s, unsigned char *d)
{
	MASKMAKER(((y/2)+(x/3))&1)
}

static void Mask_mask2(int width, const unsigned char *s, unsigned char *d)
{
	MASKMAKER((((x*y)&1)+(x*y)%3)&1)
}

static void Mask_mask3(int width, const unsigned char *s, unsigned char *d)
{
	MASKMAKER((((x+y)&1)+((x*y)%3))&1)
}

#define maskNum (4)
typedef void MaskMaker(int, const unsigned char *, unsigned char *);
static MaskMaker *maskMakers[maskNum] = {
	Mask_mask0, Mask_mask1, Mask_mask2, Mask_mask3
};

#ifdef WITH_TESTS
unsigned char *MMask_makeMaskedFrame(int width, unsigned char *frame, int mask)
{
	unsigned char *masked;

	masked = (unsigned char *)malloc(width * width);
	if(masked == NULL) return NULL;

	maskMakers[mask](width, frame, masked);

	return masked;
}
#endif

unsigned char *mmMask::MMask_makeMask(int version, unsigned char *frame, int mask, QRecLevel level)
{
	unsigned char *masked;
	int width;

	if(mask < 0 || mask >= maskNum) {
		errno = EINVAL;
		return NULL;
	}

	width = mmQrspec::MQRspec_getWidth(version);
	masked = (unsigned char *)malloc(width * width);
	if(masked == NULL) return NULL;

	maskMakers[mask](width, frame, masked);
	MMask_writeFormatInformation(version, width, masked, mask, level);

	return masked;
}

__STATIC int MMask_evaluateSymbol(int width, unsigned char *frame)
{
	int x, y;
	unsigned char *p;
	int sum1 = 0, sum2 = 0;

	p = frame + width * (width - 1);
	for(x=1; x<width; x++) {
		sum1 += (p[x] & 1);
	}

	p = frame + width * 2 - 1;
	for(y=1; y<width; y++) {
		sum2 += (*p & 1);
		p += width;
	}

	return (sum1 <= sum2)?(sum1 * 16 + sum2):(sum2 * 16 + sum1);
}

unsigned char *mmMask::MMask_mask(int version, unsigned char *frame, QRecLevel level)
{
	int i;
	unsigned char *mask, *bestMask;
	int maxScore = 0;
	int score;
	int width;

	width = mmQrspec::MQRspec_getWidth(version);

	mask = (unsigned char *)malloc(width * width);
	if(mask == NULL) return NULL;
	bestMask = NULL;

	for(i=0; i<maskNum; i++) {
		score = 0;
		maskMakers[i](width, frame, mask);
		MMask_writeFormatInformation(version, width, mask, i, level);
		score = MMask_evaluateSymbol(width, mask);
		if(score > maxScore) {
			maxScore = score;
			free(bestMask);
			bestMask = mask;
			mask = (unsigned char *)malloc(width * width);
			if(mask == NULL) break;
		}
	}
	free(mask);
	return bestMask;
}
