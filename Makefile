#L programming language was under the MIT license.
#Copyright(c) 2021 nu11ptr team.
all : make_linux

make_linux : interpreter.cpp
	$(CXX) $(CXXFLAGS) interpreter.cpp -o lpp
make_linux32 : interpreter.cpp
	$(CXX) $(CXXFLAGS) -m32 interpreter.cpp -o lpp_linux32
make_linux64 : interpreter.cpp
	$(CXX) $(CXXFLAGS) -m64 interpreter.cpp -o lpp_linux64
make_win : interpreter.cpp
	$(CXX) $(CXXFLAGS) interpreter.cpp -o lpp.exe
make_win32 : interpreter.cpp
	$(CXX) $(CXXFLAGS) -m32 interpreter.cpp -o lpp_win32.exe
make_win64 : interpreter.cpp
	$(CXX) $(CXXFLAGS) -m64 interpreter.cpp -o lpp_win64.exe
make_mingw : interpreter.cpp
	i686-w64-mingw32-g++ $(CXXFLAGS) interpreter.cpp -o lpp.exe
make_mingw32 : interpreter.cpp
	i686-w64-mingw32-g++ $(CXXFLAGS) -m32 interpreter.cpp -o lpp_win32.exe
make_mingw64 : interpreter.cpp
	x86_64-w64-mingw32-g++ $(CXXFLAGS) -m64 interpreter.cpp -o lpp_win64.exe
github : interpreter.cpp
	$(CXX) $(CXXFLAGS) -m32 interpreter.cpp -o lpp_linux32
	$(CXX) $(CXXFLAGS) -m64 interpreter.cpp -o lpp_linux64
	i686-w64-mingw32-g++ $(CXXFLAGS) -m32 interpreter.cpp -o lpp_win32.exe
	x86_64-w64-mingw32-g++ $(CXXFLAGS) -m64 interpreter.cpp -o lpp_win64.exe
clean :
	@-rm lpp
	@-rm lpp.exe
	@-rm lpp_linux32
	@-rm lpp_linux64
	@-rm lpp_win32.exe
	@-rm lpp_win64.exe
