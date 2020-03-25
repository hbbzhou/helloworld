#!/usr/bin/env python 
#encoding: utf-8

import os
import time


import smtplib
import email.mime.multipart
import email.mime.text
#from email.header import Header

# 第三方 SMTP 服务
mail_host="smtp.163.com"  #设置服务器
mail_user="huangbinbin_fd@163.com"    #用户名
mail_pass="123456jkl"   #口令 

sender = mail_user
list_receivers = [ "huangbinbin.01@bytedance.com", "1900417004@qq.com"]

def ShowTime():
	import datetime
	now = datetime.datetime.now()
	return now.strftime('%y-%m-%d %H:%M')

def SendMailStep1(str_receiver , str_title , str_content):
	message = email.mime.multipart.MIMEMultipart() 
	message['From'] = sender
	message['To'] =  str_receiver
	message['Subject'] = str_title
		
	txt = email.mime.text.MIMEText(str_content,'plain','utf-8')
	message.attach(txt)
	return message
	
	
def SendMail(str_title , str_content):
	smtpObj = smtplib.SMTP() 
	smtpObj.connect(mail_host,25)    # 25 为 SMTP 端口号
	smtpObj.login(mail_user,mail_pass)
	for one_ in list_receivers:
		msg = SendMailStep1(one_ , str_title , str_content)
		smtpObj.sendmail(sender, one_, msg.as_string())
	print "send ok"



if __name__ == "__main__":
	#strTitle = "snk监控_重启服务"
	#strConTent = ShowTime()
	#SendMail(strTitle , strConTent)

























