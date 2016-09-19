#include "stdafx.h"
#include "KbaseTest.h"




KbaseTest::KbaseTest(void)
{
	m_hTpiHandle		= 0;          //���÷����������ʼֵ
	m_hRecordSet		= 0;          //���ü�¼������ʼֵ
}

KbaseTest::~KbaseTest(void)
{
	//�رն�Ӧ�ļ�¼�����
	::TPI_CloseRecordSet( m_hRecordSet );

	//�ر��������������ͨ������
	::TPI_CloseCon( m_hTpiHandle );
}


//�򿪷�����SHost�����ط��������
bool KbaseTest::TPIServerInit(string SHost)
{
	try
	{
		//���ݿ����ر�־
		int nErrorCode = 0;               //�����־

		//���ӷ�������ַ192.168.100.215
		//cout<<"�򿪷�����:"<<SHost<<endl;
		TPI_LOGIN_PARA	para;
		::memcpy( para.szUserName, "DBOWN", 6 );
		::memset( para.szPassWord, 0x00, sizeof( para.szPassWord ) );
		::memset( &para.UserIp, 0x00, sizeof( para.UserIp ) );
		m_hTpiHandle  = ::TPI_OpenCon( SHost.c_str(), 4567, para, &nErrorCode);
		if (nErrorCode < 0) 
		{
			write_log("��ͨ�����Ӵ���");
			return false;
		}
		else
		{
			//cout<<"��ͨ�����ӳɹ���"<<endl;
		}
		if (m_hTpiHandle == NULL) 
		{
			write_log("δ�õ�ͨ�ž����");
			return false;
		}
		else
		{
			//cout<<"�õ�ͨ�ž����"<<endl;
		}
		return true;
	}
	catch (...)
	{
		write_log("TPIServerInit abnormal!");
		return false;
	}
}

//��ȡָ����¼�����
bool KbaseTest::TPIRecordSetOpen(string sSql_Name,double &SearchTime,long &RecordNum)
{
	try
	{
		if (m_hTpiHandle == 0)
		{
			write_log("TPIRecordSetOpen m_hTpiHandle is 0!");
			SearchTime = -1.0;
			RecordNum = -1;
			return false;
		}
		//���ݿ����ر�־
		int nErrorCode = 0;               //�����־

		//*************************************************************************//
		clock_t firsttime,secondtime;
		firsttime = clock();
		//*************************************************************************//

		m_hRecordSet  = ::TPI_OpenRecordSet( m_hTpiHandle, sSql_Name.c_str(), &nErrorCode );

		//*************************************************************************//
		secondtime = clock();
		double timetotal1 = (double)(secondtime-firsttime)/CLOCKS_PER_SEC;
		double timetotal2 = (double)(secondtime-firsttime);
		//cout<<"����SQLִ��ʱ�䣺"<<timetotal1<<"��"<<endl;
		//cout<<"����SQLִ��ʱ�䣺"<<timetotal2<<"����"<<endl;
		//*************************************************************************//

		if (nErrorCode<0) 
		{
			write_log("��ü�¼���������");
			SearchTime = -1.0;
			RecordNum = -1;
			return false;
		}
		if (m_hRecordSet == NULL) 
		{
			write_log("δ�õ���¼�������");
			SearchTime = -1.0;
			RecordNum = -1;
			return false;
		}
		//cout<<"RECORDNUM "<<TPI_GetRecordSetCount(m_hRecordSet)<<endl;
		long NUMHere(::TPI_GetRecordSetCount(m_hRecordSet));
		if (NUMHere<0)
		{
			write_log("��ȡ��¼����¼����ʧ�ܣ�");
			SearchTime = -1.0;
			RecordNum = -1;
			return false;
		}
		else
		{
			RecordNum = NUMHere;
		}
		SearchTime = timetotal2;
		return true;
	}
	catch (...)
	{
		write_log("TPIRecordSetOpen abnormal!");
		return false;
	}
}

//�������ȡ���ֶ�����
//��FieldName.ini�ж�ȡ�ֶ�����FieldVector
bool KbaseTest::SetFieldName(string FieldFileName,map<string,string> &FieldMap)
{
	try
	{
		ifstream infile(FieldFileName.c_str());
		if (!infile) 
		{
			write_log("SetFieldName infile Error!");
			return false;
		}
		string tline;
		while (getline(infile,tline,'\n')) 
		{
			if (tline.length()>0)
			{
				FieldMap.insert(map<string,string>::value_type(tline,"\0"));
			}
		}
		int fieldmapnum((int)(FieldMap.size()));
		if (fieldmapnum == 0) 
		{
			write_log("SetFieldName fieldmapnum zero!");
			return false;
		}
		return true;
	}
	catch (...)
	{
		write_log("SetFieldName abnormal!");
		return false;
	}
}

bool KbaseTest::SetRecNum(string RecNumFileName,string& strnum,long &RecNum)
{
	try
	{
		ifstream infile(RecNumFileName.c_str());
		if (!infile) 
		{
			write_log("RecNumFileName infile Error!");
			return false;
		}
		string tline("");
		long num=0;
		while (getline(infile,tline,'\n')) 
		{
			strnum=tline;
			num=atoi(tline.c_str());
		}
		//int fieldmapnum((int)(RecNumFileName.size()));
		RecNum=num;
		infile.close();
		return true;
	}
	catch (...)
	{
		write_log("SetRecNum abnormal!");
		return false;
	}
}

