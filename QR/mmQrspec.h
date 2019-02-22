#ifndef _M_MQRSPEC_H_
#define _M_MQRSPEC_H_

#include "mQrencode.h"

/******************************************************************************
 * Version and capacity
 *****************************************************************************/

/**
 * Maximum width of a symbol
 */
#define MQRSPEC_WIDTH_MAX 17


class mmQrspec
{
public:
	
/**
 * Return maximum data code length (bits) for the version.
 * @param version
 * @param level
 * @return maximum size (bits)
 */
static int MQRspec_getDataLengthBit(int version, QRecLevel level);

/**
 * Return maximum data code length (bytes) for the version.
 * @param version
 * @param level
 * @return maximum size (bytes)
 */
static int MQRspec_getDataLength(int version, QRecLevel level);

/**
 * Return maximum error correction code length (bytes) for the version.
 * @param version
 * @param level
 * @return ECC size (bytes)
 */
static int MQRspec_getECCLength(int version, QRecLevel level);

/**
 * Return a version number that satisfies the input code length.
 * @param size input code length (byte)
 * @param level
 * @return version number
 */
static int MQRspec_getMinimumVersion(int size, QRecLevel level);

/**
 * Return the width of the symbol for the version.
 * @param version
 * @return width
 */
static int MQRspec_getWidth(int version);

/**
 * Return the numer of remainder bits.
 * @param version
 * @return number of remainder bits
 */
static int MQRspec_getRemainder(int version);

/******************************************************************************
 * Length indicator
 *****************************************************************************/

/**
 * Return the size of lenght indicator for the mode and version.
 * @param mode
 * @param version
 * @return the size of the appropriate length indicator (bits).
 */
static int MQRspec_lengthIndicator(QRencodeMode mode, int version);

/**
 * Return the maximum length for the mode and version.
 * @param mode
 * @param version
 * @return the maximum length (bytes)
 */
static int MQRspec_maximumWords(QRencodeMode mode, int version);

/******************************************************************************
 * Version information pattern
 *****************************************************************************/

/**
 * Return BCH encoded version information pattern that is used for the symbol
 * of version 7 or greater. Use lower 18 bits.
 * @param version
 * @return BCH encoded version information pattern
 */
static unsigned int MQRspec_getVersionPattern(int version);

/******************************************************************************
 * Format information
 *****************************************************************************/

/**
 * Return BCH encoded format information pattern.
 * @param mask
 * @param version
 * @param level
 * @return BCH encoded format information pattern
 */
static unsigned int MQRspec_getFormatInfo(int mask, int version, QRecLevel level);

/******************************************************************************
 * Frame
 *****************************************************************************/

/**
 * Return a copy of initialized frame.
 * When the same version is requested twice or more, a copy of cached frame
 * is returned.
 * @param version
 * @return Array of unsigned char. You can free it by free().
 */
static unsigned char *MQRspec_newFrame(int version);

/**
 * Clear the frame cache. Typically for debug.
 */
static void MQRspec_clearCache(void);

};

/******************************************************************************
 * Mode indicator
 *****************************************************************************/

/**
 * Mode indicator. See Table 2 in Appendix 1 of JIS X0510:2004, pp.107.
 */
#define MQRSPEC_MODEID_NUM       0
#define MQRSPEC_MODEID_AN        1
#define MQRSPEC_MODEID_8         2
#define MQRSPEC_MODEID_KANJI     3

#endif