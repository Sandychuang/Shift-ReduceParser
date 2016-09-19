#ifndef _NLPE_DLL_API_
#define _NLPE_DLL_API_

// Dict Handle Type
// ��ʾ���ⲿ�ľ��
typedef long HDICT;

// ��̬�ʵ����ýṹ
#ifndef _UTILITY_H 
#define _UTILITY_H
	enum FixType{PREFIX, POSTFIX};
	const int MAXLEN_WORD = 100;
	const int MAXLEN_ITEM = 10;
	typedef struct _unitType{
		char unit[MAXLEN_WORD+1]; //һ���ִʵ�Ԫ
		int flag;
		int width;
		}unitType; //�����Ҫ��ģʽ
	typedef struct _item{
		char item[MAXLEN_WORD + 1]; //һ����ѯ���
		void* anno;	//������Ϣָ��
		}RetItem; //һ���Ӷ�̬�ʵ䷵�صĺ�ѡ����
#endif

// �ִ��������

#ifdef WORDSEGMENTOR_EXPORTS
#define WORDSEGMENTOR_API __declspec(dllexport)
#else
#define WORDSEGMENTOR_API __declspec(dllimport)
#endif

// ȫ�ֺ���

#ifdef __cplusplus
extern "C"{
#endif
// ����
	// ÿ Max_Show ����ʾһ��
	// Ĭ�� 40
	// static int Max_Show = 40;

	#define POS_Num 47

	// ȫ�з����ô���
	extern WORDSEGMENTOR_API char* FST_POS[];

	//// ȫ�з����ô���
	//static char* FST_POS[] =
	//{	"Ag",	"a",	"ad",	"an",	"Bg",	"b",	"c",	"Dg",	"d",	"e",	"f",	// 0-10
	//	"g",	"h",	"i",	"j",	"k",	"l",	"Mg",	"m",	"Ng",	"n",			// 11-20
	//	"nr",	"ns",	"nt",	"nx",	"nz",	"o",	"p",	"Qg",	"q",	"Rg",			// 21-30
	//	"r",	"s",	"Tg",	"t",	"Ug",	"u",	"Vg",	"v",	"vd",	"vn",			// 31-40
	//	"w",	"x",	"Yg",	"y",	"z",	"zz"											// 41-46
	//};

	//// ȫ�з����ô��Զ�Ӧ����
	//static char* FST_POS_Name[] =
	//{	"������",	"���ݴ�",	"���δ�",	"���δ�",	"���������",	"�����",		// 0-5
	//	"����",		"������",	"����",		"̾��",		"��λ��",						// 6-10
	//	"����",		"ǰ�ӳɷ�",	"����",		"�������",	"��ӳɷ�",						// 11-15
	//	"ϰ����",	"��������",	"����",		"������",	"����",							// 16-20
	//	"����",		"����",		"��������",	"��������",	"����ר��",						// 21-25
	//	"������",	"���",		"��������",	"����",		"��������",						// 26-30
	//	"����",		"������",	"ʱ����",	"ʱ���",	"��������",						// 31-35
	//	"����",		"������",	"����",		"������",	"������",						// 36-40
	//	"������",	"��������",	"��������",	"������",	"״̬��",	"δ��¼��"			// 41-46
	//};

	// ���ô���
	enum CommonPOS
	{
		adj = 1,
		bdif = 5,
		adv = 8,
		numeral = 18,
		noun = 20,
		person = 21,
		place = 32,
		postime = 34,
		verb = 38,
		punctuation = 41,
		nbar = 47,
		vbar,
		abar,
		ap,
		dp,
		by,
		tp,
		sp,
		mp
	};

// �ⲿ�������

	// ���壺��ʼ��
	// ��ϸ������������
	// �ִ�ʱ��ֻ���������� Dict_Init��Dict_Init ������ñ�����
	WORDSEGMENTOR_API bool HS_Word_Sect_Init( void );

	// ����ȫ�з�����ʵ䣬method = 1��3
	WORDSEGMENTOR_API bool Word_Sect_FST_Load_Dicts();
	WORDSEGMENTOR_API bool Word_Sect_FST_Load_Dicts_Search();

	// ������󳤶�2KB
	extern WORDSEGMENTOR_API int sen_max_size;

	// ���壺�����Զ��ִ�
	// ��ϸ���������ı������Զ��ִʣ����������������
	// �������壺	srcBuf����ִʵ��ı�
	//				EndPos�����飬���ÿ���ʵĽ���λ��
	//				Size��srcBuf ���ֽ�����Ҳ�� EndPos ����Ĵ�С
	//				m_Dict���û��ʵ���
	//				Sect_Method���ִ����õķ�����Ĭ��Ϊ������󳤶�ƥ�䷽��
	//					0��������󳤶�ƥ�䷽�������ֱ���з֣�����ֱ�Ӻϲ���Ӣ��ֱ�Ӻϲ�
	//					1��ʹ��sys_general_dic��ȫ�зַ�����֧�ֺ���㡢���֡�Ӣ�ĵĴ�����������Ӣ�ġ������п�
	//					2��������󳤶�ƥ�䷽�������ֱ���з֣�����ֱ�Ӻϲ���Ӣ��ֱ�Ӻϲ�
	//					3��ʹ��sys_search_dic��ȫ�зַ��������ֱ���з֣�����ֱ�Ӻϲ���Ӣ��ֱ�Ӻϲ�
	//				POS�����飬���ÿ���ʵĴ��ԣ�Ĭ��Ϊ��
	//				Sen_Max_Len��������󳤶ȡ���ȫ�з��У�������ӳ���������ȣ��򲻴������ؾ��ӵĽ���λ��
	//				  ������󳤶ȵ�ԭ�򣺾��ӹ��������ܵ����ڴ����
	//				recog_phrase���Ƿ�ʶ����Ĭ��Ϊ��ʶ��ֻ��ȫ�з���Ч
	//				all_as_sep�����б�������Ϊ���ӵķָ�����trueʱ�ɴ����߷ִ��ٶȣ����޷��зֺ��б����ŵĴʣ��硰�����塷����
	// ����Ҫ��	srcBuf����Ϊ��
	//				EndPos�������С����Size��ÿ��Ԫ�س�ʼ��Ϊ0
	//				m_Dict���������󳤶�ƥ��ִʣ�Ҫ��ǿգ������ȫ�з֣�����Ϊ�գ�ֵΪ-1������ʱʹ��ϵͳ
	//				  �Դ��Ĵʵ䣬Ҳ���Էǿգ���ֻ֧��һ���ʵ�
	//				POS��ֻ��ȫ�зַ�����ʹ�ã������С��EndPos��С��ͬ��ÿ��Ԫ�س�ʼ��Ϊ-1
	//				Sen_Max_Len��ֻ��ȫ�зַ�����ʹ��
	// �������ݣ�	�з��Ƿ�ɹ�
	// ���ؿ���ֵ��	TRUE���ɹ�
	//				FALSE��ʧ��
	// ʹ��ǰ��Ҫ��	��ѡ��ȫ�з�
	//					1. HotStar��Ӧ����sys_general_dic��sys_dynamicprg_dic����ȫ�з�ר�ôʵ䣬��û�У������е���
	//					2. �������ڵ�һ�ηִ�ʱ�Զ�����sys_general_dic��sys_dynamicprg_dic�������м���
	//					3. �ִʳ�������Ŀ¼������÷ִ����ôʵ䣨����ר�ôʵ估�û��ʵ䣩��Ӧ�������ļ�
	//					   �����ļ��Ľ�����ʹ�����˵���ĵ�
	WORDSEGMENTOR_API bool HS_Word_Sect( char *srcBuf, long *EndPos, long Size, HDICT m_Dict, int Sect_Method = 0, 
		short *POS = NULL, int Sen_Max_Len = 2048, bool recog_phrase = false );

	// �з�Ӣ�ģ��ڴʵ�m_Dict�У�ʹ�ô��δ������󳤶�ƥ�䷽��������Ӣ�ĵ��ʻ����
	// Anno��Ž��ͣ����Ϊ�գ���ʾ�ʵ��޴˴������Ӧ����Ϊ�գ�Anno�Ĵ�СΪSize��ÿ��Ԫ�صĳ����ݶ�128���ֽ�
	// Size��srcBuf ���ֽ�����Ҳ�� EndPos ����Ĵ�С
	WORDSEGMENTOR_API bool HS_EN_Sect( char *srcBuf, long *EndPos, char **Anno, long Size, HDICT m_Dict );

	// �ҵ��ı��к��е�ÿ������
	// Size��srcBuf�Ĵ�С��BeginPos��EndPos�Ĵ�СҪ�����Size*10
	// ֻ֧��һ���ʵ�
	// �����ҵ��Ĵ�������0��ʾδ�ҵ����������
	WORDSEGMENTOR_API long HS_ALL_Sect( char *srcBuf, long *BeginPos, long *EndPos, long Size, HDICT m_Dict, int Sen_Max_Len = 2048 );
	
	WORDSEGMENTOR_API bool Set_Buf( char* text );

	WORDSEGMENTOR_API char* Get_POS( int Index );

	// ��ʾ�ִ�����ʱ��
	WORDSEGMENTOR_API void Show_Time();

	// ѧ�߿�����������Ŀ����Ӧ����
	#define AUTHOR_INFO_TYPE_NUM 29
	WORDSEGMENTOR_API char*  AUTHOR_INFO_TYPE_NLPE[];
	WORDSEGMENTOR_API char*  ORG_INFO_TYPE_NLPE[];
	//static char*  AUTHOR_INFO_TYPE_NLPE[] =
	//{	"����",		"�Ա�",		"����һ","�����",		"ѧ��",		"ѧλ",		"����ʱ��",	// 0-6
	//	"�о�����",	"��ϵ��ʽ",	"E-mail",	"ͨ�ŵ�ַ",	"�ʱ�",								// 7-11
	//	"�绰",		"��ҵѧУ",	"����",		"����",		"һ��������",						// 12-16
	//	"����������",	"����������",	"�ļ�������",	"��λ����ʡ",	"��λ���ڳ��е���",	// 17-21
	//	"��λ��ַ",	"��λ�ʱ�",	"רҵ����ְ��",	"ְ��",	"���ְ��"	,						// 22-26
	//	"����ְ��","��ҵ���",	"����ȫ��"													// 27-29
	//};

	//static char*  ORG_INFO_TYPE_NLPE[] =
	//{	"��ѧ",		"ѧԺ",		"ϵ",  "�о�Ժ",		"�о���",		"�о���",	//0-5
	//    "�о�����",	"ʵ����",	"������",	"ҽԺ",	     "��",					//6-10
	//	"���Ź�˾",	"ѧУ",		"��",   "����ȫ��",      "��У"	,    "ҽѧ��"  ,    //11-16
	//	"ͼ���",   "������",   "�����", "������",      "����",     "��" ,         //17-22
	//	 " "									
	//};

	// ���壺��ȡ������Ϣ
	// ��ϸ���������ı���ȡѧ�����������ֶ��й���Ϣ����ŵ���Ӧ�ṹ��
	// �������壺	author_info����ȡѧ�߿���Ϣ���ı�
	//				size��author_info���ֽ���
	//				begin_pos���������飬�����С����size�����ÿ������ȡ��Ϣ���ı��еĿ�ʼλ��
	//				end_pos���������飬�����С����size�����ÿ������ȡ��Ϣ���ı��еĽ���λ��
	//				type���������飬��С����size�����ÿ������ȡ��Ϣ������ֵ������ֵ��ӦAUTHOR_INFO_TYPE_NLPE�����е����
	//				exnum����ȡ������Ŀ��������
	// ����Ҫ��	author_info����Ϊ��
	// �������ݣ�	��ȡ��Ŀ
	// ���ؿ���ֵ��	0��δ��ȡ����Ϣ
	//				>0����ȡ����Ӧ��Ŀ����Ϣ
	WORDSEGMENTOR_API int Author_Info_Extract( char* author_info, int size,
		long *begin_pos, long *end_pos, short *type, int *exnum = NULL );

	// ���壺��ȡ����ȫ����Ϣ
	// ��ϸ���������ı���ȡѧ�����������ֶ��й���Ϣ����ŵ���Ӧ�ṹ��
	// �������壺	author_info����ȡѧ�߿���Ϣ���ı�
	//				size��author_info���ֽ���
	//				info���ַ������飬�����С����size��ÿ������Ԫ�ؼ��ַ�������=1024������1024����Ϣֻ����ǰ1000�ֽڣ������ÿ������ȡ��Ϣ
	//				type���������飬��С����size�����ÿ������ȡ��Ϣ������ֵ������ֵ��ӦAUTHOR_INFO_TYPE_NLPE�����е����
	//				confidence���������飬��С����size�����ÿ������ȡ��Ϣ�����Ŷ�
	//				sen_confidence���������Ŷ�
	// ����Ҫ��	author_info����Ϊ��
	// �������ݣ�	��ȡ��Ŀ
	// ���ؿ���ֵ��	0��δ��ȡ����Ϣ
	//				>0����ȡ����Ӧ��Ŀ����Ϣ
	WORDSEGMENTOR_API int Author_Info_Extract_All( char* author_info, int size,
		char** info, short* type, short* confidence, short &sen_confidence );

	// ���壺��ȡ���߼�����Ϣ
	// ��ϸ���������ı���ȡѧ�߼����й���Ϣ����ŵ���Ӧ�ṹ��
	// �������壺	resume_info����ȡѧ�߿���Ϣ���ı�
	//				size��resume_info���ֽ���
	//				begin_pos���������飬�����С����size�����ÿ������ȡ��Ϣ���ı��еĿ�ʼλ��
	//				end_pos���������飬�����С����size�����ÿ������ȡ��Ϣ���ı��еĽ���λ��
	//				type���������飬��С����size�����ÿ������ȡ��Ϣ������ֵ������ֵ��ӦAUTHOR_INFO_TYPE_NLPE�����е����
	//				exnum����ȡ������Ŀ��������
	// ����Ҫ��	resume_info����Ϊ��
	// �������ݣ�	��ȡ��Ŀ
	// ���ؿ���ֵ��	0��δ��ȡ����Ϣ
	//				>0����ȡ����Ӧ��Ŀ����Ϣ
	WORDSEGMENTOR_API int Resume_Info_Extract( char* resume_info, int size,
		long *begin_pos, long *end_pos, short *type, int *exnum = NULL );

	// ���壺��ȡ����ȫ��������Ϣ
	// ��ϸ���������ı���ȡѧ�߼����й���Ϣ����ŵ���Ӧ�ṹ��
	// �������壺	resume_info����ȡѧ�߿���Ϣ���ı�
	//				size��resume_info���ֽ���
	//				info���ַ������飬�����С����size��ÿ������Ԫ�ؼ��ַ�������=1024������1024����Ϣֻ����ǰ1000�ֽڣ������ÿ������ȡ��Ϣ
	//				type���������飬��С����size�����ÿ������ȡ��Ϣ������ֵ������ֵ��ӦAUTHOR_INFO_TYPE_NLPE�����е����
	//				confidence���������飬��С����size�����ÿ������ȡ��Ϣ�����Ŷ�
	//				sen_confidence���������Ŷ�
	//				exnum����ȡ������Ŀ��������
	// ����Ҫ��	resume_info����Ϊ��
	// �������ݣ�	��ȡ��Ŀ
	// ���ؿ���ֵ��	0��δ��ȡ����Ϣ
	//				>0����ȡ����Ӧ��Ŀ����Ϣ
	WORDSEGMENTOR_API int Resume_Info_Extract_All( char* author_info, int size,
		char** info, short* type, short* confidence, short &sen_confidence );

	// �õ�����ֵ��Ӧ����
	WORDSEGMENTOR_API char* Get_Author_Type_Name( int type );

	// �������������ַ�����Ӧ������
	// ��������������Ϣ( 0~AUTHOR_INFO_TYPE_NUM-1 )
	// ������-1������Ӧtype
	WORDSEGMENTOR_API int Get_Author_Type_Index( char* type );

#ifdef __cplusplus
}
#endif


