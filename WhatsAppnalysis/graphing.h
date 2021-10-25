#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <math.h>
#include "Array Processing Functions.h"
#include "Styles.h"
#include "Test.h"
#include "Time Period Finding Functions.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;



#define TIMETYPE 0
#define INTTYPE 1
#define STRINGTYPE 2
#define DATETYPE 3
#define MONTHTYPE 4
#define WEEKTYPE 5
#define COLOUR_BLACK RGB(0,0,0)
#define GRAPH_COLOUR TEXT_COLOUR
#define LINE_GRAPH 0
#define BAR_GRAPH 1
#define NOCHANGE std::vector<double> {-1, 10000000000}
#define COLOURS std::vector<COLORREF> {RGB(0,0,0), RGB(128,128,128), RGB(255,0,127), RGB(255,0,255), RGB(0, 0, 255), RGB(0, 255, 255), RGB(0, 255, 0), RGB(255, 255, 0), RGB(255, 128, 0), RGB(255, 0, 0)}







//Assigns an object a colour on a rotating list defined by COLOURS
int assignColour(int lastColour) {
	while (COLOURS.size() < lastColour) {
		lastColour -= COLOURS.size();
	}
	return(COLOURS[lastColour]);
}


bool majorityIsDecimal(std::vector<double> thearray) {
	int numberDecimal = 0;
	int tester = 0;
	for (double val : thearray) {
		tester = val;
		if (tester != val) {
			numberDecimal++;
		}
	}
	int threshold = (int)(thearray.size() * 3.0) / 4.0;
	if (numberDecimal > threshold) {
		return true;
	}
	else {
		return false;
	}
}


double getMinPlaceValue(double thenumber) {
	std::string newNumber = to_string(thenumber);
	for (int i = 0; i < newNumber.size(); i++) {
		if (newNumber[i] == '.') {
			return(1.0/(pow(newNumber.size() - (i + 1), 10.0)));
		}
	}
	return 1;
}

//Finds the labels to be used on the y Axis
std::vector<double> yAxisValuesFind(std::vector<double> coordinates) {
	std::vector<double> yAxisVals = {};
	if (majorityIsDecimal(coordinates)) {
		double nearestMultiple = getMinPlaceValue(coordinates[0]) *10.0;
		int largestY = (int)arrayreverse(sortarr(coordinates))[0];
		int yInterval = 0;
		if (largestY < 20) {
			yInterval = 1;
			yAxisVals.push_back(0);
			for (int i = 1; i <= largestY; i++) {
				yAxisVals.push_back(yInterval * i);
			}
			yAxisVals.push_back(yInterval * (largestY + 1));
		}
		else {
			bool largestYMultiple = (largestY % int(nearestMultiple) == 0);
			while (not largestYMultiple) {
				largestY++;
				largestYMultiple = (largestY % int(nearestMultiple) == 0);
			}
			yInterval = largestY / 10;
			yAxisVals.push_back(0);
			for (int i = 1; i <= 10; i++) {
				yAxisVals.push_back(yInterval * i);
			}
			yAxisVals.push_back(yInterval * (11));
		}
	}
	else {
		int nearestMultiple = 10;
		int largestY = (int)arrayreverse(sortarr(coordinates))[0];
		int yInterval = 0;
		if (largestY < 20) {
			yInterval = 1;
			yAxisVals.push_back(0);
			for (int i = 1; i <= largestY; i++) {
				yAxisVals.push_back(yInterval * i);
			}
			yAxisVals.push_back(yInterval * (largestY + 1));
		}
		else {
			bool largestYMultiple = (largestY % nearestMultiple == 0);
			while (not largestYMultiple) {
				largestY++;
				largestYMultiple = (largestY % nearestMultiple == 0);
			}
			yInterval = largestY / 10;
			yAxisVals.push_back(0);
			for (int i = 1; i <= 10; i++) {
				yAxisVals.push_back(yInterval * i);
			}
			yAxisVals.push_back(yInterval * (11));
		}
	}
	return yAxisVals;
}




//A single bar on a bar graph
class GraphBar {
private:
	friend class boost::serialization::access;

	
	
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& yValue;
		ar& colour;
	}

public:
	std::string name;
	int yValue;
	COLORREF colour;
};


//A group of bars to create a mulitbar graph
class GraphMultiBar {
private:
	friend class boost::serialization::access;

	
	
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& subBars;
		ar& subBarsSelected;
		ar& name;
		ar& width;

	}

public:
	std::map<std::string, GraphBar> subBars;
	std::map< std::string, bool> subBarsSelected;
	std::string name;
	int width;
};

//A line of best fit
class LineOfBestFit {
private:
	friend class boost::serialization::access;



	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& xCoor;
		ar& yCoor;
		ar& gradient;
		ar& startX;
		ar& startY;
		ar& endX;
		ar& endY;
	}


