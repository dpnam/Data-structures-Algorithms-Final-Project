#ifndef _HUFFFMAN_TREE_H
#define _HUFFFMAN_TREE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "PRIORITY_QUEUE.h"
#include "H_NODE.h"
#define MAX_NODES 511

#define MAX_BIT_LEN 256
#define MAX_CODETABLE_ITEMS 256

struct CODE_TABLE_ITEM {
	char *bits; // Mã bit của ký tự
	unsigned char nBitLen; // số bit thật sự sử dụng
};

class HUFFMAN_TREE {
private:
	H_NODE HuffTree[MAX_NODES];
	short int root;
private:
	void traverseTreeEncode(CODE_TABLE_ITEM*, short int, char*, int);//duyệt cây để tạo bảng bit code phục vụ cho việc nén
public:
	HUFFMAN_TREE();
	~HUFFMAN_TREE();

	//một số hàm dùng chung cho cả nén và giải nén
	void buildTree();//xây dựng cây huffman

	//một số hàm chỉ dùng cho việc nén
	bool countChar(string, unsigned int*, unsigned long long&);//thống kê tần số xuất hiện của mỗi ký tự từ file input
	void createBitcode(CODE_TABLE_ITEM*);//Tạo ra bảng bit code

	//một số hàm chỉ dùng cho việc giải nén
	void loadFreq(unsigned int*);
	void traverseTreeDecode(string, string&);//duyệt cây để giải nén
};

#endif