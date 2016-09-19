#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include"ShiftTransition.h"
#include"CompoundUnaryTransition.h"
#include"IdleTransition.h"
#include"BinaryTransition.h"
#include"FinalizeTransition.h"
#include"UnaryTransition.h"
#include "State.h"
#include<list>
///////////////////////////////////////////////////////////////////////
// 
//          CBuildTree:         方法类，主要完成tree结构的操作
//			相关类：				
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////
using namespace std;
class CBuildTree  
{
public: map<string,vector<vector<string>>> nonTerminalInfo;
public:
	void TagNone(Node * pNode);
	void DeleteNone(Node *pNode);
	void PrevTravel(Node *pNode,CString &strSentence);
	void delete_tree(Node * &pNode);
	void build_tree(CString S,Node *&pNode);
	Node* determinHeadNode(Node *&pNode);
	Node* createNewNode(Node*&pLeftNode,Node*&pRightNode,CString label);
	void createTransitionSequence(list<Transition*>& transition,Node* &pNode,set<string> rootstates,set<string> rootonlystates);
	void createTransitionSequenceHelper(list<Transition*>& transition,Node* &pNode,set<string> rootstates,set<string> rootonlystates);
	/*void initialStateFromTaggedSentence(vector<Node*>& sentence,Node* pNode);
	State initialStateFromGoldTagTree(Node* pNode);*/
	void tansformTree(Node*&pNode);
	void deTansformTree(Node*&pNode);
	bool treeToString(Node*&pNode,string&senOut);
	void findRightChild(Node*&pNode,Node*&root,Node*&tempChild);
	void findLeftChild(Node*&pNode,Node*&root,Node*&tempChild);
	void Gethead_Star(Node *&pNode);
	CBuildTree();
	void findKnownStates(vector<Node*>&binarizedTrees,set<string>&knownStates);
	void findKnownStates(Node*&pNode,set<string>&knownStates);
	void findRootOnlyStates(Node*&pNode,set<string>&RootOnlyStates);
	void findRootOnlyStates(vector<Node*>&binarizedTrees,set<string>&RootOnlyStates);
	void findRootStates(vector<Node*>&binarizedTrees,set<string>&RootStates);
	void delete_Node(Node * &pNode);
	Node* traverLoate(vector<Node*>&childVector,vector<string>&condicate,bool lastResort);
	int  findLeftHead(vector<Node*>&childVector,vector<string>&condicate);
	int  findLeftDisHead(vector<Node*>&childVector,vector<string>&condicate);
	int  findRightHead(vector<Node*>&childVector,vector<string>&condicate);
    int  findRightDisHead(vector<Node*>&childVector,vector<string>&condicate);
	vector<string> split(const string& s, const string& delim);
	bool readCollinsHeadRule(const char * filename);
	void Gethead(Node * &pNode);
	bool CBuildTree::newGetHead(Node*&pNode);
	bool newGetHead(Node*&pNode,vector<Node*>&pos_vec);
	bool treeToSegString(Node*&pNode,string&senOut);
	virtual ~CBuildTree();

private:
	void SimplifyPos(Node * pNode);
	void DeleteNone_Sub(Node *pNode);
	Node* AddTail(CString ss, Node *pNode);
	void AddSon(CString S,Node *pNode);
};

