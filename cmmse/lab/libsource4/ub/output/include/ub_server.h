/*
 * breif 通用服务器模型，支持长短连接
 */

#ifndef _UB_SERVER_H
#define _UB_SERVER_H

#include <ul_log.h>
#include <ub_log.h>
#include <ul_net.h>
#include <ub_conf.h>
#include <ub_auth.h>
#include <ul_def.h>

//设置服务器的长短连接
enum {
	UBSVR_SHORT_CONNECT = 0,	//短连接
	UBSVR_LONG_CONNECT
};

//设置服务器句柄配置
enum {
	UBSVR_LINGER_CLOSE = 0x1,
	UBSVR_NODELAY = 0x2,
};

//设置选用的pool类型
enum {
	UBSVR_XPOOL = 0,	//建议用于多线程短连接
	UBSVR_CPOOL,		//建议用于多线程长连接
	UBSVR_EPPOOL,		//基于epool的pendding pool，建议代替cpool(2.6以上内核)
	UBSVR_POOL_NUM,	//当前有多少个pool
};
enum{
	UBPOOL_SOCKET_CLOSE_DEFAULT,
	UBPOOL_SOCKET_CLOSE_TRUST
};
typedef struct _ub_server_t ub_server_t;

typedef int (* callback_proc)();
typedef int (* callback_check)(void *req, int reqsiz);

/*
 * @brief 	创建ub服务器，
 * @const char *ub服务器的名字
 * @return 	返回NULL，表示创建失败
 * 			成功返回创建服务器句柄
 */
ub_server_t *ub_server_create(const char *svrname = NULL);

/*
 * @brief  从配置文件中载入配置
 * @param[in] ub_server_t *sev 服务器句柄
 * @param[in] ub_svr_t *svrconf 服务器配置信息
 * @return 成载入返回0，失败返回非0
 */
int ub_server_load(ub_server_t *sev, ub_svr_t *svrconf);

/*
 * @brief		启动ub服务器
 * @param[in]	ub服务器句柄
 * @return		成功返回0，失败其他
 */
int ub_server_run(ub_server_t *);

typedef int	(*ub_handle_fun_t)();
/**
 * @brief 设置每个线程启动时候的回调函数
 *
 * @param [in/out] sev   : ub_server_t*
 * @param [in/out] op   : ub_handle_fun_t
 * @return  int 设置成功返回0，其他失败
 * @retval   
 * @see 
**/
int ub_server_set_threadstartfun(ub_server_t *sev, ub_handle_fun_t op);

/**
 * @brief 设置线程退出时候的处理函数
 *
 * @param [in/out] sev   : ub_server_t*
 * @param [in/out] op   : ub_handle_fun_t
 * @return  int 设置成功返回0，其他失败
 * @retval   
 * @see 
**/
int ub_server_set_threadstopfun(ub_server_t *sev, ub_handle_fun_t op);

/**
 * @brief 停止ubserver
 *
 * @return  int 成功返回0，其他失败
 * @retval   
 * @see 
**/
int ub_server_stop(ub_server_t *);

/*
 * @brief	join ub服务器
 * @param[in]	ub服务器句柄
 * @return	成功返回0，失败其他
 */
int ub_server_join(ub_server_t *);


/*
 * @brief		摧毁ub服务器
 * @param[in]	ub服务器句柄
 * @return		成功返回0，失败其他
 */
int ub_server_destroy(ub_server_t *);

//常用设置函数

/* @brief		设置ip认证系统句柄
 * @param[in]	sev ub服务器句柄
 * @param[in]	ip认证系统句柄
 * @return		成功返回0，失败其他
 */
int ub_server_set_ip_auth(ub_server_t *sev, ub_auth_t *auth);

/* @brief		设置监听端口
 * @param[in]	sev ub服务器句柄
 * @param[in]	port监听的端口
 * @return		成功返回0，失败其他
 */
int ub_server_set_listen_port(ub_server_t *sev, int port);

/*
 * @brief 		设置朴素行回调函数
 * @param[in] 	sev ub服务器的句柄
 * @param[in] 	cb 朴素行处理回调函数
 * @return		成功返回0，失败其他
 */
