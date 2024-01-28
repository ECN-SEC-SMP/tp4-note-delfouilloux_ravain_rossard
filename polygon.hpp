/**
 * @file polygon.hpp
* @author Bastien, Victor, AlexisR 
 * @brief Header file for the Polygon class
 * @version 0.1
 * @date 2024-01-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <vector>
#include "point2d.hpp"
#include <functional>

#ifndef POLYGON_HPP
#define POLYGON_HPP

using namespace std;

template <typename T, typename U>
class Polygon;

template <typename T, typename U>
ostream& operator<<(ostream& os, const Polygon<T, U>& p);

/**
 * @brief The Polygon class is a class that represents a polygon in a 2D space. It is templated to allow for different types of coordinates (int, float, double, etc.)
 * 
 */
template <typename T, typename U>
class Polygon
{
    private:
        vector<Point2D<T, U>> vertices;
        vector<function<void()>> observers;
        void notifyObservers() {
            for (const auto& observer : observers){
                observer();
            }
                
        }
    public:
        Polygon();
        ~Polygon();
        Polygon(vector<Point2D<T, U>> vertices);
        Polygon(const Polygon<T, U>& p);
        vector<Point2D<T, U>> getVertices() const;
        void setVertices(const vector<Point2D<T, U>> &vertices);
        void addVertex(const Point2D<T, U> &p);
        void translate(T dx, U dy);
        void addObserver(function<void()> observer); 

        friend ostream& operator<< <T, U>(ostream& os, const Polygon& p);
};

/**
 * @brief Construct a new Polygon<T, U>::Polygon object
 * 
 * @tparam T 
 * @tparam U 
 */
template <typename T, typename U>
Polygon<T, U>::Polygon()
{
}

/**
 * @brief Destroy the Polygon<T, U>::Polygon object
 * 
 * @tparam T 
 * @tparam U 
 */
template <typename T, typename U>
Polygon<T, U>::~Polygon()
{
}

/**
 * @brief Construct a new Polygon<T, U>::Polygon object
 * 
 * @tparam T 
 * @tparam U 
 * @param vertices 
 */
template <typename T, typename U>
Polygon<T, U>::Polygon(vector<Point2D<T, U>> vertices)
{
    this->vertices = vertices;
}

/**
 * @brief Construct a new Polygon<T, U>::Polygon object from another Polygon
 * 
 * @tparam T 
 * @tparam U 
 * @param p 
 */
template <typename T, typename U>
Polygon<T, U>::Polygon(const Polygon<T, U>& p)
{
    this->vertices = p.vertices;
}

/**
 * @brief Get the vertices of the polygon
 * 
 * @tparam T 
 * @tparam U 
 * @return vector<Point2D<T, U>> 
 */
template <typename T, typename U>
vector<Point2D<T, U>> Polygon<T, U>::getVertices() const
{
    return this->vertices;
}

/**
 * @brief Set the vertices of the polygon
 * 
 * @tparam T 
 * @tparam U 
 * @param vertices 
 */
template <typename T, typename U>
void Polygon<T, U>::setVertices(const vector<Point2D<T, U>> &vertices)
{
    this->vertices = vertices;
    notifyObservers();
}

/**
 * @brief Add a vertex to the polygon
 * 
 * @tparam T 
 * @tparam U 
 * @param p 
 */
template <typename T, typename U>
void Polygon<T, U>::addVertex(const Point2D<T, U> &p)
{
    this->vertices.push_back(p);
    notifyObservers();
}

/**
 * @brief Translates the polygon by dx and dy
 * 
 * @tparam T 
 * @tparam U 
 * @param dx 
 * @param dy 
 */
template <typename T, typename U>
void Polygon<T, U>::translate(T dx, U dy)
{
    for (int i = 0; i < this->vertices.size(); i++)
    {
        this->vertices[i].translate(dx, dy);
    }
}

/**
 * @brief Add an observer to the polygon
 * 
 * @tparam T 
 * @tparam U 
 * @param observer 
 */
template <typename T, typename U>
void Polygon<T, U>::addObserver(function<void()> observer)
{
    observers.push_back(observer);
}

/**
 * @brief Overload of the << operator for the Polygon class
 * 
 * @tparam T 
 * @tparam U 
 * @param os 
 * @param p 
 * @return ostream& 
 */
template <typename T, typename U>
ostream& operator<<(ostream& os, const Polygon<T, U>& p)
{
    os << "Polygon: ";
    for (int i = 0; i < p.vertices.size(); i++)
    {
        os << p.vertices[i];
    }
    return os;
}

#endif // POLYGON_HPP