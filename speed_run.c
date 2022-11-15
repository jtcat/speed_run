
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
// the (very inefficient) recursive solution given to the students
//

static solution_t solution_1,solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem

static void solution_1_recursion(int move_number,int position,int speed,int final_position)
{
  int i,new_speed;

  // record move
  solution_1_count++;
  solution_1.positions[move_number] = position;
  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_1_best.n_moves)
    {
      solution_1_best = solution_1;
      solution_1_best.n_moves = move_number;
    }
    return;
  }
  // no, try all legal speeds
  for(new_speed = speed - 1;new_speed <= speed + 1;new_speed++)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++)
        ;
      if(i > new_speed)
        solution_1_recursion(move_number + 1,position + new_speed,new_speed,final_position);
    }
}

static void solve_1(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }
  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;
  solution_1_best.n_moves = final_position + 100;
  solution_1_recursion(0,0,0,final_position);
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}

//
// Improved version of original recursive func

static solution_t solution_2;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem

static int solution_2_recursion(int move_number,int position,int speed,int final_position)
{
  int i,new_speed;

  // record move
  solution_2_count++;
  solution_2.positions[move_number] = position;
  // Solution found
  if(position == final_position && speed == 1)
  {
    solution_2.n_moves = move_number;
    return 1;
  }
  // Try all legal speeds. Fastest first
  for(new_speed = speed + 1;new_speed >= speed - 1;new_speed--)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++)
        ;
      if(i > new_speed)
        if (solution_2_recursion(move_number + 1,position + new_speed,new_speed,final_position))
	  		return 1;
    }
  return 0;
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
  solution_2.n_moves = final_position + 100;
  solution_2_recursion(0,0,0,final_position);
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}

//
// Dynamic solution
//

static solution_t solution_3;
static double solution_3_elapsed_time; // time it took to solve the problem
static unsigned long solution_3_count; // effort dispended solving the problem

//	Sum 1 to n: stopping distance going at speed n
static int sum1ton(int n)
{
	return n * (n + 1) / 2;
}

//	Checks if it is possible to stop before or at finalpos going at speed from pos
static int valstop(int pos, int speed, int finalpos)
{
	return (pos + sum1ton(speed)) <= finalpos;
}

// Checks if step from pos at speed breaks any of the intermediary speed limits
static int valstep(int pos, int speed)
{
	int	end = pos + speed;
	for (; pos <= end && speed <= max_road_speed[pos]; pos++)
		;
	if (pos <= end)
		return 0;
	return 1;
}

/*		The solution works by increasing the speed as much as possible
	without overstepping the finalpos or breaking any speed limits.

		In a move, if any of the those two checks fail, the program attemps to
	mantain or decrease the speed of the car. If the two checks don't work
	for any of the possible speeds, the program moves back one step and
	retries it with the previous speed reduce by one.
*/

static void solution_3_dynamic(int final_position)
{
	// Current move
	#define	move solution_3.n_moves

	// Car position
	#define pos solution_3.positions[move]
	#define nextpos solution_3.positions[move+1]

	// Current speed "choice"
	#define incmax incmaxes[move]

	// Stores the "choice" taken at every move (slowdown, cruise, accel)
	int	incmaxes[1 + final_position];

	// Current speed
	int	speed = 0;
	
	pos = 0;
	move = 0;
	incmax = 1;
mainloop:
	while (pos != final_position)
	{
  		solution_3_count++;
		for (; incmax >= -1; incmax--)
		{
			if (valstop(pos, speed + incmax, final_position) && valstep(pos, speed + incmax))
		   	{
		   		// Found valid step, take it
		   		speed += incmax;
		   		nextpos = pos + speed;
		   		move++;
		   		incmax = 1;
		   		// Jump to main loop to see if it reaches the
		   		// end or try the next one, avoiding the fail
		   		// state after the two fors
		   		goto mainloop;
		   }
		}
		/*
			There are no possible steps in the current move,
			so lets try the previous move with it's speed reduced by one

			Move the program back one move by reverting 
			the prev speed change and by decrementing the
			move count. Then, choose the next smaller speed.
		*/
		move--;
		speed -= incmax;
		incmax--;
	}
}

static void solve_3(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_3: bad final_position\n");
    exit(1);
  }
  solution_3_elapsed_time = cpu_time();
  solution_3_count = 0;
  solution_3_dynamic(final_position);
  solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}
//
// example of the slides
//

static void example(void)
{
  int i,final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;
  solve_1(final_position);
  make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
  printf("mad road speeds:");
  for(i = 0;i <= final_position;i++)
    printf(" %d",max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for(i = 0;i <= solution_1_best.n_moves;i++)
    printf(" %d",solution_1_best.positions[i]);
  printf("\n");
}


//
// main program
//

int main(int argc,char *argv[argc + 1])
{
# define _time_limit_  15.0
  int n_mec,final_position,print_this_one;
  char file_name[64];

  // generate the example data
  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    example();
    return 0;
  }
  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;
  #define PDF_SRC "./doc"
  #define FILENAME(num) sprintf(file_name,PDF_SRC "/%03d_" #num ".pdf",final_position);
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");
  while(final_position <= _max_road_size_/* && final_position <= 20*/)
  {
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |",final_position);
    // first solution method (very bad)
//    if(solution_1_elapsed_time < _time_limit_)
//    {
//      solve_1(final_position);
//      if(print_this_one != 0)
//      {
//		FILENAME(1)
//        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
//      }
//      printf(" %3d %16lu %9.3e |",solution_1_best.n_moves,solution_1_count,solution_1_elapsed_time);
//    }
//    else
//    {
//      solution_1_best.n_moves = -1;
//      printf("                                |");
//    }
    // second solution method (less bad)
    if(solution_2_elapsed_time < _time_limit_)
    {
      solve_2(final_position);
      if(print_this_one != 0)
      {
		FILENAME(2)
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_2.n_moves,&solution_2.positions[0],solution_2_elapsed_time,solution_2_count,"Optimized recursion");
      }
      printf(" %3d %16lu %9.3e |",solution_2.n_moves,solution_2_count,solution_2_elapsed_time);
    }
    else
    {
      solution_2.n_moves = -1;
      printf("                                |");
    }

	// third solution method
    if(solution_3_elapsed_time < _time_limit_)
    {
      solve_3(final_position);
      if(print_this_one != 0)
      {
		FILENAME(3)
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_3.n_moves,&solution_3.positions[0],solution_3_elapsed_time,solution_3_count,"Dynamic(?) solution");
      }
      printf(" %3d %16lu %9.3e |",solution_3.n_moves,solution_3_count,solution_3_elapsed_time);
    }
    else
    {
      solution_3.n_moves = -1;
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
