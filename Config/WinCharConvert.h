/*#pragma once*/

#ifndef _WINCHARCONVERT_LIME_
#define _WINCHARCONVERT_LIME_

typedef struct{
	unsigned char* tag;
	int len;
}XCharPoint;

#define SetXCharPoint(Xchar,str) Xchar.tag=str;Xchar.len=sizeof(str)

class WinCharConvert
{
	public:
		
		std::wstring UTF82Unicode(const std::string&);	/* UTF-8 to Unicode | UTF-8 转换为 Unicode */

		std::string WideByte2ANSI(std::wstring&);	/* Unicode to ANSI | Unicode 转换为 ANSI */

		std::string UTF82ANSI(std::string&);	/* UTF-8 to ANSI | UTF-8 转换为 ANSI */

		std::string UTF82ANSIEx(std::string&);	/* UTF-8 to ANSI & Clean BOM | UTF-8 转换为 ANSI & 清除BOM */

		std::wstring ANSI2WideByte(std::string&);	/* ANSI to Unicode | ANSI 转换为 Unicode */

		std::string Unicode2UTF8(const std::wstring&);	/* Unicode to UTF-8 | Unicode 转换为 UTF-8 */

		std::string ANSI2UTF8(std::string&);	/* ANSI to UTF-8 | ANSI 转换为 UTF-8 */

		bool CheckUTF8BOM(XCharPoint);	/* (Head: EF BB BF) Check UTF-8 contains BOM | 检查UTF-8 BOM头 */
		bool CheckUTF8BOM(unsigned char* tag, int len);

		WinCharConvert() {}
		~WinCharConvert() {}
	private:
		void ExceptionChar(int);	/*错误处理 | Exception throw*/
};



#endif

