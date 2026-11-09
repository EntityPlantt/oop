#include <iostream>
#include <format>
#include <cassert>
using namespace std;

class myvector {
	size_t arrsz, arrcapa;
	int *arr;
public:
	myvector(const size_t size = 0, const int val = 0): arrsz(size), arrcapa(size) {
		size_t rep = 0;
		while (arrcapa > 1) {
			arrcapa >>= 1;
			++rep;
		}
		if (!rep) rep = 1; // min size: 4
		arrcapa = 1 << 1 + rep;
		// сега capacity = min power of 2 > size
		arr = new int[arrcapa];
		for (int i = 0; i < size; i++) arr[i] = val;
	}
	myvector(const myvector &v): arrsz(v.arrsz), arrcapa(v.arrcapa) {
		arr = new int[arrcapa];
		for (int i = 0; i < arrsz; i++) arr[i] = v.arr[i];
	}
	~myvector() {
		delete[] arr;
	}
	size_t size() const {
		return arrsz;
	}
	bool empty() const {
		return !arrsz;
	}
	size_t capacity() const {
		return arrcapa;
	}
	void reserve(const size_t newcapa) {
		if (newcapa == arrcapa) return; // да не прави беспотребни copies ако некој се одлучи да го користи кодов во production :D
		if (newcapa < arrsz) throw length_error(format("Can't set new capacity ({}) smaller than size of array ({})", newcapa, arrsz));
		int *a2 = new int[newcapa];
		for (size_t i = 0; i < arrsz; i++) a2[i] = arr[i];
		delete[] arr;
		arr = a2;
		arrcapa = newcapa;
	}
	void resize(const size_t size, const int val = 0) {
		int *a2 = new int[size];
		for (size_t i = 0; i < size && i < arrsz; i++) a2[i] = arr[i];
		delete[] arr;
		for (size_t i = arrsz; i < size; i++) a2[i] = val;
		arr = a2;
		arrsz = arrcapa = size;
	}
	inline void shrink_to_fit() {
		reserve(arrsz);
	}
	inline void push(const int i) {
		if (arrsz == arrcapa) reserve(arrcapa << 1);
		arr[arrsz++] = i;
	}
	int &back() {
		if (empty()) throw out_of_range("Cannot take last element of vector as it is empty");
		return arr[arrsz - 1];
	}
	int &at(const size_t i) {
		if (i < arrsz) return arr[i];
		throw out_of_range(format("Index {} greater than size of vector ({})", i, arrsz));
	}
	inline bool pop() {
		if (empty()) return false;
		--arrsz;
		return true;
	}
	void print() const {
		cout << "vector(" << arrsz << '/' << arrcapa << "): [";
		if (arrsz) {
			cout << arr[0];
			for (int i = 1; i < arrsz; i++) cout << ", " << arr[i];
		}
		cout << ']' << endl;
	}
};

int main() {
	myvector v1;
	myvector v2(10);
	myvector v3(5, 5);
	v1.print(); // vector(0/4): []
	v2.print(); // vector(10/16): [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	v3.print(); // vector(5/8): [5, 5, 5, 5, 5]
	v1.push(4);
	v1.print(); // vector(1/4): [4]
	for (int i = 0; i < 3; i++) v1.push(2);
	v1.print(); // vector(4/4): [4, 2, 2, 2]
	assert(v1.capacity() == 4);
	v1.at(1) = 5;
	assert(v1.pop() == true);
	assert(v1.pop() == true);
	assert(v1.back() == 5);
	assert(v1.pop() == true);
	assert(v1.back() == 4);
	assert(v1.pop() == true);
	assert(v1.pop() == false);
	v3.reserve(50000);
	for (int i = 0; i < 59001; i++) v3.push(i);
	assert(v3.size() == 59006 && v3.capacity() == 100000);
	v3.shrink_to_fit();
	assert(v3.at(4) == 5);
	assert(v3.at(24059) == 24054);
	assert(v3.capacity() == 59006);
	myvector v4 = v2;
	v4.push(123);
	v4.print(); // vector(11/16): [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 123]
	v2.print(); // vector(10/16): [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
}
