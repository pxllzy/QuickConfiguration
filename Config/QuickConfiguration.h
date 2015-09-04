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
		std::string Key;/*����*/
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
		/*explicit ��ȷ���캯��*/

		/* Default:Analytical & Full Size Mode | Ĭ��:����&��ȫ��ȡ */
		explicit QuickConfig(FILE*);

		/* ����:�涨��ȡ�ֽڴ�С */
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

		/* �����ݴ�����壡 */
#ifndef _CONFIG_BIG_
		std::vector<ConfigList> List;
#else
		std::list<ConfigList> List;
#endif

	private:

		void FullReading();//������ȡ

		/* �Զ����ȡ */
		void CustomReading(unsigned int);

		/* ���� */
		void Analytical();

		/* ���ע�� */
		inline void Notes();

		/* �ʻ���� */
		inline void LexicalAnalysis();

		/* ֵ����� */
		inline void ValueAnalysis(unsigned short &bigBracket);

		inline void AddKey(unsigned int& len, std::string& s);

		inline void AddItemName(unsigned int& len, std::string& s);

		inline void AddItemValue(var& val);

		/* �ʻ� */
		inline void Lex(unsigned short bracket, std::string& s);

		/* �ַ������� */
		inline std::string StringAnalysis();


		/* 10���ƽ��� */
		inline void DecAnalysis();

		/* 16���ƽ��� */
		inline void HexAnalysis();


	protected:

		unsigned char *pch, *_p;
		FILE* _fp;


	private:

		/* ָ�� */
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
		// Parameter: const char *  ��ʶ(Name)
		//************************************
		ConfigItem(const char*);
		ConfigItem(const std::string&);

		/* ����[]����������Ϊ��ֵ */
		const var& operator [](unsigned int)const;

		/* ����[]����������Ϊ��ֵ */
		var& operator [](unsigned int);


		/* ����= ����������ֵ���� */
		ConfigItem& operator=(const ConfigItem&);

		~ConfigItem();

	public:

		/* ��ĩβ���һ��ֵ */
		bool PushBack(var &val);
		bool PushBack(int val);
		bool PushBack(const unsigned int &val);
		bool PushBack(const char* val);
		bool PushBack(const double &val);
		bool PushBack(const std::string& val);

	public:

		/* ��ʶ */
		std::string Name;
		/* ֵ */
		std::vector<var> Values;

	private:

		inline bool push(var* val);

	};


}

#endif
/*
******************************************************
*/