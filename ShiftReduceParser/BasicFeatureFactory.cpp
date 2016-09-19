#include "stdafx.h" 
#include <sstream>
#include "BasicFeatureFactory.h"

void BasicFeatureFactory::addUnaryStackFeatures(list<string>&features,Lable lable,string conFeature,string wordTagFeature,string tagFeature,string wordConFeature,string tagConFeature)
{
	if(lable.taglabel==""&&lable.wordlabel==""&&lable.conlabel=="")
	{
		features.push_back(conFeature+"null");
		return;
	}
	features.push_back(conFeature+lable.conlabel);
	features.push_back(wordTagFeature+lable.wordlabel+"-"+lable.taglabel);
	features.push_back(tagFeature+lable.taglabel);
	features.push_back(wordConFeature+lable.wordlabel+"-"+lable.conlabel);
	features.push_back(tagConFeature+lable.taglabel+"-"+lable.conlabel);
}
void BasicFeatureFactory::addUnaryQueueFeatures(list<string>&features,Lable lable,string wtFeature)
{
	if(lable.wordlabel==""&&lable.conlabel=="")
	{
		features.push_back(wtFeature+"null");
		return;
	}
	features.push_back(wtFeature+lable.wordlabel+"-"+lable.taglabel);
}
void BasicFeatureFactory::addBinaryFeature(list<string>&features,string featureType,Lable lable1,FeatureComponent feature1,Lable lable2,FeatureComponent feature2)
{
	string value1=getFeatureFromLabel(lable1,feature1);
	string value2=getFeatureFromLabel(lable2,feature2);
	features.push_back(featureType+value1+"-"+value2);

}
void BasicFeatureFactory::addBinaryFeatures(list<string>&features,string name1,Lable lable1,FeatureComponent feature11,FeatureComponent feature12,string name2,Lable lable2,FeatureComponent feature21,FeatureComponent feature22)
{
	if(lable1.conlabel==""&&lable1.taglabel==""&&lable1.wordlabel=="")
	{
		if(lable2.conlabel==""&&lable2.taglabel==""&&lable2.wordlabel=="")
		{
			features.push_back(name1+"n"+name2+"n");
		}
		else
		{
			addUnaryFeature(features,name1+"n"+name2+FeatureComponentTostring(feature21),lable2,feature21);
			addUnaryFeature(features,name1+"n"+name2+FeatureComponentTostring(feature21),lable2,feature22);
		}

	}
	else{
		if(lable2.conlabel==""&&lable2.taglabel==""&&lable2.wordlabel=="")
		{
			addUnaryFeature(features,name1+FeatureComponentTostring(feature11)+"n"+name2,lable1,feature11);
			addUnaryFeature(features,name1+FeatureComponentTostring(feature11)+"n"+name2,lable1,feature12);
		}
		else
		{
			addBinaryFeature(features,name1+FeatureComponentTostring(feature11)+"n"+name2+FeatureComponentTostring(feature21),lable1,feature11,lable2,feature21);
			addBinaryFeature(features,name1+FeatureComponentTostring(feature11)+"n"+name2+FeatureComponentTostring(feature21),lable1,feature11,lable2,feature22);
			addBinaryFeature(features,name1+FeatureComponentTostring(feature11)+"n"+name2+FeatureComponentTostring(feature21),lable1,feature12,lable2,feature21);
			addBinaryFeature(features,name1+FeatureComponentTostring(feature11)+"n"+name2+FeatureComponentTostring(feature21),lable1,feature12,lable2,feature22);
		}
	}

}
void BasicFeatureFactory::addTrigramFeature(list<string>&features,string featureType,Lable lable1,FeatureComponent feature1,Lable lable2,FeatureComponent feature2,Lable lable3,FeatureComponent feature3)
{
	string value1=getFeatureFromLabel(lable1,feature1);
	string value2=getFeatureFromLabel(lable2,feature2);
	string value3=getFeatureFromLabel(lable3,feature3);
	features.push_back(featureType+value1+"-"+value2+"-"+value3);
}
void BasicFeatureFactory::addUnaryFeature(list<string>&features,string featureType,Lable lable,FeatureComponent feature)
{
	string value=getFeatureFromLabel(lable,feature);
	features.push_back(featureType+value);
}
void BasicFeatureFactory::addPositionFeatures(list<string>&features, State&state) {
    if (state.tokenPosition >= state.sentence.size()) {
      features.push_back("QUEUE_FINISHED");
    }
    if (state.tokenPosition >= state.sentence.size() && state.stack.size() == 1) {
      features.push_back("QUEUE_FINISHED_STACK_SINGLETON");
    }
  }
