/**
 * Apache#34464 reimplementation in C++.
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <chrono>
#include <thread>
#include <cassert>

using namespace std;

namespace apache34464 {

/**
 * String reimplement in C++.
 * see @ http://peterlij.iteye.com/blog/986574
 * see @ https://www.nowcoder.com/questionTerminal/b6daadf699884fb4856d5499cac7691d
 */
class String {
private:
	char* _value; // Immutable after init

public:
	String():_value(new char[1]) {
		*_value = '\0';
	}

	explicit String(const char* str):_value(new char[strlen(str) + 1]) {
		strcpy(_value, str);
	}

	String(const String& rhs):_value(new char[rhs.size() +1]) {
		strcpy(_value, rhs.c_str());
	}

	String& operator=(String rhs) {
		swap(rhs);
		return *this;
	}

	int size() const {
		return strlen(_value);
	}

	// Java String API
	int length() const {
		return size();
	}

	const char* c_str() const {
		return _value;
	}

	void swap(String& rhs) {
		std::swap(_value, rhs._value);
	}

	void debugprint() {
		std::cout << _value << std::endl;
	}
	// Java String API
	// see @ http://san-yun.iteye.com/blog/1950222
	int indexOf(const String& s) {
		const char* target = s.c_str();
		int targetLen = s.size();
		
		char first = target[0];
		int max = this->size() - targetLen;

		// look for target[0]
		for (int i = 0; i <= max; i++) {
			if (_value[i] != first) {
				while (++i <= max && _value[i] != first) {
					;;
				}
			}

			// target[0] found, now look for the rest.
			if (i <= max) {
				int j = i + 1;
				int end = j + targetLen - 1;
				for (int k = 1; j < end && _value[j] == target[k]; j++, k++) {
					;;
				}

				if (j == end) {
					// s found.
					return i;
				}
			}
		}
		return -1;
	}

	// Java String API
	bool equals(const String& rhs) {
		return strcmp(_value, rhs._value) == 0;
	}

	void print(std::ostream& out) {
		out << _value;
	}

	void read(std::istream& in) {
		in >> _value;
	}

	~String() {
		delete[] _value;
	}
};

std::ostream& operator<<(std::ostream& out, String& str) {
	str.print(out);
	return out;
}

std::istream& operator>>(std::istream& in, String& str) {
	str.read(in);
	return in;
}


/**
 * StringBuffer reimplement in C++.
 * see @ https://blog.csdn.net/u012877472/article/details/50808554
 */
class StringBuffer {
private:
	std::string _s;

public:
	StringBuffer() = default;

	explicit StringBuffer(const std::string& s): _s(s) {

	}

	explicit StringBuffer(const String& s): _s(s.c_str()) {

	}

	// Java API
	String toString() {
		return String(_s.c_str());
	}

	// Java API
	String substring(int start) {
		//std::string sub = _s.substr(start);
		return String(_s.c_str() + start);
	}

	// Java API va API
    StringBuffer& append(char ch) {
        _s += ch;
        return *this;
    }
	
	void debugprint() {
		std::cout << _s << std::endl;
	}

};


void testStringIndexOf() {
	const char a[] = "abc123";
	String* source = new String(a);	
	const char* b[] = {"a", "ab", "abc", "abc123", "bc", "c1", "bc123", "3", "c2", "dsf"};
	int expect[] = {0, 0, 0, 0, 1, 2, 1, 5, -1, -1};
	assert(sizeof(expect)/sizeof(int) == sizeof(b)/sizeof(const char*));
	
	for (int i = 0; i < sizeof(expect)/sizeof(int); i++) {
		String* target = new String(b[i]);
		assert(source->indexOf(*target) == expect[i]);
		delete target;
	}

	delete source;
}

void testStringBuffer() {
	char a[] = "abc123";
	String source(a);
	
	{
		StringBuffer* sb = new StringBuffer(source);
		std::cout << sb->toString().c_str() << std::endl;		
		sb->append('a');
		std::cout << sb->toString().c_str() << std::endl;		
		std::cout << sb->substring(1).c_str() << std::endl;		
		delete sb;
	}
}

void waitForString(String s, int timeout) {
	StringBuffer* sb = new StringBuffer();
	char ch;

	if (timeout == 0) {
		while (sb->toString().indexOf(s) == -1) {
			std::cin.get(ch);
			sb->append(ch);
		}
	} else { // timeout > 0
		auto startTime = std::chrono::high_resolution_clock::now();
		while (sb->toString().indexOf(s) == -1) {
			while (true) {
				auto endTime = std::chrono::high_resolution_clock::now();
				if (std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count() >= timeout) {
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}
			std::cin.get(ch);
			sb->append(ch);
		}
	}
#ifdef DEBUG
	std::cout << sb->toString().c_str() << std::endl;
#endif
	delete sb;
}

void waitForStringPatched(String s, int timeout) {
	StringBuffer* sb = new StringBuffer();
	int windowStart = -s.length();
	char ch;

	if (timeout == 0) {
#ifdef PATCH1
		while (windowStart < 0 || sb->substring(windowStart).indexOf(s) == -1) {
			std::cin.get(ch);
			sb->append(ch);
			windowStart++;
		}
#else
		while (windowStart < 0 || !sb->substring(windowStart).equals(s)) {
			std::cin.get(ch);
			sb->append(ch);
			windowStart++;
		}
#endif
	} else {  //  timeout > 0
		auto startTime = std::chrono::high_resolution_clock::now();
#ifdef PATCH1
		while (windowStart < 0 || sb->substring(windowStart).indexOf(s) == -1) {
			std::cin.get(ch);
			sb->append(ch);
			windowStart++;
		}
#else
		while (windowStart < 0 || !sb->substring(windowStart).equals(s)) {
			 while (true) {
				auto endTime = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count() >= timeout) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
            std::cin.get(ch);
            sb->append(ch);
			windowStart++;
		}
#endif
	}
#ifdef DEBUG
	std::cout << sb->toString().c_str() << std::endl;
#endif
	delete sb;
}

}


int main(int argc, char* argv[]) {
	using namespace apache34464;
	// testStringIndexOf();
	// testStringBuffer();

	const char* patternStr = argv[1];
#ifdef BUGGY
  	{
  		auto startTime = std::chrono::high_resolution_clock::now();
  		waitForString(String(patternStr), 0);
  		auto endTime = std::chrono::high_resolution_clock::now();
  		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << std::endl;
 	}
#else
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		waitForStringPatched(String(patternStr), 0);
		auto endTime = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() << std::endl;
	}
#endif
}

