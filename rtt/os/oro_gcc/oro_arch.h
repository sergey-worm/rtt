#ifndef __GCC_ORO_ARCH__
#define __GCC_ORO_ARCH__

#include "../../rtt-config.h"

/**
 * Structure that contains an int for atomic
 * operations.
 */
typedef struct {
    int volatile cnt;
} oro_atomic_t;

#define ORO_ATOMIC_SETUP	oro_atomic_set
#define ORO_ATOMIC_CLEANUP(a_int)

#define oro_atomic_read(a_int)		((a_int)->cnt)

#define oro_atomic_set(a_int,n)		(((a_int)->cnt) = (n))

/**
 * Add n to a_int
 */
static __inline__ void oro_atomic_add(oro_atomic_t *a_int, int n)
{
    (void)__atomic_add_fetch(&a_int->cnt, n, __ATOMIC_SEQ_CST);
}

/**
 * Substract n from a_int
 */
static __inline__ void oro_atomic_sub(oro_atomic_t *a_int, int n)
{
    (void)__atomic_sub_fetch(&a_int->cnt, n, __ATOMIC_SEQ_CST);
}

/**
 * Substract n from a_int and test for zero
 */
static __inline__ int oro_atomic_sub_and_test(oro_atomic_t *a_int, int n)
{
    return !(__atomic_sub_fetch(&a_int->cnt, n, __ATOMIC_SEQ_CST));
}

/**
 * Increment a_int atomically
 */
static __inline__ void oro_atomic_inc(oro_atomic_t *a_int)
{
    (void)__atomic_fetch_add(&a_int->cnt, 1, __ATOMIC_SEQ_CST);
}

/**
 * Decrement a_int atomically
 */
static __inline__ void oro_atomic_dec(oro_atomic_t *a_int)
{
    (void)__atomic_fetch_sub(&a_int->cnt, 1, __ATOMIC_SEQ_CST);
}

/**
 * Decrement a_int atomically and test for zero.
 */
static __inline__ int oro_atomic_dec_and_test(oro_atomic_t *a_int)
{
    return !(__atomic_sub_fetch(&a_int->cnt, 1, __ATOMIC_SEQ_CST));
}

/**
 * Increment a_int atomically and test for zero.
 */
static __inline__ int oro_atomic_inc_and_test(oro_atomic_t *a_int)
{
    return !(__atomic_add_fetch(&a_int->cnt, 1, __ATOMIC_SEQ_CST));
}

/**
 * Compare o with *ptr and swap with n if equal.
 */
template <typename T>
static __inline__ T oro_cmpxchg(volatile T* ptr, T o, T n)
{
    T expected = o;
    T desired = n;
    __atomic_compare_exchange_n(ptr, &expected, desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return expected;
}
/*
#define oro_cmpxchg(ptr,o,n)\
    ((__typeof__(*(ptr)))__sync_val_compare_and_swap((ptr),(o),(n)))
*/

#endif // __GCC_ORO_ARCH__
