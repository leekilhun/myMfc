#pragma once
#include "pch.h"


#include <string>
#include <fstream>

#include <iostream>
#include <stack>
#include <typeinfo>
#include "performanceTimer.h"
/****************************************************
 * �ۼ��� : �̱���
 * ��  �� : ���� �޸� ���ڵ�
 * 20211209_v0  (�ۼ�)
 *  1. virtualRecordSet �߰�
 ****************************************************/
#define VIRTUAL_RS_CHAR_MAX  256


 //using std::string;
 //#pragma warning(disable: 6385)
using namespace std;




enum class ValType
{
	none,
	String,
	Integer,
	Double,
	Boolian,

	type_max
};

/// <summary>
	/// Char ���ڿ��� Tchar���ڿ��� ��ȯ
	/// </summary>
	/// <param name="str">Char ���ڿ�</param>
	/// <param name="ret_str">Tchar���ڿ��� ��ȯ�ּ�</param>
	/// <returns>���� �� </returns>
inline int vrCharToLPTSTR(LPSTR str, LPTSTR ret_str, int length) {
	if (strlen(str) == 0)
		return 0;
	if (_tcslen(ret_str) != 0)
		memset(ret_str, 0x00, length);   /* _tcslen(ret_str) * sizeof(TCHAR));*/
	int ret = 0;

#if defined(UNICODE) || defined(_UNICODE)
	//int nLen = (int)_tcslen(ret_str) * (int)sizeof(TCHAR);
	ret = MultiByteToWideChar(
		CP_MACCP,
		0,
		str,
		-1,
		ret_str,
		length
	);
#else
	int nLen = strlen(str) + 1;
	_tcscpy(ret_str, str);
	ret = nLen;
#endif
	return ret;
}


//
// TCHAR �� CString
//
inline CString vrTCHARToCString(TCHAR* str) {
	CString cStr;
	cStr.Format(_T("%s"), str);
	return cStr;
}


/// <summary>
/// TCHAR ���ڿ� �� Char ���ڿ�
/// </summary>
/// <param name="str"></param>
/// <param name="ret_str"></param>
/// <returns></returns>
inline int vrTcharToLPSTR(LPTSTR str, LPSTR ret_str, int length) {
	int ret = 0;
	if (_tcslen(str) == 0)
		return 0;
	if (length != 0)
		memset(ret_str, 0x00, length * sizeof(CHAR));

#if defined(UNICODE) || defined(_UNICODE)
	ret = WideCharToMultiByte(
		CP_ACP,
		0,
		str,
		-1,
		ret_str,
		length + 1,
		NULL,
		NULL
	);
#else
	strcpy(ret_str, str);

#endif
	return ret;
}





template <typename T>
class Vector
{
	/****************************************************
	*	data
	****************************************************/
	T* data;
	int capacity;
	int length;

	/****************************************************
	*	Constructor
	****************************************************/
public:
	Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {	}

	virtual ~Vector() {
		if (data) {
			delete[] data;
		}
	}

	/****************************************************
	 *	func
	 ****************************************************/
	 /// <summary>
	 /// �� �ڿ� ���ο� ���Ҹ� �߰��Ѵ�.
	 /// </summary>
	 /// <param name="s"></param>
	inline void push_back(T s) {
		if (capacity <= length) {
			T* temp = new T[static_cast<__int64>(capacity) * 2];
			for (int i = 0; i < length; i++) {
#pragma warning(suppress: 6385)
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
			capacity *= 2;
		}
#pragma warning(suppress: 6385)
		data[length] = s;
		length++;
	}


	/// <summary>
	/// ������ ��ġ�� ���ҿ� �����Ѵ�.
	/// </summary>
	/// <param name="i"></param>
	/// <returns></returns>
	inline T operator[](int i) { return data[i]; }


	/// <summary>
	/// x ��° ��ġ�� ���Ҹ� �����Ѵ�.
	/// </summary>
	/// <param name="x"></param>
	inline void remove(int x) {
		for (int i = x + 1; i < length; i++) {
			data[i - 1] = data[i];
		}
		length--;
	}


	/// <summary>
	/// ���� ������ ũ�⸦ ���Ѵ�.
	/// </summary>
	/// <returns></returns>
	inline int size() { return length; }
};





template <typename T>
class Stack
{
	/****************************************************
	*	data
	****************************************************/
	struct Node
	{
		Node* prev;
		T s;
		Node(Node* prev, T s) : prev(prev), s(s) {}
	};

	Node* current;
	Node start;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	Stack() : start(NULL, nullptr) { current = &start; }
	virtual ~Stack() {
		while (current != &start) {
			Node* prev = current;
			current = current->prev;
			delete prev;
		}
	}


	/****************************************************
	 *	func
	 ****************************************************/
	 /// <summary>
	 /// �ֻ�ܿ� ���ο� ���Ҹ� �߰��Ѵ�.
	 /// </summary>
	 /// <param name="s"></param>
	inline void push(T s) {
		Node* n = new Node(current, s);
		current = n;
	}


	/// <summary>
	/// �ֻ���� ���Ҹ� �����ϰ� ��ȯ�Ѵ�.
	/// </summary>
	/// <returns></returns>
	inline T pop() {
		if (current == &start) return nullptr;
		T s = current->s;
		Node* prev = current;
		current = current->prev;
		// Delete popped node
		delete prev;
		return s;
	}


	/// <summary>
	/// �ֻ���� ���Ҹ� ��ȯ�Ѵ�. (���� ����)
	/// </summary>
	/// <returns></returns>
	inline T peek() { return current->s; }


