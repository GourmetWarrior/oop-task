#pragma once

#include "Registry.h"

struct Registries {
private:
	Registry<Material> m_materials;
	Registry<Product> m_products;
	Registry<Customer> m_customers;
	Registry<SaleEvent> m_saleEvents;
public:
	Registry<Material>& materials() {
		return m_materials;
	}

	Registry<Product>& products() {
		return m_products;
	}

	Registry<Customer>& customers() {
		return m_customers;
	}

	Registry<SaleEvent>& saleEvents() {
		return m_saleEvents;
	}
};

class Data {
protected:
	Registries m_registries;
public:

	Registries& registries() {
		return m_registries;
	}

	void setRegistries(Registries& registries) {
		this->m_registries = std::move(registries);
	}

	virtual bool read(const std::string& fileName) = 0 {}
	virtual bool write(const std::string& fileName) = 0 {}
};