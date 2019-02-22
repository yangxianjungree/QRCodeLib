#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "mQrBitMapEncoder.h"
//#include "showbits.h"
#include "mQrencode.h"

mQrBitMapEncoder::mQrBitMapEncoder()
{
	kFileHeader.bfType = 0x4d42;  // "BM"
	//kFileHeader.bfSize =	sizeof(BITMAPFILEHEADER) +
	//	sizeof(BITMAPINFOHEADER) 
	//	;//unDataBytes;
	kFileHeader.bfReserved1 = 0;
	kFileHeader.bfReserved2 = 0;
	kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER);
	
	kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	//kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
	//kInfoHeader.biHeight = -((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
	kInfoHeader.biPlanes = 1;
	kInfoHeader.biBitCount = 24;
	kInfoHeader.biCompression = BI_RGB;
	kInfoHeader.biSizeImage = 0;
	kInfoHeader.biXPelsPerMeter = 0;
	kInfoHeader.biYPelsPerMeter = 0;
	kInfoHeader.biClrUsed = 0;
	kInfoHeader.biClrImportant = 0;

	OUT_FILE_PIXEL_PRESCALER = 4;
	color_R = 0x00;
	color_G = 0;
	color_B = 0;
	QrcoderVersion = 0;
	Casesensitive = 1;
	ModeHint = QR_MODE_8;
	Level = QR_ECLEVEL_L;

	pQRC = nullptr;
}

mQrBitMapEncoder::~mQrBitMapEncoder()
{
	if (pRGBData)
	{
		free(pRGBData);
	}
	
	mQrencode::QRcode_free(pQRC);
}

bool mQrBitMapEncoder::calStructureappendParity(int width, int height, const unsigned char *data)
{	
	unWidthAdjusted = width * OUT_FILE_PIXEL_PRESCALER * 3;
	if (unWidthAdjusted % 4)
		unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
	
	unDataBytes = unWidthAdjusted * width * OUT_FILE_PIXEL_PRESCALER;

	if (pRGBData)
	{
		free(pRGBData);
	}
	
	if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
	{
		strRemind="Out of memory";
		return false;
	}
			
	// Preset to white
	memset(pRGBData, 0xff, unDataBytes);

	kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		unDataBytes;

	kInfoHeader.biWidth = width * OUT_FILE_PIXEL_PRESCALER;
	kInfoHeader.biHeight = -((int)height * OUT_FILE_PIXEL_PRESCALER);

	const unsigned char *pSourceData = data;
	unsigned char *pDestData;
	for(int y = 0; y < width; y++)
	{
		pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
		for(int x = 0; x < width; x++)
		{
			if (*pSourceData & 1)
			{
				for(int l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
				{
					for(int n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
					{
						*(pDestData +		n * 3 + unWidthAdjusted * l) =	color_R;
						*(pDestData + 1 +	n * 3 + unWidthAdjusted * l) =	color_G;
						*(pDestData + 2 +	n * 3 + unWidthAdjusted * l) =	color_B;
					}
				}
			}
			pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
			pSourceData++;
		}
	}
	return true;
}

bool mQrBitMapEncoder::setColor(int Color_R, int Color_G, int Color_B)
{
	color_R = Color_R;
	color_G = Color_G;
	color_B = Color_B;
	return true;
}

bool mQrBitMapEncoder::setPexelPrescaler(int out_file_pixel_prescaler)
{
	OUT_FILE_PIXEL_PRESCALER = out_file_pixel_prescaler;
	return true;
}

bool mQrBitMapEncoder::setVersion(int version)
{
	QrcoderVersion = version;
	return true;
}

bool mQrBitMapEncoder::setCasesensitive(int casesensitive)
{
	Casesensitive = casesensitive;
	return true;
}

bool mQrBitMapEncoder::setQRCodeEncodeMode(QRencoderMode hint)
{
	ModeHint = hint;
	return true;
}

bool mQrBitMapEncoder::setQRCodeErrorCorrect(ERROR_CORRECTION level)
{
	Level = level;
	return true;
}

int mQrBitMapEncoder::getPexelPrescaler()
{
	return OUT_FILE_PIXEL_PRESCALER;
}

int mQrBitMapEncoder::getVersion()
{
	return QrcoderVersion;
}

int mQrBitMapEncoder::getCasesensitive()
{
	return Casesensitive;
}

QRencoderMode mQrBitMapEncoder::getQRCodeEncodeMode()
{
	return ModeHint;
}

ERROR_CORRECTION mQrBitMapEncoder::getQRCodeErrorCorrect()
{
	return Level;
}

mQrBitMapEncoder *mQrBitMapEncoder::Encode(const char * content)
{
	mQrencode::QRcode_free(pQRC);
	
	const char*		szSourceSring = content;

	if (pQRC = mQrencode::QRcode_encodeString(szSourceSring, QrcoderVersion, Level, ModeHint, Casesensitive))
	{
		if (!calStructureappendParity(pQRC->width, pQRC->width, pQRC->data))
		{
			showWarning();
			return nullptr;
		}
		
		return this;
	}

	return nullptr;
}

bool mQrBitMapEncoder::Save(const char *path)
{
	FILE* f;
	if (!(fopen_s(&f, path, "wb")))
	{
		fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
		fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
		fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);
		
		fclose(f);
		return true;
	}
	else
	{
		strRemind="Unable to open file";
		return false;
	}
}

BITMAPFILEHEADER mQrBitMapEncoder::getBitmapFileHeader()
{
	return kFileHeader;
}

BITMAPINFOHEADER mQrBitMapEncoder::getBitmapInfoHeader()
{
	return kInfoHeader;
}

void mQrBitMapEncoder::showWarning()
{
	printf(strRemind);
}

void mQrBitMapEncoder::showbits()
{
	if (!pQRC)
	{
		//showunBits("0000");
	}
	//showunBits(pQRC->data);
}