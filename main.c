#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/*
 * Calculate (a^d) % mod that does not overflow
 */
int64_t modpow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/*
 * n: prime to prove; s,d: n-1 = (2^s)*d; a: witness
 * returns true if primality keeps up, false if composite
 */
bool witness_prime(uint64_t n, uint64_t s, uint64_t d, uint64_t a)
{
    uint64_t x = modpow(a, d, n);
    uint64_t y;
 
    while (s > 0)
    {
        y = (x * x) % n;
        if (x != 1 && x != n-1 && y == 1)
            return false;
        x = y;
        s--;
    }

    if (y != 1)
        return false;

    return true;
}

/*
 * The Miller Rabin Sieve is deterministic to a certain degree given the right witness.
 * For n < 3,215,031,751 it is enough to test a = 2, 3, 5, and 7.
 */
bool miller_rabin_sieve(uint64_t n)
{
    /* Check that number falls in a possible range and is not divisible by three or two */
    if (n < 2 || (n % 2 == 0 && n != 2) || (n % 3 == 0 && n != 3))
        return false;
    /* If above check fails the only possible options for this check are 2 and 3. */
    if (n <= 3)
        return true;

    /* n is uneven -> n-1 is even -> (n-1)/2 is even */
    uint64_t d = (n-1) / 2;
    uint64_t s = 1;
    while (d % 2 == 0)
    {
        d /= 2;
        s++;
    }

    /* Given that I'm gonna use that quite a lot... */
    #define witness(A) witness_prime(n,s,d, A)

    if (n < 2047)
        return witness(2);
    if (n < 1373653)
        return witness(2) && witness(3);
    if (n < 9080191)
        return witness(31) && witness(73);
    if (n < 25326001)
        return witness(2) && witness(3) && witness(5);
    if (n < 3215031751)
        return witness(2) && witness(3) && witness(5) && witness(7);
    if (n < 4759123141)
        return witness(2) && witness(7) && witness(61);
    if (n < 1122004669633)
        return witness(2) && witness(13) && witness(23) && witness(16) && witness(62) && witness(80) && witness(3);
    if (n < 2152302898747)
        return witness(2) && witness(3) && witness(5) && witness(7) && witness(11);
    if (n < 3474749660383)
        return witness(2) && witness(3) && witness(5) && witness(7) && witness(11) && witness(13);
    if (n < 341550071728321)
        return witness(2) && witness(3) && witness(5) && witness(7) && witness(11) && witness(13) && witness(17);
    if (n < 3825123056546413051)
        return witness(2) && witness(3) && witness(5) && witness(7) && witness(11) && witness(13) && witness(17) && witness(19) && witness(23);

    return witness(2) && witness(3) && witness(5) && witness(7) && witness(11) && witness(13) && witness(17) && witness(19) && witness(23) && witness(29) && witness(31) && witness(37);

}

int main() {
    uint64_t number = 3825123056546413051;

    if (miller_rabin_sieve(number))
        printf("%ld is a prime.\n", number);
    else
        printf("%ld is not a prime\n", number);
}
