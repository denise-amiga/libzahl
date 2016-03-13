/* See LICENSE file for copyright and license details. */
#include "internals.h"


void
zor(z_t a, z_t b, z_t c)
{
	size_t n, m;

	if (unlikely(zzero(b))) {
		if (zzero(c))
			SET_SIGNUM(a, 0);
		else
			SET(a, c);
		return;
	} else if (unlikely(zzero(c))) {
		SET(a, b);
		return;
	}

	m = MAX(b->used, c->used);
	n = b->used + c->used - m;

	ENSURE_SIZE(a, m);

	if (a == b) {
		if (b->used < c->used)
			zmemcpy(a->chars + n, c->chars + n, m - n);
		while (n--)
			a->chars[n] |= c->chars[n];
	} else if (unlikely(a == c)) {
		if (c->used < b->used)
			zmemcpy(a->chars + n, b->chars + n, m - n);
		while (n--)
			a->chars[n] |= b->chars[n];
	} else if (m == b->used) {
		zmemcpy(a->chars, b->chars, m);
		while (n--)
			a->chars[n] |= c->chars[n];
	} else {
		zmemcpy(a->chars, c->chars, m);
		while (n--)
			a->chars[n] |= b->chars[n];
	}

	a->used = m;
	SET_SIGNUM(a, zpositive2(b, c) * 2 - 1);
}
