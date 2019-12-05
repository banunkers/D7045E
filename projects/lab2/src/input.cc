#include "input.h"
#include <fstream>
#include <iostream>
#include <iomanip>

PointSet readPointsFromFile() {
	std::ifstream inFile;
	std::string line;
	PointSet inputPoints = {};

	inFile.open("input.txt");
	if (!inFile) {
		std::cout << "Unable to open input file 'input.txt'\n";
	}
	
	bool firstLine = true;
	while (std::getline(inFile, line)) {
		if (firstLine) {
			if (std::stoi(line) < 3) {
				std::cout << "Invalid input point set: First line is not the number of points or the number of points is < 3" << "\n";
				inFile.close();
				exit(0);
			}
			firstLine = false;
		} else {
			// Find white space seperated x and y cordinate (float) of a point on the input line
			auto whiteSpace = line.find(' ');
			float x = std::stof(line.substr(0, whiteSpace));
			float y = std::stof(line.substr(whiteSpace + 1, line.size()));
			auto point = Point(x, y);
			inputPoints.push_back(point);
		}
	}

	inFile.close();
	return inputPoints;
}

PointSet randomPointSet(int numPoints) {
	std::srand(std::time(NULL)); // new random seed
	while (true) {
		auto set = PointSet();
		for (int i = 0; i < numPoints; i++) {
			auto signOfX = (rand() % 2 == 1) ? -1.0f : 1.0f;
			auto signOfY = (rand() % 2 == 1) ? -1.0f : 1.0f;	
			auto x = (rand() % 100) / 100.0f * signOfX;
			auto y = (rand() % 100) / 100.0f * signOfY;		
			set.push_back(Point(x, y));
		}

		auto pointSetErrors = validatePointSet(set);
		if (pointSetErrors == 0) {
			return set;
		}
	}
}

int validatePointSet(PointSet &set) {
	float maxY, minY, maxX, minX;

	// Check for duplicate points and extract min and max coordinates
	for (int i = 0; i < set.size(); i++) {
		auto point = set[i];

		if (point.x > maxX) {
			maxX = point.x;
		} else if (point.x < minX) {
			minX = point.x;
		}

		if (point.y > maxY) {
			maxY = point.y;
		} else if (point.y < minY) {
			minY = point.y;
		}

		for (int j = i + 1; j < set.size(); j++) {
			if (point == set[j]) {
				return 1;
			}
		}
	}

	// Check if origin is inside the convex hull of the point set
	if (!(maxY > 0 && minY < 0 && maxX > 0 && minX < 0)) {
		return 2;
	}

	// point set ok
	return 0;
}