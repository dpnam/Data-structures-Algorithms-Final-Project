#include "HUFFMAN_TREE.h"

HUFFMAN_TREE::HUFFMAN_TREE() {
	for (int i = 0; i < 256; i++) {
		HuffTree[i].initNode(i, 0, -1, -1);
		HuffTree[i].setPos(i);
	}
	root = -1;
}

HUFFMAN_TREE::~HUFFMAN_TREE() {
	root = -1;
}

bool HUFFMAN_TREE::countChar(string filename, unsigned int *freq, unsigned long long &sizeInput) {

	fstream f(filename, ios::in | ios::binary);
	if (f.fail()) return false;

	while (1) {
		unsigned char a;
		f >> noskipws >> a;
		if (f.eof()) break;
		HuffTree[a].increaseFreq(1);
	}
	f.close();

	for (int i = 0; i < 256; i++)
		freq[i] = HuffTree[i].getFreq();

	fstream fIn(filename, ios::in | ios::binary);
	fIn.seekg(0, ios::end);
	sizeInput = fIn.tellg();
	fIn.close();

	return true;
}

void HUFFMAN_TREE::buildTree() {
	PRIORITY_QUEUE<H_NODE> q(256);

	unsigned short i;
	// đưa các node lá vào priority queue (max 256 node lá)
	for (i = 0; i < 256; i++)
		if (HuffTree[i].getFreq() != 0)
			q.enQueue(HuffTree[i]);

	i = 256; // các node nhánh được lưu từ vị trí 256+
	while (1) {
		H_NODE a, b;

		q.deQueue(a);

		if (q.deQueue(b) == false) {
			root = a.getPos();
			break;
		}
		HuffTree[i].initNode(a.getChar(), a.getFreq() + b.getFreq(), a.getPos(), b.getPos());
		HuffTree[i].setPos(i);
		q.enQueue(HuffTree[i]);
		i++;
	}
}

void HUFFMAN_TREE::traverseTreeEncode(CODE_TABLE_ITEM *HuffCodeTable, short int currNode, char *bitCode, int bitCodeLen) {
	if (currNode == -1) return;

	if (HuffTree[currNode].getLeft() == -1 && HuffTree[currNode].getRight() == -1) { // nếu node hiện tại là node lá
		HuffCodeTable[currNode].nBitLen = bitCodeLen;
		HuffCodeTable[currNode].bits = new char[bitCodeLen + 1];
		for (int i = 0; i < bitCodeLen; i++)
			HuffCodeTable[currNode].bits[i] = bitCode[i]; // thêm mã bit của node đó vào HuffCodeTable
		HuffCodeTable[currNode].bits[bitCodeLen] = NULL;
		return;
	}
	bitCode[bitCodeLen] = '0'; // thêm 1 bit '0' vào dãy bitCode, tại vị trí bitCodeLen
	traverseTreeEncode(HuffCodeTable, HuffTree[currNode].getLeft(), bitCode, bitCodeLen + 1); // duyệt trái

	bitCode[bitCodeLen] = '1'; // thêm 1 bit '1' vào dãy bitCode, tại vị trí bitCodeLen
	traverseTreeEncode(HuffCodeTable, HuffTree[currNode].getRight(), bitCode, bitCodeLen + 1); // duyệt phải
}

void HUFFMAN_TREE::loadFreq(unsigned int *freq)
{
	for (int i = 0; i < 256; i++)
		HuffTree[i].initNode((unsigned char)i, freq[i], -1, -1);
}

void HUFFMAN_TREE::traverseTreeDecode(string BitsCode, string &dataDecode) {
	int currNode = root;
	int sizeBitCode = BitsCode.length();
	for (int i = 0; i <= sizeBitCode; i++) {
		if (HuffTree[currNode].getLeft() == -1 && HuffTree[currNode].getRight() == -1) {
			dataDecode.push_back(HuffTree[currNode].getChar());
			currNode = root;
		}
		if (BitsCode[i] == '0') currNode = HuffTree[currNode].getLeft();
		if (BitsCode[i] == '1') currNode = HuffTree[currNode].getRight();
	}
}

void HUFFMAN_TREE::createBitcode(CODE_TABLE_ITEM *HuffCodeTable) {
	for (int i = 0; i < 256; i++)
		HuffCodeTable[i].nBitLen = 0;

	char bitCode[MAX_BIT_LEN];
	int bitCodeLen = 0;

	traverseTreeEncode(HuffCodeTable, root, bitCode, bitCodeLen);
}