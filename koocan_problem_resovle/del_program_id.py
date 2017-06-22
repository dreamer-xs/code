#!/usr/bin/env python
# -*-coding:UTF-8-*-
import sys
import httplib
import hashlib
import traceback
from requests import delete
from json import loads, dumps


def md5_str(src):
    my_md5 = hashlib.md5()
    my_md5.update(src)
    md5_digest = my_md5.hexdigest()
    return md5_digest


def cas_login():
    host = "192.168.10.231"    # 内网IP
    # host = "198.255.30.10"   # 注意：这个是线上IP
    port = "8822"
    url = "/api/cas/login"
    params = {
        "username": "admin",
        "password": md5_str("123456")
    }
    retry_flag = True
    try:
        while retry_flag:
            httpClient = httplib.HTTPConnection(host, port, timeout=90)
            headers = {"Content-type": "application/json"}
            httpClient.request("POST", url, dumps(params), headers)
            response = httpClient.getresponse()
            if response.status == 200:
                response_info = loads(response.read())
                token = response_info["token"]
                return token
            else:
                print "response.status = %s, response.reason = %s" % (response.status, response.reason)
    except:
        print traceback.format_exc()

id_list = '[' + sys.argv[1] + ']'
dcmp_url = "http://192.168.10.30:8000/api/dcmp/program?id=" + id_list
cookies = {"tokenUserName": "admin", "token": cas_login()}
response = delete(url=dcmp_url, cookies=cookies, timeout=30, headers={'Connection': 'close'})
print response.content


