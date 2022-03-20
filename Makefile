# File              : Makefile
# Author            : Igor V. Sementsov <ig.kuzm@gmail.com>
# Date              : 06.12.2021
# Last Modified Date: 19.03.2022
# Last Modified By  : Igor V. Sementsov <ig.kuzm@gmail.com>


all:
	mkdir -p build && cd build && cmake .. && make && open mysql_connect_test

win:
	mkdir -p build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../mingw.cmake .. && make && open mysql_connect_test.exe

clean:
	rm -fr build
