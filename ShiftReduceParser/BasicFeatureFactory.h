#include<list>
#include<string>
#include "FeatureFactory.h"
#include<stack>
///////////////////////////////////////////////////////////////////////
// 
//          BasicFeatureFactory.h:    ����ģ����
//			���ࣺ  FeatureFactory
//          ����Ա:       xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////

class BasicFeatureFactory:public FeatureFactory
{
  public:
	  
	  
	  BasicFeatureFactory()
	  {}
	  ~BasicFeatureFactory(){}
	  //һԪջ����
	  void addUnaryStackFeatures(list<string>&features,Lable lable,string conFeature,string wordTagFeature,string tagFeature,string wordConFeature,string tagConFeature);
	  // һԪ��������
	  void addUnaryQueueFeatures(list<string>&features,Lable lable,string wtFeature);
	  // ��Ԫ����
	  void addBinaryFeature(list<string>&features,string featureType,Lable lable1,FeatureComponent feature1,Lable lable2,FeatureComponent feature2);
	  //��Ԫ����
	  void addBinaryFeatures(list<string>&features,string name1,Lable lable1,FeatureComponent feature11,FeatureComponent feature12,string name2,Lable lable2,FeatureComponent feature21,FeatureComponent feature22);
	  //һԪ����
	  void addUnaryFeature(list<string>&features,string featureType,Lable lable,FeatureComponent feature);
	  //��Ԫ����
	  void addTrigramFeature(list<string>&features,string featureType,Lable lable1,FeatureComponent feature1,Lable lable2,FeatureComponent feature2,Lable lable3,FeatureComponent feature3);
	  //λ������
	  void addPositionFeatures(list<string>&features,State&state);
	  //�ָ������
	  void addSeparatorFeatures(list<string>&features, string name1, Lable label1, string name2, Lable label2, string separatorBetween, int countBetween);
	  //������ȡ
	  void featurize(State&state,list<string>&features);

  public:

};