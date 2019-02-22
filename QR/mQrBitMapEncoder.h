#pragma once 
#ifndef _M_BITMAP_H_
#define _M_BITMAP_H_


// BMP defines

typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef signed long		LONG;

#define BI_RGB			0L


#pragma pack(push, 2)

typedef struct  
	{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
	} BITMAPFILEHEADER;

typedef struct 
	{
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
	} BITMAPINFOHEADER;

#pragma pack(pop)

typedef enum {
	QR_MODE_NUL = -1,  ///< Terminator (NUL character). Internal use only
	QR_MODE_NUM = 0,   ///< Numeric mode
	QR_MODE_AN,        ///< Alphabet-numeric mode
	QR_MODE_8,         ///< 8-bit data mode
	QR_MODE_KANJI,     ///< Kanji (shift-jis) mode
	QR_MODE_STRUCTURE, ///< Internal use only
	QR_MODE_ECI,       ///< ECI mode
	QR_MODE_FNC1FIRST,  ///< FNC1, first position
	QR_MODE_FNC1SECOND, ///< FNC1, second position
} QRencoderMode;	//typedef QRencoderMode QRencodeMode;

typedef enum {
	QR_ECLEVEL_L = 0, ///< lowest
	QR_ECLEVEL_M,
	QR_ECLEVEL_Q,
	QR_ECLEVEL_H      ///< highest
} ERROR_CORRECTION;	//typedef ERROR_CORRECTION QRecLevel;

typedef struct {
	int version;         ///< version of the symbol
	int width;           ///< width of the symbol
	unsigned char *data; ///< symbol data
} QRcoder;




class mQrBitMapEncoder
{
public:
	mQrBitMapEncoder();
	~mQrBitMapEncoder();
public:
	bool calStructureappendParity(int width, int height, const unsigned char * data);
	bool Save(const char *path);
	void showWarning();
	void showbits();
	mQrBitMapEncoder *Encode(const char * content);

	bool setColor(int Color_R = 0x00, int Color_G = 0, int Color_B = 0);
	bool setPexelPrescaler(int out_file_pixel_prescaler = 4);
	bool setVersion(int version = 0);
	bool setCasesensitive(int casesensitive = 1);
	bool setQRCodeEncodeMode(QRencoderMode hint = QR_MODE_8);
	bool setQRCodeErrorCorrect(ERROR_CORRECTION level = QR_ECLEVEL_L);

	BITMAPFILEHEADER getBitmapFileHeader();
	BITMAPINFOHEADER getBitmapInfoHeader();
	int getPexelPrescaler();
	int getVersion();
	int getCasesensitive();
	QRencoderMode getQRCodeEncodeMode();
	ERROR_CORRECTION getQRCodeErrorCorrect();
private:
	char * strRemind;
	BITMAPFILEHEADER kFileHeader;
	BITMAPINFOHEADER kInfoHeader;
	int OUT_FILE_PIXEL_PRESCALER;	// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)
	unsigned int unWidthAdjusted;
	unsigned int unDataBytes;
	unsigned char *pRGBData;
	unsigned int color_R;
	unsigned int color_G;
	unsigned int color_B;
	int QrcoderVersion;
	int Casesensitive;
	QRencoderMode ModeHint;
	ERROR_CORRECTION Level;
	
	QRcoder*			pQRC;
};


#endif