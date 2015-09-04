#ifdef _MSC_VER
#pragma once
#endif
#ifndef _WRITE_CONFIG_LIME_
#define _WRITE_CONFIG_LIME_

#include "QuickConfiguration.h"

#include <cstdio>
#include <string>

namespace QuickConfiguration 
{
	using namespace types;

	/* ����д�� */
	void WriteConfigList(const ConfigList &cl, FILE* fp);


	/* ��������д�뺯�� */
	#ifndef _CONFIG_BIG_

	void Write(const std::vector<ConfigList>& alist, FILE* fp);

	#else

	void Write(const std::list<ConfigList>& alist, FILE* fp);

	#endif

}

#endif

