#include "stdafx.h"  
#include"ShiftTransition.h"
//IMPLEMENT_SERIAL(ShiftTransition, CObject, 1)

ShiftTransition::ShiftTransition()
{
	m_label="shift";
}
ShiftTransition::~ShiftTransition()
{
}
string ShiftTransition::tostring()
{
	return "shift";
}
void ShiftTransition::apply(State&state)
{
     apply(state,0.0);
}
void ShiftTransition::apply(State&state,float scoreDelta)
{
	if(state.tokenPosition>=state.sentence.size())return;
	Node* temp= state.sentence[state.tokenPosition];
	state.stack.push(temp);
	state.tokenPosition+=1;
	state.transition.push_back(this);
	state.score=state.score+scoreDelta;
	state.newNode=NULL;


	
}
bool ShiftTransition::isLeagal(State&state,vector<string>&constraints)
{
	if(state.isFinised())return false;
	if(state.tokenPosition<state.sentence.size())
	{
		return true;
	}
	else
	{
	     return false;
	}
}
