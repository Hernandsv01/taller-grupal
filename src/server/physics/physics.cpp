#include "physics.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

Point::Point(float x, float y) : x(x), y(y) {}

Point::Point() : x(0), y(0) {}

void Point::invertir_horizontalmente() { x = -x; }
void Point::invertir_verticalmente() { y = -y; }

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

Point& Point::operator+=(const Point& other) {
    x += other.x;
    y += other.y;

    return *this;
}

Point& Point::operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;

    return *this;
}

bool Point::esta_en_primer_cuadrante_sin_ejes() { return (x > 0 && y > 0); }

float RigidBox::x_min() const { return x_pos; }
float RigidBox::x_max() const { return x_pos + x_size; }
float RigidBox::y_min() const { return y_pos; }
float RigidBox::y_max() const { return y_pos + y_size; }

float RigidBox::x_center() const { return x_pos + x_size / 2; }
float RigidBox::y_center() const { return y_pos + y_size / 2; }

Point RigidBox::center() const { return Point(x_center(), y_center()); }
Point RigidBox::get_origen() const { return Point(x_pos, y_pos); }

// Devuelve si colisiona con otra RigidBox, ambas proyectadas al eje x o y
bool RigidBox::intersects_x(const RigidBox& other) const {
    return (this->x_min() < other.x_max() && this->x_max() > other.x_min());
}
bool RigidBox::intersects_y(const RigidBox& other) const {
    return (this->y_min() < other.y_max() && this->y_max() > other.y_min());
}

bool RigidBox::intersects_x(const Point& other) const {
    return (this->x_min() < other.x && this->x_max() > other.x);
}
bool RigidBox::intersects_y(const Point& other) const {
    return (this->y_min() < other.y && this->y_max() > other.y);
}

// Devuelve si esta en contacto (pero no colisiona) con otra RigidBox,
// ambas proyectadas al eje x o y
bool RigidBox::contact_x(const RigidBox& other) const {
    return (this->x_min() == other.x_max() || this->x_max() == other.x_min());
}
bool RigidBox::contact_y(const RigidBox& other) const {
    return (this->y_min() == other.y_max() || this->y_max() == other.y_min());
}

bool RigidBox::contact_x(const Point& other) const {
    return (this->x_min() == other.x || this->x_max() == other.x);
}
bool RigidBox::contact_y(const Point& other) const {
    return (this->y_min() == other.y || this->y_max() == other.y);
}

// Devuelve la distancia de interseccion en el eje X
float RigidBox::intersection_x(const RigidBox& other) const {
    if (intersects_x(other) == false) {
        return 0;
    }

    if (this->x_center() < other.x_center()) {
        return (other.x_min() - this->x_max());
    } else {
        return (other.x_max() - this->x_min());
    }
}

// Devuelve la distancia de interseccion en el eje Y
float RigidBox::intersection_y(const RigidBox& other) const {
    if (intersects_y(other) == false) {
        return 0;
    }

    if (this->y_center() < other.y_center()) {
        return (other.y_min() - this->y_max());
    } else {
        return (other.y_max() - this->y_min());
    }
}

// Mueve a this la distancia necesaria para que no colisione en el eje X
void RigidBox::decolisionar_horizontal_con(const RigidBox& other) {
    float mover = this->intersection_x(other);
    this->x_pos += mover;
}

// Mueve a this la distancia necesaria para que no colisione en el eje Y
void RigidBox::decolisionar_vertical_con(const RigidBox& other) {
    float mover = this->intersection_y(other);
    this->y_pos += mover;
}

// Crea una RigidBox con
RigidBox::RigidBox(float x_pos, float y_pos, float x_size, float y_size)
    : x_pos(x_pos), y_pos(y_pos), x_size(x_size), y_size(y_size) {}

std::vector<Point> RigidBox::get_vertices() const {
    std::vector<Point> vertices(4);

    vertices[0] = Point(x_min(), y_min());
    vertices[1] = Point(x_min(), y_max());
    vertices[2] = Point(x_max(), y_min());
    vertices[3] = Point(x_max(), y_max());

    return vertices;
}

bool RigidBox::colisiona_con(const Point& punto) const {
    return ((this->x_min() < punto.x && this->x_max() > punto.x) &&
            (this->y_min() < punto.y && this->y_max() > punto.y));
}

bool RigidBox::colisiona_con(const RigidBox& other) const {
    return this->intersects_x(other) && this->intersects_y(other);
}

