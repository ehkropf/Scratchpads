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
        self.opstr = input
        self._slen_ = len(input)
        self._spos_ = 0
        self.opdic = self.makedic()

    def makedic(self):
        ''' makedic constructs dictionary from OP dictionary string '''

        istr = self.opstr
        ix = self._spos_
        opdic = {}

        ix += 1
        while ix < self._slen_ and not (istr[ix] == '}'):
            ei = istr.find('=', ix)
            key = istr[ix:ei]
            ix = ei + 1
            if istr[ix] == '{':
                self._spos_ = ix
                value = self.makedic()
                ix = self._spos_
            else:
                rbi = istr.find('}', ix)
                pbi = istr.find('|', ix)
                if pbi > -1 and pbi < rbi:
                    value = istr[ix:pbi]
                    ix = pbi + 1
                else:
                    value = istr[ix:rbi]
                    ix = rbi
            opdic[key] = value
        if istr[ix:ix+2] == '}|':
            ix += 2
        else:
            ix += 1

        self._spos_ = ix
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

def default():
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

################################################################################
if __name__ == "__main__":
    default()