bool KbaseTest::GetKbaseTextByFieldName(void) 
{
	//��ȡ��Ҫȡ�õ��ֶ���
	map<string,string> FieldMap;
	if (!SetFieldName("FieldName.ini",FieldMap))
	{
		write_log("GetKbaseTextByFieldName GetKbaseTextByFieldName Error!");
		return false;
	}

	//������ļ�
	ofstream outfile("SpliteText_GetFromKBase_562270.txt");
	if (!outfile) 
	{
		write_log("GetKbaseTextByFieldName outfile Error!");
		return false;
	}

	//���ӷ�������ַ192.168.100.215
	cout<<"�򿪷�����:192.168.100.152"<<endl;
	if ( !TPIServerInit("192.168.100.152"))
	{
		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
		return false;
	}

	//�����ѯ���ֶ�����,����ѯ���ݻ�����ݾ��
	//cout<<"�����ݱ�:CJFD0608"<<endl;
	cout<<"�����ݱ�:CJFD2010"<<endl;
	string sSql_Name ="select * from ";
	sSql_Name +="CJFD2010";
	//sSql_Name +="DEFBASE_CJFD2008_B018";

	double timehere;
	long recornumhere;
	if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
	{
		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
		return false;
	}
	


	cout<<"����ÿ����¼..."<<endl;
	//����ÿ����¼
	//�����¼��Ŀ
	long iAcounter = 0;
    long inum = 0;
	while ( !::TPI_IsEOF( m_hRecordSet )  )
	{	
		cout<<"�����¼: "<<(++iAcounter)<<endl;
        if (iAcounter<562270)
        {
            ::TPI_MoveNext( m_hRecordSet );
            continue;
        }
		//��ȡһ����¼�ĸ����ֶ�����
        ++inum;
		bool bflg=false;
		map<string,string>::iterator itt = FieldMap.begin();
		while (itt!=FieldMap.end()) 
		{
			char fieldbuffer[1024*256];
			::memset( fieldbuffer,0x00,1024*256 );
			long lRet(0);
			lRet = ::TPI_GetFieldValueByName( m_hRecordSet,(itt->first).c_str(), fieldbuffer,1024*256);
			if ( lRet< 0 )
			{
				cerr<<"��ȡ�ֶ� "<<(itt->first).c_str()<<" ʧ�ܣ�"<<endl;
				bflg=true;
				break;
			}
			(itt->second) = fieldbuffer;
			++itt;
		}
		//����ĳһ�ضλ�ȡʧ�ܣ�������һ����¼����
		if (bflg)
		{
			map<string,string>::iterator itt1 = FieldMap.begin();
			while (itt1!=FieldMap.end()) 
			{
				(itt1->second) = "\0";
				++itt1;
			}
			::TPI_MoveNext( m_hRecordSet);
			continue;
		}

		//�����ѯ���������ļ�
		outfile<<"<REC>"<<iAcounter<<endl;
		vector<string> sentence_vect;
		map<string,string>::iterator itt2 = FieldMap.begin();
        string sent="";
		while (itt2!=FieldMap.end()) 
		{
			outfile<<"<"<<(itt2->first)<<">="<<(itt2->second)<<endl;
			//sentence_vect=splitEx(itt2->second,"������");
            if (itt2->second == "")//�����ֶ�Ϊ��ʱ.CJFD0608��562277��¼ȫ���ֶ�Ϊ��.
            {
                ++itt2;
                continue;
            }
            sentence_vect=SplitSentence(itt2->second.c_str());
			int i=0;
			vector<string>::iterator itt_s=sentence_vect.begin();
            
            //char chfirst;
			while(itt_s!=sentence_vect.end())
			{
                /*const char* sent_chs=itt_s->c_str();
                chfirst = *sent_chs;
                if (chfirst == 0x0d)
                {
                    sent=itt_s->substr(1,itt_s->length()-1);
                }
                else*/
                    sent=*itt_s;
                /*sent.TrimRight("\n");
                sent.TrimRight("\r");*/
				outfile<<"����"<<++i<<":"<</**itt_s */ sent<<endl;
				++itt_s;
			}
			
			(itt2->second) = "\0";
			++itt2;
		}

		//�жϻ�ȡ��¼������Ŀ
		/*if (inum == 200)
		{
			break;
		}*/
		//������һ����¼�Ĵ���
		::TPI_MoveNext( m_hRecordSet );
	}
	FieldMap.clear();
	outfile.close();
	return true;
}

bool KbaseTest::write_log(const char* TempStr)
{
	try
	{
		if (TempStr == NULL) 
		{
			cout<<"WriteLog input false!"<<endl;
			return false;
		}
		ofstream outfile("KbaseTest.log",ios::app);
		if (!outfile) 
		{
			cout<<"WriteLog openfile false!"<<endl;
			return false;
		}
		time_t timer;  
		timer=time(NULL); 

		char temptime1[256];
		strcpy_s(temptime1,256,"\0");
		ctime_s(temptime1,256,&timer);
		string temptime(temptime1);
		temptime[temptime.length()-1]='\0';
		outfile<<temptime<<" : "<<TempStr<<endl;
		outfile.close();
		return true;
	}
	catch (...)
	{
		cerr<<"KbaseTest::write_log_WriteLog abnormal!"<<endl;
		return false;
	}
}

vector<string> KbaseTest::splitEx( string text, string separate_character)
{
	vector<string> strs;
	string src=text;
	//int separate_characterLen = separate_character.size();//�ָ��ַ����ĳ���,�����Ϳ���֧���硰,,�����ַ����ķָ���
	string subsep1=separate_character.substr(0,2);
	string subsep2=separate_character.substr(2,2);
	string subsep3=separate_character.substr(4,2);
	int lastPosition = 0,index = 0,index1 = -1,index2 = -1,index3 = -1;
    if (text.c_str()=="\0")
    {
        strs.clear();
    }
	while (index<text.length())
	{
		index1 = src.find(subsep1,lastPosition);
		index2 = src.find(subsep2,lastPosition);
		index3 = src.find(subsep3,lastPosition);
		index=MinIndex(index1,index2,index3);
		//cout<<"index,1,2,3:"<<index<<" "<<index1<<" "<<index2<<" "<<index3<<endl;
		//cout<<src.substr(lastPosition,index+2 - lastPosition)<<endl;
		strs.push_back(src.substr(lastPosition,index+2 - lastPosition));
		//src=src.substr(lastPosition,src.length()-index-2);
		lastPosition = index + 2;
		if (lastPosition>=src.length())
		{
			break;
		}
	}
	//string lastString = src.substr(lastPosition);//��ȡ���һ���ָ����������
	//if (!lastString.empty())
	// strs.push_back(lastString);//������һ���ָ����������ݾ����
	return strs;
}

