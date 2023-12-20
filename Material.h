#pragma once

#include "General.h"

class Material : public General {
private:
	std::string m_pricePerGram;

public:
	Material() {}
	Material(std::string name, std::string pricePerGram) {
		this->m_name = std::move(name);
		this->m_pricePerGram = std::move(pricePerGram);
	}

	std::string pricePerGram() const {
		return m_pricePerGram;
	}

	float pricePerGramAsFloat() const {
		return std::stof(m_pricePerGram);
	}

	void setPricePerGram(std::string pricePerGram) {
		this->m_pricePerGram = std::move(pricePerGram);
	}
};