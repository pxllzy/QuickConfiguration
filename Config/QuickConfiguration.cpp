

#include "QuickConfiguration.h"


namespace QuickConfiguration {

	QuickConfig::QuickConfig(FILE* fp) :_fp(fp), pch(NULL), plist(NULL)
	{
		this->FullReading();

		/*解析模式*/
		this->Analytical();
	}

	QuickConfig::QuickConfig(FILE* fp, unsigned int size) :_fp(fp), pch(NULL), plist(NULL)
	{
		this->CustomReading(size);

		/*解析模式*/
		this->Analytical();
	}

	ConfigList& QuickConfig::operator[](const unsigned int i) const
	{
		
		#ifndef _CONFIG_BIG_
		std::vector<ConfigList>* p_list = const_cast<std::vector<ConfigList>*>(&List);
		#ifdef _DEBUG
		if (List.size() > i) {
			return (*p_list)[i];
		}
		Except("Out of range!");
		#else
		return (*p_list)[i];
		#endif // _DEBUG

		#else
		std::list<ConfigList>* p_list = const_cast<std::list<ConfigList>*>(&List);
		std::list<ConfigList>::iterator p_tor = p_list->begin();
		#ifdef _DEBUG
		if (p_list->size() <= i)
			Except("Out of range!");
		#endif // _DEBUG
		unsigned int j = i;
		while (j--)
			++p_tor;
		return *p_tor;
		#endif


	}

	ConfigList & QuickConfig::operator[](unsigned int i)
	{
		// TODO: 在此处插入 return 语句
		return List[i];
	}


	void QuickConfig::AddList(const ConfigList& cl)
	{
		this->List.push_back(cl);
	}



	QuickConfig::~QuickConfig()
	{
		if(pch)
			delete pch;
 		if(plist)
 			delete plist;
		//_list.clear();
		plist = NULL;
	}


	void QuickConfig::FullReading()
	{
		fseek(_fp, 0, SEEK_END);//指针移动到末尾
		unsigned int len = (unsigned int)ftell(_fp);
		fseek(_fp, 0, SEEK_SET);//指针移动到开始
		if (len < 1) {
			pch = NULL;
			Except("File is empty !");//文件为空
		}
		pch = new unsigned char[len + (unsigned int)1]();//()用于初始化，并置为0
		fread(pch, sizeof(unsigned char), len, _fp);
		pch[len] = '\0';
	}

	void QuickConfig::CustomReading(unsigned int size)
	{
		if (size == 0) {
			this->FullReading();
		}
		else {
			fseek(_fp, 0, SEEK_SET);//指针移动到开始
			pch = new unsigned char[size + 1]();//分配看见并初始化为0
			fread(pch, sizeof(unsigned char), size, _fp);
			pch[size] = '\0';
		}
	}


	void QuickConfig::Analytical()
	{
		_p = pch;
		LexicalAnalysis();
		delete pch;
		pch = NULL;
		_p = NULL;
	}


	inline void QuickConfig::Notes()
	{
		_p++;
		do {
			_p++;
			if (*_p == '*'&&*(_p + 1) == '/') {
				_p++;
				return;
			}
		} while (*_p);
		/////
		_p--;
	}


	inline void QuickConfig::LexicalAnalysis()
	{

		#ifdef _DEBUG_ALL
			FILE* pdebuglog = fopen("debug.log", "w");
		#endif

		unsigned short bracket = 0;
		std::string s = "";
		while (*_p) {
			/**********************************************/
			#ifdef _DEBUG_ALL
				fprintf(pdebuglog,"0x%02x , 0x%08x\n", *_p, _p);
			#endif 
			/**********************************************/
			switch (*_p)
			{
					/*********************************/
				case ' ':	case '\n':	case '\r':	case 0x9:
					if (s == "")
						break;
					Lex(bracket, s);
					break;

					/*********************************/
				case '{':
					if (s != "") {
						unsigned int len = List.size();
						if (!bracket) {
							bracket++;
							AddKey(len, s);
						}
						else {
							bracket++;
							/* 语法错误 */
						}
						s = "";
					}
					else {
						bracket++;
					}
					break;

					/*********************************/
				case ':': {
					if (!bracket) {
						/* 语法错误 */
						Except("大括号有误");
					}
					else {
						/* 处理 */
						if (s == "") {
							ValueAnalysis(bracket);
						}
						else {
							unsigned int len = List.size();
							AddItemName(len, s);
							ValueAnalysis(bracket);
						}
					}
					break;
				}
					/*********************************/
				case '/': {
					if (s != "") {
						Lex(bracket, s);
					}
					if (*(_p + 1) == '*') {
						Notes();
					}
					else {
						/* 语法错误 */
					}
					break;
				}

					/*********************************/


				case '[':	case '<':	case '|':	case '\\':
				case '~':	case '`':	case '!':	case '#':
				case '$':	case '%':	case '+':	case '-':
				case '*':	case '.':	case '^':	case '&':
				case '(':	case ')':	case ']':	case ',':
				case '?':	case '>':	case '@':	case '=':
					Except("符号有误");
					/* 语法错误 */
					break;

					/*********************************/
				case '"':

					break;
					/*********************************/

				case '}':
					if (!bracket) {
						Except("语法错误");
					}
					List.push_back(*plist);
					delete plist;
					plist = NULL;
					bracket--;
					break;
					/*********************************/
				default:
					s += (char)*_p;
					break;
				}
				_p++;
			}
			#ifdef _DEBUG_ALL
				fclose(pdebuglog);
			#endif
	}

