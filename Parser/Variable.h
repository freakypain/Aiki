#ifndef VARIABLE_H
#define VARIABLE_H

// Internal
#include "../Mixer/MCommon.h"
#include "../Mixer/MCString.h"
#include "../Pool/PReference.h"

class Variable {
public:
	enum Type {
		UNDEFINED,
		INT,
		FLOAT,
		STRING,
		OBJECT,
		Null,
		BOOL,
		TABLE,
		FUNCTION,
		CLASS,
		UserData = 9
	};

public:
	PReference mLinkerRef;

private:
	Type mType;
	int mValueID;
	int mValueInteger;
	bool mValueBool;
	float mValueFloat;
	char* mValueString;

	
public:
	static Variable* CreateVariable(const char *rValue);
	static Type	GetType(MCString rString);

	Variable(int rID = 0);
	~Variable();

	Type GetType() const;
	int GetID() const;
	int GetInteger() const;
	float GetFloat() const;
	bool GetBool() const;
	MCString GetString() const;
	
	void Set(int);
	void Set(float);
	void Set(const char*);
	void Undefine();

	void operator=(const Variable &rVariable);
	void operator+=(const Variable &rVariable);
	void operator+=(const int &rVariable);
	void operator+=(const float &rVariable);
	void operator-=(const Variable &rVariable);
	void operator-=(const int &rVariable);
	void operator-=(const float &rVariable);
	void operator*=(const Variable &rVariable);
	void operator*=(const int &rVariable);
	void operator*=(const float &rVariable);
	void operator/=(const Variable &rVariable);
	void operator/=(const int &rVariable);
	void operator/=(const float &rVariable);
	void operator%=(const Variable &rVariable);
	void operator%=(const int &rVariable);
	bool operator>(const Variable &rVariable) const;
	bool operator>(const int &rVariable) const;
	bool operator>(const float &rVariable) const;
	bool operator>=(const Variable &rVariable) const;
	bool operator>=(const int &rVariable) const;
	bool operator>=(const float &rVariable) const;
	bool operator<(const Variable &rVariable) const;
	bool operator<(const int &rVariable) const;
	bool operator<(const float &rVariable) const;
	bool operator<=(const Variable &rVariable) const;
	bool operator<=(const int &rVariable) const;
	bool operator<=(const float &rVariable) const;
	bool operator==(const Variable &rVariable) const;
	bool operator==(const int &rVariable) const;
	bool operator==(const float &rVariable) const;
	bool operator!=(const Variable &rVariable) const;
	bool operator!=(const int &rVariable) const;
	bool operator!=(const float &rVariable) const;

private:
	void Clear();
	bool Convert(const char *rString);

	enum commandPrompResults {
		UNDEFCMP = 0x01,
		GREATER = 0x02,
		LESS = 0x04,
		EQUAL = 0x08,
		NOTQUAL = 0x10,
	};

	commandPrompResults CompareWith(const Variable &rVariable) const;
	commandPrompResults CompareWithInt(const int &rVariable) const;
	commandPrompResults CompareWithFloat(const float &rVariable) const;
	commandPrompResults CompareWithString(const char *rVariable) const;
};
#endif // VARIABLE_H