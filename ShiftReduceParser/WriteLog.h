#include <fstream>
#include <iostream>
#include <string>
#include <cstring> 
#include <time.h>
///////////////////////////////////////////////////////////////////////
// 
//          WriteLog:           打印程序的出错信息和程序运行的中间结果，采用静态方法的形式
//			相关类：				所有类
//          程序员:				xu sa	2016 01 07
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
		//获取当前时间
		time_t timer;  
		time(&timer);
		char temptime[25];
		memset(temptime,0,sizeof(temptime));
		tm tm_time;
		localtime_s(&tm_time,&timer);
		strftime(temptime,sizeof(temptime),"%Y年%m月%d日%I:%M:%S%p",&tm_time);
		//strftime(temptime,sizeof(temptime),"%Y年%m月%d日%I:%M:%S%p",localtime(&timer));
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