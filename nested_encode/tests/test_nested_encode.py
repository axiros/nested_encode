import json
import unittest
import copy
import time

from nested_encode import encode_nested
from .sample_objects import big_doc

class TestConvertNested(unittest.TestCase):
    def test_equal(self):
        new_ob = encode_nested(copy.deepcopy(big_doc))
        self.assertEqual(new_ob, big_doc)

    def test_encode_happens(self):
        ob = copy.deepcopy(big_doc)
        ob['foo'] = {u'a': [{'b': [(1, 2, [u'\xd6sterreich'])]}]}

        ob = encode_nested(ob)

        self.assertTrue(isinstance(ob['foo'].keys()[0], str))
        self.assertFalse(isinstance(ob['foo'].keys()[0], unicode))

        self.assertTrue(isinstance(ob['foo']['a'][0]['b'][0][2][0], str))
        self.assertFalse(isinstance(ob['foo']['a'][0]['b'][0][2][0], unicode))

        ref = {'a': [{'b': [(1, 2, ['\xc3\x96sterreich'])]}]}
        self.assertEqual(ob['foo'], ref)

    def test_perf(self):
        return

        print
        nb = 100000

        start = time.time()
        for _ in xrange(nb):
            encode_nested(big_doc)
        print 'big object with %s bytes json: ' % len(json.dumps(big_doc)),
        print int(nb / (time.time() - start)), ' OPS/s'

        start = time.time()
        for _ in xrange(nb):
            encode_nested(big_doc['00023'])
        print 'small object with %s bytes json: ' % len(json.dumps(big_doc['00023'])),
        print int(nb / (time.time() - start)), ' OPS/s'
