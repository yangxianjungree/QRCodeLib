#pragma once

#ifndef _M_QRENCODE_H_
#define _M_QRENCODE_H_

#include "mQrBitMapEncoder.h"

/**
 * Encoding mode.
 */
//typedef enum {
//	QR_MODE_NUL = -1,  ///< Terminator (NUL character). Internal use only
//	QR_MODE_NUM = 0,   ///< Numeric mode
//	QR_MODE_AN,        ///< Alphabet-numeric mode
//	QR_MODE_8,         ///< 8-bit data mode
//	QR_MODE_KANJI,     ///< Kanji (shift-jis) mode
//	QR_MODE_STRUCTURE, ///< Internal use only
//	QR_MODE_ECI,       ///< ECI mode
//	QR_MODE_FNC1FIRST,  ///< FNC1, first position
//	QR_MODE_FNC1SECOND, ///< FNC1, second position
//} QRencodeMode;

typedef QRencoderMode QRencodeMode;

/**
 * Level of error correction.
 */
//typedef enum {
//	QR_ECLEVEL_L = 0, ///< lowest
//	QR_ECLEVEL_M,
//	QR_ECLEVEL_Q,
//	QR_ECLEVEL_H      ///< highest
//} QRecLevel;

typedef ERROR_CORRECTION QRecLevel;

/**
 * Maximum version (size) of QR-code symbol.
 */
#define QRSPEC_VERSION_MAX 40

/**
 * Maximum version (size) of QR-code symbol.
 */
#define MQRSPEC_VERSION_MAX 4



/******************************************************************************
 * Input data (mQrinput.cpp)
 *****************************************************************************/

/**
 * Singly linked list to contain input strings. An instance of this class
 * contains its version and error correction level too. It is required to
 * set them by QRinput_setVersion() and QRinput_setErrorCorrectionLevel(),
 * or use QRinput_new2() to instantiate an object.
 */
typedef struct _QRinput QRinput;


/**
 * Set of QRinput for structured symbols.
 */
typedef struct _QRinput_Struct QRinput_Struct;



/******************************************************************************
 * QRcode output (mQrencode.cpp)
 *****************************************************************************/

/**
 * QRcode class.
 * Symbol data is represented as an array contains width*width uchars.
 * Each uchar represents a module (dot). If the less significant bit of
 * the uchar is 1, the corresponding module is black. The other bits are
 * meaningless for usual applications, but here its specification is described.
 *
 * <pre>
 * MSB 76543210 LSB
 *     |||||||`- 1=black/0=white
 *     ||||||`-- data and ecc code area
 *     |||||`--- format information
 *     ||||`---- version information
 *     |||`----- timing pattern
 *     ||`------ alignment pattern
 *     |`------- finder pattern and separator
 *     `-------- non-data modules (format, timing, etc.)
 * </pre>
 */
//typedef struct {
//	int version;         ///< version of the symbol
//	int width;           ///< width of the symbol
//	unsigned char *data; ///< symbol data
//} QRcode;
typedef QRcoder QRcode;

/**
 * Singly-linked list of QRcode. Used to represent a structured symbols.
 * A list is terminated with NULL.
 */
typedef struct _QRcode_List QRcode_List;

struct _QRcode_List {
	QRcode *code;
	QRcode_List *next;
};

class mQrencode
{

public:

	/**
	 * Instantiate an input data object. The version is set to 0 (auto-select)
	 * and the error correction level is set to QR_ECLEVEL_L.
	 * @return an input object (initialized). On error, NULL is returned and errno
	 *         is set to indicate the error.
	 * @throw ENOMEM unable to allocate memory.
	 */
	static QRinput *QRinput_new(void);

	/**
	 * Instantiate an input data object.
	 * @param version version number.
	 * @param level Error correction level.
	 * @return an input object (initialized). On error, NULL is returned and errno
	 *         is set to indicate the error.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 * @throw EINVAL invalid arguments.
	 */
	static QRinput *QRinput_new2(int version, QRecLevel level);

	/**
	 * Instantiate an input data object. Object's Micro QR Code flag is set.
	 * Unlike with full-sized QR Code, version number must be specified (>0).
	 * @param version version number (1--4).
	 * @param level Error correction level.
	 * @return an input object (initialized). On error, NULL is returned and errno
	 *         is set to indicate the error.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 * @throw EINVAL invalid arguments.
	 */
	static QRinput *QRinput_newMQR(int version, QRecLevel level);

	/**
	 * Append data to an input object.
	 * The data is copied and appended to the input object.
	 * @param input input object.
	 * @param mode encoding mode.
	 * @param size size of data (byte).
	 * @param data a pointer to the memory area of the input data.
	 * @retval 0 success.
	 * @retval -1 an error occurred and errno is set to indeicate the error.
	 *            See Execptions for the details.
	 * @throw ENOMEM unable to allocate memory.
	 * @throw EINVAL input data is invalid.
	 *
	 */
	static int QRinput_append(QRinput *input, QRencodeMode mode, int size, const unsigned char *data);

