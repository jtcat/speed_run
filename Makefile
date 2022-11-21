#
# Tomás Oliveira e Silva, AED, September 2022
#
# makefile to compile the A.01 assignment (speed run)
#

CC=	gcc

DEF:= speed_run

SOL_N:= 2 3 4

SOLS:= $(addprefix $(DEF)_,$(SOL_N))

#
# Flag control
#
PRINT_PDF:= -D_print_pdf_
USE_ZLIB:= 0

#
# Targets
#

all:		clean $(DEF)

$(DEF):		$(SOLS);

$(DEF)_%:	speed_run.c make_custom_pdf.c
				${CC} -Wall  $(PRINT_PDF) -O2 -DSOL$* -D_use_zlib_=$(USE_ZLIB) speed_run.c -o $@ -lm -lz

nopdf:		PRINT_PDF=
nopdf:		clean $(DEF)

clean:
				rm -f *.pdf *.o $(DEF) $(SOLS)

fclean:		clean
				rm -f a.out ${DEF}

re:			fclean all
