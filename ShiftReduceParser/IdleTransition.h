#include "Transition.h"
///////////////////////////////////////////////////////////////////////
// 
//          IdleTransition: Idleת�ƶ�����
//			���ࣺ				Transition
//          ����Ա:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
using namespace std;
 class IdleTransition:public Transition{
public:
	IdleTransition();
	~IdleTransition();
	string tostring();
    bool isLeagal(State&state,vector<string>&containts);
	void apply(State &state);
	void apply(State &state,float scoreDelta);
	friend class boost::serialization::access;  
	template<class Archive>  
    void serialize(Archive & ar, const unsigned int version)  
    {  
      ar & boost::serialization::base_object<Transition>(*this);  
    }  
			
};