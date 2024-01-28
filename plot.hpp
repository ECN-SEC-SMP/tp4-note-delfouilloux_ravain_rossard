/**
 * @file plot.hpp
* @author Bastien, Victor, AlexisR 
 * @brief Header file for the Plot class
 * @version 0.1
 * @date 2024-01-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <vector>
#include "polygon.hpp"

#ifndef PLOT_HPP
#define PLOT_HPP

using namespace std;

/**
 * @brief The PlotType enum is used to specify the type of plot
 * 
 */
enum PlotType {
    URBAN_ZONE,
    ZONE_TO_BE_URBANIZED,
    NATURAL_AND_FOREST_ZONE,
    AGRICULTURAL_ZONE
};

/**
 * @brief Convert a PlotType to a string for printing
 * 
 * @param type 
 * @return string 
 */
string PlotTypeToString(PlotType type);

/**
 * @brief The Plot class is a base class for all types of plots
 */
class Plot
{
    private:
        int number;
        string owner;
        float area; // in square meters
        Polygon<int,float>* shape;
        int pBuildable; // percentage of buildable area of the plot
    protected:
        PlotType type;
    public:
        Plot(int number, string owner, Polygon<int,float>* shape, int pBuildable);
        Plot(const Plot& p);
        ~Plot();
        int getPBuildable() const;
        int getNumber() const;
        string getOwner() const;
        float getArea() const;
        Polygon<int,float>* getShape() const;
        PlotType getType() const;
        void setNumber(int number);
        void setOwner(string owner);
        void setShape(Polygon<int,float>* shape);
        void setPBuildable(int pBuildable);
        void calculateArea();
        virtual void setType(PlotType type) = 0;

        friend ostream& operator<<(ostream& os, const Plot& p);
};

/**
 * @brief The Buildable class is an abstract class, specfying that a plot can be built on
 */
class Buildable : public virtual Plot
{
    public:
        Buildable(int number, string owner, Polygon<int,float>* shape, int pBuildable = 0);
        Buildable(const Buildable& b);
        ~Buildable();
        virtual void setType(PlotType type) = 0;
        virtual float getBuildableArea() const = 0;
};

/**
 * @brief ZU already has a built surface area (in m2)
 */
class UrbanZone : public Buildable
{
    private:
        float builtArea;
    public:
        UrbanZone(int number, string owner, Polygon<int,float>* shape, int pBuildable, float builtArea = 0);
        UrbanZone(const UrbanZone& u);
        ~UrbanZone();
        void setType(PlotType type);
        float getBuiltArea() const;
        float getBuildableArea() const;
        friend ostream& operator<<(ostream& os, const UrbanZone& u);
};

/**
 * @brief ZAU has no built surface area
 */
class ZoneToBeUrbanized : public Buildable
{
    public:
        ZoneToBeUrbanized(int number, string owner, Polygon<int,float>* shape, int pBuildable);
        ZoneToBeUrbanized(const ZoneToBeUrbanized& z);
        ~ZoneToBeUrbanized();
        void setType(PlotType type);
        float getBuildableArea() const;
        friend ostream& operator<<(ostream& os, const ZoneToBeUrbanized& z);
};

/**
 * @brief ZN are not buildable
 */
class NaturalAndForestZone : public virtual Plot
{
    public:
        NaturalAndForestZone(int number, string owner, Polygon<int,float>* shape);
        NaturalAndForestZone(const NaturalAndForestZone& n);
        ~NaturalAndForestZone();
        void setType(PlotType type);
        friend ostream& operator<<(ostream& os, const NaturalAndForestZone& n);
};

/**
 * @brief On ZA, a farmer can build agricultural buildings as long as the built surface area does not exceed 10% of the total area and a maximum of 200m2. The ZA also has additional characteristics like the type of cultivated crop
 */
class AgriculturalZone : public NaturalAndForestZone, public Buildable
{
    private:
        string cropType;
    public:
        AgriculturalZone(int number, string owner, Polygon<int,float>* shape, string cropType);
        AgriculturalZone(const AgriculturalZone& a);
        ~AgriculturalZone();
        void setType(PlotType type);
        string getCropType() const;
        float getBuildableArea() const;
        friend ostream& operator<<(ostream& os, const AgriculturalZone& a);
};


#endif // PLOT_HPP