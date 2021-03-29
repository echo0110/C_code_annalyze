# !usr/bin/env Python
# coding=utf-8
import smtplib
from email.mime.text import MIMEText
from email.utils import formataddr
import threading

import re
import sys
import copy

import fileinput
import os
import getopt
import stat
import re
from ctypes import *
import subprocess

import sys
reload(sys)
sys.setdefaultencoding('utf-8')

def analyzecode(file_path,origin_path):
    log_line = 0
    text2_log_line = 0
    key = 0
    linux_path = file_path.replace('\\', '/')
    file = open(linux_path, 'r+')    
    try:
        text_lines = file.readlines()
        text_lines2 = copy.deepcopy(text_lines)
        for i, line in enumerate(text_lines):
            if 'log_id_msg' in line:
                log_line = i
                print(log_line)
                text2_log_line = log_line
                replace_log_line = log_line
                while text_lines[log_line].find(";", 0, len(text_lines[log_line]) - 1) == -1:
                        text_lines2[text2_log_line] += text_lines[log_line+1]
                        log_line += 1
                        key += 1
                pattern = re.compile(r'"(?:.|\n)*?"')
                log_str = pattern.findall(text_lines2[text2_log_line])
                log_str = " ".join(log_str)
		delete = re.compile(r'"')
		new_str = delete.sub('', log_str)
		c = str(new_str).encode('ascii')
                print'insert string is: ', c
                strinfo = re.compile('0xdeadbeef')
                if (text_lines[replace_log_line].find(";", 0, len(text_lines[replace_log_line]) - 1)) != -1:
                    if text_lines[replace_log_line].find("0xdeadbeef", 0, len(text_lines[replace_log_line]) - 1) > 0:
 			os.chdir(origin_path)
                	sql_id = lte.lte_string_key_insert(c)
               		print("sql_id=%d\n", sql_id)
               		sql_id_to_str = '%d' % sql_id
                        text_lines[replace_log_line] = strinfo.sub(sql_id_to_str, text_lines[replace_log_line])
                else:
                    while text_lines[replace_log_line].find(";", 0, len(text_lines[replace_log_line]) - 1) == -1:
                        if text_lines[replace_log_line].find("0xdeadbeef", 0, len(text_lines[replace_log_line]) - 1) > 0:
			    os.chdir(origin_path)
                            sql_id = lte.lte_string_key_insert(c)
                            print("sql_id=%d\n", sql_id)
                            sql_id_to_str = '%d' % sql_id
                            text_lines[replace_log_line] = strinfo.sub(sql_id_to_str, text_lines[replace_log_line])
                            print(text_lines[replace_log_line])
                        replace_log_line += 1

        file.close()
        file = open(linux_path, 'w')
        file.writelines(text_lines)
    finally:
        file.close()

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
	 origin_path = getpwd()
         linux_fmt = re.compile(r'(?:.|\n)*?[/]')
         temp_str = linux_fmt.findall(sys.argv[2])
         #print(temp_str)
         if temp_str:
             temp = "".join(temp_str)
             #print('linux_str: ', temp)
	     pattern = re.compile(temp)
             inputfile = pattern.sub('', sys.argv[2])
             os.chdir(temp)
	     sys.argv[1] = os.getcwd()
         else: 
             sys.argv[1] = getpwd()
         #print('current folder: ', sys.argv[1])
	 
         findfile(sys.argv[1],inputfile,origin_path)
      elif opt in ("-o", "--ofile"):
         outputfile = arg

def findfile(path, name,origin_path):
    file_path = ""
    full_path = ""
    for root, dirs, files in os.walk(path, topdown=False):
        if name in files:
            full_path = os.path.join(path, root, name)            
            file_path = os.path.normpath(os.path.abspath(full_path))             
    analyzecode(file_path,origin_path)

def getpwd():
    pwd = sys.path[0]
    if os.path.isfile(pwd):
        pwd = os.path.dirname(pwd)
    #print(pwd)
    return pwd
def current_path(): 
    print("changed working directory ") 
    print(os.getcwd())  
if __name__ == '__main__':
    lte = CDLL('./libstring_key.so')
    argument_parser(sys.argv[1:])

