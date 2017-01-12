# coding: utf-8
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

#os.system("pause")




























