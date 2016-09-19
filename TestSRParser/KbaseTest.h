#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include "../../bin/TpiClient.h"
#include <time.h>//Ϊ�˲���ʱ��
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

	TPI_HCON m_hTpiHandle;//���������
	TPI_HRECORDSET m_hRecordSet;//��¼����

	//дlog
	bool write_log(const char* TempStr);

	//�򿪷�����SHost�����ط��������
	//���룺SHost����������IP��ַ��"192.168.100.152"��
	bool TPIServerInit(string SHost);

	//��ȡָ����¼�����
	//���룺sSql_Name����sSql_Name��Ӧ��SQL�����ָ���ı�
	//�����SearchTime��ִ��SQL�������ʱ��(��λ������)
	bool TPIRecordSetOpen(string sSql_Name,double &SearchTime,long &RecordNum);

	//�������ȡ���ֶ�����
	//��FieldName.ini�ж�ȡ�ֶ�����FieldMap
	//���룺FieldFileName ,ini�ļ���
	//�����FieldMap ,�洢�ֶ�����MAP
	bool SetFieldName(string FieldFileName,map<string,string> &FieldMap);


	//�������׿��¼������
	//���룺RecNumFileName ��ini�ļ���
	//��������ļ��ж����ļ�¼����RecNum
	bool SetRecNum(string RecNumFileName,string& strnum,long &RecNum);

	vector<string> splitEx( string src, string separate_character);
	int MinIndex(int i,int j ,int k);

    //vector<CString>SplitSentence(CString sent);
    vector<string>SplitSentence(string sent);


public:

	//�����ֶ����ƻ�ȡ�ֶ�
	//���Ի�ȡCJFD�е�����
	//FieldName.ini���ֶ�������ʽ��ÿ��һ���ֶ���
	//             ƪ�������Ĺؼ��ʣ�����ժҪ�����Ŀ���������ţ�ȫ�ģ�����ؼ��ʣ�SYS_VSM���ļ���
	//������CJFD0608
	bool GetKbaseTextByFieldName(void);

 	bool ParseKbaseText(void);
	bool TitleParseKbaseText(void);
	bool inputNameOfCIDian(const string filename1,const string filename2,set<string>& cidian1,set<string>&cidian2);
    bool OuputHEKUKbaseText(set<string>& cidian1,set<string>&cidian2);
	bool inputNameOfguanjianci(const string filename,set<string>&guanjianci);
	bool KbaseTest::OuputDicNameText(set<string>& guanjianci);
//     bool TestCJFDpaper_length();
// 
//     //�õ����ִʱ�ע�󣬾䷨�ȶ��Բ���.
//     bool NewParseKbaseText(void);
};
