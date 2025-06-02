// RegexChain.hpp
#pragma once

#include <string>
#include <regex>

class RegexChain {
    std::string _value;

public:
    explicit RegexChain(std::string s) : _value(std::move(s)) {}

    RegexChain& replace(const std::string& pattern, char ch) {
        _value = std::regex_replace(_value, std::regex(pattern), std::string(1, ch));
        return *this;
    }

    RegexChain& replace(const std::string& pattern, const std::string& replacement) {
        _value = std::regex_replace(_value, std::regex(pattern), replacement);
        return *this;
    }

    const std::string& str() const { return _value; }

    operator std::string() const { return _value; }
};


/// Usage
/** std::string s = RegexChain(std::move(s))
        .replace("\\NUL\\", 0x00)
        .replace("\\SOH\\", 0x01)
        .replace("\\STX\\", 0x02)
        .replace("\\ETX\\", 0x03)
        // ...
        .replace("\\DEL\\", 0x7F);
        // ...
        #include "RegexChain.hpp"

		std::string s = "Hello\\NUL\\World\\DEL\\";
		s = RegexChain(std::move(s))
        .replace("\\NUL\\", 0x00)
        .replace("\\DEL\\", 0x7F);
        */

