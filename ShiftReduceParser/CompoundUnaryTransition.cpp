#include "StdAfx.h"
#include"CompoundUnaryTransition.h"
CompoundUnaryTransition::CompoundUnaryTransition()
{
}
CompoundUnaryTransition::~CompoundUnaryTransition()
{
}
CompoundUnaryTransition::CompoundUnaryTransition(vector<string> labels,bool isRoot)
{
	for(int i=0;i<labels.size();i++)
	{
		m_labels[i]=labels[i];
	}
	m_isRoot=isRoot;
}
string CompoundUnaryTransition::tostring()
{
	return "compoundunary";
}
void CompoundUnaryTransition::apply(State &state)
{
	return apply(state,0.0);
}
void CompoundUnaryTransition::apply(State&state,float scoreDelta)
{
	//State temp;
	//return state;
}
bool CompoundUnaryTransition::isLeagal(State&state,vector<string>&constraints)
{
	return false;
}
//void CompoundUnaryTransition::Serialize(CArchive& ar)
//{
//
//}