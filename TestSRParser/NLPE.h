#ifndef _NLPE_DLL_API_
#define _NLPE_DLL_API_

// Dict Handle Type
// 显示给外部的句柄
typedef long HDICT;

// 动态词典所用结构
#ifndef _UTILITY_H 
#define _UTILITY_H
	enum FixType{PREFIX, POSTFIX};
	const int MAXLEN_WORD = 100;
	const int MAXLEN_ITEM = 10;
	typedef struct _unitType{
		char unit[MAXLEN_WORD+1]; //一个分词单元
		int flag;
		int width;
		}unitType; //传入的要求模式
	typedef struct _item{
		char item[MAXLEN_WORD + 1]; //一个查询结果
		void* anno;	//附加信息指针
		}RetItem; //一个从动态词典返回的候选词组
#endif

// 分词输出函数

#ifdef WORDSEGMENTOR_EXPORTS
#define WORDSEGMENTOR_API __declspec(dllexport)
#else
#define WORDSEGMENTOR_API __declspec(dllimport)
#endif

// 全局函数

#ifdef __cplusplus
extern "C"{
#endif
// 变量
	// 每 Max_Show 句显示一次
	// 默认 40
	// static int Max_Show = 40;

	#define POS_Num 47

	// 全切分所用词性
	extern WORDSEGMENTOR_API char* FST_POS[];

	//// 全切分所用词性
	//static char* FST_POS[] =
	//{	"Ag",	"a",	"ad",	"an",	"Bg",	"b",	"c",	"Dg",	"d",	"e",	"f",	// 0-10
	//	"g",	"h",	"i",	"j",	"k",	"l",	"Mg",	"m",	"Ng",	"n",			// 11-20
	//	"nr",	"ns",	"nt",	"nx",	"nz",	"o",	"p",	"Qg",	"q",	"Rg",			// 21-30
	//	"r",	"s",	"Tg",	"t",	"Ug",	"u",	"Vg",	"v",	"vd",	"vn",			// 31-40
	//	"w",	"x",	"Yg",	"y",	"z",	"zz"											// 41-46
	//};

	//// 全切分所用词性对应名称
	//static char* FST_POS_Name[] =
	//{	"形语素",	"形容词",	"副形词",	"名形词",	"区别词语素",	"区别词",		// 0-5
	//	"连词",		"副语素",	"副词",		"叹词",		"方位词",						// 6-10
	//	"语素",		"前接成分",	"成语",		"简称略语",	"后接成分",						// 11-15
	//	"习用语",	"数词语素",	"数词",		"名语素",	"名词",							// 16-20
	//	"人名",		"地名",		"机构团体",	"其他名词",	"其他专名",						// 21-25
	//	"拟声词",	"介词",		"量词语素",	"量词",		"代词语素",						// 26-30
	//	"代词",		"处所词",	"时语素",	"时间词",	"助词语素",						// 31-35
	//	"助词",		"动语素",	"动词",		"副动词",	"名动词",						// 36-40
	//	"标点符号",	"非语素字",	"语气语素",	"语气词",	"状态词",	"未登录词"			// 41-46
	//};

	// 常用词性
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

// 外部输出函数

	// 含义：初始化
	// 详细操作：创建锁
	// 分词时，只需调用下面的 Dict_Init，Dict_Init 即会调用本函数
	WORDSEGMENTOR_API bool HS_Word_Sect_Init( void );

	// 加载全切分所需词典，method = 1、3
	WORDSEGMENTOR_API bool Word_Sect_FST_Load_Dicts();
	WORDSEGMENTOR_API bool Word_Sect_FST_Load_Dicts_Search();

	// 句子最大长度2KB
	extern WORDSEGMENTOR_API int sen_max_size;

	// 含义：汉语自动分词
	// 详细操作：对文本进行自动分词，将结果放在数组中
	// 参数含义：	srcBuf，需分词的文本
	//				EndPos，数组，存放每个词的结束位置
	//				Size，srcBuf 的字节数，也是 EndPos 数组的大小
	//				m_Dict，用户词典句柄
	//				Sect_Method，分词所用的方法，默认为正向最大长度匹配方法
	//					0，正向最大长度匹配方法，标点直接切分，数字直接合并，英文直接合并
	//					1，使用sys_general_dic的全切分方法，支持含标点、数字、英文的词条，其他的英文、数字切开
	//					2，反向最大长度匹配方法，标点直接切分，数字直接合并，英文直接合并
	//					3，使用sys_search_dic的全切分方法，标点直接切分，数字直接合并，英文直接合并
	//				POS，数组，存放每个词的词性，默认为空
	//				Sen_Max_Len，句子最大长度。在全切分中，如果句子超过这个长度，则不处理，返回句子的结束位置
	//				  限制最大长度的原因：句子过长，可能导致内存过大
	//				recog_phrase，是否识别短语，默认为不识别，只对全切分有效
	//				all_as_sep，所有标点符号作为句子的分隔符（true时可大辐提高分词速度，但无法切分含有标点符号的词，如“《彷徨》”）
	// 参数要求：	srcBuf，不为空
	//				EndPos，数组大小等于Size，每个元素初始化为0
	//				m_Dict，如果是最大长度匹配分词，要求非空；如果是全切分，可以为空（值为-1），此时使用系统
	//				  自带的词典，也可以非空，但只支持一个词典
	//				POS，只在全切分方法中使用，数组大小与EndPos大小相同，每个元素初始化为-1
	//				Sen_Max_Len，只在全切分方法中使用
	// 返回内容：	切分是否成功
	// 返回可能值：	TRUE，成功
	//				FALSE，失败
	// 使用前提要求：	若选择全切分
	//					1. HotStar中应存在sys_general_dic和sys_dynamicprg_dic两个全切分专用词典，如没有，需自行导入
	//					2. 函数将在第一次分词时自动加载sys_general_dic和sys_dynamicprg_dic，勿自行加载
	//					3. 分词程序所在目录下需放置分词所用词典（两个专用词典及用户词典）对应的索引文件
	//					   索引文件的建立和使用详见说明文档
	WORDSEGMENTOR_API bool HS_Word_Sect( char *srcBuf, long *EndPos, long Size, HDICT m_Dict, int Sect_Method = 0, 
		short *POS = NULL, int Sen_Max_Len = 2048, bool recog_phrase = false );

	// 切分英文，在词典m_Dict中，使用词形处理和最大长度匹配方法，查找英文单词或词组
	// Anno存放解释，如果为空，表示词典无此词条或对应解释为空，Anno的大小为Size，每个元素的长度暂定128个字节
	// Size，srcBuf 的字节数，也是 EndPos 数组的大小
	WORDSEGMENTOR_API bool HS_EN_Sect( char *srcBuf, long *EndPos, char **Anno, long Size, HDICT m_Dict );

	// 找到文本中含有的每个词条
	// Size是srcBuf的大小，BeginPos和EndPos的大小要求等于Size*10
	// 只支持一个词典
	// 返回找到的词条数，0表示未找到或参数有误
	WORDSEGMENTOR_API long HS_ALL_Sect( char *srcBuf, long *BeginPos, long *EndPos, long Size, HDICT m_Dict, int Sen_Max_Len = 2048 );
	
	WORDSEGMENTOR_API bool Set_Buf( char* text );

	WORDSEGMENTOR_API char* Get_POS( int Index );

	// 显示分词所用时间
	WORDSEGMENTOR_API void Show_Time();

	// 学者库所用类型数目、对应名称
	#define AUTHOR_INFO_TYPE_NUM 29
	WORDSEGMENTOR_API char*  AUTHOR_INFO_TYPE_NLPE[];
	WORDSEGMENTOR_API char*  ORG_INFO_TYPE_NLPE[];
	//static char*  AUTHOR_INFO_TYPE_NLPE[] =
	//{	"姓名",		"性别",		"籍贯一","籍贯二",		"学历",		"学位",		"出生时间",	// 0-6
	//	"研究方向",	"联系方式",	"E-mail",	"通信地址",	"邮编",								// 7-11
	//	"电话",		"毕业学校",	"党派",		"民族",		"一级机构名",						// 12-16
	//	"二级机构名",	"三级机构名",	"四级机构名",	"单位所在省",	"单位所在城市地区",	// 17-21
	//	"单位网址",	"单位邮编",	"专业技术职称",	"职务",	"社会职务"	,						// 22-26
	//	"特殊职务","毕业年份",	"机构全称"													// 27-29
	//};

	//static char*  ORG_INFO_TYPE_NLPE[] =
	//{	"大学",		"学院",		"系",  "研究院",		"研究所",		"研究室",	//0-5
	//    "研究中心",	"实验室",	"教研室",	"医院",	     "科",					//6-10
	//	"集团公司",	"学校",		"局",   "机构全称",      "党校"	,    "医学部"  ,    //11-16
	//	"图书馆",   "档案馆",   "博物馆", "出版社",      "油田",     "部" ,         //17-22
	//	 " "									
	//};

	// 含义：提取作者信息
	// 详细操作：对文本抽取学者姓名机构字段有关信息，存放到相应结构中
	// 参数含义：	author_info，抽取学者库信息的文本
	//				size，author_info的字节数
	//				begin_pos，整型数组，数组大小等于size，存放每个所抽取信息在文本中的开始位置
	//				end_pos，整型数组，数组大小等于size，存放每个所抽取信息在文本中的结束位置
	//				type，整型数组，大小等于size，存放每个所抽取信息的类型值。类型值对应AUTHOR_INFO_TYPE_NLPE数组中的序号
	//				exnum，抽取出的数目，调试用
	// 参数要求：	author_info，不为空
	// 返回内容：	抽取数目
	// 返回可能值：	0，未抽取出信息
	//				>0，抽取出相应数目的信息
	WORDSEGMENTOR_API int Author_Info_Extract( char* author_info, int size,
		long *begin_pos, long *end_pos, short *type, int *exnum = NULL );

	// 含义：提取作者全部信息
	// 详细操作：对文本抽取学者姓名机构字段有关信息，存放到相应结构中
	// 参数含义：	author_info，抽取学者库信息的文本
	//				size，author_info的字节数
	//				info，字符串数组，数组大小等于size，每个数组元素即字符串长度=1024（超过1024的信息只保存前1000字节），存放每个所抽取信息
	//				type，整型数组，大小等于size，存放每个所抽取信息的类型值。类型值对应AUTHOR_INFO_TYPE_NLPE数组中的序号
	//				confidence，整型数组，大小等于size，存放每个所抽取信息的置信度
	//				sen_confidence，句子置信度
	// 参数要求：	author_info，不为空
	// 返回内容：	抽取数目
	// 返回可能值：	0，未抽取出信息
	//				>0，抽取出相应数目的信息
	WORDSEGMENTOR_API int Author_Info_Extract_All( char* author_info, int size,
		char** info, short* type, short* confidence, short &sen_confidence );

	// 含义：提取作者简历信息
	// 详细操作：对文本抽取学者简历有关信息，存放到相应结构中
	// 参数含义：	resume_info，抽取学者库信息的文本
	//				size，resume_info的字节数
	//				begin_pos，整型数组，数组大小等于size，存放每个所抽取信息在文本中的开始位置
	//				end_pos，整型数组，数组大小等于size，存放每个所抽取信息在文本中的结束位置
	//				type，整型数组，大小等于size，存放每个所抽取信息的类型值。类型值对应AUTHOR_INFO_TYPE_NLPE数组中的序号
	//				exnum，抽取出的数目，调试用
	// 参数要求：	resume_info，不为空
	// 返回内容：	抽取数目
	// 返回可能值：	0，未抽取出信息
	//				>0，抽取出相应数目的信息
	WORDSEGMENTOR_API int Resume_Info_Extract( char* resume_info, int size,
		long *begin_pos, long *end_pos, short *type, int *exnum = NULL );

	// 含义：提取作者全部简历信息
	// 详细操作：对文本抽取学者简历有关信息，存放到相应结构中
	// 参数含义：	resume_info，抽取学者库信息的文本
	//				size，resume_info的字节数
	//				info，字符串数组，数组大小等于size，每个数组元素即字符串长度=1024（超过1024的信息只保存前1000字节），存放每个所抽取信息
	//				type，整型数组，大小等于size，存放每个所抽取信息的类型值。类型值对应AUTHOR_INFO_TYPE_NLPE数组中的序号
	//				confidence，整型数组，大小等于size，存放每个所抽取信息的置信度
	//				sen_confidence，句子置信度
	//				exnum，抽取出的数目，调试用
	// 参数要求：	resume_info，不为空
	// 返回内容：	抽取数目
	// 返回可能值：	0，未抽取出信息
	//				>0，抽取出相应数目的信息
	WORDSEGMENTOR_API int Resume_Info_Extract_All( char* author_info, int size,
		char** info, short* type, short* confidence, short &sen_confidence );

	// 得到类型值对应名称
	WORDSEGMENTOR_API char* Get_Author_Type_Name( int type );

	// 返回作者类型字符串对应的索引
	// 用于生成类型信息( 0~AUTHOR_INFO_TYPE_NUM-1 )
	// 若返回-1则无相应type
	WORDSEGMENTOR_API int Get_Author_Type_Index( char* type );

#ifdef __cplusplus
}
#endif


