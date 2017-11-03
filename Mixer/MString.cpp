#include "MString.h"

char* MString::mEmpty = "";

MString::MString() : mBuffer(MString::mEmpty)
{

}

MString::MString(double rNumber) : mBuffer(MString::mEmpty)
{
	char buffer[255];
	sprintf(buffer, "%f", rNumber);

	CopyToBuffer(buffer);

	TrimRight('0');
	Trim('.');
}

MString::MString(char rVal) : mBuffer(MString::mEmpty)
{
	mBuffer = new char[2];
	mBuffer[0] = rVal;
	mBuffer[1] = 0;
}

MString::MString(MString* rVal) : mBuffer(MString::mEmpty)
{
	CopyToBuffer(rVal->mBuffer);
}

MString::MString(const MString &rCopy) : mBuffer(MString::mEmpty)
{
	CopyToBuffer(rCopy);
}

MString::MString(MString&& rMove) : mBuffer(MString::mEmpty)
{
	rMove.mBuffer = MString::mEmpty;
}

MString::MString(const char* rVal) : mBuffer(MString::mEmpty)
{
	CopyToBuffer(rVal);
}

MString::~MString()
{
	if(mBuffer != MString::mEmpty) {
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

MString MString::Trim()
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

	return MString(offset);
}

MString MString::Trim(char rChar)
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

	return MString(offset);
}

MString MString::TrimLeft()
{
	char* offset = mBuffer;

	while(*offset == ' ' || *offset == '\n' || *offset == '\r' || *offset == '\t') {
		offset++;
	}

	return MString(offset);
}

MString MString::TrimLeft(char rChar)
{
	char* offset = mBuffer;
	while(*offset == rChar) {
		offset++;
	}

	return MString(offset);
}

MString MString::TrimRight()
{
	char* offset = mBuffer;

	for(unsigned int i = 0; i < strlen(offset) - 1; i++){
		if(offset[i] != ' ' && offset[i] != '\n' && offset[i] != '\r' && offset[i] != '\t') {
			offset[i + 1] = '\0';

			break;
		}
	}

	return MString(offset);
}

MString MString::TrimRight(char rChar)
{
	char* offset = mBuffer;

	for(unsigned int i = 0; i < strlen(offset) - 1; i++){
		if(offset[i] != rChar) {
			offset[i + 1] = '\0';
			break;
		}
	}

	return MString(offset);
}

MString MString::Replace(const MString &rNeedle, const MString &rReplace)
{
	MString str("");

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

MString MString::Reverse()
{
	MString str(mBuffer);
	return StringReverse(str.mBuffer);
}

MString MString::ToLower()
{
	MString str(mBuffer);
	return StringToLower(str.mBuffer);
}

MString MString::ToUpper()
{
	MString str(mBuffer);
	return StringToUpper(str.mBuffer);
}

MString MString::Substring(int rStart)
{
	return Substring(rStart, Length() - rStart);

}

MString MString::Substring(int rStart, int rLength)
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

	MString offset(mBuffer + rStart);

	if(rLength < 0)
		rLength = offset.Length() + rLength;

	if((unsigned int)rLength >= strlen(offset)) {
		offset.AppendToBuffer('\0');
		return offset;
	}

	offset[rLength] = '\0';

	return offset;
}

MString MString::Format(const char* rFormat, ...)
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

	MString ret(buffer);
	delete[] buffer;

	return ret;
}

void MString::SetFormat(const char* rFormat, ...)
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

void MString::AppendFormat(const char* rFormat, ...)
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

void MString::CopyToBuffer(const char* rBuffer)
{
	if(mBuffer != MString::mEmpty) {
		delete[] mBuffer;
	}

	int lenght = strlen(rBuffer);
	if(lenght == 0) {
		mBuffer = MString::mEmpty;
		return;
	}
	lenght++;
	mBuffer = new char[lenght];
	memcpy(mBuffer, rBuffer, lenght);
}

void MString::AppendToBuffer(const char* rBuffer)
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

	if(mBuffer != MString::mEmpty)
		delete[] mBuffer;

	mBuffer = buffer;
}

void MString::AppendToBuffer(const char* rBuffer, int rCounter)
{
	if(rCounter <= 0)
		return;

	int size = Length();

	char* buffer = new char[size + rCounter + 1];
	memcpy(buffer, mBuffer, size);
	memcpy(buffer + size, rBuffer, rCounter);
	buffer[size + rCounter] = 0;

	if(mBuffer != MString::mEmpty) {
		delete[] mBuffer;
	}

	mBuffer = buffer;
}

