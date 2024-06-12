#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <vector>

enum Direccion { arriba, abajo, derecha, izquierda };
enum DireccionCompuesta {
    arriba_izquieda,
    arriba_derecha,
    abajo_izquierda,
    abajo_derecha
};

class Point {
   public:
    float x;
    float y;

    Point(float x, float y);

    Point();

    friend std::ostream& operator<<(std::ostream& out, const Point& punto);

    void invertir_horizontalmente();
    void invertir_verticalmente();

    Point operator-(const Point& other) const;

    Point operator+(const Point& other) const;

    Point& operator+=(const Point& other);

    Point& operator-=(const Point& other);

    bool esta_en_primer_cuadrante_sin_ejes();
};

class RigidTriangle;

class RigidBox {
protected:
    // La posicion del rigid box esta definida por la esquina inferior
    // izquierda
    float x_pos;
    float y_pos;

    float x_size;
    float y_size;

private:
    float x_min() const;
    float x_max() const;
    float y_min() const;
    float y_max() const;

    float x_center() const;
    float y_center() const;

    Point center() const;
    Point get_origen() const;

    friend std::ostream& operator<<(std::ostream& out, const RigidBox& box);

    // Devuelve si colisiona con otra RigidBox, ambas proyectadas al eje x o y.
    bool intersects_x(const RigidBox& other) const;
    bool intersects_y(const RigidBox& other) const;

    // Lo mismo pero para un punto
    bool intersects_x(const Point& other) const;
    bool intersects_y(const Point& other) const;

    // Devuelve si esta en contacto (pero no colisiona) con otra RigidBox,
    // ambas proyectadas al eje x o y.
    bool contact_x(const RigidBox& other) const;
    bool contact_y(const RigidBox& other) const;

    // Lo mismo pero para un punto
    bool contact_x(const Point& other) const;
    bool contact_y(const Point& other) const;

    // Devuelve la distancia de interseccion en el eje X o Y
    float intersection_x(const RigidBox& other) const;
    float intersection_y(const RigidBox& other) const;

    // Mueve a this la distancia necesaria para que no colisione horizontal o
    // verticalmente
    void decolisionar_horizontal_con(const RigidBox& other);
    void decolisionar_vertical_con(const RigidBox& other);

public:
    // Crea una RigidBox con
    RigidBox(float x_pos, float y_pos, float x_size, float y_size);

    std::vector<Point> get_vertices() const;

    bool colisiona_con(const Point& punto) const;

    bool colisiona_con(const RigidBox& other) const;

    bool colisiona_con(const RigidTriangle& triangle) const;

    bool esta_en_contacto_con(const Point& punto) const;

    bool esta_en_contacto_con(const RigidBox& other) const;

    bool esta_en_contacto_con(const RigidTriangle& other) const;

    void decolisionar_con(const RigidBox& other);

    // No le encontré uso todavía
    Direccion esta_en_direccion(const RigidBox& other);

    // Divido objeto en 4 cuadrantes, a partir del centro del RigidBox. Cada
    // cuadrante apunta hacia arriba, abajo, izquiqeda o derecha.
    //  Determino el cuadrante en el que está el punto.
    Direccion esta_en_direccion(Point punto);
};

class RigidTriangle {
    Point vertice;
    float largo_cateto_x;
    float largo_cateto_y;
    float lado_x;
    float lado_y;
    DireccionCompuesta direccion_normal;

    friend std::ostream& operator<<(std::ostream& out,
                                    const RigidTriangle& triangle);

    Point convertir_punto_a_coord_triangulo_arriba_der(
        const Point& punto) const;

   public:
    std::vector<Point> get_vertices() const;

    // Crea una RigidBox con
    RigidTriangle(Point vertice, DireccionCompuesta direccion_normal);

    Point get_origen() const;

    bool colisiona_con(const Point& punto) const;

    bool esta_en_contacto_con(const Point& punto) const;

    RigidBox generate_bounding_box() const;

    bool colisiona_con(const RigidTriangle& other) const;

    // bool esta_en_contacto_con(const RigidBox& other) const {
    //     return ((intersects_x(other) && contact_y(other)) ||
    //             (contact_x(other) && intersects_y(other)));
    // }

    // void decolisionar_con(const RigidBox& other) {
    //     if (!this->colisiona_con(other)) return;

    //     float distancia_de_interseccion_x =
    //         std::abs(this->intersection_x(other));
    //     float distancia_de_interseccion_y =
    //         std::abs(this->intersection_y(other));

    //     std::cout << "distancia x,y=" << distancia_de_interseccion_x
    //               << distancia_de_interseccion_y << std::endl;

    //     if (distancia_de_interseccion_x <= distancia_de_interseccion_y) {
    //         this->decolisionar_horizontal_con(other);
    //     } else {
    //         this->decolisionar_vertical_con(other);
    //     }
    // }
};

#endif
