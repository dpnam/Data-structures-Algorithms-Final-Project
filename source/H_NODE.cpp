#include "H_NODE.h"

H_NODE::H_NODE() = default;

H_NODE::~H_NODE() = default;

const unsigned char& H_NODE::getChar() const {
	return c;
}

const unsigned int& H_NODE::getFreq() const {
	return freq;
}

const short int& H_NODE::getLeft() const {
	return left;
}

const short int& H_NODE::getRight() const {
	return right;
}

const unsigned short& H_NODE::getPos() const {
	return pos;
}

void H_NODE::setChar(unsigned char item) {
	c = item;
}

void H_NODE::setPos(unsigned short item) {
	pos = item;
}

void H_NODE::initNode(unsigned char a, unsigned int f, short int l, short int r) {
	c = a;
	freq = f;
	left = l;
	right = r;
}

void H_NODE::increaseFreq(unsigned int k) {
	freq += k;
}

bool H_NODE::operator==(const H_NODE &aH_NODE) {
	return (freq == aH_NODE.freq) && (c == aH_NODE.c);
}

bool H_NODE::operator!=(const H_NODE &aH_NODE) {
	return !(*this == aH_NODE);
}

bool H_NODE::operator>(const H_NODE &aH_NODE) {
	if (freq > aH_NODE.freq)
		return true;
	else if (freq < aH_NODE.freq)
		return false;
	return (c > aH_NODE.c) ? true : false;
}

bool H_NODE::operator<(const H_NODE &aH_NODE) {
	if (freq < aH_NODE.freq)
		return true;
	else if (freq > aH_NODE.freq)
		return false;
	return (c < aH_NODE.c) ? true : false;
}

bool H_NODE::operator>=(const H_NODE &aH_NODE) {
	return (*this > aH_NODE) || (*this == aH_NODE);
}

bool H_NODE::operator<=(const H_NODE &aH_NODE) {
	return (*this < aH_NODE) || (*this == aH_NODE);
}