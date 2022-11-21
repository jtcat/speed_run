#
# Tomás Oliveira e Silva, AED, September 2022
#
# makefile to compile the A.01 assignment (speed run)
#

CC=	gcc

DEF:= speed_run

SOL_N:= 2 3 4

SOLS:= $(addprefix $(DEF)_,$(SOL_N))

Z_SUF:= _with_zlib

DEF_Z:= ${DEF}$(Z_SUF)

$(DEF)_%:		speed_run.c make_custom_pdf.c
	${CC} -Wall -O2 -DSOL$* -D_use_zlib_=0 speed_run.c -o $@ -lm

$(DEF):			$(SOLS)

clean:
	rm -f *.pdf *.o $(DEF) $(SOLS)

all:		clean $(DEF)

fclean:		clean
	rm -f a.out ${DEF} ${DEF_Z}

re:			fclean all
