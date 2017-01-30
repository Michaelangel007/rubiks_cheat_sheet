all: make_oll_perm_tga

clean:
	rm make_oll_perm_tga
	rm pics/perm/

make_oll_tga: make_oll_perm_tga.cpp
	g++ -Wall -Wextra $< -o $@

tga: make_oll_tga
	mkdir -p pics/perm && cd pics/perm && ../../make_oll_tga && cd ../..

# OSX: 'sips'
# http://osxdaily.com/2013/01/11/converting-image-file-formats-with-the-command-line-sips/
# Linux: ImageMagik
png: pics/perm/*.tga
	cd pics/perm && sips -s format png *.tga --out ./ && cd ../..

