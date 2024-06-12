#include "Update.h"

#include <arpa/inet.h>
#include <string.h>

#include <stdexcept>

float ntohf(float f) {
    uint32_t p;
    memcpy(&p, &f, sizeof(f));
    p = ntohl(p);
    memcpy(&f, &p, sizeof(f));
    return f;
}

float htonf(float f) {
    uint32_t p;
    memcpy(&p, &f, sizeof(f));
    p = htonl(p);
    memcpy(&f, &p, sizeof(f));
    return f;
}

#define SIZE_UPDATE_CREATE 5
#define SIZE_UPDATE_POSITION 11
#define SIZE_UPDATE_VALUE 4

// USANDO OBJETO PRACTICAMENTE VACIO
using namespace Update;

std::vector<Update_new> Update_new::deserialize_all(std::vector<uint8_t> data) {
    std::vector<Update_new> updates;

    while (data.size() > 0) {
        Update_new update = Update_new::deserialize(data);
        updates.push_back(update);

        if (update.update_type_value == CreateEntity) {
            data.erase(data.begin(), data.begin() + SIZE_UPDATE_CREATE);
        } else if (update.update_type_value == Position) {
            data.erase(data.begin(), data.begin() + SIZE_UPDATE_POSITION);
        } else {
            data.erase(data.begin(), data.begin() + SIZE_UPDATE_VALUE);
        }
    }

    return updates;
}

Update_new Update_new::deserialize(const std::vector<uint8_t> &data) {
    Update_new update;
    update.update_type_value = static_cast<UpdateType>(data[0]);

    uint16_t decoded_id;
    memccpy(&decoded_id, &data[1], sizeof(uint16_t), sizeof(uint16_t));

    decoded_id = ntohs(decoded_id);

    update.id = decoded_id;

    switch (update.update_type_value) {
        case CreateEntity:
            update.entity_type_value = static_cast<EntityType>(data[3]);
            update.entity_subtype_value = static_cast<EntitySubtype>(data[4]);
            break;
        case Position:
            memcpy(&update.x, &data[3], sizeof(float));
            update.x = ntohf(update.x);

            memcpy(&update.y, &data[7], sizeof(float));
            update.y = ntohf(update.y);
            break;
        case DeleteEntity:
        case Direction:
        case State:
        case Health:
        case Score:
            update.value = data[3];
            break;
    }

    return update;
}

std::vector<uint8_t> Update_new::serialize() const {
    std::vector<uint8_t> data;
    data.push_back(update_type_value);

    // Serialize id as uint8_t
    uint16_t id_endiannes = htons(this->id);

    uint8_t *id_8bits = (uint8_t *)&id_endiannes;

    data.push_back(id_8bits[0]);
    data.push_back(id_8bits[1]);

    switch (update_type_value) {
        case CreateEntity: {
            data.push_back(entity_type_value);
            data.push_back(entity_subtype_value);
            break;
        }
        case Position: {
            float x_changed_endiannes = htonf(x);
            float y_changed_endiannes = htonf(y);

            uint8_t *x_8bits = (uint8_t *)&x_changed_endiannes;
            uint8_t *y_8bits = (uint8_t *)&y_changed_endiannes;

            data.push_back(x_8bits[0]);
            data.push_back(x_8bits[1]);
            data.push_back(x_8bits[2]);
            data.push_back(x_8bits[3]);

            data.push_back(y_8bits[0]);
            data.push_back(y_8bits[1]);
            data.push_back(y_8bits[2]);
            data.push_back(y_8bits[3]);
            break;
        }
        case DeleteEntity:
        case Direction:
        case State:
        case Health:
        case Score: {
            data.push_back(value);
            break;
        }
    }

    return data;
}

Update_new Update_new::create_create_entity(
    uint16_t id, EntityType entity_type_value,
    EntitySubtype entity_subtype_value) {
    using namespace Update;

    Update_new update;
    update.update_type_value = CreateEntity;
    update.id = id;
    update.entity_type_value = entity_type_value;
    update.entity_subtype_value = entity_subtype_value;
    return update;
}

Update_new Update_new::create_position(uint16_t id, float x, float y) {
    using namespace Update;

    Update_new update;
    update.update_type_value = Position;
    update.id = id;
    update.x = x;
    update.y = y;
    return update;
}

Update_new Update_new::create_value(uint16_t id, UpdateType key,
                                    uint8_t value) {
    Update_new update;
    update.update_type_value = key;
    update.id = id;
    update.value = value;
    return update;
}

uint8_t Update_new::get_value() const {
    using namespace Update;

    if (update_type_value != UpdateType::Score &&
        update_type_value != UpdateType::Health &&
        update_type_value != UpdateType::State &&
        update_type_value != UpdateType::Direction) {
        throw std::runtime_error("Invalid update type");
    }
    return value;
}

EntityTypeAndSubtype Update_new::get_entity_type_and_subtype() const {
    using namespace Update;

    if (update_type_value != UpdateType::CreateEntity) {
        throw std::runtime_error("Invalid update type");
    }
    return EntityTypeAndSubtype{entity_type_value, entity_subtype_value};
}

PositionFloat Update_new::get_position() const {
    using namespace Update;

    if (update_type_value != UpdateType::Position) {
        throw std::runtime_error("Invalid update type");
    }
    return PositionFloat{x, y};
}