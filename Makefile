#
# Tomás Oliveira e Silva, AED, September 2022
#
# makefile to compile the A.01 assignment (speed run)
#

CC=	gcc

DEF:= speed_run

Z_SUF:= _with_zlib

DEF_Z:= ${DEF}$(Z_SUF)

${DEF}:		speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=0 speed_run.c -o ${DEF} -lm

${DEF_Z}:	speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=1 speed_run.c -o ${DEF_Z} -lm -lz

clean:
	rm -f *.pdf *.o

all:		clean ${DEF} ${DEF_Z}

fclean:		clean
	rm -f a.out ${DEF} ${DEF_Z}

re:			fclean all
