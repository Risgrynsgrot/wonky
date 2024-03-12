#include "gameclient.h"

int main(void) {
	client_t client;
	client_init(&client);
	client_start_loop(&client);
}
