#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr std::uint32_t PAGE_SIZE = 4096;
constexpr std::uint32_t NAME_SIZE = 32;
constexpr std::uint32_t ROW_SIZE = 1 + sizeof(std::int32_t) + NAME_SIZE;
constexpr std::uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

struct Row {
    bool active = true;
    std::int32_t id = 0;
    std::string name;
};

std::string fit_text(const std::string& text, std::size_t max_size) {
    if (text.size() <= max_size) {
        return text;
    }
    return text.substr(0, max_size);
}

void serialize_row(const Row& row, std::array<char, ROW_SIZE>& output) {
    output.fill(0);
    output[0] = row.active ? 1 : 0;
    std::memcpy(output.data() + 1, &row.id, sizeof(row.id));

    const std::string stored_name = fit_text(row.name, NAME_SIZE);
    std::memcpy(output.data() + 1 + sizeof(row.id), stored_name.data(), stored_name.size());
}

Row deserialize_row(const std::array<char, ROW_SIZE>& input) {
    Row row;
    row.active = input[0] != 0;
    std::memcpy(&row.id, input.data() + 1, sizeof(row.id));
    row.name.assign(input.data() + 1 + sizeof(row.id), NAME_SIZE);
    row.name.erase(row.name.find_last_not_of('\0') + 1);
    return row;
}

std::uint64_t row_offset(std::uint64_t row_number) {
    const std::uint64_t page_number = row_number / ROWS_PER_PAGE;
    const std::uint64_t row_in_page = row_number % ROWS_PER_PAGE;
    return page_number * PAGE_SIZE + row_in_page * ROW_SIZE;
}

std::uint64_t row_count_for_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input) {
        return 0;
    }
    const auto size = static_cast<std::uint64_t>(input.tellg());
    const auto complete_pages = size / PAGE_SIZE;
    const auto tail_bytes = size % PAGE_SIZE;
    return complete_pages * ROWS_PER_PAGE + tail_bytes / ROW_SIZE;
}

bool write_row(const std::string& path, std::uint64_t row_number, const Row& row) {
    std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        return false;
    }

    std::array<char, ROW_SIZE> bytes{};
    serialize_row(row, bytes);
    file.seekp(static_cast<std::streamoff>(row_offset(row_number)));
    file.write(bytes.data(), bytes.size());
    return static_cast<bool>(file);
}

bool read_row(const std::string& path, std::uint64_t row_number, Row& row) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }

    std::array<char, ROW_SIZE> bytes{};
    file.seekg(static_cast<std::streamoff>(row_offset(row_number)));
    file.read(bytes.data(), bytes.size());
    if (file.gcount() != static_cast<std::streamsize>(bytes.size())) {
        return false;
    }

    row = deserialize_row(bytes);
    return true;
}

void print_help() {
    std::cout << "MiniDB Day 2 commands:\n"
              << "  insert <id> <name>  append a student row\n"
              << "  .rows               print stored rows\n"
              << "  .stats              show storage settings\n"
              << "  .help         show this help\n"
              << "  .exit         close MiniDB\n";
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

    std::cout << "MiniDB Day 2 opened " << database_path << '\n';
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
            std::cout << "Page size: " << PAGE_SIZE << " bytes\n"
                      << "Row size: " << ROW_SIZE << " bytes\n"
                      << "Rows per page: " << ROWS_PER_PAGE << '\n'
                      << "Stored rows: " << row_count_for_file(database_path) << '\n';
            continue;
        }
        if (line == ".rows") {
            const auto total_rows = row_count_for_file(database_path);
            if (total_rows == 0) {
                std::cout << "(no rows)\n";
                continue;
            }

            for (std::uint64_t i = 0; i < total_rows; ++i) {
                Row row;
                if (read_row(database_path, i, row) && row.active) {
                    std::cout << std::setw(4) << row.id << " | " << row.name << '\n';
                }
            }
            continue;
        }

        std::istringstream command(line);
        std::string keyword;
        command >> keyword;
        if (keyword == "insert") {
            Row row;
            command >> row.id >> row.name;
            if (!command || row.name.empty()) {
                std::cerr << "Usage: insert <id> <name>\n";
                continue;
            }

            const auto next_row = row_count_for_file(database_path);
            if (!write_row(database_path, next_row, row)) {
                std::cerr << "Could not store row.\n";
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
