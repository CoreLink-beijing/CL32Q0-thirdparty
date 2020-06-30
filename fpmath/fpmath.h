/* Copyright (c) 2019-2020, corelink inc., www.corelink.vip
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2019-10-15     corelink     the first version
 */

/*
 * IEEE 754 : IEEE Standard for Floating-Point Arithmetic
 *
 * This library is limited to handling numbers with a whole part of
 * up to 2^31 - 1 = 2.147483647e9 in magnitude, and fractional parts
 * down to 2^-32 = 2.3283064365e-10 in magnitude. This yields a decent
 * range and accuracy for many applications.
 *
 * Notice: If you use both "math.h" and "fpmath.h", include "math.h" first!!!
 */


#ifndef __FPMATH_H__
#define __FPMATH_H__


#include <stdint.h>
#include <stdio.h>
#include "./math-sll/math-sll.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef _MATH_H_

#define fabs 		cl_fabs
#define cos			cl_cos
#define sin			cl_sin
#define tan 		cl_tan

#define acos		cl_acos
#define asin 		cl_asin
#define atan		cl_atan

#define cosh		cl_cosh
#define sinh 		cl_sinh
#define tanh 		cl_tanh

#define exp			cl_exp
#define log			cl_log
#define log2		cl_log2
#define log10		cl_log10
#define pow			cl_pow
#define sqrt		cl_sqrt

#define ceil		cl_ceil
#define floor		cl_floor


#define M_E			CL_E
#define M_LOG2E		CL_LOG2E
#define M_LOG10E	CL_LOG10E
#define M_LN2		CL_LN2
#define M_LN10		CL_LN10
#define M_PI		CL_PI
#define M_PI_2		CL_PI_2
#define M_PI_4		CL_PI_4
#define M_1_PI		CL_1_PI
#define M_2_PI		CL_2_PI
#define M_2_SQRTPI	CL_2_SQRTPI
#define M_SQRT2		CL_SQRT2
#define M_SQRT1_2	CL_SQRT1_2

#define M_TWOPI     CL_TWOPI
#define M_3PI_4		CL_3PI_4
#define M_SQRTPI    CL_SQRTPI
#define M_LN2LO     CL_LN2LO
#define M_LN2HI     CL_LN2HI
#define M_SQRT3		CL_SQRT3
#define M_IVLN10    CL_IVLN10
#define M_LOG2_E    CL_LOG2_E
#define M_INVLN2    CL_INVLN2

#endif	/* #ifndef _MATH_H_ */


double cl_fabs (double d);

double cl_cos (double d);
double cl_sin (double d);
double cl_tan (double d);

double cl_acos (double d);
double cl_asin (double d);
double cl_atan (double d);

double cl_cosh (double d);
double cl_sinh (double d);
double cl_tanh (double d);

double cl_exp (double d);
double cl_log (double d);
double cl_log2 (double d);
double cl_log10 (double d);
double cl_pow (double x, double y);
double cl_sqrt (double d);

double cl_ceil (double d);
double cl_floor (double d);


#define CL_E				2.7182818284590452354
#define CL_LOG2E			1.4426950408889634074
#define CL_LOG10E			0.43429448190325182765
#define CL_LN2				0.693147180559945309417
#define CL_LN10				2.30258509299404568402
#define CL_PI				3.14159265358979323846
#define CL_PI_2				1.57079632679489661923
#define CL_PI_4				0.78539816339744830962
#define CL_1_PI				0.31830988618379067154
#define CL_2_PI				0.63661977236758134308
#define CL_2_SQRTPI			1.12837916709551257390
#define CL_SQRT2			1.41421356237309504880
#define CL_SQRT1_2			0.70710678118654752440

#define CL_TWOPI         	(CL_PI * 2.0)
#define CL_3PI_4			2.3561944901923448370E0
#define CL_SQRTPI        	1.77245385090551602792981
#define CL_LN2LO         	1.9082149292705877000E-10
#define CL_LN2HI         	6.9314718036912381649E-1
#define CL_SQRT3			1.73205080756887719000
#define CL_IVLN10        	0.43429448190325182765 	/* 1 / log(10) */
#define CL_LOG2_E        	CL_LN2
#define CL_INVLN2        	1.4426950408889633870E0 /* 1 / log(2) */


