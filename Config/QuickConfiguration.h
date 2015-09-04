/*#pragma once*/
#ifndef _QUICKCONFIGURATION_LIME_
#define _QUICKCONFIGURATION_LIME_

#ifdef _MSC_VER
#pragma warning(disable:4514)
#pragma warning(disable:4710)
#pragma once
#endif



#include "VarType.hpp"
#include "CompileConfig.h"

#include <vector>
#include <cstdio>
#include <string>


namespace QuickConfiguration {

	using namespace types;


	class ConfigItem;

	typedef struct {
		std::string Key;/*主键*/
		std::vector<ConfigItem> Data;
	}ConfigList;

	class ArrayList;

	class QuickConfig
	{
	public:
		/*
			Default:
				Reader Mode: Analytical
				Reader Size: Full
		*/
		/*explicit 明确构造函数*/

		/* Default:Analytical & Full Size Mode | 默认:解析&完全读取 */
		explicit QuickConfig(FILE*);

		/* 附加:规定读取字节大小 */
		explicit QuickConfig(FILE*, unsigned int);

		ConfigList& operator [](const unsigned int)const;

#ifndef _CONFIG_BIG_
		ConfigList& operator [](unsigned int);
#endif


		void AddList(const ConfigList&);


		~QuickConfig();

#ifndef _CONFIG_BIG_
		const std::vector<ConfigList>& GetList()const;
#else
		const std::list<ConfigList>& GetList()const;
#endif

		ConfigList* FindKey(std::string& KeyName);

	public:

		/* 大数据处理奥义！ */
#ifndef _CONFIG_BIG_
		std::vector<ConfigList> List;
#else
		std::list<ConfigList> List;
#endif

	private:

		void FullReading();//完整读取

		/* 自定义读取 */
		void CustomReading(unsigned int);

		/* 解析 */
		void Analytical();

		/* 检测注释 */
		inline void Notes();

		/* 词汇解析 */
		inline void LexicalAnalysis();

		/* 值域解析 */
		inline void ValueAnalysis(unsigned short &bigBracket);

		inline void AddKey(unsigned int& len, std::string& s);

		inline void AddItemName(unsigned int& len, std::string& s);

		inline void AddItemValue(var& val);

		/* 词汇 */
		inline void Lex(unsigned short bracket, std::string& s);

		/* 字符串解析 */
		inline std::string StringAnalysis();


		/* 10进制解析 */
		inline void DecAnalysis();

		/* 16进制解析 */
		inline void HexAnalysis();


	protected:

		unsigned char *pch, *_p;
		FILE* _fp;


	private:

		/* 指针 */
		ConfigList* plist;
	};

	class ConfigItem
	{
	public:


		//************************************
		// Method:    ConfigItem
		// FullName:  QuickConfig::ConfigItem::ConfigItem
		// Access:    public 
		// Returns:   
		// Qualifier:
		// Parameter: const char *  标识(Name)
		//************************************
		ConfigItem(const char*);
		ConfigItem(const std::string&);

		/* 重载[]操作符，作为右值 */
		const var& operator [](unsigned int)const;

		/* 重载[]操作符，作为左值 */
		var& operator [](unsigned int);


		/* 重载= 操作符，赋值运算 */
		ConfigItem& operator=(const ConfigItem&);

		~ConfigItem();

	public:

		/* 在末尾添加一个值 */
		bool PushBack(var &val);
		bool PushBack(int val);
		bool PushBack(const unsigned int &val);
		bool PushBack(const char* val);
		bool PushBack(const double &val);
		bool PushBack(const std::string& val);

	public:

		/* 标识 */
		std::string Name;
		/* 值 */
		std::vector<var> Values;

	private:

		inline bool push(var* val);

	};


}

#endif
/*
******************************************************
*/