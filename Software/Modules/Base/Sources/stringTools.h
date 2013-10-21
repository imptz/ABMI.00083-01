#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

class StringTools{
public:
	static std::string& replaceMarker(std::string& source, std::string& replaceString, const std::string& marker){
		std::string::size_type markerPos = source.find(marker);

		if (markerPos != std::string::npos){
			source.replace(markerPos, marker.length(), replaceString);
		}

		return source;
	}
};

#endif