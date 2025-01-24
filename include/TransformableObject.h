#ifndef TRANSFORMABLEOBJECT_H
#define TRANSFORMABLEOBJECT_H

#include "GameObject.h"

/**
 * @class TransformableObject
 * @brief Klasa bazowa dla obiektów, które mogą być transformowane (translacja, rotacja, skalowanie).
 */
class TransformableObject : virtual public GameObject
{
public:
    /**
     * @brief Wirtualny destruktor klasy TransformableObject.
     */
    virtual ~TransformableObject() = default;

    /**
     * @brief Przesuwa obiekt o podane wartości w osiach X i Y.
     * @param tx Wartość przesunięcia w osi X.
     * @param ty Wartość przesunięcia w osi Y.
     */
    virtual void translate(const glm::vec3& direction)=0;


    /**
     * @brief Obraca obiekt o zadany kąt względem podanego punktu.
     * @param angle Kąt obrotu w stopniach.
     * @param centerX Współrzędna X punktu obrotu (domyślnie 0.0).
     * @param centerY Współrzędna Y punktu obrotu (domyślnie 0.0).
     */
    virtual void rotate(float angle, const glm::vec3& axis)=0;

    virtual void rotatePoint(float angle, const glm::vec3& axis, const glm::vec3& point) = 0;

    /**
     * @brief Skaluje obiekt o podane współczynniki w osiach X i Y.
     * @param sxvirtual void scale(float sx, float sy) = 0; Współczynnik skalowania w osi X.
     * @param sy Współczynnik skalowania w osi Y.
     */
    virtual void scale(float sx, float sy) = 0;
};

#endif // TRANSFORMABLEOBJECT_H
