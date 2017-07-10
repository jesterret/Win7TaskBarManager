#include "Signature.h"
#include <list>

MODULEINFO PatternScanner::GetModuleInfo(const TCHAR* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0 || hModule == INVALID_HANDLE_VALUE)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}
DWORD_PTR PatternScanner::GetModuleSize(const TCHAR* szModule)
{
	auto module = GetModuleInfo(szModule);
	return module.SizeOfImage;
}

void* PatternScanner::FindPattern(const Pattern &pattern)
{
	return PatternScanner::FindPattern(GetModuleHandle(nullptr), GetModuleSize(nullptr), pattern);
}
void* PatternScanner::FindPattern(const std::wstring &ModuleName, const Pattern &pattern)
{
	auto Name = ModuleName.length() > 0 ? ModuleName.c_str() : nullptr;
	return PatternScanner::FindPattern(GetModuleHandle(Name), GetModuleSize(Name), pattern);
}
void* PatternScanner::FindPattern(void* startAddress, DWORD_PTR searchSize, const Pattern &pattern)
{
	size_t len = wcslen(pattern.pattern);
	if (len == 0)
		return nullptr;

	DWORD_PTR pos = 0;

	int byteCount = 1;
	unsigned int i = 0;
	while (i < len - 1)
	{
		if (pattern.pattern[i] == ' ')
			byteCount++;
		i++;
	}

	BYTE* patt = new BYTE[byteCount + 1];
	if (!patt)
		return nullptr;

	BYTE* mask = new BYTE[byteCount + 1];
	if (!mask)
		return nullptr;

	int offset = 0;
	int bytesCounted = 0;
	i = 0;
	while (i < len - 1)
	{
		if (pattern.pattern[i] == '[')
		{
			i++;
			offset = bytesCounted;
		}

		if (pattern.pattern[i] == '\0')
			break;

		if (pattern.pattern[i] == '?')
		{
			if (pattern.pattern[i + 1] == '?')
				i--;
			mask[bytesCounted] = '?';
			patt[bytesCounted] = '\0';
		}
		else
		{
			BYTE hn = pattern.pattern[i] > '9' ? pattern.pattern[i] - 'A' + 10 : pattern.pattern[i] - '0';
			BYTE ln = pattern.pattern[i + 1] > '9' ? pattern.pattern[i + 1] - 'A' + 10 : pattern.pattern[i + 1] - '0';
			BYTE n = (hn << 4) | ln;

			mask[bytesCounted] = 'x';
			patt[bytesCounted] = n;
		}

		bytesCounted++;

		i += 2;
		while (i < len && (pattern.pattern[i] == ' ' || pattern.pattern[i] == '\t' || pattern.pattern[i] == '\r' || pattern.pattern[i] == '\n'))
			i++;
	}
	mask[bytesCounted] = '\0';
	void* ret = PatternScanner::FindPattern((BYTE*)startAddress, searchSize, patt, mask) + offset;

	delete[] patt;
	delete[] mask;

	switch (pattern.type)
	{
	case PatternType::PointerBYTE:
		ret = PatternScanner::ResolvePtr<BYTE>(ret);
		break;

	case PatternType::PointerWORD:
		ret = PatternScanner::ResolvePtr<WORD>(ret);
		break;

	case PatternType::Pointer:
	case PatternType::PointerDWORD:
		ret = PatternScanner::ResolvePtr<DWORD>(ret);
		break;

	case PatternType::PointerQWORD:
		ret = PatternScanner::ResolvePtr<QWORD>(ret);
		break;

	case PatternType::RelativePointerBYTE:
		ret = PatternScanner::ResolveRelativePtr<BYTE>(ret);
		break;

	case PatternType::RelativePointerWORD:
		ret = PatternScanner::ResolveRelativePtr<WORD>(ret);
		break;

	case PatternType::RelativePointer:
	case PatternType::RelativePointerDWORD:
		ret = PatternScanner::ResolveRelativePtr<DWORD>(ret);
		break;

	case PatternType::RelativePointerQWORD:
		ret = PatternScanner::ResolveRelativePtr<QWORD>(ret);
		break;
	}
	return ret;
}

BYTE* PatternScanner::FindPattern(BYTE* start_pos, DWORD_PTR search_len, const BYTE* pattern, const BYTE* mask)
{
	for (BYTE *region_it = start_pos; region_it < (start_pos + search_len); ++region_it)
	{
		if (*region_it == *pattern)
		{
			const auto *pattern_it = pattern, *mask_it = mask, *memory_it = region_it;
			bool found = true;

			for (; *mask_it && (memory_it < (start_pos + search_len)); ++mask_it, ++pattern_it, ++memory_it)
			{
				if (*mask_it != 'x') continue;
				if (*memory_it != *pattern_it)
				{
					found = false;
					break;
				}
			}

			if (found)
				return region_it;
		}
	}
	return nullptr;
}