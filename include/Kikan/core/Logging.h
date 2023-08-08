#ifndef KIKAN_LOGGING_H
#define KIKAN_LOGGING_H

#include <cstdio>
#include <cstdarg>

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

            Level _level = Level::ERROR;

            void print_private(Level level, const char* format, va_list arg);
        };
    }
}

#endif //KIKAN_LOGGING_H
