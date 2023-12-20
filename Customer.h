#pragma once

#include "General.h"

class Customer : public General {
private:
	std::string m_surname;
	std::string m_secname;

public:
	Customer() {}
	Customer(std::string surname, std::string name, std::string secname) {
		this->m_name = std::move(name);
		this->m_secname = std::move(secname);
	}

	std::string surname() const {
		return m_surname;
	}

	std::string secname() const {
		return m_secname;
	}

	void setSurname(std::string surname) {
		this->m_surname = std::move(surname);
	}

	void setSecname(std::string secname) {
		this->m_secname = std::move(secname);
	}
};
