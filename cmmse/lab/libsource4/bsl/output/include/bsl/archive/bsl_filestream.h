/***************************************************************************
 * 
 * 
 **************************************************************************/
 
 
 
/**
 * @file bsl_filestream.h
 * @brief 
 *  
 **/


#ifndef  __BSL_FILESTREAM_H_
#define  __BSL_FILESTREAM_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <bsl/archive/bsl_stream.h>

namespace bsl
{
class filestream : public stream
{
	FILE *_fp;
	int _trans;
	int _trans_off;
	char *_wbuffer;
	size_t _wbuffersize;
	char *_rbuffer;
	size_t _rbuffersize;
public:
	filestream();
	~filestream();
	/**
	 * @brief �ļ���ģʽ
	 *
	 * @param [in/out] fname   : const char* �ļ���
	 * @param [in/out] mode   : const char* �ļ���ģʽ, 
	 * 								"w" д��
	 * 								"r" ����
	 * 								��Ҫ���Զ�д��,������FILEһ��
	 * @return  int  0��ʾ�ɹ�,����ʧ��
	 * @retval   
	 * @see 
	 * @note 
	**/
	int open(const char *fname, const char *mode);
	/**
	 * @brief �ر��ļ�
	 *
	 * @return  int 
	 * @retval   
	 * @see 
	 * @note 
	**/
	int close();
	/**
	 * @brief ���ļ�����д����,���д��ʧ��,ʲô��������
	 * Ҫ��д��,Ҫ��ع�����һ��״̬
	 *
	 * @param [in/out] dat   : const void* Ҫд������ָ��
	 * @param [in/out] size   : size_t д�����ݳ���
	 * @return  int 0 ��ʾ�ɹ�
	 * @retval   
	 * @see 
	 * @note 
	**/
	int write(const void *dat, size_t size);
	/**
	 * @brief ���ļ����������,�������ʧ��,ʲô��������
	 * Ҫ�����,Ҫ��ع�����һ��״̬ 
	 *
	 * @param [in/out] dat   : void*   
	 * @param [in/out] size   : size_t
	 * @return  int 0��ʾ�ɹ�
	 * @retval   
	 * @see 
	 * @note 
	**/
	int read(void *dat, size_t size);
	/**
	 * @brief ����һ������
	 *
	 * @return  int 0 �����ɹ�
	 * @retval   
	 * @see 
	 * @note 
	**/
	int start_trans();
	/**
	 * @brief ����һ������
	 *
	 * @return  int 0��ʾ�ɹ�
	 * @retval   
	 * @see 
	 * @note 
	**/
	int comit_trans();
	/**
	 * @brief ����һ������
	 *
	 * @param [in] trunc   : bool
	 * 					false : ����������Դ��̵Ĳ�������
	 * 					true : ��������Դ��̵Ĳ�������
	 * @return  int 0��ʾ�ɹ�
	 * @retval   
	 * @see 
	 * @note 
	**/
	int drop_trans(bool trunc);
	/**
	 * @brief ����д������
	 *
	 * @param [in/out] size   : size_t ��������СΪbyte
	 * @return  int �ɹ�����0
	 * @retval   
	 * @see 
	 * @note 
	**/
	int setwbuffer(size_t size);
	/**
	 * @brief ���ö�������
	 *
	 * @param [in/out] size   : size_t ��������Сλbyte
	 * @return  int �ɹ�����0
	 * @retval   
	 * @see 
	 * @note 
	**/
	int setrbuffer(size_t size);
	/**
	 * @brief ǿ�ƽ�����ˢ��Ӳ��
	 *
	 * @return  int 
	 * @retval   
	 * @see 
	 * @note 
	**/
	int flush();
};
};


#endif  //__BSL_FILESTREAM_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */