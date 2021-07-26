//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <thread>

namespace GooE {

    using FloatMs = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string Name;

        FloatMs Start;
        std::chrono::microseconds ElapsedTime;
        uint32_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        std::mutex m_Mutex;
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
    public:
        Instrumentor()
            : m_CurrentSession(nullptr)
        {
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession) {
                // If there is already a current session, then close it before beginning new one.
                // Subsequent profiling output meant for the original session will end up in the
                // newly opened session instead.  That's better than having badly formatted
                // profiling output.
                GOOE_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
                InternalEndSession();
            }
            m_OutputStream.open(filepath);

            if (m_OutputStream.is_open()) {
                m_CurrentSession = new InstrumentationSession({ name });
                WriteHeader();
            } else {
                GOOE_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
            }
        }

        void EndSession()
        {
            std::lock_guard lock(m_Mutex);
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json;

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            json << std::setprecision(3) << std::fixed;
            json << "{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << result.ElapsedTime.count() << ',';
            json << "\"name\":\"" << name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start.count();
            json << "},";

            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession) {
                m_OutputStream << json.str();
                m_OutputStream.flush();
            }
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream
                << "{}" //add empty event to avoid JSON syntax error
                << "]}";
            m_OutputStream.flush();
        }

        void InternalEndSession() {
            if (m_CurrentSession) {
                WriteFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();
            FloatMs start = FloatMs{ m_StartTimepoint.time_since_epoch() };
            auto end =
                std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

#define GOOE_PROFILE 1
#if GOOE_PROFILE
    #define GOOE_PROFILE_BEGIN_SESSION(name, filepath) ::GooE::Instrumentor::Get().BeginSession(name, filepath)
    #define GOOE_PROFILE_END_SESSION() ::GooE::Instrumentor::Get().EndSession()
    #define GOOE_PROFILE_SCOPE(name) ::GooE::InstrumentationTimer timer##__LINE__(name)
    #define GOOE_PROFILE_FUNCTION() GOOE_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define GOOE_PROFILE_BEGIN_SESSION(name, filepath)
    #define GOOE_PROFILE_END_SESSION()
    #define GOOE_PROFILE_SCOPE(name)
    #define GOOE_PROFILE_FUNCTION()
#endif