int ub_server_set_native_callback(ub_server_t *sev, callback_proc cb);

/*
 * @brief	 	设置激进行回调函数
 * @param[in] 	sev ub服务器句柄
 * @param[in] 	cb_deal激进行处理逻辑函数
 * @return		成功返回0，失败其他
 */
int ub_server_set_callback(ub_server_t *sev, callback_proc cb_deal);
int ub_server_set_callback(ub_server_t *sev, callback_proc cb_deal, callback_check cb_check);

/*
 * @brief 		设置读写缓冲区
 * @param[in] 	sev ub服务器句柄
 * @param[in] 	rddata 读缓冲区地址，N个线程所需要空间的指针数组
 * 				如果data设置为NULL，ub服务器根据设置，自己分配一段缓冲区
 * @param[in] 	read_siz 读缓冲区大小
 * @param[in] 	write_siz 写缓冲区大小
 * @param[in] 	wtdata 读缓冲区地址，N个线程所需要空间的指针数组
 * 				如果data设置为NULL，ub服务器根据设置，自己分配一段缓冲区
 * @return		成功返回0， 其他失败
 * @注意	 
 * */
int ub_server_set_buffer(ub_server_t *sev, void **rdbuf, int read_siz, void **wtbuf, int write_siz);
int ub_server_set_buffer(ub_server_t *sev, int threadnum, void **rdbuf, int read_siz, void **wtbuf, int write_siz);

/*
 * @brief 		设置用户指定的线程数据
 * @param[in] 	sev ub服务器句柄
 * @param[in] 	usersefbuf 用户设置线程数据，N个线程所需要空间的指针数组, 如果为空自动分配置顶大小的数据
 * @param[in] 	usersefsiz 用户设置线程数据的大小
 * @return		成功返回0， 其他失败	
 * @注意		
 */
int ub_server_set_user_data(ub_server_t *sev, void **usersefbuf, u_int usersefsiz);
int ub_server_set_user_data(ub_server_t *sev, int threadnum, void **, u_int);

/*
 * @brief		设置ubserver启动的线程数
 * @param[in]	sev ub服务器句柄
 * @param[in]	pnum启动的线程数
 * @return		成功返回0，失败其他
 */
int ub_server_set_pthread_num(ub_server_t *sev, int pnum);

/*
 * @brief		设置ubserver读写，连接超时, 单位毫秒
 * @param[in]	sev ub服务器句柄
 * @param[in]	connect_timeout	连接超时
 * @param[in]	read_timeout 读超时
 * @param[in]	write_timeout 写超时
 * @return		成功返回0，失败其他
 */
int ub_server_set_timeout(ub_server_t *sev, int connect_timeout, int read_timeout, int write_timeout);

/*
 * @brief	设置服务器句柄参数
 * @param	sev 服务器句柄
 * @param	type 句柄参数，可以设置UBSVR_NODELAY | UBSVR_LINGER_CLOSE
 * @return	0表示成功，其他失败
 */
int ub_server_setoptsock(ub_server_t *sev, int type);


/*
 * @brief 		设置服务器类型
 * @param[in] 	ub服务器
 * @param[in]	 服务器类型
 * 					XPOOL	处理短连接更加高效的模型
 * 					CPOOL	处理长连接更加高效的模型
 * 					EPPOOL	处理长连接更加高效的模型(在2.6内核以上使用，比CPOOL要好)
 * @return		返回0表示成功，其他失败
 */
int ub_server_set_server_type(ub_server_t *sev, int type);

/**
 * @brief 设置服务器名字
 *
 * @param [in/out] sev   : ub_server_t*
 * @param [in/out] name   : const char* 当前服务器的名字
 * @return  int 
 * @retval   
 * @see 
**/
int ub_server_set_server_name(ub_server_t *sev, const char *name);

/*
 * @brief 		 设置连接类型
 * @param[in]	 ub服务器
 * @param[in]	 连接类型
 * 					SHORT_CONNECT	//短连接
 * 					LONG_CONNECT	//长连接
 * @return		返回0表示成功，其他失败
 */
