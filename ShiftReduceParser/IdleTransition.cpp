#include "StdAfx.h"
#include"IdleTransition.h"
IdleTransition::IdleTransition()
{
}
IdleTransition::~IdleTransition()
{
}
string IdleTransition::tostring()
{
	return "Idle";
}
void IdleTransition::apply(State &state)
{
	//return 
		apply(state,0.0);
}
void IdleTransition::apply(State &state,float scoreDelta)
{
	//State temp;
	//return state;
	state.transition.push_back(this);
	state.score=state.score+scoreDelta;
	state.newNode=NULL;
}
bool IdleTransition::isLeagal(State&state,vector<string>&constraints)
{
	if(state.isFinised())
	
	{
		return true;
	}
	
	
	return false;
}
//void IdleTransition::Serialize(CArchive& ar)
//{
//}