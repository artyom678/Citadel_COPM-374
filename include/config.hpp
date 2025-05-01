#pragma once

#include <stdexcept>
#include <string>
#include <fstream>

struct Config {

    Config(int argc, char* argv[]) {

    	parse(argc, argv);
    }
   

    const std::string& get_db_path() const noexcept {
        return db_path;
    }

private:

	bool is_valid_path(const std::string& path) const { //////////
		std::ifstream file(path);
		return file.good();
	}

	void parse(int argc, char* argv[]) {

		if (argc != 2) {
			throw std::runtime_error("Expected exactly 1 argument - the path to the JSON-file with the database\n"
				"Example: ./cash_register products.json");
        }
        db_path = argv[1];
        if (db_path.empty()) {
        	throw std::runtime_error("Path to the database-file cannot be empty");
        }
        if (!is_valid_path(db_path)) {
        	throw std::runtime_error("File " + db_path + " doesn't exist or is unavailible");
        }
	}

private:

    std::string db_path;
};