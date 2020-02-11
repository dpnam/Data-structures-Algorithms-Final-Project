#include "PROGRAM.h"

PROGRAM::PROGRAM() = default;

PROGRAM::~PROGRAM() {
	if (!EN) {
		delete[] EN;
		EN = nullptr;
	}
	SH.~STATIC_HUFFMAN();
}

void PROGRAM::menuMain() {
	cout << "Nhan phim 1. Nen cac file trong forder" << endl;
	cout << "Nhan phim 2. Xem noi dung file da nen" << endl;
	cout << "Nhan phim 3. Giai nen tat ca cac file trong tap tin nen" << endl;
	cout << "Nhan phim 4. Chon va gia nen cac file rieng le" << endl;
	cout << "Nhan phim 5. Thoat!" << endl;
	cout << "Nhap lua chon: ";
}

bool PROGRAM::enCode_FullFiles()
{
	system("cls");
	cout << "<Lua chon 1. Nen cac file trong forder>" << endl;
	cin.ignore();

	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	//nhập folder cần nén
	string folder_in = "";
	cout << "Nhap vao folder chua cac file can nen: ";
	getline(cin, folder_in, '\n');
	folder_in += "\\*";
	int len = folder_in.size();

	//lấy tên đường dẫn đến folder để đọc
	for (int i = 0; i < len; i++)
		szDir[i] = (char)folder_in[i];
	szDir[len] = NULL;

	//kiểm tra folder có tồn tại hay không
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		EN = nullptr;
		return false;
	}

	//nhập file chứa dữ liệu nén
	string file_out = "";
	cout << "Nhap duong dan + ten file nen: ";
	getline(cin, file_out, '\n');

	//đếm số file có thể nén
	numberFile = 0;
	do {
		if (INVALID_HANDLE_VALUE == hFind) break;
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			numberFile++;
	} while (FindNextFile(hFind, &ffd) != 0);

	cout << "So luong file: " << numberFile << endl;
	cout << "Vui long doi vai phut!" << endl;

	//thực hiện thao tác nén các file trong folder
	hFind = FindFirstFile(szDir, &ffd);
	EN = new ENCODE[numberFile];
	int i = 0;
	folder_in.pop_back();
	do {
		if (INVALID_HANDLE_VALUE == hFind) break;
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			string linkInput = "";
			linkInput += folder_in;
			int n = wcslen(ffd.cFileName);
			for (int j = 0; j < n; j++)
				linkInput.push_back((char)ffd.cFileName[j]);

			SH.encode(linkInput, EN[i]);

			EN[i].nameInput = "";
			for (int j = 0; j < n; j++)
				EN[i].nameInput.push_back((char)ffd.cFileName[j]);

			i++;
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	//tiến hành ghi dữ liệu nén ra file_out
	fstream f(file_out, ios::out | ios::binary);
	if (f.fail()) return false;
	f << "dpnam";// mã nhận dạng file
	f << ' ' << numberFile;
	f << ' ';

	for (int i = 0; i < this->numberFile; i++) {
		int _dem = 0;
		for (int j = 0; j < 256; j++)
			if (EN[i].freq[j] != 0) _dem++;
		f << _dem << ' ';
	}
	f << endl;

	for (int i = 0; i < this->numberFile; i++) {
		f << '#' << i + 1 << endl;
		for (int j = 0; j < 256; j++)
			if (EN[i].freq[j] != 0) f << (unsigned char)j << '_' << EN[i].freq[j] << ' ';
		f << endl;
	}
	f << endl;

	for (int i = 0; i < this->numberFile; i++)
		f << i + 1 << '|' << EN[i].nameInput << '|' << EN[i].sizeInput << '|' << EN[i].sizeEncode << '|' << EN[i].bitsFinal << endl;
	f << endl;

	for (int i = 0; i < this->numberFile; i++)
		f << EN[i].data << endl;
	f.close();

	cout << "THANH CONG!" << endl;
	return true;
}