	/**
	 * Append ECI header.
	 * @param input input object.
	 * @param ecinum ECI indicator number (0 - 999999)
	 * @retval 0 success.
	 * @retval -1 an error occurred and errno is set to indeicate the error.
	 *            See Execptions for the details.
	 * @throw ENOMEM unable to allocate memory.
	 * @throw EINVAL input data is invalid.
	 *
	 */
	static int QRinput_appendECIheader(QRinput *input, unsigned int ecinum);

	/**
	 * Get current version.
	 * @param input input object.
	 * @return current version.
	 */
	static int QRinput_getVersion(QRinput *input);

	/**
	 * Set version of the QR code that is to be encoded.
	 * This function cannot be applied to Micro QR Code.
	 * @param input input object.
	 * @param version version number (0 = auto)
	 * @retval 0 success.
	 * @retval -1 invalid argument.
	 */
	static int QRinput_setVersion(QRinput *input, int version);

	/**
	 * Get current error correction level.
	 * @param input input object.
	 * @return Current error correcntion level.
	 */
	static QRecLevel QRinput_getErrorCorrectionLevel(QRinput *input);

	/**
	 * Set error correction level of the QR code that is to be encoded.
	 * This function cannot be applied to Micro QR Code.
	 * @param input input object.
	 * @param level Error correction level.
	 * @retval 0 success.
	 * @retval -1 invalid argument.
	 */
	static int QRinput_setErrorCorrectionLevel(QRinput *input, QRecLevel level);

	/**
	 * Set version and error correction level of the QR code at once.
	 * This function is recommened for Micro QR Code.
	 * @param input input object.
	 * @param version version number (0 = auto)
	 * @param level Error correction level.
	 * @retval 0 success.
	 * @retval -1 invalid argument.
	 */
	static int QRinput_setVersionAndErrorCorrectionLevel(QRinput *input, int version, QRecLevel level);

	/**
	 * Free the input object.
	 * All of data chunks in the input object are freed too.
	 * @param input input object.
	 */
	static void QRinput_free(QRinput *input);

	/**
	 * Validate the input data.
	 * @param mode encoding mode.
	 * @param size size of data (byte).
	 * @param data a pointer to the memory area of the input data.
	 * @retval 0 success.
	 * @retval -1 invalid arguments.
	 */
	static int QRinput_check(QRencodeMode mode, int size, const unsigned char *data);


	/**
	 * Instantiate a set of input data object.
	 * @return an instance of QRinput_Struct. On error, NULL is returned and errno
	 *         is set to indicate the error.
	 * @throw ENOMEM unable to allocate memory.
	 */
	static QRinput_Struct *QRinput_Struct_new(void);

	/**
	 * Set parity of structured symbols.
	 * @param s structured input object.
	 * @param parity parity of s.
	 */
	static void QRinput_Struct_setParity(QRinput_Struct *s, unsigned char parity);

	/**
	 * Append a QRinput object to the set. QRinput created by QRinput_newMQR()
	 * will be rejected.
	 * @warning never append the same QRinput object twice or more.
	 * @param s structured input object.
	 * @param input an input object.
	 * @retval >0 number of input objects in the structure.
	 * @retval -1 an error occurred. See Exceptions for the details.
	 * @throw ENOMEM unable to allocate memory.
	 * @throw EINVAL invalid arguments.
	 */
	static int QRinput_Struct_appendInput(QRinput_Struct *s, QRinput *input);

	/**
	 * Free all of QRinput in the set.
	 * @param s a structured input object.
	 */
	static void QRinput_Struct_free(QRinput_Struct *s);

	/**
	 * Split a QRinput to QRinput_Struct. It calculates a parity, set it, then
	 * insert structured-append headers. QRinput created by QRinput_newMQR() will
	 * be rejected.
	 * @param input input object. Version number and error correction level must be
	 *        set.
	 * @return a set of input data. On error, NULL is returned, and errno is set
	 *         to indicate the error. See Exceptions for the details.
	 * @throw ERANGE input data is too large.
	 * @throw EINVAL invalid input data.
	 * @throw ENOMEM unable to allocate memory.
	 */
	static QRinput_Struct *QRinput_splitQRinputToStruct(QRinput *input);

	/**
	 * Insert structured-append headers to the input structure. It calculates
	 * a parity and set it if the parity is not set yet.
	 * @param s input structure
	 * @retval 0 success.
	 * @retval -1 an error occurred and errno is set to indeicate the error.
	 *            See Execptions for the details.
	 * @throw EINVAL invalid input object.
	 * @throw ENOMEM unable to allocate memory.
	 */
	static int QRinput_Struct_insertStructuredAppendHeaders(QRinput_Struct *s);

	/**
	 * Set FNC1-1st position flag.
	 */
	static int QRinput_setFNC1First(QRinput *input);

	/**
	 * Set FNC1-2nd position flag and application identifier.
	 */
	static int QRinput_setFNC1Second(QRinput *input, unsigned char appid);


