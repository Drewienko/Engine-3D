#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/**
 * @class GameObject
 * @brief Klasa bazowa dla wszystkich obiektów w grze.
 *
 * Klasa ta pełni rolę podstawy dla wszystkich obiektów w hierarchii gry,
 * zapewniając wirtualny destruktor.
 */
class GameObject
{
public:
    /**
     * @brief Wirtualny destruktor klasy GameObject.
     */
    virtual ~GameObject() = default;
};

#endif // GAMEOBJECT_H
