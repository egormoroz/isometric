#pragma once

#include <string>

static std::string extract_local_folder(const std::string & path)
{
	for (std::int64_t i = path.length() - 1; i >= 0; --i)
		if (path[static_cast<size_t>(i)] == '/' || path[static_cast<size_t>(i)] == '\\') {
			return path.substr(0, static_cast<size_t>(i + 1));
		}
	return std::string();
}

static std::string id_by_path(std::string path) {
	for (size_t i = 0; i < path.length(); ++i)
		path[i] = path[i] == '\\' ? '/' : tolower(path[i]);

	for (size_t i = path.find("/../"); i < path.length();) {
		size_t j = path.rfind('/', i - 1);
		if (j == std::string::npos)
			break;
		path.erase(j, i + 2 - j);
		i = path.find("/../", j);
	}

	return path;
}
