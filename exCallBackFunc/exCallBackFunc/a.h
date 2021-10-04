#pragma once
#include <string>
#include <iostream>

#define WM_USER_CALLBACK_1  (WM_USER+1)  

using std::cout;
using std::endl;

//HWND hCommWnd;

class ma
{
	HWND m_hwnd;

	// 생성입니다.
public:
	ma(HWND hwnd = nullptr) :m_hwnd(nullptr)
	{
		m_hwnd = hwnd;
	}
	virtual ~ma(){}


public:

	static void StaticWrapper(void* obj, const std::string& name, void* data)
	{
		ma* self = (ma*)(obj);
		self->InstanceMethod(name, data);
		self->WindowMassgeMethod();

	}


	void InstanceMethod(const std::string& name, void* data)
	{
		int closure = *(int*)(data);
		cout << "In instance callback: '" << name << "' (closure=" << closure << ")" << endl;
	}

	void WindowMassgeMethod()
	{

 		::PostMessage(m_hwnd, WM_USER_CALLBACK_1, 0, 0);
	}


	static void StaticMethod(const std::string& name, void* data)
	{
		int closure = *(int*)(data);
		cout << "In instance callback: '" << name << "', closure=" << closure << endl;

	}

};