	/// <summary>
	/// ������ ����ִ����� ������ ��ȯ�Ѵ�.
	/// </summary>
	/// <returns></returns>
	inline bool is_empty() {
		if (current == &start) return true;
		return false;
	}

};


class Table;

class Cell
{
protected:
	int x, y;
	Table* table;

public:
	virtual string stringify() = 0;
	virtual int to_numeric() = 0;

	Cell(int x, int y, Table* table)
		: x(x), y(y), table(table) {
	}
	virtual ~Cell() {	}
};



class Table
{
	/****************************************************T
	 *	data
	 ****************************************************/
protected:
	// �� �� ���� �ִ� ũ��
	int max_row_size, max_col_size;

	// �����͸� �����ϴ� ���̺�
	// Cell* �� �����ϴ� 2���� �迭�̶� �����ϸ� ���ϴ�
	Cell*** data_table;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	Table(int max_row_size, int max_col_size)
		: max_row_size(max_row_size), max_col_size(max_col_size) {
		data_table = new Cell * *[max_row_size];
		for (int i = 0; i < max_row_size; i++) {
			data_table[i] = new Cell * [max_col_size];
			for (int j = 0; j < max_col_size; j++) {
				data_table[i][j] = NULL;
			}
		}
	}

	virtual ~Table() {
		for (int i = 0; i < max_row_size; i++) {
			for (int j = 0; j < max_col_size; j++) {
				if (data_table[i][j])
					delete data_table[i][j];
			}
		}
		for (int i = 0; i < max_row_size; i++) {
			delete[] data_table[i];
		}
		delete[] data_table;
	}


	/****************************************************
	 *	func
	 ****************************************************/
public:
	/// <summary>
	/// ���ο� ���� row �� col ���� ����Ѵ�.
	/// </summary>
	/// <param name="c">����� Cell Instance</param>
	/// <param name="row">�� ��ȣ</param>
	/// <param name="col">�� ��ȣ</param>
	inline void reg_cell(Cell* c, int row, int col) {
		if (!(row < max_row_size && col < max_col_size))
			return;

		if (data_table[row][col]) {
			delete data_table[row][col];
		}
		data_table[row][col] = c;
	}



	/// <summary>
	/// �ش� ���� �������� ��ȯ�Ѵ�.
	/// </summary>
	/// <param name="s">�� �̸� (Ex. A3, B6 �� ����)</param>
	/// <returns>���� ������</returns>
	inline int to_numeric(const string& s) {
		// Cell �̸����� �޴´�.
		int row = s[0] - 'A';
		int col = atoi(s.c_str() + 1) - 1;
		if (row < max_row_size && col < max_col_size) {
			if (data_table[row][col]) {
				return data_table[row][col]->to_numeric();
			}
		}
		return 0;
	}


	/// <summary>
	/// �� �� �� ��ȣ�� ���� ȣ���Ѵ�.
	/// </summary>
	/// <param name="row">�� ��ȣ</param>
	/// <param name="col">�� ��ȣ</param>
	/// <returns>�� ���� ��ȯ</returns>
	inline int to_numeric(int row, int col) {
		if (row < max_row_size && col < max_col_size && data_table[row][col]) {
			return data_table[row][col]->to_numeric();
		}
		return 0;
	}


	/// <summary>
	/// �ش� ���� ���ڿ��� ��ȯ�Ѵ�.
	/// </summary>
	/// <param name="s"></param>
	/// <returns></returns>
	inline string stringify(const string& s) {
		// Cell �̸����� �޴´�.
		int col = s[0] - 'A';
		int row = atoi(s.c_str() + 1) - 1;
		if (row < max_row_size && col < max_col_size) {
			if (data_table[row][col]) {
				return data_table[row][col]->stringify();
			}
		}
		return "";
	}


	/// <summary>
	/// �� �� �� ��ȣ�� ���� ���� ���ڿ��� ��ȯ�Ѵ�.
	/// </summary>
	/// <param name="row">�� ��ȣ</param>
	/// <param name="col">�� ��ȣ</param>
	/// <returns>�� ���ڿ�</returns>
	inline string stringify(int row, int col) {
		if (row < max_row_size && col < max_col_size && data_table[row][col]) {
			return data_table[row][col]->stringify();
		}
		return "";
	}

	inline friend std::ostream& operator<<(std::ostream& o, Table& table) {
		o << table.cell_table();
		return o;
	}


	virtual string cell_table() = 0;
};








/****************************************************
 *	���� Cell Ŭ����
 *
 * ValueTypeCell , StringCell , NumberCell , DoubleCell
 * BoolCell , DateCell
 ****************************************************/
class ValueTypeCell : public Cell
{
	ValType data;
public:
	inline string stringify() {
		string ret;
		switch (data) {
		case ValType::Boolian:
			ret = "Boolian";
			break;
		case ValType::Double:
			ret = "Double";
			break;
		case ValType::Integer:
			ret = "Integer";
			break;
		default:
			ret = "String";
		}

		return ret;
	}
	inline int to_numeric() { return static_cast<int>(data); }

