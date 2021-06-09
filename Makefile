#L programming language was under the MIT license.
#Copyright(c) 2021 nu11ptr team.
all : make

make : interpreter.cpp
	$(CXX) $(CXXFLAGS) interpreter.cpp -o lpp

clean :
	-rm lpp
