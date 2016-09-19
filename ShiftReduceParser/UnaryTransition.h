#include "Transition.h"
///////////////////////////////////////////////////////////////////////
// 
//          UnaryTransition:    Unary转移动作类
//			父类：				Transition
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
 class UnaryTransition:public Transition{
 public :
	 string m_label;
	 bool   m_isRoot;
 public:
	UnaryTransition();
	~UnaryTransition();
	UnaryTransition(string label,bool isRoot);
	string tostring();
    bool isLeagal(State&state,vector<string>&containts);
	void apply(State &state);
	void apply(State &state,float scoreDelta);
	/*void Serialize(CArchive& ar);*/
	friend class boost::serialization::access;  
	template<class Archive> 
	void serialize(Archive & ar, const unsigned int version)  
    {  
          ar & boost::serialization::base_object<Transition>(*this);  
		  ar & m_label;  
    }  
			
};