/*
 *
 * State.h
 *
 * The header file of the example class State.
 * The purpose of this class is to show how to 
 * use heritage from State_Base class to find
 * the direction to the target block on a block
 * world.
 *
 * */

#ifndef STATE_H
#define STATE_H

#include <core/common.h>
#include "lines_game.h"
#include "ball.h"
#include "a_star_state_base.h"

namespace AStar
{

struct Location
{
	int x,y;

	inline Location()
		: x(-1), y(-1)
	{
	}

	inline Location(int _x, int _y)
		: x(_x), y(_y)
	{
	}
};

enum MapCellState {EMPTY=0,OBSTACLE,TOUCHED,START_POINT,TARGET};

#if 0
/* Environment class is used to keep the map of the world, start and target
 * locations.
 * */
class Environment{
	private:
		int **map;				/* the map of the world */
		Location start, target;	/* the start and the destination */
		int size_x, size_y;		/* the size of the world. x is the horizontal axis */
	public:
		Environment(int **array, int sx, int sy, Location s, Location t);
		~Environment();
		void touch(Location);	/* used to mark a location as TOUCHED */
		void print();			/* prints the world as START (s), TARGET (t), TOUCHED (x), EMPTY (-) and OBSTACLE (O) */

		/* get functions */
		int ** get_map();
		Location get_target() const;
		int get_size_x() const;
		int get_size_y() const;
};
#endif

class State : public State_Base{
    private:
    	MapCellState m_Map[GAME_FIELD_WIDTH][GAME_FIELD_HEIGHT];
//		static Environment *env;	/* Since the environment is unique, it is kept as static variable */
		Location coor;				/* The current location defines the state */
		Location m_Start;
		Location m_Target;
		typedef enum {START,UP,LEFT,DOWN,RIGHT} Direction;
		Direction last_movement;	/* the last movement is kept */
	public:
		State();

		/* this is the for the first state, and it config is initialized with initial_map and other variables */
		State(GameField * map, const Location & start, const Location & target);

		/* operation denotes the direction of the movement */
		State(State & parent,Direction operation);

		~State();

		inline QPoint location() const
		{
			return QPoint(coor.x, coor.y);
		}

		/* branch new states from the state */
		State_Base** Branch(int &nodes_n);

		/* print, calculate_g and calculate_h are pure virtual functions in the base class */
		/* Print the state configuration */
		void print();
	
		/* Number of displacements done so far */
		int calculate_g(State_Base *parent);

		/* Approximate number of displacements */
		int calculate_h(State_Base *parent);

		/* absolute value */
		int abs(int x);

		/* returns env */
//		Environment* get_env();
};

}

#endif
