#pragma once

#include "Product.h"
#include "Customer.h"

class SaleEvent 
{
private:
	unsigned int m_ID;
	std::string m_date;
	unsigned int m_customerID;
	std::vector<unsigned int> m_productsID;

public:
	SaleEvent() {}
	SaleEvent(std::string date, unsigned int customerID, std::vector<unsigned int>& productsID) {
		this->m_date = std::move(date);
		this->m_customerID = std::move(customerID);
		this->m_productsID = std::move(productsID);
	}

	unsigned int ID() const {
		return m_ID;
	}

	std::vector<unsigned int> productsID() const {
		return m_productsID;
	}

	std::string date() const {
		return m_date;
	}

	unsigned int customerID() const {
		return m_customerID;
	}

	void setID(unsigned int ID) {
		this->m_ID = std::move(ID);
	}

	void setProductsID(std::vector<unsigned int>& productsID) {
		this->m_productsID = productsID;
	}

	void setDate(std::string date) {
		this->m_date = std::move(date);
	}

	void setCustomerID(unsigned int customerID) {
		this->m_customerID = std::move(customerID);
	}
};