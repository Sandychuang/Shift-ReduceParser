#include "stdafx.h"  
#pragma once
#include "State.h"
#include <vector>
#include <set>
#include <boost/archive/text_iarchive.hpp>  
#include <boost/archive/text_oarchive.hpp> 
///////////////////////////////////////////////////////////////////////
// 
//          Transition.h: 转移动作抽象类
//			相关类：       ShiftTransition BinaryTransition UnaryTransition CompoundUnaryTransition FinalizeTransition IdleTransition 
//          程序员:       xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
class Transition{

	
public :
	        Transition(){}
			//实施转移动作
	        virtual  void apply(State& state)=0 ;
			//转化为字符串
		    virtual   string tostring()=0;
			//实施转移动作
			virtual void apply(State& state,float scoreDelta)=0;
			//判断是否合法
			virtual bool isLeagal(State&state,vector<string>&constraints)=0;
			//序列化
			friend class boost::serialization::access;  
			template<class Archive>  
			void serialize(Archive & ar, const unsigned int version){}
			virtual ~Transition(){}

};
BOOST_SERIALIZATION_ASSUME_ABSTRACT(Transition)  