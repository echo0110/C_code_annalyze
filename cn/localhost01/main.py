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
import fileinput
import os
import getopt
import stat
import re







def analyzecode(file_path):
    log_line = 0
    text2_log_line = 0
    key = 0
    linux_path = file_path.replace('\\', '/')
    print(linux_path)
    # file = open('D:/4G_code/lteserver/LTE_IPR/Taobao_order_robot-develop2/cn/localhost01/lteenodeb/gtpuPacketRelay/egtpu/stack/common/src/egtpu_cdb.c', 'r+')
    # file = open('D:/4G_code/lteserver/LTE_IPR/lteenodeb/gtpuPacketRelay/egtpu/stack/common/src/egtpu_cdb.c', 'r+')
    # file = open('D:/4G_code/lteserver/LTE_IPR/lteenodeb/common/src/lteHash.c', 'r+')
    # os.chmod(file, stat.S_IREAD)  # Read by owner
    file = open(linux_path, 'r+')
    try:
        text_lines = file.readlines()
        text_lines2 = copy.deepcopy(text_lines)  # 对象拷贝，深拷贝
        # print(type(text_lines), text_lines)
        # for line in text_lines:
        for i, line in enumerate(text_lines):            # log_line = log_line+1
            if u'LOG_ID_MSG' in line:
                log_line = i
                print(log_line)
                text2_log_line = log_line
                replace_log_line = log_line
                while text_lines[log_line].find(";", 0, len(text_lines[log_line]) - 1) == -1:
                        text_lines2[text2_log_line] += text_lines[log_line+1]
                        log_line += 1
                        key += 1
                pattern = re.compile(r'["](?:.|\n)*?["]')
                log_str = pattern.findall(text_lines2[text2_log_line])
                # text_lines2_list = list(text_lines2[text2_log_line])


                print("log_str[0]=%s\n", log_str[0])
                print("log_str[1]=%s\n", log_str[1])
                sql_id = mysqlconnect(log_str[0], log_str[1])
                print("sql_id=%d\n", sql_id)
                sql_id_to_str = '%d' % sql_id


                # str1 = pattern.sub(sql_id_to_str,  text_lines2[text2_log_line], re.DOTALL)
                # str1 = pattern.sub(sql_id_to_str, text_lines[log_line], re.DOTALL)
                # print(str1)

                strinfo = re.compile('ReplaceId')

                while text_lines[replace_log_line].find(";", 0, len(text_lines[replace_log_line]) - 1) == -1:
                    if text_lines[replace_log_line].find("ReplaceId", 0, len(text_lines[replace_log_line]) - 1) > 0:
                        # text_lines[replace_log_line].replace("ReplaceId", "ni")
                        text_lines[replace_log_line] = strinfo.sub(sql_id_to_str, text_lines[replace_log_line])
                        print(text_lines[replace_log_line])
                    replace_log_line += 1

        file.close()
        file = open(linux_path, 'w')
        file.writelines(text_lines)
    finally:
        file.close()

def mysqlconnect(str,str2):
    Url = "http://www.baidu.com"
    Time = datetime.datetime.now()  # 系统当前时刻
    db = MySQLdb.connect("192.168.201.237", "root", "Mysql@123", "TESTDB", charset='utf8')

    sql = "insert into log2(str,str2, Time) values('%s','%s','%s')" % (str, str2, Time)
    cursor = db.cursor()
    try:
        cursor.execute(sql)
        # param = (str0, str1, time)
        # param = [(str, str2)]
        # cursor.executemany(sql, param)
        db.commit()  # 提交到数据库执行，一定要记提交哦
        print("already commit")
        # print(db.insert_id()) last_id = curs.lastrowid
        last_id = cursor.lastrowid
        # print(last_id)
        return last_id;
    except Exception as e:
        db.rollback()  # 发生错误时回滚
        print(e)
    cursor.close()

def argument_parser(argv):
   inputfile = ''
   outputfile = ''
   try:
      opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
   except getopt.GetoptError:
      print('test.py -i <inputfile> -o <outputfile>')
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print('test.py -i <inputfile> -o <outputfile>')
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
         sys.argv[1] = getpwd()
         findfile(sys.argv[1], inputfile)
      elif opt in ("-o", "--ofile"):
         outputfile = arg

   print('输入的文件为：', inputfile)
   print('输出的文件为：', outputfile)
def findfile(path, name):
    # for relpath, dirs, files in os.walk(start):
    for root, dirs, files in os.walk(path, topdown=False):
        if name in files:
            full_path = os.path.join(path, root, name)
            file_path = os.path.normpath(os.path.abspath(full_path))
    print(file_path)
    analyzecode(file_path)

def getpwd():
    pwd = sys.path[0]
    if os.path.isfile(pwd):
        pwd = os.path.dirname(pwd)
    print(pwd)
    return pwd
if __name__ == '__main__':
    # getpwd()
    argument_parser(sys.argv[1:])
    # argument_parser(sys.argv[1:])
    # analyzecode()
