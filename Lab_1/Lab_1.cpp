#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <set>

struct WorldMap {
	std::string Country;
	std::string City;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	os << '(';

	for (auto i = v.begin(); i != v.end(); ++i)
		os << ' ' << *i;

	os << " )";
	return os;
}

std::ostream& operator<<(std::ostream& os, const WorldMap& v) {
	os << '{';

	if (v.City != "") {
		os << " Country: " << v.Country << ", City: " << v.City;
	}

	os << " }";
	return os;
}

struct Parameters {
	Parameters(int a = 0, int b = 0, int p = 0, int m = 0)
		:a(a),
		b(b),
		p(p),
		m(m) {}

	int a, b, p, m;
};

std::ostream& operator<<(std::ostream& os, const Parameters& params) {
	os << '[';
	if (params.p != 0)
		os << " a = " << params.a << ", b = " << params.b << ", p = " << params.p << ", m = " << params.m;
	os << " ]";
	return os;
}

int universalHashFunction(const int& value, const Parameters& params);
int universalHashForStrings(const std::string& inputString, const Parameters& params);

class perfectHash {
public:
	template <typename Iterator>
	perfectHash(Iterator begin, Iterator end, const Parameters& forMainHashFunction)
		:forMainHashFunction(forMainHashFunction),
		data(forMainHashFunction.m) {
		std::vector<WorldMap> keys(begin, end);

		auto groups = outerHashing(keys);
		innerHashing(groups);
	}

	auto getData() {
		return data;
	}

	std::string operator[](const std::string& key) {
		std::string failure = "Doesn't exist";

		int groupIndex = universalHashForStrings(key, forMainHashFunction);

		if (groupIndex < 0 || groupIndex >= data.size() || data[groupIndex].second.size() == 0) {
			return failure;
		}

		Parameters groupParams = data[groupIndex].first;

		int itemIndex = universalHashForStrings(key, groupParams);

		if (itemIndex < 0 || itemIndex >= data[groupIndex].second.size()) {
			return failure;
		}

		if (data[groupIndex].second[itemIndex].City == "" || data[groupIndex].second[itemIndex].City != key)
			return failure;

		return data[groupIndex].second[itemIndex].Country;
	}

private:
	Parameters forMainHashFunction;
	std::vector<std::pair<Parameters, std::vector<WorldMap>>> data;

	auto outerHashing(const std::vector<WorldMap>& keys) {
		std::map<int, std::vector<WorldMap>> result;

		for (const auto& key : keys)
			result[universalHashForStrings(key.City, forMainHashFunction)].push_back(key);

		return result;
	}

	void innerHashing(const std::map<int, std::vector<WorldMap>>& groups) {
		for (const auto& [id, values] : groups) {
			Parameters currentParameters = defineParameters(values);
			std::vector<WorldMap> currentVector(static_cast<int>(std::pow(values.size(), 2)), WorldMap{ "" , "" });

			for (const auto& value : values) {
				currentVector[universalHashForStrings(value.City, currentParameters)] = value;
			}

			data[id] = { currentParameters, currentVector };
		}
	}

	Parameters defineParameters(const std::vector<WorldMap>& keys) {
		Parameters currentParams = forMainHashFunction;
		currentParams.m = static_cast<int>(std::pow(keys.size(), 2));

		std::set<int> values;

		for (int i = 0, p = currentParams.p; i < p; i++) {
			for (int j = 1; j < p; j++) {
				currentParams.a = i, currentParams.b = j;
				bool isCorrect = true;
				for (const auto& item : keys) {
					if (!(isCorrect = values.insert(universalHashForStrings(item.City, currentParams)).second))
						break;
				}
				if (!isCorrect) {
					values.clear();
					continue;
				}
				else {
					return currentParams;
				}
			}
		}
	}

	void search(const std::string input) {
		int vectorPlace = universalHashForStrings(input, forMainHashFunction);
	}
};

std::ostream& operator<<(std::ostream& os, const std::vector<std::pair<Parameters, std::vector<WorldMap>>>& v) {
	bool isFirst = true;
	for (auto i = v.begin(), end = v.end(); i != end; i++) {
		if (isFirst)
			isFirst = false;
		else
			std::cout << std::endl;

		os << i->first;
		os << " ===== " << i->second;
	}

	return os;
}

int universalHashForStrings(const std::string& inputString, const Parameters& params) {
	int hash = 0;
	int power = 1;

	for (const char& it : inputString) {
		hash = (hash * params.a + params.b + (it - 'a' + 1) * power) % params.m;
		power = (power * params.p) % params.m;
	}

	return hash;
}

int universalHashFunction(const int& value, const Parameters& params) {
	return ((params.a * value + params.b) % params.p) % params.m;
}

int main() {
	Parameters initialParams(3, 42, 101, 9);

	//std::set<int> keys{ 10,22,37,40,60,70,75 };

	std::vector<WorldMap> keys{ {"Italy", "Rome"},
							{"Spain","Madrid"},
							{"Ukraine", "Kyiv"},
							{"Italy", "Firence"},
							{"Spain", "Barcelona"},
							{"Russia", "Moscow"},
							{"Italy", "Neapol"},
	};

	perfectHash map(keys.begin(), keys.end(), initialParams);

	int n = 0;
	std::cout << "Enter n: ";
	std::cin >> n;


	for (int i = 0; i < n; ++i) {
		system("cls");

		std::string cityToFind;
		std::cin >> cityToFind;

		std::cout << map[cityToFind] << std::endl;

		system("pause >> void");
	}
}