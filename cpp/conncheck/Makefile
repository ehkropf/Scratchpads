CXX=clang++
# MacOS ftw.
CXXFLAGS=-Wall -Wextra -std=c++17 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.1.sdk
DEPS=CheckRemote.h

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

check: check.o
	$(CXX) -o check check.o

.PHONY: clean

clean:
	rm -rf *.o check
