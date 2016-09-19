#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include "../../bin/TpiClient.h"
#include <time.h>//为了测试时间
//#include "HitParser.h"

#include "../../bin/HitParser.h"
#include "../../bin/ShiftReduceParser.h"
//#pragma comment(lib , "../../bin/HitParser.lib")
//#pragma comment(lib , "../../bin/TpiClient.lib")

using namespace std;

class KbaseTest
{
public:
	KbaseTest(void);
	~KbaseTest(void);

private:

	TPI_HCON m_hTpiHandle;//服务器句柄
	TPI_HRECORDSET m_hRecordSet;//记录表句柄

	//写log
	bool write_log(const char* TempStr);

	//打开服务器SHost，返回服务器句柄
	//输入：SHost，服务器的IP地址（"192.168.100.152"）
	bool TPIServerInit(string SHost);

	//获取指定记录集句柄
	//输入：sSql_Name，打开sSql_Name对应的SQL语句所指定的表
	//输出：SearchTime，执行SQL语句所需时间(单位：毫秒)
	bool TPIRecordSetOpen(string sSql_Name,double &SearchTime,long &RecordNum);

	//设置与获取的字段名称
	//从FieldName.ini中读取字段名到FieldMap
	//输入：FieldFileName ,ini文件名
	//输出：FieldMap ,存储字段名称MAP
	bool SetFieldName(string FieldFileName,map<string,string> &FieldMap);


	//设置文献库记录条数字
	//输入：RecNumFileName ，ini文件名
	//输出：从文件中读到的记录数字RecNum
	bool SetRecNum(string RecNumFileName,string& strnum,long &RecNum);

	vector<string> splitEx( string src, string separate_character);
	int MinIndex(int i,int j ,int k);

    //vector<CString>SplitSentence(CString sent);
    vector<string>SplitSentence(string sent);


public:

	//按照字段名称获取字段
	//测试获取CJFD中的文章
	//FieldName.ini中字段名；格式，每行一个字段名
	//             篇名；中文关键词；中文摘要；中文刊名；分类号；全文；机标关键词；SYS_VSM；文件名
	//表名：CJFD0608
	bool GetKbaseTextByFieldName(void);

 	bool ParseKbaseText(void);
	bool TitleParseKbaseText(void);
	bool inputNameOfCIDian(const string filename1,const string filename2,set<string>& cidian1,set<string>&cidian2);
    bool OuputHEKUKbaseText(set<string>& cidian1,set<string>&cidian2);
	bool inputNameOfguanjianci(const string filename,set<string>&guanjianci);
	bool KbaseTest::OuputDicNameText(set<string>& guanjianci);
//     bool TestCJFDpaper_length();
// 
//     //用调整分词标注后，句法稳定性测试.
//     bool NewParseKbaseText(void);
};
