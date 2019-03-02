import unittest
import os
import test

class Testapp(test.apptestcase):
	def teststudent(self):
		respadd=self.makeRequest("/student/home/preferences/tests/add", method="POST", data={'name': 'test', 'title': 'test for student add', 'description': 'it is for test'})
		self.assertSuccessResponse(respadd)
		self.assertEqual('test', respadd['preferences']['name'])
		self.assertEqual('test for student add', respadd['preferences']['title'])
		self.assertEqual('it is for test', respadd['preferences']['description'])

		respref=self.getpref()
		self.assertSuccessResponse(respref)
		self.assertEqual(1, len(respref['preferences']))
		self.assertEqual(respadd['preferences']['name'], respref['preferences'][0]['name'])

		respdel=self.delpref('test')
		self.assertSuccessResponse(respdel)

	def testinstructor(self):
		respadd=self.makeRequest("/instructor/home/teaching/testi/add", method="POST", data={'name': 'test', 'title': 'test for student add', 'description': 'it is for test'})
		self.assertSuccessResponse(respadd)
		self.assertEqual('test', respadd['preferences']['name'])
		self.assertEqual('test for student add', respadd['preferences']['title'])
		self.assertEqual('it is for test', respadd['preferences']['description'])

		respref=self.getteach()
		self.assertSuccessResponse(respref)
		self.assertEqual(1, len(respref['preferences']))
		self.assertEqual(respadd['preferences']['name'], respref['preferences'][0]['name'])

		respdel=self.delteach('test')
		self.assertSuccessResponse(respdel)

if __name__=='__main__':
	unittest.main()
