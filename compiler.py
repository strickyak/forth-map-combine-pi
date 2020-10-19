import collections
import re, sys

def PrintIncludes():
    print '''
#include <assert.h>
#include <stdio.h>
#include "runtime.h"
'''

def PrintForward(name):
    print 'extern void Forth_%s(Zorth* p);' % name

def PrintStart(name):
    print ''
    print 'void Forth_%s(Zorth* p) {' % name

def PrintFinish():
    print '}'
    print ''

def CompileWord(w):
    print '    // <<<<< %s >>>>>' % w

    if re.search('^[-]?[0-9][0-9.]*$', w):
        print '    p->Push(%s);' % w

    elif w == 'do':
        print '    {'
        print '     double index = p->Pop();'
        print '     double limit = p->Pop();'
        print '    do {'

    elif w == 'loop':
        print '      index += 1.0;'
        print '    } while(index < limit);'
        print '    }'
        
    elif w == 'i':
        print '      p->Push(index);'

    elif w == '.':
        print '      printf("%.20g ", p->Pop());'

    elif w == 'drop':
        print '    p->Pop();'

    elif w == 'dup':
        print '    p->Push(p->Peek());'

    elif w == 'swap':
        print '    {double top = p->Peek();'
        print '     double second = p->PeekN(2);'
        print '     p->Poke(second); p->PokeN(2, top);}'

    elif w == 'rot':
        print '    {double top = p->Peek();'
        print '     double second = p->PeekN(2);'
        print '     double third = p->PeekN(3);'
        print '     p->Poke(third); p->PokeN(2, top); p->PokeN(3, second)}'

    elif w == 'rand':
        print '    p->Push(RandomNumber(p->rand_ptr));'

    elif w == '+':
        print '    {double top = p->Pop(); p->Poke(p->Peek() + top);}'

    elif w == '-':
        print '    {double top = p->Pop(); p->Poke(p->Peek() - top);}'

    elif w == '*':
        print '    {double top = p->Pop(); p->Poke(p->Peek() * top);}'

    elif w == '/':
        print '    {double top = p->Pop(); p->Poke(p->Peek() / top);}'

    elif w == '<':
        print '    {double top = p->Pop(); p->Poke(p->Peek() < top);}'

    elif w in definitions:
        print '    Forth_%s(p);' % w

    else:
        raise Exception('unknown word: %s' % w)

    print ''
pass

definitions = collections.defaultdict(list)
def Slurp():
    compiling = None
    ww = sys.stdin.read().split()
    while ww:
        w, ww = ww[0], ww[1:]

        if w == ';':
            compiling = None

        elif w == '(':
            while w != ')':
                w, ww = ww[0], ww[1:]

        elif w == ':':
            compiling, ww = ww[0], ww[1:]

        elif compiling:
            definitions[compiling].append(w)

        else:
            definitions['program'].append(w)


Slurp()
print '//', repr(definitions)
PrintIncludes()
for k in sorted(definitions):
    PrintForward(k)
for k, v in sorted(definitions.items()):
    PrintStart(k)
    for w in v: CompileWord(w)
    PrintFinish()

