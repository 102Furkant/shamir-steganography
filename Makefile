CC = gcc
CFLAGS = -Wall

all:
	gcc main.c menu.c sss_encode.c sss_decode.c stego_encode.c stego_decode.c lagrange.c permut.c psnr.c stb_implementer.c -lm -o pixcrypt

clean:
	rm -f pixcrypt
