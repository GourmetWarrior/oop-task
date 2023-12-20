#pragma once

#include <iostream>
#include "pugixml.hpp"
#include "Data.h"

class DataXML : public Data {
public:
	bool read(const std::string& fileName) override;
	bool write(const std::string& fileName) override;
};


bool DataXML::read(const std::string& fileName) {
	pugi::xml_document doc;
	if (!doc.load_file(fileName.c_str())) {
		std::cout << "Load file failed.\n";
		return false;
	}

	pugi::xml_node root = doc.child("jewelryworkshop");

	pugi::xml_node n_materials = root.child("materials");

	for (pugi::xml_node n_material = n_materials.first_child(); n_material; n_material = n_material.next_sibling()) {
		Material material;

		material.setID(n_material.attribute("ID").as_uint());
		material.setName(n_material.attribute("name").as_string());
		material.setPricePerGram(n_material.attribute("price_per_gram").as_string());

		m_registries.materials().add(material);
	}

	pugi::xml_node n_products = root.child("products");
	for (auto n_product : n_products.children()) {
		Product product;

		product.setID(n_product.attribute("ID").as_uint());
		product.setName(n_product.attribute("name").as_string());
		product.setType(n_product.attribute("type").as_string());
		product.setWeight(n_product.attribute("weight").as_string());
		product.setProductPrice(n_product.attribute("price").as_string());

		pugi::xml_node n_materialsID = n_product.child("materials_ID");
		std::vector<unsigned int> materialsID;
		for (auto n_materialID : n_materialsID.children()) {
			unsigned int materialID = n_materialID.attribute("ID").as_uint();
			materialsID.push_back(materialID);
		}
		product.setMaterialsID(materialsID);

		m_registries.products().add(product);
	}

	pugi::xml_node n_customers = root.child("customers");
	for (auto n_customer : n_customers.children()) {
		Customer customer;

		customer.setID(n_customer.attribute("ID").as_uint());
		customer.setSurname(n_customer.attribute("surname").as_string());
		customer.setName(n_customer.attribute("name").as_string());
		customer.setSecname(n_customer.attribute("secname").as_string());

		m_registries.customers().add(customer);
	}

	pugi::xml_node n_saleEvents = root.child("sale_events");
	for (auto n_saleEvent : n_saleEvents.children()) {
		SaleEvent saleEvent;

		saleEvent.setID(n_saleEvent.attribute("ID").as_uint());
		saleEvent.setDate(n_saleEvent.attribute("date").as_string());
		saleEvent.setCustomerID(n_saleEvent.attribute("customer_ID").as_uint());

		pugi::xml_node n_productsID = n_saleEvent.child("products_ID");
		std::vector<unsigned int> productsID;
		for (auto n_productID : n_productsID.children()) {
			unsigned int productID = n_productID.attribute("ID").as_uint();
			productsID.push_back(productID);
		}
		saleEvent.setProductsID(productsID);

		m_registries.saleEvents().add(saleEvent);
	}
	return true;
}


bool DataXML::write(const std::string& fileName) {
	pugi::xml_document doc;

	pugi::xml_node declaration_node = doc.append_child(pugi::node_declaration);
	declaration_node.append_attribute("version") = "1.0";
	declaration_node.append_attribute("encoding") = "UTF-8";

	pugi::xml_node root = doc.append_child("jewelryworkshop");

	pugi::xml_node n_materials = root.append_child("materials");
	for (auto iter = m_registries.materials().begin(); iter != m_registries.materials().end(); ++iter) {
		pugi::xml_node n_material = n_materials.append_child("material");
		Material material = m_registries.materials().getObjectByID(*iter);

		n_material.append_attribute("ID").set_value(material.ID());
		n_material.append_attribute("name").set_value(material.name().c_str());
		n_material.append_attribute("price_per_gram").set_value(material.pricePerGram().c_str());
	}

	pugi::xml_node n_products = root.append_child("products");
	for (auto iter = m_registries.products().begin(); iter != m_registries.products().end(); ++iter) {
		pugi::xml_node n_product = n_products.append_child("product");
		Product product = m_registries.products().getObjectByID(*iter);

		n_product.append_attribute("ID").set_value(product.ID());
		n_product.append_attribute("name").set_value(product.name().c_str());
		n_product.append_attribute("type").set_value(product.type().c_str());
		n_product.append_attribute("weight").set_value(product.weight().c_str());
		n_product.append_attribute("price").set_value(product.price().c_str());

		pugi::xml_node n_materialsID = n_product.append_child("materials_ID");
		std::vector<unsigned int> materialsID = product.materialsID();
		for (auto iter = materialsID.begin(); iter != materialsID.end(); ++iter) {
			pugi::xml_node n_materialID = n_materialsID.append_child("material_ID");
			n_materialID.append_attribute("ID").set_value(*iter);
		}
	}

	pugi::xml_node n_customers = root.append_child("customers");
	for (auto iter = m_registries.customers().begin(); iter != m_registries.customers().end(); ++iter) {
		pugi::xml_node n_customer = n_customers.append_child("customer");
		Customer customer = m_registries.customers().getObjectByID(*iter);

		n_customer.append_attribute("ID").set_value(customer.ID());
		n_customer.append_attribute("surname").set_value(customer.surname().c_str());
		n_customer.append_attribute("name").set_value(customer.name().c_str());
		n_customer.append_attribute("secname").set_value(customer.secname().c_str());
	}

	pugi::xml_node n_saleEvents = root.append_child("sale_events");
	for (auto iter = m_registries.saleEvents().begin(); iter != m_registries.saleEvents().end(); ++iter) {
		pugi::xml_node n_saleEvent = n_saleEvents.append_child("sale_event");
		SaleEvent saleEvent = m_registries.saleEvents().getObjectByID(*iter);

		n_saleEvent.append_attribute("ID").set_value(saleEvent.ID());
		n_saleEvent.append_attribute("date").set_value(saleEvent.date().c_str());
		n_saleEvent.append_attribute("customer_ID").set_value(saleEvent.customerID());

		pugi::xml_node n_productsID = n_saleEvent.append_child("products_ID");
		std::vector<unsigned int> productsID = saleEvent.productsID();
		for (auto iter = productsID.begin(); iter != productsID.end(); ++iter) {
			pugi::xml_node n_productID = n_productsID.append_child("product_ID");
			n_productID.append_attribute("ID").set_value(*iter);
		}
	}

	if (!doc.save_file(fileName.c_str())) {
		std::cout << "Save file failed.\n";
		return false;
	}
	return true;
}