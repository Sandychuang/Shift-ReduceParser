// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HITPARSER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HITPARSER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SRPARSER_EXPORTS
#define SRPARSER_API __declspec(dllexport)
#else
#define SRPARSER_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"{
#endif
    /****************************************************************/
    /*      			SR句法分析器 相关函数  						*/
    /****************************************************************/

     /** 
     *  @brief    模型加载
     *  @param    int inipathmode：配置文件路径选择，
				  inipathmode=0（或1），SRParser.ini文件位于应用程序所在目录
				  inipathmode=2，SRParser.ini文件位于HitParser.dll所在目录		         
     *  @return   若成功，返回1；不成功，返回0；
     */
	SRPARSER_API bool LoadModel(int inipathmode);


    /** 
     *  @brief    模型卸载，释放内存.
     *  @param    无.
     *  @return   若成功，返回1；不成功，返回0；
     */
	SRPARSER_API bool UnLoadModel();


    /**
     *  @brief SR短语结构分析
     *  @param const char* sentin   : 预短语结构分析的原始句子字符串.如："他/r 一/d 看见/vg 她/r 就/d 笑/vg 。/wj"。
     *  @param char * &sentout      : 存放短语结构分析后的字符串.调用此函数前须预先定义大小.
     *  @param soutlen              ：调用此函数前须申请存放分分析完后的句子的大小，调用函数后返回实际分词后串的大小.
     *  @return                     ：若成功返回true;不成功返回false;
    */
	SRPARSER_API bool Parser(const char* sentin,char * &sentout,int &soutlen);

    /**
     *  @brief SR短语结构分析，该接口是上面接口的速度升级版
     *  @param const char* sentin   : 预短语结构分析的原始句子字符串.如："他/r 一/d 看见/vg 她/r 就/d 笑/vg 。/wj"。
     *  @param char * &sentout      : 存放短语结构分析后的字符串.调用此函数前须预先定义大小.
     *  @param soutlen              ：调用此函数前须申请存放分分析完后的句子的大小，调用函数后返回实际分词后串的大小.
     *  @return                     ：若成功返回true;不成功返回false;
    */
	SRPARSER_API bool Parser_S(const char* sentin,char * &sentout,int &soutlen);
	
	/**	   
     *  @brief SR短语结构分析模型训练
     *  @param const char* filename   : 待训练的语料.如：[S[VOO[vg 祝][r 你][ng 好运]][wj ！]]。
   
    */
	SRPARSER_API bool TrainModel(const char* filename);
	


#ifdef __cplusplus
    }
#endif