bool RigidBox::colisiona_con(const RigidTriangle& triangle) const {
    RigidBox bounding_box_triangle = triangle.generate_bounding_box();

    if (!this->colisiona_con(bounding_box_triangle)) return false;

    std::vector<Point> vertices = this->get_vertices();

    return std::any_of(
        vertices.begin(), vertices.end(),
        [&triangle](const Point& p) { return triangle.colisiona_con(p); });
}

bool RigidBox::esta_en_contacto_con(const Point& punto) const {
    return ((intersects_x(punto) && contact_y(punto)) ||
            (contact_x(punto) && intersects_x(punto)));
}

bool RigidBox::esta_en_contacto_con(const RigidBox& other) const {
    return ((intersects_x(other) && contact_y(other)) ||
            (contact_x(other) && intersects_y(other)));
}

// bool RigidBox::esta_en_contacto_con(const RigidTriangle& other) const {}

void RigidBox::decolisionar_con(const RigidBox& other) {
    if (!this->colisiona_con(other)) return;

    float distancia_de_interseccion_x = std::abs(this->intersection_x(other));
    float distancia_de_interseccion_y = std::abs(this->intersection_y(other));

    std::cout << "distancia x,y=" << distancia_de_interseccion_x
              << distancia_de_interseccion_y << std::endl;

    if (distancia_de_interseccion_x <= distancia_de_interseccion_y) {
        this->decolisionar_horizontal_con(other);
    } else {
        this->decolisionar_vertical_con(other);
    }
}

// No le encontré uso todavía
Direccion RigidBox::esta_en_direccion(const RigidBox& other) {
    return esta_en_direccion(other.center());
}

