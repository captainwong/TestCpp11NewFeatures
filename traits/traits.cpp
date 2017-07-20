#include <iostream>
#include <cstdlib>
#include <type_traits>
#include <cassert>
#include <random>
#include <limits>

namespace test1 {
template <typename T>
void fool_impl(const T& val, std::true_type)
{
	std::cout << "foo() called for pointer to " << *val << std::endl;
}

template <typename T>
void fool_impl(const T& val, std::false_type)
{
	std::cout << "foo() called for value to " << val << std::endl;
}

template <typename T>
void foo(const T& val)
{
	fool_impl(val, std::is_pointer<T>());
}

void test()
{
	std::cout << "--------------------------test1--------------------------" << std::endl;
	foo(1);
	int i = 0;
	foo(&i);
}
}


namespace test2 {
template <typename T1, typename T2>
typename std::common_type<T1, T2>::type minimum(const T1& x, const T2& y)
{
	return x < y ? x : y;
}

void test()
{
	std::cout << "--------------------------test2--------------------------" << std::endl;
	int x = 2;
	long y = 1;
	std::cout << "return type is " << typeid(minimum(x, y)).name() << std::endl
		<< "result is " << minimum(x, y) << std::endl;
}

}


namespace test3 {
template< class T >
T findMax(const T* const data, const size_t numItems) 
{
	// Obtain the minimum value for type T 
	T largest = std::numeric_limits<T>::min();
	for (unsigned int i = 0; i < numItems; ++i) {
		if (data[i] > largest) {
			largest = data[i];
		}
	}
	return largest;
}

void test()
{
	std::cout << "--------------------------test3--------------------------" << std::endl;
	int data[] = { 2,5,9,34,99,1,0,44,23234,66666 };
	auto val = findMax(data, sizeof(data) / sizeof(int));
	std::cout << "max val is " << val << std::endl;
}

}


namespace test4 {
template <bool b>
struct algorithm_selector {
	template< typename T>
	static void implementation(T& object) {
		object.implementation();
	}
};

template<>
struct algorithm_selector<true> {
	template<typename T>
	static void implementation(T& object) {
		object.optimised_implementation();
	}
};

template<typename T>
struct supports_optimised_implementation {
	static const bool value = false;
};

template<typename T>
void algorithm(T& object) {
	algorithm_selector<supports_optimised_implementation<T>::value>::implementation(object);
}

class ObjectA {
public:
	void implementation() {
		std::cout << "ObjectA::implementation" << std::endl;
	}
};

class ObjectB {
public:
	void optimised_implementation() {
		std::cout << "ObjectB::optimised_implementation" << std::endl;
	}
};

template<>
struct supports_optimised_implementation<ObjectB> {
	static const bool value = true;
};

void test()
{
	std::cout << "--------------------------test4--------------------------" << std::endl;
	ObjectA a;
	algorithm(a);
	ObjectB b;
	algorithm(b);
}

}

namespace test5 {

template <typename T>
struct is_swapable {
	static const bool value = std::is_integral<T>::value && sizeof(T) >= 2;
};

template <typename T>
T byte_swap(T value) {
	std::assert(is_swapable<T>::value && "Cannot swap values of this type");
	unsigned char *bytes = reinterpret_cast< unsigned char * >(&value);
	for (size_t i = 0; i < sizeof(T); i += 2) {
		// Take the value on the left and switch it 
		// with the value on the right
		unsigned char v = bytes[i];
		bytes[i] = bytes[i + 1];
		bytes[i + 1] = v;
	}
	return value;
}

template <typename T>
void test_impl(std::mt19937& rng)
{
	std::cout << "is_swapable " << typeid(T).name() << " :" << is_swapable<T>::value << std::endl;
	std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
	std::cout << "calling is_swapable for " << dist(rng) << std::endl;
}

void test()
{
	std::cout << "--------------------------test5--------------------------" << std::endl;
	std::cout << "is_swapable char " << is_swapable<char>::value << std::endl;
	std::cout << "is_swapable short " << is_swapable<short>::value << std::endl;
	std::cout << "is_swapable unsigned  " << is_swapable<unsigned char>::value << std::endl;
	std::cout << "is_swapable unsigned short " << is_swapable<unsigned short>::value << std::endl;
	std::cout << "is_swapable double " << is_swapable<double>::value << std::endl;
	std::cout << "is_swapable long " << is_swapable<long>::value << std::endl;
	std::cout << "is_swapable float " << is_swapable<float>::value << std::endl;
	std::cout << "is_swapable double " << is_swapable<double>::value << std::endl;

	struct s {
		int hi : 24;
		int lo : 8;
	};

	s s;
	s.hi = 65535;
	s.lo = 255;

	std::cout << "is_swapable s.hi " << is_swapable<decltype(s.hi)>::value << std::endl;
	std::cout << "is_swapable s.lo " << is_swapable<decltype(s.lo)>::value << std::endl;
}

}

int main()
{
	test1::test();
	test2::test();
	test3::test();
	test4::test();
	test5::test();

	std::system("pause");
}