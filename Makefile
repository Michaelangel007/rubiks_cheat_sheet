all: make_oll_perm_tga

clean:
	rm make_oll_perm_tga

make_oll_tga: make_oll_perm_tga.cpp
	g++ -Wall -Wextra $< -o $@

