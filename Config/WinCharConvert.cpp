#if defined(_WINDOWS) || defined(_WIN64) || defined(_WIN32) || defined(_WIN16)

#include <string>  
#include <windows.h>  
#include <vector>  
#include "WinCharConvert.h"

/*
* ANSI->ASCII扩展
*/

/*
* USE Windows API | 使用下列Windows API
*******************

int MultiByteToWideChar(UINT CodePage, DWORD dwFlags,
						LPCSTR lpMultiByteStr, int cchMultiByte,
						LPWSTR lpWideCharStr, int cchWideChar );

int	WideCharToMultiByte(_In_ UINT CodePage, _In_ DWORD dwFlags,
						_In_NLS_string_(cchWideChar) LPCWCH lpWideCharStr, _In_ int cchWideChar,
						_Out_writes_bytes_to_opt_(cbMultiByte, return) LPSTR lpMultiByteStr,
						_In_ int cbMultiByte, _In_opt_ LPCCH lpDefaultChar,
						_Out_opt_ LPBOOL lpUsedDefaultChar );

*******************
*/

#define CError "Error."
#define CEINC "Error in conversion."



/* (Head: EF BB BF) Check UTF-8 contains BOM | 检查UTF-8 BOM头 */
bool WinCharConvert::CheckUTF8BOM(XCharPoint headChars)
{
	if (headChars.len < 3) {
		throw std::exception("Insufficient Elements");
	}
	if (headChars.tag[0] == 0xEF && headChars.tag[1] == 0xBB && headChars.tag[2] == 0xBF)
		return true;
	return false;
}

bool WinCharConvert::CheckUTF8BOM(unsigned char * tag, int len)
{
	if (len < 3) {
		throw std::exception("Insufficient Elements");
	}
	if (tag[0] == 0xEF && tag[1] == 0xBB && tag[2] == 0xBF)
		return true;
	return false;
}


/* UTF-8 to Unicode | UTF-8 转换为 Unicode */
std::wstring WinCharConvert::UTF82Unicode(const std::string& UTF8string)
{
	int wideCharSize = ::MultiByteToWideChar(CP_UTF8, 0, UTF8string.c_str(), -1, NULL, 0);

	ExceptionChar(wideCharSize);

	std::vector<wchar_t> resultStr(wideCharSize);

	int convertSize = ::MultiByteToWideChar(CP_UTF8, 0, UTF8string.c_str(), -1, &resultStr[0], wideCharSize);

	if (convertSize != wideCharSize)
		throw std::exception(CError);

	return std::wstring(&resultStr[0]);
}


/* Unicode to ANSI | Unicode 转换为 ANSI */ 
std::string WinCharConvert::WideByte2ANSI(std::wstring& wstrcode)
{
	int ANSIsize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	
	ExceptionChar(ANSIsize);

	std::vector<char> resultStr(ANSIsize);
	int convertSize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultStr[0], ANSIsize, NULL, NULL);

	if (convertSize != ANSIsize)
		throw std::exception(CError);

	return std::string(&resultStr[0]);
}


/* UTF-8 to ANSI | UTF-8 转换为 ANSI */  
std::string WinCharConvert::UTF82ANSI(std::string& strUTF8Code)
{
	std::string strRet("");
	//先把 UTF8 转为 unicode  
	std::wstring wstr = UTF82Unicode(strUTF8Code);
	//最后把 unicode 转为 ANSI  
	strRet = WideByte2ANSI(wstr);
	return strRet;
}


std::string WinCharConvert::UTF82ANSIEx(std::string& strUTF8Code)
{
	XCharPoint xcp;
	//SetXCharPoint(xcp, (unsigned char*)strUTF8Code.c_str());
	xcp.tag = (unsigned char*)strUTF8Code.c_str();
	xcp.len = strUTF8Code.size();
	if (CheckUTF8BOM(xcp))
		strUTF8Code = &strUTF8Code[3];/*去掉BOM，从第四个字节开始*/
	return UTF82ANSI(strUTF8Code);
}


/* ANSI to Unicode | ANSI 转换为 Unicode */
std::wstring WinCharConvert::ANSI2WideByte(std::string& strANSI)
{
	int wideCharSize = MultiByteToWideChar(CP_ACP, 0, (char*)strANSI.c_str(), -1, NULL, 0);

	ExceptionChar(wideCharSize);

	std::vector<wchar_t> resultStr(wideCharSize);
	int convertSize = MultiByteToWideChar(CP_ACP, 0, (char*)strANSI.c_str(), -1, &resultStr[0], wideCharSize);

	if (convertSize != wideCharSize)
		throw std::exception(CError);

	return std::wstring(&resultStr[0]);
}


/* Unicode to UTF-8 | Unicode 转换为 UTF-8 */
std::string WinCharConvert::Unicode2UTF8(const std::wstring& wideString)
{
	int uSize = ::WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, NULL, 0, NULL, NULL);
	if (uSize == 0)
		throw std::exception(CEINC);

	std::vector<char> resultStr(uSize);

	int convertSize = ::WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &resultStr[0], uSize, NULL, NULL);

	if (convertSize != uSize)
		throw std::exception(CError);

	return std::string(&resultStr[0]);
}


/* ANSI to UTF-8 | ANSI 转换为 UTF-8 */
std::string WinCharConvert::ANSI2UTF8(std::string& strANSICode)
{
	std::string resultStr("");
	//先把 ANSI 转为 Unicode  
	std::wstring wstr = ANSI2WideByte(strANSICode);
	//最后把 Unicode 转为 UTF8  
	resultStr = Unicode2UTF8(wstr);
	return resultStr;
}


/* 错误处理 | Exception throw */
void WinCharConvert::ExceptionChar(int wideCharSize)
{
	/*序列无效*/
	if (wideCharSize == ERROR_NO_UNICODE_TRANSLATION)
		throw std::exception("Sequence is invalid.");
	/*转换失败*/
	if (wideCharSize == 0)
		throw std::exception(CEINC);
}


#endif