/*
	模拟boost::bind 函数写的例子
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>
#include <set>
#include <assert.h>
#include <string>
#include <memory>
using namespace std;


//#define __SINGLE_FUC
#define __TEMPLATE_FUC

int show() {
	return 110;
}

int add(int a, int b) {
	return a + b;
}

#ifdef  __SINGLE_FUC
// 单一函数绑定
typedef int(*F)(int, int);

struct bind_t {
	bind_t(F f, int a, int b) :f_(f), a_(a), b_(b) {}
	F f_;
	int a_;
	int b_;
	int operator()() {
		return f_(a_, b_);
	}
};
bind_t bind(F f, int a ,int b) {
	return bind_t(f, a, b);
}
#elif defined __TEMPLATE_FUC
template<class R> 
struct type {};

template <int i> 
struct arg {};

arg<1> _1;
arg<1> _2;


struct list0 {
	list0() {}
	template<class R, class F, class L>
	R operator() (type<R>, F f, L l) {
		return f();

	}
};

template<class A1, class A2> 
struct list2 {
	list2(A1 a1, A2 a2) : a1_(a1), a2_(a2) {}

	//unknow to solve
	//A1 operator[](arg<1> a) { return a1_; } 
	//A2 operator[](arg<2> b) { return a2_; } 
	//template<class T>T operator[] (T t) { return t; } 

	template<class R, class F, class L>
	R operator() ( type<R>,F f, L l) {
		return f(l.a1_, l.a2_);
		//return f(l[a1_], l[a2_]);

	}

	A1 a1_;
	A2 a2_;
};

template <class R, class F, class L>
struct bind_t {
	F f_;
	L l_;
	bind_t(F f, L l) : f_(f), l_(l) {}

	R operator()() {
		return l_(type<R>(), f_, l_);
	}

	template<class A1, class A2> 
	R operator()(A1 && a1, A2 && a2) {
		list2< A1, A2 > a(a1, a2);
		return l_(type<R>(), f_, a);
	}
};

//无参数函数
template<class R>
bind_t<R, R(*) (), list0 >
bind(R(*f) ()) {
	typedef list0 list_type;
	return bind_t<R, R(*) (), list_type>(f, list_type());

}

//1. 函数类型可以拆分  R (*) (B1, B2)
//2. 类型可以向下传递
template<class R,class B1 ,class B2, class A1,class A2>
bind_t<R, R (*) (B1, B2), list2<A1, A2> > 
	bind( R (*f) (B1, B2) ,
			A1 a1, A2 a2) {
	typedef list2<A1, A2> list_type;
	return bind_t<R, R(*) (B1, B2),list_type>(f, list_type(a1, a2));
	
}
#else
#include <boost/bind.hpp>
using namespace boost;
	
#endif
int main() {
	printf("%d\n",bind(add, 2, 3)());
	printf("%d\n", bind(add, _1, _2)(3,4));

	printf("%d\n", bind(show)());
	return 0;
}

