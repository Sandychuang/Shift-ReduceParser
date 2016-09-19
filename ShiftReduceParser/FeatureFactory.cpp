#include "stdafx.h"  
#include"FeatureFactory.h"
//#include "WriteLog.h"
string FeatureFactory::getFeatureFromLabel(Lable lable,FeatureComponent feature)
{
	string temp="";
	switch(feature)
	{
	case(HEADWORD):temp=lable.wordlabel;
		break;
	case(HEADTAG):temp=lable.taglabel;
		break;
	case(VALUE):temp=lable.conlabel;
		break;
	default:
		//////////需要打印日志记录////WriteLog///
		WriteLog::write_log("getFeatureFromLabel:feature:error");
		break;

	}
	if(temp=="")temp="null";
	return temp;
}
FeatureFactory::Lable FeatureFactory::getStackLabel(stack<Node*> s_stack,int nodenum, int trasitions_size, Transition* trasitions)
{
	Lable templable;
	if(s_stack.size()<=nodenum)
	{
		//WriteLog::write_log("FeatureFactory:getStackLabel:s_stack.size():error:索引超出栈的范围");
		
		 return templable;
	}
	for(int i=0;i<nodenum;i++)
	{
		s_stack.pop();
	}
	Node* tempNode=s_stack.top();
	for(int i=0;i<trasitions_size;i++)
	{
		switch(trasitions[i])
		{
		case (LEFT):
			if(tempNode->m_childNum!=2)return templable;
			tempNode=tempNode->m_pLeftChild;
			break;
		case(RIGHT):
			if(tempNode->m_childNum!=2)return templable;
			tempNode=tempNode->m_pRightChild;
			break;
	   case(UNARY):
		   if(tempNode->m_childNum!=1)return templable;
		   tempNode=tempNode->m_pLeftChild;
		   break;
	   default:
		 WriteLog::write_log("FeatureFactory:getStackLabel:trasitions:error");
		   ;
		}
	}
	 if(tempNode!=NULL)
	 {
		 if(tempNode->m_pHead!=NULL)
		 
		{ templable.conlabel=string(tempNode->m_psPOS);
		 templable.taglabel=string(tempNode->m_pHead->headtag);
		 templable.wordlabel=string(tempNode->m_pHead->headword);
		 }
	 else
	 { 
		 templable.conlabel="";
		 if(tempNode->m_psPOS!=NULL)templable.taglabel=string(tempNode->m_psPOS);
		 if(tempNode->m_psWord!=NULL)templable.wordlabel=string(tempNode->m_psWord);
	 }


	 }
	
	 return templable;

}
FeatureFactory::Lable FeatureFactory::getQueueLabel(vector<Node*>&sentence,int tokenPosition,int offset)
{
	 Lable templable;
	if (tokenPosition + offset < 0 || tokenPosition + offset >= sentence.size()) { 
      
		 //WriteLog::write_log("FeatureFactory:getQueueLabel:tokenPosition:error:索引超出队列的范围");
		 return templable;

    }
	Node* tempNode=sentence[tokenPosition + offset];
	if(tempNode!=NULL)
	{
		templable.conlabel="";
		if(tempNode->m_psPOS!=NULL)templable.taglabel=string(tempNode->m_psPOS);
		 if(tempNode->m_psWord!=NULL)templable.wordlabel=string(tempNode->m_psWord);
	 
	}
	return templable;
}
FeatureFactory::Lable FeatureFactory::getCoreLabel(Node* tempNode)
{
	 Lable templable;
	if(tempNode!=NULL)
	{
	templable.conlabel=tempNode->headlabel;
	templable.taglabel=tempNode->headtag;
	templable.wordlabel=tempNode->headword;
	}
	return templable;
}
FeatureFactory::Lable FeatureFactory::getRecentDenpendent(stack<Node*> s_stack,Transition trasitions,int nodenum)
{
	Lable templable;
	if(s_stack.size()<=nodenum)
	{
		//WriteLog::write_log("FeatureFactory:getStackLabel:s_stack.size():error:索引超出栈的范围");
		
		 return templable;
	}
	for(int i=0;i<nodenum;i++)
	{
		s_stack.pop();
	}
	Node* tempNode=s_stack.top();
	if(tempNode==NULL)return templable;
	if(tempNode->m_psPOS!=NULL)templable.conlabel=string(tempNode->m_psPOS);
	Node*head=tempNode->m_pHead;
	while(true)
	{
	switch(trasitions)
	{
		case (LEFT):
			{
			if(tempNode->m_childNum==0)return templable;
			tempNode=tempNode->m_pLeftChild;
			if(tempNode->m_psPOS!=NULL&&head->m_psPOS!=NULL)
			{
				if(strcmp(tempNode->m_psPOS,head->m_psPOS)!=0)
				{
					//templable.conlabel=string(tempNode->m_psPOS);
					templable.taglabel=string(tempNode->m_psPOS);
					if(tempNode->m_psWord!=NULL)templable.wordlabel=string(tempNode->m_psWord);
					return templable;
				}
			}
			
			}
		case(RIGHT):
			{
			if(tempNode->m_childNum==0)return templable;
			if(tempNode->m_childNum==1)
		    {
			  tempNode=tempNode->m_pLeftChild;
			  continue;
			}
			tempNode=tempNode->m_pRightChild;
			if(tempNode->m_psPOS!=NULL&&head->m_psPOS!=NULL)
			{
				if(strcmp(tempNode->m_psPOS,head->m_psPOS)!=0)
				{
					
					templable.taglabel=string(tempNode->m_psPOS);
					if(tempNode->m_psWord!=NULL)templable.wordlabel=string(tempNode->m_psWord);
					return templable;
				}
			}
			}
		default:; //WriteLog::write_log("FeatureFactory:getStackLabel:trasitions:error");
		   

	}
	}
}