int ub_server_set_connect_type(ub_server_t *sev, int type);


//常用获取函数
/*
 * @brief		获取服务器的监听端口
 * @param[in]	服务器句柄
 * @return		返回监听端口
 */
int ub_server_get_listen_port(ub_server_t *sev);

/*
 * @brief		获取当前线程内部连接的socket句柄
 * @return		成功返回socket句柄，失败-1
 */
int ub_server_get_socket();

/*
 * @brief		重置当前线程内部连接的socket句柄
 */
int ub_server_set_socket_flags(int flag = UBPOOL_SOCKET_CLOSE_DEFAULT);

/*
 * @brief 	获取读缓冲区
 * @return 	得到缓冲区地址, 失败NULL
 */
void *ub_server_get_read_buf();

/*
 * @brief	获取用户自定义数据
 * @return	得到自动以数据地址，失败 NULL
 */
void *ub_server_get_user_data();

/*
 * @brief 	获取读缓冲区大小
 * @return 	缓冲区大小, 失败0
 */
u_int ub_server_get_read_size();

/*
 * @brief 	获取用户自定义数据大小
 * @return 	数据大小, 失败0
 */
u_int ub_server_get_userdata_size();

/*
 * @brief 	获取写缓冲区
 * @return 	得到缓冲区地址, 失败NULL
 */
void *ub_server_get_write_buf();

/*
 * @brief 	获取写缓冲区大小
 * @return 	缓冲区大小, 失败0
 */
u_int ub_server_get_write_size();

/*
 * @brief	得到连接超时 单位毫秒
 * @return	成功返回连接超时时间，失败返回0
 */
int ub_server_get_connect_timeout();

/*
 * @brief	得到读超时 单位毫秒
 * @return	成功返回读超时时间，失败返回0
 */
int ub_server_get_read_timeout();

/*
 * @brief	得到写超时 单位毫秒
 * @return	成功返回写超时时间，失败返回0
 */
int ub_server_get_write_timeout();

/*
 * @brief	得到连接端ip
 * @return  ip地址
 */
in_addr_t ub_server_get_ip();

//pendding pool(cpool/eppool)的专门数据
//历史原因这里采用cpool的参数设置和eppool的参数设置兼容
/*
 * @breif 		设置cpool/eppool的就绪队列长度，默认为100
 * @param[in] 	sev 服务器句柄
 * @param[in] 	siz 队列大小
 * @return		返回0表示成功，其他失败
 */
int ub_server_set_cpool_queue_size(ub_server_t *sev, int siz);

/*
 * @brief	获取cpool/eppool就绪队列大小
 * @return	cpool/eppool就绪队列大小
 */
int ub_server_get_cpool_queue_size(ub_server_t *sev);

/*
 * @brief		设置cpool/eppool socket缓存队列的大小
 * @param[in]	sev服务器句柄
 * @param[in]	siz socket缓存队列大小
 * @return		返回0表示成功，其他失败
 */
int ub_server_set_cpool_socket_size(ub_server_t *sev, int siz);

/*
 * @brief		获取cpool/eppool socket缓存队列大小
 * @return		返回cpool/eppool socket缓冲队列的大小
 */
int ub_server_get_cpool_socket_size(ub_server_t *sev);

/*
 * @brief		设置句柄在就绪队列里面的超时时间 单位秒
 * @param[in]	sev 服务器句柄
 * @param[in]	timeout 超时时间
 * @return		返回0表示成功，其他失败
 */
int ub_server_set_cpool_timeout(ub_server_t *sev, int timeout);
/*
 * @brief 获取句柄在缓存队列里面的超时时间 单位秒
 * @return 返回cpool/eppool缓冲队列里面的超时时间
 */
int ub_server_get_cpool_timeout(ub_server_t *sev);

//高级设置函数
/*
 * @brief	得到服务器句柄
 * @return	得到当前使用的ub服务器句柄，失败NULL
 */
ub_server_t *ub_server_get_server();

#endif

/* vim: set ts=4 sw=4 noet: */
