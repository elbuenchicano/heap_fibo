#pragma once
#include <iostream>
#include <list>
#include <stack>
#include <vector>
using namespace std;

template <class t, bool rule>
struct comp {
	bool operator()(t a, t b) {
		return rule ? a < b : a > b;
	}
};

////////////////////////////////////////////////////////////////////////////////
//..............................................................................
template <class t>
struct node_heap {
	
	typedef			node_heap<t> *	pointer;
	
	pointer			bro_[2],
					anc_		= nullptr,
					main_son_	= nullptr;

	int				rank_	= 0;
	
	t				dato_;
	
	//..........................................................................
	node_heap(t dato):dato_(dato){	}
	~node_heap() {}

	//..........................................................................
};

template <typename t>
ostream & operator<< (ostream & os, const node_heap<t> & n) {
	os << "[dato : " << n.dato_;
	os << " padre: ";
	auto value = n.anc_ ? n.anc_->dato_ : 0;
	os << value;
	os << "]\n";
	return os;
}

////////////////////////////////////////////////////////////////////////////////
//..............................................................................
struct heap_tr {
	typename	typedef		int					type;
	typename	typedef		comp<type, true>	comp;
	typename	typedef		node_heap<type>		node;
};

////////////////////////////////////////////////////////////////////////////////
//..............................................................................
template <typename tr>
struct heap
{
	typename typedef	tr::type	type;
	typename typedef	tr::comp	comp;
	typename typedef	tr::node	node;

	//..........................................................................
	int n_nodos_	= 0,
		n_mark_		= 0,
		n_trees		= 0; 

	node *main_ = nullptr;
	comp cmp;
	
	//..........................................................................
	heap() {}
	~heap() {}

	//..........................................................................
	void insert(type dato) {
		node * newnode = new node(dato);
		if (!main_) {
			main_ = newnode;
			main_->bro_[0] = main_;
			main_->bro_[1] = main_;
		}
		else {
			insert_in_place(main_, newnode);
			if (cmp(dato, main_->dato_))
				main_ = newnode;
		}		
		++n_nodos_;
	}

	//..........................................................................
	void insert_in_place(node * &ptr1, node * ptr2) {
		if (ptr1) {
			ptr1->bro_[1]->bro_[0] = ptr2;
			ptr2->bro_[1] = ptr1->bro_[1];
			ptr1->bro_[1] = ptr2;
			ptr2->bro_[0] = ptr1;
		}
		else {
			ptr1 = ptr2;
			ptr2->bro_[1] = ptr2;
			ptr2->bro_[0] = ptr2;

		}
	}

	void erase_from_place(node * ptr1) {
		if (ptr1 == main_)
			main_ = ptr1->bro_[1];
		ptr1->bro_[0]->bro_[1] = ptr1->bro_[1];
		ptr1->bro_[1]->bro_[0] = ptr1->bro_[0];
	}

	void link_anc(node * anc, node * des) {
		if (anc->rank_ <= des->rank_)
			anc->rank_ = des->rank_ + 1;
		des->anc_ = anc;
		erase_from_place(des);
		insert_in_place(anc->main_son_, des);
	}

	//..........................................................................
	void show() {
		show_single(main_);	
		cout << endl;
	}

	void show_single(node * tmp) {
		if (tmp) {
			cout << *tmp;
			show_single(tmp->main_son_);

			for (	auto  ite = tmp->bro_[1], first = tmp; ite != first ; ite = ite->bro_[1] ){
				cout << *ite;
				show_single(ite->main_son_);
			}
		}
	}
	//..........................................................................
	void pop() {
		erase_top();
		
		consolidate();
	}

	void erase_top() {
		//adding main sons into the main rank
		auto  ite = main_->main_son_;
		auto  first = ite;
		if (ite) {
			do {
				ite->anc_	= nullptr;
				--ite->rank_;
				insert_in_place(main_, ite);
			} while (ite != first);
		}

		//erasing the old main
		auto tmp	= main_;
		erase_from_place(main_);
		main_ = main_->bro_[0];
		delete tmp;
		--n_nodos_;
	}

	void consolidate() {
		int  size_table = log(n_nodos_) / log(1.618);
		
		for (auto flag = true; flag; ) {
			vector<node *> rank_table(size_table);
			fill(rank_table.begin(), rank_table.end(), nullptr);
			rank_table[main_->rank_]  = main_;
			flag = false;

			show();

			for (auto next = main_->bro_[1]; next != main_; next = next->bro_[1]) {
				if(!rank_table[next->rank_])
					rank_table[next->rank_] = next;
				else {
					flag = true;
					if (cmp(rank_table[next->rank_]->dato_, next->dato_))
						link_anc(rank_table[next->rank_], next);
					else
						link_anc(next, rank_table[next->rank_]);
					break;
				}
			}
		}
	}	

//CLASS END
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
};