public:
	std::vector<double> xCoor, yCoor;
	double gradient, startX, startY, endX, endY;

	//Finds the gradient the line of best fit should have
	void gradientFind() {
		double totalXChange = 0;
		double totalYChange = 0;
		double numPoints = (double)xCoor.size() - 1;
		for (int i = 0; i < numPoints; i++) {
			totalXChange += (xCoor[i + 1] - xCoor[i]);
			totalYChange += (yCoor[i + 1] - yCoor[i]);
		}
		double averageXChange = totalXChange / numPoints;
		double averageYChange = totalYChange / numPoints;
		gradient = averageYChange / averageXChange;
	}

	//Finds the start and end coordintes of the line
	void startEndFind() {
		double totalY = 0;
		for (double coor : yCoor) {
			totalY += coor;
		}
		double ySize = (double)yCoor.size();
		double averageY = totalY / ySize;
		std::vector<double> xCoorSorted = sortarr(xCoor);
		startX = xCoorSorted[0];
		endX = xCoorSorted[xCoorSorted.size() - 1];
		double midX = (endX - startX) / 2.0;
		double changeYHalf = gradient * midX;
		startY = averageY - changeYHalf;
		endY = averageY + changeYHalf;
	}

	//Puts data into the variables and performs calculations required to form the line of best fit
	void populate(std::vector<double> x, std::vector<double> y) {
		xCoor = x;
		yCoor = y;
		gradientFind();
		startEndFind();
	}
};

//A line used on a line graph
class GraphLine {
private:
	friend class boost::serialization::access;

	
	
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& xValues;
		ar& yValues;
		ar& colour;
		ar& name;
		ar& lobf;
	}
public:
	std::vector<double> xValues = {}, yValues = {};
	COLORREF colour = GRAPH_COLOUR;
	std::string name = "";
	LineOfBestFit lobf;


	void findLOBF() {
		lobf.populate(xValues, yValues);
	}

};



