#include "MCString.h"

char* MCString::mEmpty = "";

MCString::MCString() : mBuffer(MCString::mEmpty)
{

}

MCString::MCString(double rNumber) : mBuffer(MCString::mEmpty)
{
	va_list ap;	
	const int size = 255;
	char buffer[size];

	va_start(ap, "%f");
	vsnprintf(buffer,size, "%f", ap);
	va_end(ap);
	
	CopyToBuffer(buffer);

	TrimRight('0');
	Trim('.');
}

MCString::MCString(char rVal) : mBuffer(MCString::mEmpty)
{
	mBuffer = new char[2];
	mBuffer[0] = rVal;
	mBuffer[1] = 0;
}

MCString::MCString(MCString* rVal) : mBuffer(MCString::mEmpty)
{
	CopyToBuffer(rVal->mBuffer);
}

MCString::MCString(const MCString &rCopy) : mBuffer(MCString::mEmpty)
{
	CopyToBuffer(rCopy);
}

MCString::MCString(MCString&& rMove) : mBuffer(MCString::mEmpty)
{
	rMove.mBuffer = MCString::mEmpty;
}

MCString::MCString(const char* rVal) : mBuffer(MCString::mEmpty)
{
	CopyToBuffer(rVal);
}

MCString::~MCString()
{
	if(mBuffer != MCString::mEmpty) {
		delete[] mBuffer;
	}
}
/*
char* MString::GetEmpty() {
	return MString::mEmpty;
}

void MString::SetEmpty(char* const &rEmpty)
{
	MString::mEmpty = rEmpty;
}*/

MCString MCString::Trim()
{
	char* offset = mBuffer;

	while(*offset == ' ' || *offset == '\n' || *offset == '\r' || *offset == '\t') {
		offset++;
	}

	for(unsigned int i = strlen(offset) - 1; i >= 0; i--) {
		if(offset[i] != ' ' && offset[i] != '\n' && offset[i] != '\r' && offset[i] != '\t') {
			offset[i + 1] = '\0';
			break;
		}
	}

	return MCString(offset);
}

MCString MCString::Trim(char rChar)
{
	char* offset = mBuffer;

	while(*offset == rChar) {
		offset++;
	}

	for(unsigned int i = strlen(offset) - 1; i >= 0; i--){
		if(offset[i] != rChar){
			offset[i + 1] = '\0';

			break;
		}
	}

	return MCString(offset);
}

MCString MCString::TrimLeft()
{
	char* offset = mBuffer;

	while(*offset == ' ' || *offset == '\n' || *offset == '\r' || *offset == '\t') {
		offset++;
	}

	return MCString(offset);
}

MCString MCString::TrimLeft(char rChar)
{
	char* offset = mBuffer;
	while(*offset == rChar) {
		offset++;
	}

	return MCString(offset);
}

MCString MCString::TrimRight()
{
	char* offset = mBuffer;

	for(unsigned int i = 0; i < strlen(offset) - 1; i++){
		if(offset[i] != ' ' && offset[i] != '\n' && offset[i] != '\r' && offset[i] != '\t') {
			offset[i + 1] = '\0';

			break;
		}
	}

	return MCString(offset);
}

MCString MCString::TrimRight(char rChar)
{
	char* offset = mBuffer;

	for(unsigned int i = 0; i < strlen(offset) - 1; i++){
		if(offset[i] != rChar) {
			offset[i + 1] = '\0';
			break;
		}
	}

	return MCString(offset);
}

MCString MCString::Replace(const MCString &rNeedle, const MCString &rReplace)
{
	MCString str("");

	char* offset = mBuffer;
	char* prev = offset;

	do {
		offset = strstr(offset, rNeedle);

		if(offset != NULL) {
			str.AppendToBuffer(prev, offset - prev);
			str += rReplace;
			offset += strlen(rNeedle);
			prev = offset;
		}
		else {
			str.AppendToBuffer(prev, strlen(prev));
		}

	} while(offset != NULL);

	return str;
}

MCString MCString::Reverse()
{
	MCString str(mBuffer);
	return StringReverse(str.mBuffer);
}

MCString MCString::ToLower()
{
	MCString str(mBuffer);
	return StringToLower(str.mBuffer);
}

MCString MCString::ToUpper()
{
	MCString str(mBuffer);
	return StringToUpper(str.mBuffer);
}

MCString MCString::Substring(int rStart)
{
	return Substring(rStart, Length() - rStart);

}

