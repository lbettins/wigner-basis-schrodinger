#pragma once

#include <cstdlib>
#include <filesystem>
#include <string>

// Resolve the repo data/ directory: WIGNER_BASIS_DATA env, then compile-time
// WIGNER_BASIS_DATA_DIR, then common relative paths from the working directory.
inline std::string dataRoot() {
    if (const char* env = std::getenv("WIGNER_BASIS_DATA")) {
        return env;
    }
#ifdef WIGNER_BASIS_DATA_DIR
    return WIGNER_BASIS_DATA_DIR;
#endif
    namespace fs = std::filesystem;
    for (const char* candidate : {"data", "../data", "../../data"}) {
        std::error_code ec;
        if (fs::is_directory(candidate, ec)) {
            return fs::absolute(candidate).string();
        }
    }
    return "data";
}
