//#include "Reader.h"
#include <iostream>
#include "QuickConfiguration.h"
#include "VarType.hpp"
#include <ctime>
#include "ConfigWriter.h"

using namespace types;
using namespace QuickConfiguration;

#define FileName "a.txt"
#define FileName2 "write.txt"

int main(int argc, char* argv[])
{
	unsigned int a = clock();
	var y;
	//std::string xx = "1000asdasdasdasdastrhyhgkuiiolsdafesamfsdvbadfh00";
	var x = 888;

	//ConfigItem cf("Love");
	//var val = x;
	//cf.PushBack(100);
	//cf[0] = x;
	//int intval = cf[0].var_cast<int>();
	//std::cout << intval << "," << cf.Name;
	/*var lime = 10086;
	printf("%d,,,,", lime.var_cast<int>());
	lime = 'L';
	printf("%c", lime.var_cast<char>());
	lime = std::string("нт╡ш");
	std::cout << lime.var_cast<std::string>();
	getchar();
	return 0;*/
	FILE *fp;
	if ((fp = fopen(FileName, "rb")) == NULL) {
		std::cout << "FILE OPEN ERROR";
		return -1;
	}

	QuickConfig qc(fp);

	fclose(fp);
	/*fp = NULL;
	if ((fp = fopen(FileName2, "wb")) == NULL) {
		std::cout << "FILE OPEN ERROR";
		return -1;
	}
	Write(qc.GetList(), fp);*/
	ConfigList xxxxxx = qc[5];
	unsigned int b = clock();
	std::cout << b - a;
	getchar();
	return 0;
}


