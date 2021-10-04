#pragma once
#include <string>
#include <stdlib.h>

class mb
{
public:
	typedef void (*CallbackType)(void* obj, const std::string& name, void* data);
	typedef void (*CallbackTypeStatic)(const std::string& name, void* data); // �ݹ� Ÿ�� ����

	
private:
	void* m_Object;
	CallbackType m_Callback;
	CallbackTypeStatic m_CallbackStatic;
	void* m_Closure;

public:
	mb() : m_Object(NULL), m_Callback(NULL), m_CallbackStatic(NULL), m_Closure(NULL)
	{
	}
	virtual ~mb(){}

public:
	void SetCallback(void* obj, CallbackType cb, void* data)//�ݹ� �Լ� ���
	{
		m_Object = obj;
		m_Callback = cb;
		m_Closure = data;
	}
	void DoAction() //�ݹ� �Լ� ȣ��
	{
		if (m_Callback)
		{
			(*m_Callback)(m_Object, "Hello World", m_Closure);
		}
	}
	void SetCallbackStatic(CallbackTypeStatic cb, void* data)
	{
		m_CallbackStatic = cb;
		m_Closure = data;
	}
	void DoActionStatic()
	{
		if (m_CallbackStatic)
		{
			(*m_CallbackStatic)("Hello World !!", m_Closure);
		}
	}

};

