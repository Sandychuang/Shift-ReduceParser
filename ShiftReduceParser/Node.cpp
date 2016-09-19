
#include "stdafx.h"

#include "Node.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Node::Node()
{
	m_pNextBrother=NULL;	
	m_pPrevBrother=NULL;		
	m_pHead=NULL;		
	m_pLeftChild=NULL;		
	m_pRightChild=NULL;	
	m_pParent=NULL;	
	m_psWord=NULL;
	m_psPOS= NULL;
	lc_adj=1;
	lc_verb=0;
	rc_adj=1;
	rc_verb=0; 
	IsNone=false;
	IsCoord=false;
	IsBNP=false;
	m_childNum=0;
	binaryFig=false;
	isSeparator=false;
	pos=-1;

}

Node::~Node()
{
 

 	if ( m_psWord != NULL )
	{
		delete []m_psWord;
		m_psWord = NULL;
		 
	}
	if ( m_psPOS != NULL )
	{
		delete  []m_psPOS;
		m_psPOS = NULL;
		 
	}
 
	
}

BOOL Node::IsLeafNode()
{
	if(this == NULL)
	{
		return 0;
	}

	return ( m_pLeftChild == NULL );
}
////判断是不是一元节点；
BOOL Node::IsPreTerminal()
{
	if(this==NULL){
		return 0;
	}
	return (m_childNum==1&&m_pLeftChild->IsLeafNode());

}
////判断多元边的孩子节点是不是一元节点；
BOOL Node::IsPrePreTerminal()
{
	if(this ==NULL)
	{
		return 0;
	}
	if(m_childNum==0)
	{
	  return false;
	}
	Node * temp;
	temp=this->m_pLeftChild;
	while(temp!=NULL)
	{
		if(!temp->IsPreTerminal())
		{
			return false;
		}
		temp=temp->m_pNextBrother;
	}
	return true;

}
BOOL Node::IsPhrase()
{
	return !(m_pLeftChild==NULL||m_childNum==0||(m_childNum==1&&m_pLeftChild->IsLeafNode()));
}
