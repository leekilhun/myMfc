#pragma once
// tString.h: �����ڵ� ���ڿ� (MFC)
//
#pragma once
#include "afxstr.h"
#include <iostream>
#include <vector>


/*
	 * �ۼ��� : �̱���
	 * 21.08.12 �����ڵ� ��ȯ�� ���� ���ڿ� Ŭ����
	 */
class TString
{
	LPTSTR str;
	uint16_t buffer_length;
	enum class TrimLoc {
		head,
		tail,
		all,
	};

public:
	TString() {
		Init();
		this->str = create(buffer_length);
	}

	TString(LPCTSTR text) {
		Init();
		this->str = create(buffer_length);
		this->str = SetString(text);
		//delete  text;
	}

	TString(const TString& origin) {
		//delete[]this->str;
		this->Init();
		this->str = create(buffer_length);
		this->buffer_length = origin.buffer_length;
		this->str = SetString(origin.str);
	}

	TString& operator=(const TString& text) {
		this->SetString(text.str);
		return *this;
	}

	TString operator+(const LPCTSTR& text) {
		TString tmp(this->str);
		TString ret_text;
		_tcscat_s(tmp.str, buffer_length, text);
		ret_text = tmp.str;
		return ret_text;//*this;
	}


	TString operator+(const TCHAR in_char) {
		TString tmp(this->str);
		TString ret_text;
		_tcscat_s(tmp.str, buffer_length, &in_char);
		ret_text = tmp.str;
		return ret_text;//*this;
	}


	TString operator+(const TString& text) {
		TString tmp(this->str);
		TString ret_text;
		_tcscat_s(tmp.str, buffer_length, text.GetString());
		ret_text = tmp.str;
		return ret_text;//*this;
	}

	TString operator+=(const TString& text) {
		(*this) = (*this) + text;
		return (*this);//*this;
	}

	TString& operator+=(const LPCTSTR& text) {
		(*this) = (*this) + text;
		return *this;
	}

	virtual ~TString() {
		if (this->str != nullptr)
		{
			delete[]this->str;
		}
	}

	BOOL operator==(LPCTSTR text) {
		return  (_tcscmp(this->str, text) == 0);
	}

	BOOL operator<(const TString& text) const {
		return (_tcscmp(this->str, text.GetString()) < 0);//true;
	}

	BOOL operator>(const TString& text) const {
		return (_tcscmp(this->str, text.GetString()) > 0);//true;
	}

	inline void Init() {
		buffer_length = 1024;
		this->str = {};
	}

	inline LPTSTR create(uint16_t len) {
		this->str = new TCHAR[buffer_length];
		memset(this->str, 0x00, buffer_length * sizeof(TCHAR));
		return this->str;
	}

private:
	inline int constrain(int amt, int low, int high) {
		return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
	}

	inline BOOL between(int amt, int low, int high) {
		BOOL ret = FALSE;
		if (amt >= low)
		{
			if (amt < high)
			{
				ret = TRUE;
			}
		}
		return ret;
	}

	inline LPTSTR trim(TrimLoc trim_loc = TrimLoc::all) {
		TCHAR tmp[1024];
		memset(&tmp, 0x00, 1024 * sizeof(TCHAR));

		TCHAR re_text[1024];
		memset(&re_text, 0x00, 1024 * sizeof(TCHAR));

		int total_count = GetLength() + 1;
		_tcsncpy_s(tmp, this->str, total_count);

		if (trim_loc == TrimLoc::tail)
		{
			_tcsrev(tmp);
		}

		LPCTSTR  buf = tmp;
		int i = 1;
		int copy_str_count = 1;
		BOOL is_cplt = (trim_loc == TrimLoc::all) ? TRUE : FALSE;

		while (true)
		{
			if (*buf == L'\0') break;

			if (*buf == L' ')
			{

				if (is_cplt == FALSE || (trim_loc == TrimLoc::all))
				{
					_tcsncat_s(re_text, i + 1, L"", copy_str_count);
				}
				else
				{
					_Notnull_ _tcsncat_s(re_text, i + 1, L" ", copy_str_count);
				}

			}
			else
			{
				if (is_cplt == FALSE) { is_cplt = TRUE; }
				_Notnull_ _tcsncat_s(re_text, i + 1, buf, copy_str_count);
			}

			//_tcscat_s(re_text, 2, buf);
			++buf;
			++i;
		}

		if (trim_loc == TrimLoc::tail)
		{
			_tcsrev(re_text);
		}
		this->SetString(re_text);

		return this->str;
	}

public:

