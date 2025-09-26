#pragma once
#include <iostream>

#define ASSETS_PATH L"assets/"
#define TXT_PATH L"assets/textures/"
#define FONT_PATH L"assets/fonts/"

namespace Game
{
enum MessageType : uint8_t { INFO, DEBUG, WARNING, ERROR, SUCCESS };

class Utils
{
public:
	/// @brief Prints a timestamped message to the console with colour coding.
	///
	/// This function outputs a message prefixed by the current local time,
	/// formatted as "DD-MM-YYYY HH:MM:SS". The message is colour-coded
	/// according to its severity level:
	/// - DEBUG → cyan
	/// - WARNING → yellow
	/// - ERROR → red (bold)
	/// - SUCCESS → green (bold)
	/// - INFO (default) → plain text
	///
	/// @param msg  The message string to print.
	/// @param type The message category/severity. Defaults to INFO.
	static void PrintMsg(const std::string& msg, const MessageType type = INFO)
	{
		const auto now = std::chrono::system_clock::now();
		const std::time_t currentTime =
				std::chrono::system_clock::to_time_t(now);
		std::tm localTime = {};
		static_cast<void>(localtime_s(&localTime, &currentTime));
		//< Cast to VOID to ignore return.
		const auto timestamp = std::put_time(&localTime, "%d-%m-%Y %H:%M:%S");

		switch (type)
		{
		case DEBUG: std::cout << "\033[36m" << timestamp << '\t' << msg <<
					"\033[0m\n";
			break;
		case WARNING: std::cout << "\033[33m" << timestamp << '\t' << msg <<
					  "\033[0m\n";
			break;
		case ERROR: std::cout << "\033[1;31m" << timestamp << '\t' << msg <<
					"\033[0m\n";
			break;
		case SUCCESS: std::cout << "\033[1;32m" << timestamp << '\t' << msg <<
					  "\033[0m\n";
			break;
		default: std::cout << timestamp << '\t' << msg << '\n';
			break;
		}
	}
};
}
