#pragma once

#include <vector>
#include "Material.h"

class Product : public General {
private:
	std::string m_type;
	std::string m_weight;
	std::string m_price;
	std::vector<unsigned int> m_materialsID;

public:
	Product() {}
	Product(std::string name, std::string type, std::string weight, std::string price, std::vector<unsigned int>& materialsID) {
		this->m_name = std::move(name);
		this->m_type = std::move(type);
		this->m_weight = std::move(weight);
		this->m_price = std::move(price);
		this->m_materialsID = std::move(materialsID);
	}

	std::string type() const {
		return m_type;
	}

	std::vector<unsigned int> materialsID() const {
		return m_materialsID;
	}

	std::string weight() const {
		return m_weight;
	}

	std::string price() const {
		return m_price;
	}

	float weightAsFloat() const {
		return std::stof(m_weight);
	}

	float priceAsFloat() const {
		return std::stof(m_price);
	}

	void setType(std::string type) {
		this->m_type = std::move(type);
	}

	void setMaterialsID(std::vector<unsigned int>& materials_ID) {
		this->m_materialsID = std::move(materials_ID);
	}

	void setWeight(std::string weight) {
		this->m_weight = std::move(weight);
	}

	void setProductPrice(std::string price) {
		this->m_price = std::move(price);
	}
};
