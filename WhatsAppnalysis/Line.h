#pragma once
#include <string>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;
#define WHATSAPP_FILE 0
#define VIBER_FILE 1
//The Line class
class Line {

public:
	std::string msgstreamline;
	int fileType;
	Line(std::string, int);

	//Finds the date (DD/MM/YYYY)
	std::string datefind() {
		switch (fileType) {
		case WHATSAPP_FILE: {
			return msgstreamline.substr(0, 10);
		}
		case VIBER_FILE: {
			return msgstreamline.substr(0, 10);
		}
		default: {
			return NULL;
		}
		}
	}

	//Finds the time (HH:MM)
	std::string timefind() {
		switch (fileType) {
		case WHATSAPP_FILE: {
			return msgstreamline.substr(12, 5);
		}
		case VIBER_FILE: {
			return msgstreamline.substr(12, 5);
		}
		default: {
			return NULL;
		}
		}
	}

	//Finds the month (MM/YYYY)
	std::string monthfind() {
		switch (fileType) {
		case WHATSAPP_FILE: {
			return msgstreamline.substr(3, 7);
		}
		case VIBER_FILE: {
			return msgstreamline.substr(3, 7);
		}
		default: {
			return NULL;
		}
		}
	}

	//Finds the person
	std::string personfind() {
		switch (fileType) {
		case WHATSAPP_FILE: {
			int personendchar;
			personendchar = persongapfind();
			if (personendchar == -1) {
				return "error";
			}
			else {
				return msgstreamline.substr(20, (unsigned __int64)personendchar - (unsigned __int64)20);
			}
		}
		case VIBER_FILE: {
			int personendchar;
			personendchar = persongapfind();
			if (personendchar == -1) {
				return "error";
			}
			else {
				return msgstreamline.substr(20, (unsigned __int64)personendchar - (unsigned __int64)20);
			}
		}
		default: {
			return NULL;
		}
		}
	}

	//Finds the message
	std::string messagefind() {
		switch (fileType) {
		case WHATSAPP_FILE: {
			int personendchar;
			personendchar = persongapfind();
			if (personendchar == -1) {
				return "error";
			}
			else {
				return msgstreamline.substr((unsigned __int64)personendchar + 2, msgstreamline.size());
			}
		}
		case VIBER_FILE: {
			int personendchar;
			personendchar = persongapfind();
			if (personendchar == -1) {
				return "error";
			}
			else {
				return msgstreamline.substr((unsigned __int64)personendchar + 2, msgstreamline.size());
			}
		}
		default: {
			return NULL;
		}
		}
	}

	//Finds the spaces between names in a person's fullname
	int persongapfind() {
		switch (fileType) {
		case WHATSAPP_FILE: {
			std::string name;
			int charobserving;
			int spacenumber = 0;
			for (charobserving = 20; charobserving <= msgstreamline.size(); charobserving++) {
				char colon = ':';
				char space = ' ';
				if (msgstreamline[charobserving] == colon) {
					return charobserving;
				}
			}
		}
		case VIBER_FILE: {
			std::string name;
			int charobserving;
			int spacenumber = 0;
			for (charobserving = 20; charobserving <= msgstreamline.size(); charobserving++) {
				char colon = ':';
				char space = ' ';
				if (msgstreamline[charobserving] == colon) {
					return charobserving;
				}
			}
		}
		default: {
			return NULL;
		}
		}
	}
};

//INIT statement for Line
Line::Line(std::string msgstreamlinecurrent, int fileTypeIn) {
	msgstreamline = msgstreamlinecurrent;
	fileType = fileTypeIn;
};
