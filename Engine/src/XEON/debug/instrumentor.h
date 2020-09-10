#pragma once

#include "XEON/core/core.h"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <mutex>
#include <thread>

namespace XEON {

	struct ProfileResult {
		std::string name;
		long long start, end;
		uint64_t threadID;
	};

	class Instrumentor {
	public:
		~Instrumentor() { endSession(); }

		void beginSession(const std::string& name, const std::string& filepath = "results.json") {
			outputStream.open(filepath);
			writeHeader();
			currentSession = name;
		}

		void endSession() {
			writeFooter();
			outputStream.close();
			profileCount = 0;
		}

		void writeProfile(const ProfileResult& result) {
			std::lock_guard<std::mutex> lock(lock);

			if (profileCount++ > 0)
				outputStream << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			outputStream << "{";
			outputStream << "\"cat\":\"function\",";
			outputStream << "\"dur\":" << (result.end - result.start) << ',';
			outputStream << "\"name\":\"" << name << "\",";
			outputStream << "\"ph\":\"X\",";
			outputStream << "\"pid\":0,";
			outputStream << "\"tid\":" << result.threadID << ",";
			outputStream << "\"ts\":" << result.start;
			outputStream << "}";

			//outputStream.flush();
		}

		void writeHeader() {
			outputStream << "{\"otherData\": {},\"traceEvents\":[";
			//outputStream.flush();
		}

		void writeFooter() {
			outputStream << "]}";
			//outputStream.flush();
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}

	private:
		std::string currentSession = "";
		std::ofstream outputStream;
		int profileCount = 0;
		std::mutex lock;

	};

	class InstrumentationTimer {
	public:
		explicit InstrumentationTimer(const char* name)
			: startTimePoint(std::chrono::high_resolution_clock::now()), result({name, 0, 0, 0}) {}

		~InstrumentationTimer() {
			if (!stopped) stop();
		}

		void stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();
		
			result.start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
			result.end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
			result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

			Instrumentor::Get().writeProfile(result);
			stopped = true;
		}
	private:
		bool stopped = false;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
		ProfileResult result;
	
	};
}
#if XEON_PROFILER
	#define XEON_PROFILE_BEGIN_SESSION(name, filepath) XEON::Instrumentor::Get().beginSession(name, filepath)
	#define XEON_PROFILE_SCOPE(name) XEON::InstrumentationTimer timer##__LINE__(name);
	#define XEON_PROFILE_FN() XEON_PROFILE_SCOPE(__FUNCSIG__)
	#define XEON_PROFILE_END_SESSION() XEON::Instrumentor::Get().endSession()
	#define XEON_PROFILE_BEGIN_NEW_SESSION(name, filepath) XEON_PROFILE_END_SESSION(); XEON_PROFILE_BEGIN_SESSION(name, filepath)
#else
	#define XEON_PROFILE_BEGIN_SESSION(name, filepath)
	#define XEON_PROFILE_FN()
	#define XEON_PROFILE_SCOPE(name)
	#define XEON_PROFILE_END_SESSION()
	#define XEON_PROFILE_BEGIN_NEW_SESSION(name, filepath)
#endif