// �ʵ��������

#ifdef DICT_EXPORTS
#define Dict_API __declspec(dllexport)
#else
#define Dict_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif
// �ⲿ�������
	// ��ʼ��HotStar��һЩ����
	// �Ƽ���һ������£�ֻ���� HS_Path ����
	// HS_Path ���ݿ�ϵͳ·��, һ����"..\\TPI\\server\\System"
	// Dict_Arr_Size �ʵ������С��Ĭ��100
	// Dict_Handle_Arr_Size �ʵ��������С��Ĭ��200
	// Hash_Size ��ϣ��Ĵ�С��Ĭ��524288��Ϊ�����������ýϴ�
	// HS_initialized��HotStar�Ƿ��ѳ�ʼ��
	//   һ�����Ϊ���ʵ������/4��
	// Max_Hash_Head_Len ��һ����ϣ��Ӧ�ַ�����󳤶ȣ�Ĭ��4��һ�㲻���޸�
	#define Default_Hash_Size 524288
	Dict_API bool Dict_Init(char* HS_Path, 
							const long Dict_Arr_Size = 100,
							const long Dict_Handle_Arr_Size = 200, 
							const long Hash_Size = Default_Hash_Size,
							const long Max_Hash_Head_Len = 4 );
	Dict_API bool Dict_Init_New(const long Dict_Arr_Size = 100,
							const long Dict_Handle_Arr_Size = 200, 
							const long Hash_Size = Default_Hash_Size,
							const long Max_Hash_Head_Len = 4 );

	// �½��ʵ�
	// DictPath	�ʵ䣨������·�� + �ʵ�(��)��
	// colsName	�����������, �����á�\0���ָ�������˫��\0������
	//  ���硰����\0��Ƶ\0\0��
	// return TRUE		�ɹ�, ����ʧ��
	// ע�⣺
	//  1. �½��ʵ�󲢲����ظôʵ䣬�������Dict_Load֮�����ʹ�øôʵ�
	//  2. ���ڵĴʵ䶼������HotStar�У����HotStar����ͬ���Ĵʵ䣬�½��ʵ������ʧ��
	//  3. ���ͬʱ��HotStar�ķ��������򣬻����ɾ���ʵ�A���޷��½��ʵ�A�����
	Dict_API bool Dict_Create_New(const char *DictPath,  const char *colsName);

	// ����ʵ�
	// ����ɹ�,����ֵӦ����0
	// @param	pszPath �����ڵ�·�� + ����
	// @return ��ľ��  >0˵��������ɹ���-1��ʾ����ʧ�ܣ�-2��ʾHotStar���Ѵ��������������
	Dict_API int Dict_Import(const char *pszPath);

	// ���壺���شʵ�
	// ��ϸ��������һ�������ʵ���ص��ڴ���
	// �������壺	DictPath���ʵ�·�������������ʵ��·���������ԡ�\0����Ϊ���һ������
	//				  ���磬Dict_Load( Dict1, Dict2, "\0" );
	// ����Ҫ��	DictPath���ʵ��� HotStar ���ݿ��еı��������Ǿ���·��
	// �������ݣ�	ָ������ʵ伯�ľ��
	// ���ؿ���ֵ��	>=0��ָ������ʵ伯�ľ��
	//				-1������ʧ��
	// ʹ��ǰ��Ҫ��	�ʵ���Ϲ淶
	Dict_API HDICT Dict_Load(char* DictPath, ... );
	Dict_API HDICT Dict_Load_New(char* DictPath, ... );

	//���壺�ͷ����пռ�
	//ʹ��˵��������ʹ��NLPE�ִ�ģ������Ҫ�ͷ�������Դ��ж�������еĴʵ��
	//���ñ���������ж�������дʵ䣬�������ͷ����пռ�
	Dict_API void Free_All_Space(void);

	// ���壺����ĳ���ʵ��Ӧ�������ļ�
	// ��ϸ������	��ĳ��HotStar�ʵ䣬ÿ��ȡһ�����������붯̬�ʵ䣨�ڴ棩��������Ϻ�ͳ�������Ϣ��
	//				����д�������ļ���Ȼ���ٴδ򿪸ôʵ䣬ÿ��ȡһ��������д�������Ϣ������������Ϣ��
	//				������ʽ���ԵĴ�������¼����־�У��˳�����ʾ������Ϣ
	//				��������޷����룬��ʱ��ֱ��д��-1
	// �������壺	dict_name���ʵ���
	//				mode���ʵ�ģʽ
	// ����Ҫ��	dict_name�����ôʵ������ı��ļ�ȫ·��
	//				mode��1~6
	//				ģʽ	������ʽ		����						���ȷ�Χ
	//				1		��				��HotStar���أ��������ļ�	��1~255�ֽ�
	//				2		��+TAB+��Ƶ+TAB	��HotStar���أ��������ļ�	��1~255�ֽ�
	//				3		��				���ı��ļ���ӣ��������ļ�	��1~1024�ֽ�
	//				4		��+TAB+��Ƶ		���ı��ļ����				��1~1024�ֽ�
	//				5		��+TAB+������Ϣ	���ı��ļ����				��1~1024�ֽڣ�������Ϣ1~102400�ֽ�
	//				6		��+TAB+������Ϣ	��HotStar���أ��������ļ�	��1~255�ֽڣ�������Ϣ1~255�ֽ�
	// �������ݣ��Ƿ񴴽��ɹ�
	// ���ؿ���ֵ��	TRUE���ɹ�
	//				FALSE��ʧ�ܣ�����ͬ����
	// ʹ��ǰ��Ҫ��	�������ļ��ٶȺ����������̨����
	//					���ѡģʽ1��2��Ҫ���ѳ�ʼ��HotStar
	Dict_API bool createIndex( char *dict_name, int mode = 2 );

	// �����Ƿ����ĳ���ʵ�
	Dict_API bool Dict_Search(char* DictPath);
	Dict_API int Get_Dict_Index( char* DictPath );
	Dict_API bool Get_Dict_Name(int index, char* path, int size);

	// ���壺�ڴʵ��в��Ҵ� Word�����ظ�����Ϣ
	// ��ϸ�������� m_Dict �в���Word�Ƿ���ڣ�������ڣ���Word�����ĸ�����Ϣ�ŵ� Item ��
	// �������壺	Word������ҵĴ�
	//				m_Dict���ʵ���
	//				Item�����ø�����Ϣ�����ϡ������ṩ������Ϣ˵����
	//				ItemLen��Item����󳤶�
	// ����Ҫ��	m_Dict��ָ��һ�����Ƕ�����ʵ�ľ��
	// �������ݣ��Ƿ�ȡ�ɹ�
	// ���ؿ���ֵ��	1���ɹ�
	//				0����Word�����ڣ���Word���ڵĴ������ȴ���ItemLen
	//				-1��m_Dict��ָ�����ʵ�ľ��ʱ����-1
	// ʹ��ǰ��Ҫ����
	// �Ż��������ֹ����ض�̬�滮�ʵ��ã���Ƹ�����Ϣ���ͣ�ʹ����ʶ��̬�滮����
	Dict_API int Dict_Search_Get_Info(char *Word, HDICT m_Dict, char *Item, int ItemLen);

	// ���壺�ڴʵ��в��Ҵ�word��������ڣ��򷵻ظ�����Ϣָ��
	// ��ϸ�������� m_Dict �в���Word�Ƿ���ڣ�������ڣ��򷵻ظ�����Ϣָ�룬���򷵻ؿ�
	// �������壺	word������ҵĴ�
	//				m_Dict���ʵ���
	// ����Ҫ��	m_Dict��ָ��һ�����Ƕ�����ʵ�ľ��
	// �������ݣ�	������Ϣָ��
	// ���ؿ���ֵ��	�ǿգ����ҳɹ�
	//				-1��m_Dict��ָ�����ʵ�ľ�����word������
	// ʹ��ǰ��Ҫ����
	Dict_API void* Dict_Search_Word_Get_Pointer( char* word, HDICT m_Dict );

	// ���壺�����Ƿ����ĳ����
	// ��ϸ����������m_Dictָ��Ĵʵ����Ƿ����ĳ����
	// �������壺	word������ҵĴ�
	//				m_Dict���ʵ���
	// ����Ҫ��	m_Dict��ָ��һ�����Ƕ�����ʵ�ľ��
	// �������ݣ��Ƿ���ڸô�
	// ���ؿ���ֵ��	TRUE������
	//				FALSE��������
	// ʹ��ǰ��Ҫ����
	Dict_API bool Dict_Search_Word( char* word, HDICT m_Dict );

	// ���壺������󳤶ȵĴ�
	// ��ϸ��������str��ͷ����ʼ�������Ƿ����m_Dictָ��Ĵʵ��е�ĳ����
	// �������壺	str������ҵ��ַ���
	//				m_Dict���ʵ���
	//				anno���õ�������Ϣ
	// ����Ҫ��	m_Dict��ָ��һ�������ʵ�ľ��
	// �������ݣ��Ƿ���ڸô�
	// ���ؿ���ֵ��	0��������
	//				>0���ôʵĳ���
	// ���÷�Χ������һ��������
	// ʹ��ǰ��Ҫ����
	Dict_API int Dict_Search_Max_Word( char* str, HDICT m_Dict, void*& anno );

	// ���壺�õ���ϣ����
	// ��ϸ����������ĳ���ʺ󣬵õ��ڴʵ��в��Ҹô������Ĺ�ϣ����
	// �������壺m_Dict���ʵ���
	// ����Ҫ��	m_Dict��ָ��һ�����Ƕ�����ʵ�ľ��
	// �������ݣ���ϣ����
	// ���ؿ���ֵ��1~Max_Hash_Times
	// ʹ��ǰ��Ҫ���Ѳ���ĳ����
	Dict_API int Dict_Get_Hash_Times( HDICT m_Dict );

	// �õ�ָ��һ�������ʵ�ļ��ϵľ�������������ʵ��·���������ԡ�\0��Ϊ���һ��������
	// ���磬Dict_Load( Dict1, Dict2, "\0" );
	// ���һ�������ʵ��Ѽ��أ��򷵻�ָ����Щ�ʵ��һ�����ϵľ�������δ���أ��򷵻�-1
	// ��� DictPath Ϊ�գ��򷵻�Ĭ�Ͼ������Ҫ������ǰ�Ľӿ��ã�
	Dict_API HDICT Dict_Get_Handle(char* DictPath, ... );

	// ɾ��ָ��һ�������ʵ�ļ��ϵľ��
	Dict_API bool Dict_Delete_Handle(HDICT hDict);

	// �õ���������ʹ�õľ��
	// hDict_arr�����÷��صľ������
	// size������hDict_arr�Ĵ�С
	// ��������ʹ�õľ��������
	// ����ֵ���ܴ���size����ʱ�Է���������hDict_arr��ֻ���size��
	Dict_API int Get_All_Handle( HDICT *hDict_arr, int size );

	// ��ѯһ���������Ӧ�����дʵ������
	// hDict������ѯ�ľ��
	// Dict_Name_arr���ʵ����Ƶ�����
	// arr_size������Dict_Name_arr�Ĵ�С
	// ���ؾ����Ӧ�ʵ������
	// ����ֵ���ܴ���size����ʱ�Է���������Dict_Name_arr��ֻ���size��
	Dict_API int Get_Handle_Dict( HDICT hDict, char Dict_Name_arr[][ MAX_PATH ], int arr_size );

	// ��������Ϣ
	// ������ļ��У��ļ��� NLPE_information.log�����ִ���ļ���DLL��ͬһ��Ŀ¼��
	// ��Ҫ������
	// ���Ƽ�ʹ��
	Dict_API void Handle_Output_Info();

	// ж�� m_Dict ָ���һ�������ʵ伯
	// ע�⣺����ж���û�ͬʱʹ��ĳ���ʵ䣬����֮ǰʹ����ôʵ��δж�أ���ôж�غ��������� false
	Dict_API bool Dict_Unload(HDICT m_Dict);
	Dict_API bool Dict_Unload_New(HDICT m_Dict);

	// ר�ú��������ģ�
	// ���ļ����ء���ѯ�ʵ�
	Dict_API bool Temp_Dict_LoadByFile( char* DictPath1, char* DictPath2 );
	Dict_API bool GetFileNamesByNode(const char* pLoadFileName, const char* pNodeFileName, char* pResult);

