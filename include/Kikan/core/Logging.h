#ifndef KIKAN_LOGGING_H
#define KIKAN_LOGGING_H

#include <cstdio>
#include <cstdarg>

#define kikanPrint Kikan::Core::Logging::get()->print
#define kikanPrintL Kikan::Core::Logging::get()->printL
#define kikanPrintE Kikan::Core::Logging::get()->printE
#define kikanPrintW Kikan::Core::Logging::get()->printW
#define kikanPrintI Kikan::Core::Logging::get()->printI
#define kikanPrintV Kikan::Core::Logging::get()->printV

namespace Kikan {
    namespace Core {

        class Logging {
        private:
            static Logging* s_logging;

        public:
            enum Level {
                ERROR,
                WARNING,
                INFO,
                VERBOSE
            };

            static Logging* get();
            static void deInit();

            void print(const char* format, ...);
            void printL(Level level, const char* format, ...);
            void printE(const char* format, ...);
            void printW(const char* format, ...);
            void printI(const char* format, ...);
            void printV(const char* format, ...);
        private:
            Logging();
            ~Logging() = default;

            Level _level = Level::VERBOSE;

            void print_private(Level level, const char* format, va_list arg);
        };
    }
}

#endif //KIKAN_LOGGING_H