	/// <summary>
	/// Ŭ������ ���ڿ� Set
	/// </summary>
	/// <param name ="text">������ ���ڿ�</param>
	/// <returns>���ڿ� �ּ�</returns>
	inline LPTSTR SetString(LPCTSTR text) {
		_tcscpy_s(this->str, (buffer_length /** sizeof(TCHAR)*/), text);
		return this->str;
	}

	/// <summary>
	/// Ŭ���� ���ڿ��� ��ȯ
	/// </summary>
	/// <returns>���ڿ� �ּ�</returns>
	inline LPTSTR GetString() const {
		return this->str;
	}

	/// <summary>
	/// ���ڿ��� �������� Ȯ��
	/// </summary>
	/// <param name="text">�� ���ڿ�</param>
	/// <returns>���� 0 / ~ 0 �̿�</returns>
	inline int Compare(LPTSTR text) const {
		return _tcscmp(this->str, text);
	}

	/// <summary>
	/// ��ҹ��� ���� ���� ���ڿ��� �������� Ȯ��
	/// </summary>
	/// <param name="text">�� ���ڿ�</param>
	/// <returns>���� 0 / ~ 0 �̿�</returns>
	inline int CompareNoCase(LPTSTR text) const {
		return _tcsicmp(this->str, text);
	}


	/// <summary>
	/// ���ڿ� ���� ��ȯ
	/// </summary>
	/// <returns>���ڿ� ����</returns>
	inline int GetLength() const {
		return (int)_tcslen(this->str);
	}

	/// <summary>
	/// ���ڿ��� �������� Ȯ��
	/// </summary>
	/// <param name="text">�� ���ڿ�</param>
	/// <returns>���� TRUE / ~ FALSE</returns>
	inline BOOL IsCompare(LPTSTR text) const {
		return (_tcscmp(this->str, text) == 0);
	}

	/// <summary>
	/// ���ڿ��� �ʱ�ȭ ���� Ȯ��
	/// </summary>
	/// <returns>�ʱ�ȭ ���� TRUE / ~ FALSE</returns>
	inline BOOL IsEmpty() const {
		return (_tcslen(this->str) == 0);
	}

	/// <summary>
	/// ���ڿ��� �ʱ�ȭ
	/// </summary>
	/// <returns>����</returns>
	inline void Empty() {
		memset(this->str, 0x00, buffer_length * sizeof(TCHAR));
	}

	/// <summary>
	/// ���ڿ� ���ʿ��� count��ŭ ��ȯ �Ѵ�
	/// </summary>
	/// <returns>���ڿ� �ּ�</returns>
	inline LPTSTR Left(int count) {
		count = constrain(count, 0, GetLength());
		TCHAR buf[1024];
		_tcsncpy_s(buf, this->str, count);
		SetString((LPCTSTR)buf);
		return this->str;
	}

	/// <summary>
	/// ���ڿ� ���ʿ��� start��ŭ �̵��Ͽ� count��ŭ ��ȯ �Ѵ�
	/// </summary>
	/// <param name="start">���� �̵� ��ġ</param>
	/// <param name="count">��ȯ ���ڿ� ��</param>
	/// <returns>���ڿ� �ּ�</returns>
	inline LPTSTR Mid(int start, int count = 1020) {
		count = constrain(count, 1, (GetLength() - start));
		if (start < 0 || start >= GetLength()) goto invaild;

		LPTSTR pre = new TCHAR[buffer_length];
		memset(pre, 0x00, buffer_length * sizeof(TCHAR));

		LPTSTR  _Notnull_  buf;
		int total_count;
		total_count = GetLength();

		buf = this->str;
		for (int i = 0; i < total_count; i++)
		{
			if (i >= (start - 1))
			{
				if ((*buf == L'\0') || (count == 0))	break;

				_Notnull_ _tcsncat_s(pre, _tcslen(buf) + 1, &buf[i], 1);
				count--;
				//++buf;
			}
		}

		SetString((LPCTSTR)pre);
		delete[]pre;

	invaild:
		return this->str;
	}

