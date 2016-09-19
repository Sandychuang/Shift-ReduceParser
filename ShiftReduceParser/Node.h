#include "stdafx.h"  
///////////////////////////////////////////////////////////////////////
// 
//          Node:				树结构类，程序基础数据结构，表示句法分析树
//			相关类：				
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
#pragma once
class Node  
{
public:
	bool IsBNP;
	bool IsCoord;
	BOOL IsLeafNode();
	BOOL IsPreTerminal();//判断一个节点是不是词性节点的表示。
	BOOL IsPrePreTerminal();//判断一个节点是不是单单个的词性节点。目前描述不清，一元边；
	BOOL IsPhrase();//判断一个节点是不是短语标记节点。目前描述不清，二元边或多元边；
	Node*  m_pNextBrother;	//当前节点的下一个兄弟结点
	Node*  m_pPrevBrother;	//当前节点的前一个兄弟结点
	Node*  m_pHead;		    //核心子节点
	Node*  m_pLeftChild;	//最左儿子结点
	Node*  m_pRightChild;	//最右儿子结点
	Node*  m_pParent;		//父亲结点
	char*	m_psWord;		//词
	char*	m_psPOS;
	CString headlabel;
	CString headtag;
	CString headword;
	int m_childNum;
	bool lc_adj;
	bool lc_verb;
	bool rc_adj;
	bool rc_verb;
	int start;
	int end;
	bool IsNone;
	bool binaryFig;
	bool isSeparator;
	int  pos;
	Node();
	virtual ~Node();

};

