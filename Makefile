FILES_H := src/libvpk/libvpk.h
FILES_CPP := src/libvpk/libvpk.cpp

vpktool: $(FILES_H) $(FILES_CPP) src/vpktool/main.cpp
	$(CXX) -std=gnu++11 -DNDEBUG -O3 -g0 -Wall $(FILES_CPP) src/vpktool/main.cpp -o bin/vpktool

libvpk.so: $(FILES_H) $(FILES_CPP) src/libvpk/libvpk.h src/libvpk/libvpk.cpp
	$(CXX) -std=gnu++11 -DNDEBUG -O3 -g0 -Wall -shared -fPIC src/libvpk/libvpk.cpp -o bin/libvpk.so

vpktool-lib: libvpk.so $(FILES_H) $(FILES_CPP) src/vpktool/main.cpp
	$(CXX) -std=gnu++11 -DNDEBUG -O3 -g0 -Wall -I./bin src/vpktool/main.cpp -o bin/vpktool -L./bin -lvpk
