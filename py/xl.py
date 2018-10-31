## @file
## @brief XL language implementation in Python

## @defgroup xl XL
## @brief Python implementation
## @{

## @defgroup node Node types
## @{

class Node: pass

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

## @}
