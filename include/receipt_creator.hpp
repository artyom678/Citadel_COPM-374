#pragma once

#include "product.hpp"
#include "product_db.hpp"
#include "receipt.hpp"
#include <exception>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>


struct ReceiptCreator {

	ReceiptCreator(const ProductDB& db, double& cash_availible) 
		: database(db)
		, cash_availible(cash_availible)
	{}


	std::shared_ptr<Receipt> create_new_receipt() {

		std::shared_ptr<Receipt> receipt(new Receipt);

		std::string command;
		while(true) {

			std::cout << "\nCommands: add_item, close_receipt, cancel_receipt\n";
			std::cout << ">";
			std::getline(std::cin, command);

			if (command == "add_item") {

				try {
					handle_add_item(*receipt);
					std::cout << "== An item was successfully added ==\n"; 
				}
				catch(const std::exception& ex) {
					std::cout << "Failed to add an item : " << ex.what() << std::endl;
					continue;
				}

			}
			else if (command == "close_receipt") {

				try {

					bool to_continue = handle_close_receipt(*receipt);
					if (!to_continue) {
						std::cout << "The receipt was cancelled\n";
						receipt.reset();
					}
				}
				catch(const std::exception& ex) {
					std::cout << "Failed to close the receipt : " << ex.what() << std::endl;
					continue;
				}

				break;
			}
			else if (command == "cancel_receipt") {
				std::cout << "The receipt was cancelled\n";
				receipt.reset();
				break;
			}
			else {
				std::cout << "Unknown command!\n";
			}

		}

		return receipt;
	}

private:

	void handle_add_item(Receipt& receipt) noexcept(false) {

		std::string search_term;
		std::cout << "Enter a name or a barcode of a product : ";
		std::getline(std::cin, search_term);

		const Product* product = database.get_by_barcode(search_term);
		if (!product) {
			product = database.get_by_name(search_term);
		}

		if (!product) {
			throw std::runtime_error("product not found");
		}

		std::string str_quantity;
		std::cout << "Enter the quantity : ";
		std::getline(std::cin, str_quantity);

		int quantity;
		try {
			quantity = std::stoi(str_quantity);
		}
		catch(const std::exception& ex) {
			throw std::runtime_error("Invalid quantity (expected integer > 0)");
		}

		receipt.add_item(product, quantity);
	}

	bool handle_close_receipt(Receipt& receipt) noexcept(false) { // returns false if the receipt was cancelled, true otherwise

		const double total = receipt.get_total();
		if (total == 0.0) {
			throw std::runtime_error("cannot close an empty receipt");
		}

		std::cout << "Your total is " << total << std::endl;

		PaymentMethod method;

		if (!get_payment_method(method)) {
			return false;
		}

		double payment_amount;
		double change;

		if (!get_payment_amount(method, total, payment_amount, change)) {
			return false;
		}


		receipt.close(method, payment_amount);

		return true;
	}

	bool get_payment_method(PaymentMethod& method) const {

		std::string payment_method_string;
		while(true) {
			std::cout << "Enter a payment method (cash/card) or 'cancel_receipt': ";
			std::getline(std::cin, payment_method_string);
			if (payment_method_string == "cash") {
				method = PaymentMethod::cash;
				break;
			}
			else if (payment_method_string == "card") {
				method = PaymentMethod::card;
				break;
			}
			else if (payment_method_string == "cancel_receipt") {
				return false;
			}
			else {
				std::cout << "Error: Unknown input, try again\n";
			}
		}

		return true;

	}

	bool get_payment_amount(const PaymentMethod method, const double total, double& payment_amount, double& change) {

		std::string payment_amount_string;
		while(true) {
					
			std::cout << "Enter the payment amount or 'cancel_receipt': ";
			std::getline(std::cin, payment_amount_string);
			if (payment_amount_string == "cancel_receipt") {
				
				return false;
			}

			try {
				payment_amount = std::stod(payment_amount_string);
			}
			catch(const std::exception& ex) {
				std::cerr << "Error: " << ex.what() << std::endl;
				continue;
			}

			if (payment_amount < total) {
				std::cout << "Error: too little payment, try again\n";
				continue;
			}
			else if (payment_amount > total) {

				if (method == PaymentMethod::card) {
					
					std::cout << "Error: if you pay by card, then you have to pay the exact value of total\n";
				}
				else {

					change = payment_amount - total;
					if (change > cash_availible) {
						std::cout << "Error: there's only " << cash_availible << " availible for change\n";
						std::cout << "So, the max payment amount can be " << total + cash_availible << std::endl;
					}
					else {

						cash_availible -= change;
						break;
					}
				}

				continue;

			}
			else {
				break;
			}
		}

		return true;
	}


private:
	const ProductDB& database;
	double& cash_availible;
};