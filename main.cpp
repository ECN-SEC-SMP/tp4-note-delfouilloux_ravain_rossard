/**
 * @file main.cpp
 * @author Bastien, Victor, AlexisR 
 * @brief Main file for the project
 * @version 0.1
 * @date 2024-01-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include "point2d.hpp"
#include "polygon.hpp"
#include "plot.hpp"
#include "cmath"
#include "sstream"
#include "fstream"

using namespace std;

vector<Plot*> textToPlots(string filename);
void plotsToText(vector<Plot*> plots);

int main()
{
    srand(time(NULL));

    //Test Point2D
    Point2D<int, float> p1(1, 1);
    Point2D<int, float> p2(101, 1);
    Point2D<int, float> p3(101, 101);
    Point2D<int, float> p4(1, 101);

    cout << "p1: " << p1 << endl;

    //Test Polygon
    Polygon<int, float> poly0;
    vector<Point2D<int, float>> vertices = {p1, p2, p3};
    poly0.setVertices(vertices);

    cout << poly0 << endl;

    Polygon<int, float> poly0bis(poly0);

    poly0.translate(-1, -1);

    cout << poly0bis << endl;
    
    cout << poly0 << endl;

    //Test UrbanZone
    int pbuildable = rand() % 100;
    UrbanZone u0(1, "Bastien", &poly0, pbuildable, 100);
    cout << u0 << endl;
    poly0.addVertex(p4);
    cout << u0 << endl;

    //Test ZoneToBeUrbanized
    Point2D<int, float> p5(100, 0);
    Point2D<int, float> p6(200, 0);
    Point2D<int, float> p7(200, 100);
    Point2D<int, float> p8(100, 100);

    Polygon<int, float> poly1;
    vector<Point2D<int, float>> vertices1 = {p5, p6, p7, p8};
    poly1.setVertices(vertices1);

    pbuildable = rand() % 100;
    ZoneToBeUrbanized z0(2, "Bastien", &poly1, pbuildable);
    cout << z0 << endl;

    //Test AgriculturalZone
    Point2D<int, float> p13(0, 100);
    Point2D<int, float> p14(100, 100);
    Point2D<int, float> p15(100, 200);
    Point2D<int, float> p16(0, 200);

    Polygon<int, float> poly3;
    vector<Point2D<int, float>> vertices3 = {p13, p14, p15, p16};
    poly3.setVertices(vertices3);

    AgriculturalZone a0(4, "Bastien", &poly3, "Wheat");
    cout << a0 << endl;

    //Test NaturalAndForestZone
    Point2D<int, float> p9(100, 100);
    Point2D<int, float> p10(200, 100);
    Point2D<int, float> p11(200, 200);
    Point2D<int, float> p12(100, 200);

    Polygon<int, float> poly2;
    vector<Point2D<int, float>> vertices2 = {p9, p10, p11, p12};
    poly2.setVertices(vertices2);

    NaturalAndForestZone n0(3, "Bastien", &poly2);
    cout << n0 << endl;

    //Test textToPlots
    vector<Plot*> plots = textToPlots("./plots/plots_short.txt");
    for (auto plot : plots)
    {
        switch (plot->getType())
        {
            case PlotType::URBAN_ZONE:
                cout << *dynamic_cast<UrbanZone*>(plot) << endl;
                break;
            case PlotType::ZONE_TO_BE_URBANIZED:
                cout << *dynamic_cast<ZoneToBeUrbanized*>(plot) << endl;
                break;
            case PlotType::NATURAL_AND_FOREST_ZONE:
                cout << *dynamic_cast<NaturalAndForestZone*>(plot) << endl;
                break;
            case PlotType::AGRICULTURAL_ZONE:
                cout << *dynamic_cast<AgriculturalZone*>(plot) << endl;
                break;
            default:
                cout << *plot << endl;
                break;
        }
    }

    //Test plotsToText
    plotsToText(plots);
    
}

Polygon<int, float>* createPolygons(string &line){
    stringstream ss(line); // get the coordinates
    int x, y;
    char ch;
    vector<Point2D<int, float>> vertices;
    while (ss >> ch)
    {
        char semiColon;
        ss  >> x >> semiColon >> y >> ch;
        Point2D<int, float> p(x, y);
        vertices.push_back(p);
    }
    Polygon<int, float>* shape = new Polygon<int, float>();
    shape->setVertices(vertices);
    return shape;
}

/**
 * @brief Function allowing to create a list of plots from a file
 * 
 * @param filename
 */
vector<Plot*> textToPlots(string filename)
{
    vector<Plot*> plots;
    ifstream file(filename);
    string line, owner, type, crop;
    int number, pBuildable;
    float builtArea;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream ss(line);
            ss >> type >> number >> owner;
            getline(file, line);
            Polygon<int, float>* shape = createPolygons(line);
            if (type == "ZU")
            {
                ss >> pBuildable >> builtArea >> builtArea;
                UrbanZone* u = new UrbanZone(number, owner, shape, pBuildable, builtArea);
                plots.push_back(u);
            }
            else if (type == "ZAU")
            {
                ss >> pBuildable;
                ZoneToBeUrbanized* z = new ZoneToBeUrbanized(number, owner, shape, pBuildable);
                plots.push_back(z);
            }
            else if (type == "ZN")
            {
                NaturalAndForestZone* n = new NaturalAndForestZone(number, owner, shape);
                plots.push_back(n);
            }
            else if (type == "ZA")
            {
                ss >> crop;
                AgriculturalZone* a = new AgriculturalZone(number, owner, shape, crop);
                plots.push_back(a);
            }
        }
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
    return plots;
}

/**
 * @brief Function allowing to create a file from a list of plots
 * 
 * @param plots 
 */
void plotsToText(vector<Plot*> plots){
    ofstream file("./plots/plots_out.txt");
    if (file.is_open())
    {
        for (auto plot : plots)
        {   
            file << PlotTypeToString(plot->getType()) << " " << plot->getNumber() << " " << plot->getOwner() << " ";
            switch (plot->getType())
            {
                case PlotType::URBAN_ZONE:
                {
                    UrbanZone* zu = dynamic_cast<UrbanZone*>(plot);
                    file << zu->getPBuildable() << " " << zu->getBuiltArea();
                    break;
                }
                case PlotType::ZONE_TO_BE_URBANIZED:
                {
                    file << plot->getPBuildable();
                    break;
                }
                case PlotType::NATURAL_AND_FOREST_ZONE:
                    break;
                case PlotType::AGRICULTURAL_ZONE:
                {
                    AgriculturalZone* za = dynamic_cast<AgriculturalZone*>(plot);
                    file << za->getCropType();
                    break;
                }
                default:
                    break;
            }
            file << "\n";
            for (auto vertex : plot->getShape()->getVertices())
            {
                file << "[" << vertex.getX() << ";" << vertex.getY() << "] ";
            }
            file << "\n";
        }
    }
    else
    {
        cout << "Unable to open file" << endl;
    }
}




    