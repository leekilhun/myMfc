#pragma once
#include <string>
#include <mutex>
#include <vector>
#include "jL_utility.h"

#define LOGMANAGE_BUFF_LIST_MAX 256  
#define LOGMANAGE_TXT_LENGTH__MAX 256

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

class jL_logManager
{
	template <typename T>
	class _Que
	{
		//data
		T* _logList;
		uint32_t _Head, _Tail;
		uint32_t _Length;
		//������
	public:
		_Que() {
			init();
			//memset(_logList, 0, sizeof(_logList));
			_logList = new T[LOGMANAGE_BUFF_LIST_MAX];
		}

		virtual ~_Que() {
			if (_logList != nullptr)
			{
				delete[] _logList;
			}
			init();
		}

		//�Լ�
	private:
		inline void init() {
			_Head = _Tail = 0;
			_logList = nullptr;
			_Length = LOGMANAGE_BUFF_LIST_MAX;
		}
	public:
		inline uint32_t Put(T log) {
			uint32_t ret = _Head;
			_logList[_Head] = log;
			_Head = (_Head+1) % LOGMANAGE_BUFF_LIST_MAX;
			return ret;
		}

		inline T Get() {
			T temp = _logList[_Tail];
			_Tail = (_Tail + 1) % LOGMANAGE_BUFF_LIST_MAX;
			return temp;
		}

		inline T Pop(uint32_t addr) {
			return _logList[addr % LOGMANAGE_BUFF_LIST_MAX];
		}

		inline uint32_t Available() {
			return (_Length + (_Head - _Tail)) % _Length;
		}

		inline void Flush() {
			_Head = _Tail = 0;
		}

	};

	template <typename T>
	class _Stack
	{
		struct Node
		{
			Node* prev;
			T s;
			Node(Node* prev, T s) : prev(prev), s(s) {}
		};

		Node* _current;
		Node _start;

	public:
		_Stack() :_start(nullptr, nullptr) {
			_current = &_start;
		}

		virtual ~_Stack() {
			while (_current != &_start) {
				Node* prev = _current;
				_current = _current->prev;
				delete prev;
			}
		}

	private:


	public:

		/// <summary>
		/// �ֻ�ܿ� ���ο� ���Ҹ� �߰��Ѵ�.
		/// </summary>
		/// <param name="s"></param>
		inline void push(T s) {
			Node* n = new Node(_current, s);
			_current = n;
		}

		/// <summary>
		/// �ֻ���� ���Ҹ� �����ϰ� ��ȯ�Ѵ�.
		/// </summary>
		/// <returns></returns>
		inline T pop() {
			if (_current == &_start) return nullptr;
			T s = _current->s;
			Node* prev = _current;
			_current = _current->prev;
			// Delete popped node
			delete prev;
			return s;
		}

		/// <summary>
		/// �ֻ���� ���Ҹ� ��ȯ�Ѵ�. (���� ����)
		/// </summary>
		/// <returns></returns>
		inline T peek() { return _current->s; }

		/// <summary>
		/// ������ ����ִ����� ������ ��ȯ�Ѵ�.
		/// </summary>
		/// <returns></returns>
		inline bool is_empty() {
			if (_current == &_start) return true;
			return false;
		}
	};

	bool m_isInit;
	uint32_t m_Index;
	mutex m_Mutex;
	char* m_pBuff;
	_Que<string>* m_QueList;

public:
	jL_logManager() :m_Index(0)	, m_isInit(false), m_pBuff(nullptr){
		logInit();
		m_QueList = new _Que<string>;
		m_isInit = true;
	}

	~jL_logManager()	{
		if (m_QueList != nullptr)
		{
			delete m_QueList;
		}
		logInit();
	}

private:
	inline void logInit() {
		m_pBuff = nullptr;
		m_Index = 0;
		m_isInit = false;
		m_QueList = nullptr;
	}
	inline bool logBufPrintf(char* p_data, uint32_t length) {
		return 0;
	}

public:
	inline uint32_t PutLog(const char* fmt, ...) {
		//TODO: ���� �α׸� ǥ���ؾ� �� �� �Լ�

		uint32_t ret = 0;
		if (m_isInit != true) return ret;

		m_Mutex.lock();
		va_list args;
		va_start(args, fmt);

#if 0
		va_list copy;
		va_copy(copy, args);
		int len = vsnprintf(nullptr, 0, fmt, copy);
		va_end(copy);

		if (len >= 0) {
			string result(size_t(len) + 1, '\0');
			vsnprintf(&result[0], result.size(), fmt, args);
			result.resize(len);
			ret = m_QueList->Put(result);
		}
#endif

#if 1
		va_start(args, fmt);		
		int len = vsnprintf(nullptr, 0, fmt, args);
		if (len < 0) {
			va_end(args);
			m_Mutex.unlock();
			return ret;
		} 
		else if (len > 0)
		{
			uint32_t vec_len = len + 1;
			vector<char> buffer(vec_len);
			len = vsnprintf(&buffer[0], buffer.size(), fmt, args);
			string result = std::string(&buffer[0], len);
			ret = m_QueList->Put(result);
		}
#endif

		va_end(args);
		m_Mutex.unlock();

		return ret;
	}

	inline string GetLog() {
		return m_QueList->Get();
	}

	inline string PopLog(uint32_t addr) {
		return m_QueList->Pop(addr);
	}

	inline uint32_t Available() {
		return m_QueList->Available();
	}

};