MCString MCString::Substring(int rStart, int rLength)
{
	if((unsigned int)rStart >= strlen(mBuffer)) {
		return "";
	}

	if(rStart < 0) {
		rLength += rStart;
		rStart = 0;

		if(rLength <= 0)
			return "";
	}

	MCString offset(mBuffer + rStart);

	if(rLength < 0)
		rLength = offset.Length() + rLength;

	if((unsigned int)rLength >= strlen(offset)) {
		offset.AppendToBuffer('\0');
		return offset;
	}

	offset[rLength] = '\0';

	return offset;
}

MCString MCString::Format(const char* rFormat, ...)
{
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];

	va_list list;
	va_start(list, rFormat);

	int lenght;
	while((lenght = snprintf(buffer, size, rFormat, list)) == -1 || size <= lenght) {
		delete[] buffer;

		size += size;
		buffer = new char[size];
	}

	va_end(list);

	MCString ret(buffer);
	delete[] buffer;

	return ret;
}

void MCString::SetFormat(const char* rFormat, ...)
{
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];

	va_list list;
	va_start(list, rFormat);

	int lenght = snprintf(buffer, size, rFormat, list);
	if(size <= lenght) {
		delete[] buffer;

		buffer = new char[lenght + 1];
		lenght = snprintf(buffer, size, rFormat, list);
	}

	va_end(list);

	CopyToBuffer(buffer);

	delete[] buffer;
}

void MCString::AppendFormat(const char* rFormat, ...)
{
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];

	va_list list;
	va_start(list, rFormat);

	int lenght = vsnprintf(buffer, size, rFormat, list);
	if(size <= lenght) {
		delete[] buffer;

		buffer = new char[lenght + 1];
		lenght = vsnprintf(buffer, size, rFormat, list);
	}

	va_end(list);

	AppendToBuffer(buffer);

	delete[] buffer;
}

void MCString::CopyToBuffer(const char* rBuffer)
{
	if(mBuffer != MCString::mEmpty) {
		delete[] mBuffer;
	}

	int lenght = strlen(rBuffer);
	if(lenght == 0) {
		mBuffer = MCString::mEmpty;
		return;
	}
	lenght++;
	mBuffer = new char[lenght];
	memcpy(mBuffer, rBuffer, lenght);
}

void MCString::AppendToBuffer(const char* rBuffer)
{
	size_t count = strlen(rBuffer);
	if(count <= 0) {
		return;
	}

	int size = Length();

	char* buffer = new char[size + count + 1];
	memcpy(buffer, mBuffer, size);
	memcpy(buffer + size, rBuffer, count);
	buffer[size + count] = 0;

	if(mBuffer != MCString::mEmpty)
		delete[] mBuffer;

	mBuffer = buffer;
}

void MCString::AppendToBuffer(const char* rBuffer, int rCounter)
{
	if(rCounter <= 0)
		return;

	int size = Length();

	char* buffer = new char[size + rCounter + 1];
	memcpy(buffer, mBuffer, size);
	memcpy(buffer + size, rBuffer, rCounter);
	buffer[size + rCounter] = 0;

	if(mBuffer != MCString::mEmpty) {
		delete[] mBuffer;
	}

	mBuffer = buffer;
}

void MCString::AppendToBuffer(char rBuffer)
{
	int size = Length();

	char* buffer = new char[size + 2];
	memcpy(buffer, mBuffer, size);
	buffer[size] = rBuffer;
	buffer[++size] = 0;

	if(mBuffer != MCString::mEmpty) {
		delete[] mBuffer;
	}

	mBuffer = buffer;
}

void MCString::Split(const MCString &rNeedle, MCArray<MCString> &rResult)
{
	char* offset = mBuffer;
	char* prev = offset;

	do{
		offset = strstr(offset, rNeedle);

		if(offset != NULL) {
			int lenght = offset - prev;
			char* part = new char[lenght + 1];

			int index = 0;
			for(int i = 0; i < lenght; i++) {
				part[i] = *(offset - (lenght - i));
				index = i;
			}
			part[index] = '\0';


			rResult.Push() = part;

			delete[] part;

			offset += strlen(rNeedle);
			prev = offset;
		}
		else {
			int lenght = strlen(prev);
			char* part = new char[lenght + 1];

			int index = 0;
			for(int i = 0; i < lenght; i++) {
				part[i] = prev[i];
				index = i;
			}
			part[index] = '\0';

			rResult.Push() = part;
			delete[] part;
		}
	} while(offset != NULL);
}

