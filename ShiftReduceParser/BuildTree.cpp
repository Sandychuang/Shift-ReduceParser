#include "stdafx.h"
//#include "sp.h"
//#pragma once
#include "BuildTree.h"
//#include "WriteLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBuildTree::CBuildTree()
{

}

CBuildTree::~CBuildTree()
{

}

void CBuildTree::build_tree(CString S, Node * & pNode)
{
	while(S.Replace("[ ","["))
		S.Replace("[ ","[");
	while(S.Replace(" [","["))
		S.Replace(" [","[");

	while(S.Replace(" ]","]"))
		S.Replace(" ]","]");
	while(S.Replace("] ","]"))
		S.Replace("] ","]");
	Node * ptemp;
	ptemp=new Node;

	AddSon(S,ptemp);
	pNode=ptemp->m_pLeftChild;
	pNode->m_pParent=NULL;
	delete ptemp;
	ptemp=NULL;
}

void CBuildTree::AddSon(CString S, Node *pNode)
{
	
	//实例：[S[ng2 狗][*VP[VO[vg 看到][ng3 骨头]][c 就][vg 垂涎]][wj 。]]	 
	int Length;
	while(!S.IsEmpty())
	{
		
		if (S.GetAt(0)=='[')
		S.Delete(0,1);
		else 
		{
			/*AfxMessageBox("句子格1式有错");
            AfxMessageBox(S);*/
			 WriteLog::write_log(" CBuildTree::AddSon:error:句子格式1出错");
			return;
		}
		int u_level=0,i=-1,j=0;//u_level记载括号层数，i读取字符的位置
		                      //j=0时该字符为"["或"]"，j=1时该字符为其它字符
		int num_child=0,fig=-1;
		Length=S.GetLength();
		S.TrimLeft();
		S.TrimRight();
		while(u_level!=1)
		{
			i++;
			if(u_level==0&&fig==0)
			{
				num_child=num_child+1;
			}
			if(i>=Length)
			{
				/*AfxMessageBox("句子格22式有错 ");
                AfxMessageBox(S);*/
				 WriteLog::write_log(" CBuildTree::AddSon:error:句子格式22出错");
                return;
			}
			if(S.GetAt(i)=='[')
			{
				u_level--;
				fig=0;
			}
			else if(S.GetAt(i)==']')
			{
				u_level++;
			}			
		}
		if(u_level==1&&fig==-1)
		{
				num_child=0;
		}
		CString OneSon=S.Left(i);
		j=OneSon.Find('[');
		if (j==-1)                   //leaf node
		{		 
			Node * temp;
			temp=AddTail(OneSon,pNode);
			temp->m_childNum=num_child;
		}
		else
		{
			Node * temp;
			CString nt=OneSon.Left(j);
			temp=AddTail(nt,pNode);
			temp->m_childNum=num_child;
			OneSon.Delete(0,j);
			//OneSon=OneSon.Left(OneSon.GetLength()-1);//[NP[PRON "? ?"]]
			AddSon(OneSon,temp);
		}
		S.Delete(0,i+1);
	}
}

Node* CBuildTree::AddTail(CString ss, Node *pNode)
{
	Node * pNodetemp;
	pNodetemp=new Node;
	int i;
	i=ss.Find(' ');          
	if (i==-1)               // non-terminal node
	{
		pNodetemp->m_psPOS=new char[ss.GetLength()+1];
		strcpy(pNodetemp->m_psPOS,ss);
	}
	else                 //leaf node
	{
		CString pos=ss.Left(i);	
		////if (pos.Find("ng")!=-1)//add by lsm 20140101 变ng1... ng10 为ng 
		////{
		////	pos = "ng";
		////}
		ss.Delete(0,i+1);
		CString word=ss;
		pNodetemp->m_psPOS=new char[pos.GetLength()+1];
		strcpy(pNodetemp->m_psPOS,pos);

		if(word.GetLength()>2 && word.GetAt(0)=='"')
		{
			word.Delete(0,1);					//////delete the left "
			word=word.Left(word.GetLength()-1); //delete the right " 
            word.Replace(" ","");               //delete the spaces between characters
		}
		pNodetemp->m_psWord=new char[word.GetLength()+1];
		strcpy(pNodetemp->m_psWord,word);
	}
	if(pNode->m_pLeftChild==NULL)
	{
		pNode->m_pLeftChild=pNodetemp;
		pNodetemp->m_pParent=pNode;
	}
	else
	{
	if(pNode->m_pRightChild==NULL)
	{
		pNode->m_pRightChild=pNodetemp;
		pNode->m_pLeftChild->m_pNextBrother=pNode->m_pRightChild;
		pNode->m_pRightChild->m_pPrevBrother=pNode->m_pLeftChild;
		pNodetemp->m_pParent=pNode;
	}
	else
	{
		pNode->m_pRightChild->m_pNextBrother=pNodetemp;
		pNodetemp->m_pPrevBrother=pNode->m_pRightChild;
		pNode->m_pRightChild=pNodetemp;
		pNodetemp->m_pParent=pNode;
	}
	}
	return pNodetemp;
}

