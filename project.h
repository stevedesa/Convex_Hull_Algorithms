#ifndef PROJECT_H
#define PROJECT_H

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <random>
#include <ctime>
using namespace std;

struct Point {
    double x, y;

    // Helper Function when comparing LT 2 Points
    bool operator<(const Point& other) const {
        if (y == other.y) return x < other.x;
        return y < other.y;
    }

    // Helper Function when checked EQ 2 Points
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Reference point (bottommost point) for sorting
Point p0;

// Algorithm helper functions declarations
int orientation(const Point& p, const Point& q, const Point& r);
double polarAngle(const Point& origin, const Point& p);
double distanceSquared(const Point& p1, const Point& p2);
bool polarOrder(const Point& p1, const Point& p2);
bool isLeft(stack<Point> s);
Point nextToTop(stack<Point>& S);

// Algorithm functions declarations
vector<Point> jarvisMarch(vector<Point>& points);
vector<Point> grahamScan(vector<Point>& points);

// Main helper functions declarations
vector<Point> readPointsFromFile(const string& filename);
void printHull(const vector<Point>& hull);
vector<Point> generateCirclePoints(int radius, int numPoints);
vector<Point> generateRandomPoints(int numPoints, int seed, int maxX, int maxY);
void runTimeTest(bool useJarvis, int numRuns, const string& caseType, ofstream& csvFile);

#endif
