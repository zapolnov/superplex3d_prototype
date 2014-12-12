/*
 *
 * State.cpp
 *
 *
 * */

#include <core/logger.h>
#include "a_star_state_base.h"
#include "a_star_state.h"

namespace AStar
{


#if 0
/* Initialization of the environment */
Environment::Environment(int **array, int sx, int sy, Location s, Location t){
	map = array;
	size_x  = sx;
	size_y  = sy;
	start = s;
	target = t;
	map[s.x][s.y] = START;
	map[t.x][t.y] = TARGET;
}

void Environment::print(){
/*	for(int i=-1; i<=size_x; i++,cout << endl){
		if(-1==i || size_x==i){
			for(int j=-1; j<=size_y; j++)
				cout << '#';
		}
		else{
			cout << '#';
			for(int j=0; j<size_y; j++){
				switch(map[i][j]){
					case EMPTY: cout << '-'; break;
					case TOUCHED: cout << 'x'; break;
					case OBSTACLE: cout << 'O'; break;
					case START: cout << 's'; break;
					case TARGET: cout << 't'; break;
				}
			}
			cout << '#';
		}
	}*/
}

void Environment::touch(Location ll){
	int tmp = map[ll.x][ll.y];
	if(EMPTY==tmp)
		map[ll.x][ll.y] = TOUCHED;
}

int** Environment::get_map(){ return map; }

Location Environment::get_target() const { return target; }

int Environment::get_size_x() const {return size_x;}
int Environment::get_size_y() const {return size_y;}

Environment::~Environment() {}

Environment* State::env = NULL;

#endif

State::State(){
	calculate_f(NULL);
	last_movement = START;
}

State::~State(){}

State::State(GameField * map, const Location & start, const Location & target)
	: coor(start), m_Start(start), m_Target(target)
{
	Q_ASSERT(start.x != target.x || start.y != target.y);

	for (int x = 0; x < GAME_FIELD_WIDTH; x++)
	{
		for (int y = 0; y < GAME_FIELD_HEIGHT; y++)
		{
			if (start.x == x && start.y == y)
				m_Map[x][y] = START_POINT;
			else if (target.x == x && target.y == y)
				m_Map[x][y] = TARGET;
			else if ((*map)[x][y])
				m_Map[x][y] = OBSTACLE;
			else
				m_Map[x][y] = EMPTY;
		}
	}

	calculate_f(NULL);
	last_movement = START;
}

State::State(State& parent,Direction operation){
	*this = parent;
	switch(operation){
		case UP: --coor.x; break;
		case LEFT: --coor.y; break;
		case DOWN: ++coor.x; break;
		case RIGHT: ++coor.y; break;
		default: break;
	}

	m_Map[coor.x][coor.y] = TOUCHED;
	last_movement = operation;

	calculate_f(&parent);
	last_movement = operation;
}

/* For each branch branched_nodes are initialized with a four element array. The
 * members of the array with NULL are inappropriate nodes. These are not
 * taken into account in A_Star::solve()  */
State_Base** State::Branch(int &nodes_n){
	nodes_n = 4;
	State_Base **branched_nodes = new State_Base* [nodes_n];

	for(int i=0;i<nodes_n;i++)
		branched_nodes[i] = NULL;

	int tmp;
	if(coor.x>0 && 
		(EMPTY==(tmp=m_Map[coor.x-1][coor.y]) || TARGET==tmp))
			branched_nodes[0] = new State(*this,UP);
	if(coor.x<GAME_FIELD_WIDTH-1 && 
		(EMPTY==(tmp=m_Map[coor.x+1][coor.y]) || TARGET==tmp))
			branched_nodes[1] = new State(*this,DOWN);
	if(coor.y>0 && 
		(EMPTY==(tmp=m_Map[coor.x][coor.y-1]) || TARGET==tmp))
			branched_nodes[2] = new State(*this,LEFT);
	if(coor.y<GAME_FIELD_HEIGHT-1 && 
		(EMPTY==(tmp=m_Map[coor.x][coor.y+1]) || TARGET==tmp))
			branched_nodes[3] = new State(*this,RIGHT);

	return branched_nodes;
}

/* Prints the state information and the last operation is 
 * shown as the move of the appropriate neighboring block 
 * of the empty space */
void State::print(){
	logger << LOG_TRACE << "(%d,%d)" << coor.x << coor.y;
} 

/* Number of steps gone so far */
int State::calculate_g(State_Base *parent){
	if(!parent)
		return g=0;
	return (g = parent->get_g() + 1);
}

int State::abs(int x){ return x<0?-x:x; }

/* Approximate number of displacements that will be taken to reach the goal.
 * This is the total of Mahalonobis distance of each block to its exact place
 * in the final state */
int State::calculate_h(State_Base *parent){
	if(!parent)
		return h=1;
	return h = abs(coor.x - m_Target.x) + abs(coor.y - m_Target.y);
}

//Environment* State::get_env() { return env; }

}
