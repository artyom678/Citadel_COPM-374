#pragma once

#include "product_db.hpp"
#include <string>
#include <stdexcept>
#include "receipt.hpp"
#include "receipt_creator.hpp"


struct Session {

	Session(const ProductDB& db, const std::string& cashier_name, double initial_cash_amount) 
		: database(db)
		, cashier_name(cashier_name)
		, cash_availible(initial_cash_amount)
		, cash_total(0.0)
		, card_total(0.0)
		, closed(false)
		, receipt_creator(database, cash_availible)
	{

		if (cashier_name.empty()) {
			throw std::invalid_argument("cashier name cannot be empty");
		}
		else if (initial_cash_amount < 0.0) {
			throw std::invalid_argument("Inital amount of cash cannot be negative");
		}
	}

	bool is_closed() const noexcept {
		return closed;
	}

	void run() {

		if (closed) {
			throw std::runtime_error("Session is already closed");
		}

		std::string command;
		while(true) {

			std::cout << "\nCommands: open_receipt, close_session\n";
			std::cout << ">";
			std::getline(std::cin, command);

			if (command == "open_receipt") {
				

				auto receipt = receipt_creator.create_new_receipt(); // new receipt is created, processed and then returned
				if (!receipt) { // means that receipt was cancelled
					continue;
				}

				if (receipt->get_payment_method() == PaymentMethod::cash) {
					
					cash_total += receipt->get_total();
				}
				else {

					card_total += receipt->get_total();
				}
			}
			else if (command == "close_session") {
				break;
			}
			else {
				std::cout << "Unknown command!\n";
			}

		}

		close_session();

	}

private:

	void close_session() {

		if (closed) {
			return;
		}

		closed = true;
		print_summary();
	}

	void print_summary() const {

		std::cout << "===== Session summary =====\n"
        	<< "Total cash payments: " << cash_total << "\n"
        	<< "Total card payments: " << card_total << "\n"
        	<< "Total payments: " << card_total + cash_total << "\n"
        	<< "========================\n";

	}


private:

	const ProductDB& database;
	std::string cashier_name;
	double cash_availible;
	double card_total;
	double cash_total;

	bool closed;
	ReceiptCreator receipt_creator;
};