	ValueTypeCell(ValType data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
};



class StringCell : public Cell
{
	char* data;
public:
	inline string stringify() { return data; }
	inline int to_numeric() { return 0; }
	StringCell(LPCSTR str, int x, int y, Table* t) : data(nullptr), Cell(x, y, t) {
		int size = (int)strlen(str) + 1;
		data = new char[size];
		memset(data, 0x00, size * sizeof(char));
		strcat_s(data, size * sizeof(char), str);
	}
	~StringCell() {
		if (data != nullptr) {
			delete data;
		}
		data = nullptr;
	}
};


class NumberCell : public Cell {
	int data;
public:
	inline string stringify() { return to_string(data); }
	inline int to_numeric() { return data; }
	NumberCell(int data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
};

class DoubleCell : public Cell {
	double data;
public:
	inline string stringify() { return to_string(data); }
	inline int to_numeric() { return (int)data; }
	DoubleCell(double data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
};


class BoolCell : public Cell {
	bool data;
public:
	string stringify() { return data ? "True" : "False"; }
	int to_numeric() { return data ? 1 : 0; }
	BoolCell(bool data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
};

class DateCell : public Cell
{
	time_t data;
public:
	inline string stringify() {
		char buf[50];
		tm temp;
		localtime_s(&temp, &data);
		strftime(buf, 50, "%F", &temp);
		return string(buf);
	}
	inline int to_numeric() { return static_cast<int>(data); }
	DateCell(string s, int x, int y, Table* t) : data(data), Cell(x, y, t) {
		// �Է¹޴� Date ������ �׻� yyyy-mm-dd ���̶� �����Ѵ�.
		int year = atoi(s.c_str());
		int month = atoi(s.c_str() + 5);
		int day = atoi(s.c_str() + 8);
		tm timeinfo;
		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		timeinfo.tm_hour = 0;
		timeinfo.tm_min = 0;
		timeinfo.tm_sec = 0;
		data = mktime(&timeinfo);
	}
};

/*
@  �߰��� ������ Ÿ�� �� ����
@
@
*/




/****************************************************
 *	Cell�� ������ ������ Ŭ����
 *
 ****************************************************/
class ptrValue {
	void* p_value;
public:
	ptrValue() :p_value(nullptr) {}

	template <typename T >
	ptrValue(T data) : p_value(nullptr) {
		static_cast<void*>(p_value = new T(data));
	};

	template <>
	ptrValue(LPCSTR data) :p_value(nullptr) {
		int size = (int)strlen(data) + 1;
		static_cast<void*>(p_value = new char[size]);
		memset((char*)p_value, 0x00, size * sizeof(char));
		strcat_s((char*)p_value, size * sizeof(char), data);
	};

	template <>
	ptrValue(LPCTSTR data) :p_value(nullptr) {
		int size = (int)_tcslen(data) + 1;
		static_cast<void*>(p_value = new char[size]);
		memset((char*)p_value, 0x00, size * sizeof(char));
		vrTcharToLPSTR((LPTSTR)data, (char*)p_value, size);

		//_tcsncat_s((TCHAR*)p_value, size , data, VIRTUAL_RS_CHAR_MAX);
	};

	template <>
	ptrValue(string data) :p_value(nullptr) {
		char tmp[VIRTUAL_RS_CHAR_MAX] = { 0, };
		int size = (int)data.length() + 1;
		static_cast<void*>(p_value = new char[size]);
		for (int i = 0; i < (int)data.length(); i++)
			tmp[i] = data[i];

		memset((char*)p_value, 0x00, size * sizeof(char));
		strcat_s((char*)p_value, size * sizeof(char), tmp);
	};


	virtual ~ptrValue() {
		if (p_value != nullptr)
			delete p_value;
		p_value = nullptr;
	};

public:
	template <typename T >
	inline void* operator=(T data) {
		return static_cast<void*>(p_value = new T(data));
	}

	template <>
	inline void* operator=(LPCSTR data) {
		int size = (int)strlen(data) + 1;
		static_cast<void*>(p_value = new char[size]);
		memset((char*)p_value, 0x00, size * sizeof(char));
		strcat_s((char*)p_value, size * sizeof(char), data);
		return static_cast<void*>(p_value);
	};

	template <>
	inline void* operator=(LPCTSTR data) {
		int size = (int)_tcslen(data) + 1;
		static_cast<void*>(p_value = new char[size]);
		memset((char*)p_value, 0x00, size * sizeof(char));
		vrTcharToLPSTR((LPTSTR)data, (char*)p_value, size);

		return static_cast<void*>(p_value);
	};

	template <>
	inline void* operator=(string data) {
		char tmp[VIRTUAL_RS_CHAR_MAX] = { 0, };
		int size = (int)data.length() + 1;
		static_cast<void*>(p_value = new char[size]);
		for (int i = 0; i < (int)data.length(); i++)
			tmp[i] = data[i];

		memset((char*)p_value, 0x00, size * sizeof(char));
		strcat_s((char*)p_value, size * sizeof(char), tmp);
		return static_cast<void*>(p_value);
	};

public:
	/// <summary>
	/// value �����͸� ��ȯ
	/// </summary>
	/// <returns>value ������</returns>
	inline void* GetpValue() const { return p_value; }
	/// <summary>
	/// value �����͸� ��ȯ
	/// </summary>
	/// <returns>value ������</returns>
	inline void* Getter() const { return p_value; }

};





#define SYS_DATA_TBL_COLS_MAX   4
#define COLS_CAST(x)   static_cast<int>(virtualRS::Cols::x)

class virtualRS : public Table
{
	/****************************************************
	 *	data
	 ****************************************************/
private:
	int max_size;
	int end_cursor;

public:

	struct data_t
	{
		LPCSTR section;
		LPCSTR item;
		void* p_value;
		ValType  type;

		void Setter(LPCSTR args0, LPCSTR args1, void* args2, ValType  args3) {
			section = args0;
			item = args1;
			p_value = args2;
			type = args3;
		}

