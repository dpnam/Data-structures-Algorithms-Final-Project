#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "STATIC_HUFFMAN.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <iomanip>

class PROGRAM {
private:
	ENCODE *EN;
	unsigned int numberFile = 0;
	STATIC_HUFFMAN SH;

	void menuMain();
	bool enCode_FullFiles();
	bool printInfor_FilesDeCode();
	bool deCode_FullFiles();
	bool deCode_Files();
	bool loadDataEnCode(string);
public:
	PROGRAM();
	~PROGRAM();
	void run();
};

#endif