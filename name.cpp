#include"name.h"
#include"md5.h"
#include<string>
using namespace std;
string md(string strPlain)
{
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	MD5Init(&mdContext);
	MD5Update(&mdContext, (unsigned char*)const_cast<char*>(strPlain.c_str()), strPlain.size());
	MD5Final(&mdContext);

	string md5;
	char buf[3];
	for (int i = 0; i < 16; i++)
	{
		sprintf(buf, "%02x", mdContext.digest[i]);
		md5.append(buf);
	}
	return md5;
}