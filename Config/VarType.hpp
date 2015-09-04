#ifdef _MSC_VER
	#pragma once
#endif

#ifndef _VARTYPE_LIME_
#define _VARTYPE_LIME_
#include <typeinfo>
#include <string>

namespace types 
{
	/* �����ˣ�רҵ������30�� */
	class IData
	{
		public:
			virtual ~IData() { };

			/*��ȡ����->�麯��*/
			virtual const std::type_info& type() const = 0;

			/*��¡ֵ->�麯��*/
			virtual IData* clone()const = 0;

		private:

	};

	
	/* �����ļ�ֵ���� */
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
			//			��¡ֵ
			//************************************
			virtual IData* clone()const;

			//************************************
			// Method:    Value
			// FullName:  types::Data<ValueType>::Value
			// Access:    public 
			// Returns:   ValueType
			//			���ڻ�ȡֵ
			//************************************
			inline ValueType Value();

			inline ~Data() {};


		private:
			/*������ֵ*/
			ValueType _val;
	};
	

	/* ����Ӧ���� */
	class var
	{
		public:
			/*C++ 11*/
			#if __cplusplus >= 201103L || _MSC_VER >= 1700
			inline var() :_ida(nullptr) {};
			#else
			inline var() :_ida(NULL) {};
			#endif
			/*��������*/
			inline var(const var& copyvar);

			template<typename ValueType>
			inline var(const ValueType&);

			template<typename ValueType>
			inline var& operator= (const ValueType& val);

			inline var& operator=(const var&);

			inline const std::type_info& GetType()const;

			inline bool Empty();/*�Ƿ�Ϊ��*/

			template<typename ValueType>
			inline const ValueType var_cast()const;

			~var();	

		private:
			IData* _ida;
	};


	/*ʵ��*/
	template<typename ValueType>
	inline Data<ValueType>::Data(const ValueType &val)
		:_val(val) {	}


	template<typename ValueType>
	inline const std::type_info & Data<ValueType>::type() const
	{
		// TODO: �ڴ˴����� return ���
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
		// TODO: �ڴ˴����� return ���
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
		// TODO: �ڴ˴����� return ���
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
		// TODO: �ڴ˴����� return ���
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