		void SetpValue(void* args2) { p_value = args2; }
	};

	enum class Cols
	{
		section,
		item,
		value,
		type,

		_max
	};



	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	virtualRS(int row) : max_size(row), end_cursor(0), Table(row, COLS_CAST(_max)) {}
	virtual ~virtualRS() {  }

	/****************************************************
	 *	func
	 ****************************************************/
private:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="n"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	inline  string repeat_char(int n, char c) {
		string s = "";
		for (int i = 0; i < n; i++)
			s.push_back(c);
		return s;
	}

	/// <summary>
	/// ���ڷ� �� �� ��ȣ�� A, B, .... Z, AA, AB, ... �̷� ������ �Ű��ش�.
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	inline  string col_num_to_str(int n) {
		string s = "";
		if (n < 26) {
			s.push_back('A' + n);
		}
		else {
			char first = 'A' + n / 26 - 1;
			char second = 'A' + n % 26;
			s.push_back(first);
			s.push_back(second);
		}
		return s;
	}


	/*

������ ����
		sectino   :    item    :    value   : vlaue_type
	0 		.            .					  .            .
	1			.            .						.            .
	n			.						 .						.						 .
*/

	inline string col_title_str() {
		string s = "";
	}


	/// <summary>
	/// ������ ��ĭ�� ����
	/// </summary>
	/// <param name="s"></param>
	inline  void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	/// <summary>
	/// ������ ��ĭ�� ����
	/// </summary>
	/// <param name="s"></param>
	inline void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	/// <summary>
	/// �������� Ȯ��
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	inline bool isNumber(const string& str) {
		for (char const& c : str) {
			if (std::isdigit(c) == 0) return false;
		}
		return true;

		//return str.find_first_not_of(".0123456789") == string::npos &&
		//	str.front() != '.' && str.back() != '.';
	}

	/// <summary>
	/// ������ Section�׸��� �ִ��� Ȯ��
	/// </summary>
	/// <param name="str">Section�׸�</param>
	/// <returns>������ -1, ������ cursor</returns>
	inline int findSection(string& str) {
		int fine_no = 0;
		string rec_str;
		while (fine_no < end_cursor) {
			rec_str = data_table[fine_no][COLS_CAST(section)]->stringify();
			rtrim(rec_str);
			if (str.compare(rec_str) == 0)
				break;
			fine_no++;
		}
		return (fine_no == end_cursor) ? -1 : fine_no;
	}

	/// <summary>
	/// ������ Item�׸��� �ִ��� Ȯ��
	/// </summary>
	/// <param name="str">Item�׸�</param>
	/// <returns>������ -1, ������ cursor</returns>
	inline int findItem(string& str) {
		int fine_no = 0;
		string rec_str;
		while (fine_no < end_cursor) {
			rec_str = data_table[fine_no][COLS_CAST(item)]->stringify();
			rtrim(rec_str);
			if (str.compare(rec_str) == 0)
				break;
			fine_no++;
		}
		return (fine_no == end_cursor) ? -1 : fine_no;
	}

	inline int findCursor(string& section, string& item) {
		int fine_no = 0;
		string str_section, str_item;
		while (fine_no < end_cursor) {
			str_section = data_table[fine_no][COLS_CAST(section)]->stringify();
			str_item = data_table[fine_no][COLS_CAST(item)]->stringify();
			rtrim(str_section);
			rtrim(str_item);
			bool is_same_section = str_section.compare(section) == 0;
			bool is_same_item = str_item.compare(item) == 0;

			if (is_same_section && is_same_item)
				break;
			fine_no++;
		}
		return (fine_no == end_cursor) ? -1 : fine_no;
	}


	/// <summary>
	/// value�� ������ ���¸� ��ȯ
	/// </summary>
	/// <param name="value"></param>
	/// <returns>vlaue ��������</returns>
	inline ValType getType(string& value) {
		ValType ret = ValType::none;
		int word_length = (int)value.length();
		int number_of_words = 0;
		bool is_number = value.find_first_not_of(".0123456789") == string::npos
			&& value.front() != '.'
			&& value.back() != '.';

		for (int i = 0; i < word_length; i++) {
			if (value[i] == '.')
				number_of_words++;
		}

		if (is_number == true) {
			if (number_of_words == 1)
				ret = ValType::Double;
			else if (number_of_words == 0)
				ret = ValType::Integer;
			else
				ret = ValType::String;
		}
		else {
			if (value.compare("True") == 0
				|| value.compare("true") == 0
				|| value.compare("TRUE") == 0
				|| value.compare("False") == 0
				|| value.compare("false") == 0
				|| value.compare("FALSE") == 0)
				ret = ValType::Boolian;
			else
				ret = ValType::String;
		}
		return ret;
	}


public:

	inline int GetEndCursor() {
		return end_cursor;
	}


