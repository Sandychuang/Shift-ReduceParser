#include "Transition.h"
///////////////////////////////////////////////////////////////////////
// 
//          CompoundUnaryTransition:     CompoundUnaryת�ƶ�����
//			���ࣺ				Transition
//          ����Ա:				xu sa	2016 01 07
//          ˵��������δʹ��
//////////////////////////////////////////////////////////////////////
using namespace std;
 class CompoundUnaryTransition:public Transition{
 public:
	 vector<string> m_labels;
	 bool m_isRoot;
public:
	CompoundUnaryTransition();
	~CompoundUnaryTransition();
	CompoundUnaryTransition(vector<string> labels, bool isRoot);
	string tostring();
    bool isLeagal(State&state,vector<string>&containts);
	void apply(State&state);
	void apply(State&state,float scoreDelta);
	friend class boost::serialization::access;  
	template<class Archive>  
     void serialize(Archive & ar, const unsigned int version)  
      {  
          ar & boost::serialization::base_object<Transition>(*this);  
		  //ar & m_label;

      }  
			

};
