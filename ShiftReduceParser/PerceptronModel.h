#pragma once
#include<vector>
#include<set>
#include<string>
#include"Transition.h"
#include"Weight.h"
#include<map>
#include<queue>
#include<algorithm>
#include"BuildTree.h"
#include"BasicFeatureFactory.h"
//#include "BuildTree.h"
///////////////////////////////////////////////////////////////////////
// 
//          PerceptronModel:	���ڽṹ����֪ģ���࣬��ɾ䷨����ѵ����Ԥ�⡢ģ�͵Ȳ���
//			����ࣺ				
//          ����Ա:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
class PerceptronModel{
public:PerceptronModel();
	   ~PerceptronModel();
	   vector<Transition*> IndexTransition;
	   set<string> KownStates;
	   set<string> RootStates;
	   set<string> RootOnlyStates;
	   map<string,Weight*> featureWeights;
	   BasicFeatureFactory featureFactory;
	   int trainingIterations;
	   int BeamSize;
	   int maxBeamSize;
	   CBuildTree buildTree;
	   struct Pair{
		   int index;
		   float score;
		   Pair()
		   {
			   index=0;
			   score=0.0;
		   }
		  
		  friend bool operator<( Pair a,Pair b)// ,const Pair& b)const�������µ����˷�ʱ��
		   {
			 
			 /*  if(a.score==42.0||b.score==42.0)
			   {
				   int c=0;
			   }*/
			  /* if(a.score<b.score)
			   {
				   return true;
			   }
			   else
			   {
				   return false;
			   }*/
			   return a.score>b.score;///��С�������У�
			  
		   }

	/*	  friend bool operator >(Pair a,Pair b)  
  {  
       return a.score<b.score;  
   }  */

		   //  bool operator>(const Pair& a)// ,const Pair& b)
		   //{
			  //if(score>a.score)
			  //{
			  //return true;
			  //}
			  //else
			  //{
				 // return false;
			  //}		  
			  //
		   //}
			  
	   };
	   struct Update{
		   list<string> features;
		   int goldTransition;
		   int predictedTransition;
		   float delta;
		   Update()
		   {
			   delta=1.0;
		   }
		   Update(list<string> m_features, int goldtransition,int predictedtransition,float m_delta)
		   {
			   features=m_features;
			   goldTransition=goldtransition;
			   predictedTransition=predictedtransition;
			   delta=m_delta;
		   }

	   };
	   void findHighestScoringTransitions(State &state,bool isLegal,int numTransitions,vector<string>constraints,priority_queue<Pair>& queue);
	   void findHighestScoringTransitions(State &state,list<string>&features,bool isLegal,int numTransitions,vector<string>constraints,priority_queue<Pair>& queue);
	   void findHighestScoringTransitions(State&state,list<string>&features,bool isLegal,int numTransitions,vector<string>constraints,Pair& pair);
	   pair<int,int> trainTree(int index,vector<Node*>&binarizedTrees,vector<list<Transition*>> &transitionLists,list<Update>&Updates);
	   pair<int,int> trainTree_withBeam(int index,vector<Node*>&binarizedTrees,vector<list<Transition*>> &transitionLists,list<Update>&Updates);
	   
	   void trainModel(vector<Node*>&binarizedTrees,vector<list<Transition*>>&transitionLists);
	   void initialStateFromTaggedSentence(vector<Node*>& sentence,Node* pNode);
	   bool initialStateFromTaggedSentence(vector<Node*>& sentence,string strin);
	   State initialStateFromGoldTagTree(Node* pNode);
	   ///���ö������л��Ĳ�������////////
	   bool outPutModel(const char* KnownStatesFile,const char* rootstatesfile,const char* rootonlystatesfile,const char* indextranitonfile,const char* featureweightsfile);
	   bool inPutModel(const char* KnownStatesFile,const char* rootstatesfile,const char* rootonlystatesfile,const char* indextranitonfile,const char* featureweightsfile);
	   bool isTransitonEqual(State& one,State& other);
	   bool findStateOnAgent( priority_queue<State> queueState,State& other);
	   bool parser(const char* senin,char* senout,int &senoutlen);
	   bool parser_s(const char* senin,char* &senout,int &senoutlen);
	   bool unload();

};
