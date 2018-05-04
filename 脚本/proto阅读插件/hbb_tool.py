#!/usr/bin/env python 
#encoding: utf-8
import os

import win32clipboard as w

import win32con


def GetText():
	w.OpenClipboard()
	d = w.GetClipboardData(win32con.CF_TEXT)
	w.CloseClipboard()
	return d


def SetText(aString):
	w.OpenClipboard()
	w.EmptyClipboard()
	#w.SetClipboardData(win32con.CF_TEXT, aString.decode("utf").encode("gbk") )
	w.SetClipboardData(win32con.CF_TEXT, aString )
	w.CloseClipboard()

	

def File2ClipboardData(file):
	cur_path = os.path.split(os.path.realpath(__file__))[0]
	os.chdir(cur_path)
	
	f = open(file , "r")
	list_ = f.readlines()
	f.close()
	
	str_ = ""
	for one_ in list_:
		str_ += one_
		
	SetText(str_)


def ClipboardData2File(file):
	cur_path = os.path.split(os.path.realpath(__file__))[0]
	os.chdir(cur_path)

	str_ = GetText()
	
	f = open(file , "w")
	f.write(str_)
	f.close()
	
def AppandData2File(file):
	cur_path = os.path.split(os.path.realpath(__file__))[0]
	os.chdir(cur_path)

	str_ = GetText()
	
	f = open(file , "a")
	f.write("\n")
	f.write(str_)
	f.close()




























