#include "stdafx.h"
#include "mBitstream.h"
#include <stdlib.h>
#include <string>


#include <iostream>
#include <iomanip>

BitStream *mBitStream::BitStream_new(void)
{
	BitStream *bstream;

	bstream = (BitStream *)malloc(sizeof(BitStream));
	if(bstream == NULL) return NULL;

	bstream->length = 0;
	bstream->data = NULL;

	return bstream;
}

int mBitStream::BitStream_allocate(BitStream *bstream, int length)
{
	unsigned char *data;

	if(bstream == NULL) {
		return -1;
	}

	data = (unsigned char *)malloc(length);
	if(data == NULL) {
		return -1;
	}

	if(bstream->data) {
		free(bstream->data);
	}
	bstream->length = length;
	bstream->data = data;

	return 0;
}

BitStream *mBitStream::BitStream_newFromNum(int bits, unsigned int num)
{
	unsigned int mask;
	int i;
	unsigned char *p;
	BitStream *bstream;

	bstream = BitStream_new();
	if(bstream == NULL) return NULL;

	if(BitStream_allocate(bstream, bits)) {
		BitStream_free(bstream);
		return NULL;
	}

	p = bstream->data;
	mask = 1 << (bits - 1);
	for(i=0; i<bits; i++) {
		if(num & mask) {
			*p = 1;
		} else {
			*p = 0;
		}
		p++;
		mask = mask >> 1;
	}

	return bstream;
}

BitStream *mBitStream::BitStream_newFromBytes(int size, unsigned char *data)
{
	unsigned char mask;
	int i, j;
	unsigned char *p;
	BitStream *bstream;

	bstream = BitStream_new();
	if(bstream == NULL) return NULL;

	if(BitStream_allocate(bstream, size * 8)) {
		BitStream_free(bstream);
		return NULL;
	}

	p = bstream->data;
	for(i=0; i<size; i++) {
		mask = 0x80;
		for(j=0; j<8; j++) {
			if(data[i] & mask) {
				*p = 1;
			} else {
				*p = 0;
			}
			p++;
			mask = mask >> 1;
		}
	}

	return bstream;
}

int mBitStream::BitStream_append(BitStream *bstream, BitStream *arg)
{
	unsigned char *data;

	if(arg == NULL) {
		return -1;
	}
	if(arg->length == 0) {
		return 0;
	}
	if(bstream->length == 0) {
		if(BitStream_allocate(bstream, arg->length)) {
			return -1;
		}
		memcpy(bstream->data, arg->data, arg->length);
		return 0;
	}

	data = (unsigned char *)malloc(bstream->length + arg->length);
	if(data == NULL) {
		return -1;
	}
	memcpy(data, bstream->data, bstream->length);
	memcpy(data + bstream->length, arg->data, arg->length);

	free(bstream->data);
	bstream->length += arg->length;
	bstream->data = data;

	return 0;
}

int mBitStream::BitStream_appendNum(BitStream *bstream, int bits, unsigned int num)
{
	BitStream *b;
	int ret;

	if(bits == 0) return 0;

	b = BitStream_newFromNum(bits, num);
	if(b == NULL) return -1;

	ret = BitStream_append(bstream, b);
	BitStream_free(b);

	return ret;
}

int mBitStream::BitStream_appendBytes(BitStream *bstream, int size, unsigned char *data)
{
	BitStream *b;
	int ret;

	if(size == 0) return 0;

	b = BitStream_newFromBytes(size, data);
	if(b == NULL) return -1;

	ret = BitStream_append(bstream, b);
	BitStream_free(b);

	return ret;
}

unsigned char *mBitStream::BitStream_toByte(BitStream *bstream)
{
	int i, j, size, bytes;
	unsigned char *data, v;
	unsigned char *p;

	size = BitStream_size(bstream);
	if(size == 0) {
		return NULL;
	}
	data = (unsigned char *)malloc((size + 7) / 8);
	if(data == NULL) {
		return NULL;
	}

	bytes = size  / 8;

	p = bstream->data;
	for(i=0; i<bytes; i++) {
		v = 0;
		for(j=0; j<8; j++) {
			v = v << 1;
			v |= *p;
			p++;
		}
		data[i] = v;
	}
	if(size & 7) {
		v = 0;
		for(j=0; j<(size & 7); j++) {
			v = v << 1;
			v |= *p;
			p++;
		}
		data[bytes] = v;
	}

	return data;
}

void mBitStream::BitStream_free(BitStream *bstream)
{
	if(bstream != NULL) {
		free(bstream->data);
		free(bstream);
	}
}


void mBitStream::ShowBitStream(BitStream *bstream)
{	
	using namespace std;

	cout << "bits:     ";
	for (int i = 0; i < 16; i++)
	{
		cout << setw(3) << hex << i;
	}
	
	int iiiii = 0;
	const unsigned char *source = bstream->data;
	int iindex = 0;
	while (iindex < bstream->length)
	{
		if (iiiii % 16 == 0)
		{
			cout<< endl;
			cout << "OX" << setw(6) << setfill('0') << hex << iiiii / 16 <<"  ";
			
			cout << setfill(' ');	
		}
		else
		{
			cout << setfill(' ');
		}

		if ((*source)&1)
		{
			cout << setw(3) << '1';
		}
		else
		{
			cout << setw(3) << '0';
		}
		source++;
		iiiii++;

		iindex++;
	}

	cout<< endl << endl;
}


void mBitStream::ShowByteStream(unsigned char * source, int lenght)
{
	using namespace std;
	unsigned char *p = source;

	if (p == nullptr)
	{
		return;
	}

	cout << "couvert int numbers: " <<endl;
	int len = 0;

	while (len < lenght)
	{	
		cout << setw(4) << (int)*p;
		p++;
		len++;
	}

	cout << endl << endl;
}


void mBitStream::ShowMatrix(unsigned char * source1, int width)
{
	using namespace std;

	cout << "data:     ";
	for (int i = 0; i < width; i++)
	{
		cout << setw(2) << hex << i;
	}
	
	int iiiii = 0;
	const unsigned char *source = source1;
	int iindex = 0;
	while (iindex < width * width)
	{
		if (iiiii % width == 0)
		{
			cout<< endl;
			cout << "OX" << setw(6) << setfill('0') << hex << iiiii / width <<"  ";
			
			cout << setfill(' ');	
		}
		else
		{
			cout << setfill(' ');
		}

		/*if ((*source)&1)
		{
			cout << setw(4) << '1';
		}
		else
		{
			cout << setw(4) << '0';
		}*/
		cout << setw(2) <<(int)*source;

		source++;
		iiiii++;

		iindex++;
	}

	cout<< endl << endl;
}