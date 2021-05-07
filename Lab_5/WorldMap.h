#pragma once
#include <iostream>
#include <exception>

struct WorldMap {
	std::string country;
	std::string city;

	bool operator <(const WorldMap& rhs) const {
		return this->city < rhs.city;
	}

	bool operator >(const WorldMap& rhs)const {
		return this->city > rhs.city;
	}

	bool operator ==(const WorldMap& rhs)const {
		return this->city == rhs.city;
	}

	bool operator !=(const WorldMap& rhs)const {
		return this->city != rhs.city;
	}

	bool operator <=(const WorldMap& rhs) const {
		return (this->city < rhs.city) || (this->city == rhs.city);
	}

	bool operator >=(const WorldMap& rhs) const {
		return (this->city > rhs.city) || (this->city == rhs.city);
	}
};

std::ostream& operator<<(std::ostream& os, const WorldMap& v) {
	os << '(';

	if (v.city != "") {
		//os << " Country: " << v.country << "; City: " << v.city;
		os << v.city;
	}

	os << " )";
	return os;
}