int KbaseTest::MinIndex(int i,int j ,int k)
{
	int minidex=-1;
	if (i>j)
	{
		minidex=j;
	}
	else
		minidex=i;
	if (minidex>k)
	{
		minidex=k;
	}
	else
		minidex=minidex;

	if (minidex!=-1)
	{
		return minidex;
	}
	else
		if (i>j && j!=-1)
		{
			return j;
		}
		else
			if (i!=-1)
			{
				return i;
			}
			else
				if (j!=-1)
				{
					return j;
				}
				else 
					return k;

}

 bool KbaseTest::ParseKbaseText(void)
 {
 	//��ȡ��Ҫȡ�õ��ֶ���
 	map<string,string> FieldMap;
 	if (!SetFieldName("FieldName.ini",FieldMap))
 	{
 		write_log("GetKbaseTextByFieldName GetKbaseTextByFieldName Error!");
 		return false;
 	}
 	//������ļ�
 	
 	
 	//���ӷ�������ַ192.168.100.215
 	cout<<"�򿪷�����:192.168.100.157"<<endl;
     if ( !TPIServerInit("192.168.100.157"))
     {
         write_log("GetKbaseTextByFieldName TPIServerInit Error!");
         return false;
     }
 	//�����ѯ���ֶ�����,����ѯ���ݻ�����ݾ��
 	//cout<<"�����ݱ�:CJFD0608"<<endl;
 	cout<<"�����ݱ�:CJFD0608"<<endl;
 	string sSql_Name ="select * from ";
 	sSql_Name +="CJFD2013";
 	//sSql_Name +="DEFBASE_CJFD2008_B018";
 	double timehere;
 	long recornumhere;
 	if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
 	{
 		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
 		return false;
 	}
 
 	//�䷨������ʼ����ʼ���䷨����
 	if (!HParser_Init(0))
 	{
 		cout<<"HParser_Init ERROR!"<<endl;
 		return false;
 	}
	if(!LoadModel(0))
	{
		cout<<"����ʧ��"<<endl;
	}
	else
	{
		cout<<"���سɹ�"<<endl;
	}
 	 
 	cout<<"���ؾ䷨��Դ�ɹ���\n����ÿ����¼..."<<endl;
 	//����ÿ����¼
 	//�����¼��Ŀ
 	long iAcounter = 0;
 	long rec_num=0;
 	string str_num;
 	if (!SetRecNum("RecNum.ini",str_num,rec_num))
 	{
 		write_log("SetRecNum Error!");
 		return false;
 	}


 	//������ļ�
 	string filename="";
 	//sprintf(filename,"Text_GetFromKBase_%d.txt",rec_num);
	filename="����䷨��������"+str_num+".txt";
//	filename
 	ofstream outfile(filename.c_str());
 	if (!outfile) 
 	{
 		write_log("GetKbaseTextByFieldName outfile Error!");
 		return false;
 	}
     vector<string> sentence_vect;//�洢��ȫ���ֶη־�֮������о���.
     char fieldbuffer[1024*256];
     long allSent=0;
     if (TPI_Move(m_hRecordSet, rec_num ) != true)
     {
         cout<<"Move record num error!"<<endl;
         return false;
     }
     iAcounter=rec_num-1;
   
 	while ( !::TPI_IsEOF( m_hRecordSet )  )
 	{	
 		cout<<"�����¼: "<<(++iAcounter)/*<<endl*/;
		if(iAcounter>300)break;
 		bool bflg=false;
 		map<string,string>::iterator itt = FieldMap.begin();
         
 		while (itt!=FieldMap.end()) 
 		{
 			
 			::memset( fieldbuffer,0x00,262144 );
 			long lRet(0);
 			lRet = ::TPI_GetFieldValueByName( m_hRecordSet,(itt->first).c_str(), fieldbuffer,/*1024*256*/ 262144);
 			if ( lRet< 0 )
 			{
 				cerr<<"��ȡ�ֶ� "<<(itt->first).c_str()<<" ʧ�ܣ�"<<endl;
 				bflg=true;
 				break;
 			}
 			(itt->second) = fieldbuffer;
 			++itt;
 		}
 		//����ĳһ�ضλ�ȡʧ�ܣ�������һ����¼����
 		if (bflg)
         {
             map<string,string>::iterator itt1 = FieldMap.begin();
 			while (itt1!=FieldMap.end()) 
 			{
 				(itt1->second) = "\0";
 				++itt1;
 			}
 			::TPI_MoveNext( m_hRecordSet);
 			continue;
 		}
 		
 		//�����ѯ���������ļ�
 
         
        outfile<<"<REC>="<<iAcounter<<"ƪ.\n";
 		map<string,string>::iterator itt2 = FieldMap.begin();
 		while (itt2!=FieldMap.end()) 
 		{
             
 			//outfile<<"<"<<(itt2->first)<<">="<<(itt2->second)<<"\n";
             sentence_vect=SplitSentence(itt2->second);
             if (sentence_vect.size()==0)
             {
                 
                 cout<<",������0,������"<<allSent<<endl;
                 (itt2->second) = "\0";
                 //sentence_vect.clear();
                 ++itt2;
                 break;
             }
 			vector<string>::iterator itt_s=sentence_vect.begin();
             int nSent=0;
 			while(itt_s!=sentence_vect.end())
 			{
                 ++nSent;
 				//outfile<<"<SETENCE>="<<nSent<<"="<<itt_s->c_str()<<"\n";
				   char *outsent=new char[1024];
                  int outlen=1024;
	              char *outsent2=new char[1024];
                 int outlen2=1024;
 				outlen=1024;
				outlen2=1024;
 				strcpy_s(outsent,1024,"\0");
				strcpy_s(outsent2,1024,"\0");
 				if (HParser_SegPos(itt_s->c_str(),outsent,outlen)==false)
 				{
 					/*cout<<"���0��"<<outsent<<endl;
 					cout<<"ParSent ERROR!"<<endl;*/
 					write_log("HITSEG ERROR:");
 					write_log(itt_s->c_str());
                     outfile<<"<HITSEG>=error!\n";
                     ++itt_s;
                     continue;
 				}
				string sin(outsent);
				//sin=sin.substr(0,sin.find_last_not_of(" ")+1);
				if(!Parser_S(sin.c_str(),outsent2,outlen2))
				{
					write_log("SRPARSE ERROR:");
				}
				else
				{
 				outfile<<"<PARSE>="<<outsent2<<"\n";
				}
				delete[] outsent;
                outsent = NULL;
	            delete[] outsent2;
               outsent2 = NULL;
 				++itt_s;
 			}
             
             allSent+=nSent;
             cout<<",������"<<nSent<<",������"<<allSent<<endl;
 			(itt2->second) = "\0";
             sentence_vect.clear();
 			++itt2;
 		}
         
 		//�жϻ�ȡ��¼������Ŀ
         /*if (iAcounter == 500)
         {
         break;
         }*/
 		//������һ����¼�Ĵ���
 		::TPI_MoveNext( m_hRecordSet );
 	}
     /*cout<<"ÿƪ���¾�������"<< allSent/iAcounter<<endl;*/
 	FieldMap.clear();
     sentence_vect.clear();
 	
     outfile.close();
	 if(!UnLoadModel())
	{
		cout<<"ж��ʧ��"<<endl;
	}
	else
	{
		cout<<"ж�سɹ�"<<endl;
	}
 
 	if (!HParser_UnInit())
 	{
 		cout<<"UNInit ERROR!"<<endl;
 		return false;
 	}
    
 	return true;
 }
 
