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

def analyzecode():
    log_line = 0
    log_list = []
    file = open(r'D:\4G_code\lteserver\LTE_IPR\lteenodeb\gtpuPacketRelay\egtpu\stack\common\src\egtpu_cdb.c')
    try:
        text_lines = file.readlines()
        print(type(text_lines), text_lines)
        # print(text_lines[80])
        # text_lines[59] + text_lines[60] + text_lines[61]
        print(text_lines[59] + text_lines[60] + text_lines[61])
        for line in text_lines:
            log_line = log_line+1
            if u'LOG_MSG' in line:
                # log_line = file.readline()
                print(log_line)
                log_line = log_line - 1
                # print(type(text_lines), text_lines[log_line-1])
                log_list.append(line)
                while text_lines[log_line].find(";", 0, len(text_lines[log_line]) - 1) == -1:
                    # log_list[log_line] += text_lines[log_line+1]
                    log_list.append(text_lines[log_line+1])
                    log_line += 1
                print(type(text_lines), log_list[0])
                print(type(text_lines), log_list[1])
                print(type(text_lines), log_list[2])
                print(type(text_lines), log_list)

    finally:
        file.close()


    # line = read_file(r'C:\Users\casa\Desktop\logtest.c')
    # #     # line = read_file('D:/4G_code/lteserver/LTE_IPR/lteenodeb/gtpuPacketRelay/egtpu/stack/common/src/egtpu_cdb.c')
    # print(line)
    #     # 行号
    line_num = 0
    # 多行注释起始行号
    line_comment_start = 0
    comment_start1 = '//'
    comment_start2 = '/*'
    comment_end = '*/'

    # for i in line:
    #     # 去除空白行
    #     if len(i) < 1:
    #         line_num = line_num + 1
    #         continue
    #     else:
    #         #  处理注释行
    #         if cflag == 0:
    #             if i.startswith(comment_start1):  # 如果是以 // 开头
    #                 line_num = line_num + 1
    #                 continue
    #             elif i.startswith(comment_start2):  # 如果是以 /* 开头
    #                 line_comment_start = line_num
    #                 cflag = 1
    #                 line_num = line_num + 1
    #                 continue
    #         elif cflag == 1:
    #             if i.endswith(comment_end):  # 如果是以 */ 结尾
    #                 for m in range(line_comment_start, line_num + 1):
    #                     cflag = 0
    #                 line_num = line_num + 1
    #                 continue
    #             else:
    #                 line_num = line_num + 1
    #                 continue
    #
    #         line_num = line_num + 1
    #
    #     # 存放字符的列表
    #     each = []
    #     # 分解每个字符
    #     for m in i:
    #         each.append(m)  # 将每个字符添加到列表 each 中
    #     word = ''
    #     for e in each:
    #         # 是操作符
    #         # 是常数 NUM
    #         if oflag == 2:
    #             if e == ' ':
    #                 NUM_List.append(word)
    #                 print_List.append('Line:' + str(line_num) + ' ' + word + ', NUM')
    #                 content_List.append(word)
    #                 word = ''
    #                 oflag = 0
    #             elif e in SEPARATOR:
    #                 NUM_List.append(word)
    #                 print_List.append('Line:' + str(line_num) + ' ' + word + ', NUM')
    #                 content_List.append(word)
    #                 SEPARATOR_list.append(e)
    #                 print_List.append('Line:' + str(line_num) + ' ' + e + ', SEPARATOR')
    #                 content_List.append(e)
    #                 word = ''
    #                 oflag = 0
    #             else:
    #                 word = word + e
    #             continue
    #         # 是关键字或变量名
    #
    #         # 是字符串 STRING
    #         elif oflag == 4:
    #             if e != '"':
    #                 word = word + e
    #             elif e == '"':
    #                 word = word + e
    #                 STRING_List.append(word)
    #                 print_List.append('Line:' + str(line_num) + ' ' + word + ', STRING')
    #                 content_List.append(word)
    #                 word = ''
    #                 oflag = 0
    #             continue


if __name__ == '__main__':

    # 正则：解析留言内容
    # re_note = re.compile(
    #     ur"^留言:[\u3000\u0020]*([\w.-]+@[\w.-]+\.\w+)\s*$")  # 格式; 留言： +任意空格+邮箱
    analyzecode()