//Retrieves an array of the keys of a string : GraphLine map
std::vector<std::string> mapkeysGraph(std::map<std::string, GraphLine> themap) {
	std::vector<std::string> keys;
	for (map<std::string, GraphLine>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

//Retrieves an array of the keys of a string : GraphBar map
std::vector<std::string> mapkeysGraph(std::map<std::string, GraphBar> themap) {
	std::vector<std::string> keys;
	for (map<std::string, GraphBar>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

//Retrieves an array of the keys of a string : GraphMultiBar map
std::vector<std::string> mapkeysGraph(std::map<std::string, GraphMultiBar> themap) {
	std::vector<std::string> keys;
	for (map<std::string, GraphMultiBar>::iterator it = themap.begin(); it != themap.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}







//Graph
class Graph {
public:
	HDC hdc;
	std::string title;
	std::string xAxisLabel, yAxisLabel;
	int xLength, xIntervalActual, xIntervalPixel, xType,
		yLength, yIntervalActual, yIntervalPixel, yType;
	std::vector<std::string> xAxisValues, yAxisValues;
	HWND window;
	int type;
	std::map<std::string, COLORREF> keyItems = {};
	int keyXPos = 0;
	int keyYPos = 0;
	int keyLength = 0;
	int keyHeight = 0;
	int xDist = 0;
	int yDist = 0;
	int graphXPos = 0;
	int	graphYPos = 0;

	
	//Adds Text to a graph
	void addGraphText(std::string text, int x, int y, COLORREF colour) {
		(HFONT)SelectObject(hdc, hFont);
		SetTextColor(hdc, colour);
		SetBkMode(hdc, TRANSPARENT);
		TextOutA(hdc,
			x , y,
			text.c_str(), text.size());
		

	}

	//////////Axis 

	//Draws axis values
	bool drawValueLabelsLine(int xPos, int yPos) {
		SetTextAlign(hdc, TA_CENTER | TA_TOP | TA_NOUPDATECP);
		for (int i = 0; i < xAxisValues.size(); i++) {
			addGraphText((xAxisValues[i]), xPos + (xIntervalPixel * i), (yPos + yLength) + 11, GRAPH_COLOUR);
			drawLinePortion(xPos + (xIntervalPixel * i), (yPos + yLength), xPos + (xIntervalPixel * i), (yPos + yLength) + 10, GRAPH_COLOUR);

		}
		SetTextAlign(hdc, TA_RIGHT | TA_BASELINE | TA_NOUPDATECP);
		for (int i = 0; i < yAxisValues.size(); i++) {
			addGraphText((yAxisValues[i]), xPos - 15, yPos + (yLength - (yIntervalPixel * i)) + 5, GRAPH_COLOUR);
			drawLinePortion(xPos, yPos + (yLength - (yIntervalPixel * i)), xPos - 10, (yLength - (yIntervalPixel * i)), GRAPH_COLOUR);

		}
		SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);


		return true;
	}

	
	//Draws Y axis text
	bool drawVerticalText(std::string textIn, int xPos, int yPos) {

		for (int i = 0; i < textIn.size(); i++) {
			addGraphText(textIn.substr(i, 1), xPos, yPos + (i * 15), GRAPH_COLOUR);
		}
		return true;
	}

	//Draws axis labels
	bool drawLabels(int xPos, int yPos) {
		addGraphText(xAxisLabel, ((xPos)+((xLength / 2) - ((xAxisLabel.size() * 7) / 2))), yPos + (yLength + 30), GRAPH_COLOUR);
		drawVerticalText(yAxisLabel, xPos - 100, (((yLength / 2) - ((xAxisLabel.size() / 2) * 45))));

		return true;
	}

	//Draws title
	bool drawTitle(int xPos, int yPos) {
		addGraphText(title, ((xLength / 2) - ((xAxisLabel.size() * 7) / 2)) + xPos, 0, GRAPH_COLOUR);
		return true;
	}

	//Draws the graph axis
	bool drawAxis(int xPos, int yPos) {
		//Y-Axis
		drawLinePortion(xPos, yPos, xPos, yPos + yLength, GRAPH_COLOUR);
		//X-Axis
		drawLinePortion(xPos, yPos + yLength, xPos + xLength, yPos + yLength, GRAPH_COLOUR);
		if (type == LINE_GRAPH) {
			drawValueLabelsLine(xPos, yPos);
		}
		drawLabels(xPos, yPos);
		drawTitle(xPos, yPos);
		return true;
	}


	//////////////////


	////////////////Lines


	//Draws a line between 2 points
	bool drawLinePortion(int xStart, int yStart, int xEnd, int yEnd, COLORREF colour, int type = PS_SOLID) {
		HGDIOBJ pen = CreatePen(type, 1, colour);
		SelectObject(hdc, pen);
		MoveToEx(hdc, xStart, yStart, (LPPOINT)NULL);
		LineTo(hdc, xEnd, yEnd);
		DeleteObject(pen);
		return true;

	}

	

	///////////////Key

	//Draws outline of a rectangle
	void drawRectangle(int xPos, int yPos, int length, int height) {
		drawLinePortion(xPos, yPos, xPos, yPos + height, GRAPH_COLOUR);
		drawLinePortion(xPos, yPos + height, xPos + length, yPos + height, GRAPH_COLOUR);
		drawLinePortion(xPos + length, yPos + height, xPos + length, yPos, GRAPH_COLOUR);
		drawLinePortion(xPos + length, yPos, xPos, yPos, GRAPH_COLOUR);
	}


	//Draws a square filled with colour 'colour'
	void drawFilledSquare(COLORREF colour, int xPos, int yPos, int length, int height) {
		HBRUSH brush = CreateSolidBrush(colour);
		HPEN pen = CreatePen(PS_SOLID, 1, colour);
		SelectObject(hdc, brush);
		SelectObject(hdc, pen);
		Rectangle(hdc, xPos, yPos + height / 2, xPos + length, yPos + height / 2 + height);
		DeleteObject(brush);
		DeleteObject(pen);
	}

	//Adds an item to the key
	void addKeyItem(std::string name, COLORREF colour) {
		keyItems[name] = colour;
	}

	//Removes an item from the key
	void removeKeyItem(std::string name) {
		if (keyItems.find(name) != keyItems.end()) {
			keyItems.erase(name);
		}
	}

	//Calculates the dimensions of the key
	void keySizeCalc(std::vector<std::string> items) {
		int longestItem = 0;
		keyHeight = items.size() * 30 + 40;
		for (std::string item : items) {
			if (item.length() > longestItem) {
				longestItem = item.length();
			}
		}
		keyLength = (longestItem * 10) + 50;
	}

	//Sets the position of the key relative to the start of the graph
	void keyPositionSet() {
		keyYPos = 10;
		keyXPos = 50;
	}

};


//Bar Graph
class BarGraph : public Graph {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& bars;
		ar& barsSelected;
		ar& title;
		ar& yAxisValuesInt;
		ar& xAxisLabel;
		ar& yAxisLabel;
		ar& xLength;
		ar& xIntervalActual;
		ar& xIntervalPixel;
		ar& xType;
		ar& yLength;
		ar& yIntervalActual;
		ar& yIntervalPixel;
		ar& yType;
		ar& xAxisValues;
		ar& yAxisValues;
		ar& type;
		ar& lastColour;
		ar& keyXPos;
		ar& keyYPos;
		ar& keyItems;
		ar& keyLength;
		ar& keyHeight;
		ar& width;
	}


public:
	std::map<std::string, GraphMultiBar> bars = {};
	std::vector<std::string> barNames = {};
	std::vector<int> yAxisValuesInt = {};
	std::map<std::string, bool> barsSelected = {};
	int lastColour = 0;
	std::map<std::string, COLORREF> subBarColours = {};
	int xIntervalPixel = 0;
	int width = 0;

	///////////Data processing
	
	//Finds the intervals between x and y axis values
	void findIntervals() {
		std::vector<int> newYAxisValues = {};
		int valueIntervals = 0;
		for (int i = 1; i < yAxisValuesInt.size(); i++) {
			valueIntervals = i;
			yIntervalPixel = floor(yLength / (yAxisValuesInt.size() / i));
			if (yIntervalPixel != 0) {
				i = yAxisValuesInt.size();
			}
		}
		for (int i = 0; i < yAxisValuesInt.size(); i += valueIntervals) {
			newYAxisValues.push_back(yAxisValuesInt[i]);
		}
		yAxisValues = {};
		for (int value : newYAxisValues) {
			yAxisValues.push_back(to_string(value));
		}
	}

	

	//////////////////Key

	//Draws the key
	void drawKey() {
		keyPositionSet();
		std::vector<std::string> names = mapkeysColour(keyItems);
		std::vector<COLORREF> colours = mapvaluesColour(keyItems);
		int displayed = 0;
		std::vector<std::string> displayedNames = {};
		std::map<std::string, bool> subBarsSelected = {};
		for (std::string bar : barNames) {
			for (std::string subBar : mapkeysGraph(bars[bar].subBars)) {
				if (bars[bar].subBarsSelected[subBar]) {
					subBarsSelected[subBar] = true;
				}
				else {
					if (subBarsSelected.find(subBar) == subBarsSelected.end()) {
						subBarsSelected[subBar] = false;
					}
				}
			}
		}
		if (subBarsSelected["Total"]) {
			displayed++;
			displayedNames.push_back("Total");
			addGraphText("Total", keyXPos + 50, (keyYPos + 40), GRAPH_COLOUR);
			drawFilledSquare(COLOUR_BLACK, keyXPos + 10, keyYPos + 40, 10, 10);
		}
		for (int i = 0; i < names.size(); i++) {
			if (names[i] != "Total") {
				if (subBarsSelected[names[i]]) {
					addGraphText(names[i], keyXPos + 50, (keyYPos + (30 * (displayed)) + 40), GRAPH_COLOUR);
					drawFilledSquare(colours[i], keyXPos + 10, (keyYPos + (30 * (displayed)) + 40), 10, 10);
					displayed++;
					displayedNames.push_back(names[i]);
				}
			}
		}
		if (displayed > 0) {
			keySizeCalc(displayedNames);
			addGraphText("KEY", keyXPos + (keyLength / 2) - 10, keyYPos, GRAPH_COLOUR);
			drawRectangle(keyXPos, keyYPos, keyLength, keyHeight);
		}
		else {
			addGraphText("No Items Selected", keyXPos, keyYPos + 10, GRAPH_COLOUR);
			keyLength = 150;
		}

	}


	///////////////Display


	//Sets the width of a bar
	void setBarWidth(int widthIn) {
		width = widthIn;
	}

	//Sets the the distance between bars
	void setBarInterval(int interval) {
		xIntervalPixel = interval;
	}

	//Updates the values of a bar to new values
	bool updateBar(std::string name, std::map<std::string, double> newY) {
		if (newY.size() == bars[name].subBars.size()) {
			for(std::string subBar : mapkeysGraph(bars[name].subBars))
			bars[name].subBars[subBar].yValue = newY[subBar];
		}
		return true;
	}


	//Removes a bar from the graph
	bool removeBar(std::string name) {
		bars.erase(name);
		return true;
	}

	//Removes a subBar from one bar on the graph
	bool removeSubBar(std::string barName, std::string subBarName) {
		bars[barName].subBars.erase(subBarName);
		removeKeyItem(subBarName);
		return true;
	}


	//Adds a framework subBar to the graph
	bool addSubBarNoVals(std::string subBarName){
		subBarColours[subBarName] = assignColour(lastColour);
		lastColour++;
		return true;
	}

	//Adds a bar to the graph
	bool addBar(std::map<std::string, double> heights, std::string name, bool selected) {
		GraphMultiBar bar;
		bar.name = name;
		double yIntervalPixelDecimal = yIntervalPixel;
		double yIntervalActualDecimal = yIntervalActual;
		for (std::string subName : mapkeysdouble(heights)) {
			GraphBar subBar;
			if (subBarColours.find(subName) == subBarColours.end()) {
				subBar.colour = assignColour(lastColour);
				addKeyItem(subName, subBar.colour);
				subBarColours[subName] = subBar.colour;
				lastColour++;
			}
			else {
				subBar.colour = subBarColours[subName];
			}
			subBar.yValue = ((heights[subName]- yAxisValuesInt[0]) * (yIntervalPixelDecimal / yIntervalActualDecimal)) - graphYPos;
			bar.subBars[subName] = subBar;
			bar.subBarsSelected[subName] = true;
		}
		bars[name] = bar;
		barsSelected[name] = selected;
		return true;
	}

	//Adds a subBar with values to one bar
	bool addSubBar(std::string barName, std::string subBarName, int height, bool selected) {
		GraphBar subBar;
		double yIntervalPixelDecimal = yIntervalPixel;
		double yIntervalActualDecimal = yIntervalActual;
		if (subBarColours.find(subBarName) == subBarColours.end()) {
			subBar.colour = assignColour(lastColour);
			addKeyItem(subBarName, subBar.colour);

			subBarColours[subBarName] = subBar.colour;
			lastColour++;
		}
		else {
			subBar.colour = subBarColours[subBarName];
		}
		subBar.yValue = (height - yAxisValuesInt[0]) * (yIntervalPixelDecimal / yIntervalActualDecimal) - graphYPos;
		bars[barName].subBars[subBarName] = subBar;
		bars[barName].subBarsSelected[subBarName] = selected;
	}

	//Adds a subBar with values to all bars
	bool addSubBarAll(std::string subBarName, std::map < std::string, double > heights, bool selected) {
		double yIntervalPixelDecimal = yIntervalPixel;
		double yIntervalActualDecimal = yIntervalActual;
		if (heights.size() == bars.size()) {
			for (std::string bar : barNames) {
				GraphBar subBar;
				if (subBarColours.find(subBarName) == subBarColours.end()) {
					subBar.colour = assignColour(lastColour);
					addKeyItem(subBarName, subBar.colour);
					subBarColours[subBarName] = subBar.colour;
					lastColour++;
				}
				else {
					subBar.colour = subBarColours[subBarName];
				}
				subBar.yValue = (heights[subBarName] - yAxisValuesInt[0]) * (yIntervalPixelDecimal / yIntervalActualDecimal);
				bars[bar].subBars[subBarName] = subBar;
				bars[bar].subBarsSelected[subBarName] = selected;
			}
		}
		return true;
	}

	//Selects a bar for display
	void selectBar(std::string bar) {
		barsSelected[bar] = true;
	}

	//Selects a specific subBar for display
	void selectSubBarSingle(std::string barName, std::string subBarName) {
		bars[barName].subBarsSelected[subBarName] = true;
	}

	//Selects a subBar for display on all bars
	void selectSubBarAll(std::string subBarName) {
		for (std::string bar : barNames) {
			bars[bar].subBarsSelected[subBarName] = true;
		}
	}

	//Deselects a bar from display
	void deselectBar(std::string bar) {
		barsSelected[bar] = false;
	}

	//Deselects a specific subBar from display
	void deselectSubBarSingle(std::string barName, std::string subBarName) {
		bars[barName].subBarsSelected[subBarName] = false;
	}

	//Deselects a subBar from display on all Bars
	void deselectSubBarAll(std::string subBarName) {
		for (std::string bar : barNames) {
			bars[bar].subBarsSelected[subBarName] = false;
		}
	}



	//Draws a bar
	void drawBar(GraphMultiBar bar, int startPos) {
		int numBars = 0, subBarWidth , xPos, yPos;
		SetTextAlign(hdc, TA_CENTER);
		addGraphText(bar.name,
			startPos + (width /2) + xIntervalPixel,
			(graphYPos + yLength),
			GRAPH_COLOUR);
		SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);

		std::vector<std::string> subBarNames = {};
		subBarNames.push_back("Total");
		for (std::string subBar : mapkeysGraph(bar.subBars)) {
			if (subBar != "Total") {
				subBarNames.push_back(subBar);
			}
		}
		for (std::string subBar : subBarNames) {
			if (bar.subBarsSelected[subBar]) {
				subBarWidth = width / numSelected(bar.subBarsSelected);
				HBRUSH brush = CreateSolidBrush(bar.subBars[subBar].colour);
				HPEN pen = CreatePen(NULL, NULL, NULL);
				SelectObject(hdc, pen);
				SelectObject(hdc, brush);
				xPos = startPos + (numBars * subBarWidth) + xIntervalPixel;
				yPos = graphYPos + yLength + 1;
				Rectangle(hdc,
					xPos,
					yPos,
					xPos + subBarWidth + 1,
					yPos - bar.subBars[subBar].yValue);
				numBars++;
				DeleteObject(brush);
				DeleteObject(pen);
			}
		}
	}

	//Draws the value labels for the Y axis on the bar chart
	bool drawValueLabelsBar(int xPos, int yPos){
		SetTextAlign(hdc, TA_RIGHT | TA_BASELINE | TA_NOUPDATECP);
		for (int i = 0; i < yAxisValues.size(); i++) {
			addGraphText((yAxisValues[i]), xPos - 15, yPos + (yLength - (yIntervalPixel * i)) + 5, GRAPH_COLOUR);
			drawLinePortion(xPos, yPos + (yLength - (yIntervalPixel * i)), xPos - 10, (yLength - (yIntervalPixel * i)), GRAPH_COLOUR);
		}
		SetTextAlign(hdc, TA_LEFT | TA_TOP | TA_NOUPDATECP);
		
		return true;
	}

	//Refreshes the bars
	void refreshBars() {
		int barsDrawn = 0;
		std::vector<std::string> xAxisValuesNew = {};
		double xIntervalPixelDouble = xIntervalPixel;
		double constantAdd = xIntervalPixelDouble * (150.0 / xIntervalPixelDouble);
		for (std::string bar : barNames) {
			if (barsSelected[bar]) {
				xAxisValuesNew.push_back(bar); 
				drawBar(bars[bar], graphXPos + barsDrawn * (xIntervalPixel+width));
				barsDrawn++;
			}
		}
		xAxisValues = xAxisValuesNew;
	}

	//Gets labels to be used under bars
	void getBarNames() {
		switch (xType) {
		case DATETYPE: {
			barNames = dateArraySort(mapkeysGraph(bars));
			break;
		}
		default: barNames = mapkeysGraph(bars);
		}
	}


	//Refreshes the graph
	void refresh() {
		getBarNames();
		ShowWindow(window, SW_HIDE);
		ShowWindow(window, SW_SHOW);
		hdc = GetDC(window);
		drawKey();
		graphXPos = keyXPos + keyLength + 150;
		graphYPos = 0;
		refreshBars();
		drawAxis(graphXPos, graphYPos);
		drawValueLabelsBar(graphXPos, graphYPos);
		ReleaseDC(window, hdc);
	}


};


//A line graph
class LineGraph : public Graph{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
		ar& lines;
		ar& xAxisValuesInt;
		ar& yAxisValuesInt;
		ar& linesSelected;
		ar& title;
		ar& xAxisLabel; 
		ar& yAxisLabel;
		ar& xLength;
		ar& xIntervalActual;
		ar& xIntervalPixel;
		ar& xType;
		ar& yLength;
		ar& yIntervalActual;
		ar& yIntervalPixel;
		ar& yType;
		ar& xAxisValues;
		ar& yAxisValues;
		ar& type;
		ar& lastColour;
		ar& keyXPos;
		ar& keyYPos;
		ar& keyItems;
		ar& keyLength;
		ar& keyHeight;
		ar& lobfSelected;
		}
		public:
			std::map<std::string, GraphLine> lines = {};
			std::vector<double> xAxisValuesInt = {}, yAxisValuesInt = {};
			std::map<std::string, bool> linesSelected = {};
		int lastColour = 0;
		bool lobfSelected = true;
	
		
		//////Line of best fit

		//Draws Line of best fit
		void drawLOBF(GraphLine line) {
			int xPos = graphXPos;
			drawLinePortion(line.lobf.startX + xPos,
				line.lobf.startY,
				line.lobf.endX + xPos,
				line.lobf.endY,
				line.colour,
				PS_DOT);
		}

		//Selects the LOBF for display
		void selectLOBF() {
			lobfSelected = true;
		}

		//Deselects the LOBF from display
		void deselectLOBF() {
			lobfSelected = false;
		}

		//Graph

		//Finds the intervals from the graph values
		void findIntervals() {
			std::vector<double> newXAxisValues = {}, newYAxisValues = {};
			int valueIntervals = 0;
			for (int i = 1; i < xAxisValuesInt.size(); i++) {
				valueIntervals = i;
				xIntervalPixel = (xLength / (xAxisValuesInt.size() / i));
				if (xIntervalPixel != 0) {
					i = xAxisValuesInt.size();
				}
			}
			for (int i = 0; i < xAxisValuesInt.size(); i += valueIntervals){
				newXAxisValues.push_back(xAxisValuesInt[i]);
			}
			xAxisValuesInt = newXAxisValues;
			for (int i = 1; i < yAxisValuesInt.size(); i++) {
				valueIntervals = i;
				yIntervalPixel = (yLength / (yAxisValuesInt.size() / i));
				if (yIntervalPixel != 0) {
					i = yAxisValuesInt.size();
				}
			}
			for (int i = 0; i < yAxisValuesInt.size(); i += valueIntervals){
				newYAxisValues.push_back(yAxisValuesInt[i]);
			}
			yAxisValuesInt = newYAxisValues;

		}
		
		//Updates a line with new values
		bool updateLine(std::string name, std::vector<double> newX, std::vector<double> newY) {
			if (newX != NOCHANGE) {
				lines[name].xValues = newX;
			}
			if (newY != NOCHANGE) {
				lines[name].yValues = newY;
			}
			return true;
		}


		//Removes a line
		bool removeLine(std::string name) {
			lines.erase(name);
			removeKeyItem(name);
			return true;
		}


		//Adds a line
		bool addLine(std::vector<std::vector <double> > coordinates, std::string name, bool selected) {
			coordinates = sort2DArray(coordinates);
			double xIntervalPixelD = xIntervalPixel, yIntervalPixelD = yIntervalPixel,
				xIntervalActualD = xIntervalActual, yIntervalActualD = yIntervalActual;
			if (coordinates[0].size() != coordinates[1].size()) {
				return false;
			}
			GraphLine line;
			line.name = name;
			line.colour = assignColour(lastColour);
			lastColour++;
			for (int i = 0; i < coordinates[0].size(); i++) {
				line.xValues.push_back((coordinates[0][i] - xAxisValuesInt[0]) * (xIntervalPixelD / xIntervalActualD));
				line.yValues.push_back(graphYPos + (yLength - ((coordinates[1][i] - yAxisValuesInt[0]) * (yIntervalPixelD / yIntervalActualD))));
			}
			line.findLOBF();
			lines[name] = line;
			linesSelected[name] = selected;
			addKeyItem(name, line.colour);
			return true;
		}

		//Selects a line for display on the graph
		void selectLine(std::string line) {
			linesSelected[line] = true;
		}

		//Deselects a line for display on the graph
		void deselectLine(std::string line) {
			linesSelected[line] = false;
		}


		//Draws a GraphLine on the graph
		bool drawDataLine(GraphLine line) {
			if (not(line.xValues[0] == 0 || line.yValues[0] == 0)) {
				drawLinePortion(graphXPos, yLength - 0, line.xValues[0] + graphXPos, line.yValues[0], line.colour);

			}
			POINT* points = new POINT[line.xValues.size() - 1];
			for (int point = 1; point < line.xValues.size(); point++) {
				points[point - 1].x = line.xValues[point] + graphXPos;
				points[point - 1].y = line.yValues[point] + graphYPos;
				//drawLineBetweenPoints(point, graphXPos, line);
			}
			HGDIOBJ pen = CreatePen(PS_SOLID, 1, line.colour);
			SelectObject(hdc, pen);
			Polyline(hdc, points, line.xValues.size() - 1);
			DeleteObject(pen);
			delete[] points;
			return true;
		}



		//Draws a line between 2 points
		bool drawLineBetweenPoints(int endPointNum, int xPos, GraphLine line) {
			drawLinePortion(line.xValues[endPointNum - 1] + xPos, line.yValues[endPointNum - 1], line.xValues[endPointNum] + xPos, line.yValues[endPointNum], line.colour);
			return true;
		}


		//Draws the key
		void drawKey() {
			keyPositionSet();
			std::vector<std::string> names = mapkeysColour(keyItems);
			std::vector<COLORREF> colours = mapvaluesColour(keyItems);
			int displayed = 0;
			std::vector<std::string> displayedNames = {};
			if (linesSelected["Total"]) {
				displayed++;
				displayedNames.push_back("Total");
				addGraphText("Total", keyXPos + 50, (keyYPos + 40), GRAPH_COLOUR);
				drawFilledSquare(COLOUR_BLACK, keyXPos + 10, keyYPos + 40, 10, 10);
			}
			for (int i = 0; i < names.size(); i++) {
				if (names[i] != "Total") {
					if (linesSelected[names[i]]) {
						addGraphText(names[i], keyXPos + 50, (keyYPos + (30 * (displayed)) + 40), GRAPH_COLOUR);
						drawFilledSquare(colours[i], keyXPos + 10, (keyYPos + (30 * (displayed)) + 40), 10, 10);
						displayed++;
						displayedNames.push_back(names[i]);
					}
				}
			}
			if (displayed > 0) {
				keySizeCalc(displayedNames);
				addGraphText("KEY", keyXPos + (keyLength / 2) - 10, keyYPos, GRAPH_COLOUR);
				drawRectangle(keyXPos, keyYPos, keyLength, keyHeight);
			}
			else {
				addGraphText("No Items Selected", keyXPos, keyYPos + 10, GRAPH_COLOUR);
				keyLength = 150;
			}
		
		}

		//Refreshes the lines
		void refreshLines() {
			for (std::string line : mapkeysGraph(lines)) {
				if (linesSelected[line]) {
					drawDataLine(lines[line]);
					if (lobfSelected) {
						drawLOBF(lines[line]);
					}
				}
			}
		}

		//Refreshes the graph
		void refresh(){
			ShowWindow(window, SW_HIDE);
			ShowWindow(window, SW_SHOW);
			hdc = GetDC(window);
			drawKey();
			graphXPos = keyXPos + keyLength + 150;
			graphYPos = 0;
			refreshLines();
			drawAxis(graphXPos, graphYPos);
			ReleaseDC(window, hdc);
		}
};


//Creates a new linegraph
LineGraph CreateLineGraph(HWND window,
	int length, int height,
	std::vector<double> xAxisValues,
	std::vector<double> yAxisValues,
	int xType, int yType,
	std::string xAxisLabel,
	std::string yAxisLabel,
	std::string title,
	int xDist,
	int yDist,
	std::string firstValue = ""
	) {


	LineGraph graphSkeleton;
	
	graphSkeleton.type = LINE_GRAPH;
	graphSkeleton.window = window;
	graphSkeleton.xAxisValuesInt = xAxisValues;
	graphSkeleton.yAxisValuesInt = yAxisValues;
	graphSkeleton.title = title;
	graphSkeleton.xAxisLabel = xAxisLabel;
	graphSkeleton.yAxisLabel = yAxisLabel;
	graphSkeleton.xLength = length;
	graphSkeleton.yLength = height;
	graphSkeleton.xType = xType;
	graphSkeleton.yType = yType;
	graphSkeleton.xIntervalActual = graphSkeleton.xAxisValuesInt[1] - graphSkeleton.xAxisValuesInt[0];
	graphSkeleton.yIntervalActual = graphSkeleton.yAxisValuesInt[1] - graphSkeleton.yAxisValuesInt[0];
	graphSkeleton.xDist = xDist;
	graphSkeleton.yDist = yDist;
	graphSkeleton.findIntervals();
	switch (xType) {
	case INTTYPE: {
		for (int i = 0; i < graphSkeleton.xAxisValuesInt.size(); i++) {
			graphSkeleton.xAxisValues.push_back(to_string((int)graphSkeleton.xAxisValuesInt[i]));
		}
		break;
	}
	case TIMETYPE: {
		for (int i = 0; i < graphSkeleton.xAxisValuesInt.size(); i++) {
			graphSkeleton.xAxisValues.push_back(timeStamp(graphSkeleton.xAxisValuesInt[i]));
		}
		break;
	}
	case MONTHTYPE: {
		graphSkeleton.xAxisValues = getDaysMonths(firstValue, xAxisValues);
		break;
	}
	}


	switch (yType) {
	case INTTYPE: {
		for (int i = 0; i < graphSkeleton.yAxisValuesInt.size(); i++) {
			graphSkeleton.yAxisValues.push_back(to_string((int)graphSkeleton.yAxisValuesInt[i]));
		}
		break;
	}
	case TIMETYPE: {
		for (int i = 0; i < graphSkeleton.yAxisValuesInt.size(); i++) {
			graphSkeleton.yAxisValues.push_back(timeStamp(graphSkeleton.yAxisValuesInt[i]));
		}
		break;
	}
	}
	return graphSkeleton;
}


//Creates a bar graph
BarGraph CreateBarGraph(HWND window, int xType,
	int length, int height,
	std::vector<double> yAxisValues,
	std::string xAxisLabel,
	std::string yAxisLabel,
	std::string title,
	int xDist,
	int yDist,
	int barWidth,
	int barGap) {


	BarGraph graphSkeleton;

	graphSkeleton.type = BAR_GRAPH;
	graphSkeleton.window = window;
	for (double value : yAxisValues) {
		graphSkeleton.yAxisValuesInt.push_back((int)value);
	}
	graphSkeleton.width = barWidth;
	graphSkeleton.xIntervalPixel = barGap;
	graphSkeleton.title = title;
	graphSkeleton.xAxisLabel = xAxisLabel;
	graphSkeleton.yAxisLabel = yAxisLabel;
	graphSkeleton.xLength = length;
	graphSkeleton.yLength = height;
	graphSkeleton.xType = STRINGTYPE;
	graphSkeleton.yType = INTTYPE;
	graphSkeleton.yIntervalActual = graphSkeleton.yAxisValuesInt[1] - graphSkeleton.yAxisValuesInt[0];
	graphSkeleton.xDist = xDist;
	graphSkeleton.yDist = yDist;
	graphSkeleton.findIntervals();
	graphSkeleton.xType = xType;
	return graphSkeleton;
}


