// TestSRParser.cpp : �������̨Ӧ�ó������ڵ㡣
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
		cout<<"����ʧ��"<<endl;
	}
	else
	{
		cout<<"���سɹ�"<<endl;
	}
	string ifile("..\\..\\Data\\HIT���Ϸִʾ��Ӳ���.txt");
	string ofile("..\\..\\Data\\�ٶȲ�������ĵ�.txt");
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
		cout<<"ж��ʧ��"<<endl;
	}
	else
	{
		cout<<"ж�سɹ�"<<endl;
	}
	outfile<<num<<endl;
	outfile<<"����ʱ��"<<duration<<"�룬ƽ����"<<num/duration<<"��/��."<<endl;
	return true;
}

bool TestSenParser()
{
	
	if(!LoadModel(0))
	{
		cout<<"����ʧ��"<<endl;
	}
	else
	{
		cout<<"���سɹ�"<<endl;
	}
	string ifile("..\\..\\Data\\CNKI�����test1.txt");
	string ofile("..\\..\\Data\\CNKI�����test-19-1.txt");
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
	
    if(textline.find("HIT�����")!=string::npos)
	{
		temp1=textline.substr(textline.find("=")+1);
		//;
		//continue;
	}
	char* sout=new char[1024];
	int len=1024;
	if(textline.find("HIT�ִʾ���")!=string::npos)
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
		outfile<<"<HIT�����>="<<temp1<<endl;
		outfile<<"<SRParser >="<<temp2<<endl;
	   }

	
	  temp1="";
	  temp2="";
	 }
	}
	if(!UnLoadModel())
	{
		cout<<"ж��ʧ��"<<endl;
	}
	else
	{
		cout<<"ж�سɹ�"<<endl;
	}
	return true;
}

void TestTrainModel()
{
	if(!TrainModel("..\\..\\Data\\CNKI�����train_train.txt"))
	{
		cout<<"ѵ��ʧ��"<<endl;
	}
	else
	{
		cout<<"ѵ���ɹ�"<<endl;
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

