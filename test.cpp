#include "numeric_domain.hpp"

using namespace numeric_domain;

#include <iostream>
#include <sstream>
#include <string>

template <typename T>
std::string print_min_and_max_of_bounds() {
	std::ostringstream oss;
	oss << " (min: " << +numeric_domain<T>::min() << ", max: " << +numeric_domain<T>::max() << ") ";
	return oss.str();
}

template <typename T>
void convert(value_type_of<T> v) {
	std::cout << "          " << v << std::endl;
	std::cout << " float11: " << +domain_cast<float11,T>(v) << print_min_and_max_of_bounds<float11>() << std::endl;
	std::cout << " float01: " << +domain_cast<float01,T>(v) << print_min_and_max_of_bounds<float01>() << std::endl;
	std::cout << " float05: " << +domain_cast<float_0_and_0_5,T>(v) << print_min_and_max_of_bounds<float_0_and_0_5>() << std::endl;
	std::cout << " uint8_t: " << +domain_cast<uint8_t,T>(v) << print_min_and_max_of_bounds<uint8_t>() << std::endl;
	std::cout << "  int8_t: " << +domain_cast<int8_t,T>(v) << print_min_and_max_of_bounds<int8_t>() << std::endl;
	std::cout << "   uint7: " << +domain_cast<unsigned_int<7>,T>(v) << print_min_and_max_of_bounds<unsigned_int<7>>() << std::endl;
	std::cout << "    int7: " << +domain_cast<signed_int<7>,T>(v) << print_min_and_max_of_bounds<signed_int<7>>() << std::endl;
	std::cout << "  uint12: " << +domain_cast<unsigned_int<12>,T>(v) << print_min_and_max_of_bounds<unsigned_int<12>>() << std::endl;
	std::cout << "   int12: " << +domain_cast<signed_int<12>,T>(v) << print_min_and_max_of_bounds<signed_int<12>>() << std::endl;
	std::cout << "uint16_t: " << +domain_cast<uint16_t,T>(v) << print_min_and_max_of_bounds<uint16_t>() << std::endl;
	std::cout << " int16_t: " << +domain_cast<int16_t,T>(v) << print_min_and_max_of_bounds<int16_t>() << std::endl;
	std::cout << std::endl;
}

#include <random>
#include <vector>

int main(int argc, char** argv) {
	std::random_device r;
	std::default_random_engine e(r());

	std::normal_distribution<float> dist;

	std::cout << "STATIC DOMAINS:" << std::endl << std::endl;

	std::cout << "* from float11:" << std::endl << std::endl;
	convert<float11>(-1);
	convert<float11>(0);
	convert<float11>(0.5);
	convert<float11>(1);
	convert<float11>(dist(e));

	std::cout << "* from float01:" << std::endl << std::endl;
	convert<float01>(-1);
	convert<float01>(0);
	convert<float01>(0.5);
	convert<float01>(1);
	convert<float01>(dist(e));

	std::cout << "* from unsigned_int<12>:" << std::endl << std::endl;
	convert<unsigned_int<12>>(600);
	convert<unsigned_int<12>>(0);
	convert<unsigned_int<12>>(1300);
	convert<unsigned_int<12>>(6000);
	convert<unsigned_int<12>>(dist(e));

	std::cout << "DYNAMIC DOMAINS:" << std::endl << std::endl;

	std::cout << "150<dynamic float(100,200)> to dynamic int8(-10, 50): " << +domain_cast(make_domain<int8_t>(-10, 50), 150, make_domain(100.0f, 200.0f)) << std::endl;
	std::cout << "2047<static uint12> to dynamic float(100,200): " << +domain_cast<unsigned_int<12>>(make_domain(100.0f, 200.0f), 2047) << std::endl;
	std::cout << "150<dynamic float(100,200)> to static uint12: " << +domain_cast<unsigned_int<12>>(150, make_domain(100.0f, 200.0f)) << std::endl;

	return 0;
}