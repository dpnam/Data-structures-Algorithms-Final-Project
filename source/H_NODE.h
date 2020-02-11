#ifndef _H_NODE_H
#define _H_NODE_H

class H_NODE {
private:
	unsigned char c;
	unsigned int freq;
	short int left;
	short int right;
	unsigned short pos;
public:
	H_NODE();
	~H_NODE();

	const unsigned char& getChar()const;
	const unsigned int& getFreq() const;
	const short int& getLeft() const;
	const short int& getRight() const;
	const unsigned short& getPos() const;

	void setChar(unsigned char);
	void setPos(unsigned short);

	void initNode(unsigned char, unsigned int, short int, short int);
	void increaseFreq(unsigned int k = 1);

	bool operator== (const H_NODE&);
	bool operator!= (const H_NODE&);
	bool operator> (const H_NODE&);
	bool operator< (const H_NODE&);
	bool operator>= (const H_NODE&);
	bool operator<= (const H_NODE&);
};

#endif