// 删除以pNode为根的所有节点
void CBuildTree::delete_tree(Node * &pNode) 
{	
	if(pNode==NULL)
		return;
	Node *t2;
	Node * temp=pNode->m_pLeftChild;
	while (temp!=NULL)
	{
		t2=temp->m_pNextBrother;
		delete_tree(temp);
		temp=t2;
	}
	delete pNode;
	pNode=NULL;
}

void CBuildTree::PrevTravel(Node *pNode, CString &strSentence)
{
	if( pNode==NULL)
		return;

	if(pNode->IsLeafNode())
		strSentence=strSentence+'['+pNode->m_psPOS+' '+ pNode->m_psWord +']'; 	 
	else
	{
		strSentence=strSentence+'['+pNode->m_psPOS;
		 Node * temp=pNode->m_pLeftChild;
		 while(temp!=NULL)
		 {
			 PrevTravel(temp,strSentence);
			 temp=temp->m_pNextBrother;
		 }
		 strSentence=strSentence+']';
	}

}

void CBuildTree::DeleteNone(Node *pNode)
{
	if(pNode==NULL)
		return;
	TagNone(pNode);
	DeleteNone_Sub(pNode);
	SimplifyPos(pNode);

 
}

void CBuildTree::TagNone(Node *pNode)
{
	if(pNode==NULL)
		return;
	TagNone(pNode->m_pNextBrother);
	CString str(pNode->m_psPOS);
	if(str.Find("-NONE-")!=-1)
	{
		pNode->IsNone=true;
		return;
	}
	if(pNode->IsLeafNode())
		return;
	TagNone(pNode->m_pLeftChild);
	Node * temp;
	temp=pNode->m_pLeftChild;
	while(temp!=NULL)
	{
		if(!temp->IsNone)
		{
			pNode->IsNone=false;
			return;
		}
		temp=temp->m_pNextBrother;
	}
	pNode->IsNone=true;
		
}

void CBuildTree::DeleteNone_Sub(Node *pNode)
{
	if(pNode==NULL)
		return;
	if(pNode->IsNone)
	{
		if(pNode->m_pPrevBrother==NULL)
		{
			pNode->m_pParent->m_pLeftChild=pNode->m_pNextBrother;
		}
		else
			pNode->m_pPrevBrother->m_pNextBrother=pNode->m_pNextBrother;
		if(pNode->m_pNextBrother==NULL)
			pNode->m_pParent->m_pRightChild=pNode->m_pPrevBrother;
		else
			pNode->m_pNextBrother->m_pPrevBrother=pNode->m_pPrevBrother;
		DeleteNone_Sub(pNode->m_pNextBrother);
		delete_tree(pNode);
	}
	else
	{
		DeleteNone_Sub(pNode->m_pLeftChild);
		DeleteNone_Sub(pNode->m_pNextBrother);
	}
 
}