// bool KbaseTest::TestCJFDpaper_length(void)
// {
// 	//��ȡ��Ҫȡ�õ��ֶ���
// 	map<string,string> FieldMap;
// 	if (!SetFieldName("FieldName.ini",FieldMap))
// 	{
// 		write_log("GetKbaseTextByFieldName GetKbaseTextByFieldName Error!");
// 		return false;
// 	}
// 	//������ļ�
// 	//���ӷ�������ַ192.168.100.215
// 	cout<<"�򿪷�����:192.168.100.114"<<endl;
// 	if ( !TPIServerInit("192.168.100.114"))
// 	{
// 		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
// 		return false;
// 	}
// 	//�����ѯ���ֶ�����,����ѯ���ݻ�����ݾ��
// 	//cout<<"�����ݱ�:CJFD0608"<<endl;
// 	cout<<"�����ݱ�:CJFD0608"<<endl;
// 	string sSql_Name ="select * from ";
// 	sSql_Name +="CJFD0608";
// 	//sSql_Name +="DEFBASE_CJFD2008_B018";
// 	double timehere;
// 	long recornumhere;
// 	if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
// 	{
// 		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
// 		return false;
// 	}
// 
// 	//�䷨������ʼ����ʼ���䷨����
// 	/*if (!HitParserAPI_Init())
// 	{
// 		cout<<"INIT ERROR!"<<endl;
// 		return false;
// 	}
//     char *outsent=new char[2048];
//     int outlen=2048;*/
// 
// 
// 	cout<<"����ÿ����¼..."<<endl;
// 	//����ÿ����¼
// 	//�����¼��Ŀ
// 	long iAcounter = 0;
//     long iAcounter_fact=0;
//     /*int rec_num=0;
//     string str_num;
//     if (!SetRecNum("RecNum.ini",str_num,rec_num))
//     {
//     write_log("SetRecNum Error!");
//     return false;
//     }*/
// 
// 	//������ļ�
// 	//string filename="";
// 	//sprintf(filename,"Text_GetFromKBase_%d.txt",rec_num);
// 	/*filename="Text_GetFromKBase_"+str_num+".txt";
// 	ofstream outfile(filename.c_str());
// 	if (!outfile) 
// 	{
// 		write_log("GetKbaseTextByFieldName outfile Error!");
// 		return false;
// 	}*/
// 
//     long int rec_length=0; //һ�����¼�¼��ƽ�����ȣ�
//     long int allrec_all_length=0;//���м�¼���µ�ƽ�����ȵĺͣ�
//     long int allrec_1_length=0;//���м�¼���ƽ�����ȣ�
//     long int rec_sentence_num=0;
//     long int rec_all_sentence_num=0;
// 
//     CSentFromFile SentNodeProcess;
//     int sent_1_count=0;
//     long sent_all_count=0;
//     CString *str_sent;
//     InputNode SentNode[500];
// 
// 
// 	while ( !::TPI_IsEOF( m_hRecordSet )  )
// 	{	
// 		cout<<"�����¼: "<<(++iAcounter);
//         if (iAcounter==1000)
//         {
//             break;
//         }
// 
//         /*if (iAcounter < 562277)
//         {
//             ::TPI_MoveNext( m_hRecordSet);
//             continue;
//         }*/
// 		//��ȡһ����¼�ĸ����ֶ�����
// 		bool bflg=false;
// 		map<string,string>::iterator itt = FieldMap.begin();
// 		while (itt!=FieldMap.end()) 
// 		{
// 			char fieldbuffer[1024*256];
// 			::memset( fieldbuffer,0x00,1024*256 );
// 			long lRet(0);
// 			lRet = ::TPI_GetFieldValueByName( m_hRecordSet,(itt->first).c_str(), fieldbuffer,1024*256);
// 			if ( lRet< 0 )
// 			{
// 				cerr<<"��ȡ�ֶ� "<<(itt->first).c_str()<<" ʧ�ܣ�"<<endl;
// 				bflg=true;
// 				break;
// 			}
// 			(itt->second) = fieldbuffer;
// 			++itt;
// 		}
// 		//����ĳһ�ضλ�ȡʧ�ܣ�������һ����¼����
// 		if (bflg)
//         {
//             map<string,string>::iterator itt1 = FieldMap.begin();
// 			while (itt1!=FieldMap.end()) 
// 			{
// 				(itt1->second) = "\0";
// 				++itt1;
// 			}
// 			::TPI_MoveNext( m_hRecordSet);
// 			continue;
// 		}
// 		
// 		//�����ѯ���������ļ�
// 		//outfile<<"<REC>="<<iAcounter<<"\n";
// 
// 		vector<string> sentence_vect;
// 		map<string,string>::iterator itt2 = FieldMap.begin();
// 		while (itt2!=FieldMap.end()) 
// 		{
// 			//outfile<<"<"<<(itt2->first)<<">="<<(itt2->second)<<"\n";
// 			//sentence_vect=splitEx(itt2->second,"������");
// 
// 			//vector<string>::iterator itt_s=sentence_vect.begin();
// 			//while(itt_s!=sentence_vect.end())
// 			//{
// 			//	outfile<<"<SETENCE>="<<itt_s->c_str()<<"\n";
// 			//	outlen=2048;
// 			//	strcpy_s(outsent,1024,"\0");
// 			//	if (HitParserAPI_ParSent(itt_s->c_str(),0,outsent,outlen)==false)
// 			//	{
// 			//		/*cout<<"���0��"<<outsent<<endl;
// 			//		cout<<"ParSent ERROR!"<<endl;*/
// 			//		write_log("PARSE ERROR:");
// 			//		write_log(itt_s->c_str());
// 			//		//return 0;
// 			//	}
// 			//	outfile<<"<PARSE>="<<outsent<<"\n";
// 			//	++itt_s;
// 			//}
//             sentence_vect=SplitSentence(itt2->second);
//             rec_sentence_num=sentence_vect.size();//һƪ���׵ľ�����Ŀ.
//             
//             if (rec_sentence_num==0)
//             {
//                 break;
//             }
// 
//             if (rec_sentence_num<15)
//             {
//                 cout<<endl;
//                 break;
//             } 
//             
//             rec_length=(long int)itt2->second.length()/rec_sentence_num;//һƪ���׵�ƽ�����ӳ����ֽ���.
//             if (rec_length>256)
//             {
//                 cout<<endl;
//                 break;
//             }
//             ++iAcounter_fact;
//             cout<<",��"<<rec_sentence_num<<"��,�����¾���ƽ�����ȣ�"<<rec_length<<endl;
//             allrec_all_length+=rec_length;
//             
//             for (int i=0;i<rec_sentence_num;i++)
//             {
//             //cout<<"���ӣ�"<<i<<"Ϊ��"<<sentence_vect[i].c_str()<<endl;
//             //sent_1_count = SentNodeProcess.ReadSentLineFromFile(sentence_vect[i].c_str(),SentNode,str_sent,500);
//             sent_1_count=SentNodeProcess.LineProcess(sentence_vect[i].c_str(),SentNode,500);
//             cout<<"NODE�ڵ�����"<<sent_1_count<<endl;
//             if (sent_1_count==0)
//             {
//                 continue;
//             }
//             sent_all_count+=sent_1_count;
//             }
//             rec_all_sentence_num+=rec_sentence_num;
// 
// 			(itt2->second) = "\0";
// 			++itt2;
// 		}
//         
// 		//�жϻ�ȡ��¼������Ŀ
// 		/*if (iAcounter == 100)
// 		{
// 			break;
// 		}*/
// 		//������һ����¼�Ĵ���
// 		::TPI_MoveNext( m_hRecordSet );
// 	}
// 	FieldMap.clear();
//     allrec_1_length=allrec_all_length/iAcounter_fact;
//     cout<<"�������¼��"<<iAcounter<<",ʵ�ʷ���������¼��"<<iAcounter_fact<<endl;
//     cout<<"ͳ�Ƶó�CJFD�������׵�ƽ�����ӳ����ǣ�"<<allrec_1_length<<endl;
// 
//     cout<<"***********"<<endl;
//     cout<<"����ƽ���ڵ�����"<< sent_all_count/rec_all_sentence_num<<endl;;
// 	//outfile.close();
//     /*if (!HitParserAPI_UnInit())
// 	{
// 		cout<<"UNInit ERROR!"<<endl;
// 		return false;
// 	}*/
// 
// 	return true;
// }

