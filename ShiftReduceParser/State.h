#include "stdafx.h"  
#pragma once
#include <string>
#include <stack>
#include "Node.h"
#include"WriteLog.h"
#include <list>
#include<set>
#include <map>
#include<vector>
///////////////////////////////////////////////////////////////////////
// 
//          State:				״̬�࣬����ṹ����ʱ�洢���ӵ��м�״̬��Ϣ��
//			����ࣺ				
//          ����Ա:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
 class Transition;//����ͷ�ļ����໥���ó��ִ���
class State{
public :
	float score;
	bool isfinished;
	//int lastPos;///���µ�ǰ�ڵ������λ��;
	//set<Node*> newNode;///���µ�ǰstate�����ӵĽڵ�;
	Node* newNode;//���µ�ǰstate�����ӵĽڵ�
    vector<Node*> sentence;//ԭʼҶ�ӽڵ���Ϣ����������Ϣ
    stack<Node*> stack;//ջ��Ϣ
	list<Transition*> transition;//����ɵ�ת�ƶ����б�
	map<int,string> separators;//�ָ�����Ϣ
	int tokenPosition;//����ɶ��еĳ���
    enum HeadPosition { NONE, LEFT, RIGHT, BOTH, HEAD };//ͷ����λ��
	//State& operator=(const State&rhs)
	//{
	//	//*this=rhs;
	//	score=rhs.score;
 //       isfinished=rhs.isfinished;
	//	newNode=rhs.newNode;
	//	sentence=rhs.sentence;
 //       stack=rhs.stack;
	//	transition= rhs.transition;
	//	separators =rhs.separators;
	//	tokenPosition=rhs.tokenPosition;
	//	lastPos=rhs.newNode.size();
	//	return *this;
	//}
	//State(const State&rhs)
	//{
	//	//*this=rhs;
	//	score=rhs.score;
 //       isfinished=rhs.isfinished;
	//	newNode=rhs.newNode;
	//	sentence=rhs.sentence;
 //       stack=rhs.stack;
	//	transition= rhs.transition;
	//	separators =rhs.separators;
	//	tokenPosition=rhs.tokenPosition;
	//	lastPos=rhs.newNode.size();
	//	//lastPos=rhs.newNode.size();
	//}
public : 
	State(){
	score=0.0;
	isfinished=false;
	tokenPosition=0;
	newNode=NULL;
	//lastPos=0;
	}
	~State(){
	
	//clear();
	}
	void clear(set<Node*>& newNode)///�������ڴ�ڵ��ⲿ�ͷ�
	{
		auto itt=newNode.begin();
		
		for(;itt!=newNode.end();itt++)
		{
			Node*temp=*itt;
			if(temp!=NULL)
			{
				try{
					delete temp;
					temp=NULL;
				}
				catch(...)
				{
					WriteLog::write_log("Sate:clear:�ͷ��ڴ�ʧ��");
				}
				//stack.pop();
				//*itt=NULL;
			}
		}
		/*for(;itt!=sentence.end();itt++)
		{
			delete *itt;
			*itt=NULL;
		}*/
	}
	bool isFinised()//״̬�Ƿ���������Ƿ���ɶ���ṹ�ķ���
	{
		return isfinished;
	}
	bool endOfQueue()//�����Ƿ�������
	{
		return sentence.size()<=tokenPosition;
	}
	 bool operator<(const State& a) const// ,const Pair& b)const�������µ����˷�ʱ��,���ȶ�����Ҳ��������������ͣ�
		   {
			 
			   
			   return score>a.score;
			  
		   }
	/*bool  isTransitonEqual(State& other)
	{
		if(transition.size()!=other.transition.size())return false;
		auto icc=transition.begin();
		auto igg=other.transition.begin();
		
		for(;icc!=transition.end()&&igg!=transition.end();icc++,igg++)
		{
			
			if(strcmp((*igg)->tostring().c_str(),(*icc)->tostring().c_str())!=0)
			{
				return false;
			}
		}



		return true;

	}*/
//ɾ���µĽڵ���Ϣ
	 ////�õ�ջ������Ԫ�صķָ�����
	 string getSeparator(Node*& right,Node*&left)
	 {
	  if (right ==NULL || left == NULL)return "";
	  int rightPos=-1;
	  if(right->m_pHead!=NULL)
	  {
		  rightPos=right->m_pHead->pos;
	  }
	  else
	  {
		   rightPos=right->pos;
	  }
	    int leftPos=-1;
	  if(left->m_pHead!=NULL)
	  {
		  leftPos=left->m_pHead->pos;
	  }
	  else
	  {
		   leftPos=left->pos;
	  }
      if (rightPos ==-1 || leftPos == -1)return"";
	  //int count=0;
	  for(int i=leftPos;i<=rightPos;i++)
	  {
		  if(sentence[i]->isSeparator)
		  {
			  string temp(sentence[i]->m_psWord);
			  return temp;
		  }

	  }
	  return "";
	 }
	 int getSeparatorCount(Node*& right, Node*&left) 
	 {
	   if (right ==NULL || left == NULL)return 0;
	   int rightPos=-1;
	  if(right->m_pHead!=NULL)
	  {
		  rightPos=right->m_pHead->pos;
	  }
	  else
	  {
		   rightPos=right->pos;
	  }
	    int leftPos=-1;
	  if(left->m_pHead!=NULL)
	  {
		  leftPos=left->m_pHead->pos;
	  }
	  else
	  {
		   leftPos=left->pos;
	  }
      if (rightPos ==-1 || leftPos == -1)return 0;
	  int count=0;
	  for(int i=leftPos;i<=rightPos;i++)
	  {
		  if(sentence[i]->isSeparator)count++;

	  }
	  return count;

	 }
private: void delete_NewNode(Node*&pNode)
	
	{if(pNode==NULL)return;
	if(pNode->IsLeafNode())
	{
		return;
	}
	
	Node* tempLeftNode=pNode->m_pLeftChild;
	if(tempLeftNode!=NULL)
	{
		delete_NewNode(tempLeftNode);
	
	}
	Node* tempRightNode=pNode->m_pRightChild;
	if(tempRightNode!=NULL)
	{
		delete_NewNode(tempRightNode);
	}
	//delete []pNode->m_psPOS;
	delete pNode;
	pNode=NULL;
}


};
