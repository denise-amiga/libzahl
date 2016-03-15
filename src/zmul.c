/* See LICENSE file for copyright and license details. */
#include "internals.h"


void
zmul(z_t a, z_t b, z_t c)
{
	/*
	 * Karatsuba algorithm
	 * 
	 * Basically, this is how you were taught to multiply large numbers
	 * by hand in school: 4010⋅3020 = (4000 + 10)(3000 + 20) =
	 = 40⋅30⋅10⁴ + (40⋅20 + 30⋅10)⋅10² + 10⋅20, but the middle is
	 * optimised to only one multiplication:
	 * 40⋅20 + 30⋅10 = (40 + 10)(30 + 20) − 40⋅30 − 10⋅20.
	 */

	size_t m, m2;
	z_t z0, z1, z2, b_high, b_low, c_high, c_low;
	int b_sign, c_sign;

	b_sign = zsignum(b);
	c_sign = zsignum(c);

	if (unlikely(!b_sign || !c_sign)) {
		SET_SIGNUM(a, 0);
		return;
	}

	m = zbits(b);
	m2 = b == c ? m : zbits(c);

	if (m + m2 <= BITS_PER_CHAR) {
		/* zsetu(a, b->chars[0] * c->chars[0]); { */
		ENSURE_SIZE(a, 1);
		a->used = 1;
		a->chars[0] = b->chars[0] * c->chars[0];
		/* } */
		SET_SIGNUM(a, b_sign * c_sign);
		return;
	}

	SET_SIGNUM(b, 1);
	SET_SIGNUM(c, 1);

        m = MAX(m, m2);
	m2 = m >> 1;

	zinit(z0);
	zinit(z1);
	zinit(z2);
	zinit(b_high);
	zinit(b_low);
	zinit(c_high);
	zinit(c_low);

	zsplit(b_high, b_low, b, m2);
	zsplit(c_high, c_low, c, m2);


	zmul(z0, b_low, c_low);
	zmul(z2, b_high, c_high);
	zadd_unsigned_assign(b_low, b_high);
	zadd_unsigned_assign(c_low, c_high);
	zmul(z1, b_low, c_low);

	zsub_nonnegative_assign(z1, z0);
	zsub_nonnegative_assign(z1, z2);

	zlsh(z1, z1, m2);
	m2 <<= 1;
	zlsh(a, z2, m2);
	zadd_unsigned_assign(a, z1);
	zadd_unsigned_assign(a, z0);


	zfree(z0);
	zfree(z1);
	zfree(z2);
	zfree(b_high);
	zfree(b_low);
	zfree(c_high);
	zfree(c_low);

	SET_SIGNUM(b, b_sign);
	SET_SIGNUM(c, c_sign);
	SET_SIGNUM(a, b_sign * c_sign);
}
