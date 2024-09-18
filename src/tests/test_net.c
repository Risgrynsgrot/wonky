#include "../components.h"
#include "../network_common.h"
#include <memory.h>
#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

void test_net_write() {
	serializer_t ser_write = new_writer_network((ser_net_t){0});
	net_write_byte(&ser_write.ser.net, 1, "value");
	net_write_byte(&ser_write.ser.net, 2, "value");
	net_write_byte(&ser_write.ser.net, 3, "value");
	net_write_byte(&ser_write.ser.net, 4, "value");
	printf("%#08x\n", ser_write.ser.net.net_buf.data[0]);
	TEST_ASSERT_EQUAL(ser_write.ser.net.net_buf.data[0], 0x04030201);
	net_write_byte(&ser_write.ser.net, 5, "value");
	net_write_byte(&ser_write.ser.net, 6, "value");
	net_write_byte(&ser_write.ser.net, 7, "value");
	net_write_byte(&ser_write.ser.net, 8, "value");
	printf("%#08x\n", ser_write.ser.net.net_buf.data[1]);
	TEST_ASSERT_EQUAL(ser_write.ser.net.net_buf.data[1], 0x08070605);
}

void test_net_read() {

	serializer_t ser_write = new_writer_network((ser_net_t){0});
	net_write_byte(&ser_write.ser.net, 1, "value");
	net_write_byte(&ser_write.ser.net, 2, "value");
	net_write_byte(&ser_write.ser.net, 3, "value");
	net_write_byte(&ser_write.ser.net, 4, "value");
	printf("%#08x\n", ser_write.ser.net.net_buf.data[0]);
	net_write_byte(&ser_write.ser.net, 5, "value");
	net_write_byte(&ser_write.ser.net, 6, "value");
	net_write_byte(&ser_write.ser.net, 7, "value");
	net_write_byte(&ser_write.ser.net, 8, "value");
	printf("%#08x\n", ser_write.ser.net.net_buf.data[1]);

	serializer_t ser_read = new_reader_network((ser_net_t){0});
	memcpy(ser_read.ser.net.net_buf.data,
		   ser_write.ser.net.net_buf.data,
		   1024 * sizeof(uint32_t));
	char value = 0;
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	net_read_byte(&ser_read.ser.net, &value, "value");
	printf("%c\n", value);
	TEST_ASSERT_EQUAL(value, 8);
}

void test_net_serialize() {

	comp_net_test_t test = {.a = 10, .extra = false, .b = 25, .c = 12, .d = 30};
	serializer_t ser_write = new_writer_network((ser_net_t){0});
	net_write_byte(&ser_write.ser.net, COMPONENT_NET_TEST, "type");
	ser_net_test(&ser_write, &test);
	net_buffer_flush(&ser_write.ser.net.net_buf);
	net_buffer_print(&ser_write.ser.net.net_buf);

	serializer_t ser_read = new_reader_network((ser_net_t){0});
	net_buffer_reset(&ser_read.ser.net.net_buf);
	memcpy(ser_read.ser.net.net_buf.data,
		   ser_write.ser.net.net_buf.data,
		   1024 * sizeof(uint32_t));
	net_buffer_print(&ser_read.ser.net.net_buf);
	char type = 0;
	net_read_byte(&ser_read.ser.net, &type, "type");
	TEST_ASSERT_EQUAL(type, COMPONENT_NET_TEST);
	comp_net_test_t test2 = {0};
	ser_net_test(&ser_read, &test2);

	TEST_ASSERT_EQUAL(test.a, test2.a);
	TEST_ASSERT_EQUAL(test.b, test2.b);
	TEST_ASSERT_EQUAL(test.c, test2.c);
	TEST_ASSERT_EQUAL(test.d, test2.d);
}

int main(void) {
	UNITY_BEGIN();
	RUN_TEST(test_net_serialize);
	RUN_TEST(test_net_write);
	RUN_TEST(test_net_read);
	return UNITY_END();
}
