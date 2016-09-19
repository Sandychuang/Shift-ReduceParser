#include<list>
#include<string>
#include "FeatureFactory.h"
#include<stack>
///////////////////////////////////////////////////////////////////////
// 
//          BasicFeatureFactory.h:    特征模板类
//			父类：  FeatureFactory
//          程序员:       xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////

class BasicFeatureFactory:public FeatureFactory
{
  public:
	  
	  
	  BasicFeatureFactory()
	  {}
	  ~BasicFeatureFactory(){}
	  //一元栈特征
	  void addUnaryStackFeatures(list<string>&features,Lable lable,string conFeature,string wordTagFeature,string tagFeature,string wordConFeature,string tagConFeature);
	  // 一元队列特征
	  void addUnaryQueueFeatures(list<string>&features,Lable lable,string wtFeature);
	  // 二元特征
	  void addBinaryFeature(list<string>&features,string featureType,Lable lable1,FeatureComponent feature1,Lable lable2,FeatureComponent feature2);
	  //二元特征
	  void addBinaryFeatures(list<string>&features,string name1,Lable lable1,FeatureComponent feature11,FeatureComponent feature12,string name2,Lable lable2,FeatureComponent feature21,FeatureComponent feature22);
	  //一元特征
	  void addUnaryFeature(list<string>&features,string featureType,Lable lable,FeatureComponent feature);
	  //三元特征
	  void addTrigramFeature(list<string>&features,string featureType,Lable lable1,FeatureComponent feature1,Lable lable2,FeatureComponent feature2,Lable lable3,FeatureComponent feature3);
	  //位置特征
	  void addPositionFeatures(list<string>&features,State&state);
	  //分割符特征
	  void addSeparatorFeatures(list<string>&features, string name1, Lable label1, string name2, Lable label2, string separatorBetween, int countBetween);
	  //特征提取
	  void featurize(State&state,list<string>&features);

  public:

};