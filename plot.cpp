/**
 * @file plot.cpp
 * @author Bastien, Victor, AlexisR 
 * @brief Implementation file for the Plot class and its derived classes
 * @version 0.1
 * @date 2024-01-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <vector>
#include "plot.hpp"
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());

string PlotTypeToString(PlotType type) {
    switch(type) {
        case PlotType::URBAN_ZONE: return "ZU";
        case PlotType::ZONE_TO_BE_URBANIZED: return "ZAU";
        case PlotType::NATURAL_AND_FOREST_ZONE: return "ZN";
        case PlotType::AGRICULTURAL_ZONE: return "ZA";
        default: return "Unknown";
    }
}

/**
 * @brief Construct a new Plot::Plot object
 * 
 * @param number 
 * @param owner 
 * @param shape 
 * @param pBuildable 
 */
Plot::Plot(int number, string owner, Polygon<int,float>* shape, int pBuildable) 
{
    this->number = number;
    this->owner = owner;
    this->shape = shape;
    this->pBuildable = pBuildable;
    this->calculateArea();

    this->shape->addObserver([this]() { this->calculateArea(); });
}

/**
 * @brief Construct a new Plot::Plot object by copy
 * 
 * @param p 
 */
Plot::Plot(const Plot& p)
{
    this->number = p.number;
    this->owner = p.owner;
    this->shape = p.shape;
    this->area = p.area;
    this->pBuildable = p.pBuildable;

    this->shape->addObserver([this]() { this->calculateArea(); }); 
}

/**
 * @brief Destroy the Plot::Plot object
 * 
 */
Plot::~Plot()
{
}

/**
 * @brief Get the number of the plot
 * 
 * @return int 
 */
int Plot::getNumber() const
{
    return this->number;
}

/**
 * @brief Get the owner of the plot
 * 
 * @return string 
 */
string Plot::getOwner() const
{
    return this->owner;
}

/**
 * @brief Get the area of the plot
 * 
 * @return float 
 */
float Plot::getArea() const
{
    return this->area;
}

/**
 * @brief Get the shape of the plot
 * 
 * @return Polygon<int,float>* 
 */
Polygon<int,float>* Plot::getShape() const
{
    return this->shape;
}

/**
 * @brief Set the number of the plot
 * 
 * @param number 
 */
void Plot::setNumber(int number)
{
    this->number = number;
}

/**
 * @brief Set the owner of the plot
 * 
 * @param owner 
 */
void Plot::setOwner(string owner)
{
    this->owner = owner;
}

/**
 * @brief Calculate the area of the plot
 * 
 */
void Plot::calculateArea()
{
    try{
        float area = 0;
        for (int i = 0; i < this->shape->getVertices().size(); i++)
        {
            if (i == this->shape->getVertices().size() - 1) //if we are at the last vertex, we need to use the first vertex as the second point
            {
                area += (this->shape->getVertices()[i].getX() * this->shape->getVertices()[0].getY()) - (this->shape->getVertices()[i].getY() * this->shape->getVertices()[0].getX());
            }
            else
            {
                area += (this->shape->getVertices()[i].getX() * this->shape->getVertices()[i+1].getY()) - (this->shape->getVertices()[i].getY() * this->shape->getVertices()[i+1].getX());
            }
        }
        area = area / 2;
        if (area <= 0) {
            throw runtime_error("The area of a polygon cannot be negative or null");
        }
        this->area = area;
    }
    catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}

/**
 * @brief Set the shape of the plot. Also computes the area of the plot and throws an error if the area is negative or null
 * 
 * @param shape 
 */
void Plot::setShape(Polygon<int,float>* shape)
{
    this->shape = shape;
    this->calculateArea();
}

/**
 * @brief Set the percentage of buildable area of the plot
 * 
 * @param pBuildable 
 */
void Plot::setPBuildable(int pBuildable)
{
    this->pBuildable = pBuildable;
}

/**
 * @brief Get the type of the plot
 * 
 * @return PlotType 
 */
PlotType Plot::getType() const
{
    return this->type;
}

/**
 * @brief Get the percentage of buildable area of the plot
 * 
 * @return int 
 */
int Plot::getPBuildable() const
{
    return this->pBuildable;
}

/**
 * @brief Overload of the << operator for printing a plot
 * 
 * @param os 
 * @param p 
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const Plot& p)
{
    os << "Plot number: " << p.number << endl;
    os << "\t" << *(p.shape) << endl;
    os << "\tOwner: " << p.owner << endl;
    os << "\tArea: " << p.area << " m2" << endl;
    return os;
}

/**
 * @brief Construct a new Buildable::Buildable object
 * 
 * @param number 
 * @param owner 
 * @param shape 
 * @param pBuildable // default value is 0
 */
