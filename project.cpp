#include "project.h"
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

// Determines the orientation (collinear/CW/CCW) of 3 points
int orientation(const Point& p, const Point& q, const Point& r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;   // 0 -> collinear
    return (val > 0) ? 1 : 2; // 1 -> CW, 2 -> CCW
}

// Calculates polar angle between 2 points
double polarAngle(const Point& origin, const Point& p) {
    return atan2(p.y - origin.y, p.x - origin.x);
}

// Calculates squared distance between two points
double distanceSquared(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Sorts points based on polar angle
bool polarOrder(const Point& p1, const Point& p2) {
    int o = orientation(p0, p1, p2);
    if (o == 0) return distanceSquared(p0, p1) < distanceSquared(p0, p2);
    return o == 2;
}

// Checks if point at top is to the left of the line ab using cross product
bool isLeft(stack<Point> s) {
    if (s.size() < 3) return false;

    Point c = s.top();
    s.pop(); // Unload
    Point b = s.top();
    s.pop(); // Unload
    Point a = s.top();

    s.push(b); // Reload
    s.push(c); // Reload

    double crossProduct = ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
    return crossProduct > 0; // +ve = left turn
}

// Determines the point next to the top of stack
Point nextToTop(stack<Point>& S) {
    Point top = S.top();
    S.pop();
    Point next = S.top();
    S.push(top);
    return next;
}

// GRADING: GRAHAM
vector<Point> grahamScan(vector<Point>& points) {
    int n = points.size();

    // Not possible with less than 3 points
    if (n < 3) return {};

    // Find the bottom-most point (or leftmost in case of tie)
    p0 = *min_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        return (p1.y < p2.y) || (p1.y == p2.y && p1.x < p2.x);
    });

    // Sort points by polar angle with respect to p0
    sort(points.begin(), points.end(), polarOrder);

    // Remove duplicates and keep only the farthest point for collinear points
    vector<Point> uniquePoints = { points[0] };
    for (size_t i = 1; i < points.size(); ++i) {
        while (i < points.size() - 1 && orientation(p0, points[i], points[i + 1]) == 0) {
            i++;
        }
        uniquePoints.push_back(points[i]);
    }

    // Stack for the convex hull points
    stack<Point> S;
    S.push(uniquePoints[0]);
    S.push(uniquePoints[1]);
    S.push(uniquePoints[2]);

    for (size_t i = 3; i < uniquePoints.size(); ++i) {
        while (S.size() > 1 && orientation(nextToTop(S), S.top(), uniquePoints[i]) != 2) {
            S.pop();
        }
        S.push(uniquePoints[i]);
    }

    // Collect points from the stack into the convex hull vector
    vector<Point> hull;
    while (!S.empty()) {
        hull.push_back(S.top());
        S.pop();
    }

    // Reverse the hull to maintain the order from p0 outward
    reverse(hull.begin(), hull.end());

    return hull;
}

// GRADING: JARVIS
vector<Point> jarvisMarch(vector<Point>& points) {
    int n = points.size();

    // Not possible with less than 3 points
    if (n < 3) return {};

    vector<Point> hull;
    stack<Point> stack;

    // Find the bottom-most point (or leftmost in case of tie)
    p0 = *min_element(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        return (p1.y < p2.y) || (p1.y == p2.y && p1.x < p2.x);
    });

    // Initialize with bottom-most point
    stack.push(p0);
    hull.push_back(p0);

    // Left side going up
    while (stack.size() < 3 || isLeft(stack)) {
        double minAngle = 180.0;
        Point next = points[0];
        bool foundNext = false;

        for (const Point& p : points) {
            if ((find(hull.begin(), hull.end(), p) != hull.end()) || (p.x == stack.top().x && p.y == stack.top().y)) {
                continue; // Skip if point is already in hull or if its the current point
            }

            // Only consider points above or at same level
            if (p.y >= stack.top().y) {
                double angle = polarAngle(stack.top(), p);
                double distToP = distanceSquared(stack.top(), p);
                double distToNext = distanceSquared(stack.top(), next);

                bool isFarColinear = (angle == minAngle && distToP > distToNext);
                if (!foundNext || angle < minAngle || isFarColinear) {
                    minAngle = angle;
                    next = p;
                    foundNext = true;
                }
            }
        }

        if (!foundNext) break; // If point not found

        stack.push(next);
        hull.push_back(next);
    }

    Point top = stack.top(); // Store top point for the right side

    // Right side going down
    while (true) {
        double maxAngle = 180.0;
        Point next = points[0];
        bool foundNext = false;

        for (const Point& p : points) {
            if (p.x == stack.top().x && p.y == stack.top().y) {
                continue; // Skip if it's the current point
            }

            // Only consider points below or at same level
            if (p.y <= stack.top().y) {
                double angle = polarAngle(stack.top(), p);
                double distToP = distanceSquared(stack.top(), p);
                double distToNext = distanceSquared(stack.top(), next);

                bool isFarColinear = (angle == maxAngle && distToP > distToNext);
                if (!foundNext || angle < maxAngle || isFarColinear) {
                    maxAngle = angle;
                    next = p;
                    foundNext = true;
                }
            }
        }

        if (!foundNext || (next == p0)) {
            break; // If we reached the start point or didn't find a next point
        }

        stack.push(next);
        hull.push_back(next);
    }

    return hull;
}

