#pragma once
#include<list>
#include<string>
#include<vector>
#include"State.h"
///////////////////////////////////////////////////////////////////////
// 
//          FeatureFactory.h:   ������ȡ��
//			����ࣺ				BasicFeatureFactory
//          ����Ա:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
class FeatureFactory
{
public :
	FeatureFactory(){}
	~FeatureFactory(){}
	   enum Transition { LEFT, RIGHT, UNARY };
	   enum FeatureComponent{
		   HEADWORD,HEADTAG,VALUE
		   };
	   string FeatureComponentTostring(FeatureComponent feature)
	   {
		   switch(feature)
		   {
		   case(HEADWORD):return "w";
		    case(HEADTAG):return "t";
			case(VALUE):return "c";
			default:return"";
		   }

		   return"";
	   }
	   struct Lable
	  {
		  string conlabel;
		  string wordlabel;
		  string taglabel;
		  Lable()
		  {
			  conlabel="";
			  wordlabel="";
			  taglabel="";
		  }
	  };
	   Lable getStackLabel(stack<Node*> s_stack,int nodenum, int trasitions_size,Transition* trasitions);
	   Lable getQueueLabel(vector<Node*>&sentence,int takenposition,int offset);
	   Lable getCoreLabel(Node* tree);
	   string getFeatureFromLabel(Lable lable,FeatureComponent feature);
	   ///////////////////////////////////////////////////////////////////////////////
	   ///�ú�����δʵ��,��δ���������ã�
	   ////////////////////////////////////////////////////////////////////////////////
	   Lable getRecentDenpendent(stack<Node*>s_stack,Transition trasitions,int nodenum);
};