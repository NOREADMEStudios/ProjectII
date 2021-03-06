#include "Defs.h"
#include "Log.h"

void Utils::Release(void* object) {
	try {
		delete object;
	} catch (int exception) {
		LOG("Can't delete element at position: %d", object);
	}
}

void Utils::ReleasePtr(void** objectPtr) {
	try {
		delete (*objectPtr);
		(*objectPtr) = nullptr;
	}
	catch (int exception) {
		LOG("Can't delete element at position: %d", *objectPtr);
	}
}

void Utils::ReleaseArray(void* array) {
	delete[] array;
}

/**
* Parse string to integers if possible
*/
int Utils::ParseInt(std::string s) {
	uint i = 0;
	int result = 0;
	bool err = false;
	while (s[i] != '\0' && !err) {
		switch (s[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			result *= 10;
			result += (s[i] - 48);
			break;
		default:
			result = 0;
			err = true;
			LOG("Cannot parse int. String contains non-numerical characters");
			break;
		}
		i++;
	}
	return result;
}

int Utils::ParseInt(const char* s) {
	uint i = 0;
	int result = 0;
	bool err = false;
	while (s[i] != '\0' && !err) {
		switch (s[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			result *= 10;
			result += (s[i] - 48);
			break;
		default:
			result = 0;
			err = true;
			LOG("Cannot parse int. String contains non-numerical characters");
			break;
		}
		i++;
	}
	return result;
}