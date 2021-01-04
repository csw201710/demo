#include <iostream>
#include <vector>

template<typename iter>
typename iter::value_type mysum(iter begin, iter end)
{
	typename iter::value_type sum = 0;
	for (iter i = begin; i != end; ++i)
		sum += *i;
	return sum;
}

template<typename T>
class A {
public:
	typedef typename T value_type;

};

int main()
{
	A<float> b;
	A<float>::value_type c;
	c = 1.2;
	printf("%.2f ", c);

	int a ;
	printf("%d ", a);
	std::vector<int> v;
	for (int i = 0; i<=100; ++i)
		v.push_back(i);
	std::cout << mysum(v.begin(), v.end()) << '\n';
}