	/// <summary>
	/// ���� row ���ڵ带 �߰� , ������ �׸��� ������ ����
	/// </summary>
	/// <param name="p_data">�Է� ������ ����ü</param>
	/// <returns>������ ���ڵ� ���� �� </returns>
	inline int AddNew(data_t* p_data) {
		performanceTimer timer;
		double addnew_time = .0;
		timer.StartTimer();

		string rev_str[2];
		int result;

		if (!(max_size > (end_cursor + 1)))
			return -1;


		rev_str[COLS_CAST(section)] = p_data->section;
		rev_str[COLS_CAST(item)] = p_data->item;
		result = GetCursorLoc(rev_str[COLS_CAST(section)], rev_str[COLS_CAST(item)]);

		addnew_time = timer.GetElaspTime();
		//TRACE("AddNew GetCursorLoc  :  [%7.3f] \n\r", addnew_time);
		if (result == -1) {
			Table::reg_cell(new StringCell(p_data->section, end_cursor, 0, this), end_cursor, 0);
			Table::reg_cell(new StringCell(p_data->item, end_cursor, 1, this), end_cursor, 1);
			int iDat;
			switch (p_data->type) {
			case ValType::Boolian:
				Table::reg_cell(new BoolCell(*(static_cast<bool*>(p_data->p_value)), end_cursor, 2, this)
					, end_cursor
					, 2);
				break;
			case ValType::Double:
				Table::reg_cell(new DoubleCell(*(static_cast<double*>(p_data->p_value)), end_cursor, 2, this)
					, end_cursor
					, 2);
				break;
			case ValType::Integer:
				iDat = *(static_cast<int*>(p_data->p_value));
				Table::reg_cell(new NumberCell(iDat, end_cursor, 2, this)
					, end_cursor
					, 2);
				break;
			case ValType::String:
				Table::reg_cell(new StringCell(static_cast<LPCSTR>(p_data->p_value), end_cursor, 2, this)
					, end_cursor
					, 2);
				break;
			}

			Table::reg_cell(new ValueTypeCell(p_data->type, end_cursor, 3, this), end_cursor, 3);
			end_cursor++;
		}
		else {
			Update(p_data->p_value, result, p_data->type);
		}

		addnew_time = timer.GetElaspTime();
		//TRACE("AddNew end :  [%7.3f] \n\r", addnew_time);

		return end_cursor;
	}

	/// <summary>
	/// ���� row ���ڵ带 �߰� , ������ �׸��� ������ ����
	/// </summary>
	/// <param name="p_data">�Է� ������ ����ü</param>
	/// <param name="count">������ ��</param>
	/// <returns>������ ���ڵ� ���� �� </returns>
	inline int AddNew(data_t* p_data, int count) {
		string rev_str[2];
		int result;
		for (int i = 0; i < count; i++) {
			if (!(max_size > (end_cursor + 1)))
				return end_cursor;

			rev_str[COLS_CAST(section)] = p_data->section;
			rev_str[COLS_CAST(item)] = p_data->item;
			result = GetCursorLoc(rev_str[COLS_CAST(section)], rev_str[COLS_CAST(item)]);
			if (result == -1) {
				Table::reg_cell(new StringCell(p_data->section, end_cursor, 0, this), end_cursor, 0);
				Table::reg_cell(new StringCell(p_data->item, end_cursor, 1, this), end_cursor, 1);

				switch (p_data->type) {
				case ValType::Boolian:
					Table::reg_cell(new BoolCell(*(static_cast<bool*>(p_data->p_value)), end_cursor, 2, this)
						, end_cursor
						, 2);
					break;
				case ValType::Double:
					Table::reg_cell(new DoubleCell(*(static_cast<double*>(p_data->p_value)), end_cursor, 2, this)
						, end_cursor
						, 2);
					break;
				case ValType::Integer:
					Table::reg_cell(new NumberCell(*(static_cast<int*>(p_data->p_value)), end_cursor, 2, this)
						, end_cursor
						, 2);
					break;
				case ValType::String:
					Table::reg_cell(new StringCell(static_cast<LPCSTR>(p_data->p_value), end_cursor, 2, this)
						, end_cursor
						, 2);
					break;
				default:
					break;
				}

				Table::reg_cell(new ValueTypeCell(p_data->type, end_cursor, 3, this), end_cursor, 3);
				end_cursor++;
			}
			else // ���� �����Ͱ� �ִٸ�
			{
				Update(p_data->p_value, result, p_data->type);
			}
			p_data++;
		}



		return end_cursor;
	}

	/// <summary>
	/// ���� ���ڵ� ���� ������ ����
	/// </summary>
	/// <param name="ptrValue">���� �ּ� ������</param>
	/// <param name="cursor">���ڵ� ��ȣ</param>
	/// <param name="type">���� ��������</param>
	/// <returns>���� Cell �ּ� (Section ������ �ּ�)</returns>
	inline Cell* Update(void* ptrValue, int cursor, ValType type = ValType::String) {
		switch (type) {
		case ValType::Boolian:
			Table::reg_cell(new BoolCell(*(static_cast<bool*>(ptrValue)), cursor, 2, this)
				, cursor
				, 2);
			break;
		case ValType::Double:
			Table::reg_cell(new DoubleCell(*(static_cast<double*>(ptrValue)), cursor, 2, this)
				, cursor
				, 2);
			break;
		case ValType::Integer:
			Table::reg_cell(new NumberCell(*(static_cast<int*>(ptrValue)), cursor, 2, this)
				, cursor
				, 2);
			break;
		case ValType::String:
			Table::reg_cell(new StringCell(static_cast<LPCSTR>(ptrValue), cursor, 2, this)
				, cursor
				, 2);
			break;
		default:
			break;
		}
		Table::reg_cell(new ValueTypeCell(type, cursor, 3, this), cursor, 3);

		return data_table[cursor][COLS_CAST(section)];
	}

	inline void Delete() {
		//TO-DO ���� ��ɿ� ���� ������ �ȵ�
	}

