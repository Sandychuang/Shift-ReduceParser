#include "stdafx.h"  
#pragma once
#include"PerceptronModel.h"
//#include "WriteLog.h"
//IMPLEMENT_SERIAL(Weight, CObject, 1)  
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
PerceptronModel::PerceptronModel()
{
	trainingIterations=19;
	BeamSize=4;
	maxBeamSize=4;
}
PerceptronModel::~PerceptronModel()
{
}
void PerceptronModel::findHighestScoringTransitions(State&state,bool isLegal,int numTransitions,vector<string>constraints,priority_queue<Pair>& queue)
{
	list<string> features;		
    featureFactory.featurize(state,features);
	return findHighestScoringTransitions(state,features,isLegal,numTransitions,constraints,queue);

}
void PerceptronModel::findHighestScoringTransitions(State&state,list<string>&features,bool isLegal,int numTransitions,vector<string>constraints,priority_queue<Pair>& queue)
{
	float*scores=new float[IndexTransition.size()];
	///////////////////////////////////////////
	////////数组初始化
	///////////////////////////////////////////
	for(int i=0;i<IndexTransition.size();i++)
	{
			scores[i]=0.0;
	}
		
	auto itt=features.begin();
	
	for(;itt!=features.end()&&featureWeights.size()>0;itt++)
	{
		auto igg=featureWeights.find(*itt);
		if(igg!=featureWeights.end())
		{
		Weight* weight=(*igg).second;
		if(weight->size()==0)
		{
			continue;
		}
		weight->score(scores);
		}
		else{
			continue;
		}
		/*for(int i=0;i<IndexTransition.size();i++)
		{
			cout<<i<<":"<<scores[i]<<" ";
		}
		cout<<endl;*/
	}
	
	
	for(int i=0;i<IndexTransition.size();i++)
	{
		
		bool judeLegal=IndexTransition[i]->isLeagal(state,constraints);
		
		
		if(!isLegal||judeLegal)
		{
			Pair temppair;
			temppair.index=i;
			temppair.score=scores[i];
			queue.push(temppair);
			if(queue.size()>numTransitions&&queue.size()>1)
			{
				//cout<<queue.top().score<<endl;
				queue.pop();
			}
		}
	   
	}



	delete []scores;
}

