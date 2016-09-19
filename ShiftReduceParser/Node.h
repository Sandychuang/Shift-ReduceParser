#include "stdafx.h"  
///////////////////////////////////////////////////////////////////////
// 
//          Node:				���ṹ�࣬����������ݽṹ����ʾ�䷨������
//			����ࣺ				
//          ����Ա:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
#pragma once
class Node  
{
public:
	bool IsBNP;
	bool IsCoord;
	BOOL IsLeafNode();
	BOOL IsPreTerminal();//�ж�һ���ڵ��ǲ��Ǵ��Խڵ�ı�ʾ��
	BOOL IsPrePreTerminal();//�ж�һ���ڵ��ǲ��ǵ������Ĵ��Խڵ㡣Ŀǰ�������壬һԪ�ߣ�
	BOOL IsPhrase();//�ж�һ���ڵ��ǲ��Ƕ����ǽڵ㡣Ŀǰ�������壬��Ԫ�߻��Ԫ�ߣ�
	Node*  m_pNextBrother;	//��ǰ�ڵ����һ���ֵܽ��
	Node*  m_pPrevBrother;	//��ǰ�ڵ��ǰһ���ֵܽ��
	Node*  m_pHead;		    //�����ӽڵ�
	Node*  m_pLeftChild;	//������ӽ��
	Node*  m_pRightChild;	//���Ҷ��ӽ��
	Node*  m_pParent;		//���׽��
	char*	m_psWord;		//��
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

