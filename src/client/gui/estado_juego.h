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
    "Stand",     "Shot",    "Jump",   "Fall", "Run",      "Intox",
    "Intoxwalk", "Roasted", "Gethit", "Dash", "Shotfall", "Special"};
// Posibles estados de ¿solo jugador?

// Clase que se encarga de mantener el estado del juego actualizado.
// Incluye todos los jugadores, proyectiles, enemigos e items, indexado por ID,
// para ser mas facil su actualizacion.
class UpdatableGameState2 {
   private:
    std::map<int, std::shared_ptr<Entity2>> gameState;

   public:
    explicit UpdatableGameState2() {
        // std::shared_ptr<Entity2> ptr = std::make_unique<Entity2>();
        // gameState.emplace(1, std::move(ptr));
    }

    void handleUpdate(Update::Update_new update, int tick) {
        switch (update.get_update_type()) {
            case Update::CreateEntity: {
                Update::EntityType entityType = update.getEntityType();
                Update::EntitySubtype entitySubtype = update.getEntitySubType();
                addEntity(update.get_id(), entityType, entitySubtype);
                break;
            }
            case Update::Position: {
                std::cout << "UPDATE POSICION\n";
                std::cout << "actualizando posicion (server):"
                          << std::to_string(update.getPositionX()) << ","
                          << std::to_string(update.getPositionY()) << std::endl;

                int xPosition = update.getPositionX() * (float)FACTOR_TAMANIO;
                int yPosition = update.getPositionY() * (float)FACTOR_TAMANIO;
                updatePosition(update.get_id(), xPosition, yPosition);
                break;
            }
            case Update::Direction: {
                bool isRight = (update.get_value() == 0) ? true : false;
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
                // TODO:
                // No se que hacer con esto
                break;
            }

            case Update::RemainingSeconds: {
                // TODO
                // No se que hacer con esto
                break;
            }

            case Update::ChangeAmmoType: {
                // TODO
                // No se que hacer con esto
                break;
            }

            case Update::BulletsRemaining: {
                // TODO
                // No se que hacer con esto
                break;
            }

            default:
                throw std::runtime_error("Faltan valores para los cases");
                break;
        }
    }

    void copyAllEntities(SDL2pp::Renderer &renderer, const int &mainId,
                         const int &xCenter, const int &yCenter, const int& xReference, const int& yReference) {
        const auto &mainPlayer = gameState.at(mainId);

        for (auto &pair : gameState) {
            if (isNotMain(pair.first, mainId)) {
                pair.second->renderize(renderer, xReference, yReference);
            }
        }
        mainPlayer->renderMainPj(renderer, xReference, yReference);
        mainPlayer->showHud(renderer, xCenter * 2, yCenter * 2);
    }

    /*
    std::vector<std::tuple<int , std::string, int>> getPlayersScores() {
        std::vector<std::tuple<int , std::string, int>> scores;
        for (auto& pair: gameState) {
            if (pair.second->isPlayer()) {
                std::string playerType = pair.second->getType();
                int score = pair.second->getScore();
                scores.push_back(std::make_tuple(pair.first, playerType,
    score));
            }
        }
        return std::move(scores);
    */
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
        entity->updateHealth(healthPoint);
    }

    void updateScore(const int &id, const int &score) {
        std::shared_ptr<Entity2> &entity = gameState.at(id);
        entity->updateHealth(score);
    }

    int getEntityPositionX(int id) const {
        const auto &entity = gameState.at(id);
        return entity->getPosX();
    }

    int getEntityPositionY(int id) const {
        const auto &entity = gameState.at(id);
        return entity->getPosY();
    }

   private:
    bool isNotMain(const int &playerId, const int &mainId) {
        return (playerId != mainId);
    }
};

#endif  // ESTADO_JUEGO_H