void PerceptronModel::findHighestScoringTransitions(State&state,list<string>&features,bool isLegal,int numTransitions,vector<string>constraints,Pair& pair)
{
	float*scores=new float[IndexTransition.size()];
	///////////////////////////////////////////
	////////数组初始化
	///////////////////////////////////////////
	for(int i=0;i<IndexTransition.size();i++)
	{
			scores[i]=0.0;
	}
		
	auto itt=features.begin();
	for(;itt!=features.end()&&featureWeights.size()>0;itt++)
	{
		auto igg=featureWeights.find(*itt);
		if(igg!=featureWeights.end())
		{
		Weight* weight=(*igg).second;
		if(weight->size()==0)
		{
			continue;
		}
		weight->score(scores);
		}
		else{
			continue;
		}
		/*for(int i=0;i<IndexTransition.size();i++)
		{
			cout<<i<<":"<<scores[i]<<" ";
		}
		cout<<endl;*/
	}
	pair.index=0;
	pair.score=scores[0];
	for(int i=0;i<IndexTransition.size();i++)
	{
		bool judeLegal=IndexTransition[i]->isLeagal(state,constraints);
		if(!isLegal||judeLegal)
		{
			Pair temppair;
			temppair.index=i;
			temppair.score=scores[i];
			//queue.push(temppair);
			if(temppair.score>=pair.score)
			{
				//cout<<queue.top().score<<endl;
				pair.index=temppair.index;
				pair.score=temppair.score;
			}
		}
	}



	delete []scores;
}
 pair<int,int> PerceptronModel::trainTree(int index,vector<Node*>&binarizedTrees,vector<list<Transition*>>&transitionLists,list<Update>&Updates)
 {
	 int numCorrect=0;
	 int numWrong=0;
	 Node* tree=binarizedTrees[index];
	 //CBuildTree buildtree;
	 vector<string>constraints;
	 set<Node*> tempNewNode;
	 State state=initialStateFromGoldTagTree(tree);
	 /*auto itt=state.sentence.begin();   ////叶子节点的释放留到最后
	 for(;itt!=state.sentence.end();itt++)
      {
			tempNewNode.insert(*itt);
      }*/
	 list<Transition*> transition=transitionLists[index];
	 while(transition.size()>0)
	 {
		 list<string> features;
		 priority_queue<Pair> queue;
		 Transition* goldenTransition=transition.front();
		 int goldenNum=-1;
		 for(int i=0;i<IndexTransition.size();i++)
		 {
			 if(goldenTransition->tostring()==IndexTransition[i]->tostring())
			 {
				 goldenNum=i;
				 break;
			 }
		 }
		 
		 featureFactory.featurize(state,features);
		 findHighestScoringTransitions(state,features,true,1,constraints,queue);
		 int predictedNum=-1;
		 Transition* predictedTransition=NULL;		
		 if(queue.size()!=0)
		 {
			 predictedNum=queue.top().index;
		 }
		 else
		 { 
			 WriteLog::write_log("PerceptronModel:trainTree:queue.size=0:error:预测失败");
			 break;
			 
		 }
		 predictedTransition=IndexTransition[predictedNum];
		 if(goldenNum==predictedNum)
		 {
			 numCorrect++;
			 transition.pop_front();
			 goldenTransition->apply(state);
			 if(state.newNode!=NULL)tempNewNode.insert(state.newNode);
			
		 }
		 else
		 {
			 numWrong++;
			 Update tempUpdate(features,goldenNum,predictedNum,1.0f);
			 Updates.push_back(tempUpdate);
			
			 break;
		 }




	 }
	 //state.clear(tempNewNode);///size为1说明结束;训练说明
	 state.clear(tempNewNode);////对训练过程中新增加的节点内存释放处理；
	 cout<<"预测成功的个数"<<numCorrect<<"预测失败的个数"<<numWrong<<endl;
	 return make_pair(numCorrect,numWrong);
 }
 /////加入beamsearch之后的搜索方法；
  pair<int,int> PerceptronModel::trainTree_withBeam(int index,vector<Node*>&binarizedTrees,vector<list<Transition*>>&transitionLists,list<Update>&Updates)
 {
	 int numCorrect=0;
	 int numWrong=0;
	 Node* tree=binarizedTrees[index];
	 priority_queue<State> agent;
	 vector<string>constraints;
	 State goldState=initialStateFromGoldTagTree(tree);
	 set<Node*> tempNewNode;
	 //////////////////////////////////////////////////////////////////
	/* auto itt=goldState.sentence.begin();    叶子节点的释放留到最后
	 for(;itt!=goldState.sentence.end();itt++)
      {
			tempNewNode.insert(*itt);
      }*/
	/////////////////////////////////////////////////////////////////////
	
	 list<Transition*> transition=transitionLists[index];
	 agent.push(goldState);
	 int transitionCount=0;
	 const int size=BeamSize;
	 Pair* maxTransition=new Pair[BeamSize];
	 while(transition.size()>0)
	 {
		
		 priority_queue<State> queueState;
		 priority_queue<Pair> queue;
		 State highestScoringState;
		 State highestCurrentState;
		
		 Transition* goldenTransition=transition.front();
		 Transition* highestScoringTransitionFromGoldState=NULL;
		 float highestScoreFromGoldState = 0.0;
		 int goldenNum=-1;
		 for(int i=0;i<IndexTransition.size();i++)
		 {
			 if(goldenTransition->tostring()==IndexTransition[i]->tostring())
			 {
				 goldenNum=i;
				 break;
			 }
		 }
		 
		 while(!agent.empty())
		 {
			State currentState=agent.top();
			 list<string> features;
			featureFactory.featurize(currentState,features);
		    findHighestScoringTransitions(currentState,features,true,BeamSize,constraints,queue);
			int sizeOfBeam=queue.size();
			for(int i=0;i<sizeOfBeam;i++)
			{
				maxTransition[i]=queue.top();////利用数组转换一下顺序
				queue.pop();
			}
			//cout<<maxTransition[BeamSize-1].index<<endl;
			for(int j=sizeOfBeam-1;j>=0;j--)
		     {
				 
				 bool isGoldState= isTransitonEqual(goldState,currentState);
				 
				 State newState=agent.top();
				 //queue.
				 //int i=maxTransition[j].index;
				 IndexTransition[maxTransition[j].index]->apply(newState,maxTransition[j].score);////如果值传递不行，可以考虑采用指针传递；
				 //queueState.push(agent.top());
			   
				
				 queueState.push(newState);
				 if(newState.newNode!=NULL)tempNewNode.insert(newState.newNode);
				/* auto ikk=newState.newNode.begin();
				for(;ikk!=newState.newNode.end();ikk++)
				{
						tempNewNode.insert(*ikk);
				}*/
				 if(queueState.size()>BeamSize)
				 {
					 ////////////////////////////////////////////////////////// 用于新增节点的内存释放；
					/* auto icc=queueState.top().newNode.begin();
					for(;icc!=queueState.top().newNode.end();icc++)
					 {
						tempNewNode.insert(*icc);
					}*/
						//agent.pop();
					 ///////////////////////////////////////////////////////////
					 queueState.pop();
				 }
				 if (highestScoringState.sentence.size() ==0 || highestScoringState.score< newState.score) {
                     highestScoringState = newState;
                    highestCurrentState = currentState;
				 }
				 if (isGoldState &&(highestScoringTransitionFromGoldState == NULL || maxTransition[j].score > highestScoreFromGoldState)) 
				 {
					highestScoringTransitionFromGoldState =  IndexTransition[maxTransition[j].index];
					highestScoreFromGoldState =maxTransition[j].score;
				}

				
			 }
			///////////////////////////////////////////////////////用于新增节点的内存释放
			//auto icc=agent.top().newNode.begin();
		 //  for(;icc!=agent.top().newNode.end();icc++)
   //        {
			//tempNewNode.insert(*icc);
   //         }
			 agent.pop();
			//////////////////////////////////////////////////////////
		 }

		 State newGoldState=goldState;
		 goldenTransition->apply(newGoldState, 0.0);
		  if(newGoldState.newNode!=NULL)tempNewNode.insert(newGoldState.newNode);
		/* auto igg=newGoldState.newNode.begin();
	    for(;igg!=newGoldState.newNode.end();igg++)
        {
			tempNewNode.insert(*igg);
        }*/
		 if(!isTransitonEqual(newGoldState,highestScoringState))
		 {
			 numWrong++;
			 list<string> goldFeatures;
			 featureFactory.featurize(goldState,goldFeatures);
			 list<string> lastFeatures;
			 featureFactory.featurize(highestCurrentState,lastFeatures);
			 Update tempUpdate1(goldFeatures,goldenNum,-1,1.0f);
			 Transition* lastTransition=NULL;
			 if(highestScoringState.transition.size()>0)
			 {
				 auto first=highestScoringState.transition.begin();
				 while(first!=highestScoringState.transition.end())
				 {
					 lastTransition=*first;
					 first++;
				 }
				 //lastTransition=*(igg--);
			 }
			 int lastTransitonNum=-1;
			 for(int i=0;i<IndexTransition.size()&&lastTransition!=NULL;i++)
		    {
			   if(lastTransition->tostring()==IndexTransition[i]->tostring())
			  {
				 lastTransitonNum=i;
				 break;
			  }
		    }
			 Update tempUpdate2(lastFeatures,-1,lastTransitonNum,1.0f);
			 Updates.push_back(tempUpdate1);
			 Updates.push_back(tempUpdate2);
			 ////明天继续
			 if(!findStateOnAgent(queueState,newGoldState)) 
			 {
				/* while(!queueState.empty())
				 {
		       auto imm=queueState.top().newNode.begin();
		       for(;imm!=queueState.top().newNode.end();imm++)
               {
			tempNewNode.insert(*imm);
             }
	         queueState.pop();
            }*/

				 
				 break;///最优目标不在beam的范围之内;
			 }
			 else
			 {
				transition.pop_front();///最优目标在beam范围之内；
			 }
		 }
		 else
		 {
			 numCorrect++;
			 transition.pop_front();
		 }
		 goldState=newGoldState;
		 agent=queueState;
	 }
	/////////////////////////////////////////////////////用于新增节点的内存释放;
	/*while(!agent.empty())
	{
		auto igg=agent.top().newNode.begin();
		for(;igg!=agent.top().newNode.end();igg++)
        {
			tempNewNode.insert(*igg);
        }
	agent.pop();
    }
*/
	
	 ////////////////////////////////////////////////////////////
	goldState.clear(tempNewNode);
	delete maxTransition; ///释放内存；
	//goldState.clear(goldState.sentence);
	cout<<"预测成功的个数"<<numCorrect<<"预测失败的个数"<<numWrong<<endl;
	return make_pair(numCorrect,numWrong);

 }

  bool PerceptronModel:: isTransitonEqual(State& one,State& other)
	{
		if(one.transition.size()!=other.transition.size())return false;
		auto icc=one.transition.begin();
		auto igg=other.transition.begin();
		
		for(;icc!=one.transition.end()&&igg!=other.transition.end();icc++,igg++)
		{
			
			if(strcmp((*igg)->tostring().c_str(),(*icc)->tostring().c_str())!=0)
			{
				return false;
			}
		}



		return true;

	}
  bool PerceptronModel:: findStateOnAgent( priority_queue<State> queueState,State& other)
  {
	  if(queueState.size()==0)return false;
	  while(!queueState.empty())
	  {
		  if(!isTransitonEqual(queueState.top(), other))
		  {
			  return false;
		  }
		  queueState.pop();
	  }
	  return true;
  }
 void PerceptronModel::trainModel(vector<Node*>&binarizedTrees,vector<list<Transition*>>&transitionLists)
 {
	 list<int> inices;
	 for(int i=0;i<binarizedTrees.size();i++)
	 {
		 inices.push_back(i);
	 }
	 list<Update> Updates;
	 pair<int,int> tempInfor;
	 for(int i=0;i<trainingIterations;i++)
	 {
		 for(int j=0;j<inices.size();j++)
		 {
			trainTree(j,binarizedTrees, transitionLists,Updates);

		 
		 auto itt=Updates.begin();
		 for(;itt!=Updates.end();itt++)
		 {
			 auto icc=(*itt).features.begin();
			 for(;icc!=(*itt).features.end();icc++)
			 {
				 auto igg=featureWeights.find((*icc));
				 if(igg!=featureWeights.end())
				 {
					 (*igg).second->updateWeight(itt->goldTransition,itt->delta);
					 (*igg).second->updateWeight(itt->predictedTransition,-1*(itt->delta));
				 }
				 else
				 {
					 Weight *weight=new Weight();
					 weight->updateWeight(itt->goldTransition,itt->delta);
					 weight->updateWeight(itt->predictedTransition,-1*(itt->delta));
					 featureWeights.insert(make_pair(*icc,weight));
				 }
				

			 }
		 }
		 Updates.clear();
		auto ipp=featureWeights.begin();
	    for(;ipp!=featureWeights.end();ipp++)
	    {
		  ipp->second->sumWeight();
	    }


	}
	 }
	 ////平滑求平均值,是否对最终的结果有意义。
	int nums=trainingIterations*inices.size();
	 auto ihh=featureWeights.begin();
	 for(;ihh!=featureWeights.end();ihh++)
	 {
		 ihh->second->averaging(nums);
	 }
 }

 void PerceptronModel::initialStateFromTaggedSentence(vector<Node*>& sentence,Node* pNode)
{
	if(pNode==NULL)return;
	if(pNode->IsLeafNode())
	{
		sentence.push_back(pNode);
		return;
	}
	if(pNode->IsPreTerminal())
	{
		
	  Node* tempLeftNode=pNode->m_pLeftChild;
	  if(tempLeftNode!=NULL)
	  {
		initialStateFromTaggedSentence(sentence,tempLeftNode);
	
	   }
	 return;
	}
	Node* tempLeftNode=pNode->m_pLeftChild;
	if(tempLeftNode!=NULL)
	{
		initialStateFromTaggedSentence(sentence,tempLeftNode);
	
	}
	Node* tempRightNode=pNode->m_pRightChild;
	if(tempRightNode!=NULL)
	{
		initialStateFromTaggedSentence(sentence,tempRightNode);
	}
	
}
 bool PerceptronModel::initialStateFromTaggedSentence(vector<Node*>& sentence,string senin)///完成测试；
 {
	 int length=0;//记录句子叶子节点的个数，即句子节点的位置。
	 int begin=0;
	 while(senin.find(" ",begin)!=string::npos)
	 {
		 int pos1=senin.find(" ",begin);
		 int pos2=senin.find("/",begin);
		 if(pos1<pos2)
		 {
				begin=pos1+1;
				continue;
		 }
			 string temp=senin.substr(0,pos1);
			 string word=temp.substr(0,pos2);
			 string tag=temp.substr(pos2+1);
			 Node* tempNode=new Node();
			 tempNode->m_psPOS=new char[tag.size()+1];
			 strcpy(tempNode->m_psPOS,tag.c_str());
			 tempNode->m_psWord=new char[word.size()+1];
			 strcpy(tempNode->m_psWord,word.c_str());
			 if(tag.find("w")!=string::npos)
			 {
				 tempNode->isSeparator=true;
			 }
			  tempNode->pos=length;
			  length++;
			 sentence.push_back(tempNode);
			 //senin=se
	
 
		/* else
		 {
			  WriteLog::write_log("PerceptronModel:parser:已标注词性的句子不符合规范");
			  senin="";
			  return false;
		 }*/
		senin= senin.substr(pos1+1);
		begin=0;
	 }
	 //if(senin.find("/")!=string::npos)
		// {
		//	 int pos2=senin.find("/");
		//	 string word=senin.substr(0,pos2);
		//	 string tag=senin.substr(pos2+1);
		//	 Node* tempNode=new Node();
		//	 tempNode->m_psPOS=new char[tag.size()+1];
		//	 strcpy(tempNode->m_psPOS,tag.c_str());
		//	 tempNode->m_psWord=new char[word.size()+1];
		//	 strcpy(tempNode->m_psWord,word.c_str());
		//	 if(tag.find("w")!=string::npos)
		//	 {
		//		 tempNode->isSeparator=true; //根据节点的词性信息判断节点是否是标点符号。
		//	 }
		//	  tempNode->pos=length;//记录节点的位置
		//	  length++;//
		//	 sentence.push_back(tempNode);
		//	 //senin=se
		// }
		// else
		// {
		//	  WriteLog::write_log("PerceptronModel:parser:已标注词性的句子不符合规范");
		//	  return false;
		// }
	 //
	 return true;

 }