	/// <summary>
	/// section,item ������ ���ڵ���� ��ȣ�� ��ȯ
	/// </summary>
	/// <param name="section"></param>
	/// <param name="item"></param>
	/// <returns>������ -1, ������ ���ڵ���� ��ȣ</returns>
	inline int GetCursorLoc(string& section, string& item) {
		int result = 0;
		result = findCursor(section, item);
		return (result == -1) ? -1 : result;
	}


	inline int GetCursorLoc(LPCSTR sec, int value_no) {
		int section_no = 0;
		string section_str = sec;
		if (section_no == -1)
			return -1;
		int fine_no = 0;
		bool is_section;
		bool is_number;
		while (fine_no < end_cursor) {
			is_section = data_table[fine_no][COLS_CAST(section)]->stringify().compare(section_str) == 0;
			is_number = data_table[fine_no][COLS_CAST(type)]->to_numeric() == static_cast<int>(ValType::Integer);
			if (is_section && is_number) {
				if (data_table[fine_no][COLS_CAST(value)]->to_numeric() == value_no)
					break;
			}
			fine_no++;
		}

		if (fine_no == end_cursor)
			return -1;

		return fine_no;
	}






	/// <summary>
	/// ���̺� �����͸� ���ڿ� ���·� ��ȯ �Ѵ�
	/// </summary>
	/// <param name="p_data">��ȯ ���� ������ �ּ�</param>
	/// <param name="cursor">���ڵ� ��ȣ</param>
	inline void GetValue(LPSTR p_data, int cursor) {
		if (end_cursor > cursor == false)
			return;
		string vlu[4];
		vlu[0] = data_table[cursor][COLS_CAST(section)]->stringify();
		vlu[1] = data_table[cursor][COLS_CAST(item)]->stringify();
		vlu[2] = data_table[cursor][COLS_CAST(value)]->stringify();
		vlu[3] = data_table[cursor][COLS_CAST(type)]->stringify();

		strcpy_s(p_data, vlu[2].length() + 1, vlu[2].c_str());
	}

	/// <summary>
	/// ���̺� �����͸� ���ڿ� ���·� ��ȯ �Ѵ�
	/// </summary>
	/// <param name="cursor">���ڵ� ��ȣ</param>
	/// <param name="type">cols Ÿ��</param>
	/// <returns>�� �ּ�</returns>
	inline Cell* GetValue(int cursor, Cols type = Cols::value) {
		if (end_cursor > cursor == false)
			return nullptr;
		if (type == Cols::_max)
			return nullptr;
		string vlu[4];
		vlu[0] = data_table[cursor][COLS_CAST(section)]->stringify();
		vlu[1] = data_table[cursor][COLS_CAST(item)]->stringify();
		vlu[2] = data_table[cursor][COLS_CAST(value)]->stringify();
		vlu[3] = data_table[cursor][COLS_CAST(type)]->stringify();

		return data_table[cursor][static_cast<int>(type)];
		//strcpy_s(p_data, vlu[2].length() + 1, vlu[2].c_str());
	}


	inline string GetItem(int cursor) {
		if (end_cursor > cursor == false)
			return "";
		return data_table[cursor][COLS_CAST(item)]->stringify();
	}


	inline void GetItem(int cursor, LPSTR ret, int length) {
		if (end_cursor > cursor == false)
			return;
		string value = data_table[cursor][COLS_CAST(item)]->stringify();
		for (int i = 0; i < length; i++)
		{
			if (!(i < value.length()))
				break;
			ret[i] = value[i];
		}
	}

	inline void GetItem(int cursor, LPTSTR ret, int length) {
		if (end_cursor > cursor == false)
			return;

		memset(ret, 0x00, length * sizeof(TCHAR));
		string value = data_table[cursor][COLS_CAST(item)]->stringify();
		for (int i = 0; i < length; i++)
		{
			if (!(i < value.length()))
				break;
			ret[i] = value[i];
		}
	}




	int WriteFile(LPCSTR file_loc) {
		string str_table = write_form();

		/* ������ �ִ��� Ȯ���ؼ� ������ ���� ����, ����*/

		std::ofstream out(file_loc);
		out << str_table;
		return 0;
	}

#if 1

