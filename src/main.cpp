#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void print_help() {
    std::cout << "MiniDB Day 1 commands:\n"
              << "  write <text>  append text to the database file\n"
              << "  read          print everything stored in the database file\n"
              << "  .stats        show file size in bytes\n"
              << "  .help         show this help\n"
              << "  .exit         close MiniDB\n";
}

std::streamoff file_size(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input) {
        return 0;
    }
    return input.tellg();
}

bool append_line(const std::string& path, const std::string& text) {
    std::ofstream output(path, std::ios::binary | std::ios::app);
    if (!output) {
        return false;
    }
    output << text << '\n';
    return true;
}

bool read_all(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return false;
    }

    std::string line;
    int line_number = 1;
    while (std::getline(input, line)) {
        std::cout << line_number++ << ": " << line << '\n';
    }

    if (line_number == 1) {
        std::cout << "(empty database file)\n";
    }
    return true;
}

std::string trim_left(std::string text) {
    const auto first = text.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return "";
    }
    return text.substr(first);
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <database-file>\n";
        return 1;
    }

    const std::string database_path = argv[1];

    {
        std::ofstream create_if_missing(database_path, std::ios::binary | std::ios::app);
        if (!create_if_missing) {
            std::cerr << "Could not open database file: " << database_path << '\n';
            return 1;
        }
    }

    std::cout << "MiniDB Day 1 opened " << database_path << '\n';
    std::cout << "Type .help for commands.\n";

    std::string line;
    while (true) {
        std::cout << "MiniDB > ";
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;
        }

        if (line == ".exit") {
            break;
        }
        if (line == ".help") {
            print_help();
            continue;
        }
        if (line == ".stats") {
            std::cout << "File size: " << file_size(database_path) << " bytes\n";
            continue;
        }
        if (line == "read") {
            if (!read_all(database_path)) {
                std::cerr << "Could not read database file.\n";
            }
            continue;
        }
        if (line.rfind("write ", 0) == 0) {
            const std::string text = trim_left(line.substr(6));
            if (text.empty()) {
                std::cerr << "Usage: write <text>\n";
                continue;
            }
            if (!append_line(database_path, text)) {
                std::cerr << "Could not write to database file.\n";
                continue;
            }
            std::cout << "ok\n";
            continue;
        }

        std::cerr << "Unrecognized command. Type .help for help.\n";
    }

    std::cout << "Bye.\n";
    return 0;
}

