#include "Transition.h"
///////////////////////////////////////////////////////////////////////
// 
//          FinalizeTransition: Finalize转移动作类
//			父类：				Transition
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
 class FinalizeTransition:public Transition{
 public:
	 set<string> m_rootstates;
public:
	FinalizeTransition();
	~FinalizeTransition();
	FinalizeTransition(set<string> rootstates);
	string tostring();
    bool isLeagal(State&state,vector<string>&containts);
	void apply(State &state);
	void apply(State &state,float scoreDelta);
	friend class boost::serialization::access;  
	template<class Archive>  
     void serialize(Archive & ar, const unsigned int version)  
      {  
          ar & boost::serialization::base_object<Transition>(*this);  
		  //ar & m_label;  
      }  
			

};