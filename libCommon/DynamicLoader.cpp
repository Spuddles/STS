#include "DynamicLoader.h"
#include <Windows.h>
#include "IStrategy.h"

using namespace STS;

DynamicLoader::DynamicLoader()
{
}

DynamicLoader::~DynamicLoader()
{
}

typedef __declspec(dllimport) IStrategy* (__stdcall *creationFunc)();

IStrategy * DynamicLoader::loadModule(const std::string &name)
{
	std::wstring stemp = std::wstring(name.begin(), name.end());
	LPCWSTR sw = stemp.c_str();
	HMODULE hModule = LoadLibrary(sw);
	if (!hModule)
		return nullptr;

	creationFunc cf = (creationFunc)GetProcAddress(hModule, "instantiate");
	if (!cf)
		return nullptr;

	IStrategy *is = cf();

	return is;
}
