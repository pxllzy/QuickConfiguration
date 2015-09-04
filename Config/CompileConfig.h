
#ifndef _QUICKLY_CONFIGURATION_COMPILE_LIME_
#define _QUICKLY_CONFIGURATION_COMPILE_LIME_

#ifdef _MSC_VER
#pragma once
#endif

#include <stdexcept>



/* Safe C version case */
/* 安全C语言版本特性开关 */
#if _MSC_VER>=1700
#define _SAFE_C 1
#endif

////////////////////////////////////////////////////////////

/* Debug mode case | Visual C++ Auto Open in Debug Mode */
/* 调试开关，Visual C++ DEBUG模式下自动开启 */

//#define _DEBUG

/* 高强度完全调试 */

#ifdef _DEBUG
//#define _DEBUG_ALL
#endif

////////////////////////////////////////////////////////////

/* 用于处理大数据配置文件的开关 */
/* 但在顺序访问、随机访问时会损失性能 */
/* 使用此方式后：不再检查主键是否唯一与是否具有二义性!*/
/* 超过1W条配置文件时，建议开启 */

//#define _CONFIG_BIG_

////////////////////////////////////////////////////////////

/* WINDOWS&LINUX CASE */

//#define _LINUX_

////////////////////////////////////////////////////////////

#ifdef _CONFIG_BIG_
#include <list>	//容器一枚
#endif

////////////////////////////////////////////////////////////

#if __cplusplus >= 201103L || _MSC_VER >= 1700
#define _CX11 1
#else
#define _CX11 1
#endif

////////////////////////////////////////////////////////////

/* 标准错误处理 */
inline void Except(const char* errorInfo)
{
	std::logic_error ex(errorInfo);
	throw std::exception(ex);
}



#endif 
