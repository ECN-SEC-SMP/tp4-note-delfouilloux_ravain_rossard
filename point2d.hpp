/**
 * @file point2d.hpp
* @author Bastien, Victor, AlexisR 
 * @brief Header file for the Point2D class
 * @version 0.1
 * @date 2024-01-16
  * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>

#ifndef POINT2D_HPP
#define POINT2D_HPP

using namespace std;

template <typename T, typename U>
class Point2D;

template <typename T, typename U>
ostream& operator<<(ostream& os, const Point2D<T, U>& p);

/**
 * @brief The Point2D class is a class that represents a point in a 2D space. It is templated to allow for different types of coordinates (int, float, double, etc.)
 * 
 */
template <typename T, typename U>
class Point2D
{
private:
    T x;
    U y;
public:
    Point2D(T x, U y);
    ~Point2D();
    Point2D(const Point2D<T, U>& p);
    void translate(T dx, U dy);
    T getX() const;
    U getY() const;
    void setX(T x);
    void setY(U y);

    friend ostream& operator<< <T, U>(ostream& os, const Point2D<T, U>& p);
};

/**
 * @brief Construct a new Point2D<T, U>:: Point2D object
 * 
 * @tparam T 
 * @tparam U 
 * @param x 
 * @param y 
 */
template <typename T, typename U>
Point2D<T, U>::Point2D(T x, U y)
{
    this->x = x;
    this->y = y;
}

/**
 * @brief Destroy the Point2D<T, U>:: Point2D object
 * 
 * @tparam T 
 * @tparam U 
 */
template <typename T, typename U>
Point2D<T, U>::~Point2D()
{
}

/**
 * @brief Construct a new Point2D<T, U>:: Point2D object from another Point2D
 * 
 * @tparam T 
 * @tparam U 
 * @param p 
 */
template <typename T, typename U>
Point2D<T, U>::Point2D(const Point2D<T, U>& p)
{
    this->x = p.x;
    this->y = p.y;
}

/**
 * @brief Translates the point by dx and dy
 * 
 * @tparam T 
 * @tparam U 
 * @param dx 
 * @param dy 
 */
template <typename T, typename U>
void Point2D<T, U>::translate(T dx, U dy)
{
    this->x += dx;
    this->y += dy;
}

/**
 * @brief Returns the x coordinate of the point
 * 
 * @tparam T 
 * @tparam U 
 * @return T 
 */
template <typename T, typename U>
T Point2D<T, U>::getX() const
{
    return this->x;
}

/**
 * @brief Returns the y coordinate of the point
 * 
 * @tparam T 
 * @tparam U 
 * @return U 
 */
template <typename T, typename U>
U Point2D<T, U>::getY() const
{
    return this->y;
}

/**
 * @brief Sets the x coordinate of the point
 * 
 * @tparam T 
 * @tparam U 
 * @param x 
 */
template <typename T, typename U>
void Point2D<T, U>::setX(T x)
{
    this->x = x;
}

/**
 * @brief Sets the y coordinate of the point
 * 
 * @tparam T 
 * @tparam U 
 * @param y 
 */
template <typename T, typename U>
void Point2D<T, U>::setY(U y)
{
    this->y = y;
}

/**
 * @brief Overload of the << operator to print a Point2D
 * 
 * @tparam T 
 * @tparam U 
 * @param os 
 * @param p 
 * @return ostream& 
 */
template <typename T, typename U>
ostream& operator<<(ostream& os, const Point2D<T, U>& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


#endif // POINT2D_HPP