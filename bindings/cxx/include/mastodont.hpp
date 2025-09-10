/*
 * Licensed under BSD 3-Clause License
 */

#ifndef MASTODONT_CXX_MASTODONT_HPP
#define MASTODONT_CXX_MASTODONT_HPP

#include <memory>
#include <mastodont.h>

namespace mstdnt
{

class Mastodont
{
public:
	Mastodont();
	~Mastodont() = default;

private:
	std::shared_ptr<mastodont_t> inst;
};

}

#endif