	int ReadFile(LPCSTR file_loc, LPSTR ret_str = nullptr, int buff_size = 0) {
		enum class parsingState {
			none,
			section,
			item,
			value,
		};
		/*performanceTimer timer;
		timer.StartTimer();
		TRACE("\n\r");
		TRACE("Check Time : 1 [%7.6f] \n\r", timer.GetElaspTime());*/

		std::ifstream in(file_loc);
		std::string s = "";
		std::string str[3];
		parsingState currState = parsingState::none;
		char buf[256] = { 0, };
		char* ptr = nullptr;
		int next_idx;
		if (!in.is_open()) {
			AfxMessageBox(L"������ ã�� �� �����ϴ�!");
		}
		//TRACE("Check Time : 2 [%7.6f] \n\r", timer.GetElaspTime());

		int count = 0;
		while (in)
		{
			ptr = &buf[0];
			in.getline(ptr, 256);
			if (strnlen_s(ptr, 256) < 1)
				continue;
			s = "";
			next_idx = 1;
			//bool is_front = true;
			//bool is_value = false;

			if (currState == parsingState::value) {
				int cursor_no = regRecord(&str[0], 3);
				str[COLS_CAST(item)] = "";
				str[COLS_CAST(value)] = "";
			}
			currState = parsingState::none;
			while (*ptr != '\0')
			{
				if (*ptr == '#' && buf[next_idx] == '#') // '##'- �� ���� 
					break;

				if (*ptr == '/' && buf[next_idx] == '/') // '//' �� ����
					break;

				if (*ptr == ' ') // ��ĭ ����
					goto gotoPass;;

				if (*ptr == '\t') // '\t' �� ����
					goto gotoPass;;


				switch (currState)
				{
				case parsingState::none:
				{
					str[COLS_CAST(section)] = "";
					str[COLS_CAST(section)].push_back(*ptr);

					currState = parsingState::section; //
					next_idx++;
					ptr++;
					continue;
				}
				break;
				case parsingState::section:
				{
					if (*ptr == ',') {
						currState = parsingState::item;
						str[COLS_CAST(item)] = "";
						next_idx++;
						ptr++;
						continue;
					}
					else {
						str[COLS_CAST(section)].push_back(*ptr);
					}

				}
				break;
				case parsingState::item:
				{
					if (*ptr == ',') {
						currState = parsingState::value;
						str[COLS_CAST(value)] = "";
						next_idx++;
						ptr++;
						continue;
					}
					else {
						str[COLS_CAST(item)].push_back(*ptr);
					}

				}
				break;
				case parsingState::value:
				{
					str[COLS_CAST(value)].push_back(*ptr);
				}
				break;
				} //switch

			gotoPass:
				next_idx++;
				ptr++;
			}
			count++;
		}

		if (currState == parsingState::value) {
			int cursor_no = regRecord(&str[0], 3);
			str[COLS_CAST(item)] = "";
			str[COLS_CAST(value)] = "";
		}
		//TRACE("Check Time : 3 [%7.6f] \n\r", timer.GetElaspTime());
		in.close();
		//TRACE("Check Time : 4 [%7.6f] \n\r", timer.GetElaspTime());
		string strTable = view_table();
		//TRACE("Check Time : 5 [%7.6f] \n\r", timer.GetElaspTime());
		for (int i = 0; i < buff_size; i++) {
			if (strTable[i] == '\0')
				break;
			ret_str[i] = strTable[i];
		}
		//TRACE("Check Time : 6 [%7.6f] \n\r", timer.GetElaspTime());
		return count;
	}

#endif // 0

