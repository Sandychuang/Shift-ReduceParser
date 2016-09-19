#pragma once
#include "stdafx.h"
#define SRPARSER_EXPORTS
#include"SRParser.h"
#include"PerceptronModel.h"
#include <boost/serialization/export.hpp>  
SRPARSER_API PerceptronModel SRModelTrain;
BOOST_CLASS_EXPORT_GUID(CompoundUnaryTransition,"CompoundUnaryTransition")
BOOST_CLASS_EXPORT_GUID(ShiftTransition,"ShiftTransition")
BOOST_CLASS_EXPORT_GUID(UnaryTransition,"UnaryTransition")
BOOST_CLASS_EXPORT_GUID(BinaryTransition,"BinaryTransition")
BOOST_CLASS_EXPORT_GUID(FinalizeTransition,"FinalizeTransition")
BOOST_CLASS_EXPORT_GUID(IdleTransition,"IdleTransition")
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
SRPARSER_API bool LoadModel(int inipathmode)
{
	return SRModelTrain.inPutModel("..\\..\\Data\\model\\knowstates.txt","..\\..\\Data\\model\\rootstates.txt","..\\..\\Data\\model\\rootonlystates.txt","..\\..\\Data\\model\\indextransition.txt","..\\..\\Data\\model\\featureweight.txt");
	//return true;
}
SRPARSER_API bool UnLoadModel()
{
	return SRModelTrain.unload();
	
}
SRPARSER_API bool Parser(const char* sentin,char * &sentout,int &soutlen)
{
	return SRModelTrain.parser(sentin,sentout,soutlen);
}
SRPARSER_API bool Parser_S(const char* sentin,char *&sentout,int &soutlen)
{
	return SRModelTrain.parser_s(sentin,sentout,soutlen);
}
SRPARSER_API bool TrainModel(const char* filename)
{
	
	string ifile(filename);
	ifstream infile(ifile.c_str());
	if(!infile)
	{
		WriteLog::write_log("打开训练文件失败");
		return false;
	}
	string textline("");
	if(!SRModelTrain.buildTree.readCollinsHeadRule("..\\..\\Data\\model\\HeadFindRule.txt"))
	{
		WriteLog::write_log("打开HeadFindRule文件失败");
		return false;
	}
	
	vector<Transition*> IndexTransition;
	vector<Node*> binarizedTrees;
	vector<list<Transition*>>transitionLists;
	set<Transition*,compareClass> tempIndexTranition;
	set<string>knownstates;
	set<string>rootstates;
	set<string>rootonlystates;
	try
	{
	while(getline(infile,textline)!= '\0')
    {
		Node* pNode=NULL;
		SRModelTrain.buildTree.build_tree(textline.c_str(),pNode);
		SRModelTrain.buildTree.newGetHead(pNode);//,temp);
		SRModelTrain.buildTree.tansformTree(pNode);
		binarizedTrees.push_back(pNode);
	}
	}
	catch(...)
	{
		WriteLog::write_log("树结构建立失败");
		return false;
	}
	SRModelTrain.buildTree.findKnownStates(binarizedTrees,SRModelTrain.KownStates);
	SRModelTrain.buildTree.findRootStates(binarizedTrees,SRModelTrain.RootStates);
	SRModelTrain.buildTree.findRootOnlyStates(binarizedTrees,SRModelTrain.RootOnlyStates);
	try{
	auto itt=binarizedTrees.begin();
	for(;itt!=binarizedTrees.end();itt++)
	{
	  list<Transition*> transition;
	  SRModelTrain.buildTree.createTransitionSequenceHelper(transition,(*itt),SRModelTrain.RootStates,SRModelTrain.RootOnlyStates);
	  auto igg=transition.begin();
	  for(;igg!=transition.end();igg++)
	  {
		  tempIndexTranition.insert(*igg);
	  }
	 transitionLists.push_back(transition);

	}
	}
	catch(...)
	{
		WriteLog::write_log("转移序列建立失败");
		return false;
	}
	
	auto icc=tempIndexTranition.begin();
	for(;icc!=tempIndexTranition.end();icc++)
	{
		SRModelTrain.IndexTransition.push_back(*icc);
	}

	SRModelTrain.trainModel(binarizedTrees,transitionLists);
	if(!SRModelTrain.outPutModel("..\\..\\Model\\knowstates.txt","..\\..\\Model\\rootstates.txt","..\\..\\Model\\rootonlystates.txt","..\\..\\Model\\indextransition.txt","..\\..\\Model\\featureweight.txt"))
	{
		WriteLog::write_log("模型输出失败");
		return false;
	}
	try
	{
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
	auto iuu=binarizedTrees.begin();
	for(;iuu!=binarizedTrees.end();iuu++)
	{
		SRModelTrain.buildTree.delete_Node(*iuu);
	}
	}
	catch(...)
	{
		WriteLog::write_log("模型训练内存释放失败");
		return false;
	}
	return true;
}

