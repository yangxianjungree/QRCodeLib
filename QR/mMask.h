#ifndef _M_MASK_H_
#define _M_MASK_H_

class mMask
{
public:

static unsigned char *Mask_makeMask(int width, unsigned char *frame, int mask, QRecLevel level);
static unsigned char *Mask_mask(int width, unsigned char *frame, QRecLevel level);

};

#ifdef WITH_TESTS
extern int Mask_calcN2(int width, unsigned char *frame);
extern int Mask_calcN1N3(int length, int *runLength);
extern int Mask_calcRunLength(int width, unsigned char *frame, int dir, int *runLength);
extern int Mask_evaluateSymbol(int width, unsigned char *frame);
extern int Mask_writeFormatInformation(int width, unsigned char *frame, int mask, QRecLevel level);
extern unsigned char *Mask_makeMaskedFrame(int width, unsigned char *frame, int mask);
#endif

#endif