#pragma once

#include <string>

class General {
protected:
	unsigned int m_ID;
	std::string m_name;

public:
	virtual ~General() = 0 {}

	unsigned int ID() const {
		return m_ID;
	}

	std::string name() const {
		return m_name;
	}

	void setID(unsigned int ID) {
		this->m_ID = std::move(ID);
	}

	void setName(std::string name) {
		this->m_name = std::move(name);
	}
};