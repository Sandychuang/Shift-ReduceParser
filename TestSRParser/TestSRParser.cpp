// TestSRParser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include"afx.h"
#include "..\\..\\bin\\ShiftReduceParser.h"
#include "KbaseTest.h"

using namespace std;
bool TestLargeSentencesParser_s()
{
	KbaseTest largeTest;
	largeTest.ParseKbaseText();
	
	return 0;
}
bool TestSpeedSenParser_s()
{
	
	if(!LoadModel(0))
	{
		cout<<"加载失败"<<endl;
	}
	else
	{
		cout<<"加载成功"<<endl;
	}
	string ifile("..\\..\\Data\\HIT语料分词句子测速.txt");
	string ofile("..\\..\\Data\\速度测试输出文档.txt");
	ifstream infile(ifile.c_str());
	ofstream outfile(ofile.c_str());
	if (!infile|| !outfile)
    {
        return false;
    }
	string textline="";
	char* sout=new char[1024];
	int len=1024;
	strcpy_s(sout,1024,"\0");
	int num = 0;     
    double   duration; 
    DWORD start=GetTickCount();
	while(getline(infile,textline)!= '\0')
    {
	  ++num;
	  len = 1024;
      strcpy_s(sout,1024,"\0");
	 Parser_S(textline.c_str(),sout,len);
	 outfile<<sout<<endl;
	 
	}
	DWORD finish=GetTickCount(); 
    duration = (double)(finish - start) /1000; 
	delete sout;
	if(!UnLoadModel())
	{
		cout<<"卸载失败"<<endl;
	}
	else
	{
		cout<<"卸载成功"<<endl;
	}
	outfile<<num<<endl;
	outfile<<"共用时："<<duration<<"秒，平均："<<num/duration<<"句/秒."<<endl;
	return true;
}

bool TestSenParser()
{
	
	if(!LoadModel(0))
	{
		cout<<"加载失败"<<endl;
	}
	else
	{
		cout<<"加载成功"<<endl;
	}
	string ifile("..\\..\\Data\\CNKI树库句test1.txt");
	string ofile("..\\..\\Data\\CNKI树库句test-19-1.txt");
	ifstream infile(ifile.c_str());
	ofstream outfile(ofile.c_str());
	 if (!infile || !outfile)
    {
        return false;
    }
	string textline="";
	int num=0;
	string temp1="";
	string temp2="";
	while(getline(infile,textline)!= '\0')
    {
		/*num++;
		if(num<12000)continue;*/
	num++;
	
    if(textline.find("HIT树库句")!=string::npos)
	{
		temp1=textline.substr(textline.find("=")+1);
		//;
		//continue;
	}
	char* sout=new char[1024];
	int len=1024;
	if(textline.find("HIT分词句子")!=string::npos)
	{
	string sin=textline.substr(textline.find("=")+2);
	sin=sin.substr(0,sin.find_last_not_of(" ")+1);
	Parser_S(sin.c_str(),sout,len);

	if(sout!=NULL)temp2=sout;
	//;
	delete sout;
	}
	if(num%2==0)
	{
	  if(temp1.size()!=0&&temp2.size()!=0)
	  {
		outfile<<"<HIT树库句>="<<temp1<<endl;
		outfile<<"<SRParser >="<<temp2<<endl;
	   }

	
	  temp1="";
	  temp2="";
	 }
	}
	if(!UnLoadModel())
	{
		cout<<"卸载失败"<<endl;
	}
	else
	{
		cout<<"卸载成功"<<endl;
	}
	return true;
}

void TestTrainModel()
{
	if(!TrainModel("..\\..\\Data\\CNKI树库句train_train.txt"))
	{
		cout<<"训练失败"<<endl;
	}
	else
	{
		cout<<"训练成功"<<endl;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	
	
	//TestSpeedSenParser_s();
	TestLargeSentencesParser_s();
	//TestSenParser();
   //TestTrainModel();
	return 0;
}