Buildable::Buildable(int number, string owner, Polygon<int,float>* shape, int pBuildable) : Plot(number, owner, shape, pBuildable)
{
}

/**
 * @brief Construct a new Buildable::Buildable object by copy
 * 
 * @param b 
 */
Buildable::Buildable(const Buildable& b) : Plot(b)
{
}

/**
 * @brief Destroy the Buildable::Buildable object
 * 
 */
Buildable::~Buildable()
{
}

/**
 * @brief Construct a new UrbanZone::UrbanZone object. The built area is randomly generated between 0 and the maximum buildable area
 * 
 * @param number 
 * @param owner 
 * @param shape 
 * @param pBuildable 
 * @param builtArea Default value is 0 if not specified 
 */
UrbanZone::UrbanZone(int number, string owner, Polygon<int,float>* shape, int pBuildable, float builtArea) : Plot(number, owner, shape, pBuildable), Buildable(number, owner, shape, pBuildable)
{
    if (!builtArea) { //if builtArea is not specified, we generate a random value between 0 and the maximum buildable area
        float maxBuiltArea = getArea() * (static_cast<float>(getPBuildable()) / 100.0f);
        uniform_real_distribution<float> dis(0, maxBuiltArea);
        this->builtArea = dis(gen);
    }
    else {
        this->builtArea = builtArea;
    }
    this->setType(PlotType::URBAN_ZONE);
}

/**
 * @brief Construct a new UrbanZone::UrbanZone object by copy
 * 
 * @param u 
 */
UrbanZone::UrbanZone(const UrbanZone& u) : Plot(u), Buildable(u)
{
    this->builtArea = u.builtArea;
}

/**
 * @brief Destroy the UrbanZone::UrbanZone object
 * 
 */
UrbanZone::~UrbanZone()
{
}

/**
 * @brief Set the type of the plot
 * 
 * @param type 
 */
void UrbanZone::setType(PlotType type)
{
    this->type = type;
}

/**
 * @brief Get the built area of the plot
 * 
 * @return float 
 */
float UrbanZone::getBuiltArea() const
{
    return this->builtArea;
}

/**
 * @brief Get the buildable area of the plot
 * 
 * @return float 
 */
float UrbanZone::getBuildableArea() const
{
    float buildableArea = this->getArea() * (static_cast<float>(this->getPBuildable()) / 100.0f);
    return buildableArea - this->builtArea;
}

/**
 * @brief Overload of the << operator for printing an urban zone
 * 
 * @param os 
 * @param u 
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const UrbanZone& u)
{
    os << "Plot number: " << u.getNumber() << endl;
    os << "\tType: " << PlotTypeToString(u.getType()) << endl;
    os << "\t" << *(u.getShape()) << endl;
    os << "\tOwner: " << u.getOwner() << endl;
    os << "\tArea: " << u.getArea() << " m2" << endl;
    os << "\tBuildable area: " <<  u.getPBuildable() << "%" <<  endl;
    os << "\tBuilt area: " << u.getBuiltArea() << " m2" << endl;
    os << "\tFree area: " << u.getBuildableArea() << " m2" << endl;
    return os;
}

/**
 * @brief Construct a new ZoneToBeUrbanized::ZoneToBeUrbanized object
 * 
 * @param number 
 * @param owner 
 * @param shape 
 * @param pBuildable 
 */
ZoneToBeUrbanized::ZoneToBeUrbanized(int number, string owner, Polygon<int,float>* shape, int pBuildable) : Plot(number, owner, shape, pBuildable), Buildable(number, owner, shape, pBuildable)
{
    this->setType(PlotType::ZONE_TO_BE_URBANIZED);
}

/**
 * @brief Construct a new ZoneToBeUrbanized::ZoneToBeUrbanized object by copy
 * 
 * @param z 
 */
ZoneToBeUrbanized::ZoneToBeUrbanized(const ZoneToBeUrbanized& z) : Plot(z), Buildable(z)
{
}

/**
 * @brief Destroy the ZoneToBeUrbanized::ZoneToBeUrbanized object
 * 
 */
ZoneToBeUrbanized::~ZoneToBeUrbanized()
{
}

/**
 * @brief Get the buildable area of the plot
 * 
 * @return float 
 */
float ZoneToBeUrbanized::getBuildableArea() const
{
    float buildableArea = this->getArea() * static_cast<float>(this->getPBuildable() / 100);
    return buildableArea;
}

/**
 * @brief Set the type of the plot
 * 
 * @param type 
 */
void ZoneToBeUrbanized::setType(PlotType type)
{
    this->type = type;
}

