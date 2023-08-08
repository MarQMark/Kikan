#include "Kikan/core/Logging.h"

namespace Kikan {
    namespace Core {
        Logging *Logging::s_logging = nullptr;

        Logging::Logging() {

        }

        Logging* Logging::get() {
            if(s_logging == nullptr)
                s_logging = new Logging();

            return s_logging;
        }

        void Logging::deInit() {
            delete s_logging;
        }

        void Logging::print_private(Level level, const char* format, va_list arg) {
            if (level > _level)
                return;

            vprintf(format, arg);
        }

        void Logging::print(const char *format, ...) {
            va_list arg;
            va_start(arg, format);
            print_private(_level, format, arg);
            va_end(arg);
        }

        void Logging::printE(const char* format, ...) {
            va_list arg;
            va_start(arg, format);
            print_private(Level::ERROR, format, arg);
            va_end(arg);
        }

        void Logging::printW(const char* format, ...) {
            va_list arg;
            va_start(arg, format);
            print_private(Level::WARNING, format, arg);
            va_end(arg);
        }

        void Logging::printI(const char* format, ...) {
            va_list arg;
            va_start(arg, format);
            print_private(Level::INFO, format, arg);
            va_end(arg);
        }

        void Logging::printV(const char* format, ...) {
            va_list arg;
            va_start(arg, format);
            print_private(Level::VERBOSE, format, arg);
            va_end(arg);
        }

        void Logging::printL(Logging::Level level, const char *format, ...) {
            va_list arg;
            va_start(arg, format);
            print_private(level, format, arg);
            va_end(arg);
        }
    }
}