#include "Update.h"

#include <string.h>

update_complex::update_complex(update_type update_type_value, uint16_t id)
    : update_type_value(update_type_value), id(id) {}

std::vector<uint8_t> update_complex::serialize() const {
    std::vector<uint8_t> data;
    data.push_back(update_type_value);

    // Serialize id as uint8_t
    uint8_t* id_8bits = (uint8_t*)&id;

    data.push_back(id_8bits[0]);
    data.push_back(id_8bits[1]);
    return data;
}

update_complex update_complex::deserialize(const std::vector<uint8_t>& data) {
    update_type decoded_type = static_cast<update_type>(data[0]);
    uint16_t decoded_id;
    memccpy(&decoded_id, &data[1], sizeof(uint16_t), sizeof(uint16_t));

    switch (decoded_type) {
        case CreateEntity:
            return update_create_entity(decoded_id, data[2], data[3]);
        case Position:
            return update_position(decoded_id, data[2], data[3]);
        case Direction:
            return update_value(decoded_id, Direction, data[2]);
        case State:
            return update_value(decoded_id, State, data[2]);
        case Health:
            return update_value(decoded_id, Health, data[2]);
        case Score:
            return update_value(decoded_id, Score, data[2]);
        case DeleteEntity:
            return update_complex(DeleteEntity, decoded_id);
    }

    return update_complex(decoded_type, decoded_id);
}

update_create_entity::update_create_entity(uint16_t id, uint8_t entity_type,
                                           uint8_t entity_subtype)
    : update_complex(CreateEntity, id),
      entity_type(entity_type),
      entity_subtype(entity_subtype) {}

std::vector<uint8_t> update_create_entity::serialize() const {
    std::vector<uint8_t> data = update_complex::serialize();
    data.push_back(entity_type);
    data.push_back(entity_subtype);
    return data;
}

update_position::update_position(uint16_t id, float x, float y)
    : update_complex(Position, id), x(x), y(y) {}

std::vector<uint8_t> update_position::serialize() const {
    std::vector<uint8_t> data = update_complex::serialize();
    // Serialize x and y as floats
    const uint8_t* floatData = reinterpret_cast<const uint8_t*>(&x);
    data.insert(data.end(), floatData, floatData + sizeof(float));
    floatData = reinterpret_cast<const uint8_t*>(&y);
    data.insert(data.end(), floatData, floatData + sizeof(float));
    return data;
}

update_value::update_value(uint16_t id, update_type update_type_value,
                           uint8_t value)
    : update_complex(update_type_value, id), value(value) {}

std::vector<uint8_t> update_value::serialize() const {
    std::vector<uint8_t> data = update_complex::serialize();
    data.push_back(value);
    return data;
}