	inline void QuickConfig::ValueAnalysis(unsigned short &bigBracket)
	{
		_p++;
		bool isbracket = false;
		while (*_p) {
			switch (*_p)
			{
			case '\"': {
				var v(StringAnalysis());
				AddItemValue(v);
				break;
			}
			case '0':
				HexAnalysis();
				break;

			case '1':	case '2':	case '3':
			case '4':	case '5':	case '6':
			case '7':	case '8':	case '9':
			case '-':
				DecAnalysis();
				break;

			case 't':
				if (*(_p + 1) == 'r'&&*(_p + 2) == 'u'&&*(_p + 3) == 'e') {
					var v = true;
					_p += 3;
					AddItemValue(v);
				}
				else {
					/* 语法错误 */
				}
				break;
			case 'f':
				if (*(_p + 1) == 'a'&&*(_p + 2) == 'l'&&*(_p + 3) == 's'&&*(_p + 4) == 'e') {
					var v = false;
					AddItemValue(v);
					_p += 4;
				}
				else {
					/* 语法错误 */
				}
				break;
			case '[':
				if (isbracket) {
					/* 语法错误 */
				}
				else {
					isbracket = true;
				}
				break;

			case ']':
				if (!isbracket) {
					/* 语法错误 */
				}
				else {
					isbracket = false;
				}
				break;

			case ',':
				if (!isbracket) {
					return;
				}
				break;

			case '\\':
				Notes();
				break;
			case '\n':	case '\r':	case ' ':	case 0x9:
				/* 忽略 */
				break;

			case '}': {
				bigBracket = 0;
				List.push_back(*plist);
				delete plist;
				plist = NULL;
				return;
			}

			default:
				/* 语法错误 */
				//Except("值域解析异常");
				break;
			}
			_p++;
		}
		_p--;
	}

	inline void QuickConfig::AddKey(unsigned int& len, std::string& s)
	{
		#ifndef _CONFIG_BIG_
			if (len) {
				for (unsigned int i = 0; i < len; i++) {
					if (List[i].Key == s) {
						s += ":被重定义!";
						Except(s.c_str());
					}
				}
			}
			if (plist) {
				s += ":具有二义性!(主键被连续定义多次!)";
				Except(s.c_str());
			}
			#if _DEBUG
			delete plist;
			#endif
			plist = new ConfigList;
			plist->Key = s;

			s = "";
		#else
		#if _DEBUG
				delete plist;
			//plist = NULL;
			#endif
			plist = new ConfigList;
			plist->Key = s;
			s = "";
		#endif
	}

	inline void QuickConfig::AddItemName(unsigned int& len, std::string& s)
	{
		if (!plist) {
			Except("主键未指定");
		}
		#ifdef _DEBUG_ALL

		#endif

		ConfigItem cfi(s);
		plist->Data.push_back(cfi);
		s = "";
	}

	inline void QuickConfig::AddItemValue(var& val)
	{
		#ifdef _DEBUG
		if (!plist) {
			Except("plist is NULL!");
		}
		#endif

		plist->Data[plist->Data.size() - 1].Values.push_back(val);

		
	}

	inline void QuickConfig::Lex(unsigned short bracket, std::string & s)
	{
		unsigned int len = List.size();
		if (!bracket) {
			AddKey(len, s);
		}
		else {
			AddItemName(len, s);
		}
	}

	inline std::string QuickConfig::StringAnalysis()
	{
		*_p++;
		std::string str = "";
		while (*_p) {
			if (*_p == '\"') {
				/* 返回完整字符串 */
				return str;
			}
			else if (*_p == '\\') {
				_p++;
				switch (*_p)
				{
				case 'a':
					str += '\a';
					break;
				case 'b':
					str += '\b';
					break;
				case 'f':
					str += '\f';
					break;
				case 'n':
					str += '\n';
					break;
				case 'r':
					str += '\r';
					break;
				case 't':
					str += '\t';
					break;
				case 'v':
					str += '\v';
					break;
				case '\\':
					str += '\\';
					break;
				case '\'':
					str += '\'';
					break;
				case '\"':
					str += '\"';
					break;

				default:
					str += *(_p - 1);
					_p--;
					break;
				}
			}
			else {
				str += *_p;
			}
			_p++;
		}
		/////
		_p--;
		return str;
	}
	#ifndef _CONFIG_BIG_

