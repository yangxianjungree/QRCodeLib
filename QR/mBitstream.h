#pragma once 
#ifndef _M_BITSTREAM_H_
#define _M_BITSTREAM_H_

#define BitStream_size(__bstream__) (__bstream__->length)

typedef struct
{
	int length;
	unsigned char *data;
} BitStream;

class mBitStream
{
public:
	static BitStream *BitStream_new(void);
	static int BitStream_append(BitStream *bstream, BitStream *arg);
	static int BitStream_appendNum(BitStream *bstream, int bits, unsigned int num);
	static int BitStream_appendBytes(BitStream *bstream, int size, unsigned char *data);
	static unsigned char *BitStream_toByte(BitStream *bstream);
	static void BitStream_free(BitStream *bstream);
	static void ShowBitStream(BitStream *bstream);
	static void ShowByteStream(unsigned char * source, int length);
	static void ShowMatrix(unsigned char * source, int width);
private:
	static int BitStream_allocate(BitStream *bstream, int length);
	static BitStream *BitStream_newFromNum(int bits, unsigned int num);
	static BitStream *BitStream_newFromBytes(int size, unsigned char *data);
};


#endif _M_BITSTREAM_H_