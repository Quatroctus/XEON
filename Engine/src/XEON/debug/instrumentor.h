#pragma once

#include "XEON/core/core.h"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <thread>

// TODO: Manage the file type. Maybe we want to write and support different profile tracing types.
namespace XEON {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult {
		std::string name;
		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;
		std::thread::id threadID;
	};

	class Instrumentor {
	public:
		bool log = false, over = false;
		uint32_t frame = 0;
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void beginSession(const std::string& name, const std::string& filepath = "results.json", bool forceLog=false) {
			std::lock_guard<std::mutex> lock(lock); 
			if (forceLog) {
				over = (log = true);
			}
			outputStream.open(filepath);
			writeHeader();
			currentSession = name;
		}

		void endSession(bool reset=true) {
			std::lock_guard<std::mutex> lock(lock);
			if (reset) {
				over = (log = false);
			}
			writeFooter();
			outputStream.close();
			profileCount = 0;
		}

		void writeProfile(const ProfileResult& result) {
			std::lock_guard<std::mutex> lock(lock);
			if (!log) return;

			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.elapsedTime.count()) << ',';
			json << "\"name\":\"" << result.name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ",";
			json << "\"ts\":" << result.start.count();
			json << "}";
			
			outputStream << json.str();
			//outputStream.flush();
		}

		void writeHeader() {
			outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
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
		Instrumentor() = default;
		~Instrumentor() { endSession(); }

	};

	class InstrumentationTimer {
	public:
		explicit InstrumentationTimer(const char* name)
			: startTimePoint(std::chrono::high_resolution_clock::now()), result({ name, {}, {}, std::this_thread::get_id() }) {}

		~InstrumentationTimer() {
			if (!stopped) stop();
		}

		void stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();
			
			result.start = FloatingPointMicroseconds { startTimePoint.time_since_epoch() };
			result.elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch();
			result.threadID = std::this_thread::get_id();

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
	#define XEON_PROFILE_BEGIN_SESSION_OVERRIDE(name, filepath) XEON::Instrumentor::Get().beginSession(name, filepath, true)
	#define XEON_PROFILE_SCOPE(name) XEON::InstrumentationTimer timer##__LINE__(name);
	#define XEON_PROFILE_FN() XEON_PROFILE_SCOPE(__FUNCSIG__)
	#define XEON_PROFILE_END_SESSION() XEON::Instrumentor::Get().endSession()
	#define XEON_PROFILE_BEGIN_NEW_SESSION(name, filepath) XEON_PROFILE_END_SESSION(); XEON_PROFILE_BEGIN_SESSION(name, filepath)
	#define XEON_PROFILE_BEGIN_NEW_SESSION_OVERRIDE(name, filepath) XEON_PROFILE_END_SESSION(); XEON_PROFILE_BEGIN_SESSION_OVERRIDE(name, filepath)
#else
	#define XEON_PROFILE_BEGIN_SESSION(name, filepath)
	#define XEON_PROFILE_FN()
	#define XEON_PROFILE_SCOPE(name)
	#define XEON_PROFILE_END_SESSION()
	#define XEON_PROFILE_BEGIN_NEW_SESSION(name, filepath)
#endif