// Function to read points from file
vector<Point> readPointsFromFile(const string& filename) {
    ifstream file(filename);
    vector<Point> points;

    double x, y;
    while (file >> x >> y) {
        points.push_back({ x, y });
    }

    return points;
}

// Function to print points in the required format
void printHull(const vector<Point>& hull) {
    cout << fixed << setprecision(2); // Set precision to 2

    cout << hull.size() << endl;

    for (const auto& point : hull) {
        cout << point.x << " " << point.y << endl;
    }
}

// Function to generate points in a circle
vector<Point> generateCirclePoints(int radius, int numPoints) {
    vector<Point> points;
    for (int i = 0; i < numPoints; i++) {
        double angle = 2 * 3.141592653589793 * i / numPoints;
        double x = radius * cos(angle);
        double y = radius * sin(angle);
        points.push_back({ x, y });
    }
    return points;
}

// Function to generate random points
vector<Point> generateRandomPoints(int numPoints, int seed, int maxX, int maxY) {
    vector<Point> points;
    mt19937 gen(seed); // Using seed to make sure data sets are the same for testing purposes
    uniform_int_distribution<> disX(0, maxX);
    uniform_int_distribution<> disY(0, maxY);

    for (int i = 0; i < numPoints; ++i) {
        points.push_back({ static_cast<double>(disX(gen)), static_cast<double>(disY(gen)) });
    }
    return points;
}

// Function to run time tests
void runTimeTest(bool useJarvis, int numRuns, const string& caseType, ofstream& csvFile) {
    string algorithm = useJarvis ? "Jarvis" : "Graham";
    vector<Point> points;

    vector<int> seeds = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 }; // INCREASE IF YOU NEED TO

    for (int run = 1; run <= numRuns; run++) {
        if (caseType == "Average") {
            points = generateRandomPoints(100, seeds[run], 100, 100); // Random points within 100x100 grid
        }
        else if (caseType == "Worst") {
            points = generateCirclePoints(100 + run, 100); // Vary radius slightly
        }
        else if (caseType == "Best") {
            points = generateRandomPoints((40*(numRuns - run + 1)-4), seeds[run], 9, 9); // Generate interior points
            points.insert(points.end(), { {0, 0}, {10, 0}, {10, 10}, {0, 10} }); // Guaranteed hull points
        }

        auto start = chrono::high_resolution_clock::now();
        vector<Point> hull = useJarvis ? jarvisMarch(points) : grahamScan(points);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end - start;

        double hullRatio = static_cast<double>(hull.size()) / static_cast<double>(points.size());

        csvFile << algorithm << "," << caseType << "," << duration.count() << "," << hullRatio << "\n";
    }
}

// Main function with test cases
int main(int argc, char* argv[]) {
    if (argc == 2 || argc == 3) {
        string option = argv[1];

        if (option == "-j" || option == "-g") {
            string testfile = argv[2];

            vector<Point> points = readPointsFromFile(testfile);

            vector<Point> hull = (option == "-j") ? jarvisMarch(points) : grahamScan(points);

            printHull(hull);
        }
        else if (option == "-t") {
            int numRuns = 10; // IF YOU CHANGE THIS YOU NEED TO ADD MORE SEEDS IN RUNTIMETESTS

            ofstream csvFile("results.csv");
            csvFile << "Algorithm, Case, Time(ms), Hull Ratio\n";

            // Average case tests
            runTimeTest(true, numRuns, "Average", csvFile);
            runTimeTest(false, numRuns, "Average", csvFile);

            // Best case tests
            runTimeTest(true, numRuns, "Best", csvFile);
            runTimeTest(false, numRuns, "Best", csvFile);

            // Worst case tests
            runTimeTest(true, numRuns, "Worst", csvFile);
            runTimeTest(false, numRuns, "Worst", csvFile);

            csvFile.close();
        }
        else {
            cerr << "Invalid option / arguments\n";
            return 1;
        }
    }
    else {
        cerr << "Usage: project.exe -[j|g|t] testfile\n";
        cerr << "       -j|g: JavisMarch|GrahamScan\n";
        cerr << "       -t: Run tests\n";
        return 1;
    }

    return 0;
}
