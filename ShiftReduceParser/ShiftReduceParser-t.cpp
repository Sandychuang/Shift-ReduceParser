// ShiftReduceParser.cpp : �������̨Ӧ�ó������ڵ㡣
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
// Ψһ��Ӧ�ó������

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
//	string ifile(".\\HIT�������ǺŰ�ԭʼ���ӡ����ִ�.txt");
//	string ofile(".\\HIT����ԭʼ����.txt");
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
//		  if(textline.find("HIT�����")!=string::npos)
//	{
//		string temp=textline.substr(textline.find("=")+1);
//		outfile<<"<HIT�����>="<<"S["<<temp<<"]"<<endl;
//		//Node* pNode=NULL;
//		//buildtree.build_tree(temp.c_str(),pNode);
//		string segPos;
//		//buildtree.treeToSegString(pNode,segPos);
//		outfile<<"<HIT�ִʾ���>="<<segPos<<endl;
//		//continue;
//	}
//	}
//}
////////////���ٺ���///////////////////////////////////////////////
bool TestSpeedSenParser_s()
{
	PerceptronModel SRModelTrain2;
	SRModelTrain2.inPutModel(".\\20160104ѵ��ģ��\\knowstates.txt",".\\20160104ѵ��ģ��\\rootstates.txt",".\\20160104ѵ��ģ��\\rootonlystates.txt",".\\20160104ѵ��ģ��\\indextransition.txt",".\\20160104ѵ��ģ��\\featureweight.txt");
	string ifile(".\\HIT���Ϸִʾ��Ӳ���.txt");
	string ofile(".\\�ٶȲ�������ĵ�.txt");
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
	outfile<<"����ʱ��"<<duration<<"�룬ƽ����"<<num/duration<<"��/��."<<endl;
	return true;
}
////////////////////////////�ԱȲ���׼ȷ�ʺ��ٻ���///////////////////
bool TestSenParser()
{
	PerceptronModel SRModelTrain2;
	SRModelTrain2.inPutModel(".\\20160104ѵ��ģ��\\knowstates.txt",".\\20160104ѵ��ģ��\\rootstates.txt",".\\20160104ѵ��ģ��\\rootonlystates.txt",".\\20160104ѵ��ģ��\\indextransition.txt",".\\20160104ѵ��ģ��\\featureweight.txt");
	string ifile(".\\HIT�������ǺŰ�ԭʼ�ִʾ���.txt");
	string ofile(".\\����������䷨�����Ա�.txt");
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

    if(textline.find("HIT�����")!=string::npos)
	{
		string temp=textline.substr(textline.find("=")+1);
		outfile<<"<HIT�����>="<<"S["<<temp<<"]"<<endl;
		continue;
	}
	char* sout=new char[1024];
	int len=1024;
	if(textline.find("HIT�ִʾ���")!=string::npos)
	{
	string sin=textline.substr(textline.find("=")+1);
	sin=sin.substr(0,sin.find_last_not_of(" ")+1);
	SRModelTrain2.parser_s(sin.c_str(),sout,len);
	//SRModelTrain2.parser("����/nd ��Ա/ng ����/a",sout,len);
	//SRModelTrain2.parser("����/ng ����/vg �Ϻ�/ng",sout,len);
	//SRModelTrain2.parser("һ/m ��/q ��/f ��/wo ��/r ��ʼ/vg ����/a ��/usde ��Ϣ/ng ��/wj",sout,len);
	//SRModelTrain2.parser("�Ϻ�/nr �ֶ�/nr ����/nn ��/cc ����/nn ����/nn ͬ��/vg",sout,len);
	//SRModelTrain2.parser("è/ng ��/usde ������/ng",sout,len);
	//SRModelTrain2.parser("һ/m ��/q ����/a ��/usde ��Ŀ/ng",sout,len);
	//SRModelTrain2.parser("��/r ˵/vg ��/r ��/vx һ/m ��/q ��/a ѧ��/ng ��/wj",sout,len);
	//SRModelTrain2.parser("��/r һ/d ����/vg ��/r ��/d Ц/vg ��/wj",sout,len);
	//SRModelTrain2.parser("��/r ��ǰ/t ����/d û��/d ��/vg ��/ut ��/p ѹ��/vg ��/usde ���/ng ��/wo ��/vg һ/m ��/q ��/ng �ŷ�/vg ����/vq ��/wj",sout,len);
	//SRModelTrain2.parser("��/r �ǵ�/vg ����/t ��/d ��Ҫ/vg ��/r ��/q ��/ng ��/wo ��/r ��/d ����/d ��/vg ��/wo ��/c ȴ/d û��/d �ҵ�/vg ��/wj ��/r һֱ/d ��/d ��/vg ��/r ��/q ��/ng ����/d ��/vg ��/p ʲô/r �ط�/ng ��/y ��/wj",sout,len);
	//cout<<sout<<endl;
	outfile<<"<SRParser >="<<sout<<endl;
	delete sout;
	}
	}
	SRModelTrain2.unload();
	return true;
}
//////////////////////////////////////////////////
///////ѵ������
//////////////////////////////////////////////////
bool TestSenTree()
{
	inputHeadFindRule();
	HeadFindRule.find("BNP");
	string ifile(".\\����������ԭʼ����4.txt");
	//string ifile2(".\\DATA\\�������ϴ𰸣�ANSI��ʽ��.txt");
    string ofile(".\\�����������Զ���עͷ���.txt");
    //string ofile("22��ѧ��ԭʼ61093���ӷִʱ�ע�䷨�ɷֽ��(model=1)_20130121.txt");
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
	//SRModelTrain.initialStateFromTaggedSentence(binarizedTrees,"��ҵ/ng �ص�/ng ѧ/vg ��ѧ/ng ��/ng ���齨��/ng ��/usde ˼��/ng ");

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
		/*cout<<"*****************���Ƿָ���******************"<<endl;*/
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
	//	oa1<<(*ipp).first;oa1<<(*ipp).second;////����CString�Ľṹ���������
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
	//	oa << pa;  ////����CString�Ľṹ���������
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
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ

			
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		    //TestSenTree();
			//TestSenParser();
			//switchToSegString();
			TestSpeedSenParser_s();
			
		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
