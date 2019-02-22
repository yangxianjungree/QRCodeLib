#ifndef _M_QRINPUT_H_
#define _M_QRINPUT_H_


#include "mQrencode.h"
#include "mBitstream.h"


/******************************************************************************
 * Entry of input data
 *****************************************************************************/
typedef struct _QRinput_List QRinput_List;

struct _QRinput_List {
	QRencodeMode mode;
	int size;				///< Size of data chunk (byte).
	unsigned char *data;	///< Data chunk.
	BitStream *bstream;
	QRinput_List *next;
};

/******************************************************************************
 * Input Data
 *****************************************************************************/
struct _QRinput {
	int version;
	QRecLevel level;
	QRinput_List *head;
	QRinput_List *tail;
	int mqr;
	int fnc1;
	unsigned char appid;
};

/******************************************************************************
 * Structured append input data
 *****************************************************************************/
typedef struct _QRinput_InputList QRinput_InputList;

struct _QRinput_InputList {
	QRinput *input;
	QRinput_InputList *next;
};

struct _QRinput_Struct {
	int size;					///< number of structured symbols
	int parity;
	QRinput_InputList *head;
	QRinput_InputList *tail;
};


/**
 * Look up the alphabet-numeric convesion table (see JIS X0510:2004, pp.19).
 * @param __c__ character
 * @return value
 */
#define QRinput_lookAnTable(__c__) \
	((__c__ & 0x80)?-1:mQrinput::QRinput_anTable[(int)__c__])

/**
 * Length of a standard mode indicator in bits.
 */

#define MODE_INDICATOR_SIZE 4

/**
 * Length of a segment of structured-append header.
 */
#define STRUCTURE_HEADER_SIZE 20

/**
 * Maximum number of symbols in a set of structured-appended symbols.
 */
#define MAX_STRUCTURED_SYMBOLS 16

class mQrinput
{
public:
	
	static int QRinput_isSplittableMode(QRencodeMode mode);

	/**
	 * Pack all bit streams padding bits into a byte array.
	 * @param input input data.
	 * @return padded merged byte stream
	 */
	static unsigned char *QRinput_getByteStream(QRinput *input);


	static int QRinput_estimateBitsModeNum(int size);
	static int QRinput_estimateBitsModeAn(int size);
	static int QRinput_estimateBitsMode8(int size);
	static int QRinput_estimateBitsModeKanji(int size);

	static QRinput *QRinput_dup(QRinput *input);

	static const signed char QRinput_anTable[128];


};


#ifdef WITH_TESTS
	extern BitStream *QRinput_mergeBitStream(QRinput *input);
	extern BitStream *QRinput_getBitStream(QRinput *input);
	extern int QRinput_estimateBitStreamSize(QRinput *input, int version);
	extern int QRinput_splitEntry(QRinput_List *entry, int bytes);
	extern int QRinput_lengthOfCode(QRencodeMode mode, int version, int bits);
	extern int QRinput_insertStructuredAppendHeader(QRinput *input, int size, int index, unsigned char parity);
#endif


#endif