/*
 *
CONST_0				// 0.0
CONST_1				// 1.0
CONST_2				// 2.0
CONST_3				// 3.0
CONST_4				// 4.0
CONST_10			// 10.0

CONST_1_2			// 1.0 / 2.0
CONST_1_3			// 1.0 / 3.0
CONST_1_4			// 1.0 / 4.0
CONST_1_5			// 1.0 / 5.0
CONST_1_6			// 1.0 / 6.0
CONST_1_7			// 1.0 / 7.0
CONST_1_8			// 1.0 / 8.0
CONST_1_9			// 1.0 / 9.0
CONST_1_10			// 1.0 / 10.0
CONST_1_11			// 1.0 / 11.0
CONST_1_12			// 1.0 / 12.0
CONST_1_20			// 1.0 / 20.0
CONST_1_30			// 1.0 / 30.0
CONST_1_42			// 1.0 / 42.0
CONST_1_56			// 1.0 / 56.0
CONST_1_72			// 1.0 / 72.0
CONST_1_90			// 1.0 / 90.0
CONST_1_110			// 1.0 / 110.0
CONST_1_132			// 1.0 / 132.0
CONST_1_156			// 1.0 / 156.0

CONST_E				// E
CONST_1_E			// 1 / E
CONST_SQRTE			// sqrt(E)
CONST_1_SQRTE		// 1 / sqrt(E)
CONST_LOG2_E		// ln(E)
CONST_LOG10_E		// log(E)
CONST_LN2			// ln(2)
CONST_LN10			// ln(10)

CONST_PI			// PI
CONST_PI_2			// PI / 2
CONST_PI_4			// PI / 4
CONST_1_PI			// 1 / PI
CONST_2_PI			// 2 / PI
CONST_2_SQRTPI		// 2 / sqrt(PI)
CONST_SQRT2			// sqrt(2)
CONST_1_SQRT2		// 1 / sqrt(2)

CONST_FACT_0		// 0!
CONST_FACT_1		// 1!
CONST_FACT_2		// 2!
CONST_FACT_3		// 3!
CONST_FACT_4		// 4!
CONST_FACT_5		// 5!
CONST_FACT_6		// 6!
CONST_FACT_7		// 7!
CONST_FACT_8		// 8!
CONST_FACT_9		// 9!
CONST_FACT_10		// 10!
CONST_FACT_11		// 11!
CONST_FACT_12		// 12!

fp32_t fp32_from_double(double d);
double fp32_to_double(fp32_t s);
fp32_t fp32_from_int(int i);
int fp32_to_int(fp32_t s);

fp32_t fp32_int_part(fp32_t s);
fp32_t fp32_frac_part(fp32_t s);

fp32_t fp32_add(fp32_t x, fp32_t y);
fp32_t fp32_sub(fp32_t x, fp32_t y);
fp32_t fp32_neg(fp32_t x);
fp32_t fp32_mul(fp32_t x, fp32_t y);
fp32_t fp32_mul2(fp32_t x);
fp32_t fp32_mul2n(fp32_t x, int n);
fp32_t fp32_mul4(fp32_t x);
fp32_t fp32_div(fp32_t x, fp32_t y);
fp32_t fp32_div2(fp32_t x);
fp32_t fp32_div2n(fp32_t x, int n);
fp32_t fp32_div4(fp32_t x);

fp32_t fp32_cos(fp32_t x);
fp32_t fp32_sin(fp32_t x);
fp32_t fp32_tan(fp32_t x);
fp32_t fp32_acos(fp32_t x);
fp32_t fp32_asin(fp32_t x);
fp32_t fp32_atan(fp32_t x);

fp32_t fp32_sec(fp32_t x);
fp32_t fp32_csc(fp32_t x);
fp32_t fp32_cot(fp32_t x);

fp32_t fp32_cosh(fp32_t x);
fp32_t fp32_sinh(fp32_t x);
fp32_t fp32_tanh(fp32_t x);
fp32_t fp32_sech(fp32_t x);
fp32_t fp32_csch(fp32_t x);
fp32_t fp32_coth(fp32_t x);

fp32_t fp32_exp(fp32_t x);
fp32_t fp32_log(fp32_t x);
fp32_t fp32_log2(fp32_t x);
fp32_t fp32_log10(fp32_t x);
fp32_t fp32_pow(fp32_t x, fp32_t y);
fp32_t fp32_sqrt(fp32_t x);
fp32_t fp32_inv(fp32_t x);

fp32_t fp32_floor(fp32_t x);
fp32_t fp32_ceil(fp32_t x);
 *
 */

#define fp32_t	sll

/*
 * double to fp32_t
 * fp32_t fp32_from_double(double d);
 */
#define fp32_from_double 			dbl2sll

/*
 * fp32_t to double
 * double fp32_from_double(fp32_t s);
 */
#define fp32_to_double				sll2dbl

/*
 * int to fp32_t
 * fp32_t fp32_from_int(int i);
 */
#define fp32_from_int 				int2sll

/*
 * fp32_t to int
 * int fp32_to_int(fp32_t s);
 */
#define fp32_to_int 				sll2int

/*
 * Set fractional-part to 0
 * fp32_t fp32_int_part(fp32_t s);
 */
#define fp32_int_part				sllint

/*
 * Set integer-part to 0
 * fp32_t fp32_frac_part(fp32_t s);
 */
#define fp32_frac_part				sllfrac

/*
 * add(x, y) = x + y
 * fp32_t fp32_add(fp32_t x, fp32_t y);
 */
#define fp32_add					slladd

/*
 * sub(x, y) = x - y
 * fp32_t fp32_sub(fp32_t x, fp32_t y);
 */
#define fp32_sub					sllsub

/*
 * neg(x) = -x
 * fp32_t fp32_neg(fp32_t x);
 */
#define fp32_neg					sllneg