// 词典输出函数

#ifdef DICT_EXPORTS
#define Dict_API __declspec(dllexport)
#else
#define Dict_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"{
#endif
// 外部输出函数
	// 初始化HotStar及一些参数
	// 推荐：一般情况下，只设置 HS_Path 即可
	// HS_Path 数据库系统路径, 一般是"..\\TPI\\server\\System"
	// Dict_Arr_Size 词典数组大小，默认100
	// Dict_Handle_Arr_Size 词典句柄数组大小，默认200
	// Hash_Size 哈希表的大小，默认524288，为保险起见，设得较大
	// HS_initialized，HotStar是否已初始化
	//   一般可设为（词典词条数/4）
	// Max_Hash_Head_Len 词一级哈希对应字符串最大长度，默认4。一般不用修改
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

	// 新建词典
	// DictPath	词典（表）保存路径 + 词典(表)名
	// colsName	表的所有列名, 依次用’\0’分隔，并用双’\0’结束
	//  例如“词条\0词频\0\0”
	// return TRUE		成功, 否则失败
	// 注意：
	//  1. 新建词典后并不加载该词典，必须调用Dict_Load之后才能使用该词典
	//  2. 现在的词典都存在在HotStar中，如果HotStar中有同名的词典，新建词典操作将失败
	//  3. 如果同时打开HotStar的服务器程序，会造成删除词典A后，无法新建词典A的情况
	Dict_API bool Dict_Create_New(const char *DictPath,  const char *colsName);

	// 引入词典
	// 如果成功,返回值应大于0
	// @param	pszPath 表所在的路径 + 表名
	// @return 表的句柄  >0说明表引入成功，-1表示引入失败，-2表示HotStar中已存在这个表，不引入
	Dict_API int Dict_Import(const char *pszPath);

	// 含义：加载词典
	// 详细操作：将一个或多个词典加载到内存中
	// 参数含义：	DictPath，词典路径，可输入多个词典的路径。必须以”\0”作为最后一个参数
	//				  例如，Dict_Load( Dict1, Dict2, "\0" );
	// 参数要求：	DictPath，词典在 HotStar 数据库中的表名，不是绝对路径
	// 返回内容：	指向这个词典集的句柄
	// 返回可能值：	>=0，指向这个词典集的句柄
	//				-1，加载失败
	// 使用前提要求：	词典符合规范
	Dict_API HDICT Dict_Load(char* DictPath, ... );
	Dict_API HDICT Dict_Load_New(char* DictPath, ... );

	//含义：释放所有空间
	//使用说明：结束使用NLPE分词模块后，如果要释放所有资源，卸载完所有的词典后，
	//调用本函数，光卸载完所有词典，并不能释放所有空间
	Dict_API void Free_All_Space(void);

	// 含义：建立某个词典对应的索引文件
	// 详细操作：	打开某个HotStar词典，每次取一个词条，插入动态词典（内存），插入完毕后，统计相关信息，
	//				将其写入索引文件，然后再次打开该词典，每次取一个词条，写入相关信息（包括附加信息）
	//				遇到格式不对的词条，记录到日志中，退出，显示出错信息
	//				允许词条无法插入，此时，直接写入-1
	// 参数含义：	dict_name，词典名
	//				mode，词典模式
	// 参数要求：	dict_name，所用词典名或文本文件全路径
	//				mode，1~6
	//				模式	词条格式		加载						长度范围
	//				1		词				从HotStar加载，需索引文件	词1~255字节
	//				2		词+TAB+词频+TAB	从HotStar加载，需索引文件	词1~255字节
	//				3		词				从文本文件添加，需索引文件	词1~1024字节
	//				4		词+TAB+词频		从文本文件添加				词1~1024字节
	//				5		词+TAB+附加信息	从文本文件添加				词1~1024字节，附加信息1~102400字节
	//				6		词+TAB+附加信息	从HotStar加载，需索引文件	词1~255字节，附加信息1~255字节
	// 返回内容：是否创建成功
	// 返回可能值：	TRUE，成功
	//				FALSE，失败，有相同词条
	// 使用前提要求：	建索引文件速度很慢，建议后台进行
	//					如果选模式1或2，要求已初始化HotStar
	Dict_API bool createIndex( char *dict_name, int mode = 2 );

	// 查找是否存在某个词典
	Dict_API bool Dict_Search(char* DictPath);
	Dict_API int Get_Dict_Index( char* DictPath );
	Dict_API bool Get_Dict_Name(int index, char* path, int size);

	// 含义：在词典中查找词 Word，返回附加信息
	// 详细操作：在 m_Dict 中查找Word是否存在，如果存在，将Word词条的附加信息放到 Item 中
	// 参数含义：	Word，需查找的词
	//				m_Dict，词典句柄
	//				Item，放置附加信息，符合“对外提供附加信息说明”
	//				ItemLen，Item的最大长度
	// 参数要求：	m_Dict，指向一个（非多个）词典的句柄
	// 返回内容：是否取成功
	// 返回可能值：	1，成功
	//				0，词Word不存在，或Word所在的词条长度大于ItemLen
	//				-1，m_Dict是指向多个词典的句柄时返回-1
	// 使用前提要求：无
	// 优化方案：现供加载动态规划词典用，设计附加信息类型，使其能识别动态规划词条
	Dict_API int Dict_Search_Get_Info(char *Word, HDICT m_Dict, char *Item, int ItemLen);

	// 含义：在词典中查找词word，如果存在，则返回附加信息指针
	// 详细操作：在 m_Dict 中查找Word是否存在，如果存在，则返回附加信息指针，否则返回空
	// 参数含义：	word，需查找的词
	//				m_Dict，词典句柄
	// 参数要求：	m_Dict，指向一个（非多个）词典的句柄
	// 返回内容：	附加信息指针
	// 返回可能值：	非空，查找成功
	//				-1，m_Dict是指向多个词典的句柄或词word不存在
	// 使用前提要求：无
	Dict_API void* Dict_Search_Word_Get_Pointer( char* word, HDICT m_Dict );

	// 含义：查找是否存在某个词
	// 详细操作：查找m_Dict指向的词典中是否存在某个词
	// 参数含义：	word，需查找的词
	//				m_Dict，词典句柄
	// 参数要求：	m_Dict，指向一个（非多个）词典的句柄
	// 返回内容：是否存在该词
	// 返回可能值：	TRUE，存在
	//				FALSE，不存在
	// 使用前提要求：无
	Dict_API bool Dict_Search_Word( char* word, HDICT m_Dict );

	// 含义：查找最大长度的词
	// 详细操作：从str的头部开始，查找是否存在m_Dict指向的词典中的某个词
	// 参数含义：	str，需查找的字符串
	//				m_Dict，词典句柄
	//				anno，得到附加信息
	// 参数要求：	m_Dict，指向一个或多个词典的句柄
	// 返回内容：是否存在该词
	// 返回可能值：	0，不存在
	//				>0，该词的长度
	// 适用范围：查找一级机构名
	// 使用前提要求：无
	Dict_API int Dict_Search_Max_Word( char* str, HDICT m_Dict, void*& anno );

	// 含义：得到哈希次数
	// 详细操作：查找某个词后，得到在词典中查找该词所花的哈希次数
	// 参数含义：m_Dict，词典句柄
	// 参数要求：	m_Dict，指向一个（非多个）词典的句柄
	// 返回内容：哈希次数
	// 返回可能值：1~Max_Hash_Times
	// 使用前提要求：已查找某个词
	Dict_API int Dict_Get_Hash_Times( HDICT m_Dict );

	// 得到指向一个或多个词典的集合的句柄，可输入多个词典的路径。必须以”\0”为最后一个参数。
	// 例如，Dict_Load( Dict1, Dict2, "\0" );
	// 如果一个或多个词典已加载，则返回指向这些词典的一个集合的句柄，如果未加载，则返回-1
	// 如果 DictPath 为空，则返回默认句柄（主要延续以前的接口用）
	Dict_API HDICT Dict_Get_Handle(char* DictPath, ... );

	// 删除指向一个或多个词典的集合的句柄
	Dict_API bool Dict_Delete_Handle(HDICT hDict);

	// 得到所有正在使用的句柄
	// hDict_arr，放置返回的句柄数组
	// size，数组hDict_arr的大小
	// 返回正在使用的句柄的总数
	// 返回值可能大于size，此时仍返回总数，hDict_arr里只存放size个
	Dict_API int Get_All_Handle( HDICT *hDict_arr, int size );

	// 查询一个句柄所对应的所有词典的名称
	// hDict，所查询的句柄
	// Dict_Name_arr，词典名称的数组
	// arr_size，数组Dict_Name_arr的大小
	// 返回句柄对应词典的总数
	// 返回值可能大于size，此时仍返回总数，Dict_Name_arr里只存放size个
	Dict_API int Get_Handle_Dict( HDICT hDict, char Dict_Name_arr[][ MAX_PATH ], int arr_size );

	// 输出句柄信息
	// 输出到文件中，文件名 NLPE_information.log，与可执行文件和DLL在同一个目录下
	// 主要调试用
	// 不推荐使用
	Dict_API void Handle_Output_Info();

	// 卸载 m_Dict 指向的一个或多个词典集
	// 注意：如果有多个用户同时使用某个词典，或者之前使用完该词典后未卸载，那么卸载函数将返回 false
	Dict_API bool Dict_Unload(HDICT m_Dict);
	Dict_API bool Dict_Unload_New(HDICT m_Dict);

	// 专用函数（韩文）
	// 从文件加载、查询词典
	Dict_API bool Temp_Dict_LoadByFile( char* DictPath1, char* DictPath2 );
	Dict_API bool GetFileNamesByNode(const char* pLoadFileName, const char* pNodeFileName, char* pResult);