/**
 * @brief Overload of the << operator for printing a zone to be urbanized
 * 
 * @param os 
 * @param z 
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const ZoneToBeUrbanized& z)
{
    os << "Plot number: " << z.getNumber() << endl;
    os << "\tType: " << PlotTypeToString(z.getType()) << endl;
    os << "\t" << *(z.getShape()) << endl; 
    os << "\tOwner: " << z.getOwner() << endl;
    os << "\tArea: " << z.getArea() << " m2" << endl;
    os << "\tBuildable area: " <<  z.getPBuildable() << "%" <<  endl;
    return os;
}

/**
 * @brief Construct a new NaturalAndForestZone::NaturalAndForestZone object
 * 
 * @param number 
 * @param owner 
 * @param shape 
 */
NaturalAndForestZone::NaturalAndForestZone(int number, string owner, Polygon<int,float>* shape) : Plot(number, owner, shape, 0)
{
    this->setType(PlotType::NATURAL_AND_FOREST_ZONE);
}

/**
 * @brief Construct a new NaturalAndForestZone::NaturalAndForestZone object by copy
 * 
 * @param n 
 */
NaturalAndForestZone::NaturalAndForestZone(const NaturalAndForestZone& n) : Plot(n)
{
}

/**
 * @brief Destroy the NaturalAndForestZone::NaturalAndForestZone object
 * 
 */
NaturalAndForestZone::~NaturalAndForestZone()
{
}

/**
 * @brief Set the type of the plot
 * 
 * @param type 
 */
void NaturalAndForestZone::setType(PlotType type)
{
    this->type = type;
}

/**
 * @brief Overload of the << operator for printing a natural and forest zone
 * 
 * @param os 
 * @param n 
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const NaturalAndForestZone& n)
{
    os << "Plot number: " << n.getNumber() << endl;
    os << "\tType: " << PlotTypeToString(n.getType()) << endl;
    os << "\t" << *(n.getShape()) << endl;
    os << "\tOwner: " << n.getOwner() << endl;
    os << "\tArea: " << n.getArea() << " m2" << endl;
    return os;
}

/**
 * @brief Construct a new AgriculturalZone::AgriculturalZone object. 
 * 
 * @param number 
 * @param owner 
 * @param shape 
 * @param cropType 
 */
AgriculturalZone::AgriculturalZone(int number, string owner, Polygon<int,float>* shape, string cropType) : Plot(number, owner, shape, 0), Buildable(number, owner, shape, 0), NaturalAndForestZone(number, owner, shape)
{

    this->setType(PlotType::AGRICULTURAL_ZONE);
    this->cropType = cropType;
    int pBuildableArea = static_cast<int>((this->getBuildableArea() / this->NaturalAndForestZone::getArea()) * 100.0f);
    this->Buildable::setPBuildable(pBuildableArea);
}

/**
 * @brief Construct a new AgriculturalZone::AgriculturalZone object by copy
 * 
 * @param a 
 */
AgriculturalZone::AgriculturalZone(const AgriculturalZone& a) : Plot(a), Buildable(a), NaturalAndForestZone(a)
{
    this->cropType = a.cropType;
}

/**
 * @brief Destroy the AgriculturalZone::AgriculturalZone object
 * 
 */
AgriculturalZone::~AgriculturalZone()
{
}

/**
 * @brief Set the type of the plot
 * 
 * @param type 
 */
void AgriculturalZone::setType(PlotType type)
{
    this->type = type;
}

/**
 * @brief Get the crop type of the plot
 * 
 * @return string 
 */
string AgriculturalZone::getCropType() const
{
    return this->cropType;
}

/**
 * @brief Get the buildable area of the plot. The buildable area of an agricultural zone is 10% of the area of the plot, with a maximum of 200m2
 * 
 * @return float 
 */
float AgriculturalZone::getBuildableArea() const
{
    float maxBuiltArea = this->getArea() * 0.10f;
    return maxBuiltArea < 200.0f ? maxBuiltArea : 200.0f; 
}

/**
 * @brief Overload of the << operator for printing an agricultural zone
 * 
 * @param os 
 * @param a 
 * @return ostream& 
 */
ostream& operator<<(ostream& os, const AgriculturalZone& a)
{
    os << "Plot number: " << a.getNumber() << endl;
    os << "\tType: " << PlotTypeToString(a.getType()) << endl;
    os << "\t" << *(a.getShape()) << endl;
    os << "\tOwner: " << a.getOwner() << endl;
    os << "\tArea: " << a.getArea() << " m2" << endl;
    os << "\tCrop type: " << a.getCropType() << endl;
    return os;
}