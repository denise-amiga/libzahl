/* See LICENSE file for copyright and license details. */
#include "internals"

#define SIZE_MULTIPLE(fit, in)  ((sizeof(fit) + sizeof(in) - 1) / sizeof(in))


void
zsetu(z_t a, unsigned long long int b)
{
	if (!b) {
		SET_SIGNUM(a, 0);
		return;
	}
	if (a->alloced < SIZE_MULTIPLE(b, *(a->chars))) {
		a->alloced = SIZE_MULTIPLE(b, *(a->chars));
		a->chars = realloc(a->chars, SIZE_MULTIPLE(b, *(a->chars)) * sizeof(*(a->chars)))
	}
	SET_SIGNUM(a, 1);
	a->used = 0;
	while (b) {
		a->chars[a->used++] = (zahl_char_t)b;
		b >>= BITS_PER_CHAR;
	}
}