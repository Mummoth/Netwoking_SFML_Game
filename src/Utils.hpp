#pragma once
#include <iostream>

#define ASSETS_PATH L"assets/"
#define TXT_PATH L"assets/textures/"
#define FONT_PATH L"assets/fonts/"

namespace Game
{
enum MessageType : uint8_t { INFO, DEBUG, WARNING, ERROR, SUCCESS };

enum SourceType : uint8_t { SERVER, CLIENT };

class Utils
{
public:
	/// @brief Prints a timestamped message to the console with colour coding.
	///
	/// This function outputs a message prefixed by the current local time,
	/// formatted as "DD-MM-YYYY HH:MM:SS". The message is colour-coded
	/// according to its severity level:
	/// - DEBUG → Cyan
	/// - WARNING → Brown
	/// - ERROR → Red (bold)
	/// - SUCCESS → Green (bold)
	/// - INFO (default) → Plain Text
	///
	/// @param msg  The message string to print.
	/// @param mType The message category/severity. Defaults to INFO.
	/// @param sType The source of the message. Defaults to CLIENT.
	/// @param terminateOnError A flag for whether to terminate the application
	/// if an error occurs. Defaults to FALSE.
	static void PrintMsg(const std::string& msg, const MessageType mType = INFO,
						 const SourceType sType = CLIENT,
						 bool terminateOnError = false)
	{
		const auto now = std::chrono::system_clock::now();
		const std::time_t currentTime =
				std::chrono::system_clock::to_time_t(now);
		std::tm localTime = {};
		// Cast to VOID to ignore return.
		static_cast<void>(localtime_s(&localTime, &currentTime));

		std::ostringstream timestamp;
		timestamp << std::put_time(&localTime, "%d-%m-%Y %H:%M:%S");

		// Source header.
		std::string sourceText;
		switch (sType)
		{
		case SERVER: sourceText = "Server";
			break;
		case CLIENT: sourceText = "Client";
			break;
		}

		// Severity colour.
		std::string severityColour;
		switch (mType)
		{
		case DEBUG: severityColour = "\033[36m";
			break;
		case WARNING: severityColour = "\033[33m";
			break;
		case ERROR: severityColour = "\033[1;31m";
			break;
		case SUCCESS: severityColour = "\033[1;32m";
			break;
		case INFO: severityColour = "\033[0m";
			break;
		}

		const std::string gold = "\033[38;5;220m";
		const std::string reset = "\033[0m";

		// Print message.
		std::cout << "[" << gold << std::setw(6) << std::left << sourceText
				<< reset << "]  "
				<< severityColour
				<< std::setw(20) << std::left << timestamp.str() << "    "
				<< msg << reset << '\n';

		// If the terminate flag is set, and it's an ERROR, stop the program.
		if (terminateOnError && mType == ERROR)
			throw std::runtime_error(msg);
	}
};
}
