#include "StdAfx.h"
#include"UnaryTransition.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
UnaryTransition::UnaryTransition()
{
}
UnaryTransition::UnaryTransition(string label,bool isRoot)
{
	m_label=label;
	m_isRoot=isRoot;
}
UnaryTransition::~UnaryTransition()
{
}
string UnaryTransition::tostring()
{
	string temp=m_isRoot ? "*" : "";
	return "Unary" + temp + "(" + m_label + ")";
}
void UnaryTransition::apply(State&state)
{
	//return
	apply(state,0.0);
}
void UnaryTransition::apply(State&state,float scoreDelta)
{
	//State temp;
	//return state;
	if(state.stack.size()==0)return;
	Node*tempNode=state.stack.top();
	state.stack.pop();
	Node* newNode=new Node();
	newNode->m_pLeftChild=tempNode;
	newNode->m_psPOS=new char[m_label.size()+1];
	strcpy(newNode->m_psPOS,m_label.c_str());
	
	Node*head=tempNode;
	if(head->m_pHead!=NULL)
	{
		newNode->m_pHead=head->m_pHead;
	}
	else
	{
	  newNode->m_pHead=head;
	}
	newNode->headlabel=newNode->m_pHead->m_psPOS;
    newNode->headword=newNode->m_pHead->m_psWord;
    newNode->headtag=newNode->m_pHead->m_psPOS;
	newNode->m_childNum=1;
	state.transition.push_back(this);
	state.stack.push(newNode);
	//state.newNode.insert(newNode);
	state.score=state.score+scoreDelta;
	state.newNode=newNode;

}
bool UnaryTransition::isLeagal(State&state,vector<string>&constraints)
{
	if(state.stack.size()==0)return false;
	if(state.isFinised())return false;
	if(strcmp(state.stack.top()->m_psPOS,"S")==0)
	return false;
	
	if(state.stack.top()->m_childNum==1)return false;
	if(strcmp(m_label.c_str(),"S")!=0)return false;
	return true;
}
//void UnaryTransition::Serialize(CArchive& ar)
//{
//
//}
