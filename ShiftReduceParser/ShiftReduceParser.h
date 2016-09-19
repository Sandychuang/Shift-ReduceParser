// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� HITPARSER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// HITPARSER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SRPARSER_EXPORTS
#define SRPARSER_API __declspec(dllexport)
#else
#define SRPARSER_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"{
#endif
    /****************************************************************/
    /*      			SR�䷨������ ��غ���  						*/
    /****************************************************************/

     /** 
     *  @brief    ģ�ͼ���
     *  @param    int inipathmode�������ļ�·��ѡ��
				  inipathmode=0����1����SRParser.ini�ļ�λ��Ӧ�ó�������Ŀ¼
				  inipathmode=2��SRParser.ini�ļ�λ��HitParser.dll����Ŀ¼		         
     *  @return   ���ɹ�������1�����ɹ�������0��
     */
	SRPARSER_API bool LoadModel(int inipathmode);


    /** 
     *  @brief    ģ��ж�أ��ͷ��ڴ�.
     *  @param    ��.
     *  @return   ���ɹ�������1�����ɹ�������0��
     */
	SRPARSER_API bool UnLoadModel();


    /**
     *  @brief SR����ṹ����
     *  @param const char* sentin   : Ԥ����ṹ������ԭʼ�����ַ���.�磺"��/r һ/d ����/vg ��/r ��/d Ц/vg ��/wj"��
     *  @param char * &sentout      : ��Ŷ���ṹ��������ַ���.���ô˺���ǰ��Ԥ�ȶ����С.
     *  @param soutlen              �����ô˺���ǰ�������ŷַ������ľ��ӵĴ�С�����ú����󷵻�ʵ�ʷִʺ󴮵Ĵ�С.
     *  @return                     �����ɹ�����true;���ɹ�����false;
    */
	SRPARSER_API bool Parser(const char* sentin,char * &sentout,int &soutlen);

    /**
     *  @brief SR����ṹ�������ýӿ�������ӿڵ��ٶ�������
     *  @param const char* sentin   : Ԥ����ṹ������ԭʼ�����ַ���.�磺"��/r һ/d ����/vg ��/r ��/d Ц/vg ��/wj"��
     *  @param char * &sentout      : ��Ŷ���ṹ��������ַ���.���ô˺���ǰ��Ԥ�ȶ����С.
     *  @param soutlen              �����ô˺���ǰ�������ŷַ������ľ��ӵĴ�С�����ú����󷵻�ʵ�ʷִʺ󴮵Ĵ�С.
     *  @return                     �����ɹ�����true;���ɹ�����false;
    */
	SRPARSER_API bool Parser_S(const char* sentin,char * &sentout,int &soutlen);
	
	/**	   
     *  @brief SR����ṹ����ģ��ѵ��
     *  @param const char* filename   : ��ѵ��������.�磺[S[VOO[vg ף][r ��][ng ����]][wj ��]]��
   
    */
	SRPARSER_API bool TrainModel(const char* filename);
	


#ifdef __cplusplus
    }
#endif