State PerceptronModel::initialStateFromGoldTagTree(Node* pNode)
{ 
	State state;
    initialStateFromTaggedSentence(state.sentence,pNode);
	return state;

}
bool PerceptronModel::outPutModel(const char* KnownStatesFile,const char* rootstatesfile,const char* rootonlystatesfile,const char* indextranitonfile,const char* featureweightsfile)
{
	std::ofstream ofs_KSF(KnownStatesFile);
	if(!ofs_KSF)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:打开KnownStatesFile失败");
	  return false;
	}
	try
	{
	auto icc=KownStates.begin();
	for(;icc!=KownStates.end();icc++)
	{
		ofs_KSF<<*icc<<endl;
	}
	}
	catch(...)
	{
		WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:写入KnownStatesFile异常");
		return false;
	}
	ofs_KSF.close();
	std::ofstream ofs_RSF(rootstatesfile);
	if(!ofs_RSF)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:打开rootstatesfile失败");
	  return false;
	}
	try
	{
	auto igg=RootStates.begin();
	for(;igg!=RootStates.end();igg++)
	{
		ofs_RSF<<*igg<<endl;
	}
	}catch(...)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:写入rootstatesfile异常");
	  return false;
	}
	ofs_RSF.close();
	std::ofstream ofs_ROSF(rootonlystatesfile);
	if(!ofs_ROSF)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:打开rootonlystatesfile失败");
	  return false;
	}
	try
	{
	auto itt=RootOnlyStates.begin();
	for(;itt!=RootOnlyStates.end();itt++)
	{
		ofs_ROSF<<*itt<<endl;
	}
	}
	catch(...)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:写入rootonlystatesfile异常");
	  return false;
	}
	ofs_ROSF.close();
	std::ofstream ofs_FWF(featureweightsfile);  
	if(!ofs_FWF)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:打开featureweightsfile失败");
	  return false;
	}
	try
	{
    boost::archive::text_oarchive oa(ofs_FWF);
	auto ipp=featureWeights.begin();
	for(;ipp!=featureWeights.end();ipp++)
	{
	   oa<<(*ipp).first<<(*ipp).second;
		
	}
	
	}
	catch(...)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:写入featureweightsfile异常");
	  return false;
	}
	//ofs_FWF<<"#end#";
	ofs_FWF.close();
	std::ofstream ofs_ITF(indextranitonfile);
	if(!ofs_ITF)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:打开indextranitonfile失败");
	  return false;
	}
	try
	{
    boost::archive::text_oarchive oa(ofs_ITF);
	auto iaa=IndexTransition.begin();
	for(;iaa!=IndexTransition.end();iaa++)
	{
		 Transition* pa=(*iaa);
		oa << pa;  
	}
	}
	catch(...)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:outPutModel:error:写入indextranitonfile异常");
	  return false;
	}
   ofs_ITF.close();  
   return true;

	
}
bool PerceptronModel::inPutModel(const char* KnownStatesFile,const char* rootstatesfile,const char* rootonlystatesfile,const char* indextranitonfile,const char* featureweightsfile)
{
	std::ifstream ifs_KSF(KnownStatesFile);
	if(!ifs_KSF)
	{
	   WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:打开KnownStatesFile失败");
	  return false;
	}
	string temp_KSF="";
	try{
	while(getline(ifs_KSF,temp_KSF)!= '\0')
	{
		KownStates.insert(temp_KSF);
	}
	}
	catch(...)
	{
		 WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:读取KnownStatesFile异常");
	  return false;
	}
	ifs_KSF.close();

	std::ifstream ifs_RSF(rootstatesfile);
	if(!ifs_RSF)
	{
	   WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:打开rootstatesfile失败");
	  return false;
	}
	string temp_RSF="";
	try
	{
	while(getline(ifs_RSF,temp_RSF)!= '\0')
	{
		RootStates.insert(temp_RSF);
	}
	}
	catch(...)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:读取rootstatesfile异常");
	  return false;
	}
	ifs_RSF.close();
	std::ifstream ifs_ROSF(rootonlystatesfile);
	if(!ifs_ROSF)
	{
	   WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:打开rootonlystatesfile失败");
	  return false;
	}
	string temp_ROSF="";
	try{

	while(getline(ifs_ROSF,temp_ROSF)!= '\0')
	{
		RootOnlyStates.insert(temp_ROSF);
	}
	}
	catch(...)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:读取rootonlystatesfile异常");
	  return false;
	}
	ifs_ROSF.close();
	std::ifstream ifs_FWT(featureweightsfile);
	if(!ifs_FWT)
	{
	  WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:打开featureweightsfile失败");
	  return false;
	}
    boost::archive::text_iarchive ia_FWT(ifs_FWT);
	
	/*string temp_FWT="";
	Weight* pz=NULL;*/
	int i=0;
	try{
	while(!ifs_FWT.eof())
	{
		i++;
		if(i==596)
		{
		int cc=0;
		}
		///*string temp;
		//Weight* pz;*/
		/*if(ifs_FWT.fail())
		{
			break;
		}*/
		string temp_FWT="";
	    Weight* pz=NULL;
		ia_FWT>>temp_FWT;
		if(temp_FWT.size()>200)
		{
			break;
		}
		ia_FWT>> pz;
		featureWeights.insert(make_pair(temp_FWT,pz));	
		/*cout<<i<<endl;*/
	}
	}
	catch(...)
	{
		WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:读取featureweightsfile异常");
	 // return false;
	}
    ifs_FWT.close(); 
	std::ifstream ifs_ITF(indextranitonfile); 
	if(!ifs_ITF)
	{
		 WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:打开indextranitonfile失败");
	     //return false;
	}
    boost::archive::text_iarchive ia_ITF(ifs_ITF);
	try{
	while(!ifs_ITF.eof())
	{
		Transition* pa2;
		ia_ITF >> pa2;
		IndexTransition.push_back(pa2);
		
	}
	}
	catch(...)
	{
		 WriteLog::write_log("PerceptronModel:trainTree:inPutModel:error:读取indextranitonfile异常");
	     //return false;
	}
    ifs_ITF.close();  
    return true;  

}
//////////////////////////////
//句法分析的时候的内存卸载
///////////////////////////////
bool PerceptronModel::unload()
{
	auto ioo=featureWeights.begin();
	for(;ioo!=featureWeights.end();ioo++)
	{
		delete ioo->second;
	}
	auto ijj=IndexTransition.begin();
	for(;ijj!=IndexTransition.end();ijj++)
	{
		delete (*ijj);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//利用beamSearch的宽度，增加准确率和召回率
//////////////////////////////////////////////////////////////////////////
bool PerceptronModel::parser(const char* senin,char* senout,int &senoutlen)
{
	State state;
	set<Node*> tempNewNode;
	if(!initialStateFromTaggedSentence(state.sentence,senin))
	{
		WriteLog::write_log("PerceptronModel:parser:initialStateFromTaggedSentence:error");
		return false;
	}
	 priority_queue<State> beamState;
	 ///ShiftTransition tempshift; 
	 IndexTransition[IndexTransition.size()-1]->apply(state,0.0);
	 auto itt=state.sentence.begin();   /// 叶子节点的释放留到最后
	 for(;itt!=state.sentence.end();itt++)
      {
			tempNewNode.insert(*itt);
      }
	 beamState.push(state);
	 vector<string>constraints;
	 BeamSize=4;
	 Pair* maxTransition=new Pair[BeamSize];
	 // State highestScoringState;	
	 while(beamState.size()>0)
	 {
		
		  priority_queue<State> newBeamState;
		 
		 priority_queue<State> oldBeamState=beamState;
		/* while(!beamState.empty()) ///清空beamstate;
		 {
			 beamState.top().clear();
			 beamState.pop();
		 }*/
		 priority_queue<Pair> queue;
		 State highestScoringState;	
		 
		 while(!oldBeamState.empty())
			{
			 State currentState=oldBeamState.top();
			 list<string> features;
			featureFactory.featurize(currentState,features);
		    findHighestScoringTransitions(currentState,features,true,maxBeamSize,constraints,queue);
			BeamSize=queue.size();
			for(int i=0;i<BeamSize;i++)
			{
				maxTransition[i]=queue.top();////利用数组转换一下顺序
				queue.pop();
			}
			//cout<<maxTransition[BeamSize-1].index<<endl;
			for(int j=BeamSize-1;j>=0;j--)
		     {
				 State newState=oldBeamState.top();
				 IndexTransition[maxTransition[j].index]->apply(newState,maxTransition[j].score);////如果值传递不行，可以考虑采用指针传递；
				 if(newState.newNode!=NULL)tempNewNode.insert(newState.newNode);
				/*for(;ikk!=newState.newNode.end();ikk++)
				{
						tempNewNode.insert(*ikk);
				}*/
				 if (highestScoringState.sentence.size() ==0 || highestScoringState.score< newState.score) {
                     highestScoringState = newState;
                   }
				  newBeamState.push(newState);
				 if(newBeamState.size()>maxBeamSize)
				 {
					 /*auto igg=newBeamState.top().newNode.begin();   /// 叶子节点的释放留到最后
					for(;igg!=newBeamState.top().newNode.end();igg++)
					 {
							tempNewNode.insert(*igg);
					 }
					 */
					 newBeamState.pop();
				 }

				
			 }
			  
			 /*  auto icc=oldBeamState.top().newNode.begin();   /// 叶子节点的释放留到最后
			   for(;icc!=oldBeamState.top().newNode.end();icc++)
			  {
				   tempNewNode.insert(*icc);
			  }*/
			  oldBeamState.pop();
		 }
	     beamState=newBeamState;
		 if (highestScoringState.sentence.size() ==0||( highestScoringState.isFinised()&&highestScoringState.stack.size()==1))
		  {
             break;
          }
	 }
		  if(beamState.empty())
		  {
			  return false;
		  }
		  else
		  {
			 State bestState;
			  int size=beamState.size();
			  for(int i=0;i<size-1;i++)
		  {
			 //beamState.top().clear();
			/* auto icc=beamState.top().newNode.begin();   /// 叶子节点的释放留到最后
			 for(;icc!=beamState.top().newNode.end();icc++)
			 {
				   tempNewNode.insert(*icc);
			 }*/
			 beamState.pop();
		  }
			  Node* finalTree=beamState.top().stack.top();
			  buildTree.deTansformTree(finalTree);
			  string tempsentout;
			  buildTree.treeToString(finalTree,tempsentout);
			  if(tempsentout.size()>senoutlen-1)
			  {
				  return false;
			  }
			  strcpy(senout,tempsentout.c_str());
			/* auto imm=beamState.top().newNode.begin();   /// 叶子节点的释放留到最后
			 for(;imm!=beamState.top().newNode.end();imm++)
			 {
				   tempNewNode.insert(*imm);
			 }
			 beamState.pop();*/
			 bestState.clear(tempNewNode);
			 delete[] maxTransition;
			 maxTransition=NULL;
			  ///二叉树转换为多叉树；
			  ///输出为字符串，
			  return true;

		  }

	 }
/////////////////////////////////////////////////////////////////////////////
///优化速度,每一步分析只保留最优的结果
/////////////////////////////////////////////////////////////////////////////

bool PerceptronModel::parser_s(const char* senin,char*& senout,int &senoutlen)
{
	//WriteLog::write_log(senin);
			State state;
			set<Node*> tempNewNode;
	try{
	if(!initialStateFromTaggedSentence(state.sentence,senin))
	{
			WriteLog::write_log("PerceptronModel:parser:initialStateFromTaggedSentence:error");
			senout=NULL;
			return false;
	}
			IndexTransition[IndexTransition.size()-1]->apply(state,0.0);
	auto itt=state.sentence.begin();   /// 叶子节点的释放留到最后
	for(;itt!=state.sentence.end();itt++)
    {
			tempNewNode.insert(*itt);
    }
			vector<string>constraints;
			priority_queue<Pair> queue;
			list<string> features;
	 while(!(state.isFinised()&&state.stack.size()==1))
	 {
		    
			featureFactory.featurize(state,features);
			findHighestScoringTransitions(state,features,true,1,constraints,queue);
			if(queue.size()!=0)
		    {
			 IndexTransition[queue.top().index]->apply(state,queue.top().score);
			 if(state.newNode!=NULL)tempNewNode.insert(state.newNode);
		    }
			else
			{
			 IndexTransition[IndexTransition.size()-1]->apply(state,0.0);
			}
			if(queue.size()!=0)queue.pop();
			features.clear();
	 }
		    Node* finalTree=state.stack.top();
			buildTree.deTansformTree(finalTree);
			string tempsentout="";
			buildTree.treeToString(finalTree,tempsentout);
			//tempsentout="不知道为啥会有 bug！！！！！";
			if(tempsentout.size()>senoutlen-1)
			{
			  state.clear(tempNewNode);
			  return false;
			}
			int pos=tempsentout.find("[");
			string temproot=tempsentout.substr(0,pos);
			if(temproot.find("S")==string::npos)
			{
				tempsentout="S"+tempsentout.substr(pos);
			}
			strcpy(senout,tempsentout.c_str());
			state.clear(tempNewNode);
	}catch(...)
	{
		state.clear(tempNewNode);
		return false;
	}
			//WriteLog::write_log(senout);
			return true;

}

	 


		



	
	
	
	
