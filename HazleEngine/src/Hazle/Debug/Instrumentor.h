#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

// Instrumentor class for profiling code execution time
namespace Hazle
{
	// The Instrumentor class is responsible for profiling code execution time and writing the results to a JSON file.
	// This helps in performance analysis and optimization of the code.
	struct ProfileResult // Profile Result struct holds the profiling data for a single code block
	{
		std::string Name;           // Name of the profiled code section
		long long Start, End;       // Start and end timestamps in microseconds
		uint32_t ThreadID;          // ID of the thread where the code section was executed
	};

	struct InstrumentationSession   // Instrumentation Session struct holds the name of the profiling session
    {
        std::string Name;
    };


    class Instrumentor
    {
    public:
        Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0)
        {}

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")   // Begin a new profiling session and open the output file for writing
        {
			m_OutputStream.open(filepath);                                       // Open the output file for writing
			WriteHeader();                                                       // Write the JSON header to the output file
			m_CurrentSession = new InstrumentationSession{ name };               // Create a new InstrumentationSession object with the given name
        }                                                                        
                                                                                 
		void EndSession()                                                        // End the current profiling session and close the output file
		{                                                                        
			WriteFooter();                                                       // Write the JSON footer to the output file
			m_OutputStream.close();                                              // Close the output file
			delete m_CurrentSession;                                             // Delete the current InstrumentationSession object
			m_CurrentSession = nullptr;                                          // Set the current session pointer to nullptr
			m_ProfileCount = 0;                                                  // Reset the profile count
		}                                                                        
                                                                                 
		void WriteProfile(const ProfileResult& result)                           // Write a single profiling result to the output file in JSON format
		{                                                                        
			if (m_ProfileCount++ > 0)                                            // If this is not the first profile result, add a comma to separate it from the previous result
				m_OutputStream << ",";                                           
			std::string name = result.Name;                                      // Get the name of the profiled code section
			std::replace(name.begin(), name.end(), '"', '\'');                   // Replace double quotes with single quotes in the name to avoid JSON formatting issues
			m_OutputStream << "{";                                               
			m_OutputStream << "\"cat\":\"function\",";                           
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';  
			m_OutputStream << "\"name\":\"" << name << "\",";                    
			m_OutputStream << "\"ph\":\"X\",";                                   
			m_OutputStream << "\"pid\":0,";                                      
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";              
			m_OutputStream << "\"ts\":" << result.Start;                         
			m_OutputStream << "}";                                               
			m_OutputStream.flush();                                              // Flush the output stream to ensure that the data is written to the file immediately
		}

		void WriteHeader()                                                       // Write the JSON header to the output file
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();                                              // Flush the output stream to ensure that the data is written to the file immediately
		}

		void WriteFooter()                                                       // Write the JSON footer to the output file
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();                                              // Flush the output stream to ensure that the data is written to the file immediately
		}

		static Instrumentor& Get()                                               // Get the singleton instance of the Instrumentor class
		{
			static Instrumentor instance;                                        // Create a static instance of the Instrumentor class
			return instance;                                                     // Return a reference to the static instance
		}

    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    };

	// InstrumentationTimer class for measuring code execution time
    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
			m_StartTimepoint = std::chrono::high_resolution_clock::now();	// Record the start time of the code section being profiled
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
                Stop();
        }

		void Stop()			// Stop the timer and record the profiling result
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();															// Record the end time of the code section being profiled
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();	// Convert start time to microseconds
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();		// Convert end time to microseconds
			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());											// Get the ID of the current thread
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });														// Write the profiling result to the output file
			m_Stopped = true;																										// Mark the timer as stopped
		}

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

// Definig macros for profiling a code section with a given name
#define HZ_PROFILE 1
#if HZ_PROFILE
#define HZ_PROFILE_BEGIN_SESSION(name, filepath)  ::Hazle::Instrumentor::Get().BeginSession(name, filepath)
#define HZ_PROFILE_END_SESSION()                  ::Hazle::Instrumentor::Get().EndSession()
#define HZ_PROFILE_SCOPE(name)                    ::Hazle::InstrumentationTimer timer##__LINE__(name);
#define HZ_PROFILE_FUNCTION()                     HZ_PROFILE_SCOPE(__FUNCSIG__)
#else
#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
#define HZ_PROFILE_END_SESSION()
#define HZ_PROFILE_FUNCTION()
#define HZ_PROFILE_SCOPE(name)
#endif
