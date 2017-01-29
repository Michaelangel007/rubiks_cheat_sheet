all: make_oll_bmp

clean:
	rm make_oll_bmp

make_oll_bmp: make_oll_bmp.cpp
	g++ -Wall -Wextra $< -o $@