int MCString::Count(const MCString &rNeedle)
{
	int counter = 0;

	char* offset = mBuffer;
	char* prev = offset;

	do {

		offset = strstr(offset, rNeedle);

		if(offset != NULL) {
			counter++;
			offset += strlen(rNeedle);
			prev = offset;
		}
	} while(offset != NULL);

	return counter;
}

int MCString::Length()
{
	return strlen(mBuffer);
}

int MCString::IndexOf(char rChar)
{
	int lenght = Length();
	for(int i = 0; i < lenght; i++) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}
	return -1;
}

int MCString::IndexOf(char rChar, int rPos)
{
	int lenght = Length();
	for(int i = rPos; i < lenght; i++) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}
	return -1;
}

int MCString::IndexOf(MCString rTxt, int rPos)
{
	int lenght = Length();
	int textLenght = rTxt.Length();

	for(int i = rPos; i <= lenght - textLenght; i++) {
		if(MCString(mBuffer).Substring(i, textLenght) == rTxt) {
			return i;
		}
	}
	return -1;
}

int MCString::LastIndexOf(char rChar)
{
	int lenght = Length();
	for(unsigned int i = lenght - 1; i > -1; i--) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}

	return -1;
}

int MCString::LastIndexOf(char rChar, int rPos)
{
	int lenght = Length();
	for(int i = rPos; i > -1; i--) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}
	return -1;
}

int MCString::LastIndexOf(MCString rTxt, int rPos)
{
	int lenght = Length();
	int textLenght = Length();

	for(int i = rPos; i > -1; i--){
		if(MCString(mBuffer).Substring(i, textLenght) == rTxt) {
			return i;
		}
	}
	return -1;
}

bool MCString::Contains(const MCString &rPoint)
{
	return strstr(mBuffer, rPoint) != NULL;
}

bool MCString::StartsWith(const MCString &rPoint)
{
	return strstr(mBuffer, rPoint) == mBuffer;
}

bool MCString::StartsWith(const char rPoint)
{
	return rPoint == mBuffer[0];
}

bool MCString::EndsWith(const MCString &rPoint)
{
	MCString* point = (MCString*)&rPoint;

	int a = Length();
	int b = point->Length();

	if(b > a) return false;

	return Substring(a - b) == rPoint;
}

bool MCString::EndsWith(const char rPoint)
{
	return rPoint == mBuffer[strlen(mBuffer) - 1];
}

MCString::operator const char*() {
	return  mBuffer;
}

MCString::operator const char*() const {
	return mBuffer;
}

MCString& MCString::operator=(char* rChar)
{
	CopyToBuffer(rChar);
	return *this;
}

MCString& MCString::operator=(const char* rChar)
{
	CopyToBuffer(rChar);
	return *this;
}

MCString& MCString::operator=(const MCString &rStr)
{
	if(this != &rStr){
		CopyToBuffer(rStr);
	}
	return *this;
}

MCString& MCString::operator+=(const char* rChar)
{
	AppendToBuffer(rChar);
	return *this;
}

MCString& MCString::operator+=(const char rChar)
{
	AppendToBuffer(rChar);
	return *this;
}

bool MCString::operator==(const char* rChar)
{
	return !strcmp(mBuffer, rChar);
}

bool MCString::operator!=(const char* rChar)
{
	return strcmp(mBuffer, rChar) != 0;
}

char& MCString::operator[](int rIndex)
{
	return mBuffer[rIndex];
}

char* MCString::StringReverse(char* rString)
{
	char ch;
	int index = 0;
	for(unsigned int i = 0; i < strlen(rString) - 1; i++){
		ch = rString[i];
		rString[i] = rString[index];
		rString[index] = ch;
		index++;
	}
	return rString;
}

char* MCString::StringToLower(char* rString)
{

	unsigned char *point = (unsigned char *)rString;

	while(*point) {
		*point = tolower((unsigned char)*point);
		point++;
	}

	return rString;
}

char* MCString::StringToUpper(char* rString)
{

	unsigned char *point = (unsigned char *)rString;

	while(*point) {
		*point = toupper((unsigned char)*point);
		point++;
	}

	return rString;
}

MCString operator+(const MCString &rLHS, const char* rRHS)
{
	return MCString(rLHS) += rRHS;
}

MCString operator+(const MCString &rLHS, const char rRHS)
{
	return MCString(rLHS) += rRHS;
}