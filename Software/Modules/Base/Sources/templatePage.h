#ifndef TEMPLATE_PAGE_H
#define TEMPLATE_PAGE_H

#include <string>

struct TemplatePage{
	static const std::string PAGE;
	static const std::string MAIN_BLOCK_MARKER;
	static const std::string getPage(std::string& mainBlock);
};
#endif
