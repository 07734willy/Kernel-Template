#include <stdio.h>
#include <stdlib.h>
#include <framebuffer.h>
#include <unittest.h>

/** This exists solely for debugging purposes, this file will be removed
 *  as soon as unit testing is fixed in the Makefile
 */
int main() {
	int x = 4;
	int y = 4;
	int z = 30;
	ut_assert_equal("Testing true equality", 0, 0);
	ut_assert_equal("Testing false equality", 4, 7);
	ut_assert_equiv("Testing true equivalence", &x, &y);
	ut_assert_equiv("Testing false equivalence", &x, &z);
	ut_pass();
	ut_fail("Purposely fail");
	ut_assert_nonequal("Testing true inequality", 2, 0);
	ut_assert_nonequal("Testing false inequality", 6.1, 6.1);
	ut_assert_nonequiv("Testing true equivalence", &y, &z);
	ut_assert_nonequiv("Testing false equivalence", &x, &x);
}