// �����Ƕ�̬�ʵ�ר�ú���

	// �½���̬�ʵ�
	Dict_API int Dyn_Dict_New( int inMode = 4 );

	// ���ض�̬�ʵ�
	Dict_API int Dyn_Dict_Load( char* DictPath, int inMode = 4 );

	// ��Ӵ���
	Dict_API int Dyn_Dict_Add_Item( char* Item, int Fre );
	Dict_API int Dyn_Dict_Add_Item2( char** Items, int Item_Num, void*& pAnno, bool Reverse = false );
	Dict_API int Dyn_Dict_Add_Item3( char* Item, char* Anno );

	// ��ѯ����
	Dict_API bool Dyn_Dict_Search_Item( char* Item, void*& Handle, int& Fre );
	// Reverse = false��������ʵ��в��ң�true���ڷ���ʵ��в���
	Dict_API bool Dyn_Dict_Search_Item2( char** Items, int Item_Num, void*& pAnno, bool Reverse = false );
	// ������ʵ��в���
	Dict_API bool Dyn_Dict_Search_Item3( char* Item, void*& pAnno );

	// ��ѯ��ĳ��λ�ÿ�ʼ�����дʵĽ���λ��
	Dict_API int Dyn_Dict_Search_Ends( char* String, int Begin, int* Ends, int* Fres, int Ends_Size );

	// ��ѯ��ĳЩ�ִʵ�ԪΪǰ׺�����д���
	// ֻ֧��4��5��6ģʽ
	Dict_API int Dyn_Dict_Fix_Search_Item(unitType* units, int unit_num, FixType fixtype, RetItem* items, int item_num, int join_num = 0);

	// ��ѯ�滻һ�������ִʵ�Ԫ������д���
	Dict_API int Dyn_Dict_Replace_Search_Item(unitType* units, int unit_num, RetItem* items, int item_num );

	// ��ѯ���һ�������ִʵ�Ԫ������д���
	// end_add=0��ĩβ����ӣ�end_add=1��ĩβ���һ���ִʵ�Ԫ
	Dict_API int Dyn_Dict_Insert_Search_Item(unitType* units, int unit_num, RetItem* items, int item_num, int end_add );

	// �޸Ĵ���Ƶ��
	Dict_API void Dyn_Dict_Change_Fre( void* Handle, int Fre );

	// ����ʵ�
	Dict_API int Dyn_Dict_Save( char* DictPath );

	// ��մʵ�
	Dict_API void Dyn_Dict_Clear();

