#include "entity.h"
#include "gameclient.h"

entity_t entity_new(client_t* client) {
    entity_t result;
    result.id = client->empty_ids[client->empty_id_count - 1];
    client->empty_id_count--;
    return result;
}

void entity_fill_empty(client_t* client) {
    for(int i = 0; i < MAX_ENTITIES; i++) {
        client->empty_ids[i] = i;
        client->empty_id_count = MAX_ENTITIES;
    }
}

void entity_delete(client_t* client, entity_t entity) {
    client->empty_ids[client->empty_id_count - 1] = entity.id;
    client->empty_id_count++;
}
