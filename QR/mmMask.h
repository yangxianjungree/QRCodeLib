#ifndef _M_MMASK_H_
#define _M_MMASK_H_

class mmMask
{
public:

static unsigned char *MMask_makeMask(int version, unsigned char *frame, int mask, QRecLevel level);
static unsigned char *MMask_mask(int version, unsigned char *frame, QRecLevel level);

};

#ifdef WITH_TESTS
extern int MMask_evaluateSymbol(int width, unsigned char *frame);
extern void MMask_writeFormatInformation(int version, int width, unsigned char *frame, int mask, QRecLevel level);
extern unsigned char *MMask_makeMaskedFrame(int width, unsigned char *frame, int mask);
#endif

#endif