#include <iostream>

#include "Update.h"

void print_update_simple(Update::Update_new update) {
    std::cout << "Update id: " << std::to_string(update.id) << std::endl;

    std::cout << "Update type: " << std::to_string(update.update_type_value)
              << std::endl;

    switch (update.update_type_value) {
        case Update::CreateEntity: {
            Update::EntityTypeAndSubtype tipo_y_subtipo =
                update.get_entity_type_and_subtype();

            std::cout << "Create entity. Entity type: "
                      << std::to_string(tipo_y_subtipo.type)
                      << ". Entity subtype: "
                      << std::to_string(tipo_y_subtipo.subtype) << std::endl;
            break;
        }
        case Update::Position: {
            Update::PositionFloat pos = update.get_position();
            std::cout << "Position. X: " << std::to_string(pos.x)
                      << ". Y: " << std::to_string(pos.y) << std::endl;
            break;
        }
        case Update::DeleteEntity: {
            std::cout << "Delete entity" << std::endl;
            break;
        }

        default: {
            std::cout << "Update value: " << std::to_string(update.get_value())
                      << std::endl;
        }
    }
}

void print_vector(std::vector<uint8_t> vector) {
    for (uint8_t byte : vector) {
        std::cout << std::to_string(byte) << " ";
    }
    std::cout << std::endl;
}

void serialize_deserialize_and_prinet(Update::Update_new update) {
    std::vector<uint8_t> data_simple = update.serialize();

    print_vector(data_simple);

    Update::Update_new deserialized_simple =
        Update::Update_new::deserialize(data_simple);

    print_update_simple(deserialized_simple);
    std::cout << std::endl << std::endl;
}

void compare_position() {
    Update::Update_new simple_update =
        Update::Update_new::create_position(50, 3.5, 5.6);

    serialize_deserialize_and_prinet(simple_update);
}

void compare_value() {
    Update::Update_new simple_update =
        Update::Update_new::create_value(35, Update::UpdateType::Score, 10);

    serialize_deserialize_and_prinet(simple_update);
}

void compare_create() {
    Update::Update_new simple_update = Update::Update_new::create_create_entity(
        1, Update::EntityType::Item, Update::EntitySubtype::Coin);

    serialize_deserialize_and_prinet(simple_update);
}

int main() {
    compare_position();
    compare_value();
    compare_create();
}