	/**
	 * Create a symbol from the input data.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 * @param input input data.
	 * @return an instance of QRcode class. The version of the result QRcode may
	 *         be larger than the designated version. On error, NULL is returned,
	 *         and errno is set to indicate the error. See Exceptions for the
	 *         details.
	 * @throw EINVAL invalid input object.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 */
	static QRcode *QRcode_encodeInput(QRinput *input);

	/**
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
	static QRcode *QRcode_encodeString(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);

	/**
	 * Same to QRcode_encodeString(), but encode whole data in 8-bit mode.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 */
	static QRcode *QRcode_encodeString8bit(const char *string, int version, QRecLevel level);

	/**
	 * Micro QR Code version of QRcode_encodeString().
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 */
	static QRcode *QRcode_encodeStringMQR(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);

	/**
	 * Micro QR Code version of QRcode_encodeString8bit().
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 */
	static QRcode *QRcode_encodeString8bitMQR(const char *string, int version, QRecLevel level);

	/**
	 * Encode byte stream (may include '\0') in 8-bit mode.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 * @param size size of the input data.
	 * @param data input data.
	 * @param version version of the symbol. If 0, the library chooses the minimum
	 *                version for the given input data.
	 * @param level error correction level.
	 * @throw EINVAL invalid input object.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 * @throw ERANGE input data is too large.
	 */
	static QRcode *QRcode_encodeData(int size, const unsigned char *data, int version, QRecLevel level);

	/**
	 * Micro QR Code version of QRcode_encodeData().
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 */
	static QRcode *QRcode_encodeDataMQR(int size, const unsigned char *data, int version, QRecLevel level);

	/**
	 * Free the instance of QRcode class.
	 * @param qrcode an instance of QRcode class.
	 */
	static void QRcode_free(QRcode *qrcode);

	/**
	 * Create structured symbols from the input data.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 * @param s
	 * @return a singly-linked list of QRcode.
	 */
	static QRcode_List *QRcode_encodeInputStructured(QRinput_Struct *s);

	/**
	 * Create structured symbols from the string. The library automatically parses
	 * the input string and encodes in a QR Code symbol.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 * @param string input string. It must be NUL terminated.
	 * @param version version of the symbol.
	 * @param level error correction level.
	 * @param hint tell the library how non-alphanumerical characters should be
	 *             encoded. If QR_MODE_KANJI is given, kanji characters will be
	 *             encoded as Shif-JIS characters. If QR_MODE_8 is given, all of
	 *             non-alphanumerical characters will be encoded as is. If you want
	 *             to embed UTF-8 string, choose this.
	 * @param casesensitive case-sensitive(1) or not(0).
	 * @return a singly-linked list of QRcode. On error, NULL is returned, and
	 *         errno is set to indicate the error. See Exceptions for the details.
	 * @throw EINVAL invalid input object.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 */
	static QRcode_List *QRcode_encodeStringStructured(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);

	/**
	 * Same to QRcode_encodeStringStructured(), but encode whole data in 8-bit mode.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 */
	static QRcode_List *QRcode_encodeString8bitStructured(const char *string, int version, QRecLevel level);

	/**
	 * Create structured symbols from byte stream (may include '\0'). Wholde data
	 * are encoded in 8-bit mode.
	 * @warning This function is THREAD UNSAFE when pthread is disabled.
	 * @param size size of the input data.
	 * @param data input dat.
	 * @param version version of the symbol.
	 * @param level error correction level.
	 * @return a singly-linked list of QRcode. On error, NULL is returned, and
	 *         errno is set to indicate the error. See Exceptions for the details.
	 * @throw EINVAL invalid input object.
	 * @throw ENOMEM unable to allocate memory for input objects.
	 */
	static QRcode_List *QRcode_encodeDataStructured(int size, const unsigned char *data, int version, QRecLevel level);

	/**
	 * Return the number of symbols included in a QRcode_List.
	 * @param qrlist a head entry of a QRcode_List.
	 * @return number of symbols in the list.
	 */
	static int QRcode_List_size(QRcode_List *qrlist);

	/**
	 * Free the QRcode_List.
	 * @param qrlist a head entry of a QRcode_List.
	 */
	static void QRcode_List_free(QRcode_List *qrlist);


	/******************************************************************************
	 * System utilities
	 *****************************************************************************/

	/**
	 * Return a string that identifies the library version.
	 * @param major_version
	 * @param minor_version
	 * @param micro_version
	 */
	static void QRcode_APIVersion(int *major_version, int *minor_version, int *micro_version);

	/**
	 * Return a string that identifies the library version.
	 * @return a string identifies the library version. The string is held by the
	 * library. Do NOT free it.
	 */
	static char *QRcode_APIVersionString(void);

	/**
	 * Clear all caches. This is only for debug purpose. If you are attacking a
	 * complicated memory leak bug, try this to reduce the reachable blocks record.
	 */
	static void QRcode_clearCache(void);

};

#endif
