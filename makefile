#
# Tomás Oliveira e Silva, AED, September 2022
#
# makefile to compile the A.01 assignment (speed run)
#

CC=	gcc

DEF:= speed_run

Z_SUF:= _with_zlib

DEF_Z:= ${DEF}$(Z_SUF)

SOL1:= sol1

SOL2:= sol2

SOL1_Z:= ${SOL1}$(Z_SUF)

SOL2_Z:= ${SOL2}$(Z_SUF)

${DEF}:		speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=0 speed_run.c -o ${DEF} -lm

${DEF_Z}:	speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=1 speed_run.c -o ${DEF_Z} -lm -lz

${SOL1}:	solution_speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -o ${SOL1} -lm

${SOL2}:	speed_run_im.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=0 speed_run_im.c -o ${SOL2} -lm

${SOL2_Z}:	speed_run_im.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=1 speed_run_im.c -o ${SOL2_Z} -lm -lz

${SOL1_Z}:	solution_speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -o ${SOL1_Z} -lm -lz

clean:
	rm -f *.pdf *.o

all:		clean ${DEF} ${DEF_Z} ${SOL1} ${SOL1_Z} ${SOL2} ${SOL2_Z}

fclean:		clean
	rm -f a.out ${DEF} ${DEF_Z} ${SOL1} ${SOL1_Z} ${SOL2} ${SOL2_Z}

re:			fclean all