void CBuildTree::SimplifyPos(Node *pNode)
{
	if(pNode==NULL)
		return;
	CString str(pNode->m_psPOS);
	if(str.Find('-')>0)
	{
		str=str.Left(str.Find('-'));
		delete pNode->m_psPOS;
		pNode->m_psPOS=new char[str.GetLength()+1];
		strcpy(pNode->m_psPOS,str);
	}
	
	if(str.Find('=')>0)
	{
		str=str.Left(str.Find('='));
		delete pNode->m_psPOS;
		pNode->m_psPOS=new char[str.GetLength()+1];
		strcpy(pNode->m_psPOS,str);
	}
	SimplifyPos(pNode->m_pLeftChild);
	SimplifyPos(pNode->m_pNextBrother);


}
void CBuildTree::tansformTree(Node*&pNode)
{
	if(pNode==NULL)
	{
		return;
	}
	CString cat=pNode->m_psPOS;
	if(pNode->IsLeafNode())
	{
		return ;
	}
	if(pNode->binaryFig)return;
	if(pNode->IsPreTerminal())
	{
		return;
	}
	Node* tempChild=determinHeadNode(pNode);
	if(tempChild==NULL)return;
	////////////////////////////////////////////
	////头结点设置,核心词的传递
	////////////////////////////////////////////
	//pNode->m_pHead=tempChild;
	//pNode->headlabel=tempChild->headlabel;
	//pNode->headtag=tempChild->headtag;
	//pNode->headword=tempChild->headword;
	/////////////////////////////////////////////
	while(tempChild->m_pPrevBrother!=NULL)
	{
		tansformTree(tempChild);
		Node* templeftChild=tempChild->m_pPrevBrother;
		if(templeftChild->m_pPrevBrother==NULL&&tempChild->m_pNextBrother==NULL)
		{
			tansformTree(templeftChild);
			pNode->m_pRightChild=tempChild;
			pNode->m_pLeftChild->m_pNextBrother=tempChild;
			pNode->m_childNum=2;
			return;
		}
		else
		{
		tansformTree(templeftChild);
		Node* tempNewNode=createNewNode(templeftChild,tempChild,cat);
		Node*tempNode=templeftChild->m_pPrevBrother;
		tempNewNode->m_pPrevBrother=tempNode;
		tempNewNode->m_pNextBrother=tempChild->m_pNextBrother;
		if(tempNode!=NULL)tempNode->m_pNextBrother=tempNewNode;
		templeftChild->m_pPrevBrother=NULL;
		tempChild=tempNewNode;
		tempNewNode->m_pHead=pNode->m_pHead;
		tempNewNode->headlabel=pNode->headlabel;
		tempNewNode->headword=pNode->headword;
		tempNewNode->headtag=pNode->headtag;
		}
	}
    while(tempChild->m_pNextBrother!=NULL)
	{
		
		tansformTree(tempChild);
		Node* tempRightChild=tempChild->m_pNextBrother;
		if(tempChild->m_pPrevBrother==NULL&&tempRightChild->m_pNextBrother==NULL)
		{
			tansformTree(tempRightChild);
			pNode->m_pLeftChild=tempChild;
			pNode->m_pRightChild->m_pPrevBrother=tempChild;
			pNode->m_childNum=2;
			return;
		}
		tansformTree(tempRightChild);
		Node* tempNewNode=createNewNode(tempChild,tempRightChild,cat);
		Node*tempNode=tempRightChild->m_pNextBrother;
		tempNewNode->m_pNextBrother=tempNode;
		if(tempNode!=NULL)tempNode->m_pPrevBrother=tempNewNode;
		tempChild=tempNewNode;
		tempNewNode->m_pHead=pNode->m_pHead;
		tempNewNode->headlabel=pNode->headlabel;
		tempNewNode->headword=pNode->headword;
		tempNewNode->headtag=pNode->headtag;

	}    

}
Node* CBuildTree::createNewNode(Node*&pLeftNode,Node*&pRightNode,CString label)
{
 
  Node*tempNewNode=new Node();
  tempNewNode->m_pLeftChild=pLeftNode;
  tempNewNode->m_pRightChild=pRightNode;
  if(label.Find("*")==-1)
  {
	  label="@"+label;
  }
  else
  {
  label.Replace("*","@");
  }
  CString templabel=label;
  tempNewNode->m_psPOS=new char[templabel.GetLength()+1];
  strcpy(tempNewNode->m_psPOS,templabel);
  tempNewNode->m_childNum=2;
  tempNewNode->binaryFig=true;
  return tempNewNode;
  
}
void  CBuildTree::deTansformTree(Node*&pNode)
{
	if(pNode==NULL)return;
	if(pNode->IsPreTerminal())return;
	if(pNode->IsLeafNode())return;
	
	Node*nextBrother=NULL;
	Node* preBrother=NULL;
	if(pNode->m_pLeftChild!=NULL)
	{
	 string label(pNode->m_pLeftChild->m_psPOS);
	if(label.find("@")!=string::npos)
	{
	    Node* tempHead=pNode->m_pLeftChild;
		pNode->m_pLeftChild=NULL;
		//nextBrother=pNode->m_pLeftChild;//pNode->m_pLeftChild=NULL;
		if(pNode->m_pRightChild!=NULL)
	   {
	     string label(pNode->m_pRightChild->m_psPOS);		////不容许左右孩子节点都为临时节点。
		if(label.find("@")!=string::npos&&label.find("@S")==string::npos)
	    {
		     CString temp=pNode->m_pRightChild->m_psPOS;
			temp.Delete(0,1);
			delete pNode->m_pRightChild->m_psPOS;
			pNode->m_pRightChild->m_psPOS=NULL;
			pNode->m_pRightChild->m_psPOS=new char[temp.GetLength()+1];
			strcpy(pNode->m_pRightChild->m_psPOS,temp);
		}
		}
	  findLeftChild(tempHead,pNode,nextBrother);

	}
	else
	{
        nextBrother=pNode->m_pLeftChild;
		//nextBrother->m_pPrevBrother=pNode->m_pLeftChild;
		//findLeftChild(pNode->m_pLeftChild,pNode,nextBrother);
	}
	}
	if(pNode->m_pRightChild!=NULL)
	{
	   string label(pNode->m_pRightChild->m_psPOS);		
		if(label.find("@")!=string::npos)
	  {
	    Node* tempHead=pNode->m_pRightChild;
		pNode->m_pRightChild=NULL;
		//preBrother=pNode->m_pRightChild;
	    findRightChild(tempHead,pNode,preBrother);
	  }
	else
	{
	    preBrother=pNode->m_pRightChild;
		//preBrother->m_pNextBrother=pNode->m_pRightChild;
        //findRightChild(pNode->m_pRightChild,pNode,preBrother);
		}
	}
	if(nextBrother!=NULL&&preBrother!=NULL)
	
	{nextBrother->m_pNextBrother=preBrother;
	preBrother->m_pPrevBrother=nextBrother;
	}
	Node* tempNode=pNode->m_pLeftChild;
	while(tempNode!=NULL)
	{
		deTansformTree(tempNode);
		tempNode=tempNode->m_pNextBrother;
	}

	
}
void  CBuildTree::findLeftChild(Node*&pNode,Node*&root,Node*&tempChild)
{
	 string label(pNode->m_psPOS);
	if(label.find("@")!=string::npos)
	{
	    //root->m_pLeftChild=NULL;
		 Node* tempNode=NULL;
		 //Node* tempPreNode=NULL;
		findLeftChild(pNode->m_pLeftChild,root,tempChild);
		
		if(pNode->m_pLeftChild!=NULL)
	   {
		  if(root->m_pLeftChild==NULL)
		{
			root->m_pLeftChild=pNode->m_pLeftChild;
			tempChild=root->m_pLeftChild;
		}
		 else
		 {
		 tempNode=pNode->m_pLeftChild;
		 //Node* tempNode=tempChild;
		 tempChild->m_pNextBrother=tempNode;
		 tempNode->m_pPrevBrother=tempChild;
		 tempChild=tempNode;
		 }

		}
		findLeftChild(pNode->m_pRightChild,root,tempChild);
	    if(pNode->m_pRightChild!=NULL)
	   {
	      
		  /* findLeftChild(pNode->m_pRightChild,root,tempChild);*/
		   
		   if(root->m_pLeftChild==NULL)
		{
			root->m_pRightChild=pNode->m_pRightChild;
			tempChild=root->m_pRightChild;
		}
		 else
		 {
		   
		tempNode=pNode->m_pRightChild;
		tempChild->m_pNextBrother=tempNode;
		tempNode->m_pPrevBrother=tempChild;
		tempChild=tempNode;
		 }
	   }
		/*delete pNode;
		pNode=NULL;*/
		pNode->m_pLeftChild=NULL;
		pNode->m_pRightChild=NULL;
		pNode->m_pNextBrother=NULL;
		pNode->m_pPrevBrother=NULL;
	}
	return;
	/* if(pNode->m_pRightChild!=NULL)
	 {
	   tempChild=pNode->m_pRightChild;
	    Node* tempNode=tempChild;
		tempChild=tempNode->m_pNextBrother;
		tempNode->m_pPrevBrother=tempChild;
	 }
*/
}
void  CBuildTree::findRightChild(Node*&pNode,Node*&root,Node*&tempChild)
{
	 string label(pNode->m_psPOS);
	 if(label.find("@")!=string::npos)
	 {
		Node* tempNode=NULL;
	   findRightChild(pNode->m_pRightChild,root,tempChild);
	   if(pNode->m_pRightChild!=NULL)
	   {
		   //tempChild->=pNode->m_pRightChild;
		if(root->m_pRightChild==NULL)
		{
			root->m_pRightChild=pNode->m_pRightChild;
			tempChild=root->m_pRightChild;
		}
		else
		{
		tempNode=pNode->m_pRightChild;
		tempChild->m_pPrevBrother=tempNode;
		tempNode->m_pNextBrother=tempChild;
		tempChild=tempNode;
		}
		 
	   }
	    if(pNode->m_pLeftChild!=NULL)
	   {
	    if(root->m_pRightChild==NULL)
		{
			root->m_pRightChild=pNode->m_pLeftChild;
			tempChild=root->m_pRightChild;
		}
		else
		{
		tempNode=pNode->m_pLeftChild;
		tempChild->m_pPrevBrother=tempNode;
		tempNode->m_pNextBrother=tempChild;
		tempChild=tempNode;
		}
		}
     /*  delete pNode;
		pNode=NULL;*/
		pNode->m_pLeftChild=NULL;
		pNode->m_pRightChild=NULL;
		pNode->m_pNextBrother=NULL;
		pNode->m_pPrevBrother=NULL;
	 }
	 return ;
	/* if(pNode->m_pLeftChild!=NULL)
	 {
	    tempChild=pNode->m_pLeftChild;
		Node* tempNode=tempChild;
		tempChild=tempNode->m_pPrevBrother;
		tempNode->m_pNextBrother=tempChild;
	 }*/

}
bool CBuildTree::newGetHead(Node*&pNode)
{
	vector<Node*>temp;
	bool isOK=newGetHead(pNode,temp);
	temp.clear();
	return isOK;

}
bool CBuildTree::newGetHead(Node*&pNode,vector<Node*>&pos_vec)
{
	if(pNode==NULL)return false;
	int start=-1;
	if(pNode->IsLeafNode())
	{
		pos_vec.push_back(pNode);
	  pNode->headword=pNode->m_psWord;
	  pNode->headtag=pNode->m_psPOS;
	  pNode->headlabel=pNode->m_psPOS;
		return true;
	}
	else
	{
		pos_vec.push_back(pNode);
		start=pos_vec.size();
	}
	
	newGetHead(pNode->m_pLeftChild,pos_vec);
	Node* temp=NULL;
	if(pNode->m_pLeftChild!=NULL)
	{
		temp=pNode->m_pLeftChild->m_pNextBrother;
	}
	while(temp!=NULL)
	{
	   	newGetHead(temp,pos_vec);
		temp=temp->m_pNextBrother;
	}
	
	int end=pos_vec.size();
	vector<Node*> childPos;
	for(int i=start;i<end;++i)
	{
		childPos.push_back(pos_vec[i]);

	}
	for(int i=start;i<end;++i)
	{
		pos_vec.pop_back();

	}
	Node* headNode=NULL;
	for(int i=0;i<childPos.size();i++)
	{
		if(string(childPos[i]->m_psPOS).find("*")!=string::npos)///////////////带星号的头结点被优先选择的原则
		{
			CString temp=childPos[i]->m_psPOS;
			temp.Delete(0,1);
			delete childPos[i]->m_psPOS;
			childPos[i]->m_psPOS=NULL;
			childPos[i]->m_psPOS=new char[temp.GetLength()+1];
			strcpy(childPos[i]->m_psPOS,temp);
			headNode=childPos[i];
		}
	}
	if(headNode==NULL)
	{
		auto igg=nonTerminalInfo.find(string(pNode->m_psPOS));
		if(igg!=nonTerminalInfo.end())
		{
		for(int i=0;i<igg->second.size();i++)
		{
		headNode=traverLoate(childPos,igg->second[i],true);
		}
		}
	}
	if(headNode==NULL)headNode=pNode->m_pRightChild;
	if(headNode==NULL)headNode=pNode->m_pLeftChild;
	pNode->m_pHead=headNode;
	/*if(pNode->IsLeafNode())
	{
	  pNode->headword=headNode->m_psWord;
	  pNode->headtag=headNode->m_psPOS;
	  pNode->headlabel=headNode->m_psPOS;
	}
	else
	{*/
	pNode->headword=headNode->headword;
	pNode->headtag=headNode->headtag;
	pNode->headlabel=headNode->m_psPOS;
	/////////////////////////////测试头结点标注规则
	/*string tempPos(headNode->m_psPOS);
	tempPos="*"+tempPos;
	delete headNode->m_psPOS;
	headNode->m_psPOS=NULL;
	headNode->m_psPOS=new char[tempPos.size()+1];
	strcpy(headNode->m_psPOS,tempPos.c_str()); */////测试头结点标注规则；
	//headNode->m_psPOS
	/*}*/
	return true;
}
bool CBuildTree::treeToSegString(Node*&pNode,string&senOut)
{
	if(pNode==NULL)return false;
	if(pNode->IsLeafNode())
	{
		if(pNode->m_psWord!=NULL&&pNode->m_psPOS!=NULL)senOut=senOut+pNode->m_psWord+"/"+pNode->m_psPOS;
		return true;
	}
	else
	{
		//senOut=senOut+pNode->m_psPOS+"[";
	}
	
	treeToSegString(pNode->m_pLeftChild,senOut);
	Node* temp=NULL;
	if(pNode->m_pLeftChild!=NULL)
	{
		temp=pNode->m_pLeftChild->m_pNextBrother;
	}
	while(temp!=NULL)
	{
	    senOut=senOut+" ";
		treeToSegString(temp,senOut);
		temp=temp->m_pNextBrother;
	}
	
	//senOut=senOut+"]";
	return true;
}
bool CBuildTree::treeToString(Node*&pNode,string&senOut)
{
	if(pNode==NULL)return false;
	if(pNode->IsLeafNode())
	{
		if(pNode->m_psWord!=NULL&&pNode->m_psPOS!=NULL)senOut=senOut+pNode->m_psWord+"/"+pNode->m_psPOS;
		return true;
	}
	else
	{   string label(pNode->m_psPOS);
	    int pos=label.find("@");
	    if(pos!=string::npos)
		{
			label=label.substr(pos+1);
		}
		
		senOut=senOut+label+"[";
	}
	
	treeToString(pNode->m_pLeftChild,senOut);
	Node* temp=NULL;
	if(pNode->m_pLeftChild!=NULL)
	{
		temp=pNode->m_pLeftChild->m_pNextBrother;
	}
	while(temp!=NULL)
	{
	    senOut=senOut+" ";
		treeToString(temp,senOut);
		temp=temp->m_pNextBrother;
	}
	
	senOut=senOut+"]";
	return true;
}
Node* CBuildTree::determinHeadNode(Node*&pNode)
{
	if(pNode==NULL)
		return NULL;
	///*if(pNode->m_childNum<3)
	//{
	//	return NULL;
	//}*/
	//Node* tempNode=pNode->m_pLeftChild;
	//while(tempNode->m_pNextBrother!=NULL)
	//{
	//	if(tempNode!=NULL)
	//	{
	//		CString templable(tempNode->m_psPOS);
	//		if(templable.Find('*')>-1)
	//		{
	//			break;
	//		}
	//		else
	//		{
	//			tempNode=tempNode->m_pNextBrother;
	//		}
	//	}
	//}
	return pNode->m_pHead;
}
void CBuildTree::createTransitionSequence(list<Transition*> &tansition,Node*& pNode,set<string> rootstates,set<string> rootonlystates)
{
	if(pNode==NULL)return;
	if(pNode->IsLeafNode())
	{
		
		
		//Transition* tempshift=new ShiftTransition();
		tansition.push_back(new ShiftTransition());
		return;
	}
	if(pNode->IsPreTerminal())
	{
		
	  Node* tempLeftNode=pNode->m_pLeftChild;
	  if(tempLeftNode!=NULL)
	  {
		createTransitionSequence(tansition,tempLeftNode,rootstates,rootonlystates);
	
	  }
	  //CompoundUnaryTransition compoundunarytransition;
	  string label(pNode->m_psPOS);
	  bool isRoot=false;
	  auto itt=rootonlystates.find(label);
	  if(itt!=rootonlystates.begin())
	  {
		  isRoot=true;
	  }
	  if(label!="")
      tansition.push_back(new UnaryTransition(label,isRoot));
	  return;
	}
	Node* tempLeftNode=pNode->m_pLeftChild;
	if(tempLeftNode!=NULL)
	{
		createTransitionSequence(tansition,tempLeftNode,rootstates,rootonlystates);
	
	}
	Node* tempRightNode=pNode->m_pRightChild;
	if(tempRightNode!=NULL)
	{
		createTransitionSequence(tansition,tempRightNode,rootstates,rootonlystates);
	}
	if(tempLeftNode!=NULL&&tempRightNode==NULL)
	{
		//////
		tansition.push_back(new CompoundUnaryTransition());
	}
	if(tempLeftNode==NULL&&tempRightNode==NULL)
	{
		//////
		tansition.push_back(new CompoundUnaryTransition());
	}
	if(tempLeftNode!=NULL&&tempRightNode!=NULL)
	{
	//////加入二元节点边
	 string label(pNode->m_psPOS);
	
	 if(pNode->m_pHead==NULL)
	 {
		 WriteLog::write_log(" CBuildTree::createTransitionSequence:NULL:头结点为空");
	 }
	 if(pNode->m_pHead==tempLeftNode||pNode->m_pHead==tempLeftNode->m_pHead)
	 {
	   
		 
		 tansition.push_back(new BinaryTransition(label,BinaryTransition::Side::LEFT));
	 }
	 if(pNode->m_pHead==tempRightNode->m_pHead||pNode->m_pHead==tempRightNode)
	 {
	   tansition.push_back(new BinaryTransition(label,BinaryTransition::Side::RIGHT));
	 }

	}

}
//void CBuildTree::initialStateFromTaggedSentence(vector<Node*>& sentence,Node* pNode)
//{
//	if(pNode==NULL)return;
//	if(pNode->IsLeafNode())
//	{
//		sentence.push_back(pNode);
//		return;
//	}
//	if(pNode->IsPreTerminal())
//	{
//		
//	  Node* tempLeftNode=pNode->m_pLeftChild;
//	  if(tempLeftNode!=NULL)
//	  {
//		initialStateFromTaggedSentence(sentence,tempLeftNode);
//	
//	   }
//	 return;
//	}
//	Node* tempLeftNode=pNode->m_pLeftChild;
//	if(tempLeftNode!=NULL)
//	{
//		initialStateFromTaggedSentence(sentence,tempLeftNode);
//	
//	}
//	Node* tempRightNode=pNode->m_pRightChild;
//	if(tempRightNode!=NULL)
//	{
//		initialStateFromTaggedSentence(sentence,tempRightNode);
//	}
//	
//}
//State CBuildTree::initialStateFromGoldTagTree(Node* pNode)
//{ 
//	State state;
//    initialStateFromTaggedSentence(state.sentence,pNode);
//	return state;
//
//}
vector<string> CBuildTree::split(const string& s, const string& delim)//字符串切割函数 
{  
 vector<string> elems;  
 size_t pos = 0;  
 size_t len = s.length();  
 size_t delim_len = delim.length();  
 if (delim_len == 0) return elems;  
   while (pos < len)  
  {  
     int find_pos = s.find(delim, pos);  
     if (find_pos < 0)  
      {  
        elems.push_back(s.substr(pos, len - pos));  
        break;  
       }  
       elems.push_back(s.substr(pos, find_pos - pos));  
      pos = find_pos + delim_len;  
   }  
   return elems;  
}
bool CBuildTree::readCollinsHeadRule(const char * filename)
{
	
	ifstream in(filename);
	string lines;
	vector<string> temp;
	//string wj_m="";
	if(in)
	{
		//cout<<"success in open file"<<endl;
		//readRecordInfor Infor;
		
		while(getline(in,lines))
		{
			temp=split(lines,";");
			auto icc=nonTerminalInfo.find(temp[0]);
			if(icc!=nonTerminalInfo.end())
			{
				temp.erase(temp.begin());
				icc->second.push_back(temp);
			}
			else
			{
			  string key=temp[0];
			  temp.erase(temp.begin());
			  vector<vector<string>> temp_vec;
			  temp_vec.push_back(temp);
			  nonTerminalInfo.insert(make_pair(key,temp_vec));
			}
		}
		return true;

	}
	else
	{
		return false;
	}
	
}
void CBuildTree::Gethead(Node*&pNode)
{
	if (pNode==NULL) return;
	if ( pNode->IsLeafNode()) 
	{		
		pNode->m_pHead=NULL;
		pNode->headlabel=pNode->m_psPOS;
		pNode->headword=pNode->m_psWord;
		pNode->headtag=pNode->m_psPOS;		 
		Gethead(pNode->m_pNextBrother);
		return;
	}

	Gethead(pNode->m_pLeftChild);
	Gethead(pNode->m_pNextBrother);
	vector<Node*> childVector;
	Node* child=pNode->m_pLeftChild;
	while (child!=NULL )
	{ 	
	childVector.push_back(child);
	child=child->m_pNextBrother;			
	}
	string temp_c(pNode->m_psPOS);
	auto igg=nonTerminalInfo.find(temp_c);
	if(igg!=nonTerminalInfo.end())
	{for(int i=0;i<igg->second.size();i++)
	{
	    bool lastResort=(i==igg->second.size()-1);
		child=traverLoate(childVector,igg->second[i],lastResort);////此处有疑问，有两条规则 那么选择用哪一个呢？,规则出来之后进行实验。

	}
	}
	pNode->m_pHead=child;
	pNode->headlabel=child->m_psPOS;
	pNode->headword=child->headword;
	pNode->headtag=child->headtag;
}



 void CBuildTree::Gethead_Star(Node *&pNode)
{
	if (pNode==NULL) return;
	if ( pNode->IsLeafNode()) 
	{		
		pNode->m_pHead=NULL;
		/*pNode->headlabel=pNode->m_psPOS;
		pNode->headword=pNode->m_psWord;
		pNode->headtag=pNode->m_psPOS;		 */
		Gethead_Star(pNode->m_pNextBrother);
		return;
	}

	Gethead_Star(pNode->m_pLeftChild);
	Gethead_Star(pNode->m_pNextBrother);

	Node * child;
	child=pNode->m_pLeftChild;
	while( child != NULL )
	{
		if( strchr(child->m_psPOS,'*') != NULL ||
			( child->m_psWord != NULL && strchr(child->m_psWord,'*') != NULL )
			)
			break;
		child=child->m_pNextBrother;
	}
	if( child == NULL  )
	{
		if( pNode->m_pLeftChild != pNode->m_pRightChild )
		{
			CString temp=pNode->m_psPOS;
			//AfxMessageBox(temp+" "+"some node have no star child");
		}
		child=pNode->m_pLeftChild;
	}

	pNode->m_pHead=child;
	/*pNode->headlabel=child->m_psPOS;
	pNode->headword=child->headword;
	pNode->headtag=child->headtag;*/
}
void CBuildTree::createTransitionSequenceHelper(list<Transition*>& transition,Node* &pNode,set<string> rootstates,set<string> rootonlystates)
{
	createTransitionSequence(transition,pNode,rootstates,rootonlystates);
	transition.push_back(new FinalizeTransition(rootstates));
	transition.push_back(new IdleTransition());

}
void CBuildTree::findKnownStates(vector<Node*>&binarizedTrees,set<string>&knownStates)
{
	auto itt=binarizedTrees.begin();
	for(;itt!=binarizedTrees.end();itt++)
	{
		findKnownStates(*itt,knownStates);
	}

}
void CBuildTree::findKnownStates(Node*&pNode,set<string>&knownStates)
{

	if(pNode==NULL)return;
	if(pNode->IsLeafNode())
	{
		return;
	}
	 string label(pNode->m_psPOS);
	 if(label.find("@")==string::npos)
	 {
		if(label.find("*")!=string::npos)
		{
			label=label.substr(label.find("*")+1);
		}
		 
		knownStates.insert(label);
	 }

	if(pNode->IsPreTerminal())
	{
		
	  Node* tempLeftNode=pNode->m_pLeftChild;
	  if(tempLeftNode!=NULL)
	  {
		findKnownStates(tempLeftNode,knownStates);
	
	   }
	 return;
	}
	
	Node* tempLeftNode=pNode->m_pLeftChild;
	if(tempLeftNode!=NULL)
	{
		findKnownStates(tempLeftNode,knownStates);
	
	}
	Node* tempRightNode=pNode->m_pRightChild;
	if(tempRightNode!=NULL)
	{
		findKnownStates(tempRightNode,knownStates);
	}
}
void CBuildTree::findRootStates(vector<Node*>&binarizedTrees,set<string>&RootStates)
{
	auto itt=binarizedTrees.begin();
	for(;itt!=binarizedTrees.end();itt++)
	{
	   string label((*itt)->m_psPOS);
	   if(label.find("@")==string::npos)
	    {
		if(label.find("*")!=string::npos)
		{
			label=label.substr(label.find("*")+1);
		}
		 
		RootStates.insert(label);
	   }

	}

}
void CBuildTree::findRootOnlyStates(vector<Node*>&binarizedTrees,set<string>&RootOnlyStates)
{
	auto itt=binarizedTrees.begin();
	for(;itt!=binarizedTrees.end();itt++)
	{
	   string label((*itt)->m_psPOS);
	   if(label.find("@")==string::npos)
	    {
		if(label.find("*")!=string::npos)
		{
			label=label.substr(label.find("*")+1);
		}
		 
		RootOnlyStates.insert(label);
	   }
	   if((*itt)->m_pLeftChild!=NULL)
	   {
		   findRootOnlyStates((*itt)->m_pLeftChild,RootOnlyStates);
	   }
	   if((*itt)->m_pRightChild!=NULL)
	   {
		   findRootOnlyStates((*itt)->m_pRightChild,RootOnlyStates);
	   }
	 }

}
void CBuildTree::findRootOnlyStates(Node*&pNode,set<string>&RootOnlyStates)
{

	if(pNode==NULL)return;
	if(pNode->IsLeafNode())
	{
		return;
	}
	 string label(pNode->m_psPOS);
	 if(label.find("@")==string::npos)
	 {
		if(label.find("*")!=string::npos)
		{
			label=label.substr(label.find("*")+1);
		}
		 
		auto itt=RootOnlyStates.find(label);
		if(itt!=RootOnlyStates.end()){
			RootOnlyStates.erase(itt);
		}
	 }

	if(pNode->IsPreTerminal())
	{
		
	  Node* tempLeftNode=pNode->m_pLeftChild;
	  if(tempLeftNode!=NULL)
	  {
		findRootOnlyStates(tempLeftNode,RootOnlyStates);
	
	   }
	 return;
	}
	
	Node* tempLeftNode=pNode->m_pLeftChild;
	if(tempLeftNode!=NULL)
	{
		findRootOnlyStates(tempLeftNode,RootOnlyStates);
	
	}
	Node* tempRightNode=pNode->m_pRightChild;
	if(tempRightNode!=NULL)
	{
		findRootOnlyStates(tempRightNode,RootOnlyStates);
	}
}

