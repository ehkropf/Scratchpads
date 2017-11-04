#!/opt/local/bin/python3

import sys
import re

convert = lambda text: int(text) if text.isdigit() else text.lower()
alphanum_key = lambda item: [ convert(c) for c in re.split('([0-9]+)', item[0]) ]

class OPdic:
    ''' Represents OP dictionary.
        Create from string.
    '''

    def __init__(self, input):
        self._tstr_ = input
        self.opdic = self.getdict()

    def getdict(self):
        ''' getdict returns dictionary from OP dictionary string '''

        opdic = {}

        self._tstr_ = self._tstr_[1:]
        while len(self._tstr_) > 0 and not (self._tstr_[0] == '}'):
            key, self._tstr_ = self._tstr_.split('=', maxsplit=1)
            if self._tstr_[0] == '{':
                value = self.getdict()
            else:
                rbi = self._tstr_.find('}')
                pbi = self._tstr_.find('|')
                if pbi > -1 and pbi < rbi:
                    value, self._tstr_ = self._tstr_.split('|', maxsplit=1)
                else:
                    value = self._tstr_[0:rbi]
                    self._tstr_ = self._tstr_[rbi:]
            opdic[key] = value
        if self._tstr_[0:2] == '}|':
            self._tstr_ = self._tstr_[2:]
        else:
            self._tstr_ = self._tstr_[1:]

        return opdic

    def print(self, lead=[]):
        OPdic.printdic(self.opdic, lead=lead)

    def printdic(opdic, lead=[]):
        ''' Prints parsed OP dictionary nicely.'''

        pad = ''.join(lead)
        for k, v in sorted(opdic.items(), key=alphanum_key):
            if type(v) is dict:
                print(pad + k + ' = ')
                lead.append('    ')
                OPdic.printdic(v, lead=lead)
            else:
                print(pad + k + ' = ' + v)
        if len(lead) > 1:
            lead.pop()

fh = open(sys.argv[1], 'r')

for line in fh:
    line = line.rstrip('\n')
    opdic = OPdic(line[10:]).opdic
    if 'reply_to' in opdic:
        mkind = 'reply_to'
    else:
        mkind = 'message_info'
    if 'private' in opdic[mkind]:
        pstr = '|' + 'private=' + opdic[mkind]['private']
    else:
        pstr = ''
    print('{' + mkind + '={message_type=' + opdic[mkind]['message_type'] + pstr + '}}')
    OPdic.printdic(opdic, lead=['  '])

fh.close()
