//
// Copyright (C) 2011-14 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

#include "inc_gtest.hpp"
#include "dynd_assertions.hpp"

#include <dynd/array.hpp>
#include <dynd/func/functor_arrfunc.hpp>

using namespace std;
using namespace dynd;

int func0(int x, int y)
{
	return 2 * (x - y);
}

double func1(double x, int y)
{
  return x + 2.6 * y;
}

/*
void func2(float x)
{
  x = 5.0f;
}
*/

TEST(Apply, Function)
{
	nd::arrfunc af;

	af = nd::make_apply_arrfunc<decltype(&func0), &func0>();
	EXPECT_EQ(4, af(5, 3).as<int>());
	af = nd::make_apply_arrfunc(func0);
	EXPECT_EQ(4, af(5, 3).as<int>());

	af = nd::make_apply_arrfunc<decltype(&func1), &func1>();
	EXPECT_EQ(53.15, af(3.75, 19).as<double>());
	af = nd::make_apply_arrfunc(func1);
	EXPECT_EQ(53.15, af(3.75, 19).as<double>());

//  af = nd::make_apply_arrfunc<decltype(funce2)
}

TEST(Apply, FunctionWithKeywords)
{
  nd::arrfunc af;

  af = nd::make_apply_arrfunc<decltype(&func0), &func0>("y");
  EXPECT_EQ(4, af(5, kwds("y", 3)).as<int>());
  af = nd::make_apply_arrfunc(func0, "y");

  af = nd::make_apply_arrfunc<decltype(&func1), &func1>("y");
  EXPECT_EQ(53.15, af(3.75, kwds("y", 19)).as<double>());
}

struct callable0
{
  int operator ()(int x, int y) const
  {
	 return func0(x, y);
  }
};

struct callable1
{
  double operator ()(double x, int y) const
  {
	return func1(x, y);
  }
};

struct callable2
{
  int m_y;

  callable2(int y) : m_y(y) {
  }

  int operator ()(int x) const
  {
   return func0(x, m_y);
  }
};

TEST(Apply, Callable)
{
  nd::arrfunc af;

  af = nd::make_apply_arrfunc<callable0>();
  EXPECT_EQ(4, af(5, 3).as<int>());
  af = nd::make_apply_arrfunc(callable0());
  EXPECT_EQ(4, af(5, 3).as<int>());

  af = nd::make_apply_arrfunc<callable1>();
  EXPECT_EQ(53.15, af(3.75, 19).as<double>());
  af = nd::make_apply_arrfunc(callable1());
  EXPECT_EQ(53.15, af(3.75, 19).as<double>());
}

TEST(Apply, CallableWithKeywords)
{
  nd::arrfunc af;

  af = nd::make_apply_arrfunc<callable2, int>("y");
  EXPECT_EQ(4, af(5, kwds("y", 3)).as<int>());
}

TEST(Apply, MemberFunction)
{

}