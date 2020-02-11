#include "STATIC_HUFFMAN.h"

STATIC_HUFFMAN::STATIC_HUFFMAN() = default;

STATIC_HUFFMAN::~STATIC_HUFFMAN() = default;


string STATIC_HUFFMAN::enCodeToStringBits(string input, CODE_TABLE_ITEM *HuffCodeTable, unsigned short &bitsFinal) {
	fstream fIn(input, ios::in | ios::binary);

	string enCodeBits = "";
	while (1) {
		unsigned char a;
		fIn >> noskipws >> a;
		if (fIn.eof()) break;
		for (int j = 0; j < HuffCodeTable[a].nBitLen; j++)
			enCodeBits.push_back(HuffCodeTable[a].bits[j]);
	}
	fIn.close();
	bitsFinal = enCodeBits.size() % 8;
	if (bitsFinal == 0) bitsFinal = 8;
	for (int i = 0; i < 8 - bitsFinal; i++)
		enCodeBits.push_back('0');

	return enCodeBits;
}

bool STATIC_HUFFMAN::encode(string linkInput, ENCODE &EN)
{
	//Bước 1: Duyệt file, lập bảng thống kê tần số xuất hiện của mỗi loại ký tự
	if (!HTree_Encode.countChar(linkInput, EN.freq, EN.sizeInput)) 	return false;


	//Bước 2: Phát sinh cây Huffman dựa vào bảng thống kê
	HTree_Encode.buildTree();

	//Bước 3: Từ cây Huffman phát sinh bảng mã bit cho từng loại ký tự
	CODE_TABLE_ITEM HuffCodeTable[MAX_CODETABLE_ITEMS];
	HTree_Encode.createBitcode(HuffCodeTable);

	//Bước 4: Duyệt file, thay thế các ký tự bằng mã bit tương ứng
	string enCodeBits = enCodeToStringBits(linkInput, HuffCodeTable, EN.bitsFinal);
	//cout << enCodeBits << endl;

	EN.nameInput = linkInput;
	EN.idFile = "dpnam";

	string enCodeData = "";
	int sizeEncode = enCodeBits.length();
	unsigned char out = 0x00;
	for (int i = 0; i < sizeEncode; i++) {
		if (enCodeBits[i] == '1') out = out | (1 << (7 - (i % 8)));
		if ((i + 1) % 8 == 0) {
			enCodeData.push_back(out);
			out = 0x00;
		}
	}

	EN.data = enCodeData;
	EN.sizeEncode = enCodeData.length();

	return true;
}

bool STATIC_HUFFMAN::decode(ENCODE EN, ofstream &f)
{
	if (EN.idFile != "dpnam") return false;

	HTree_DeCode.loadFreq(EN.freq);
	HTree_DeCode.buildTree();

	string stringBits = "";
	for (int i = 0; i < EN.sizeEncode; i++) {
		unsigned char out = EN.data[i];
		for (int j = 0; j < 8; j++) {
			if ((out >> (7 - (j % 8))) & 1)
				stringBits += '1';
			else stringBits += '0';
		}
	}

	for (int i = 0; i < 8 - EN.bitsFinal; i++)
		stringBits.pop_back();

	string dataDecode = "";
	HTree_DeCode.traverseTreeDecode(stringBits, dataDecode);


	if (EN.sizeInput != dataDecode.length()) return false;//check sum

	f << dataDecode;

	return true;
	
}