vector<string>KbaseTest::SplitSentence(string sent)
 {
     vector<string> vect_sent;
     vect_sent.clear();
     if (sent.empty()==true)
     {
         vect_sent.clear();
         return vect_sent;
     }
     /*if (sent.length()==0)
     {
         vect_sent.clear();
         return vect_sent;
     }*/
     unsigned char ch_first,ch_second;//ch_firstΪ˫�ֽڵĵ�һ�ֽ�.//���ֽ�ʱ����ch_first,ch_second��˫�ֽڵĵڶ��ֽ�
     int index=0;
     vector<int> vect_pos;
     int pos_i=0;
     vect_pos.push_back(-1);
     int length=sent.length();
     ch_first=(unsigned char)sent.at(0);
     //cout<<"�ļ����ݣ�"<<sent<<endl;
     while(index<(unsigned int)length)
     {
         ch_first=(unsigned char)sent.at(index);
         if (ch_first<= 0x80)//Ϊ���ֽ��ַ�
         {
             if (ch_first==0x0d)//�س���.
             {
                 vect_pos.push_back(index);
                 index+=1;
             }
             else if (ch_first==0x3b || ch_first==0x3f || ch_first==0x21)//���;?!.
             {
                 vect_pos.push_back(index);
                 index+=1;
             }
             else
                 ++index;
         }

         else//Ϊ˫�ֽ�
         {
             if (ch_first==0xa1)
             {
                 ch_second=sent.at(++index);
                 if (ch_second==0xa3)//��
                 {
                     vect_pos.push_back(index);
                     index+=1;
                 }
                 else index+=1;
             }
             else if (ch_first==0xa3)
             {
                 ch_second=sent.at(++index);
                 if (ch_second==0xa1 || ch_second==0xbb || ch_second==0xbf)//���ķ��ţ�����
                 {
                     vect_pos.push_back(index);
                     index+=1;
                 }
                 else index+=1;
             }
             else
                 index+=2;
         }
     }
     if (vect_pos[vect_pos.size()-1]!=length-1)//ȷ���������һ��.
     {
         vect_pos.push_back(length-1);
     }
     /*int i=0;
     while(i<vect_pos.size())
     {
         cout<<vect_pos[i]<<endl;
         ++i;
     }*/

     string sentence="";
     for (int i=0;i<vect_pos.size()-1;i++)
     {
         sentence=sent.substr(vect_pos[i]+1,vect_pos[i+1]-vect_pos[i]);
         if (sentence.size()>1 )
         {
             vect_sent.push_back(sentence);
         }               
     }
      /*for(int j=0;j<vect_sent.size();j++)
      {
          
          cout<<"���ӣ�"<<j<<"��"<<vect_sent[j]<<endl;
      }*/

     return vect_sent;
 }
bool KbaseTest::inputNameOfCIDian(const string filename1,const string filename2,set<string>& cidian1,set<string>&cidian2)
{
	ifstream in(filename1);
	ifstream in2(filename2);
	string lines;
	if(in)
	{
		cout<<"success in open file1"<<endl;
		while(getline(in,lines))
		{
			cidian1.insert(lines);
		}
	}
	if(in2)
	{
		cout<<"success in open file2"<<endl;
		while(getline(in2,lines))
		{
			cidian2.insert(lines);
		}
	}
		  
	return true;
}
bool KbaseTest::inputNameOfguanjianci(const string filename,set<string>&guanjianci)
{
	ifstream in(filename);

	string lines;
	if(in)
	{
		cout<<"success in open file1"<<endl;
		while(getline(in,lines))
		{
			guanjianci.insert(lines);
		}
	}
	
		  
	return true;
}

