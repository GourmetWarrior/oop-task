#pragma once

#include <unordered_map>
#include "SaleEvent.h"

template <class Object>
class Registry {
private:
	static_assert(std::is_same<Object, Material>::value || std::is_same<Object, Product>::value ||
		std::is_same<Object, Customer>::value || std::is_same<Object, SaleEvent>::value, "Invalid registry type");

	std::vector<unsigned int> m_objectsID;
	std::unordered_map<unsigned int, Object> m_objects;
public:
	void clear() {
		m_objectsID.clear();
		m_objects.clear();
	}

	void remove(unsigned int ID) {
		try {
			auto got = std::find(this->m_objectsID.begin(), this->m_objectsID.end(), ID);
			if (got == m_objectsID.end()) {
				throw std::invalid_argument("Cannot remove a object: ID is not registered.\n");
			}
			m_objectsID.erase(got);
			m_objects.erase(ID);
			std::cout << typeid(Object).name() << " object " << ID << " is removed\n";
		}
		catch (const std::invalid_argument& error) {
			std::cout << error.what();
		}
	}

	void add(Object& object) {
		try {
			auto got = std::find(this->m_objectsID.begin(), this->m_objectsID.end(), object.ID());
			if (got != m_objectsID.end()) {
				throw std::invalid_argument("Cannot add an object: this ID has already registered.\n");
			}
			this->m_objectsID.push_back(object.ID());
			this->m_objects.insert({ object.ID(), std::move(object) });
			std::cout << typeid(Object).name() << " object " << object.ID() << " is registered\n";
		}
		catch (const std::invalid_argument& error) {
			std::cout << error.what();
		}
	}

	Object& getObjectByID(unsigned int ID) {
		try {
			auto got = std::find(this->m_objectsID.begin(), this->m_objectsID.end(), ID);
			if (got == m_objectsID.end()) {
				throw std::invalid_argument("Cannot find a object: ID is not registered.\n");
			}
			return this->m_objects.at(ID);
		}
		catch (const std::invalid_argument& error) {
			std::cout << error.what();
		}
	}

	auto begin() {
		return m_objectsID.begin();
	}

	auto end() {
		return m_objectsID.end();
	}
};