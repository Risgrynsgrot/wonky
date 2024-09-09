#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

void test_net_equal() {
	TEST_ASSERT_TRUE(1 == 2);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_net_equal);
	return UNITY_END();
}
