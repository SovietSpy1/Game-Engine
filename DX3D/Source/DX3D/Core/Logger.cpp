#include <DX3D/Core/Logger.h>
#include <iostream>
dx3d::Logger::Logger(LogLevel logLevel) : m_logLevel{ logLevel }
{
	std::clog << "C++ Graphics Simulator" << "\n";
	std::clog << "--------------------------------------" << "\n";
}

void dx3d::Logger::log(LogLevel level, const char* message) const
{
	auto LogLevelToString = [](LogLevel level) {
		switch (level) {
		case LogLevel::Error: return "Error";
		case LogLevel::Warning: return "Warning";
		case LogLevel::Info: return "Info";
		default: return "Unknown";
		}
	};
	if (level > m_logLevel) return;
	std::clog << "[DX3D " << LogLevelToString(level) << "]:" << message << "\n";
}