	const std::vector<ConfigList>& QuickConfig::GetList()const
	{
		return List;
	}

	#else

	const std::list<ConfigList>& QuickConfig::GetList()const
	{
		// TODO: 在此处插入 return 语句
		return List;
	}
	#endif
	

	ConfigList* QuickConfig::FindKey(std::string& KeyName)
	{

		#ifndef _CONFIG_BIG_

		size_t len = this->List.size();
		for (size_t  i = 0; i < len; i++) {
			if (this->List[i].Key == KeyName)
				return &List[i];
		}

		#else

		std::list<ConfigList>::iterator p_cltor = List.begin();
		while (p_cltor!= this->List.end()){
			if (p_cltor->Key == KeyName) {
				return &(*p_cltor);
			}
			p_cltor++;
		}

		#endif

		return NULL;

	}


	inline void QuickConfig::DecAnalysis()
	{
		std::string sum = "";
		short x = 0;
		if (*_p == '-') {
			sum += *_p;
			_p++;
		}
		var vsum;
		while (*_p) {
			if ((*_p >= '0'&&*_p <= '9') || *_p == '.') {
				if (*_p == '.')
					x++;
				sum += *_p;
			}
			else {
				if (sum != "") {
					if (!x) {
						/**/
						vsum = atoi(sum.c_str());
						AddItemValue(vsum);
						_p--;
						return;
					}
					else if (x == 1) {
						/**/
						vsum = atof(sum.c_str());
						AddItemValue(vsum);
						_p--;
						return;
					}
					else {
						/* 语法有误 */
					}
				}
				return;
			}
			_p++;
		}
		//////////////////
		_p--;
	}

	inline void QuickConfig::HexAnalysis()
	{
		std::string sum = "0x";
		_p++;
		if (*_p == 'x' || *_p == 'X') {
			_p++;
			while (*_p) {
				if ((*_p >= 'A'&&*_p <= 'F') || (*_p >= 'a'&&*_p <= 'f') || (*_p >= '0'&&*_p <= '9')) {
					sum += *_p;
				}
				else 
				{
					if (sum != "0x") {
						unsigned int isum;

						#ifndef _SAFE_C
						sscanf(sum.c_str(), "%x", &isum);
						#else
						sscanf_s(sum.c_str(), "%x", &isum);
						#endif

						var vsum(isum);
						AddItemValue(vsum);
						_p--;
					}
					return;
				}
				_p++;
			}
			//////
			_p--;
		}
		else {
			_p--;
			DecAnalysis();
		}
	}


	ConfigItem::ConfigItem(const char * name)
	{
		Name = name;
	}

	ConfigItem::ConfigItem(const std::string& name)
	{
		Name = name;
	}


	const var& ConfigItem::operator[](unsigned int address) const
	{
		#ifdef _DEBUG
			if (Values.size() > address) {
				return this->Values[address];
			}
			else {
				/* 超出数组范围 */
				std::logic_error ex("Beyond the range of the array!");
				throw std::exception(ex);
			}
		#endif

		#ifndef _DEBUG
			return this->Values[address];
		#endif // !_DEBUG
	}

	var& ConfigItem::operator[](unsigned int address)
	{
		#ifdef _DEBUG
			if (Values.size() > address) {
				return Values[address];
			}
			else {
				/* 超出数组范围 */
				std::logic_error ex("Beyond the range of the array!");
				throw std::exception(ex);
			}
		#endif
		
		#ifndef _DEBUG
			return Values[address];
		#endif
	}


	ConfigItem& ConfigItem::operator=(const ConfigItem& other)
	{
		// TODO: 在此处插入 return 语句
		if (this != &other) {
			this->Name = other.Name;
			this->Values = other.Values;
		}
		return *this;
	}


	bool ConfigItem::PushBack(var &val)
	{
		return this->push(&val);
	}

	bool ConfigItem::PushBack(int val)
	{
		return this->push(new var(val));
	}

	bool ConfigItem::PushBack(const unsigned int& val)
	{
		return this->push(new var(val));
	}

	bool ConfigItem::PushBack(const char* val)
	{
		std::string x = val;
		return this->push(new var(x));
	}

	bool ConfigItem::PushBack(const double& val)
	{
		return this->push(new var(val));
	}

	bool ConfigItem::PushBack(const std::string& val)
	{
		return this->push(new var(val));
	}


	ConfigItem::~ConfigItem()
	{
		Values.clear();
	}

	inline bool ConfigItem::push(var* val)
	{
		unsigned int size_ = Values.size();
		Values.push_back(val);
		if (size_ < Values.size()) {
			return true;
		}
		return false;
	}

}
/*
***************************************************************
*/