// 以下是动态词典专用函数

	// 新建动态词典
	Dict_API int Dyn_Dict_New( int inMode = 4 );

	// 加载动态词典
	Dict_API int Dyn_Dict_Load( char* DictPath, int inMode = 4 );

	// 添加词条
	Dict_API int Dyn_Dict_Add_Item( char* Item, int Fre );
	Dict_API int Dyn_Dict_Add_Item2( char** Items, int Item_Num, void*& pAnno, bool Reverse = false );
	Dict_API int Dyn_Dict_Add_Item3( char* Item, char* Anno );

	// 查询词条
	Dict_API bool Dyn_Dict_Search_Item( char* Item, void*& Handle, int& Fre );
	// Reverse = false，在正向词典中查找，true，在反向词典中查找
	Dict_API bool Dyn_Dict_Search_Item2( char** Items, int Item_Num, void*& pAnno, bool Reverse = false );
	// 在正向词典中查找
	Dict_API bool Dyn_Dict_Search_Item3( char* Item, void*& pAnno );

	// 查询从某个位置开始的所有词的结束位置
	Dict_API int Dyn_Dict_Search_Ends( char* String, int Begin, int* Ends, int* Fres, int Ends_Size );

	// 查询以某些分词单元为前缀的所有词条
	// 只支持4、5、6模式
	Dict_API int Dyn_Dict_Fix_Search_Item(unitType* units, int unit_num, FixType fixtype, RetItem* items, int item_num, int join_num = 0);

	// 查询替换一个或多个分词单元后的所有词条
	Dict_API int Dyn_Dict_Replace_Search_Item(unitType* units, int unit_num, RetItem* items, int item_num );

	// 查询添加一个或多个分词单元后的所有词条
	// end_add=0，末尾不添加，end_add=1，末尾添加一个分词单元
	Dict_API int Dyn_Dict_Insert_Search_Item(unitType* units, int unit_num, RetItem* items, int item_num, int end_add );

	// 修改词条频度
	Dict_API void Dyn_Dict_Change_Fre( void* Handle, int Fre );

	// 保存词典
	Dict_API int Dyn_Dict_Save( char* DictPath );

	// 清空词典
	Dict_API void Dyn_Dict_Clear();

