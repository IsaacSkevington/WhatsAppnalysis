#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>
#include "Version.h"
#include <string>

using namespace std;

class TextBox {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& xPos;
		ar& yPos;
		ar& xSize;
		ar& ySize;
		ar& text;
		ar& ID;
	}
public:
	int xPos;
	int yPos;
	int xSize;
	int ySize;
	int ID;
	std::string text;
};

class TextBoxList {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& textBoxes;
		ar& IDsUsed;
		ar& IDsUnused;
	}
public:
	std::map<int, TextBox> textBoxes = {};
	std::map<int, bool> isIDUsed = {};
	std::vector<int> IDsUnused = populateIDs();

	std::vector<int> populateIDs() {
		std::vector<int> ids = {};
		for (int i = 0xF000; i < 0xFFFF; i++) {
			ids.push_back(i);
			isIDUsed[i] = false;
		}
	}

};