	inline ValType regRecord(string* str, ptrValue& pValue, bool is_check = false) {
		ValType type = getType(*str);
		double dtype = 0;
		int itype = 0;
		char tmpValue[256] = { 0, };
		string rev_str = *str;
		switch (type) {
		case ValType::Integer:
			itype = stoi(rev_str);
			pValue = itype;
			break;
		case ValType::Double:
			dtype = stoi(rev_str);
			pValue = dtype;
			break;
		case ValType::Boolian:
		{
			if (rev_str.compare("True") == 0
				|| rev_str.compare("true") == 0
				|| rev_str.compare("TRUE") == 0)
				pValue = true;
			else
				pValue = false;

		}
		break;
		default: // string
			for (int i = 0; i < (int)rev_str.length(); i++)
				tmpValue[i] = rev_str[i];
			pValue = (LPCSTR)tmpValue;
			break;
		}
		return type;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="str"></param>
	/// <param name="count"></param>
	/// <returns></returns>
	inline int regRecord(string* str, int count, bool is_check = false) {
		int ret = 0;
		if (count != 3)  // �Է� ī���ʹ� �ݵ�� 3
			return -1;
		std::string rev_str[3];

		performanceTimer timer;
		double addnew_time = .0;
		timer.StartTimer();

		for (int i = 0; i < count; i++)
			rev_str[i] = str[i];

		ValType type = getType(rev_str[2]);
		data_t data;
		char tmpItem[256] = { 0, };
		char tmpSect[256] = { 0, };
		char tmpValue[256] = { 0, };

		for (int i = 0; i < (int)rev_str[0].length(); i++)
			tmpSect[i] = rev_str[COLS_CAST(section)][i];

		for (int i = 0; i < (int)rev_str[COLS_CAST(item)].length(); i++)
			tmpItem[i] = rev_str[1][i];


		int result = -1;
		if (is_check)
			result = GetCursorLoc(rev_str[COLS_CAST(section)], rev_str[COLS_CAST(item)]);
		if (result == -1) {
			data.section = &tmpSect[0];
			data.item = &tmpItem[0];
			data.type = type;
			ptrValue ptrVlu;
			double dtype = 0;
			int itype = 0;
			switch (type) {
			case ValType::Integer:
				itype = stoi(rev_str[COLS_CAST(value)]);
				ptrVlu = itype;
				break;
			case ValType::Double:
				dtype = stoi(rev_str[COLS_CAST(value)]);
				ptrVlu = dtype;
				break;
			case ValType::Boolian:
			{
				if (rev_str[COLS_CAST(value)].compare("True") == 0
					|| rev_str[COLS_CAST(value)].compare("true") == 0
					|| rev_str[COLS_CAST(value)].compare("TRUE") == 0)
					ptrVlu = true;
				else
					ptrVlu = false;
			}
			break;
			default: // string
				for (int i = 0; i < (int)rev_str[COLS_CAST(value)].length(); i++)
					tmpValue[i] = rev_str[COLS_CAST(value)][i];
				ptrVlu = (LPCSTR)tmpValue;
				break;
			}
			data.p_value = ptrVlu.GetpValue();

			addnew_time = timer.GetElaspTime();
			//TRACE("regRecord  :  [%7.6f] \n\r", addnew_time);
			timer.StartTimer();
			ret = AddNew(&data);
			addnew_time = timer.GetElaspTime();
			//TRACE("regRecord AddNew end  :  [%7.6f] \n\r", addnew_time);
			return ret;
		}
		return ret;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	inline string view_table() {
		string total_table;
		string vlu[4];
		string pre_section;
		//int* col_max_wide = new int[max_col_size];
		// �� ��ܿ� �� ���� ǥ��
		total_table = "################# System Config Data #################\r\n";
		total_table += "\r\n";
		for (int i = 0; i < end_cursor; i++) {
			vlu[COLS_CAST(section)] = "[" + data_table[i][COLS_CAST(section)]->stringify() + "]";
			vlu[COLS_CAST(item)] = data_table[i][COLS_CAST(item)]->stringify();
			vlu[COLS_CAST(value)] = data_table[i][COLS_CAST(value)]->stringify();
			vlu[COLS_CAST(type)] = data_table[i][COLS_CAST(type)]->stringify();

			if (pre_section.compare(data_table[i][COLS_CAST(section)]->stringify()) != 0) {
				total_table += "\r\n";
				pre_section = data_table[i][0]->stringify();
				total_table += vlu[0];
				total_table += "\r\n";
			}
			total_table += " ";
			total_table += vlu[1];
			total_table += repeat_char(20 - (int)vlu[1].length(), ' ');
			total_table += " : " + vlu[2];
			total_table += "\r\n";
		}

		total_table += "\r\n\r\n";
		total_table += "########################  end  ########################";


		return total_table;
	}

	inline int view_table(LPTSTR p_str, int length) {
		string vt = view_table();
		int size = (int)vt.length() + 1;
		//char* tmp = new char(size * (int)sizeof(char));
		char tmp[4096] = { 0, };
		for (int i = 0; i < size; i++)
			tmp[i] = vt[i];
		vrCharToLPTSTR(tmp, p_str, length);
		//delete tmp;
		return 0;
	}


	/// <summary>
	/// �ؽ�Ʈ�� ǥ�� �����ϰ� ������ش�.
	/// </summary>
	/// <returns></returns>
	inline string cell_table() {
		string total_table;
		int* col_max_wide = new int[max_col_size];
		for (int i = 0; i < max_col_size; i++) {
			unsigned int max_wide = 2;
			for (int j = 0; j < max_row_size; j++) {
				if (data_table[j][i] &&
					data_table[j][i]->stringify().length() > max_wide) {
					max_wide = (unsigned int)data_table[j][i]->stringify().length();
				}
			}
			col_max_wide[i] = max_wide;
		}
		// �� ��ܿ� �� ���� ǥ��
		total_table += " ";
		int total_wide = 4;
		for (int i = 0; i < max_col_size; i++) {
			if (col_max_wide[i]) {
				int max_len = max(2, col_max_wide[i]);
				total_table += " | " + col_num_to_str(i);
				total_table += repeat_char(max_len - (int)col_num_to_str(i).length(), ' ');
				total_wide += (max_len + 3);
			}
		}
		total_table += "\n";

		// 
		for (int i = 0; i < end_cursor; i++) {
			total_table += repeat_char(total_wide, '-');
			total_table += "\n" + to_string(i + 1);
			total_table += repeat_char(4 - (int)to_string(i + 1).length(), ' ');
			for (int j = 0; j < max_col_size; j++) {
				if (col_max_wide[j]) {
					int max_len = max(2, col_max_wide[j]);
					string s = "";
					if (data_table[i][j]) {
						s = data_table[i][j]->stringify();
					}
					total_table += " | " + s;
					total_table += repeat_char(max_len - (int)s.length(), ' ');
				}
			}
			total_table += "\n";
		}

		delete[] col_max_wide;
		return total_table;
	}


	inline int cell_table(LPTSTR p_str, int length) {
		string vt = cell_table();
		int size = (int)vt.length() + 1;
		//char* tmp = new char(size * (int)sizeof(char));
		char tmp[4096] = { 0, };
		for (int i = 0; i < size; i++)
			tmp[i] = vt[i];
		vrCharToLPTSTR(tmp, p_str, length);
		//delete tmp;
		return 0;
	}




	inline string write_form() {
		string total_table;
		string vlu[4];
		char prefix = ' ';
		char hex[6] = { 0, };
		int buff_size = 6 * sizeof(char);
		//int* col_max_wide = new int[max_col_size];
		// �� ��ܿ� �� ���� ǥ��
		total_table = "################## IO Define Data #################\r\n";
		total_table += "\r\n";
		for (int i = 0; i < end_cursor; i++) {
			//if (pre_section.compare(data_table[i][COLS_CAST(section)]->stringify()) != 0) {


			vlu[COLS_CAST(section)] = data_table[i][COLS_CAST(section)]->stringify();
			vlu[COLS_CAST(item)] = data_table[i][COLS_CAST(item)]->stringify();
			vlu[COLS_CAST(value)] = data_table[i][COLS_CAST(value)]->stringify();
			vlu[COLS_CAST(type)] = data_table[i][COLS_CAST(type)]->stringify();

			total_table += vlu[0];
			total_table += ",\t";
			total_table += vlu[1];
			total_table += ",\t";
			total_table += vlu[2];
			total_table += "// ";


			if (vlu[0].compare("INPUT") == 0)
				prefix = 'X';
			else
				prefix = 'Y';

			memset(&hex[0], 0, 6 * sizeof(char));
			sprintf_s(&hex[0], buff_size, "%c%03X", prefix, i + 1);
			//total_table
			total_table += hex;
			total_table += "\r\n";
		}

		total_table += "\r\n\r\n";
		total_table += "########################  end  ########################";

		//memset(&hex[0], 0, 6 * sizeof(char));
		return total_table;
	}




};