void BasicFeatureFactory::addSeparatorFeatures(list<string>&features, string name1, Lable label1, string name2, Lable label2, string separatorBetween, int countBetween)
{
	if ((label1.taglabel==""&&label1.wordlabel==""&&label1.conlabel=="") || (label2.taglabel==""&&label2.wordlabel==""&&label2.conlabel=="")) {
      return;
    }

    // 0 separators is captured by the countBetween features
	 stringstream tempS;
	 tempS<<countBetween; 
     string countBetweenStr = tempS.str();///int型转换为string类型
    if (separatorBetween !="") {
      string separatorBetweenName = "Sepb" + name1 + name2 + "-" + separatorBetween + "-";
      addUnaryFeature(features, name1 + "w" + separatorBetweenName, label1, HEADWORD);
      addBinaryFeature(features, name1 + "wc" + separatorBetweenName, label1, HEADWORD, label1, VALUE);
      addUnaryFeature(features, name2 + "w" + separatorBetweenName, label2, HEADWORD);
      addBinaryFeature(features, name2 + "wc" + separatorBetweenName, label2, HEADWORD, label2, VALUE);
      addBinaryFeature(features, name1 + "c" + name2 + "c" + separatorBetweenName, label1, VALUE, label2, VALUE);
    }

    string countBetweenName = "Sepb" + name1 + name2 + "-" + countBetweenStr + "-";
    addUnaryFeature(features, name1 + "w" + countBetweenName, label1, HEADWORD);
    addBinaryFeature(features, name1 + "wc" + countBetweenName, label1, HEADWORD, label1, VALUE);
    addUnaryFeature(features, name2 + "w" + countBetweenName, label2, HEADWORD);
    addBinaryFeature(features, name2 + "wc" + countBetweenName, label2, HEADWORD, label2,VALUE);
    addBinaryFeature(features, name1 + "c" + name2 + "c" + countBetweenName, label1, VALUE, label2, VALUE);
}
////////////特征化分词中的某一个状态
void BasicFeatureFactory::featurize(State&state,list<string>&features)
{
	stack<Node*>s_stack=state.stack;
	//vector<Node*> sentence=state.sentence;
	int tokenPosition=state.tokenPosition;
	Transition transition[2];
	Lable S0Label=getStackLabel(s_stack,0,0,transition);
	Lable S1Label=getStackLabel(s_stack,1,0,transition);
	Lable S2Label=getStackLabel(s_stack,2,0,transition);
	Lable S3Label=getStackLabel(s_stack,3,0,transition);
	transition[0]=LEFT;
	Lable S0LLabel=getStackLabel(s_stack,0,1,transition);
	transition[0]=RIGHT;
	Lable S0RLabel=getStackLabel(s_stack,0,1,transition);
	transition[0]=UNARY;
	Lable S0ULabel=getStackLabel(s_stack,0,1,transition);
	transition[0]=LEFT;
	transition[1]=LEFT;
	Lable S0LLLabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=LEFT;
	transition[1]=RIGHT;
	Lable S0LRLabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=LEFT;
	transition[1]=UNARY;
	Lable S0LULabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=RIGHT;
	transition[1]=RIGHT;
	Lable S0RRLabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=RIGHT;
	transition[1]=LEFT;
	Lable S0RLLabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=RIGHT;
	transition[1]=UNARY;
	Lable S0RULabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=UNARY;
	transition[1]=RIGHT;
	Lable S0URLabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=UNARY;
	transition[1]=LEFT;
	Lable S0ULLabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=UNARY;
	transition[1]=UNARY;
	Lable S0UULabel=getStackLabel(s_stack,0,2,transition);
	transition[0]=LEFT;
	Lable S1LLabel=getStackLabel(s_stack,1,1,transition);
	transition[0]=RIGHT;
	Lable S1RLabel=getStackLabel(s_stack,1,1,transition);
	transition[0]=UNARY;
	Lable S1ULabel=getStackLabel(s_stack,1,1,transition);
	Lable q0Label=getQueueLabel(state.sentence,tokenPosition,0);
    Lable q1Label=getQueueLabel(state.sentence,tokenPosition,1);
	Lable q2Label=getQueueLabel(state.sentence,tokenPosition,2);
	Lable q3Label=getQueueLabel(state.sentence,tokenPosition,3);
    Lable qP1Label=getQueueLabel(state.sentence,tokenPosition,-1);
	Lable qP2Label=getQueueLabel(state.sentence,tokenPosition,-2);
	addUnaryStackFeatures(features,S0Label,"S0C-","S0WT-","S0T-","S0WC-","S0TC-");
	addUnaryStackFeatures(features,S1Label,"S1C-","S1WT-","S1T-","S1WC-","S1TC-");
	addUnaryStackFeatures(features,S2Label,"S2C-","S2WT-","S2T-","S2WC-","S2TC-");
	addUnaryStackFeatures(features,S3Label,"S3C-","S3WT-","S3T-","S3WC-","S3TC-");
	addUnaryStackFeatures(features,S0LLabel,"S0LC-","S0LWT-","S0LT-","S0LWC-","S0LTC-");
	addUnaryStackFeatures(features,S0RLabel,"S0RC-","S0RWT-","S0RT-","S0RWC-","S0RTC-");
	addUnaryStackFeatures(features,S0ULabel,"S0UC-","S0UWT-","S0UT-","S0UWC-","S0UTC-");
	addUnaryStackFeatures(features,S0LLLabel,"S0LLC-","S0LLWT-","S0LLT-","S0LLWC-","S0LLTC-");
	addUnaryStackFeatures(features,S0LRLabel,"S0LRC-","S0LRWT-","S0LRT-","S0LRWC-","S0LRTC-");
	addUnaryStackFeatures(features,S0LULabel,"S0LUC-","S0LUWT-","S0LUT-","S0LUWC-","S0LUTC-");
	addUnaryStackFeatures(features,S0RLLabel,"S0RLC-","S0RLWT-","S0RLT-","S0RLWC-","S0RLTC-");
	addUnaryStackFeatures(features,S0RRLabel,"S0RRC-","S0RRWT-","S0RRT-","S0RRWC-","S0RRTC-");
	addUnaryStackFeatures(features,S0RULabel,"S0RUC-","S0RUWT-","S0RUT-","S0RUWC-","S0RUTC-");
	addUnaryStackFeatures(features,S0LLLabel,"S0ULC-","S0ULWT-","S0ULT-","S0ULWC-","S0ULTC-");
	addUnaryStackFeatures(features,S0LRLabel,"S0URC-","S0URWT-","S0URT-","S0URWC-","S0URTC-");
	addUnaryStackFeatures(features,S0LULabel,"S0UUC-","S0UUWT-","S0UUT-","S0UUWC-","S0UUTC-");
	addUnaryStackFeatures(features,S1LLabel,"S1LC-","S1LWT-","S1LT-","S1LWC-","S1LTC-");
	addUnaryStackFeatures(features,S1RLabel,"S1RC-","S1RWT-","S1RT-","S1RWC-","S1RTC-");
	addUnaryStackFeatures(features,S1ULabel,"S1UC-","S1UWT-","S1UT-","S1UWC-","S1UTC-");
    addUnaryQueueFeatures(features,q0Label, "Q0WT-");
	addUnaryQueueFeatures(features,q1Label, "Q1WT-");
    addUnaryQueueFeatures(features,q2Label, "Q2WT-");
    addUnaryQueueFeatures(features, q3Label, "Q3WT-");
    addUnaryQueueFeatures(features, qP1Label, "QP1WT-");
    addUnaryQueueFeatures(features, qP2Label, "QP2WT-");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	Lable recentL0Label = getRecentDenpendent(s_stack, LEFT, 0);
    Lable recentR0Label = getRecentDenpendent(s_stack, RIGHT, 0);
    Lable recentL1Label = getRecentDenpendent(s_stack, LEFT, 1);
    Lable recentR1Label = getRecentDenpendent(s_stack,RIGHT, 1);
    addUnaryStackFeatures(features, recentL0Label, "recL0C-", "recL0WT-", "recL0T-", "recL0WC-", "recL0TC-");
    addUnaryStackFeatures(features, recentR0Label, "recR0C-", "recR0WT-", "recR0T-", "recR0WC-", "recR0TC-");
    addUnaryStackFeatures(features, recentL1Label, "recL1C-", "recL1WT-", "recL1T-", "recL1WC-", "recL1TC-");
    addUnaryStackFeatures(features, recentR1Label, "recR1C-", "recR1WT-", "recR1T-", "recR1WC-", "recR1TC-");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////rencentDenpebndentFeature//////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	addBinaryFeatures(features, "S0", S0Label, HEADWORD, VALUE, "S1", S1Label,HEADWORD, VALUE);
    addBinaryFeatures(features, "S0", S0Label, HEADWORD, VALUE, "Q0", q0Label, HEADWORD, HEADTAG);
    addBinaryFeatures(features, "S1", S1Label, HEADWORD, VALUE, "Q0", q0Label, HEADWORD, HEADTAG);
    addBinaryFeatures(features, "Q0", q0Label, HEADWORD, HEADTAG, "Q1", q1Label, HEADWORD, HEADTAG);
	addTrigramFeature(features, "S0cS1cS2c-", S0Label, VALUE, S1Label, VALUE, S2Label, VALUE);
    addTrigramFeature(features, "S0wS1cS2c-", S0Label, HEADWORD, S1Label, VALUE, S2Label, VALUE);
    addTrigramFeature(features, "S0cS1wS2c-", S0Label, VALUE, S1Label, HEADWORD, S2Label, VALUE);
    addTrigramFeature(features, "S0cS1cS2w-", S0Label, VALUE, S1Label, VALUE, S2Label, HEADWORD);

    addTrigramFeature(features, "S0cS1cQ0t-", S0Label, VALUE, S1Label, VALUE, q0Label, HEADTAG);
    addTrigramFeature(features, "S0wS1cQ0t-", S0Label, HEADWORD, S1Label, VALUE, q0Label, HEADTAG);
    addTrigramFeature(features, "S0cS1wQ0t-", S0Label, VALUE, S1Label, HEADWORD, q0Label, HEADTAG);
    addTrigramFeature(features, "S0cS1cQ0w-", S0Label, VALUE, S1Label, VALUE, q0Label, HEADWORD);
	addPositionFeatures(features, state);
	/////////////////////////////////////
	////加入分割符特征
	//if(s_stack.size()<2)return ;
	//Node* s0Node=s_stack.top();///栈顶节点
	//s_stack.pop();
	//Node* s1Node=s_stack.top();///栈顶第二个节点
	//Node* q0Node=NULL;
	//if(state.tokenPosition<state.sentence.size())q0Node=state.sentence[state.tokenPosition];
	//addSeparatorFeatures(features, "S0", S0Label, "S1", S1Label, state.getSeparator(s0Node, s1Node), state.getSeparatorCount(s0Node, s1Node)); //加入逗号特征

	//addSeparatorFeatures(features, "S0", S0Label, "Q0", q0Label, state.getSeparator(q0Node, s0Node), state.getSeparatorCount(q0Node, s0Node)); //加入逗号特征
	//////////////////////////////////////
}