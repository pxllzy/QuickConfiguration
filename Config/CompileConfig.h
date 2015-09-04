
#ifndef _QUICKLY_CONFIGURATION_COMPILE_LIME_
#define _QUICKLY_CONFIGURATION_COMPILE_LIME_

#ifdef _MSC_VER
#pragma once
#endif

#include <stdexcept>



/* Safe C version case */
/* ��ȫC���԰汾���Կ��� */
#if _MSC_VER>=1700
#define _SAFE_C 1
#endif

////////////////////////////////////////////////////////////

/* Debug mode case | Visual C++ Auto Open in Debug Mode */
/* ���Կ��أ�Visual C++ DEBUGģʽ���Զ����� */

//#define _DEBUG

/* ��ǿ����ȫ���� */

#ifdef _DEBUG
//#define _DEBUG_ALL
#endif

////////////////////////////////////////////////////////////

/* ���ڴ�������������ļ��Ŀ��� */
/* ����˳����ʡ��������ʱ����ʧ���� */
/* ʹ�ô˷�ʽ�󣺲��ټ�������Ƿ�Ψһ���Ƿ���ж�����!*/
/* ����1W�������ļ�ʱ�����鿪�� */

//#define _CONFIG_BIG_

////////////////////////////////////////////////////////////

/* WINDOWS&LINUX CASE */

//#define _LINUX_

////////////////////////////////////////////////////////////

#ifdef _CONFIG_BIG_
#include <list>	//����һö
#endif

////////////////////////////////////////////////////////////

#if __cplusplus >= 201103L || _MSC_VER >= 1700
#define _CX11 1
#else
#define _CX11 1
#endif

////////////////////////////////////////////////////////////

/* ��׼������ */
inline void Except(const char* errorInfo)
{
	std::logic_error ex(errorInfo);
	throw std::exception(ex);
}



#endif 
