#include "Update.h"

#include <string.h>

// USANDO OBJETO PRACTICAMENTE VACIO
update_simple update_simple::deserialize(const std::vector<uint8_t>& data) {
    update_simple update;
    update.update_type_value = static_cast<update_type>(data[0]);

    uint16_t decoded_id;
    memccpy(&decoded_id, &data[1], sizeof(uint16_t), sizeof(uint16_t));

    update.id = decoded_id;

    switch (update.update_type_value) {
        case CreateEntity:
            update.entity_type_value = static_cast<entity_type>(data[3]);
            update.entity_subtype_value = static_cast<entity_subtype>(data[4]);
            break;
        case Position:
            memcpy(&update.x, &data[3], sizeof(float));
            memcpy(&update.y, &data[7], sizeof(float));
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

std::vector<uint8_t> update_simple::serialize() const {
    std::vector<uint8_t> data;
    data.push_back(update_type_value);

    // Serialize id as uint8_t
    uint8_t* id_8bits = (uint8_t*)&id;

    data.push_back(id_8bits[0]);
    data.push_back(id_8bits[1]);

    switch (update_type_value) {
        case CreateEntity: {
            data.push_back(entity_type_value);
            data.push_back(entity_subtype_value);
            break;
        }
        case Position: {
            const uint8_t* floatData = reinterpret_cast<const uint8_t*>(&x);
            data.insert(data.end(), floatData, floatData + sizeof(float));
            floatData = reinterpret_cast<const uint8_t*>(&y);
            data.insert(data.end(), floatData, floatData + sizeof(float));
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

update_simple update_simple::create_create_entity(
    uint16_t id, entity_type entity_type_value,
    entity_subtype entity_subtype_value) {
    update_simple update;
    update.update_type_value = CreateEntity;
    update.id = id;
    update.entity_type_value = entity_type_value;
    update.entity_subtype_value = entity_subtype_value;
    return update;
}

update_simple update_simple::create_position(uint16_t id, float x, float y) {
    update_simple update;
    update.update_type_value = Position;
    update.id = id;
    update.x = x;
    update.y = y;
    return update;
}

update_simple update_simple::create_value(uint16_t id, update_type key,
                                          uint8_t value) {
    update_simple update;
    update.update_type_value = key;
    update.id = id;
    update.value = value;
    return update;
}

uint8_t update_simple::get_value() const {
    if (update_type_value != update_type::Score &&
        update_type_value != update_type::Health &&
        update_type_value != update_type::State &&
        update_type_value != update_type::Direction) {
        throw std::runtime_error("Invalid update type");
    }
    return value;
}

entity_type_and_subtype update_simple::get_entity_type_and_subtype() const {
    if (update_type_value != update_type::CreateEntity) {
        throw std::runtime_error("Invalid update type");
    }
    return entity_type_and_subtype{entity_type_value, entity_subtype_value};
}

position update_simple::get_position() const {
    if (update_type_value != update_type::Position) {
        throw std::runtime_error("Invalid update type");
    }
    return position{x, y};
}

// USANDO HERENCIA
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

update_complex* update_complex::deserialize(const std::vector<uint8_t>& data) {
    update_type decoded_update_type = static_cast<update_type>(data[0]);
    uint16_t decoded_id;
    memccpy(&decoded_id, &data[1], sizeof(uint16_t), sizeof(uint16_t));

    switch (decoded_update_type) {
        case CreateEntity: {
            entity_type decoded_entity_type = static_cast<entity_type>(data[3]);
            entity_subtype decoded_entity_subtype =
                static_cast<entity_subtype>(data[4]);

            return new update_create_entity(decoded_id, decoded_entity_type,
                                            decoded_entity_subtype);
        }
        case Position: {
            float x, y;

            memccpy(&x, &data[3], sizeof(float), sizeof(float));
            memccpy(&y, &data[7], sizeof(float), sizeof(float));

            return new update_position(decoded_id, x, y);
        }

        case DeleteEntity:
            return new update_complex(DeleteEntity, decoded_id);

        case Direction:
        case State:
        case Health:
        case Score: {
            uint8_t decoded_data = data[3];
            return new update_value(decoded_id, decoded_update_type,
                                    decoded_data);
        }
    }

    throw std::runtime_error("Invalid update type");
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