bool PROGRAM::printInfor_FilesDeCode()
{
	system("cls");
	cout << "<Lua chon 2. Xem noi dung file da nen>" << endl;
	cin.ignore();

	char _check;
	cout << "Ban muon load data nen moi y/n: ";
	cin >> _check;
	cin.ignore();
	if (_check == 'y' || _check == 'Y') {
		string linkEncode = "";
		cout << "Nhap file nen: ";
		getline(cin, linkEncode, '\n');
		cout << "Vui long doi vai phut Load Data!" << endl;
		if (!loadDataEnCode(linkEncode)) {
			cout << "THAT BAI!!!" << endl;
			return false;
		}
		else cout << "THANH CONG!" << endl;
	}

	if (!EN || numberFile == 0) {
		cout << "Du lieu khong co san!" << endl;
		return false;
	}

	cout << setw(5) << left << "STT  " << setw(40) << left << "Ten File" << setw(25) << left << "Size truoc khi nen" << setw(10) << left << "Size sau nen" << endl;
	for (int i = 0; i < this->numberFile; i++)
		cout << setw(5) << left << i + 1 << setw(40) << left << EN[i].nameInput << setw(25) << left << EN[i].sizeInput << setw(12) << left << EN[i].sizeEncode << " (bytes)" << endl;

	return true;
}

bool PROGRAM::deCode_FullFiles() {
	system("cls");
	cout << "<Lua chon 3. Giai nen tat ca cac file trong tap tin nen>" << endl;
	cin.ignore();

	char _check;
	cout << "Ban muon load data nen moi y/n: ";
	cin >> _check;
	cin.ignore();
	if (_check == 'y' || _check == 'Y') {
		string linkEncode = "";
		cout << "Nhap file nen: ";
		getline(cin, linkEncode, '\n');
		cout << "Vui long doi vai phut Load Data!" << endl;
		if (!loadDataEnCode(linkEncode)) {
			cout << "THAT BAI!!!" << endl;
			return false;
		}
		else cout << "THANH CONG!" << endl;
	}

	if (!EN || numberFile == 0) {
		cout << "Du lieu khong co san!" << endl;
		return false;
	}

	string folder_out;
	cout << "Nhap folder chua file giai nen: ";
	getline(cin, folder_out, '\n');

	cout << "Vui long doi vai phut!" << endl;
	TCHAR buffer[MAX_PATH];
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, buffer);

	int len = folder_out.size();
	for (int i = 0; i < len; i++)
		szDir[i] = (char)folder_out[i];
	szDir[len] = NULL;

	CreateDirectory(szDir, NULL);
	SetCurrentDirectory(szDir);

	for (int i = 0; i < this->numberFile; i++) {
		ofstream f(EN[i].nameInput, ios::out | ios::binary);
		if (!SH.decode(EN[i], f)) cout << EN[i].nameInput << ": FALSE" << endl;
		f.close();
	}
	SetCurrentDirectory(buffer);
	cout << "THANH CONG!" << endl;
	return true;
}

bool PROGRAM::deCode_Files() {
	system("cls");
	cout << "<Lua chon 4. Chon va gia nen cac file rieng le>" << endl;
	cin.ignore();

	char _check;
	cout << "Ban muon load data nen moi y/n: ";
	cin >> _check;
	cin.ignore();
	if (_check == 'y' || _check == 'Y') {
		string linkEncode = "";
		cout << "Nhap file nen: ";
		getline(cin, linkEncode, '\n');
		cout << "Vui long doi vai phut Load Data!" << endl;
		if (!loadDataEnCode(linkEncode)) {
			cout << "THAT BAI!!!" << endl;
			return false;
		}
		else cout << "THANH CONG!" << endl;
	}

	if (!EN || numberFile == 0) {
		cout << "Du lieu khong co san!" << endl;
		return false;
	}


	string luaChon = "";
	cout << "Nhap thu tu cac file muon nen: ";
	getline(cin, luaChon, '\n');

	string folder_out;
	cout << "Nhap folder chua file giai nen: ";
	getline(cin, folder_out, '\n');

	cout << "Vui long doi vai phut!" << endl;

	TCHAR buffer[MAX_PATH];
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];

	int len = folder_out.size();
	for (int i = 0; i < len; i++)
		szDir[i] = (char)folder_out[i];
	szDir[len] = NULL;

	GetCurrentDirectory(MAX_PATH, buffer);

	CreateDirectory(szDir, NULL);
	SetCurrentDirectory(szDir);

	for (int i = 0; i < luaChon.length(); i++) {

		//tách các chuỗi số ra từ chuỗi nhập ban đầu
		string tmpStr = "";
		while (luaChon[i] > '0' && luaChon[i] < '9' && i < luaChon.length()) {
			tmpStr.push_back(luaChon[i]);
			i++;
		}

		char* number = new char[tmpStr.length() + 1];
		for (int j = 0; j < tmpStr.length(); j++)
			number[j] = tmpStr[j];
		number[tmpStr.length()] = NULL;

		//chuyển chuỗi ký tự thành số
		int pos = atoi(number);
		pos--;
		if (pos <= numberFile) {
			ofstream f(EN[pos].nameInput, ios::out | ios::binary);
			if (!SH.decode(EN[pos], f)) cout << EN[pos].nameInput << ": FALSE" << endl;
			f.close();
		}
		else cout << "File thu " << pos << " Khong ton tai!" << endl;
	}

	SetCurrentDirectory(buffer);
	cout << "THANH CONG!" << endl;
	return true;
}

