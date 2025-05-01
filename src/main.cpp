#include <iostream>
#include "session.hpp"
#include "product_db.hpp"
#include <stdexcept>
#include <string>
#include "config.hpp"







struct CashRegister {

	CashRegister(const std::string& db_path) 
		: database(db_path)
	{}

	void run() {

		std::string command;
		while(true) {

			std::cout << "\nCommands: open_session, exit\n";
			std::cout << ">";
			std::getline(std::cin, command);

			if (command == "open_session") {
				try {
					open_session();
				}
				catch(const std::exception& ex) {
					std::cerr << "Session error : " << ex.what() << std::endl;
				}
			}
			else if (command == "exit") {
				break;
			}
			else {
				std::cout << "Error: Unknown command!\n";
			}

		}

	}

private:

	void open_session() noexcept(false) {

		std::string cashier_name;
		std::cout << "Enter the cashier name : ";
		std::getline(std::cin, cashier_name);

		std::string str_cash_availible;
		std::cout << "Enter the initial amount of cash : ";
		std::getline(std::cin, str_cash_availible);

		double cash_availible;
		try {
			cash_availible = std::stod(str_cash_availible);	
		}
		catch(const std::exception& ex) {
			std::cerr << "Invalid input : " << ex.what() << std::endl;
			return;
		}

		Session session(database, cashier_name, cash_availible);
		std::cout << "\n== Session successfully created! ==\n";

		session.run();

	}


private:

	ProductDB database;

};





int main(int argc, char* argv[]) {

	try {

		Config config(argc, argv);

		CashRegister cash_register(config.get_db_path());
		cash_register.run();

		return 0;
	}
	catch(const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

}