#ifdef __cplusplus
}
#endif


// 编码输出函数

#ifdef CODE_EXPORTS
#define Code_API __declspec(dllexport)
#else
#define Code_API __declspec(dllimport)
#endif

	// 编码类型：1 – ASCII；2 – GB2312；3 – GB2312以外，GBK以内；
	// 4 – GBK以外，GB18030以内；5 – 其他；7 - 句子分隔符；8 - 数字；9 - 英文
	// 10 - 除句子分隔符之外的标点符号
	#define ASCII	1
	#define GB2312	2
	#define GBK		3
	#define GB18030	4
	#define OTHER	5
	#define seperator	7
	#define number	8
	#define English	9
	#define PUNCTUATION	10

// 全局函数

#ifdef __cplusplus
extern "C"{
#endif
// 外部输出函数

	// SourceTxt 需判断编码的文本
	// EndPos 数组，存放每个字的结束位置
	// Type 数组，存放每个字的编码类型
	// size_in_char，字符串的长度，以字为单位
	// det_other，是否判断ASCII、GB2312中的分隔符、数字、英文和标点符号
	// 要求： Size 是 SourceTxt 的字节数，是 EndPos 数组和 Type 数组的大小
	// 返回结果中，EndPos以一个0结尾，Type以一个-1结尾，表示已到达结束位置
	// 例如 SourceTxt 为“Dell C640笔记本暢销”（中间有一空格），Size 为19
	// 如果det_other=true
	// EndPos为	“4 5 6 9 11 13 15 17 19”
	// Type为	“9 10 9 8 2 2 2 3 2 0 0 0 0 0”
	// 如果det_other=false
	// EndPos 为	“1 2 3 4 5 6 7 8 9 11 13 15 17 19 0 0 0 0 0”
	// Type 为	“1 1 1 1 1 1 1 1 1 2 2 2 3 2 0 0 0 0 0”
	Code_API bool Code_Determine(char *SourceTxt, long *EndPos, unsigned char *Type, long Size, 
		long *size_in_char = NULL, bool det_other = true );

	// 得到某个字
	// Size 是存放 Word 的字符串的长度
	Code_API bool Get_Word(char *SourceTxt, long *EndPos, long Index, char* Word, long Size);

	// 判断是否为ASCII码
	// 只判断第一个字符
	Code_API bool ASCII_Determine(const char* Word);

	// 判断是否为GB2312码
	// 只判断前两个字节
	// 要求：Word长度大于等于2
	Code_API bool GB2312_Determine(const char* Word);

	// 判断是否为GBK码
	// 只判断前两个字节
	// 要求：Word长度大于等于2
	Code_API bool GBK_Determine(const char* Word);

	// 判断是否为GB18030码
	// 只判断前四个字节
	Code_API bool GB18030_Determine(const char* Word);

	// 是否是分段的中英文符号
	Code_API bool IsParagraphSep(char *p);

	//// 回车符和换行符
	//static char *paraSep[] = {"\r","\n",NULL};

	// 判断句子分隔符
	// 默认字长度为0，此时判断所有长度的分隔符
	Code_API bool IsSentenceSep( char *character, int char_len = 0 );

	//// 分句的中英文符号
	//static char *sentence_sep[] = {
	//	"\t", "\n", "\r", "!", "?",
	//	"。", "！", "？", NULL};
	//static int sentence_sep_size = 8;

	//// 长度为1的分句符号
	//static char *sentence_sep_len1[] = { "\t", "\n", "\r", "!", "?", NULL};
	//static int sentence_sep_len1_size = 5;

	//// 长度为2的分句符号
	//static char *sentence_sep_len2[] = { "。", "！", "？", NULL};
	//static int sentence_sep_len2_size = 3;

	// 判断标点符号
	// 默认字长度为0，此时判断所有长度的标点符号
	Code_API bool IsPunctuation( char *character, int char_len = 0 );

	//// 标点符号
	//static char *sentence_Punctuation[] = {
	//	"\t", "\n", "\r", " ", "!", "\"", "#", "$", "&", "'", "(", ")", "*", "+", 
	//	",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "\\", "]", "^", 
	//	"`", "{", "|", "}", "~",
	//	"", "　", "、", "。", "·", "—", "～", "…", "‘", "’", "”", "〔", "〕", "〈", "〉", 
	//	"《", "》", "「", "」", "『", "』", "〖", "〗", "【", "】", "×", "∶", "∩", "≠", "≤",
	//	"≥", "°", "○", "！", "＇", "（", "）", "，", "－", "．", "：", "；", "＜",
	//	"＞", "？", "［", "］", "｛", "｝", "Φ",
	//		NULL};
	//static int sentence_Punctuation_size = 84;

	//// 长度为1的标点符号
	//static char *sentence_Punctuation_len1[] = {
	//	"\t", "\n", "\r", " ", "!", "\"", "#", "$", "&", "'", "(", ")", "*", "+", 
	//	",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "\\", "]", "^", 
	//	"`", "{", "|", "}", "~",
	//		NULL};
	//static int sentence_Punctuation_len1_size = 34;

	//// 长度为2的标点符号
	//static char *sentence_Punctuation_len2[] = { 
	//	"", "　", "、", "。", "·", "—", "～", "…", "‘", "’", "”", "〔", "〕", "〈", "〉", 
	//	"《", "》", "「", "」", "『", "』", "〖", "〗", "【", "】", "×", "∶", "∩", "≠", "≤",
	//	"≥", "°", "○", "！", "＇", "（", "）", "，", "－", "．", "：", "；", "＜",
	//	"＞", "？", "［", "］", "｛", "｝", "Φ",
	//		NULL};
	//static int sentence_Punctuation_len2_size = 50;

	// 是否是英文字母
	// 暂只能判断ASCII和GB2312中的英文字母
	// a-z,A-Z,ａ－ｚ，Ａ－Ｚ
	// size=0时自动判断p的长度
	Code_API bool IsEnglishLetter(char *p, int size=0);

	// 是否是大写英文字母
	// 暂只能判断ASCII和GB2312中的英文字母
	Code_API bool IsEnglishCapitalLetter(char *p);

	// 是否含有英文单词
	// 只判断第一个英文单词
	// 暂只能判断ASCII和GB2312中的英文字母
	Code_API bool HasEnglishLetter(char *p);

	// 是否含有首字大写的英文单词
	// 只判断第一个英文单词
	// 暂只能判断ASCII和GB2312中的英文字母
	Code_API bool HasEnglishCapitalWord(char *p);

	// 是否含有全部大写的英文单词
	// 只判断第一个英文单词
	// 暂只能判断ASCII和GB2312中的英文字母
	Code_API bool AllEnglishCapitalWord(char *p);

	// 转换为大写字母
	// 暂只能判断ASCII和GB2312中的英文字母
	Code_API void ConvertEnglishCapitalWord(char *Src, char *Res, long Size);

	// 是否是数字
	// 暂只能判断ASCII和GB2312中的数字
	// 0-9,０－９
	Code_API bool IsNumber(char *p, int size=0);
	//static char *ChineseNum[] = {"一","二","三","四","五","六","七","八","九",NULL};

	// 是否英文停用词
	// 暂空
	Code_API bool IsWordInStopDict(char *p);

	// 其他函数二期再实现
	// 例如 bool	IsGenericCChar(char *p);    /// 是否是中文字符

#ifdef __cplusplus
}
#endif

// 规则输出函数

#ifdef RULE_EXPORTS
#define Rule_API __declspec(dllexport)
#else
#define Rule_API __declspec(dllimport)
#endif

// 全局函数

#ifdef __cplusplus
extern "C"{
#endif
// 外部输出函数

	// 加载规则集
	// RuleSetPath 规则集路径
	// 现在只能从HotStar中引入规则集
	Rule_API bool Load_RuleSet(char* RuleSetPath);

	// 匹配规则
	Rule_API bool Match_Rule( char* Rule, char* Seperator );

	// 根据规则进行替换
	// 例如将“ａ”替换为“a”
	// 现只能处理“A=B”的替换情况
	// 现只能处理单字符的替换情况
	// 可处理“A=”的替换情况，即删除A
	Rule_API void Match_Rule_Convert( char* Content, char* Converted );

	// 卸载规则集
	// RuleSetPath 规则集路径
	Rule_API bool Unload_RuleSet(char* RuleSetPath);

#ifdef __cplusplus
}
#endif

#endif	///  _NLPE_DLL_API_
