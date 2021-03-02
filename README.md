
利用Python爬取C语言.c代码中的字符串
主要实现是在main.py文件中


Linux环境下：

把main.py脚本放到代码工程目录下,  执行python main.py -i test.C
main.py脚本会自动寻找test.c文件  并爬取tets.c文件中log_id_msg函数中的字符串 连接mysql数据库 写入mysql中
