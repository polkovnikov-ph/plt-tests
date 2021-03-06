#include <iostream>
using namespace std;

// Meta-level primitives
struct nil {};

template <class F, class S>
struct cons {};

template <typename B, typename D>
struct is_base_of {
	struct test {
		operator B*() const;
		operator D*();
	};
	template <typename T> 
	static char (& check(D*, T))[1];
	static char (& check(B*, int))[2];
	static const bool value = sizeof(check(test(), int())) == 1;
};

template <bool A, class B, class C>
struct if_;
template <class B, class C>
struct if_<true, B, C> {
	typedef B type;
};
template <class B, class C>
struct if_<false, B, C> {
	typedef C type;
};

template <class T>
struct id { typedef T type; };

template <unsigned x>
struct uint_ { static const unsigned value = x; };


// Computation
static const unsigned bitc = 8 * sizeof(unsigned);

template <unsigned i, unsigned x, unsigned s>
struct bit {};

template <unsigned i, unsigned x, unsigned s>
struct bits : bit<i, x & 1, s>, bits<i + 1, (x >> 1), s | ((x & 1) << i)> {};

template <unsigned x, unsigned s>
struct bits<bitc, x, s> {};

template <unsigned x>
struct event : bits<0, x, 0>, uint_<x> {};

template <class T, unsigned i, unsigned S, class L, unsigned d, bool v>
struct improve;

template <class T, unsigned i, unsigned S, class L1>
struct list {
	typedef typename improve<T, i, S, L1, 0, is_base_of<bit<i, 0, S>, T>::value>::type L2;
	typedef typename improve<T, i, S, L2, 1, is_base_of<bit<i, 1, S>, T>::value>::type type;
};

template <class T, unsigned S, class L>
struct list<T, bitc, S, L> : id< cons<event<S>, L> > {};

template <class T, unsigned i, unsigned S, class L, unsigned d>
struct improve<T, i, S, L, d, false> : id<L> {};

template <class T, unsigned i, unsigned S, class L, unsigned d>
struct improve<T, i, S, L, d, true> : list<T, i + 1, S | (d << i), L> {};


// Debug
template <class>
struct print_ids_impl;

template <>
struct print_ids_impl<nil> {
	static void print() {}
};

template <class F, class S>
struct print_ids_impl< cons<F, S> > {
	static void print() {
		cout << F::value << endl;
		print_ids_impl<S>::print();
	}
};

template <class T>
void print_ids() {
	print_ids_impl<typename list<T, 0, 0, nil>::type>::print();
}


// Example
struct test :
	public event<2000000000>,
	public event<3000000000>,
	public event<1>,
	public event<4000000000> {};

int main() {
	print_ids<test>();
}