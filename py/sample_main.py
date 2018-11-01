from xl import *

class Function(Node): pass

class Type(Node): pass

class Pointer(Node): pass

class Arg(Node): pass

int = Type('int')
char = Type('char')

array = Type('array')

argc = Arg('argc') ; argc['type'] = int
argv = Arg('argv') ; argv['type'] = ( array << ( Pointer('*') << char ) )
    
main = Function('main') \
    << Function('init') \
    << ( Name('hello') << Name('World') ) \
    << Function('fini') \
    << ( Name('return') << Integer(0) )
    
main['return'] = int
main['args'] = Block('args') << argc << argv
    
print main    