	/// <summary>
	/// ���ڿ� �����ʿ��� count��ŭ ��ȯ �Ѵ�
	/// </summary>
	/// <returns>���ڿ� �ּ�</returns>
	inline LPTSTR Right(int count) {
		count = constrain(count, 0, GetLength());
		LPTSTR  buf;
		int rev_count, total_count;
		total_count = GetLength();
		rev_count = total_count - count;

		LPTSTR pre = new TCHAR[buffer_length];
		memset(pre, 0x00, buffer_length * sizeof(TCHAR));
		int copy_str_count = 1;
		buf = this->str;
		for (int i = 0; i < total_count; i++)
		{
			if (i >= rev_count)
			{
				_tcsncat_s(pre, count + 1, buf, copy_str_count);
			}
			++buf;
		}

		SetString((LPCTSTR)pre);
		delete[]pre;

		return this->str;
	}

	/// <summary>
	/// ���� ��ȯ �̿��Ͽ� ���ڿ��� ���
	/// </summary>
	/// <returns>���ڿ� �ּ�</returns>
	inline LPTSTR Format(LPCTSTR fmt, ...) {
		Empty();
		va_list args;
		TCHAR buf[1024];
		va_start(args, fmt);
		vswprintf_s(buf, fmt, args);
		SetString(buf);
		va_end(args);
		return this->str;
	}

	/// <summary>
	/// ���б�ȣ�� ���ڿ��� �и�(����ȭ)
	/// </summary>
	/// <param name ="pV">���� ���� �ּ�</param>
	/// <param name ="delimiters">���� ���� ��ȣ (default "/-:, ")</param>
	/// <returns>������</returns>
	inline int Split(std::vector<TString>* pV, LPCTSTR delimiters = _T("/-:, ")) {
		LPTSTR rec_text = new TCHAR[buffer_length];
		memset(rec_text, 0x00, buffer_length * sizeof(TCHAR));
		_tcscpy_s(rec_text, buffer_length, this->str);
		LPTSTR pReturn = {};
		LPTSTR RetStr = {};

		while (true)
		{
			RetStr = _tcstok_s(rec_text, delimiters, &pReturn);
			pV->push_back(RetStr);
			if (pReturn != nullptr) { if (*pReturn == L'\0')	break; }

			if (pReturn != nullptr)
			{
				_tcscpy_s(rec_text, _tcslen(pReturn) + 1, pReturn);
			};
		}

		delete[]rec_text;
		return (int)pV->size();
	}

	/// <summary>
	/// text ���ڿ��� ã�� ���� ��ġ�� ���ڿ��� ��ȯ�Ѵ� (������ -1 ��ȯ)
	/// </summary>
	/// <param name ="text"> ã�� ���ڿ�</param>
	/// <param name ="start_location"> �˻��� ������ ��ġ </param>
	/// <param name ="ret_str">ã�� ���ڱ����� ��ȯ ���ڿ� </param>
	/// <returns>ã�� ���� ��ġ (ù��° = 1,..) </returns>
	inline int Find(LPCTSTR text, int start_location = 0, LPTSTR ret_str = nullptr) {
		LPTSTR  buf = {}, origin_str = {};
		int total_count, ret;
		ret = -1;
		origin_str = this->str;
		for (int i = 0; i < start_location; i++)
		{
			++origin_str;
		}

		total_count = (int)_tcslen(origin_str);
		// ã���� ã�� ���ڿ� ���� ���� ��ȯ, ������ NULL
		buf = _tcsstr(origin_str, text);
		// ã�� ���ڰ� ������ �ε��� �� ���� 0,1,2...
		if (buf == NULL)  goto invaild;

		ret = total_count - (int)_tcslen(buf);

		if (ret_str != nullptr)
		{
			buf = origin_str;
			int i = 1;
			int copy_str_count = 1;
			while (1)
			{
				if (i > ret) break;
				_tcsncat_s(ret_str, i + 1, buf, copy_str_count);
				++buf;
				++i;
			}
		}

	invaild:
		return ret;
	}