// Divido objeto en 4 cuadrantes, a partir del centro del RigidBox. Cada
// cuadrante apunta hacia arriba, abajo, izquiqeda o derecha.
//  Determino el cuadrante en el que está el punto.
Direccion RigidBox::esta_en_direccion(Point punto) {
    punto.x -= this->x_center();
    punto.y -= this->y_center();

    float pendiente_diagonal = this->y_size / this->x_size;

    // Recta con forma /
    float valor_de_y_en_recta_diagonal = pendiente_diagonal * punto.x;

    if (valor_de_y_en_recta_diagonal > punto.y) {  // Abajo y derecha

        if (-valor_de_y_en_recta_diagonal > punto.y) {
            return Direccion::abajo;
        } else {
            return Direccion::derecha;
        }

    } else {  // Arriba e izquierda
        if (-valor_de_y_en_recta_diagonal > punto.y) {
            return Direccion::izquierda;
        } else {
            return Direccion::arriba;
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Point& punto) {
    out << "(" << punto.x << ", " << punto.y << ")";
    return out;
}

std::vector<Point> RigidTriangle::get_vertices() const {
    std::vector<Point> vertices(3);

    vertices[0] = this->get_origen();
    vertices[1] = get_origen() + Point(lado_x, 0);
    vertices[2] = get_origen() + Point(0, lado_y);

    return vertices;
}

// Crea una RigidBox con
RigidTriangle::RigidTriangle(Point vertice, DireccionCompuesta direccion_normal)
    : vertice(vertice),
      largo_cateto_x(1),
      largo_cateto_y(1),
      lado_x(0),
      lado_y(0),
      direccion_normal(direccion_normal) {
    switch (direccion_normal) {
        case DireccionCompuesta::abajo_derecha:
            lado_x = 1;
            lado_y = -1;
            break;
        case DireccionCompuesta::abajo_izquierda:
            lado_x = -1;
            lado_y = -1;
            break;
        case DireccionCompuesta::arriba_derecha:
            lado_x = 1;
            lado_y = 1;
            break;
        case DireccionCompuesta::arriba_izquieda:
            lado_x = -1;
            lado_y = 1;
            break;
    }
}

Point RigidTriangle::get_origen() const { return vertice; }

Point RigidTriangle::convertir_punto_a_coord_triangulo_arriba_der(
    const Point& punto) const {
    Point p(punto);

    // Cambio el punto de referencia, para que el vertice del triangulo sea
    // (0,0)
    p -= get_origen();

    // Cambio la rotacion de la referencia, para que el triangulo sea el que
    // apunta arriba a la derecha
    switch (direccion_normal) {
        case DireccionCompuesta::arriba_derecha:
            break;

        case DireccionCompuesta::arriba_izquieda:
            p.invertir_horizontalmente();
            break;

        case DireccionCompuesta::abajo_derecha:
            p.invertir_verticalmente();
            break;

        case DireccionCompuesta::abajo_izquierda:
            p.invertir_horizontalmente();
            p.invertir_verticalmente();
    }

    return p;
}

bool RigidTriangle::colisiona_con(const Point& punto) const {
    Point p = convertir_punto_a_coord_triangulo_arriba_der(punto);

    // Chequeo si el punto se encuentra en el primer cuadrante
    if (!p.esta_en_primer_cuadrante_sin_ejes()) return false;

    // chequeo si el punto se encuentra bajo la hipotenusa.
    return (1 - p.x > p.y);
}

RigidBox RigidTriangle::generate_bounding_box() const {
    return RigidBox(this->get_origen().x, this->get_origen().y, largo_cateto_x,
                    largo_cateto_y);
}

bool RigidTriangle::colisiona_con(const RigidTriangle& other) const {
    if (other.colisiona_con(this->get_origen())) return true;

    std::vector<Point> vertices_otro = other.get_vertices();
    std::vector<Point> vertices_this = this->get_vertices();

    return (std::any_of(vertices_otro.begin(), vertices_otro.end(),
                        [&](const Point& p) { return colisiona_con(p); })) or
           (std::any_of(
               vertices_this.begin(), vertices_this.end(),
               [other](const Point& p) { return other.colisiona_con(p); }));
}

bool RigidTriangle::esta_en_contacto_con(const Point& punto) const {
    Point p = convertir_punto_a_coord_triangulo_arriba_der(punto);

    // No se encuentra en el cuadrado de 1,1 en el primer cuadrante
    if (p.y < 0 || p.y > 1 || p.x < 0 || p.x > 1) return false;

    // Esta en cateto Y
    if (p.x == 0) return true;

    // Esta en cateto X
    if (p.y == 0) return true;

    // Esta en hipotenusa
    if (1 - p.x == p.y) return true;

    return false;
}

std::ostream& operator<<(std::ostream& out, const RigidTriangle& triangle) {
    (out << "Origen: ") << triangle.get_origen() << ". Orientacion: ";

    switch (triangle.direccion_normal) {
        case DireccionCompuesta::arriba_derecha:
            out << "◣";
            break;

        case DireccionCompuesta::arriba_izquieda:
            out << "◢";
            break;

        case DireccionCompuesta::abajo_derecha:
            out << "◤";
            break;

        case DireccionCompuesta::abajo_izquierda:
            out << "◥";
            break;
    }

    out << " .";

    return out;
}

std::ostream& operator<<(std::ostream& out, const RigidBox& box) {
    (out << "Posicion: ") << box.get_origen() << ". Ancho: " << box.x_size
                          << ". Alto: " << box.y_size << ".";

    return out;
}

int main() {
    RigidBox box(-1, 6, 4, 10);
    RigidBox box2(0, 0, 10, 10);
    // Point punto(0, 2);

    // https://www.desmos.com/calculator/1qxlycdgyb
    // Pagina de desmos para ver boxes segun parametros
    std::cout << "Colisiona: " << box.colisiona_con(box2)
              << " En contacto: " << box.esta_en_contacto_con(box2)
              << std::endl;
    std::cout << box << std::endl;

    std::cout << "\nDECOLISIONADO\n" << std::endl;

    box.decolisionar_con(box2);

    std::cout << "Colisiona: " << box.colisiona_con(box2)
              << " En contacto: " << box.esta_en_contacto_con(box2)
              << std::endl;
    std::cout << box << std::endl;

    RigidTriangle triangle(Point(0, 0), DireccionCompuesta::arriba_derecha);
    RigidTriangle triangle2(Point(-0.45, -0.45),
                            DireccionCompuesta::arriba_derecha);

    RigidTriangle triangle3(Point(-0.45, -0.45),
                            DireccionCompuesta::arriba_izquieda);

    std::cout << triangle << triangle.colisiona_con(triangle2) << std::endl;

    std::cout << box2 << triangle << triangle2 << triangle3 << std::endl;

    std::cout << box2.colisiona_con(triangle);
    std::cout << box2.colisiona_con(triangle2);
    std::cout << box2.colisiona_con(triangle3);
    std::cout << std::endl;

    return 0;
}