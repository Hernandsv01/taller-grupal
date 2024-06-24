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
#define SIZE_UPDATE_DELETE 3
#define SIZE_UPDATE_END_MATCH 3

// USANDO OBJETO PRACTICAMENTE VACIO
using namespace Update;

bool Update_new::updateTypeIsValue() const {
    return update_type_value == UpdateType::Direction ||
           update_type_value == UpdateType::State ||
           update_type_value == UpdateType::Health ||
           update_type_value == UpdateType::Score ||
           update_type_value == UpdateType::RemainingSeconds ||
           update_type_value == UpdateType::ChangeAmmoType ||
           update_type_value == UpdateType::BulletsRemaining;
}

std::vector<Update_new> Update_new::deserialize_all(std::vector<uint8_t> data) {
    std::vector<Update_new> updates;

    while (data.size() > 0) {
        Update_new update = Update_new::deserialize(data);
        updates.push_back(update);

        uint8_t erase_size = 0;

        switch (update.get_update_type()) {
            case CreateEntity:
                erase_size = SIZE_UPDATE_CREATE;
                break;
            case Position:
                erase_size = SIZE_UPDATE_POSITION;
                break;
            case DeleteEntity:
                erase_size = SIZE_UPDATE_DELETE;
                break;
            case MatchEnded:
                erase_size = SIZE_UPDATE_END_MATCH;
                break;
            case Direction:
            case State:
            case Health:
            case Score:
            case RemainingSeconds:
            case ChangeAmmoType:
            case BulletsRemaining:
                erase_size = SIZE_UPDATE_VALUE;
                break;

            default:
                throw std::runtime_error("Invalid update type");
        }

        data.erase(data.begin(), data.begin() + erase_size);
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
        case MatchEnded:
            break;
        case Direction:
        case State:
        case Health:
        case Score:
        case RemainingSeconds:
        case ChangeAmmoType:
        case BulletsRemaining:
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
        case Direction:
        case State:
        case Health:
        case Score:
        case RemainingSeconds:
        case ChangeAmmoType:
        case BulletsRemaining: {
            data.push_back(value);
            break;
        }
        case MatchEnded:
        case DeleteEntity:
            break;
    }

    return data;
}

Update_new Update_new::create_create_entity(
    uint16_t id, EntityType entity_type_value,
    EntitySubtype entity_subtype_value) {
    Update_new update;
    update.update_type_value = CreateEntity;
    update.id = id;
    update.entity_type_value = entity_type_value;
    update.entity_subtype_value = entity_subtype_value;
    return update;
}

Update_new Update_new::create_delete_entity(uint16_t id) {
    Update_new update;
    update.update_type_value = DeleteEntity;
    update.id = id;
    return update;
}

Update_new Update_new::create_position(uint16_t id, float x, float y) {
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

Update_new Update_new::create_end_match() {
    Update_new update;
    update.update_type_value = UpdateType::MatchEnded;
    return update;
}

uint16_t Update_new::get_id() const { return id; }

UpdateType Update_new::get_update_type() const { return update_type_value; }

uint8_t Update_new::get_value() const {
    if (!updateTypeIsValue()) {
        throw std::runtime_error("Invalid update type");
    }
    return value;
}

EntityTypeAndSubtype Update_new::get_entity_type_and_subtype() const {
    if (update_type_value != UpdateType::CreateEntity) {
        throw std::runtime_error("Invalid update type");
    }
    return EntityTypeAndSubtype{entity_type_value, entity_subtype_value};
}

PositionFloat Update_new::get_position() const {
    if (update_type_value != UpdateType::Position) {
        throw std::runtime_error("Invalid update type");
    }
    return PositionFloat{x, y};
}

Update::EntityType Update_new::getEntityType() const {
    return entity_type_value;
}

Update::EntitySubtype Update_new::getEntitySubType() const {
    return entity_subtype_value;
}

float Update_new::getPositionX() const { return x; }

float Update_new::getPositionY() const { return y; }