bool KbaseTest::OuputDicNameText(set<string>& guanjianci)
{
     map<string,set<string>> dicName;
	 string outfilename="����ؼ��ʴʵ�����.txt";
	 ofstream outfile1(outfilename.c_str());
	 if (!outfile1)//||!outfile2) 
 	 {
 		write_log("GetKbaseTextByFieldName outfile Error!");
 	    return false;
 	 }
	   cout<<"�򿪷�����:192.168.100.244"<<endl;
     if ( !TPIServerInit("192.168.100.244"))
     {
         write_log("GetKbaseTextByFieldName TPIServerInit Error!");
         return false;
     }
	 string database[]={"CRFD2008","CRFD2009","CRFD2010","CRFD2011","CRFD2012","CRFD2013","CRFD2014","CRFD2015"};
	 string sSql_Name="";
	 for(int i=0;i<4;i++)
	 {
		 cout<<"�����ݱ�:"<<database[i]<<endl;
		 auto icc=guanjianci.begin();
		 for(;icc!=guanjianci.end();icc++)
		 {
		 sSql_Name ="select * from ";
 	      sSql_Name +=database[i]+" where ��Ŀ����='"+*icc+"'";
		 double timehere;
 	     long recornumhere;
		 long iAcounter = 0;
 	     long rec_num=0;
 	     string str_num;
		 char fieldbuffer[1024*256];
 	     if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
 	     {
 		     write_log("GetKbaseTextByFieldName TPIServerInit Error!");
 		     return false;
 	     }
		 if (!SetRecNum("RecNum.ini",str_num,rec_num))
 	      {
 		      write_log("SetRecNum Error!");
 		      return false;
 	     }
		 if (TPI_Move(m_hRecordSet, 1 ) != true)
         {
           //cout<<"Move record num error!"<<endl;
            continue;
         }
		 else
		 {
			 cout<<"�ҵ��ʵ�"<<database[i]<<"��"<<*icc<<endl;
			 //string outfilename=".\\�ʵ�\\"+*icc+".txt";
			 //ofstream outfile1(outfilename.c_str());
	   //      if (!outfile1)//||!outfile2) 
 	  //       {
 		 //          write_log("GetKbaseTextByFieldName outfile Error!");
 		 //          return false;
 	  //        }
		 
		while ( !::TPI_IsEOF( m_hRecordSet )  )
 		{	
		
			cout<<"�����¼: "<<iAcounter<<endl;
			++iAcounter;
		::memset( fieldbuffer,0x00,1024*256);
		int lRet = ::TPI_GetFieldValueByName( m_hRecordSet,"����������", fieldbuffer,1024*256);
		if ( lRet< 0 )
		{
			cerr<<"��ȡ�ֶ� "<<"����������"<<" ʧ�ܣ�"<<endl;
			::TPI_MoveNext( m_hRecordSet);
			continue;
		}
		auto imm=dicName.find(*icc);
	    if(imm!=dicName.end())
		{
			imm->second.insert(string(fieldbuffer));

		}
		else
		{
			set<string> temp;
			temp.insert(string(fieldbuffer));
			dicName.insert(make_pair(*icc,temp));

		}
		//dicName.insert(string(fieldbuffer));
		::TPI_MoveNext( m_hRecordSet);
		continue;
		}
		 }
		 }
	 }
	 auto igg=dicName.begin();
	 for(;igg!=dicName.end();igg++)
	 {
		 auto ikk=igg->second.begin();
		 for(;ikk!=igg->second.end();ikk++)
		 {
			 outfile1<<igg->first<<"\t"<<*ikk<<endl;
		   
		 }

	 }
 
}
bool KbaseTest::OuputHEKUKbaseText(set<string>& cidian1,set<string>&cidian2)
{
     cout<<"�򿪷�����:192.168.100.244"<<endl;
     if ( !TPIServerInit("192.168.100.244"))
     {
         write_log("GetKbaseTextByFieldName TPIServerInit Error!");
         return false;
     }
	 string database[]={"CRFD2008","CRFD2009","CRFD2010","CRFD2011","CRFD2012","CRFD2013","CRFD2014","CRFD2015"};
	 string sSql_Name="";
	 for(int i=0;i<4;i++)
	 {
		 cout<<"�����ݱ�:"<<database[i]<<endl;
		 auto icc=cidian1.begin();
		 for(;icc!=cidian1.end();icc++)
		 {
		 sSql_Name ="select * from ";
 	      sSql_Name +=database[i]+" where ����������='"+*icc+"'";
		 double timehere;
 	     long recornumhere;
		 long iAcounter = 0;
 	     long rec_num=0;
 	     string str_num;
		 char fieldbuffer[1024*256];
 	     if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
 	     {
 		     write_log("GetKbaseTextByFieldName TPIServerInit Error!");
 		     return false;
 	     }
		 if (!SetRecNum("RecNum.ini",str_num,rec_num))
 	      {
 		      write_log("SetRecNum Error!");
 		      return false;
 	     }
		 if (TPI_Move(m_hRecordSet, 1 ) != true)
         {
           //cout<<"Move record num error!"<<endl;
            continue;
         }
		 else
		 {
			 cout<<"�ҵ��ʵ�"<<database[i]<<"��"<<*icc<<endl;
			 string outfilename=".\\�ʵ�\\"+*icc+".txt";
			 ofstream outfile1(outfilename.c_str());
	         if (!outfile1)//||!outfile2) 
 	         {
 		           write_log("GetKbaseTextByFieldName outfile Error!");
 		           return false;
 	          }
		 
		while ( !::TPI_IsEOF( m_hRecordSet )  )
 		{	
		
			cout<<"�����¼: "<<iAcounter<<endl;
			++iAcounter;
		::memset( fieldbuffer,0x00,1024*256);
		int lRet = ::TPI_GetFieldValueByName( m_hRecordSet,"��Ŀ����", fieldbuffer,1024*256);
		if ( lRet< 0 )
		{
			cerr<<"��ȡ�ֶ� "<<"��Ŀ����"<<" ʧ�ܣ�"<<endl;
			::TPI_MoveNext( m_hRecordSet);
			continue;
		}
 		outfile1<<fieldbuffer<<endl;
		::TPI_MoveNext( m_hRecordSet);
		continue;
		}
		 }
		 }
	 }
	
 
}
bool KbaseTest::TitleParseKbaseText()
{
	//��ȡ��Ҫȡ�õ��ֶ���
 /*	map<string,string> FieldMap;
 	if (!SetFieldName("FieldName.ini",FieldMap))
 	{
 		write_log("GetKbaseTextByFieldName GetKbaseTextByFieldName Error!");
 		return false;
 	}*/
 	//������ļ�
 	
 	
 	//���ӷ�������ַ192.168.100.215
 	cout<<"�򿪷�����:192.168.100.157"<<endl;
     if ( !TPIServerInit("192.168.100.157"))
     {
         write_log("GetKbaseTextByFieldName TPIServerInit Error!");
         return false;
     }
 	//�����ѯ���ֶ�����,����ѯ���ݻ�����ݾ��
 	//cout<<"�����ݱ�:CJFD0608"<<endl;
 	cout<<"�����ݱ�:CRFD2009"<<endl;
 	string sSql_Name ="select * from ";
 	sSql_Name +="CRFD2009";
 	//sSql_Name +="DEFBASE_CJFD2008_B018";
 	double timehere;
 	long recornumhere;
 	if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
 	{
 		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
 		return false;
 	}
 
 	//�䷨������ʼ����ʼ���䷨����
	
	cout<<"��ʼ���شʵ�..."<<endl;
 	if (!HParser_Init(2))
 	{
 		cout<<"HParser_Init ERROR!"<<endl;
 		return false;
 	}
 	 
 	cout<<"���ؾ䷨��Դ�ɹ���\n����ÿ����¼..."<<endl;
 	//����ÿ����¼
 	//�����¼��Ŀ
 	long iAcounter = 0;
 	long rec_num=0;
 	string str_num;
 	if (!SetRecNum("RecNum.ini",str_num,rec_num))
 	{
 		write_log("SetRecNum Error!");
 		return false;
 	}


 	//������ļ�
 	string filename1="";
	string filename2="";
 	//sprintf(filename,"Text_GetFromKBase_%d.txt",rec_num);
 //	filename="����ļ�\\����䷨��������"+str_num+".txt";
	filename1="�ִ�ƪ��ԭʼ����.txt";
	//filename2="NLPE�ִ�ժҪ���Խ��.txt";
 	ofstream outfile1(filename1.c_str());
	ofstream outfile2(filename2.c_str());
 	if (!outfile1)//||!outfile2) 
 	{
 		write_log("GetKbaseTextByFieldName outfile Error!");
 		return false;
 	}
     vector<string> sentence_vect;//�洢��ȫ���ֶη־�֮������о���.
     char fieldbuffer[1024*256];
     long allSent=0;
     if (TPI_Move(m_hRecordSet, rec_num ) != true)
     {
         cout<<"Move record num error!"<<endl;
         return false;
     }
     iAcounter=1;
     char *outsent1=new char[1024];
     int outlen1=1024;
	 char *outsent2=new char[1024];
     int outlen2=1024;
	 string fieldTitle="ƪ��";
	 //string fieldAbstract="";
	 /*Dict_Init_New();
	 HDICT handle1=Dict_Load_New("J:\\DICTBIN\\NLPE_DICT\\sys_dynamicprg_dic", "\0");
     HDICT handle2 =Dict_Load_New("J:\\DICTBIN\\NLPE_DICT\\sys_general_dic", "\0");
	 HDICT handle3 = Dict_Load_New("J:\\DICTBIN\\NLPE_DICT\\General_Dict", "\0");*/

	/* if(handle1<0||handle2<0||handle3<0)
	 {
	   cout<<"�ʵ����ʧ��"<<endl;
		 return 0;
	 }*/
 	while ( !::TPI_IsEOF( m_hRecordSet )  )
 	{	
		if (iAcounter>1001)
		{
			cout<<"�����¼: "<<iAcounter<<endl;
			break;
		}
 		++iAcounter;
		::memset( fieldbuffer,0x00,1024*256);
		int lRet = ::TPI_GetFieldValueByName( m_hRecordSet,fieldTitle.c_str(), fieldbuffer,1024*256);
		if ( lRet< 0 )
		{
			cerr<<"��ȡ�ֶ� "<<fieldTitle.c_str()<<" ʧ�ܣ�"<<endl;
			::TPI_MoveNext( m_hRecordSet);
			continue;
		}
 		outfile1<<fieldbuffer<<endl;
 		
		//sentence_vect=splitEx(fieldbuffer,"������");
 		/*if (HParser_ParsTitle(fieldbuffer,outsent,outlen)==false)
 		{
 			write_log("PARSE ERROR:");
			outfile<<"<Title>="<<fieldbuffer<<endl;
			outfile<<"<PARSE>=error!"<<endl;
			::TPI_MoveNext( m_hRecordSet);
			continue;
		}*/
		//outfile<<"<Title>="<<fieldbuffer<<endl;
 		//outfile<<"<PARSE>="<<outsent<<endl;
		//strcpy_s(outsent,1024,"\0");
		//outlen=1024;
		//vector<string>::iterator itt_s=sentence_vect.begin();
      //while(itt_s!=sentence_vect.end())
    //{
		/*strcpy_s(outsent1,1024,"\0");
		outlen1=1024;
		outlen2=1024;
		strcpy_s(outsent2,1024,"\0");
		if(HParser_Seg(itt_s->c_str(),outsent1,outlen1)==false)
		{
			write_log("PARSE ERROR:");
			//outfile<<"<Title>="<<fieldbuffer<<endl;
			outfile1<<"<Seg>=error!"<<endl;
			
		}
		outfile1<<outsent1<<endl;
		string temp(itt_s->c_str());
		 long endpos[10240];
	    for(int i = 0; i < 10240; i++)
	    {
				endpos[i] = 0;
	    }
		short pos[10240];
			//HDICT handle = Dict_Load_New("wd.ptd", "\0");
			//textline = "";
		bool state = HS_Word_Sect((char*)temp.c_str(), endpos,temp.size() , handle3, 1, pos);
			 int label = 0;
			 string outsent="";
			 for(int i = 0; i < temp.size(); ++i)
			 {
				if(i != endpos[label])
				 {
					outfile2<<temp[i];
				 }
				else
				{
					outfile2<<'/'<<" "<<temp[i];
					++label;
				}
			}
			outfile2<<endl;*/
		   // outfile1<<itt_s->c_str()<<endl;
			//itt_s++;

	 // }
	 // sentence_vect.clear();
	 // outfile1<<endl;
	  //outfile2<<endl;
		//if(HParser_ParsTitle_S(outsent,outsent1,outlen1)==false)
		//{
		//	write_log("PARSE ERROR:");
		//	outfile<<"<Title>="<<fieldbuffer<<endl;
		//	outfile<<"<Tiltle_s>=error!"<<endl;
		//	::TPI_MoveNext( m_hRecordSet);
		//	continue;
		//}
		//outfile<<"<Tilte_s>="<<outsent1<<endl;
		//strcpy_s(outsent1,1024,"\0");
		//outlen1=1024;
		//if(HParser_ParsSent(fieldbuffer,outsent1,outlen1)==false)
		//{
		//	write_log("PARSE ERROR:");
		//	outfile<<"<Title>="<<fieldbuffer<<endl;
		//	outfile<<"<Parser>=error!"<<endl;
		//	::TPI_MoveNext( m_hRecordSet);
		//	continue;
		//}
		//outfile<<"<Parser>="<<outsent1<<endl;
 		//�����ѯ���������ļ�
      
 		
		::TPI_MoveNext( m_hRecordSet);
		continue;
 	}
     /*cout<<"ÿƪ���¾�������"<< allSent/iAcounter<<endl;*/
 	delete[] outsent1;
	delete[] outsent2;
    outsent1 = NULL;
	outsent2=NULL;
    outfile1.close();
	outfile2.close();
 
 	if (!HParser_UnInit())
 	{
 		cout<<"UNInit ERROR!"<<endl;
 		return false;
 	}
 
 	return true;
}