	/// <summary>
	///  text ���ڿ��� �ڿ��� ã�� ���� ��ġ�� ���ڿ��� ��ȯ�Ѵ� (������ -1 ��ȯ)
	/// </summary>
	/// <param name ="ret_str">ã�� ���ڱ����� ��ȯ ���ڿ� </param>
	/// <returns>ã�� ���� ��ġ (ù��° = 1,..) </returns>
	inline int ReverseFind(LPCTSTR text, LPTSTR ret_str = nullptr) {
		int total_count, ret = -1, text_len = 0, buf_len = 0;
		total_count = GetLength() + 1;
		text_len = (int)_tcslen(text) + 1;

		LPTSTR rev_buf = new TCHAR[total_count];
		memset(rev_buf, 0x00, total_count * sizeof(TCHAR));
		_tcscpy_s(rev_buf, total_count, this->str);
		_tcsrev(rev_buf);

		LPTSTR rev_text = new TCHAR[text_len];
		memset(rev_text, 0x00, text_len * sizeof(TCHAR));
		_tcscpy_s(rev_text, text_len, text);
		_tcsrev(rev_text);

		LPTSTR  buf = {};
		// ã���� ã�� ���ڿ� ���� ���� ��ȯ, ������ NULL
		buf = _tcsstr(rev_buf, rev_text);
		// ã�� ���ڰ� ������ �ε��� �� ���� 0,1,2...
		if (buf == NULL)  goto invaild;
		//
		buf_len = (int)_tcslen(buf) - (text_len - 1);

		TCHAR tmp[1024];
		_tcsncpy_s(tmp, this->str, buf_len);

		ret = buf_len;

		if (ret_str != nullptr)
		{
			if (tmp != nullptr)
			{
				_Notnull_ _tcscpy_s(ret_str, _tcslen(tmp) + 1, tmp);
			}
		}

	invaild:
		delete[]rev_buf;
		delete[]rev_text;
		return ret;
	}

	/// <summary>
	/// ���ڿ��� ã�� �����Ѵ�
	/// </summary>
	/// <param name="find_str">ã�� ���� ��ġ</param>
	/// <param name="replace">��ġ�� ����</param>
	/// <returns>����� ���ڿ�</returns>
	inline LPTSTR Replace(LPTSTR find_str, LPTSTR replace) {
		int total_count = GetLength() + 1;
		int find_word_len = (int)_tcslen(find_str);
		int replace_word_len = (int)_tcslen(replace);
		int fine_loc = Find(find_str);
		if (fine_loc < 0)  goto invaild;

		int start_loc = fine_loc + 1;
		TCHAR tmp[1024];
		memset(&tmp, 0x00, 1024 * sizeof(TCHAR));

		TCHAR re_text[1024];
		memset(&re_text, 0x00, 1024 * sizeof(TCHAR));

		_tcsncpy_s(tmp, this->str, total_count);
		LPCTSTR  _Notnull_ buf = tmp;

		int i = 1;
		int copy_str_count = 1;
		int check_cnt = find_word_len;
		while (true)
		{
			if (*buf == L'\0') break;

			if (between(i, start_loc, (start_loc + replace_word_len)))
			{
				_tcsncat_s(re_text, i + 1, replace, copy_str_count);
				++replace;
				--find_word_len;
			}
			else
			{
				if (find_word_len == 0 || find_word_len == check_cnt)
				{
					if (buf != nullptr)
					{
						_Notnull_ _tcsncat_s(re_text, i + 1, buf, copy_str_count);
					}
				}
				else
				{
					--find_word_len;
				}
			}

			++buf;
			++i;
		}
		this->SetString(re_text);

	invaild:
		return this->str;
	}

	/// <summary>
	/// ���ڿ����� ��ĭ�� �����Ѵ�
	/// </summary>
	/// <param name=""></param>
	/// <returns>����� ���ڿ�</returns>
	inline LPTSTR Trim(void) {
		return trim();
	}

	/// <summary>
	/// ���ڿ� �� ���ڸ��� �����Ѵ�
	/// </summary>
	/// <param name=""></param>
	/// <returns>����� ���ڿ�</returns>
	inline LPTSTR TrimLeft(void) {
		return trim(TrimLoc::head);
	}

	/// <summary>
	/// ���ڿ� �� ���ڸ��� �����Ѵ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline LPTSTR TrimRight(void) {
		return trim(TrimLoc::tail);
	}

};

