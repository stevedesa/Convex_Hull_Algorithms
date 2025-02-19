# Convex Hull Algorithms: Jarvis's March vs. Graham's Scan

## Project Overview

This project compares the performance of two popular convex hull algorithms: **Jarvis's March** and **Graham's Scan**. The goal is to evaluate their efficiency and correctness under different input scenarios, including average, best, and worst-case conditions. The project includes:

1. **Implementation of both algorithms** in 2D.
2. **Testing framework** to measure execution time and hull ratio (ratio of hull points to total points).
3. **Graphical and tabular analysis** of the results.

The project also explores the extension of Graham's Scan to 3D, providing a high-level approach and pseudocode for constructing a 3D convex hull.

---

## Key Features

### 1. **2D Convex Hull Algorithms**
   - **Jarvis's March**: A simple algorithm with O(nh) complexity, where `n` is the number of points and `h` is the number of hull points.
   - **Graham's Scan**: A more efficient algorithm with O(n log n) complexity, leveraging sorting and stack-based operations.

### 2. **3D Convex Hull Extension**
   - **Graham's Scan in 3D**: A proposed method to extend Graham's Scan to 3D by projecting points onto a 2D plane, constructing a base hull, and then extending it into the third dimension using triangular facets.

### 3. **Testing Framework**
   - **Input Generation**: Generates points for different test cases:
     - **Random Points**: Uniformly distributed points.
     - **Circle Points (Worst Case)**: Points on a circle, forcing the hull to include all points.
     - **Best Case**: Points forming a rectangle with additional interior points.
   - **Performance Metrics**: Measures execution time and hull ratio for each algorithm.
   - **Result Logging**: Exports results to a CSV file (`results.csv`) for further analysis.

### 4. **Graphical and Tabular Analysis**
   - **Graphs**: Visual comparison of execution times and hull ratios for both algorithms across different cases.
   - **Raw Data Tables**: Detailed execution times and hull ratios for each test run.

---

## Results Summary

### 1. **Average Case (Random Points)**
   - **Graham's Scan** outperforms Jarvis's March due to its O(n log n) complexity.
   - **Jarvis's March** is slower as it iterates over all points for each hull vertex.

### 2. **Best Case (Rectangular Hull with Interior Points)**
   - **Graham's Scan** is more efficient, quickly filtering out interior points.
   - **Jarvis's March** is slower due to its O(nh) complexity, even when the number of hull points is small.

### 3. **Worst Case (Points on a Circle)**
   - **Graham's Scan** maintains steady performance with O(n log n) complexity.
   - **Jarvis's March** suffers significantly with O(n^2) complexity, as it must include all points in the hull.

---

## Conclusion
This project provides a comprehensive comparison of Jarvis's March and Graham's Scan, highlighting the strengths and weaknesses of each algorithm under different scenarios. The results demonstrate that Graham's Scan is generally more efficient, especially for large datasets and worst-case scenarios. The proposed 3D extension of Graham's Scan offers a promising direction for future work in computational geometry.
