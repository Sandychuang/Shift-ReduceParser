#include<stdlib.h>
#include<string>
#include "Transition.h"
///////////////////////////////////////////////////////////////////////
// 
//          ShiftTransition:    Shift转移动作类
//			父类：				Transition
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
 class ShiftTransition: public Transition{
 
	
 public:string m_label;
 public:
	ShiftTransition();
	~ShiftTransition();
	string tostring();
    bool isLeagal(State&state,vector<string>&containts);
	void apply(State& state);
	void apply(State &state,float scoreDelta);
    friend class boost::serialization::access;  
	template<class Archive>  
    void serialize(Archive & ar, const unsigned int version)  
    {  
          ar & boost::serialization::base_object<Transition>(*this);  
		  ar & m_label;  
    }  
			


};
