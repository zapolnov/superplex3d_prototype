/*
 *
 * State_Base.h
 *
 * This is the header file of a pure virtual base class for the 
 * state class that will be written by the
 * programmer for his/her own purpose
 * 
 * The derivation from this class is required
 * and the name of the new class should be 
 * "State" with the necessary functions defined.
 * 
 * */

#ifndef STATE_BASE_H
#define STATE_BASE_H

namespace AStar
{

class State_Base{
	protected:
		int f,g,h;										/* f, g, h values */
		virtual int calculate_g(State_Base *parent)=0;	/* calculation of the cost so far */
		virtual int calculate_h(State_Base *parent)=0;	/* calculation of the heuristic that approximates the further cost */
	public:
		State_Base();
		virtual void print()=0;							/* to print the state data */
		int calculate_f(State_Base *parent);			/* calculation of f (f=g+h) */
		virtual State_Base** Branch(int &nodes_n)=0;	/* states after branching is returned and the number of
														   non-NULL states in the returned array is saved in the variable nodes_n */
		bool is_leaf() const;							/* leaf control for an admissible heuristic function; the test of h==0*/

		int get_f() const;								/* get functions for the variables */
		int get_g() const;
		int get_h() const;

		void set_f(int);								/* get functions for the variables */
		void set_g(int);
		void set_h(int);
};

}

#endif

