// ShiftReduceParser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"afx.h"
#include "ShiftReduceParser.h"
#include <iostream>
#include <fstream>
#include <string>
//#include "Node.h"
#include "BuildTree.h"
#include "PerceptronModel.h"
#include <boost/serialization/export.hpp>  
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BOOST_CLASS_EXPORT_GUID(CompoundUnaryTransition,"CompoundUnaryTransition")
BOOST_CLASS_EXPORT_GUID(ShiftTransition,"ShiftTransition")
BOOST_CLASS_EXPORT_GUID(UnaryTransition,"UnaryTransition")
BOOST_CLASS_EXPORT_GUID(BinaryTransition,"BinaryTransition")
BOOST_CLASS_EXPORT_GUID(FinalizeTransition,"FinalizeTransition")
BOOST_CLASS_EXPORT_GUID(IdleTransition,"IdleTransition")
// 唯一的应用程序对象

CWinApp theApp;

using namespace std;
class compareClass{
public:
	bool operator()(  Transition* a1,  Transition* a2) const

	{
		
		if(a1->tostring()==a2->tostring())
		{
			return false;
		}
		else
		{
			return a1->tostring()<a2->tostring();
		}
	}
};
map<string,vector<string>> HeadFindRule;
bool inputHeadFindRule()
{
	// char a[3][10] = { "Hello", "world", "C++" };
	//string temp[1][1]={{"right"}};
	vector<string> temp;
	temp.push_back("right");
	temp.push_back("nx");
	temp.push_back("nx");
	temp.push_back("ny");
	temp.push_back("nz");
	temp.push_back("nz");
	temp.push_back("BNP");
	//int **zz={{1,2,3},{1,2,3}};
	HeadFindRule.insert(make_pair("BNP",temp));
	return true;
}
//bool switchToSegString()
//{
//	CBuildTree buildtree;
//	string ifile(".\\HIT语料无星号版原始句子——分词.txt");
//	string ofile(".\\HIT语料原始句子.txt");
//	ifstream infile(ifile.c_str());
//	ofstream outfile(ofile.c_str());
//	 if (!infile || !outfile)
//    {
//        return false;
//    }
//	string textline="";
//	int num=0;
//	while(getline(infile,textline)!= '\0')
//    {
//		  if(textline.find("HIT树库句")!=string::npos)
//	{
//		string temp=textline.substr(textline.find("=")+1);
//		outfile<<"<HIT树库句>="<<"S["<<temp<<"]"<<endl;
//		//Node* pNode=NULL;
//		//buildtree.build_tree(temp.c_str(),pNode);
//		string segPos;
//		//buildtree.treeToSegString(pNode,segPos);
//		outfile<<"<HIT分词句子>="<<segPos<<endl;
//		//continue;
//	}
//	}
//}
////////////测速函数///////////////////////////////////////////////
bool TestSpeedSenParser_s()
{
	PerceptronModel SRModelTrain2;
	SRModelTrain2.inPutModel(".\\20160104训练模型\\knowstates.txt",".\\20160104训练模型\\rootstates.txt",".\\20160104训练模型\\rootonlystates.txt",".\\20160104训练模型\\indextransition.txt",".\\20160104训练模型\\featureweight.txt");
	string ifile(".\\HIT语料分词句子测速.txt");
	string ofile(".\\速度测试输出文档.txt");
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
	 SRModelTrain2.parser_s(textline.c_str(),sout,len);
	 outfile<<sout<<endl;
	 
	}
	DWORD finish=GetTickCount(); 
    duration = (double)(finish - start) /1000; 
	delete sout;
	SRModelTrain2.unload();
	outfile<<num<<endl;
	outfile<<"共用时："<<duration<<"秒，平均："<<num/duration<<"句/秒."<<endl;
	return true;
}
////////////////////////////对比测量准确率和召回率///////////////////
bool TestSenParser()
{
	PerceptronModel SRModelTrain2;
	SRModelTrain2.inPutModel(".\\20160104训练模型\\knowstates.txt",".\\20160104训练模型\\rootstates.txt",".\\20160104训练模型\\rootonlystates.txt",".\\20160104训练模型\\indextransition.txt",".\\20160104训练模型\\featureweight.txt");
	string ifile(".\\HIT语料无星号版原始分词句子.txt");
	string ofile(".\\哈工大树库句法分析对比.txt");
	ifstream infile(ifile.c_str());
	ofstream outfile(ofile.c_str());
	 if (!infile || !outfile)
    {
        return false;
    }
	string textline="";
	int num=0;
	while(getline(infile,textline)!= '\0')
    {
		num++;
		if(num<12000)continue;

    if(textline.find("HIT树库句")!=string::npos)
	{
		string temp=textline.substr(textline.find("=")+1);
		outfile<<"<HIT树库句>="<<"S["<<temp<<"]"<<endl;
		continue;
	}
	char* sout=new char[1024];
	int len=1024;
	if(textline.find("HIT分词句子")!=string::npos)
	{
	string sin=textline.substr(textline.find("=")+1);
	sin=sin.substr(0,sin.find_last_not_of(" ")+1);
	SRModelTrain2.parser_s(sin.c_str(),sout,len);
	//SRModelTrain2.parser("美国/nd 幅员/ng 辽阔/a",sout,len);
	//SRModelTrain2.parser("布朗/ng 访问/vg 上海/ng",sout,len);
	//SRModelTrain2.parser("一/m 周/q 后/f ，/wo 他/r 开始/vg 短暂/a 的/usde 休息/ng 。/wj",sout,len);
	//SRModelTrain2.parser("上海/nr 浦东/nr 开发/nn 与/cc 法制/nn 建设/nn 同步/vg",sout,len);
	//SRModelTrain2.parser("猫/ng 的/usde 警惕性/ng",sout,len);
	//SRModelTrain2.parser("一/m 个/q 短暂/a 的/usde 项目/ng",sout,len);
	//SRModelTrain2.parser("他/r 说/vg 他/r 是/vx 一/m 个/q 好/a 学生/ng 。/wj",sout,len);
	//SRModelTrain2.parser("他/r 一/d 看见/vg 她/r 就/d 笑/vg 。/wj",sout,len);
	//SRModelTrain2.parser("我/r 以前/t 从来/d 没有/d 见/vg 过/ut 被/p 压抑/vg 的/usde 情感/ng ，/wo 像/vg 一/m 团/q 火/ng 迸发/vg 出来/vq 。/wj",sout,len);
	//SRModelTrain2.parser("我/r 记得/vg 后来/t 又/d 想要/vg 这/r 本/q 书/ng ，/wo 我/r 就/d 到处/d 找/vg ，/wo 但/c 却/d 没有/d 找到/vg 。/wj 我/r 一直/d 在/d 想/vg 这/r 本/q 书/ng 到底/d 丢/vg 在/p 什么/r 地方/ng 了/y 。/wj",sout,len);
	//cout<<sout<<endl;
	outfile<<"<SRParser >="<<sout<<endl;
	delete sout;
	}
	}
	SRModelTrain2.unload();
	return true;
}
//////////////////////////////////////////////////
///////训练函数
//////////////////////////////////////////////////
bool TestSenTree()
{
	inputHeadFindRule();
	HeadFindRule.find("BNP");
	string ifile(".\\哈工大树库原始句子4.txt");
	//string ifile2(".\\DATA\\测试语料答案（ANSI格式）.txt");
    string ofile(".\\哈工大树库自动标注头结点.txt");
    //string ofile("22个学科原始61093句子分词标注句法成分结果(model=1)_20130121.txt");
    ifstream infile(ifile.c_str());
	//ifstream infile2(ifile2.c_str());
	ShiftTransition tempTransition;
    ofstream outfile(ofile.c_str());
    if (!infile || !outfile)
    {
        return false;
    }
	int n=0;
	string textline("");
	CBuildTree buildtree;
	buildtree.readCollinsHeadRule("HeadFindRule.txt");
	vector<Transition*> IndexTransition;
	vector<Node*> binarizedTrees;
	vector<list<Transition*>>transitionLists;
	//compareClass com;
	set<Transition*,compareClass> tempIndexTranition;
	set<string>knownstates;
	set<string>rootstates;
	set<string>rootonlystates;
	PerceptronModel SRModelTrain;
	char* sout=new char[1024];
	delete sout;
	//SRModelTrain.initialStateFromTaggedSentence(binarizedTrees,"林业/ng 重点/ng 学/vg 科学/ng 术/ng 队伍建设/ng 的/usde 思考/ng ");

	while(getline(infile,textline)!= '\0')
    {
		Node* pNode=NULL;
		n++;
		if(n>6000)break;
		//vector<Node*> sentence;
		buildtree.build_tree(textline.c_str(),pNode);
		//vector<Node*>temp;
		buildtree.newGetHead(pNode);//,temp);
		buildtree.tansformTree(pNode);
		//buildtree.deTansformTree(pNode);
		//string senOut;
		//buildtree.treeToString(pNode,senOut);
		//outfile<<senOut<<endl;
		//buildtree.Gethead_Star(pNode);
		binarizedTrees.push_back(pNode);
		/*buildtree.findKnownStates(pNode,knownstates);
		buildtree.createTransitionSequenceHelper(transition,pNode,rootstates,rootonlystates);
		buildtree.initialStateFromTaggedSentence(sentence,pNode);
		State state=buildtree.initialStateFromGoldTagTree(pNode);
		for(vector<Transition*>::iterator iter=transition.begin();iter!=transition.end();iter++)
		{
			cout<<(*iter)->tostring()<<endl;
			tempIndexTranition.insert(*iter);
		}
		transition.clear();*/
		/*cout<<"*****************我是分割线******************"<<endl;*/
	}
	/*int cc=0;
	auto itt=IndexTranition.begin();*/
	buildtree.findKnownStates(binarizedTrees,SRModelTrain.KownStates);
	buildtree.findRootStates(binarizedTrees,SRModelTrain.RootStates);
	buildtree.findRootOnlyStates(binarizedTrees,SRModelTrain.RootOnlyStates);
	auto itt=binarizedTrees.begin();
	for(;itt!=binarizedTrees.end();itt++)
	{
	  list<Transition*> transition;
	  buildtree.createTransitionSequenceHelper(transition,(*itt),SRModelTrain.RootStates,SRModelTrain.RootOnlyStates);
	  auto igg=transition.begin();
	  for(;igg!=transition.end();igg++)
	  {
		  tempIndexTranition.insert(*igg);
	  }
	 /* for(list<Transition*>::iterator iter=transition.begin();iter!=transition.end();iter++)
		{
			cout<<(*iter)->tostring()<<endl;
			
		}*/
	  transitionLists.push_back(transition);

	}
	
	auto icc=tempIndexTranition.begin();
	for(;icc!=tempIndexTranition.end();icc++)
	{
		SRModelTrain.IndexTransition.push_back(*icc);
	}

	SRModelTrain.trainModel(binarizedTrees,transitionLists);
	SRModelTrain.outPutModel("knowstates.txt","rootstates.txt","rootonlystates.txt","indextransition.txt","featureweight.txt");
	
	auto ihh=transitionLists.begin();
	for(;ihh!=transitionLists.end();ihh++)
	{
		auto inn=(*ihh).begin();
		for(;inn!=(*ihh).end();inn++)
		{
			delete *inn;
		}
	}
	auto ioo=SRModelTrain.featureWeights.begin();
	for(;ioo!=SRModelTrain.featureWeights.end();ioo++)
	{
		delete ioo->second;
	}
	/*auto ijj=SRModelTrain.IndexTransition.begin();
	for(;ijj!=SRModelTrain.IndexTransition.end();ijj++)
	{
		delete (*ijj);
	}*/
	auto iuu=binarizedTrees.begin();
	for(;iuu!=binarizedTrees.end();iuu++)
	{
		buildtree.delete_Node(*iuu);
	}
	////CFile OutPutFile("FeatureWeight.txt",CFile::modeCreate|CFile::modeWrite);  
 ////   CArchive oar(&OutPutFile, CArchive::store);
	////

	//std::ofstream ofs1("FeatureWeight.txt");  
 //   boost::archive::text_oarchive oa1(ofs1);
	//auto ipp=SRModelTrain.featureWeights.begin();
	//int i=0;
	//for(;ipp!=SRModelTrain.featureWeights.end();ipp++)
	//{
	//	/*i++;
	//	if(i==596)
	//	{
	//		int cc=0;
	//	}*/
	//	oa1<<(*ipp).first;oa1<<(*ipp).second;////按照CString的结构进行输出。
	//	
	//}
	//
	// ofs1.close();  
	//std::ifstream ifs1("FeatureWeight.txt");  
 //   boost::archive::text_iarchive ia1(ifs1);
	//map<string,Weight*> tempmap;
	//string temp;
	//Weight* pz;
	//while(!ifs1.eof())
	//{
	//	i++;
	//	if(i==596)
	//	{
	//		int cc=0;
	//	}
	//	/*string temp;
	//	Weight* pz;*/
	//	ia1>>temp;
	//	ia1>> pz;
	//	tempmap.insert(make_pair(temp,pz));	
	//	cout<<i<<endl;
	//}
 //   ifs1.close();  
	////
	////Transition* temp=new ShiftTransition();
	////oar<<temp;
	////oar.Close(); 
	////OutPutFile.Close();  
	////CFile iFile("FeatureWeight.txt", CFile::modeRead);  
 ////   CArchive iar(&iFile, CArchive::load);  
	/////*Transition* tempzz;*/

	////try{
 ////  Transition* p2 = (Transition*)iar.ReadObject(RUNTIME_CLASS(Transition));  

	////}
	////catch(...)
	////{
	////}
	////Transition*temp=new CompoundUnaryTransition();
	//std::ofstream ofs("aaa.txt");  
 //   boost::archive::text_oarchive oa(ofs);
	//auto iaa=SRModelTrain.IndexTransition.begin();
	//for(;iaa!=SRModelTrain.IndexTransition.end();iaa++)
	//{
	//	 Transition* pa=(*iaa);
	//	oa << pa;  ////按照CString的结构进行输出。
	//}
 //  ofs.close();  
 //  Transition * pa2;  
 //   std::ifstream ifs("aaa.txt");  
 //   boost::archive::text_iarchive ia(ifs);
	//vector<Transition*> tempvect;
	//while(ifs)
	//{
	//	ia >> pa2;
	//	tempvect.push_back(pa2);
	//	cout<<pa2->tostring()<<endl;
	//}
 //   ifs.close();  
    return 0;  
	//int i=0;


	
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要

			
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		    //TestSenTree();
			//TestSenParser();
			//switchToSegString();
			TestSpeedSenParser_s();
			
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
