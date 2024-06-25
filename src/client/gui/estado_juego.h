#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H

#include <SDL2pp/SDL2pp.hh>
#include <algorithm>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <vector>

#include "../../common/Update.h"
#include "entityFactory.h"
#include "entityGame.h"
#include "playableCharacter.h"
#include "soundManager.h"

#define FACTOR_TAMANIO 32

struct Position {
    // La posicion estaría definida en pixeles, para que le sea más util al
    // renderer.
    int x = 0;
    int y = 0;
};
// Direccion a la que apunta la entidad (para saber si voltear horizontalmente
// la textura)

const std::vector<std::string> posibleStates = {
    "Stand",     "Shot", "Jump",   "Fall", "Run",      "Intox",
    "Intoxwalk", "Dead", "Gethit", "Dash", "Shotfall", "Special"};
// Posibles estados de ¿solo jugador?

// Clase que se encarga de mantener el estado del juego actualizado.
// Incluye todos los jugadores, proyectiles, enemigos e items, indexado por ID,
// para ser mas facil su actualizacion.

class UpdatableGameState2 {
   private:
    std::map<int, std::shared_ptr<Entity2>> gameState;
    uint16_t mainId;
    int remainingSeconds = 0;
    bool matchEnded = false;
    uint32_t tick = 0;

   public:
    explicit UpdatableGameState2(uint16_t mainId) : mainId(mainId) {
        // std::shared_ptr<Entity2> ptr = std::make_unique<Entity2>();
        // gameState.emplace(1, std::move(ptr));
    }

    void handleUpdate(Update::Update_new update, int tick) {
        switch (update.get_update_type()) {
            case Update::CreateEntity: {
                Update::EntityType entityType = update.getEntityType();

                if (entityType == Update::EntityType::Bullet) {
                    SoundManager::PlayMusic("shoot", SHOOT_CHANNEL, 0);
                }

                Update::EntitySubtype entitySubtype = update.getEntitySubType();
                addEntity(update.get_id(), entityType, entitySubtype);
                break;
            }
            case Update::Position: {
                int xPosition = update.getPositionX() * (float)FACTOR_TAMANIO;
                int yPosition = update.getPositionY() * (float)FACTOR_TAMANIO;
                updatePosition(update.get_id(), xPosition, yPosition);
                break;
            }
            case Update::Direction: {
                bool isRight =
                    (update.get_value() == enums_value_update::Direction::Right)
                        ? true
                        : false;
                updateDirection(update.get_id(), isRight);
                break;
            }
            case Update::State: {
                int state = update.get_value();
                updateState(update.get_id(), posibleStates[state]);
                break;
            }
            case Update::Health: {
                int healthPoints = update.get_value();
                updateHealthPoints(update.get_id(), healthPoints);
                break;
            }
            case Update::Score: {
                int score = update.get_value();
                updateScore(update.get_id(), score);
                break;
            }
            case Update::DeleteEntity: {
                gameState.erase(update.get_id());
                break;
            }

            case Update::MatchEnded: {
                matchEnded = true;
                break;
            }

            case Update::RemainingSeconds: {
                remainingSeconds = update.get_value();
                break;
            }

            case Update::ChangeAmmoType: {
                int weaponType = update.get_value();
                updateWeapon(update.get_id(), weaponType);
                break;
            }
            case Update::BulletsRemaining: {
                int ammoQuantity = update.get_value();
                updateAmmoQuantity(update.get_id(), ammoQuantity);
                break;
            }
            default:
                throw std::runtime_error("Faltan valores para los cases");
                break;
        }
    }

    void copyAllEntities(SDL2pp::Renderer &renderer, const int &mainId,
                         const int &xCenter, const int &yCenter,
                         const int &xReference, const int &yReference) {
        const auto &mainPlayer = gameState.at(mainId);

        for (auto &pair : gameState) {
            pair.second->renderize(renderer, xReference, yReference, tick);
        }

        dynamic_cast<PlayableCharacter *>(mainPlayer.get())
            ->showHud(renderer, xCenter * 2, yCenter * 2, remainingSeconds,
                      tick);
    }

    std::vector<std::tuple<int, std::string, int>> getPlayersScores() {
        std::vector<std::tuple<int, std::string, int>> scores;
        for (auto &pair : gameState) {
            if (pair.second->isPlayer()) {
                PlayableCharacter *player =
                    dynamic_cast<PlayableCharacter *>(pair.second.get());

                std::string playerType = player->getCharacterType();
                int score = player->getScore();
                scores.push_back(
                    std::make_tuple(pair.first, playerType, score));
            }
        }
        return scores;
    }

    // Devuelve los puntajes de los jugadores.
    // El formato es (Id, tipo de jugador, puntaje)

    // devolver vector de todos id tipo puntaje

    void addEntity(const int &id, const int &type, const int &subType) {
        std::shared_ptr<Entity2> entity =
            EntityFactory::createEntity(type, subType);

        gameState.insert(std::make_pair(id, std::move(entity)));
    }

    void updatePosition(const int &id, const int &x, const int &y) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        entity->updatePosition(x, y);
    }

    void updateState(const int &id, std::string newState) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        entity->updateState(newState);
    }

    void updateDirection(const int &id, bool &isRight) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        entity->updateDirection(isRight);
    }

    void updateHealthPoints(const int &id, const int &healthPoint) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);

        if (!entity->isPlayer()) return;
        auto player = std::dynamic_pointer_cast<PlayableCharacter>(entity);

        if (id == mainId && player->getHealth() > healthPoint) {
            SoundManager::PlayMusic("hurt", HURT_CHANNEL, 0);
        }

        player->updateHealth(healthPoint);
    }

    void updateScore(const int &id, const int &score) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        if (!entity->isPlayer()) return;
        auto player = std::dynamic_pointer_cast<PlayableCharacter>(entity);

        if (id == mainId) {
            SoundManager::PlayMusic("coin", SCORE_CHANNEL, 0);
        }

        player->updateScore(score);
    }

    void updateWeapon(const int &id, const int &weaponCode) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        std::string weaponName;
        switch (weaponCode) {
            case (enums_value_update::Ammo_type::NORMAL):
                weaponName = "weaponNormal";
                break;
            case (enums_value_update::Ammo_type::LIGHT):
                weaponName = "weaponLight";
                break;
            case (enums_value_update::Ammo_type::HEAVY):
                weaponName = "weaponHeavy";
                break;
            case (enums_value_update::Ammo_type::POWER):
                weaponName = "weaponPower";
                break;
            default:
                break;
        }
        entity->updateWeapon(weaponName);
    }

    void updateAmmoQuantity(const int &id, int ammoQuantity) {
        if (ammoQuantity == 255) {
            ammoQuantity = -1;
        }
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        entity->updateAmmoQuantity(ammoQuantity);
    }

    int getEntityPositionX(int id) const {
        const auto &entity = gameState.at(id);
        return entity->getPosX();
    }

    int getEntityPositionY(int id) const {
        const auto &entity = gameState.at(id);
        return entity->getPosY();
    }

    bool hasMatchEnded() { return this->matchEnded; }

    void updateTick(uint32_t tick) { this->tick = tick; }
};

#endif  // ESTADO_JUEGO_H
