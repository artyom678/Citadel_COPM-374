#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include "product.hpp"
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>

//database for products
struct ProductDB {

	using json = nlohmann::json;

	ProductDB(const std::string& path_to_db) {

		load_db(path_to_db);
	}

	const Product* get_by_barcode(const std::string& barcode) const {
		
		auto it = by_barcode.find(barcode); 
		return it != by_barcode.end() ? &products[it->second] : nullptr;
	}

	const Product* get_by_name(const std::string& name) const { 
		
		auto it = by_name.find(name); 
		return it != by_name.end() ? &products[it->second] : nullptr;
	}


private:

	void load_db(const std::string& path_to_db) {

		std::ifstream db(path_to_db);
		if (!db) {
			throw std::runtime_error("Failed to open the JSON-file with product database");
		}

		json data;
		try {
			db >> data;
		}
		catch(json::parse_error& err) {
			throw std::runtime_error("Failed to parse JSON-file with product database : " + std::string(err.what()));
		}

		if (!data.is_array()) {
			throw std::runtime_error("Invalid JSON-file format, expected JSON-array");
		}

		for (const auto& product : data) {

			try {

				auto name = product["name"].get<std::string>();
				auto barcode = product["barcode"].get<std::string>();
				auto price = product["price"].get<double>();

				products.emplace_back(name, barcode, price);
				by_barcode.emplace(std::move(barcode), products.size() - 1);
				by_name.emplace(std::move(name), products.size() - 1);

			}
			catch(const json::exception& err) {
				throw std::runtime_error("Failed to process a product : " + std::string(err.what()));
			} 
		}

	}

private:

	std::vector<Product> products;
	std::unordered_map<std::string, std::size_t> by_barcode;
	std::unordered_map<std::string, std::size_t> by_name;
};