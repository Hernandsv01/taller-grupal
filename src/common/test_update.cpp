#include <iostream>

#include "Update.h"

void print_update_complex(update_complex* update) {
    std::cout << "Update id: " << std::to_string(update->id) << std::endl;

    std::cout << "Update type: " << std::to_string(update->update_type_value)
              << std::endl;

    switch (update->update_type_value) {
        case CreateEntity: {
            update_create_entity* as_create_entity =
                dynamic_cast<update_create_entity*>(update);
            std::cout << "Create entity. Entity type: "
                      << std::to_string(as_create_entity->entity_type)
                      << ". Entity subtype: "
                      << std::to_string(as_create_entity->entity_subtype)
                      << std::endl;
            break;
        }
        case Position: {
            update_position* as_position =
                dynamic_cast<update_position*>(update);
            std::cout << "Position. X: " << std::to_string(as_position->x)
                      << ". Y: " << std::to_string(as_position->y) << std::endl;
            break;
        }
        case DeleteEntity: {
            std::cout << "Delete entity" << std::endl;
            break;
        }

        default: {
            update_value* as_update_value = dynamic_cast<update_value*>(update);

            std::cout << "Update value: "
                      << std::to_string(as_update_value->value) << std::endl;
        }
    }
}

void print_update_simple(update_simple update) {
    std::cout << "Update id: " << std::to_string(update.id) << std::endl;

    std::cout << "Update type: " << std::to_string(update.update_type_value)
              << std::endl;

    switch (update.update_type_value) {
        case CreateEntity: {
            entity_type_and_subtype tipo_y_subtipo =
                update.get_entity_type_and_subtype();

            std::cout << "Create entity. Entity type: "
                      << std::to_string(tipo_y_subtipo.type)
                      << ". Entity subtype: "
                      << std::to_string(tipo_y_subtipo.subtype) << std::endl;
            break;
        }
        case Position: {
            position pos = update.get_position();
            std::cout << "Position. X: " << std::to_string(pos.x)
                      << ". Y: " << std::to_string(pos.y) << std::endl;
            break;
        }
        case DeleteEntity: {
            std::cout << "Delete entity" << std::endl;
            break;
        }

        default: {
            std::cout << "Update value: " << std::to_string(update.get_value())
                      << std::endl;
        }
    }
}

void print_update_variant(update_variant update) {
    std::visit(
        [](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, update_variant_value>) {
                std::cout << "Update id: " << std::to_string(arg.id)
                          << std::endl;
                std::cout << "Update type: " << std::to_string(arg.key)
                          << std::endl;
                std::cout << "Update value: " << std::to_string(arg.value)
                          << std::endl;
            } else if constexpr (std::is_same_v<T, update_variant_position>) {
                std::cout << "Update id: " << std::to_string(arg.id)
                          << std::endl;
                std::cout << "Update type: "
                          << std::to_string(update_type::Position) << std::endl;

                std::cout << "Position. X: " << std::to_string(arg.x)
                          << ". Y: " << std::to_string(arg.y) << std::endl;

            } else if constexpr (std::is_same_v<T,
                                                update_variant_create_entity>) {
                std::cout << "Update id: " << std::to_string(arg.id)
                          << std::endl;
                std::cout << "Update type: "
                          << std::to_string(update_type::CreateEntity)
                          << std::endl;

                std::cout << "Create entity. Entity type: "
                          << std::to_string(arg.type)
                          << ". Entity subtype: " << std::to_string(arg.subtype)
                          << std::endl;
            }
        },
        update);
}

void print_vector(std::vector<uint8_t> vector) {
    for (uint8_t byte : vector) {
        std::cout << std::to_string(byte) << " ";
    }
    std::cout << std::endl;
}

void serialize_deserialize_and_prinet(update_simple update,
                                      update_complex* update2,
                                      update_variant update3) {
    std::vector<uint8_t> data_simple = update.serialize();
    std::vector<uint8_t> data_complex = update2->serialize();
    std::vector<uint8_t> data_variant = serialize(update3);

    print_vector(data_simple);
    print_vector(data_complex);
    print_vector(data_variant);

    update_complex* deserialized_complex =
        update_complex::deserialize(data_complex);
    update_simple deserialized_simple = update_simple::deserialize(data_simple);
    update_variant deserialized_variant = deserialize(data_variant);

    print_update_complex(deserialized_complex);
    print_update_simple(deserialized_simple);
    print_update_variant(deserialized_variant);
    std::cout << std::endl << std::endl;
}

void compare_position() {
    update_position update(50, 3.5, 5.6);
    update_simple simple_update = update_simple::create_position(50, 3.5, 5.6);
    update_variant variant_update = update_variant_position{50, 3.5, 5.6};

    serialize_deserialize_and_prinet(simple_update, &update, variant_update);
}

void compare_value() {
    update_value update(35, update_type::Score, 10);
    update_simple simple_update =
        update_simple::create_value(35, update_type::Score, 10);
    update_variant variant_update =
        update_variant_value{35, update_type::Score, 10};

    serialize_deserialize_and_prinet(simple_update, &update, variant_update);
}

void compare_create() {
    update_create_entity update(1, entity_type::Item, entity_subtype::Coin);
    update_simple simple_update = update_simple::create_create_entity(
        1, entity_type::Item, entity_subtype::Coin);
    update_variant variant_update = update_variant_create_entity{
        1, entity_type::Item, entity_subtype::Coin};

    serialize_deserialize_and_prinet(simple_update, &update, variant_update);
}

int main() {
    compare_position();
    compare_value();
    compare_create();
}
