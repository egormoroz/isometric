#include "base64.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

using namespace boost::archive::iterators;
namespace bio = boost::iostreams;
using base64_enc = base64_from_binary<transform_width<const char*, 6, 8>>;
using base64_dec = transform_width<binary_from_base64<const char *>, 8, 6>;

std::string base64::encode(const std::string &data, bool compress)
{
	if (compress)
	{
		std::stringstream original(data);
		std::stringstream compressed;
		bio::filtering_streambuf<bio::input> out;
		out.push(bio::zlib_compressor());
		out.push(original);
		bio::copy(out, compressed);

		std::string bytes = compressed.str();
		compressed.clear();
		compressed.str("");

		std::copy(base64_enc(bytes.c_str()), base64_enc(bytes.c_str() + bytes.length()),
				  std::ostream_iterator<char>(compressed));
		return compressed.str();
	}
	else
	{
		std::stringstream ss;
		std::copy(base64_enc(data.c_str()), base64_enc(data.c_str() + data.length()),
				  std::ostream_iterator<char>(ss));
		return ss.str();
	}
}

std::string base64::decode(std::string data, bool compressed)
{
	boost::trim(data);
	std::stringstream ss;
	size_t len = data.length();
	if (len && data[len - 1] == '=')
	{
		--len;
		if (len && data[len - 1] == '=')
			--len;
	}
	if (!len) return std::string();

	std::copy(base64_dec(data.c_str()), base64_dec(data.c_str() + len),
			  std::ostream_iterator<char>(ss));

	if (compressed)
	{
		std::stringstream decompressed;
		bio::filtering_streambuf<bio::input> in;
		in.push(bio::zlib_decompressor());
		in.push(ss);
		bio::copy(in, decompressed);

		return decompressed.str();
	}
	return ss.str();
}