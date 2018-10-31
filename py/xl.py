## @file
## @brief XL language implementation in Python

import os,sys

## @defgroup xl XL
## @brief Python implementation
## @{

## @defgroup node Node types
## @{

## root class
class Node:
    
    ## construct leaf with value
    ## @param[in] V value
    def __init__(self,V):
        ## primitive value
        self.value = V
        ## nested elements = vector & stack
        self.nest = []
        
    # vector/stack
    
    ## `<<` operator
    def __lshift__(self,obj):
        return self.push(obj)
    
    ## push as stack
    def push(self,obj):
        self.nest.append(obj) ; return self
        
    ## print
    def __repr__(self):
        return self.dump()
    ## full tree dump
    def dump(self,depth=0):
        S = self.pad(depth) + self.head()
        for j in self.nest: S += j.dump(depth+1)
        return S
    ## short header-only dump
    def head(self):
        return '<%s:%s>' % (self.__class__.__name__.lower(),self.value)
    ## left pad with tabs (for tree)
    def pad(self,N):
        return '\n' + '\t'*N

## @defgroup leaf leaf
## @brief leaf nodes
## @{

## leaf in the parse tree
class Leaf(Node): pass

## integer number
class Integer(Leaf): pass

## floating point number
class Real(Leaf): pass

## string
class Text(Leaf): pass

## names and symbols
class Name(Leaf): pass

## @}

## @defgroup inner inner
## @brief inner nodes (trees)
## @{

## structural trees
class Inner(Node): pass

## infix operrator
class Infix(Inner): pass

## prefix operator
class Prefix(Inner): pass

## postfix operator
class Postfix(Inner): pass

## curly block
class Block(Inner): pass

## @}

## @}

## @defgroup parser Parser
## @brief Powered by PLY library
## @{

import ply.lex  as lex
import ply.yacc as yacc

## @defgroup lexer lexer
## @{

## tokens list
tokens = ['int','real','text','name','op','nl']

## ignored chars
t_ignore = ' \t\r'

## line comment
t_ignore_COMMENT = '//.*'

## new line
def t_nl(t):
    r'\n'
    t.lexer.lineno += 1
    return t

## operator
def t_op(t):
    r'[\+\-]'
    t.value = Name(t.value) ; return t
    
## floating point
def t_real(t):
    r'[0-9]+\.[0-9]+([eE][\+\-]?[0-9]+)?'
    t.value = Real(float(t.value)) ; return t

## integer
def t_int(t):
    r'[0-9]+'
    t.value = Integer(int(t.value)) ; return t
    
## name
def t_name(t):
    r'[a-zA-Z0-9_]+'
    t.value = Name(t.value) ; return t
    
## lexer error callback
def t_error(t): raise SyntaxError(t)

## lexer
lexer = lex.lex()

## @}

## @defgroup syntax syntax
## @{

## empty file
def p_none(p):
    ' REPL : '

## drop newlines
def p_nl(p):
    ' REPL : REPL nl'

## recursive parser (sequence of **ex**pressions)
def p_recur(p):
    ' REPL : REPL ex '
    print p[2]

## prefix expression
def p_prefix(p):
    ' ex : op ex '
    p[0] = p[1] << p[2]
    
## integer is expression
def p_int(p):
    ' ex : int '
    p[0] = p[1]
    
## real is expression
def p_real(p):
    ' ex : real '
    p[0] = p[1]
    
## text is expression
def p_text(p):
    ' ex : text '
    p[0] = p[1]
    
## name is expression
def p_name(p):
    ' ex : name '
    p[0] = p[1]
    
## syntax error callback
def p_error(p): raise SyntaxError(p)

## parser
parser = yacc.yacc(debug=False,write_tables=False)

## @}

## @}

## @defgroup startup Startup code
## @{

## source code
try:
    SRC = open(sys.argv[1]).read()
except IndexError: 
    SRC = open('test.xl').read()

parser.parse(SRC)

## @}

## @}
