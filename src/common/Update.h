#ifndef COMMON_DTOS_H
#define COMMON_DTOS_H

#include <sstream>
#include <vector>

// packete de actualizacion: se utiliza el id, una clave del elemento
// actualizabel y el valor.
// struct Update {
//     uint16_t id;
//     uint8_t key;
//     uint32_t value;
// } __attribute__((packed));

#endif  // COMMON_DTOS_H

enum update_type : uint8_t {
    CreateEntity = 0,
    Position = 1,
    Direction = 2,
    State = 3,
    Health = 4,
    Score = 5,
    DeleteEntity = 6
};

enum entity_type : uint8_t { Player = 0, Bullet = 1, Wall = 2, Item = 3 };

enum entity_subtype : uint8_t {
    Jazz,
    Spaz,
    Lori,
    Enemy1,
    Enemy2,
    Enemy3,
    Coin,
    Weapon
};

struct entity_type_and_subtype {
    entity_type type;
    entity_subtype subtype;
};

struct position {
    float x;
    float y;
};

// USANDO OBJETO PRACTICAMENTE VACIO
class update_simple {
   public:
    uint16_t id;
    update_type update_type_value;

   private:
    // Create entity
    entity_type entity_type_value = Player;
    entity_subtype entity_subtype_value = Jazz;

    // Position
    float x = 0;
    float y = 0;

    // Value
    uint8_t value = 0;

   public:
    static update_simple deserialize(const std::vector<uint8_t>& data);

    std::vector<uint8_t> serialize() const;

    static update_simple create_create_entity(
        uint16_t id, entity_type entity_type_value,
        entity_subtype entity_subtype_value);

    static update_simple create_position(uint16_t id, float x, float y);

    static update_simple create_value(uint16_t id, update_type key,
                                      uint8_t value);

    uint8_t get_value() const;

    entity_type_and_subtype get_entity_type_and_subtype() const;

    position get_position() const;
};

// USANDO HERENCIA
class update_complex {
   protected:
    update_complex(update_type update_type_value, uint16_t id);

   public:
    virtual std::vector<uint8_t> serialize() const;

    static update_complex* deserialize(const std::vector<uint8_t>& data);

   public:
    update_type update_type_value;
    uint16_t id;
};

class update_create_entity : public update_complex {
   public:
    update_create_entity(uint16_t id, uint8_t entity_type,
                         uint8_t entity_subtype);

    std::vector<uint8_t> serialize() const override;

   public:
    uint8_t entity_type;
    uint8_t entity_subtype;
};

class update_position : public update_complex {
   public:
    update_position(uint16_t id, float x, float y);

    std::vector<uint8_t> serialize() const override;

   public:
    float x;
    float y;
};

class update_value : public update_complex {
   public:
    update_value(uint16_t id, update_type key, uint8_t value);

    std::vector<uint8_t> serialize() const override;

   public:
    uint8_t value;
};
