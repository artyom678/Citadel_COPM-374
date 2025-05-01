#pragma once

#include <string>

struct Product {

	Product(const std::string& name, const std::string& barcode, double price) 
		: name(name)
		, barcode(barcode)
		, price(price)
	{}

	const std::string& get_name() const noexcept {
		return name;
	}

	const std::string& get_barcode() const noexcept {
		return barcode;
	}

	double get_price() const noexcept {
		return price;
	}


private:

	std::string name;
	std::string barcode;
	double price;

};