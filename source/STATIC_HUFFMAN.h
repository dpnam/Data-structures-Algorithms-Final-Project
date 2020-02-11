#ifndef _STATIC_HUFFMAN_H
#define _STATIC_HUFFMAN_H

#include "HUFFMAN_TREE.h"

struct ENCODE {
	string idFile;//mã nhận dạng file
	string nameInput;//tên file ban đầu
	unsigned int freq[256];
	unsigned long long sizeInput;//kích thước file ban đầu
	unsigned long long sizeEncode;//kích thước file nén
	unsigned short bitsFinal;//số bit có nghĩa trong byte cuối
	string data;//dữ liệu nén
};

class STATIC_HUFFMAN {
private:
	HUFFMAN_TREE HTree_Encode;
	HUFFMAN_TREE HTree_DeCode;

	string enCodeToStringBits(string, CODE_TABLE_ITEM *, unsigned short&);
public:
	STATIC_HUFFMAN();
	~STATIC_HUFFMAN();

	bool encode(string, ENCODE&);
	bool decode(ENCODE, ofstream&);
};

#endif