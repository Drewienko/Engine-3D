#ifndef SHAPEOBJECT_H
#define SHAPEOBJECT_H

#include "DrawableObject.h"
#include "TransformableObject.h"

/**
 * @class ShapeObject
 * @brief Klasa abstrakcyjna reprezentująca obiekt kształtu, który można rysować i transformować.
 *
 * Dziedziczy z `DrawableObject` oraz `TransformableObject`, łącząc funkcjonalność
 * rysowania i transformacji geometrycznych (przesunięcia, skalowania, rotacji).
 */
class ShapeObject : public DrawableObject, public TransformableObject
{
public:
    /**
     * @brief Wirtualny destruktor klasy ShapeObject.
     */
    virtual ~ShapeObject() = default;

    /**
     * @brief Rysuje kształt przy użyciu podanego renderera.
     * @param renderer Wskaźnik na obiekt klasy `PrimitiveRenderer` do rysowania.
     */
    virtual void draw(PrimitiveRenderer *renderer) override = 0;

    /**
     * @brief Przesuwa kształt o podane wartości w osiach X i Y.
     * @param tx Wartość przesunięcia w osi X.
     * @param ty Wartość przesunięcia w osi Y.
     */
    virtual void translate(float tx, float ty) override = 0;

    /**
     * @brief Obraca kształt o zadany kąt względem podanego punktu.
     * @param angle Kąt obrotu w stopniach.
     * @param centerX Współrzędna X punktu obrotu (domyślnie 0.0).
     * @param centerY Współrzędna Y punktu obrotu (domyślnie 0.0).
     */
    virtual void rotate(float angle, float centerX = 0.0, float centerY = 0.0) override = 0;

    /**
     * @brief Skaluje kształt o podane współczynniki w osiach X i Y.
     * @param sx Współczynnik skalowania w osi X.
     * @param sy Współczynnik skalowania w osi Y.
     */
    virtual void scale(float sx, float sy) override = 0;
};

#endif // SHAPEOBJECT_H