// bool KbaseTest::NewParseKbaseText(void)
//{
//	//��ȡ��Ҫȡ�õ��ֶ���
//	map<string,string> FieldMap;
//	if (!SetFieldName("FieldName.ini",FieldMap))
//	{
//		write_log("GetKbaseTextByFieldName GetKbaseTextByFieldName Error!");
//		return false;
//	}
//	//������ļ�
//	//���ӷ�������ַ192.168.100.215
//	cout<<"�򿪷�����:192.168.100.114"<<endl;
//    if ( !TPIServerInit("192.168.100.114"))
//    {
//        write_log("GetKbaseTextByFieldName TPIServerInit Error!");
//        return false;
//    }
//	//�����ѯ���ֶ�����,����ѯ���ݻ�����ݾ��
//	//cout<<"�����ݱ�:CJFD0608"<<endl;
//	cout<<"�����ݱ�:CJFD0608"<<endl;
//	string sSql_Name ="select * from ";
//	sSql_Name +="CJFD0608";
//	//sSql_Name +="DEFBASE_CJFD2008_B018";
//	double timehere;
//	long recornumhere;
//	if (!TPIRecordSetOpen(sSql_Name,timehere,recornumhere))
//	{
//		write_log("GetKbaseTextByFieldName TPIServerInit Error!");
//		return false;
//	}
//
//	//�䷨������ʼ����ʼ���䷨����
//    FixSegPos CfixSegPos;
//	if (!CfixSegPos.DictLoadInit())
//	{
//		cout<<"NEW SEGPOS INIT ERROR!"<<endl;
//		return false;
//	}
//    Parser *CnewParse = new Parser ;
//    if (!CnewParse->Init())
//    {
//        cout<<"NWE PARSER INIT ERROR!"<<endl;
//        return false;
//    }
//	/*char *outsent=new char[1024];
//	int outlen=1024;*/
//    string outsent;
//    cout<<"������Դ�ɹ���\n����ÿ����¼..."<<endl;
//	//����ÿ����¼
//	//�����¼��Ŀ
//	long iAcounter = 0;
//	long rec_num=0;
//	string str_num;
//	if (!SetRecNum("RecNum.ini",str_num,rec_num))
//	{
//		write_log("SetRecNum Error!");
//		return false;
//	}
//
//	//������ļ�
//	string filename="";
//	//sprintf(filename,"Text_GetFromKBase_%d.txt",rec_num);
//	filename="20121210_CJFD0608�¾䷨����\\20121210_Text_GetFromKBase_"+str_num+".txt";
//	ofstream outfile(filename.c_str());
//	if (!outfile) 
//	{
//		write_log("GetKbaseTextByFieldName outfile Error!");
//		return false;
//	}
//    vector<string> sentence_vect;//�洢��ȫ���ֶη־�֮������о���.
//    char fieldbuffer[1024*256];
//    long allSent=0;
//    if (TPI_Move(m_hRecordSet, rec_num ) != true)//��¼����λ��(������1��ʼ)
//    {
//        cout<<"Move record num error!"<<endl;
//        return false;
//    }
//    iAcounter=rec_num-1;
//    vector<string> v_seg;
//    vector<string> v_pos;
//	while ( !::TPI_IsEOF( m_hRecordSet )  )
//	{	
//		cout<<"�����¼: "<<(++iAcounter)/*<<endl*/;
//		bool bflg=false;
//		map<string,string>::iterator itt = FieldMap.begin();
//        
//		while (itt!=FieldMap.end()) 
//		{
//			
//			::memset( fieldbuffer,0x00,262144 );
//			long lRet(0);
//			lRet = ::TPI_GetFieldValueByName( m_hRecordSet,(itt->first).c_str(), fieldbuffer,/*1024*256*/ 262144);
//			if ( lRet< 0 )
//			{
//				cerr<<"��ȡ�ֶ� "<<(itt->first).c_str()<<" ʧ�ܣ�"<<endl;
//				bflg=true;
//				break;
//			}
//			(itt->second) = fieldbuffer;
//			++itt;
//		}
//		//����ĳһ�ضλ�ȡʧ�ܣ�������һ����¼����
//		if (bflg)
//        {
//            map<string,string>::iterator itt1 = FieldMap.begin();
//			while (itt1!=FieldMap.end()) 
//			{
//				(itt1->second) = "\0";
//				++itt1;
//			}
//			::TPI_MoveNext( m_hRecordSet);
//			continue;
//		}	
//		//�����ѯ���������ļ� 
//        outfile<<"<REC>="<<iAcounter<<"ƪ.\n";
//		map<string,string>::iterator itt2 = FieldMap.begin();
//		while (itt2!=FieldMap.end()) 
//		{
//            //outfile<<"<"<<(itt2->first)<<">="<<(itt2->second)<<"\n";
//            sentence_vect=SplitSentence(itt2->second);
//            if (sentence_vect.size()==0)
//            {
//                cout<<",������0,������"<<allSent<<endl;
//                (itt2->second) = "\0";
//                //sentence_vect.clear();
//                ++itt2;
//                break;
//            }
//			vector<string>::iterator itt_s=sentence_vect.begin();
//            int nSent=0;
//			while(itt_s!=sentence_vect.end())
//			{
//                ++nSent;
//				outfile<<"<SETENCE>="<<nSent<<"="<<itt_s->c_str()<<"\n";
//                outsent="";
//                v_seg.clear();
//                v_pos.clear();
//                if (!CfixSegPos.OutPutFixedSegPosUnit(itt_s->c_str(),v_seg,v_pos))
//                {
//                    ++itt_s;
//                    outfile<<"<NEWPARSE>="<<"error: new segposing error!\n";
//                    continue;
//                }
//                if (!CnewParse->ParsingInSegPosUnit(v_seg,v_pos,outsent))
//                {
//                    outsent = "<NEWPARSE>=error: new parsering error!";
//                    //continue;
//                }
//                else
//                {
//                    outsent = "<NEWPARSE>=" + outsent;
//                }
//                outfile<<outsent<<endl;
//				++itt_s;
//			}
//            
//            allSent+=nSent;
//            cout<<"  ������"<<nSent<<",������"<<allSent<<endl;
//			(itt2->second) = "\0";
//            sentence_vect.clear();
//			++itt2;
//		}
//        
//		//�жϻ�ȡ��¼������Ŀ
//        /*if (iAcounter == 2095)
//        {
//        break;
//        }*/
//		//������һ����¼�Ĵ���
//		::TPI_MoveNext( m_hRecordSet );
//	}
//    /*cout<<"ÿƪ���¾�������"<< allSent/iAcounter<<endl;*/
//	FieldMap.clear();
//    sentence_vect.clear();
//    outfile.close();
//
//    if (!CfixSegPos.DictUnloadUninit())
//    {
//        cout<<"NEW SEGPOS UNLOAD ERROR!"<<endl;
//        return false;
//    }
//    if (!CnewParse->Exit())
//    {
//        cout<<"NWE PARSER UNLOAD ERROR!"<<endl;
//        return false;
//    }
//    delete CnewParse;
//    CnewParse = NULL;
//	return true;
//}