//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. XXXXXX  Name: XXXXXXX
//


//
// static configuration
//

#define _max_road_size_  800  // the maximum problem size
#define _min_road_speed_   2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_   9  // must not be larger than 9 (only because of the PDF figure)


//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "elapsed_time.h"
#include "make_custom_pdf.c"


//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for(i = 0;i <= _max_road_size_;i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
    if(max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if(max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}


//
// description of a solution
//

typedef struct
{
  int n_moves;                         // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_];  // the positions (the first one must be zero)
}
solution_t;

//
//	Custom dynamic solution
//


static solution_t solution_2;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem

//	Sum 1 to n: stopping distance going at speed n
static int sum1ton(int n)
{
	return n * (n + 1) / 2;
}

//	Checks if it is possible to stop before or at finalpos going at speed
//	from pos
static int valstop(int pos, int speed, int finalpos)
{
	return (pos + sum1ton(speed)) <= finalpos;
}

static int valstep(int pos, int speed)
{
	int	end = pos + speed;
	for (; pos <= end; pos++)
		if (speed > max_road_speed[pos])
			return 0;
	return 1;
}

static void solution_2_dynamic(int final_position)
{
	int	pos = 0;
	int	incmaxes[1 + final_position];
	int	move = 0;
	int	speed = 0;
	
	#define incmax incmaxes[move]

	#define MOVEBACK\
		speed -= incmaxes[--move];\
		--incmaxes[move];\
		pos = solution_2.positions[move];
	incmax = 1;
	solution_2.positions[0] = pos;
	while (pos != final_position)
	{
  		solution_2_count++;
		if (valstop(pos, speed + incmax, final_position))
		{
			stepval:
			if (valstep(pos, speed + incmax))
			{
				speed += incmax;
				pos += speed;
				solution_2.positions[++move] = pos;
				incmax = 1;
				continue;
			}
			else
			{
				incmax--;
				if (incmax >= -1)
					goto stepval;
				else
				{
					MOVEBACK
				}
			}		
		}
		else
		{
			incmax--;
			if (incmax >= -1)
				continue;
			else
			{
				MOVEBACK
			}
		}
	}
	solution_2.n_moves = move;
}

static void solve_2(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_2: bad final_position\n");
    exit(1);
  }
  solution_2_elapsed_time = cpu_time();
  solution_2_count = 0ul;
  solution_2_dynamic(final_position);
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}

//
// main program
//

int main(int argc,char *argv[argc + 1])
{
# define _time_limit_  3600.0
  int n_mec,final_position,print_this_one;
  char file_name[64];

  // generate the example data
  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    return 0;
  }
  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_2_elapsed_time = 0.0;
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");
  while(final_position <= _max_road_size_/* && final_position <= 20*/)
  {
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 41 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |",final_position);
	//
	// second solution method (less bad)
    // ...
	if(solution_2_elapsed_time < _time_limit_)
    {
      solve_2(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_2.pdf",final_position);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_2.n_moves,&solution_2.positions[0],solution_2_elapsed_time,solution_2_count,"Dynamic solution");
      }
      printf(" %3d %16lu %9.3e |",solution_2.n_moves,solution_2_count,solution_2_elapsed_time);
    }
    else
    {
      solution_2.n_moves = -1;
      printf("                                |");
    }
    // done
    printf("\n");
    fflush(stdout);
    // new final_position
    if(final_position < 50)
      final_position += 1;
    else if(final_position < 100)
      final_position += 5;
    else if(final_position < 200)
      final_position += 10;
    else
      final_position += 20;
  }
  printf("--- + --- ---------------- --------- +\n");
  return 0;
# undef _time_limit_
}
