#include "StdAfx.h"
#include"BinaryTransition.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BinaryTransition::BinaryTransition()
{
}
BinaryTransition::BinaryTransition(string label,Side side)
{
	m_label=label;
	m_side=side;
}
BinaryTransition::~BinaryTransition()
{
}
string BinaryTransition::tostring()
{
	switch(m_side) {
    case LEFT:
      return "LeftBinary(" + m_label + ")";
    case RIGHT:
      return "RightBinary(" + m_label + ")";
    default:
      return ("Unknown side ");
	}
}
bool BinaryTransition::isTemporary(Node* pNode)
{
	string label(pNode->m_psPOS);
	return label.find("@")!=string::npos;

}
bool BinaryTransition::isTemporary()
{
	//string label(pNode->m_psPOS);
	return m_label.find("@")!=string::npos;

}
bool BinaryTransition::isSeparator(Node* pNode)
{
	string label(pNode->m_psPOS);
	return label.find("wp")!=string::npos;

}
bool BinaryTransition::isSeparator(stack<Node*> tempstack)
{
	if(isSeparator(tempstack.top()))return true;
	if(tempstack.size()>0)tempstack.pop();
	if(isSeparator(tempstack.top()))return true;
	return false;
}
bool BinaryTransition::isBinarized()
{
	return m_label.find("@S")!=string::npos;
}
void BinaryTransition::apply(State&state)
{
	return apply(state,0.0);
}
void BinaryTransition::apply(State&state,float scoreDelta)
{
	if(state.stack.size()<2)return;
	Node*right=state.stack.top();
	state.stack.pop();
	Node*left=state.stack.top();
	state.stack.pop();
	Node* head;
	switch(m_side) {
    case LEFT:
      head = left;
      break;
    case RIGHT:
      head = right;
      break;
    default:NULL;
	  //////////////
	  //////	日志记录内容
	//return state;
	}
	Node* newNode=new Node();
	newNode->m_pLeftChild=left;
	newNode->m_pRightChild=right;
	if(head->m_pHead!=NULL)
	{newNode->m_pHead=head->m_pHead;
	}
	else
	{
	  newNode->m_pHead=head;
	}
	newNode->m_psPOS=new char[m_label.size()+1];
	strcpy(newNode->m_psPOS,m_label.c_str());
	newNode->headlabel=head->headlabel;
    newNode->headword=head->headword;
    newNode->headtag=head->headtag;
	newNode->m_childNum=2;
	state.stack.push(newNode);
	//state.newNode.insert(newNode);
	state.transition.push_back(this);
	state.score=state.score+scoreDelta;
	state.newNode=newNode;

	
}
bool BinaryTransition::isLeagal(State&state,vector<string>&constraints)
{
	//DWORD start=GetTickCount();
	if(state.isFinised())return false;
	if(state.stack.size()<=1)return false;
	/*if(isSeparator(state.stack))
	{ 
	  if(isBinarized())
	  {return true;}else
	  {
		  return false;
	  }
	}*/
	 
	/*DWORD finish=GetTickCount();
    double duration = (double)(finish - start) /1000;
	if(duration>0.015)
	{
		int c=0;
	}
    cout<<duration<<endl;*/
	stack<Node*> tempstack=state.stack;
	Node* tempNode1=tempstack.top();
	string label1(tempNode1->m_psPOS);
	tempstack.pop();
	Node* tempNode2=tempstack.top();
	string label2(tempNode2->m_psPOS);
	if(label2.find("@")!=string::npos&&label1.find("@")!=string::npos)return false;
	if(label1.find("@")!=string::npos)
	{
		//if(m_side==LEFT)return false;
		if(label1.find(m_label)==string::npos)return false;
	}
	if(label2.find("@")!=string::npos)
	{
		//if(m_side==RIGHT)return false;
		if(label2.find(m_label)==string::npos)return false;
	}
	/*if(label2.find("@")!=string::npos)
	{
		if(label2.find(m_label)==string::npos)return false;
	}*/
	/*if(label1.find("wj")!=string::npos)
	{
	   if(strcmp(label1.c_str(),"S")!=0)return false;
	}*/
	//if(label1.find("wo")!=string::npos||label2.find("wo")!=string::npos)
	//{
	//   if(m_label.find("@")==string::npos)return false;
	//}
	//
	//if(label1.find("wp")!=string::npos||label2.find("wp")!=string::npos)
	//{
	//   if(m_label.find("S")==string::npos)return false;
	//}
	//
	//if(strcmp(label1.c_str(),"c")==0)
	//{
	// if(m_label.find("@")==string::npos)return false;
	//}
	if(m_label.find("INP")!=string::npos)
	{
	  if(label2.find("c")==string::npos&&label2.find("w")==string::npos)return false;
	}
	if(!state.endOfQueue())
	{
		if(strcmp(m_label.c_str(),"S")==0)return false;
	}
	if (state.stack.size() == 2 && isTemporary() && state.endOfQueue()) {
      return false;
    }
	//if (state.stack.size() == 2 && isTemporary() &&m_side==LEFT)return false;
    
	if (state.stack.size() > 2)
	{
		
	tempstack.pop();
	Node* tempNode3=tempstack.top();
	string label3(tempNode3->m_psPOS);
	if( label3.find("@")!=string::npos&&isTemporary() && state.endOfQueue())return false;
	//if( label3.find("@")!=string::npos&&isTemporary() && m_side==LEFT)return false;
    
   }
	return true;
	
}
//void BinaryTransition::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())  
//    {  
//        
//		CString label=this->m_label.c_str();
//		int side=-1;
//		switch(m_side) {
//    case LEFT:side=0;
//      break;
//    case RIGHT:side=1;
//		break;
//    default:
//          side=-1;
//	    }
//		ar<<label<<side;
//    }  
//    else 
//    {  
//		CString temp;
//		int side;
//		ar>>temp>>side;
//		if(side==0)
//		{
//			this->m_side=LEFT;
//
//		}
//		if(side==1)
//		{
//			this->m_side=RIGHT;
//		}
//       
//		this->m_label=temp.GetBuffer(0);
//    }  
//}