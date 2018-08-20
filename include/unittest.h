#ifndef UNITTEST_HH
#define UNITTEST_HH

#define ut_assert_equiv(message, actual, expected) _ut_assert_equal(message, memcmp(actual, expected, sizeof(*expected)) == 0, __FILE__, __LINE__)

#define ut_assert_nonequiv(message, actual, expected) _ut_assert_equal(message, memcmp(actual, expected, sizeof(*expected)) != 0, __FILE__, __LINE__)

#define ut_assert_equal(message, actual, expected) _ut_assert_equal(message, actual == expected, __FILE__, __LINE__)

#define ut_assert_nonequal(message, actual, expected) _ut_assert_equal(message, actual != expected, __FILE__, __LINE__)

#define ut_fail(message) _ut_fail(message, __FILE__, __LINE__)
#define ut_pass() _ut_pass(__FILE__, __LINE__)
#define ut_summary() _ut_summary(__FILE__)

#endif