namespace t
{

	/************************************************************************/
	/* �����ڵ� �� ��Ƽ����Ʈ ���ڿ� ��ȯ�Լ� ������                            */
	/************************************************************************/

	/// <summary>
	/// CString -> LPSTR
	/// </summary>
	/// <param name="str">CString ����</param>
	/// <param name="ret_str">������ char���ڿ�</param>
	/// <returns></returns>
	inline int CStringToLPSTR(CString& str, LPSTR ret_str) {
		if (str.IsEmpty())
			return 0;

		if (strlen(ret_str) != 0)
			memset(ret_str, 0x00, strlen(ret_str) * sizeof(CHAR));

#if defined(UNICODE) || defined(_UNICODE)
		int nlen = str.GetLength() + 1;
		int ret = WideCharToMultiByte(
			CP_ACP,
			0, 
			str.GetBuffer(), 
			-1, 
			ret_str, 
			nlen, 
			NULL, 
			NULL
		);
#else
		int nlen = str.GetLength() + 1;
		if (strcpy_s(ret_str, nlen, str) == 0)
			ret = nlen;

#endif
		return ret;
	}



	//
	// CString �� LPTSTR
	//
	inline void CStringToLPTSTR(CString str, LPTSTR ret) {
		int nLen = str.GetLength() + 1;
		memset(ret, 0x00, nLen * sizeof(TCHAR));
		_tcscpy_s(ret, nLen, str);
	}
	

	//
	// CString �� std::string
	//
	inline std::string CStringToStdString(CString str) {
		std::string stdStr;
		char szStr[1024] = {};
		CStringToLPSTR(str, szStr);
		if (szStr)
		{
			stdStr = szStr;
			delete[] szStr;
		}

		return stdStr;
	}

	//
	// Char �� CString
	//
	inline CString CharToCString(char* str) {
		CString cStr;
#if defined(UNICODE) || defined(_UNICODE)
		int nLen = (int)strlen(str) + 1;
		TCHAR* tszTemp = NULL;
		tszTemp = new TCHAR[nLen];
		memset(tszTemp, 0x00, nLen * sizeof(TCHAR));
		MultiByteToWideChar(
			CP_ACP, 
			0, 
			str, 
			-1, 
			tszTemp, 
			nLen * sizeof(TCHAR)
		);

		cStr.Format(_T("%s"), tszTemp);
		if (tszTemp)
		{
			delete[] tszTemp;
			tszTemp = NULL;
		}
#else
		cStr.Format("%s", str);
#endif
		return cStr;
	}	



	/// <summary>
	/// Char ���ڿ��� Tchar���ڿ��� ��ȯ
	/// </summary>
	/// <param name="str">Char ���ڿ�</param>
	/// <param name="ret_str">Tchar���ڿ��� ��ȯ�ּ�</param>
	/// <returns>���� �� </returns>
	inline int CharToLPTSTR(LPSTR str, LPTSTR ret_str) {
		if (strlen(str) == 0)
			return 0;
		if (_tcslen(ret_str) != 0)
			memset(ret_str, 0x00, _tcslen(ret_str) * sizeof(TCHAR));
		int ret = 0;
		
#if defined(UNICODE) || defined(_UNICODE)
		int nLen = (int)strlen(str) + 1;
		ret = MultiByteToWideChar(
			CP_ACP, 
			0, 
			str, 
			-1, 
			ret_str, 
			nLen * sizeof(TCHAR)
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
	inline CString TCHARToCString(TCHAR* str) {
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
	inline int TcharToLPSTR(LPTSTR str, LPSTR ret_str){
		int ret = 0;
		if (_tcslen(str) == 0)
			return 0;
		if (strlen(ret_str) != 0)
			memset(ret_str, 0x00, strlen(ret_str) * sizeof(CHAR));
		
#if defined(UNICODE) || defined(_UNICODE)
		ret = WideCharToMultiByte(
			CP_ACP, 
			0, 
			str, 
			-1, 
			ret_str, 
			strlen(ret_str) * sizeof(CHAR) +1,
			NULL, 
			NULL
		);
#else
		strcpy(ret_str, str);

#endif
		return ret;
	}

}