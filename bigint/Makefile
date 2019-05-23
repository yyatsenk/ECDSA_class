# Mention default target.
all:

# Implicit rule to compile C++ files.  Modify to your taste.
%.o: %.cc
	g++ -c -O2 -Wall -Wextra -pedantic -std=c++11 $<

# Components of the library.
library-objects = \
	BigUnsigned.o \
        BigInteger.o \
	BigIntegerUtils.o \
	EllipseCurveClass.o \

library-headers = \
        NumInBlocks.hh \
	BigUnsigned.hh \
        BigInteger.hh \
	BigIntegerLibrary.hh \

library: $(library-objects)

program = ellipse_class_test
program-objects = ellipse_class_test.o

$(program-objects) : $(library-headers)

$(program) : $(program-objects) $(library-objects)
	g++ -std=c++11 -g $^ -o $@

clean :
	rm -f $(library-objects) $(testsuite-cleanfiles) $(program-objects) $(program)

all : library $(program)

re : clean all
