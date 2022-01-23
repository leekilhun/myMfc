#pragma once
#include "afxstr.h"
#include <iostream>
#include <vector>

#define TQUE_BUF_MAX 4096 

/*
 * �ۼ��� : �̱���
 * 21.10.10 byte ������ ���
 */
class TQueue
{

	BYTE m_Buf[TQUE_BUF_MAX]; //  Buffer
	int m_Head, m_Tail;  // CQueue Head Tail Position
	int m_Length;
public:


public:
	TQueue() {
		Init();
	}
	virtual ~TQueue() {
		Init();
	}

	inline void Init()
	{
		m_Head = m_Tail = 0;
		m_Length = TQUE_BUF_MAX;
		memset(m_Buf, 0x00, TQUE_BUF_MAX);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="data"></param>
	/// <param name="length"></param>
	/// <returns>FALSE : overflow �߻�</returns>
	inline BOOL Write(LPCSTR data, uint32_t length) {
		BOOL ret = TRUE;
		uint32_t next_in;

		for (int i = 0; i < (int)length; i++)
		{
			next_in = (m_Head + 1) % m_Length;
			if (next_in != m_Tail)
			{
				m_Buf[m_Head] = data[i];
				m_Head = next_in;
			}
			else
			{
				ret = FALSE;
				break;
			}
		}
		return ret;
	}

	/// <summary>
	/// length ��ŭ ���� �����͸� �о
	/// </summary>
	/// <param name="data"></param>
	/// <param name="length"></param>
	/// <returns>FALSE : underflow �߻� </returns>
	inline BOOL Read(LPSTR data, uint32_t length) {
		BOOL ret = TRUE;
		for (int i = 0; i < (int)length; i++)
		{
			data[i] = m_Buf[m_Tail];
			if (m_Tail != m_Head)
			{
				m_Tail = (m_Tail + 1) % m_Length;
			}
			else
			{
				ret = FALSE;
				break;
			}
		}

		return ret;
	}

	/// <summary>
	/// ä���� buffer data size 
	/// </summary>
	/// <returns>0 �̸� ������ ������ ����</returns>
	inline int Available() {
		return (m_Length + (m_Head - m_Tail)) % m_Length;
	}


	/// <summary>
	/// Queue�� �� ����Ʈ�� �ְ� Head Pointer�� ����
	/// </summary>
	/// <param name="b"> Queue�� 1byte �ֱ�</param>
	/// <returns>FALSE : ����, TRUE : ����</returns>
	inline BOOL PutByte(BYTE b) {
		BOOL ret = TRUE;
		uint32_t next_in;
		next_in = (m_Head + 1) % m_Length;
		if (next_in != m_Tail) {
			m_Buf[m_Head] = b;
			m_Head = next_in;
		}
		else
			ret = FALSE;

		return ret;
	}


	/// <summary>
	/// // Queue�� 1byte ������
	/// </summary>
	/// <param name="pb">�޾ư� ���� �ּ�</param>
	/// <returns></returns>
	inline BOOL GetByte(byte* pb) {
		BOOL ret = TRUE;

		if (m_Tail != m_Head) {
			*pb = m_Buf[m_Tail];
			m_Tail = (m_Tail + 1) % m_Length;
		}
		else
			ret = FALSE;

		return ret;
	}

	// ���� �� �����͸� ��� ó��
	inline void Flush() {
		m_Head = m_Tail = 0;
	}

};


#if 0

TQueue q;
q.Write("12345", 6);
CHAR read[100] = { 0, };
q.Read((LPSTR)&read, 3);
q.GetByte((byte*)&read[3]);
q.PutByte('6');

#endif