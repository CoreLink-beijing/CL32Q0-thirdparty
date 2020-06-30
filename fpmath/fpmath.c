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


#include <stdint.h>
#include <stdio.h>
#include "fpmath.h"


double cl_fabs (double d)
{
	if (d >= 0) {
		return d;
	}
	else {
		return (double)(fp32_to_double(fp32_neg(fp32_from_double(d))));
	}
}

double cl_cos (double d)
{
	return (double)(fp32_to_double(fp32_cos(fp32_from_double(d))));
}

double cl_sin (double d)
{
	return (double)(fp32_to_double(fp32_sin(fp32_from_double(d))));
}

double cl_tan (double d)
{
	return (double)(fp32_to_double(fp32_tan(fp32_from_double(d))));
}

double cl_acos (double d)
{
	return (double)(fp32_to_double(fp32_acos(fp32_from_double(d))));
}

double cl_asin (double d)
{
	return (double)(fp32_to_double(fp32_asin(fp32_from_double(d))));
}

double cl_atan(double d)
{
	return (double)(fp32_to_double(fp32_atan(fp32_from_double(d))));
}

double cl_cosh (double d)
{
	return (double)(fp32_to_double(fp32_cosh(fp32_from_double(d))));
}

double cl_sinh (double d)
{
	return (double)(fp32_to_double(fp32_sinh(fp32_from_double(d))));
}

double cl_tanh (double d)
{
	return (double)(fp32_to_double(fp32_tanh(fp32_from_double(d))));
}

double cl_exp (double d)
{
	return (double)(fp32_to_double(fp32_exp(fp32_from_double(d))));
}

double cl_log (double d)
{
	return (double)(fp32_to_double(fp32_log(fp32_from_double(d))));
}

double cl_log2 (double d)
{
	return (double)(fp32_to_double(fp32_log2(fp32_from_double(d))));
}

double cl_log10 (double d)
{
	return (double)(fp32_to_double(fp32_log10(fp32_from_double(d))));
}

double cl_pow (double x, double y)
{
	return (double)(fp32_to_double(fp32_pow(fp32_from_double(x), fp32_from_double(y))));
}

double cl_sqrt (double d)
{
	return (double)(fp32_to_double(fp32_sqrt(fp32_from_double(d))));
}

double cl_floor (double d)
{
	return (double)(fp32_to_double(fp32_floor(fp32_from_double(d))));
}

double cl_ceil (double d)
{
	return (double)(fp32_to_double(fp32_ceil(fp32_from_double(d))));
}

fp32_t fp32_log2 (fp32_t x)
{
	return fp32_div(fp32_log(x), CONST_LN2);
}

fp32_t fp32_log10 (fp32_t x)
{
	return fp32_div(fp32_log(x), CONST_LN10);
}
