#ifdef _MSC_VER
	#pragma once
#endif

#ifndef _VARTYPE_LIME_
#define _VARTYPE_LIME_
#include <typeinfo>
#include <string>

namespace types 
{
	/* 稻草人，专业做靶子30年 */
	class IData
	{
		public:
			virtual ~IData() { };

			/*获取类型->虚函数*/
			virtual const std::type_info& type() const = 0;

			/*克隆值->虚函数*/
			virtual IData* clone()const = 0;

		private:

	};

	
	/* 真正的价值所在 */
	template<typename ValueType>
	class Data : public IData
	{
		public:
			inline Data(const ValueType& val);
			
			virtual const std::type_info& type()const;

			//************************************
			// Method:    clone
			// FullName:  types::Data<ValueType>::clone
			// Access:    virtual public 
			// Returns:   IData*
			// Qualifier: const
			//			克隆值
			//************************************
			virtual IData* clone()const;

			//************************************
			// Method:    Value
			// FullName:  types::Data<ValueType>::Value
			// Access:    public 
			// Returns:   ValueType
			//			用于获取值
			//************************************
			inline ValueType Value();

			inline ~Data() {};


		private:
			/*真正的值*/
			ValueType _val;
	};
	

	/* 自适应类型 */
	class var
	{
		public:
			/*C++ 11*/
			#if __cplusplus >= 201103L || _MSC_VER >= 1700
			inline var() :_ida(nullptr) {};
			#else
			inline var() :_ida(NULL) {};
			#endif
			/*拷贝函数*/
			inline var(const var& copyvar);

			template<typename ValueType>
			inline var(const ValueType&);

			template<typename ValueType>
			inline var& operator= (const ValueType& val);

			inline var& operator=(const var&);

			inline const std::type_info& GetType()const;

			inline bool Empty();/*是否为空*/

			template<typename ValueType>
			inline const ValueType var_cast()const;

			~var();	

		private:
			IData* _ida;
	};


	/*实现*/
	template<typename ValueType>
	inline Data<ValueType>::Data(const ValueType &val)
		:_val(val) {	}


	template<typename ValueType>
	inline const std::type_info & Data<ValueType>::type() const
	{
		// TODO: 在此处插入 return 语句
		return typeid(ValueType);
	}


	template<typename ValueType>
	inline IData * Data<ValueType>::clone() const
	{
		return new Data<ValueType>(_val);
	}


	template<typename ValueType>
	inline ValueType Data<ValueType>::Value()
	{
		return _val;
	}


	template<typename ValueType>
	inline var::var(const ValueType &val)
		:_ida(new Data<ValueType>(val))
	{	}


	template<typename ValueType>
	inline var& var::operator=(const ValueType &val)
	{
		// TODO: 在此处插入 return 语句
		if (_ida) {
			delete _ida;
		}
		_ida = new Data<ValueType>(val);
		return *this;
	}


	template<typename ValueType>
	inline const ValueType var::var_cast()const
	{
		if (this->GetType()==typeid(ValueType)){
			Data<ValueType>* dt = (Data<ValueType>*)this->_ida;
			return dt->Value();
		}
		return NULL;
	}


	inline var::var(const var & copyvar)
	{
		_ida = copyvar._ida->clone();
	}

	inline var& var::operator=(const var& val)
	{
		// TODO: 在此处插入 return 语句
		if (this != &val) {
			if (_ida) {
				delete _ida;
			}
			_ida = val._ida->clone();
		}
		return *this;
	}


	inline const std::type_info & var::GetType() const
	{
		// TODO: 在此处插入 return 语句
		return _ida ? _ida->type() : typeid(void);
	}


	inline bool var::Empty()
	{
		return !_ida;
	}


	inline var::~var()
	{
		delete _ida;
	}

}

#endif

