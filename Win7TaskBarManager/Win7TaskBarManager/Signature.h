#pragma once
#include <windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <string>
#pragma warning(disable:4312)

using QWORD = unsigned long long;

enum class PatternType
{
	Address,
	Pointer,
	PointerBYTE,
	PointerWORD,
	PointerDWORD,
	PointerQWORD,
	RelativePointer,
	RelativePointerBYTE,
	RelativePointerWORD,
	RelativePointerDWORD,
	RelativePointerQWORD,
};
struct Pattern
{
	const TCHAR *pattern;
	PatternType type;

	Pattern(const TCHAR* Pat = nullptr, PatternType Type = PatternType::Address)
	{
		pattern = Pat;
		type = Type;
	}
};

class PatternScanner
{
private:
	template <typename T, typename ret> static ret ResolveRelativePtr(void *Address)
	{
		if (!Address)
			return nullptr;

		T offset = *(T*)Address;
		if (!offset)
			return nullptr;

		return (ret)((DWORD_PTR)Address + offset + sizeof(T));
	}
	template <typename T> static void *ResolveRelativePtr(void *Address)
	{
		return ResolveRelativePtr<T, void*>(Address);
	}
	template <typename T, typename ret> static ret ResolvePtr(void *Address)
	{
		if (!Address)
			return nullptr;

		return (ret)*(T*)Address;
	}
	template <typename T> static void *ResolvePtr(void *Address)
	{
		return ResolvePtr<T, void*>(Address);
	}
	static MODULEINFO GetModuleInfo(const TCHAR* szModule);
public:
	static DWORD_PTR GetModuleSize(const TCHAR* szModule);
	static void *FindPattern(void *startAddress, DWORD_PTR searchSize, const Pattern &pattern);
	static void *FindPattern(const std::wstring &ModuleName, const Pattern &pattern);
	static void *FindPattern(const Pattern &pattern);

	static BYTE *FindPattern(BYTE *start_pos, DWORD_PTR search_len, const BYTE *pattern, const BYTE *mask);
};