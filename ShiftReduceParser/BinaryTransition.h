#include "Transition.h"
///////////////////////////////////////////////////////////////////////
// 
//          BinaryTransition:    Binary转移动作类
//			父类：				Transition
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
 class BinaryTransition:public Transition{

 public:
	 string m_label;
	 enum Side { LEFT, RIGHT  };
	 Side m_side;
 
 public:
	BinaryTransition();
	~BinaryTransition();
	BinaryTransition(string label,Side side);
	bool isTemporary(Node* pNode);
	bool isTemporary();
	bool isBinarized();
	bool isSeparator(Node* pNode);
	bool isSeparator(stack<Node*> tempstack);
	string tostring();
    bool isLeagal(State&state,vector<string>&containts);
	void apply(State&state);
	void apply(State&state,float scoreDelta);
    friend class boost::serialization::access;  
	template<class Archive>  
     void serialize(Archive & ar, const unsigned int version)  
      {  
          ar & boost::serialization::base_object<Transition>(*this);  
		  ar & m_label;
		  ar & m_side;

      }  
			
};