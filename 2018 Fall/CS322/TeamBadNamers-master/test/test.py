import unittest
import traceback
import http.client
import sys
import os
import json

class RestTestCase(unittest.TestCase):

    serverToTest = "localhost:5000"
    VERBOSE = 1


    def makeRequest(self, url, method="GET", data={ }):

        printHeaders = (self.VERBOSE == 1)

        headers = { }
        body = ""
        if data is not None:
            headers = { "content-type": "application/json", "Accept": "application/json" }
            body = json.dumps(data)

        try:
            self.conn.request(method, url, body, headers)
        except Exception as e:
            if str(e).find("Connection refused") >= 0:
                print ("Cannot connect to the server ",RestTestCase.serverToTest,". You should start the server first, or pass the proper TEST_SERVER environment variable")
                sys.exit(1)
            raise

        self.conn.sock.settimeout(100.0) # Give time to the remote server to start and respond
        
        #get the response
        resp = self.conn.getresponse()
        data_string = "<unknown"
        try:
            if printHeaders:
                print("\n****")
                print("  Request: ",method," url=",url," data=",str(data))
                print("  Request headers: ",str(headers))
                print("")
                print("  Response status: ",str(resp.status))
                print("  Response headers: ")
                for h, hv in resp.getheaders():
                    print("    ",h,"  =  ",hv)

            self.assertEqual(200, resp.status)
            #extract the response data
            data_string = resp.read().decode()
            if printHeaders:
                print("  Response data: ",data_string)
            # The response must be a JSON request
            # Note: Python (at least) nicely tacks UTF8 information on this,
            #   we need to tease apart the two pieces.
            self.assertTrue(resp.getheader('content-type') is not None, "content-type header must be present in the response")
            self.assertTrue(resp.getheader('content-type').find('application/json') == 0, "content-type header must be application/json")

            #return the data in the response
            data = json.loads(data_string)
            return data

        except:
            print("Got exception when processing response to url="+url+" method="+method+" data="+str(data))
            print("  Response status = "+str(resp.status))
            print("  Response headers: ")
            for h, hv in resp.getheaders():
                print("    "+h+"  =  "+hv)
            print("  Data string: ",data_string)
            print("  Exception: ",traceback.format_exc ())
            if not printHeaders:
                print("  If you want to see the request headers, use VERBOSE=1")


    def setUp(self):
        self.conn = http.client.HTTPConnection(RestTestCase.serverToTest, timeout=1)

    def tearDown(self):
        self.conn.close ()

class apptestcase(RestTestCase):
    def setUp(self):
        RestTestCase.setUp(self)

    def getcourse(self):
        url='/home/courses'
        print(url)
        resp=self.makeRequest(url, method='GET')
        return resp

    def getpref(self):
        url='/student/home/preferences/tests'
        print(url)
        resp=self.makeRequest(url, method='GET')
        return resp

    def getteach(self):
        url='/instructor/home/teaching/testi'
        print(url)
        resp=self.makeRequest(url, method='GET')
        return resp

    def delpref(self, name):
        url='/student/home/preferences/tests/delete/'+name
        print(url)
        resp=self.makeRequest(url, method='DELETE')
        return resp

    def delteach(self, name):
        url='/instructor/home/teaching/<_username>/delete/'+name
        print(url)
        return resp

    def assertSuccessResponse(self, resp, msg=None):
        self.assertTrue(resp!=None, 'Response is None')
        self.assertEqual(1, resp['status'], msg)