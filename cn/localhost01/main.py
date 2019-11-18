# -*- coding: utf-8 -*-
# import schedule
# import time
# from selenium import webdriver
# from selenium.webdriver import ActionChains
# import  urllib2
# from bs4 import BeautifulSoup
# import requests
import smtplib
from email.mime.text import MIMEText
from email.utils import formataddr
import threading

# from util.str_util import print_msg, send_mail
# from spider.taobao_climber import TaobaoClimber
# from mail.mail_sender_browser import MailSenderBrow*
# from __init__ import *ser
# from mail.mail_sender import


import re
import sys
import numpy
import copy   #导入copy模块

import MySQLdb
import regex as re
import datetime

def analyzecode():
    log_line = 0
    text2_log_line = 0
    key = 0
    # log_list = []
    log_list = [([0] * 5) for i in range(10)]
    num_list = numpy.zeros((20, 10))
    file = open(r'D:\4G_code\lteserver\LTE_IPR\lteenodeb\gtpuPacketRelay\egtpu\stack\common\src\egtpu_cdb.c')
    try:
        text_lines = file.readlines()
        text_lines2 = copy.deepcopy(text_lines)  # 对象拷贝，深拷贝
        print(type(text_lines), text_lines)
        # text_lines2[59] = text_lines2[59] + text_lines2[60] + text_lines2[61]
        # print(text_lines2[59])
        # num_list[1] = text_lines[59]
        # print(text_lines2[59])
        # pattern = re.compile(r'[?<=()](.*?)[?=)]')
        # str1 = pattern.sub("0",  text_lines2[59])
        # print(str1)

        # for line in text_lines:
        for i, line in enumerate(text_lines):            # log_line = log_line+1
            if u'LOG_MSG' in line:
                # log_line = file.readline()
                log_line = i
                print(log_line)
                text2_log_line = log_line
                # print(text_lines2[text2_log_line])
                while text_lines[log_line].find(";", 0, len(text_lines[log_line]) - 1) == -1:
                        text_lines2[text2_log_line] += text_lines[log_line+1]
                        log_line += 1
                        key += 1
                # print(type(text_lines2[log_line]), text_lines2[log_line])
                # print(type(text_lines2[text2_log_line]), text_lines2[text2_log_line])
                # pattern = re.compile(r'(?<=().+?(?=))')
                # pattern = re.compile(r'(?<=().+?(?=))')
                # pattern = re.compile(r'[(](.*?)[)]')
                # pattern = re.compile(r'[(](.*?)[)]')
                # str1 = re.sub(r'(?<=().+?(?=))', "0", "adod(23d)das")

                # pattern = re.compile(r'[(](?:.|\n)*?[)]')
                # # pattern = re.compile(r'[?<=(].+?[?=)]')
                # # str1 = pattern.sub('(8)',  "log_msg(23d)das", re.DOTALL)

                pattern = re.compile(r'["](?:.|\n)*?["]')
                str1 = pattern.sub('8',  text_lines2[text2_log_line], re.DOTALL)
                print(str1)


                # print(type(text_lines), log_list)
        # print(type(text_lines), log_list)
    finally:
        file.close()



def mysqlconnect():
    Url = "http://www.baidu.com"
    Time = datetime.datetime.now()  # 系统当前时刻
    db = MySQLdb.connect("192.168.201.237", "root", "Mysql@123", "TESTDB", charset='utf8')
    # 使用cursor()方法获取操作游标
    # cursor = db.cursor()
    # # 使用execute方法执行SQL语句
    # cursor.execute("SELECT VERSION()")
    # # 使用 fetchone() 方法获取一条数据
    # data = cursor.fetchone()
    # print("Database version : %s " % data)



    sql = "insert into log_test66(str, time) values('%s','%s')" % (Url, Time)
    cursor = db.cursor()
    try:
        cursor.execute(sql)
        db.commit()  # 提交到数据库执行，一定要记提交哦
        print("already commit")
        print(db.insert_id())
    except Exception as e:
        db.rollback()  # 发生错误时回滚
        print(e)
    cursor.close()


if __name__ == '__main__':
    # 正则：解析留言内容
    # re_note = re.compile(
    #     ur"^留言:[\u3000\u0020]*([\w.-]+@[\w.-]+\.\w+)\s*$")  # 格式; 留言： +任意空格+邮箱
    analyzecode()
    mysqlconnect()