#ifdef __cplusplus
}
#endif


// �����������

#ifdef CODE_EXPORTS
#define Code_API __declspec(dllexport)
#else
#define Code_API __declspec(dllimport)
#endif

	// �������ͣ�1 �C ASCII��2 �C GB2312��3 �C GB2312���⣬GBK���ڣ�
	// 4 �C GBK���⣬GB18030���ڣ�5 �C ������7 - ���ӷָ�����8 - ���֣�9 - Ӣ��
	// 10 - �����ӷָ���֮��ı�����
	#define ASCII	1
	#define GB2312	2
	#define GBK		3
	#define GB18030	4
	#define OTHER	5
	#define seperator	7
	#define number	8
	#define English	9
	#define PUNCTUATION	10

// ȫ�ֺ���

#ifdef __cplusplus
extern "C"{
#endif
// �ⲿ�������

	// SourceTxt ���жϱ�����ı�
	// EndPos ���飬���ÿ���ֵĽ���λ��
	// Type ���飬���ÿ���ֵı�������
	// size_in_char���ַ����ĳ��ȣ�����Ϊ��λ
	// det_other���Ƿ��ж�ASCII��GB2312�еķָ��������֡�Ӣ�ĺͱ�����
	// Ҫ�� Size �� SourceTxt ���ֽ������� EndPos ����� Type ����Ĵ�С
	// ���ؽ���У�EndPos��һ��0��β��Type��һ��-1��β����ʾ�ѵ������λ��
	// ���� SourceTxt Ϊ��Dell C640�ʼǱ����������м���һ�ո񣩣�Size Ϊ19
	// ���det_other=true
	// EndPosΪ	��4 5 6 9 11 13 15 17 19��
	// TypeΪ	��9 10 9 8 2 2 2 3 2 0 0 0 0 0��
	// ���det_other=false
	// EndPos Ϊ	��1 2 3 4 5 6 7 8 9 11 13 15 17 19 0 0 0 0 0��
	// Type Ϊ	��1 1 1 1 1 1 1 1 1 2 2 2 3 2 0 0 0 0 0��
	Code_API bool Code_Determine(char *SourceTxt, long *EndPos, unsigned char *Type, long Size, 
		long *size_in_char = NULL, bool det_other = true );

	// �õ�ĳ����
	// Size �Ǵ�� Word ���ַ����ĳ���
	Code_API bool Get_Word(char *SourceTxt, long *EndPos, long Index, char* Word, long Size);

	// �ж��Ƿ�ΪASCII��
	// ֻ�жϵ�һ���ַ�
	Code_API bool ASCII_Determine(const char* Word);

	// �ж��Ƿ�ΪGB2312��
	// ֻ�ж�ǰ�����ֽ�
	// Ҫ��Word���ȴ��ڵ���2
	Code_API bool GB2312_Determine(const char* Word);

	// �ж��Ƿ�ΪGBK��
	// ֻ�ж�ǰ�����ֽ�
	// Ҫ��Word���ȴ��ڵ���2
	Code_API bool GBK_Determine(const char* Word);

	// �ж��Ƿ�ΪGB18030��
	// ֻ�ж�ǰ�ĸ��ֽ�
	Code_API bool GB18030_Determine(const char* Word);

	// �Ƿ��Ƿֶε���Ӣ�ķ���
	Code_API bool IsParagraphSep(char *p);

	//// �س����ͻ��з�
	//static char *paraSep[] = {"\r","\n",NULL};

	// �жϾ��ӷָ���
	// Ĭ���ֳ���Ϊ0����ʱ�ж����г��ȵķָ���
	Code_API bool IsSentenceSep( char *character, int char_len = 0 );

	//// �־����Ӣ�ķ���
	//static char *sentence_sep[] = {
	//	"\t", "\n", "\r", "!", "?",
	//	"��", "��", "��", NULL};
	//static int sentence_sep_size = 8;

	//// ����Ϊ1�ķ־����
	//static char *sentence_sep_len1[] = { "\t", "\n", "\r", "!", "?", NULL};
	//static int sentence_sep_len1_size = 5;

	//// ����Ϊ2�ķ־����
	//static char *sentence_sep_len2[] = { "��", "��", "��", NULL};
	//static int sentence_sep_len2_size = 3;

	// �жϱ�����
	// Ĭ���ֳ���Ϊ0����ʱ�ж����г��ȵı�����
	Code_API bool IsPunctuation( char *character, int char_len = 0 );

	//// ������
	//static char *sentence_Punctuation[] = {
	//	"\t", "\n", "\r", " ", "!", "\"", "#", "$", "&", "'", "(", ")", "*", "+", 
	//	",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "\\", "]", "^", 
	//	"`", "{", "|", "}", "~",
	//	"�C", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", 
	//	"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	//	"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	//	"��", "��", "��", "��", "��", "��", "��",
	//		NULL};
	//static int sentence_Punctuation_size = 84;

	//// ����Ϊ1�ı�����
	//static char *sentence_Punctuation_len1[] = {
	//	"\t", "\n", "\r", " ", "!", "\"", "#", "$", "&", "'", "(", ")", "*", "+", 
	//	",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "\\", "]", "^", 
	//	"`", "{", "|", "}", "~",
	//		NULL};
	//static int sentence_Punctuation_len1_size = 34;

	//// ����Ϊ2�ı�����
	//static char *sentence_Punctuation_len2[] = { 
	//	"�C", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", 
	//	"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	//	"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	//	"��", "��", "��", "��", "��", "��", "��",
	//		NULL};
	//static int sentence_Punctuation_len2_size = 50;

	// �Ƿ���Ӣ����ĸ
	// ��ֻ���ж�ASCII��GB2312�е�Ӣ����ĸ
	// a-z,A-Z,�ᣭ����������
	// size=0ʱ�Զ��ж�p�ĳ���
	Code_API bool IsEnglishLetter(char *p, int size=0);

	// �Ƿ��Ǵ�дӢ����ĸ
	// ��ֻ���ж�ASCII��GB2312�е�Ӣ����ĸ
	Code_API bool IsEnglishCapitalLetter(char *p);

	// �Ƿ���Ӣ�ĵ���
	// ֻ�жϵ�һ��Ӣ�ĵ���
	// ��ֻ���ж�ASCII��GB2312�е�Ӣ����ĸ
	Code_API bool HasEnglishLetter(char *p);

	// �Ƿ������ִ�д��Ӣ�ĵ���
	// ֻ�жϵ�һ��Ӣ�ĵ���
	// ��ֻ���ж�ASCII��GB2312�е�Ӣ����ĸ
	Code_API bool HasEnglishCapitalWord(char *p);

	// �Ƿ���ȫ����д��Ӣ�ĵ���
	// ֻ�жϵ�һ��Ӣ�ĵ���
	// ��ֻ���ж�ASCII��GB2312�е�Ӣ����ĸ
	Code_API bool AllEnglishCapitalWord(char *p);

	// ת��Ϊ��д��ĸ
	// ��ֻ���ж�ASCII��GB2312�е�Ӣ����ĸ
	Code_API void ConvertEnglishCapitalWord(char *Src, char *Res, long Size);

	// �Ƿ�������
	// ��ֻ���ж�ASCII��GB2312�е�����
	// 0-9,������
	Code_API bool IsNumber(char *p, int size=0);
	//static char *ChineseNum[] = {"һ","��","��","��","��","��","��","��","��",NULL};

	// �Ƿ�Ӣ��ͣ�ô�
	// �ݿ�
	Code_API bool IsWordInStopDict(char *p);

	// ��������������ʵ��
	// ���� bool	IsGenericCChar(char *p);    /// �Ƿ��������ַ�

#ifdef __cplusplus
}
#endif

// �����������

#ifdef RULE_EXPORTS
#define Rule_API __declspec(dllexport)
#else
#define Rule_API __declspec(dllimport)
#endif

// ȫ�ֺ���

#ifdef __cplusplus
extern "C"{
#endif
// �ⲿ�������

	// ���ع���
	// RuleSetPath ����·��
	// ����ֻ�ܴ�HotStar���������
	Rule_API bool Load_RuleSet(char* RuleSetPath);

	// ƥ�����
	Rule_API bool Match_Rule( char* Rule, char* Seperator );

	// ���ݹ�������滻
	// ���罫���ᡱ�滻Ϊ��a��
	// ��ֻ�ܴ���A=B�����滻���
	// ��ֻ�ܴ����ַ����滻���
	// �ɴ���A=�����滻�������ɾ��A
	Rule_API void Match_Rule_Convert( char* Content, char* Converted );

	// ж�ع���
	// RuleSetPath ����·��
	Rule_API bool Unload_RuleSet(char* RuleSetPath);

#ifdef __cplusplus
}
#endif

#endif	///  _NLPE_DLL_API_
