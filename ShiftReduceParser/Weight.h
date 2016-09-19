#include "stdafx.h"
#include<string>
#include <boost/archive/text_iarchive.hpp>  
#include <boost/archive/text_oarchive.hpp>  

///////////////////////////////////////////////////////////////////////
// 
//          Weight:				特征权值的存储和计算类
//			相关类：				
//          程序员:				xu sa	2016 01 07
//
//////////////////////////////////////////////////////////////////////




class Weight{

private:	
	       struct Infor{
			   int index;
			   float score;
			   Infor()
			   {
				   index=0;
				   score=0.0;
			   }
		   };
	
	        Infor* packed;
			Infor* packed_sum;
	        int length;

public :  Weight()
	{
		packed=NULL;
		packed_sum=NULL;
		length=0;
	}
	~Weight()
	{
		
		if(packed!=NULL)
	 {
        delete []packed;
		delete []packed_sum;
		packed=NULL;
		packed_sum=NULL;
	 }

	}
	void clear()
	{
		if(packed!=NULL)
	 {
        delete []packed;
		packed=NULL;
	 }
	 if(packed_sum!=NULL)
	 {
        delete []packed_sum;
		packed_sum=NULL;
	 }
	}
	int size()
	{
		if(packed==NULL)return 0;
		return length;
	}
	//计算权值
	void score(float*scores)
	{
		for(int i=0;i<size();i++)
		{
			int index=packed[i].index;
			float score=packed[i].score;
			scores[index]+=score;
		}
	}
	void sumWeight()
	{
		for(int i=0;i<length;i++)
		{
			
		 packed_sum[i].score=packed_sum[i].score+ packed[i].score;
		 //packed[i].score=packed_sum[i].score;
				
		}
	}
	void averaging(int nums)
	{
		for(int i=0;i<length;i++)
		{
			
		 packed_sum[i].score=packed_sum[i].score*(1.0/nums);
		 packed[i].score=packed_sum[i].score;
				
		}
			
		
	}

	void updateWeight(int index,float increment)
	{
		if(index<0)return;
		if(packed==NULL)
		{
			packed=new Infor[1];
			packed_sum=new Infor[1];
			packed[0]=pack(index,increment);
			//packed_sum[0]=packed[0];
			length=length+1;
			return;
		}
		for(int i=0;i<length;i++)
		{
			if(packed[i].index==index)
			{
				packed[i].score+=increment;
				//packed_sum[i].score=packed_sum[i].score+packed[i].score;
				return;
			}
			
		}
		Infor* newPacked=new Infor[length+1];
		Infor* newPacked_sum=new Infor[length+1];
		for(int i=0;i<length;i++)
		{
			newPacked[i]=packed[i];
			newPacked_sum[i]=packed_sum[i];
		}
		newPacked[length].index=index;
		newPacked[length].score=increment;
		//newPacked_sum[length].index=index;
		//newPacked_sum[length].score=increment;
		Infor* temp=packed;
		Infor* temp_sum=packed_sum;
		packed=newPacked;
		packed_sum=newPacked_sum;
		delete []temp;
		delete []temp_sum;
		temp=NULL;
		temp_sum=NULL;
		newPacked=NULL;
		newPacked_sum=NULL;
		length=length+1;

	}

	////序列化方法
	friend class boost::serialization::access;  
    template<class Archive>  
	void serialize(Archive & ar, const unsigned int version)  
    {  
          ar & length;
		   if(packed==NULL)
		  {
			  packed= new Infor[length];
		  }
		  for(int i=0;i<length;i++)
	     {
		 
		  ar & packed[i].index;
		  ar& packed[i].score;
	     }
		 
   }  
			
private:
	int unpackedIndex(int i)
	{
		if(i<=length)
		{
			Infor pack=packed[i];
			return pack.index;
		}
		else{

			////////////////添加日志记录
			return 0;
		}

	}
	float unpackedScore(int i)
	{
		if(i<=length)
		{
			Infor pack=packed[i];
			return pack.score;
		}
		else{
			
			////////////////添加日志记录
			return 0;
		}
	}
	Infor pack(int index,float score)
	{
		Infor temp;
		temp.index=index;
		temp.score=score;
		return temp;
	}

	
//public:   void Serialize(CArchive& ar)///weight函数的序列化，采用了MFC序列化方法。
//{
//
//
//	if (ar.IsStoring())  
//    {  
//        ar<< this->length;
//		 for(int i=0;i< this->length;i++)
//		{
//			ar<<this->packed[i].index<<this->packed[i].score;///读取内存空间的内容，记得释放掉。  
//	    }
//    }  
//    else 
//    {  
//        ar>> this->length;
//		this->packed= new Infor[length];///有内存空间的申请记得释放掉 
//		 for(int i=0;i<this->length;i++)
//		{
//			ar>>this->packed[i].index>>this->packed[i].score;  
//	    }
//    }  
//}

};
//#endif