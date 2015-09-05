
#include "ConfigWriter.h"
#include <typeinfo>
#include <cstdio>
#include <vector>

using namespace types;


namespace QuickConfiguration
{

	inline static std::string convertVar(const var& val);

	void WriteConfigList(const ConfigList &cl, FILE* fp);

	inline static void writeVar(const std::vector<var>& val, FILE* fp);


	inline static std::string convertVar(const var& val)
	{
		const std::type_info* x = &val.GetType();


		if (*x == typeid(int)) {
			
			/*64Bit*/
			char  str[24];
			#ifndef _SAFE_C
			sprintf(str, "%d", val.var_cast<int>());
			#else
			sprintf_s(str, "%d", val.var_cast<int>());
			#endif

			return std::string(str);
		}

		else if (*x == typeid(double)) {
			
			
			char  str[40];
			#ifndef _SAFE_C
			sprintf(str, "%.8lf", val.var_cast<double>());
			#else
			sprintf_s(str, "%.8lf", val.var_cast<double>());
			#endif
			return std::string(str);
		}

		else if(*x == typeid(unsigned int)) {

			/*64Bit*/
			char  str[24];
			#ifndef _SAFE_C
			sprintf(str, "0x%x", val.var_cast<unsigned int>());
			#else
			sprintf_s(str, "0x%x", val.var_cast<unsigned int>());
			#endif
			return std::string(str);
		}

		else if (*x == typeid(std::string)) {
			std::string str = '\"' + val.var_cast<std::string>() + '\"';
			return str;
		}

		else if (*x == typeid(bool)) {
			if (val.var_cast<bool>()) 
				return std::string("true");
			else
				return std::string("false");
		}

		else if (*x == typeid(char)) {
			char str[2] = { 0 };
			str[0] = val.var_cast<char>();
			return std::string(str);
		}

		else if (*x == typeid(unsigned char)) {
			char str[2] = { 0 };
			str[0] = val.var_cast<unsigned char>();
			return std::string(str);
		}

		else if (*x == typeid(float)) {
			char  str[32];
			#ifndef _SAFE_C
			sprintf(str, "%.6f", val.var_cast<unsigned int>());
			#else
			sprintf_s(str, "%.6f", val.var_cast<unsigned int>());
			#endif
			return std::string(str);
		}

		else if(*x == typeid(short)) {

			char  str[8];
			#ifndef _SAFE_C
			sprintf(str, "%d", val.var_cast<short>());
			#else
			sprintf_s(str, "%d", val.var_cast<short>());
			#endif
			return std::string(str);
		}

		else if (*x==typeid(unsigned short)) {

			char  str[8];
			#ifndef _SAFE_C
			sprintf(str, "%d", val.var_cast<unsigned short>());
			#else
			sprintf_s(str, "%d", val.var_cast<unsigned short>());
			#endif
			return std::string(str);
		}

		else {
			/* Warning */
			#ifdef _DEBUG
				Except("数据类型有误!");
			#endif
		}
		/* Bug */
		return NULL;
	}


	void WriteConfigList(const ConfigList &cl, FILE* fp)
	{
		std::string str = cl.Key + " {";
		fputs(str.c_str(), fp);
		unsigned int len = cl.Data.size();
		if (!len) {
			;
		}
		else if (len == 1) {
			#ifdef _LINUX_
			fputs("\n\t", fp);

			#else
			fputs("\r\n\t", fp);

			#endif

			fputs(cl.Data[0].Name.c_str(), fp);
			fputs(" : ", fp);
			writeVar(cl.Data[0].Values, fp);
		}
		else {
			for (unsigned int i = 0;i < len - 1;i++) {
				#ifdef _LINUX_
				fputs("\n\t", fp);

				#else
				fputs("\r\n\t", fp);

				#endif

				fputs(cl.Data[i].Name.c_str(), fp);
				fputs(" : ", fp);
				writeVar(cl.Data[i].Values, fp);
				fputc(',', fp);
			}
			#ifdef _LINUX_
			fputs("\n\t", fp);

			#else
			fputs("\r\n\t", fp);

			#endif

			fputs(cl.Data[len - 1].Name.c_str(), fp);
			fputs(" : ", fp);
			writeVar(cl.Data[len - 1].Values, fp);
		}
		#ifdef _LINUX_

		fputs("\n}\n", fp);
		
		#else

		fputs("\r\n}\r\n", fp);

		#endif
	}


	inline static void writeVar(const std::vector<var>& val, FILE* fp)
	{
		unsigned int vallen = val.size();
		if (!vallen) {
			return;
		}
		else if (vallen == 1) {
			fputs(convertVar(val[0]).c_str(), fp);
		}
		else {
			fputc('[', fp);
			for (unsigned int j = 0;j < vallen - 1;j++)
			{
				fputs(convertVar(val[j]).c_str(), fp);
				fputc(',', fp);
			}
			fputs(convertVar(val[vallen - 1]).c_str(), fp);
			fputc(']', fp);
		}
	}


	#ifndef _CONFIG_BIG_

	void Write(const std::vector<ConfigList>& alist, FILE* fp)
	{
		rewind(fp);
		unsigned int len = alist.size();
		for (unsigned int i = 0;i < len;i++) {
			WriteConfigList(alist[i], fp);
		}
	}

	#else

	void Write(const std::list<ConfigList>& alist, FILE* fp)
	{
		rewind(fp);
		//去除Const
		std::list<ConfigList>* plist = const_cast<std::list<ConfigList>*>(&alist);

		std::list<ConfigList>::iterator p_list_tor = plist->begin();
		for (;p_list_tor != alist.end();++p_list_tor) {
			WriteConfigList(*p_list_tor, fp);
		}
	}

	#endif
	
}
/*
******************************************************
*/
