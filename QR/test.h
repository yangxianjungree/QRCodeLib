#include "stdafx.h"
#include <string.h>
#include <errno.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

#include "mQrencode.h"
#include "mQrBitMapEncoder.h"
#include <iostream>


#define QRCODE_TEXT "abcdefg!!!!!!!@#$%hijk123131lmn"						// Text to encode into QRCode
#define OUT_FILENEW					"d:/testmmm.bmp"								// Output file name



int test_tmain(int argc, _TCHAR* argv[])
{
	/*
	 * Create a symbol from the string. The library automatically parses the input
	 * string and encodes in a QR Code symbol.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 * @param string input string. It must be NUL terminated.
	 * @param version version of the symbol. If 0, the library chooses the minimum
	 *                version for the given input data.
	 * @param level error correction level.
	 * @param hint tell the library how non-alphanumerical characters should be
	 *             encoded. If QR_MODE_KANJI is given, kanji characters will be
	 *             encoded as Shif-JIS characters. If QR_MODE_8 is given, all of
	 *             non-alphanumerical characters will be encoded as is. If you want
	 *             to embed UTF-8 string, choose this.
	 * @param casesensitive case-sensitive(1) or not(0).
	 * @return an instance of QRcode class. The version of the result QRcode may
	 *         be larger than the designated version. On error, NULL is returned,
	 *         and errno is set to indicate the error. See Exceptions for the
	 *         details.
	 * @throw EINVAL invalid input object.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 * @throw ERANGE input data is too large.
	 */

	// Compute QRCode

	mQrBitMapEncoder mbitmap;
	mbitmap.setPexelPrescaler(6);	// set number of pixels in bmp file for each QRCode pixel, on each dimension
	mQrBitMapEncoder *coder = mbitmap.Encode(QRCODE_TEXT);	// encode content of qrcode
	if (!coder)
	{
		coder->showWarning();
	}

	if (!coder->Save(OUT_FILENEW))	// save image in output file path
	{
		mbitmap.showWarning();
		exit(-1);
	}
	
	coder->showbits();
	
	return 0;
}