void CBuildTree::delete_Node(Node * &pNode) 
{	
	if(pNode==NULL)return;
	/*if(pNode->IsLeafNode())
	{
		delete pNode;
		pNode==NULL;
	}
	*/
	Node* tempLeftNode=pNode->m_pLeftChild;
	if(tempLeftNode!=NULL)
	{
		delete_Node(tempLeftNode);
	
	}
	Node* tempRightNode=pNode->m_pRightChild;
	if(tempRightNode!=NULL)
	{
		delete_Node(tempRightNode);
	}
	//delete []pNode->m_psPOS;
	delete pNode;
	pNode=NULL;
}
Node* CBuildTree::traverLoate(vector<Node*>&childVector,vector<string>&condicate,bool lastResort)
{
	int headIndex=-1;	
	if(strcmp("left",condicate[0].c_str())==0)
	{
		headIndex=findLeftHead(childVector,condicate);
	}
	else if(strcmp("leftdis",condicate[0].c_str())==0)
	{
		headIndex=findLeftDisHead(childVector,condicate);
	}
	else if(strcmp("right",condicate[0].c_str())==0)
	{
		headIndex=findRightDisHead(childVector,condicate);
	}
	else if(strcmp("rightdis",condicate[0].c_str())==0)
	{
		headIndex=findRightDisHead(childVector,condicate);
	}
	else
	{

	}
	if(headIndex>-1)
	{
		return childVector[headIndex];
	}
	else
	{
		return childVector[childVector.size()-1];
	}
	

}
int  CBuildTree::findLeftHead(vector<Node*>&childVector,vector<string>&condicate)
{
	for(int i=1;i<condicate.size();i++)
	{
		for(int j=0;j<childVector.size();j++)
		{
			if(strcmp(childVector[j]->m_psPOS,condicate[i].c_str())==0)
			{
				return j;
			}
		}
	}
	return -1;
}
int  CBuildTree::findLeftDisHead(vector<Node*>&childVector,vector<string>&condicate)
{
	
		for(int j=0;j<childVector.size();j++)
		{
			for(int i=1;i<condicate.size();i++)
	      {
			if(strcmp(childVector[j]->m_psPOS,condicate[i].c_str())==0)
			{
				return j;
			}
		}
	}
	return -1;
}
int  CBuildTree::findRightHead(vector<Node*>&childVector,vector<string>&condicate)
{
	for(int i=1;i<condicate.size();i++)
	{
		for(int j=childVector.size()-1;j>=0;j--)
		{
			if(strcmp(childVector[j]->m_psPOS,condicate[i].c_str())==0)
			{
				return j;
			}
		}
	}
	return -1;
}
int  CBuildTree::findRightDisHead(vector<Node*>&childVector,vector<string>&condicate)
{
	
		for(int j=childVector.size()-1;j>=0;j--)
		{
			for(int i=1;i<condicate.size();i++)
	        {
			if(strcmp(childVector[j]->m_psPOS,condicate[i].c_str())==0)
			{
				return j;
			}
		   }
	}
	return -1;
}