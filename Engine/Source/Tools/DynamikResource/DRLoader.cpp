#include "dmkafx.h"
#include "DRLoader.h"

#include "Utilities/Functions/DMK_Functions.h"

#define IS_SPACE(chr) (chr == ' ')
#define IS_COMMAND(chr)	(chr == '>')
#define IS_COMMENT(chr) (chr == '#')

namespace Dynamik {

	std::vector<CCPTR> attributes = {
		"position",
		"color",
		"normal",
		"texture",
	};

	DResFile DMKResLoader::loadFile(CCPTR path)
	{
		std::ifstream _file(path);

		if (!_file.is_open())
			DMK_CORE_FATAL("Unable to read Dynamik Resource file!");

		std::string _strLine;
		while (std::getline(_file, _strLine))
		{
			if (_strLine.size() < 2)
				continue;

			for (UI32 index = 0; index < _strLine.size(); index++)
				_strLine[index] = std::tolower(_strLine[index]);

			CCPTR _line = _strLine.c_str();
			if(IS_COMMENT(_line[0]))
				continue;


			if (IS_COMMAND(_line[0]))
			{

			}
		}

		return DResFile();
	}
}