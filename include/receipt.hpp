#pragma once



#include "product.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>



enum class PaymentMethod {
		cash,
		card
	};


struct ReceiptItem {

	ReceiptItem(const Product* product, int count) 
		: product(product)
		, count(count)
	{
		if (count < 1) {
			throw std::invalid_argument("Less than a single(1) example of an item can't be added");
		}
	}

	const Product* get_product() const noexcept {
		return product;
	}

	int get_count() const noexcept {
		return count;
	}

	double get_total_price() const noexcept {
		return product->get_price() * count;
	}

private:

	const Product* product;
	int count;
};



struct ReceiptCreator;

class Receipt {

	friend ReceiptCreator;

	static constexpr PaymentMethod default_method = PaymentMethod::cash;

	explicit Receipt() 
		: payment_method(default_method)
		, total(0.0)
		, payment(0.0)
		, change(0.0)
		, closed(false)
	{}

	void add_item(const Product* product, int count) {

		if (count < 1) {
			throw std::invalid_argument("cannot add less than a single(1) example of an item");
		}

		items.emplace_back(product, count);
		total += items.back().get_total_price();
	}

	void close(PaymentMethod method, double payment_amount) {

		if (closed) {
			throw std::runtime_error("The receipt has already been closed"); ///
		}

		if (payment_amount < total) {

			throw std::invalid_argument("The payment amount cannot be less than total");
		}
		else if (payment_amount > total && method == PaymentMethod::card) {

			throw std::runtime_error("If you pay by card, then you have to pay the exact value of total");
		}

		payment_method = method;
		payment = payment_amount;
		change = payment - total;

		closed = true;

		print_result();

	}

	bool is_closed() const noexcept {
		return closed;
	}

public:

	void print_result() const {
		
		std::cout << "===== Receipt =====\n";
        std::cout << "Items:\n";
        for (const auto& item : items) {
            std::cout << "- " << item.get_product()->get_name() << " (x" << item.get_count() << ") @ "
                      << item.get_product()->get_price() << " = " << item.get_total_price() << "\n";
        }
        std::cout << "Total: " << total << "\n";
        std::cout << "Payment method: " << (payment_method == PaymentMethod::cash ? "cash" : "card") << "\n";
        std::cout << "Payment amount: " << payment << "\n";
        if (payment_method == PaymentMethod::cash) {
            std::cout << "Change: " << change << "\n";
        }
        std::cout << "==================\n";
	}

	double get_total() const noexcept {
		return total;
	}

	PaymentMethod get_payment_method() const noexcept {
		return payment_method;
	}

	double get_change() const noexcept {
		return change;
	}

private:

	std::vector<ReceiptItem> items;
	PaymentMethod payment_method;
	double total;
	double payment;
	double change;

	bool closed;
};