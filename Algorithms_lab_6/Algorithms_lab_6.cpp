#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> prefix_Function(std::string s) { //prefix-function

    size_t len = s.length();

    std::vector<size_t> prefs(len);

    for (size_t i = 1; i < len; ++i) {

        size_t j = prefs[i - 1];

        while ((j > 0) && (s[i] != s[j]))
            j = prefs[j - 1];

        if (s[i] == s[j])
            ++j;

        prefs[i] = j;
    }
    return prefs;
}

bool Contains(std::string str, std::string substr) { //KMP
    auto prefixes = prefix_Function(substr + "@" + str);

    for (int pref : prefixes)
        if (pref == substr.length())
            return true;

    return false;
}

bool is_Cyclic_Shifted(std::string str, std::string substr) { //general function
    if (str.length() != substr.length())
        return false;

    if (Contains(str + str, substr))
        return true;
    else
        return false;
}

int main()
{
    std::string str, substr;

    std::cout << "Enter string: ";
    std::cin >> str;

    std::cout << "Enter to_check cycled shift: ";
    std::cin >> substr;

    std::cout << "Is second string a cycled shift of the first one: ";

    if (is_Cyclic_Shifted(str, substr))
        std::cout << "Yes\n";
    else
        std::cout << "No\n";

    return 0;
}
