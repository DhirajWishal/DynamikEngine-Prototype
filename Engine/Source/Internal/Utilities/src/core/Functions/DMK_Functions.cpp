/*
 Stand alone function library for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 DMK_Functions.cpp file.
*/

#include "DMK_Functions.h"
#include <codecvt>

namespace Dynamik {
	namespace utils {
		/* ---------- MIN FUNCTIONS ---------- */
		// var = utils::min(first, secons);
		template<class Alloc>
		Alloc min(Alloc first, Alloc second)
		{
			if (first < second)
				return (Alloc)first;
			else
				return (Alloc)second;
		}

		// var = utils::min(first, secons);
		int min(int first, int second)
		{
			if (first < second)
				return (first);
			else
				return (second);
		}

		// var = utils::min(first, secons);
		UI32 min(UI32 first, UI32 second)
		{
			if (first < second)
				return (first);
			else
				return (second);
		}

		// var = utils::min(first, secons);
		float min(float first, float second)
		{
			if (first < second)
				return (first);
			else
				return (second);
		}

		/* ---------- MAX FUNCTIONS ---------- */
		// var = utils::max(first, secons);
		template<class Alloc>
		Alloc max(Alloc first, Alloc second)
		{
			if (first > second)
				return (Alloc)first;
			else
				return (Alloc)second;
		}

		// var = utils::max(first, secons);
		int max(int first, int second)
		{
			if (first > second)
				return first;
			else
				return second;
		}

		// var = utils::max(first, secons);
		UI32 max(UI32 first, UI32 second)
		{
			if (first > second)
				return (first);
			else
				return (second);
		}

		// var = utils::max(first, secons);
		float max(float first, float second)
		{
			if (first > second)
				return (first);
			else
				return (second);
		}

		/* ---------- SWAP FUNCTIONS ---------- */
		// var = utils::swap(&first, &secons);
		template<class Alloc>
		void swap(Alloc* first, Alloc* second)
		{
			Alloc* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(int* first, int* second)
		{
			int* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(UI32* first, UI32* second)
		{
			UI32* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(float* first, float* second)
		{
			float* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(char* first, char* second)
		{
			char* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(CCPTR* first, CCPTR* second)
		{
			CCPTR* temp = first;
			first = second;
			second = temp;
		}

		// var = utils::swap(&first, &secons);
		void swap(std::string* first, std::string* second)
		{
			std::string* temp = first;
			first = second;
			second = temp;
		}

		/* ---------- FACTORIAL FUNCTIONS ---------- */
		// var = utils::factorial(value);
		long double factorial(long double value)
		{
			if (value == 0 || value == 1) return 1;
			if (value == 2) return value;

			long double _result = 1;
			for (long double _counter = 1; _counter <= value; _counter++)
				_result *= _counter;

			return _result;
		}

		// var = utils::power(value);
		long double power(double base, double power)
		{
			if (power == 0) return 1;
			if (power == 1) return base;
			if (power == 2) return base * base;

			long double result = base;
			for (long int itr = 1; itr < power; itr++)
				result *= base;

			return result;
		}

		// var = utils::square(value);
		long double square(double base)
		{
			return power(base, 2);
		}

		// var = utils::splitLine(line, character);
		std::vector<std::string> splitLine(std::string_view line, char splitBy)
		{
			if (line.find(splitBy) != std::string::npos)
			{
				std::vector<std::string> _splits = {};
				std::string _split = DMK_TEXT("");
				uint32_t _lineSize = line.size();
				uint32_t _index = 0;

				while (_index < _lineSize)
				{
					if ((line[_index] == splitBy))
					{
						_splits.push_back(_split);
						_split.clear();
						_index++;
						continue;
					}
					if (_index == (_lineSize - 1))
					{
						_split.push_back(line[_index]);
						_splits.push_back(_split);
						_index++;
						break;
					}

					_split.push_back(line[_index]);
					_index++;
				}

				return _splits;
			}

			return std::vector<std::string>();
		}

		// var = rand();
		uint32_t rand(uint32_t from, uint32_t to)
		{
			return std::rand();
		}

		std::vector<UI32> bubbleSort(std::vector<UI32> data)
		{
			UI32 _indexCount = 0;
			UI32 _index, _itr;
			UI32 _size = data.size();
			while (_indexCount < _size - 1) {
				_index = 0;
				_itr = 1;
				while (_itr < _size - _indexCount) {
					if (data[_index] > data[_itr])
						std::swap(data[_index], data[_itr]);
					_index++;
					_itr++;
				}
				_indexCount++;
			}
			return data;
		}
	}

	std::string WSTRtoCSTR(std::wstring string)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(string);
	}

	std::wstring CSTRtoWSTR(std::string string)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(string);
	}
}