void MString::AppendToBuffer(char rBuffer)
{
	int size = Length();

	char* buffer = new char[size + 2];
	memcpy(buffer, mBuffer, size);
	buffer[size] = rBuffer;
	buffer[++size] = 0;

	if(mBuffer != MString::mEmpty) {
		delete[] mBuffer;
	}

	mBuffer = buffer;
}

void MString::Split(const MString &rNeedle, MArray<MString> &rResult)
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

int MString::Count(const MString &rNeedle)
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

int MString::Length()
{
	return strlen(mBuffer);
}

int MString::IndexOf(char rChar)
{
	int lenght = Length();
	for(int i = 0; i < lenght; i++) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}
	return -1;
}

int MString::IndexOf(char rChar, int rPos)
{
	int lenght = Length();
	for(int i = rPos; i < lenght; i++) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}
	return -1;
}

int MString::IndexOf(MString rTxt, int rPos)
{
	int lenght = Length();
	int textLenght = rTxt.Length();

	for(int i = rPos; i <= lenght - textLenght; i++) {
		if(MString(mBuffer).Substring(i, textLenght) == rTxt) {
			return i;
		}
	}
	return -1;
}

int MString::LastIndexOf(char rChar)
{
	int lenght = Length();
	for(unsigned int i = lenght - 1; i > -1; i--) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}

	return -1;
}

int MString::LastIndexOf(char rChar, int rPos)
{
	int lenght = Length();
	for(int i = rPos; i > -1; i--) {
		if(mBuffer[i] == rChar) {
			return i;
		}
	}
	return -1;
}

int MString::LastIndexOf(MString rTxt, int rPos)
{
	int lenght = Length();
	int textLenght = Length();

	for(int i = rPos; i > -1; i--){
		if(MString(mBuffer).Substring(i, textLenght) == rTxt) {
			return i;
		}
	}
	return -1;
}

bool MString::Contains(const MString &rPoint)
{
	return strstr(mBuffer, rPoint) != NULL;
}

bool MString::StartsWith(const MString &rPoint)
{
	return strstr(mBuffer, rPoint) == mBuffer;
}

bool MString::StartsWith(const char rPoint)
{
	return rPoint == mBuffer[0];
}

bool MString::EndsWith(const MString &rPoint)
{
	MString* point = (MString*)&rPoint;

	int a = Length();
	int b = point->Length();

	if(b > a) return false;

	return Substring(a - b) == rPoint;
}

bool MString::EndsWith(const char rPoint)
{
	return rPoint == mBuffer[strlen(mBuffer) - 1];
}

MString::operator const char*() {
	return  mBuffer;
}

MString::operator const char*() const {
	return mBuffer;
}

MString& MString::operator=(char* rChar)
{
	CopyToBuffer(rChar);
	return *this;
}

MString& MString::operator=(const char* rChar)
{
	CopyToBuffer(rChar);
	return *this;
}

MString& MString::operator=(const MString &rStr)
{
	if(this != &rStr){
		CopyToBuffer(rStr);
	}
	return *this;
}

MString& MString::operator+=(const char* rChar)
{
	AppendToBuffer(rChar);
	return *this;
}

MString& MString::operator+=(const char rChar)
{
	AppendToBuffer(rChar);
	return *this;
}

bool MString::operator==(const char* rChar)
{
	return !strcmp(mBuffer, rChar);
}

bool MString::operator!=(const char* rChar)
{
	return strcmp(mBuffer, rChar) != 0;
}

char& MString::operator[](int rIndex)
{
	return mBuffer[rIndex];
}

char* MString::StringReverse(char* rString)
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

char* MString::StringToLower(char* rString)
{

	unsigned char *point = (unsigned char *)rString;

	while(*point) {
		*point = tolower((unsigned char)*point);
		point++;
	}

	return rString;
}

char* MString::StringToUpper(char* rString)
{

	unsigned char *point = (unsigned char *)rString;

	while(*point) {
		*point = toupper((unsigned char)*point);
		point++;
	}

	return rString;
}

MString operator+(const MString &rLHS, const char* rRHS)
{
	return MString(rLHS) += rRHS;
}

MString operator+(const MString &rLHS, const char rRHS)
{
	return MString(rLHS) += rRHS;
}