/*
 * mul(x) = x * y
 * fp32_t fp32_mul(fp32_t x, fp32_t y);
 */
#define fp32_mul					sllmul

/*
 * mul2(x) = x * 2
 * fp32_t fp32_mul2(fp32_t x);
 */
#define fp32_mul2					sllmul2

/*
 * mul2n(x, n) = x * 2^n, 0 <= n <= 31
 * fp32_t fp32_mul2n(fp32_t x, int n);
 */
#define fp32_mul2n					sllmul2n

/*
 * fp32_mul4(x) = x * 4
 * fp32_t fp32_mul4(fp32_t x);
 */
#define fp32_mul4					sllmul4

/*
 * div(x, y) = x / y
 * fp32_t fp32_div(fp32_t x, fp32_t y);
 */
#define fp32_div					slldiv

/*
 * div2(x) = x / 2
 * fp32_t fp32_div2(fp32_t x);
 */
#define fp32_div2					slldiv2

/*
 * div2n(x, n) = x / 2^n, 0 <= n <= 31
 * fp32_t fp32_div2n(fp32_t x, int n);
 */
#define fp32_div2n					slldiv2n

/*
 * div4(x) = x / 4
 * fp32_t fp32_div4(fp32_t x);
 */
#define fp32_div4					slldiv4

/*
 * cos(x)
 * fp32_t fp32_cos(fp32_t x);
 */
#define fp32_cos					sllcos

/*
 * sin(x)
 * fp32_t fp32_sin(fp32_t x);
 */
#define fp32_sin					sllsin

/*
 * tan(x)
 * fp32_t fp32_tan(fp32_t x);
 */
#define fp32_tan					slltan

/*
 * acos(x)
 * fp32_t fp32_acos(fp32_t x);
 */
#define fp32_acos					sllacos

/*
 * asin(x)
 * fp32_t fp32_asin(fp32_t x);
 */
#define fp32_asin					sllasin

/*
 * atan(x)
 * fp32_t fp32_atan(fp32_t x);
 */
#define fp32_atan					sllatan

/*
 * sec(x) = 1 / cos x
 * fp32_t fp32_sec(fp32_t x);
 */
#define fp32_sec					sllsec

/*
 * csc(x) = 1 / sin x
 * fp32_t fp32_csc(fp32_t x);
 */
#define fp32_csc 					sllcsc

/*
 * cot(x) = 1 / tan x = cos x / sin x
 * fp32_t fp32_cot(fp32_t x);
 */
#define fp32_cot 					sllcot

/*
 * cosh(x) = [e^x + e^(-x)] / 2
 * fp32_t fp32_cosh(fp32_t x);
 */
#define fp32_cosh					sllcosh

/*
 * sinh(x) = [e^x - e^(-x)] / 2
 * fp32_t fp32_sinh(fp32_t x);
 */
#define fp32_sinh					sllsinh

/*
 * tanh(x) = sinh(x) / cosh(x) = [e^x - e^(-x)] / [e^x + e^(-x)]
 * fp32_t fp32_tanh(fp32_t x);
 */
#define fp32_tanh					slltanh

/*
 * sech(x) = 1 / cosh(x) = 2 / [e^x + e^(-x)]
 * fp32_t fp32_sech(fp32_t x);
 */
#define fp32_sech					sllsech

/*
 * csch(x) = 1 / sinh(x) = 2 / [e^x - e^(-x)]
 * fp32_t fp32_csch(fp32_t x);
 */
#define fp32_csch					sllcsch

/*
 * coth(x) = cosh(x) / sinh(x) = [e^x + e^(-x)] / [e^(x) - e^(-x)]
 * fp32_t fp32_coth(fp32_t x);
 */
#define fp32_coth					sllcoth

/*
 * exp(x) = e^x
 * fp32_t fp32_exp(fp32_t x);
 */
#define fp32_exp					sllexp

/*
 * log(x) = ln x
 * fp32_t fp32_log(fp32_t x);
 */
#define fp32_log					slllog

/*
 * pow(x, y) = x^y
 * fp32_t fp32_pow(fp32_t x, fp32_t y);
 */
#define fp32_pow					sllpow

/*
 * sqrt(x) = x^(1 / 2)
 * fp32_t fp32_sqrt(fp32_t x);
 */
#define fp32_sqrt					sllsqrt

/*
 * inv(x) = 1 / x
 * fp32_t fp32_inv(fp32_t x);
 */
#define fp32_inv					sllinv

/*
 * floor(x) = floor x
 * fp32_t fp32_floor(fp32_t x);
 */
#define fp32_floor 					sllfloor

/*
 * ceil(x) = ceiling x
 * fp32_t fp32_ceil(fp32_t x);
 */
#define fp32_ceil 					sllceil

/*
 * log2(x) = log2 x
 * fp32_t fp32_log2(fp32_t x);
 */
fp32_t fp32_log2(fp32_t x);

/*
 * log10(x) = log10 x
 * fp32_t fp32_log10(fp32_t x);
 */
fp32_t fp32_log10(fp32_t x);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef __FPMATH_H__ */

