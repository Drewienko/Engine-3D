#ifndef UPDATABLEOBJECT_H
#define UPDATABLEOBJECT_H

#include "GameObject.h"
#include <SFML/Window/Event.hpp>

/**
 * @class UpdatableObject
 * @brief Klasa bazowa dla obiektów, które mogą być aktualizowane w odpowiedzi na zdarzenia.
 */
class UpdatableObject : virtual public GameObject
{
public:
    /**
     * @brief Wirtualny destruktor klasy UpdatableObject.
     */
    virtual ~UpdatableObject() = default;

    /**
     * @brief Wirtualna metoda do aktualizacji obiektu na podstawie zdarzeń.
     * @param event Referencja do zdarzenia SFML.
     */
    virtual void update(sf::Event &event) = 0;
};

#endif // UPDATABLEOBJECT_H
