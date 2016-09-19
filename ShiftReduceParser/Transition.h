#include "stdafx.h"  
#pragma once
#include "State.h"
#include <vector>
#include <set>
#include <boost/archive/text_iarchive.hpp>  
#include <boost/archive/text_oarchive.hpp> 
///////////////////////////////////////////////////////////////////////
// 
//          Transition.h: ת�ƶ���������
//			����ࣺ       ShiftTransition BinaryTransition UnaryTransition CompoundUnaryTransition FinalizeTransition IdleTransition 
//          ����Ա:       xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
class Transition{

	
public :
	        Transition(){}
			//ʵʩת�ƶ���
	        virtual  void apply(State& state)=0 ;
			//ת��Ϊ�ַ���
		    virtual   string tostring()=0;
			//ʵʩת�ƶ���
			virtual void apply(State& state,float scoreDelta)=0;
			//�ж��Ƿ�Ϸ�
			virtual bool isLeagal(State&state,vector<string>&constraints)=0;
			//���л�
			friend class boost::serialization::access;  
			template<class Archive>  
			void serialize(Archive & ar, const unsigned int version){}
			virtual ~Transition(){}

};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Transition)  