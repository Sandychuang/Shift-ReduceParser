#include <fstream>
#include <iostream>
#include <string>
#include <cstring> 
#include <time.h>
///////////////////////////////////////////////////////////////////////
// 
//          WriteLog:           ��ӡ����ĳ�����Ϣ�ͳ������е��м��������þ�̬��������ʽ
//			����ࣺ				������
//          ����Ա:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
class WriteLog {


public: static	bool write_log(const char* msg)
{
	try
	{
		if (msg == NULL) 
		{
			cerr<<"WriteLog Input false!"<<endl;
			return false;
		}
		ofstream outfile("E:\\SRParser\\bin\\SR_Parser.log",ios::app);
		if (!outfile) 
		{
			cerr<<"WriteLog outfile false!"<<endl;
			return false;
		}
		//��ȡ��ǰʱ��
		time_t timer;  
		time(&timer);
		char temptime[25];
		memset(temptime,0,sizeof(temptime));
		tm tm_time;
		localtime_s(&tm_time,&timer);
		strftime(temptime,sizeof(temptime),"%Y��%m��%d��%I:%M:%S%p",&tm_time);
		//strftime(temptime,sizeof(temptime),"%Y��%m��%d��%I:%M:%S%p",localtime(&timer));
		outfile<<temptime<<"\t"<<msg<<endl;
		outfile.close();
		return true;
	}
	catch (...)
	{
		cerr<<"WriteLog abnormal!"<<endl;
		return false;
	}
}
};