bool PROGRAM::loadDataEnCode(string linkEncode) {
	fstream f(linkEncode, ios::in | ios::binary);
	if (f.fail()) return false;

	string _idFile = "";
	for (int i = 0; i < 5; i++) {
		char tmp;
		f >> tmp;
		_idFile.push_back(tmp);
	}

	if (_idFile != "dpnam") return false;//kiểm tra mã nhận dạng file

	f.seekg(_idFile.length() + 1, ios::beg);
	f >> numberFile;

	if (EN) delete[] EN;
	EN = new ENCODE[numberFile];

	for (int i = 0; i < numberFile; i++)
		EN[i].idFile = _idFile;

	f.seekg(1, ios::cur);

	int *_countChar = new int[numberFile];
	for (int i = 0; i < numberFile; i++) {
		f >> _countChar[i];
		f.seekg(1, ios::cur);
	}

	f.seekg(1, ios::cur);

	for (int i = 0; i < numberFile; i++)
		for (int j = 0; j < 256; j++)
			EN[i].freq[j] = 0;

	//đọc bảng tần số ký tự tương ứng cho từng dữ liệu nén
	f.seekg(1, ios::cur);
	for (int i = 0; i < numberFile; i++) {
		int stt;//số thứ tự
		f >> stt;
		f.seekg(1, ios::cur);
		for (int j = 0; j < _countChar[i]; j++) {
			unsigned char c = 0x00;
			f >> noskipws >> c;
			f.seekg(1, ios::cur);
			f >> EN[i].freq[c];
			f.seekg(1, ios::cur);
		}
		f.seekg(2, ios::cur);
	}

	for (int i = 0; i < numberFile; i++) {
		int stt;//số thứ tự
		f >> stt;
		f.seekg(1, ios::cur);
		getline(f, EN[i].nameInput, '|');
		f >> EN[i].sizeInput;
		f.seekg(1, ios::cur);
		f >> EN[i].sizeEncode;
		f.seekg(1, ios::cur);
		f >> EN[i].bitsFinal;
		f.seekg(1, ios::cur);
	}

	f.seekg(1, ios::cur);

	for (int i = 0; i < numberFile; i++) {
		EN[i].data = "";
		for (int j = 0; j < EN[i].sizeEncode; j++) {
			unsigned char c;
			f >> noskipws >> c;
			EN[i].data.push_back(c);
		}
		EN[i].data[EN[i].sizeEncode] = NULL;
		f.seekg(1, ios::cur);
	}

	f.close();

	return true;
}

void PROGRAM::run()
{
	while (1) {
		system("cls");
		int _chon;
		menuMain();
		cin >> _chon;
		if (_chon == 1) enCode_FullFiles();
		else if (_chon == 2)printInfor_FilesDeCode();
		else if (_chon == 3)deCode_FullFiles();
		else if (_chon == 4)deCode_Files();
		else return;

		char _quayve;
		cout << "Quay ve menu chinh (y/n): ";
		cin >> _quayve;
		if (_quayve != 'y'&&_quayve != 'Y') return;
	}
}