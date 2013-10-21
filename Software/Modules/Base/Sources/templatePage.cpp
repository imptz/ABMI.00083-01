#include "templatePage.h"
#include "stringTools.h"

const std::string TemplatePage::MAIN_BLOCK_MARKER = std::string("<!--MAIN-->");

const std::string TemplatePage::PAGE = std::string("\
<!DOCTYPE html>\n\
<html lang=\"ru\">\n\
<head>\n\
	<meta charset=\"UTF-8\"/>\n\
	<title>Uso</title>\n\
</head>\n\
<body>\n\
	<div>\n" + 
	MAIN_BLOCK_MARKER + 
"	</div>\n\
</body>\n\
</html>");

const std::string TemplatePage::getPage(std::string& mainBlock){
	std::string page(PAGE);
	StringTools::replaceMarker(const_cast<std::string&>(page), mainBlock, MAIN_BLOCK_MARKER);

	return page;
}
