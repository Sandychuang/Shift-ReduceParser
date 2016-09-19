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
//          State:				状态类，短语结构分析时存储句子的中间状态信息。
//			相关类：				
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
 class Transition;//避免头文件的相互调用出现错误
class State{
public :
	float score;
	bool isfinished;
	//int lastPos;///记下当前节点的增加位置;
	//set<Node*> newNode;///加下当前state新增加的节点;
	Node* newNode;//加下当前state新增加的节点
    vector<Node*> sentence;//原始叶子节点信息，及队列信息
    stack<Node*> stack;//栈信息
	list<Transition*> transition;//已完成的转移动作列表
	map<int,string> separators;//分隔符信息
	int tokenPosition;//已完成队列的长度
    enum HeadPosition { NONE, LEFT, RIGHT, BOTH, HEAD };//头结点的位置
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
	void clear(set<Node*>& newNode)///产生的内存节点外部释放
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
					WriteLog::write_log("Sate:clear:释放内存失败");
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
	bool isFinised()//状态是否结束，及是否完成短语结构的分析
	{
		return isfinished;
	}
	bool endOfQueue()//队列是否分析完毕
	{
		return sentence.size()<=tokenPosition;
	}
	 bool operator<(const State& a) const// ,const Pair& b)const函数导致调试浪费时间,优先队列中也必须符合这种类型；
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
//删除新的节点信息
	 ////得到栈顶两个元素的分隔符。
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
