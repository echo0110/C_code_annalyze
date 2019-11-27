#include<stdlib.h>
#include<stdio.h>
#include<mysql/mysql.h>

MYSQL *conn_ptr;
unsigned int timeout = 7;   //超时时间7秒
int mysql_connect()
{
    int ret = 0;
    conn_ptr = mysql_init(NULL);//初始化
    if(!conn_ptr)
    {
        printf("mysql_init failed!\n");
        return -1;
    }

    ret = mysql_options(conn_ptr,MYSQL_OPT_CONNECT_TIMEOUT,(const char*)&timeout);//设置超时选项
    if(ret)
    {
        printf("Options Set ERRO!\n");
    }
    conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","aabbcc","testdb",0,NULL,0);//连接MySQL testdb数据库
    if(conn_ptr)
    {
        printf("Connection Succeed!\n");
        mysql_close(conn_ptr);
        printf("Connection closed!\n");
    }
    else    //错误处理
    {
        printf("Connection Failed!\n");
        if(mysql_errno(conn_ptr))
        {
            printf("Connect Erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));//返回错误代码、错误消息
        }
        return -2;
    }

    return 0;
}

