#include <iostream>
#include <cstdio>

using namespace std;

int main()
{
	string a = "{\ndemo:", b = ",class:[11220,0x755,\"xxx\"],", c = "test:110\n}\n";
	FILE* fp;
	if((fp=fopen("./a.txt", "w"))==NULL) {
		cout << "Error";
	}
	for (size_t i = 1000000; i;i--) {
		fprintf(fp, "lime%d%s%d%s%s", i, a.c_str(), i+i, b.c_str(), c.c_str());
	}
	fclose(fp);
	return 0;
}