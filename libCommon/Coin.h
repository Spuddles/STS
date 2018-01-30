#pragma once
#include <string>

class Coin
{
public:
	Coin(unsigned int id, const std::string &name, const std::string &description, const std::string &token) :
		m_ID(id), m_Name(name), m_Description(description), m_Token(token) {};
	~Coin() {};

	unsigned int	getID() const { return m_ID; }
	std::string		getName() const { return m_Name; }
	std::string		getDescription() const { return m_Description; }
	std::string		getToken() const { return m_Token; }

private:
	unsigned int	m_ID;
	std::string		m_Name;
	std::string		m_Description;
	std::string		m_Token;
};