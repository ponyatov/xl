
XL = ../xl2

C += $(XL)/main.cpp $(XL)/scanner.cpp $(XL)/parser.cpp $(XL)/tree.cpp
C += $(XL)/ctrans.cpp $(XL)/context.cpp $(XL)/options.cpp $(XL)/errors.cpp

./xl: $(C) xl.syntax Makefile
	$(CXX) -o $@ $(C)

xl.syntax: $(XL)/xl.syntax
	cp $< $@
