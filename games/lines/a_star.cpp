#include <core/common.h>
#include <core/logger.h>
#include "a_star_state.h"
#include "a_star.h"

namespace AStar
{

Tree_Element::Tree_Element() : parent(NULL) {}

Tree_Element::Tree_Element(State_Base *st, Tree_Element *prnt=NULL) {
	Q_ASSERT(st!=NULL);
	set_state(*static_cast<State*>(st));
	set_parent(prnt);
}

Tree_Element::~Tree_Element() {}

void Tree_Element::set_parent(Tree_Element *prnt){ parent = prnt; }
void Tree_Element::set_state(State &st){ state = st; }

Tree_Element* Tree_Element::get_parent() const {return parent;}
State Tree_Element::get_state() const {return state;}


Queue_Element::Queue_Element() : t_element(NULL) {}
Queue_Element::Queue_Element(Tree_Element* te) { set_t_element(te); }
Queue_Element::~Queue_Element() {}
void Queue_Element::set_t_element(Tree_Element *te) { t_element = te; }
Tree_Element* Queue_Element::get_t_element() {return t_element;}


int Prioritize_Queue_Elements::operator() (Queue_Element &x, Queue_Element &y)
	{ return x.get_t_element()->get_state().get_f() > y.get_t_element()->get_state().get_f(); }


State* A_Star::solve(State* initial_state, int& solution_n){
	a_star_state = NOT_FOUND;
	solution_n = 0;
	solution_leaf = NULL;
	solution = NULL;
	root = new Tree_Element(initial_state);
	Queue_Element *qe = new Queue_Element(root);
	Q.push(*qe);

	Queue_Element q_tmp;

	while(!Q.empty()){
		q_tmp = Q.top();
		Q.pop();

		/* The solution is found */
		if(q_tmp.get_t_element()->get_state().get_h()==0){
			a_star_state = FOUND;
			Tree_Element *solution_leaf = q_tmp.get_t_element();
			Tree_Element *t_tmp = solution_leaf;
			int step_n = 0;

			while(t_tmp){
				++step_n;
				t_tmp = t_tmp->get_parent();
			}
			solution_n = step_n;

			solution = new State [step_n];
			t_tmp = solution_leaf;
			for(int i=step_n-1;i>=0;i--){
				solution[i] = t_tmp->get_state();
				t_tmp = t_tmp->get_parent();
			}

			return solution;
		}

		int new_states_n;
		State_Base** branched_states = ((q_tmp.get_t_element())->get_state()).Branch(new_states_n);

		for(int i=0; i<new_states_n; i++){
			if(branched_states[i]!=NULL){
				Tree_Element *t_tmp = new Tree_Element(branched_states[i],(q_tmp.get_t_element()));
				Queue_Element *qe = new Queue_Element(t_tmp);
				Q.push(*qe);
			}
		}
	}

	if(NOT_FOUND==a_star_state){
		logger << LOG_INFO << "A*: The Solution does not exist";
		return NULL;
	}

	return NULL;
}

void A_Star::print(){
	for(int i=0; i<solution_n; i++)
		solution->print();
}

}
