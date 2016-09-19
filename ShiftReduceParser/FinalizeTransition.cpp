#include "StdAfx.h"
#include"FinalizeTransition.h"
FinalizeTransition::FinalizeTransition()
{
}
FinalizeTransition::~FinalizeTransition()
{
}
FinalizeTransition::FinalizeTransition(set<string> rootstates)
{
	m_rootstates=rootstates;
}
string FinalizeTransition::tostring()
{
	return "finalize";
}
void FinalizeTransition::apply(State &state)
{
	//return
		apply(state,0.0);
}
void FinalizeTransition::apply(State &state,float scoreDelta)
{
	state.isfinished=true;
	state.transition.push_back(this);
	state.score=state.score+scoreDelta;
	state.newNode=NULL;
	//return state;
}
bool FinalizeTransition::isLeagal(State&state,vector<string>&constraints)
{
	

	if(state.stack.size()>0)
	{
	auto itt=m_rootstates.find(string(state.stack.top()->m_psPOS));
	
	bool isleagal=state.isFinised()!=true&&state.endOfQueue()==1&&state.stack.size()==1;//&&itt!=m_rootstates.end();
	
	if(isleagal!=1)
	{
		return false;
	}
	
	return true;
	}else
	{
		return false;
	}
}
//void FinalizeTransition:: Serialize(CArchive& ar)
//{
//}