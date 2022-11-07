#
# Tomás Oliveira e Silva, AED, September 2022
#
# makefile to compile the A.01 assignment (speed run)
#

CC=	gcc

DEF:= speed_run

DEF_Z:= ${DEF}_with_zlib

SOL:= solution_speed_run

SOL_Z:= ${SOL}_with_zlib

${DEF}:		speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=0 speed_run.c -o ${DEF} -lm

${DEF_Z}:	speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=1 speed_run.c -o ${DEF_Z} -lm -lz

${SOL}:		solution_speed_run.c make_custom_pdf.c
	${CC} -g -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -o ${SOL} -lm

${SOL_Z}:	solution_speed_run.c make_custom_pdf.c
	${CC} -g -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -o ${SOL_Z} -lm -lz

clean:
	rm -f *.pdf *.o

fclean:		clean
	rm -f a.out ${DEF} ${SOL}
