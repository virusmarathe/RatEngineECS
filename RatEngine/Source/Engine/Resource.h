#pragma once
#include <string>

class Resource
{
public:
	Resource(const wchar_t* fullPath);
	virtual ~Resource();

protected:
	std::wstring m_FullPath;

private:

};

