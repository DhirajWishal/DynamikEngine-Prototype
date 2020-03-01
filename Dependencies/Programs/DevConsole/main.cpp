#include "dcnsafx.h"
#include "src/core/console.h"

#ifdef CMD

int main(int argc, char* argv[]) {
	std::wstring input;
	std::vector<std::wstring> inputs;
	std::wstringstream ss;

	for (int i = 0; i < argc; i++)
		if (i != 0)
			inputs.push_back(argv[i]);

	for (auto i : inputs)
		ss << i << " ";

	input = ss.str();
	input.pop_back();

	Dynamik::console::Console myConsole;
	myConsole.write(input);

	return 0;
}

// ----------
#else

int main() {
	std::wstring input;

	while (true) {
		std::getline(std::cin, input, '\n');

		Dynamik::console::Console myConsole;
		myConsole.write(input);

		if (input == L"TERMINATE")
			break;
	}
	return 0;
}

// ----------
#endif