#include <iostream>

#include "Update.h"

int main() {
    update_value update(1, Health, 10);

    std::vector<uint8_t> data = update.serialize();

    update_complex deserialized = update_complex::deserialize(data);

    std::cout << "Update id: " << std::to_string(deserialized.id) << std::endl;

    std::cout << "Update type: "
              << std::to_string(deserialized.update_type_value) << std::endl;

    update_value as_update_value = dynamic_cast<update_value&>(deserialized);

    std::cout << "Update value: " << std::to_string(as_update_value.value)
              << std::endl;
}