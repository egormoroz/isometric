#ifndef BASE64_HPP
#define BASE64_HPP

#include <string>

namespace base64
{
	std::string encode(const std::string &data, bool compress = true);
	std::string decode(std::string data, bool compressed = true);
}

#endif
