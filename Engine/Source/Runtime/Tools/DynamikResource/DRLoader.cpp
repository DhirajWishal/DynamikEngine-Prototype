#include "dmkafx.h"
#include "DRLoader.h"

#include "Utilities/Functions/DMK_Functions.h"

#define IS_SPACE(chr) (chr == ' ')
#define IS_COMMAND(chr)	(chr == '>')
#define IS_COMMENT(chr) (chr == '#')

namespace Dynamik {
	DResFile DMKResLoader::loadFile(CCPTR path)
	{
		std::ifstream _file(path);

		std::string _line;
		while (std::getline(_file, _line))
		{
			if(IS_COMMENT(_line[0]))
				continue;
			if (_line.size() < 2)
				continue;

			if (IS_COMMAND(_line[0]))
			{

			}
		}
	}
}