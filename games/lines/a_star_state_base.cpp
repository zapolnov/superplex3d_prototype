/*
 *
 * State_Base.cpp
 *
 * This is a virtual base class for the 
 * state class that will be written by the
 * programmer for his/her own purpose
 *
 * The derivation from this class is required
 * and the name of the new class should be 
 * "State" with the necessary functions defined.
 *
 * */

#include <limits>
#include "a_star_state_base.h"

namespace AStar
{

State_Base::State_Base() {}

int State_Base::calculate_f(State_Base *parent){ return f = (calculate_g(parent) + calculate_h(parent)); }
bool State_Base::is_leaf() const {return (0==h);}

int State_Base::get_f() const {return f;}
int State_Base::get_g() const {return g;}
int State_Base::get_h() const {return h;}

void State_Base::set_f(int x){f=x;}
void State_Base::set_g(int x){g=x;}
void State_Base::set_h(int x){h=x;}


}
