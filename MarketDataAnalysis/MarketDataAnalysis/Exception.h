#pragma once

#include <cstdio>
#include <exception>

class E : public std::exception {
	const char * msg = nullptr;
	E() {};
public:
	E(const char * s) throw() : msg(s) {}
	const char * what() const throw() { return msg; }
};

