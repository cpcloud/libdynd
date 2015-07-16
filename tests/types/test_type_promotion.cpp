//
// Copyright (C) 2011-15 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <iostream>
#include <stdexcept>
#include <cmath>
#include "inc_gtest.hpp"

#include "dynd/type_promotion.hpp"

using namespace std;
using namespace dynd;

template<class S, class T, class U>
void type_promotion_matches_cxx_test(S, T, U) {
    EXPECT_EQ(ndt::type::make<U>(), promote_types_arithmetic(ndt::type::make<S>(), ndt::type::make<T>()));
    if (ndt::type::make<U>() != promote_types_arithmetic(ndt::type::make<S>(), ndt::type::make<T>()))
        cout << "S: " << ndt::type::make<S>() << ", T: " << ndt::type::make<T>() << ", U: " << ndt::type::make<U>() << "\n";
}

template<class S, class T>
void type_promotion_matches_cxx() {
    S a = S();
    T b = T();
    type_promotion_matches_cxx_test(a, b, a + b);
}

#define TEST_ALL_SECOND(first) \
    type_promotion_matches_cxx<first, bool1>(); \
    type_promotion_matches_cxx<first, int8_t>(); \
    type_promotion_matches_cxx<first, int16_t>(); \
    type_promotion_matches_cxx<first, int32_t>(); \
    type_promotion_matches_cxx<first, int64_t>(); \
    type_promotion_matches_cxx<first, uint8_t>(); \
    type_promotion_matches_cxx<first, uint16_t>(); \
    type_promotion_matches_cxx<first, uint32_t>(); \
    type_promotion_matches_cxx<first, uint64_t>(); \
    type_promotion_matches_cxx<first, float>(); \
    type_promotion_matches_cxx<first, double>()

#define TEST_ALL_FIRST() \
    TEST_ALL_SECOND(bool1); \
    TEST_ALL_SECOND(int8_t); \
    TEST_ALL_SECOND(int16_t); \
    TEST_ALL_SECOND(int32_t); \
    TEST_ALL_SECOND(int64_t); \
    TEST_ALL_SECOND(uint8_t); \
    TEST_ALL_SECOND(uint16_t); \
    TEST_ALL_SECOND(uint32_t); \
    TEST_ALL_SECOND(uint64_t); \
    TEST_ALL_SECOND(float); \
    TEST_ALL_SECOND(double)

TEST(DTypePromotion, MatchesCxx) {
    TEST_ALL_FIRST();
}

#undef TEST_ALL_FIRST
#undef TEST_ALL_SECOND

TEST(DTypePromotion, IntWithComplex) {
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int8_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int16_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int32_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int64_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint8_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint16_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint32_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint64_t>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());

    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int8_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int16_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int32_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<int64_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint8_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint16_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint32_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<uint64_t>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
}

TEST(DTypePromotion, FloatWithComplex) {
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<float>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<float>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<double>(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<double>(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<float> >(), ndt::type::make<float>()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<double> >(), ndt::type::make<float>()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<float> >(), ndt::type::make<double>()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<double> >(), ndt::type::make<double>()), ndt::type::make<dynd::complex<double> >());
}

TEST(DTypePromotion, ComplexWithComplex) {
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<float> >(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<float> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<float> >(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<double> >(), ndt::type::make<dynd::complex<float> >()), ndt::type::make<dynd::complex<double> >());
    EXPECT_EQ(promote_types_arithmetic(ndt::type::make<dynd::complex<double> >(), ndt::type::make<dynd::complex<double> >()), ndt::type::make<dynd::complex<double> >());
}
