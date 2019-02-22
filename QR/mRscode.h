#ifndef _M_RSCODE_H_
#define _M_RSCODE_H_

/*
 * General purpose RS codec, 8-bit symbols.
 */

typedef struct _RS RS;

class mRscode
{
public:

	static RS *init_rs(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad);
	static void encode_rs_char(RS *rs, const unsigned char *data, unsigned char *parity);
	static void free_rs_char(RS *rs);
	static void free_rs_cache(void);

};

#endif