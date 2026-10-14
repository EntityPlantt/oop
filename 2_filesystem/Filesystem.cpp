#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

// Homework for 21.10.2026
// To complete the homework, it MUST be uploaded to your oop repo on Github!! 

// 0. Enforce that no file/directory with the same nameext can be created in the same dir.
// 1. When printing the directory, print the contents of all child directories!!!
// 2. Implement 'Up' function that return the parent directory of the current directory
//    If no parent is available, print '/(root)'
// 3. Implement "Find" that searches for a file or directory inside a specific directory.
//    The file/directory can be nested inside a subdirectory
// 4. DirectorySize function includes the size of child directories

struct File {
	string name;
	string extension;
	uint64_t byteSize;
};

inline constexpr string getFullFilename(const File &f) {
	return f.name + '.' + f.extension;
}

struct Directory {
	string name;
	vector<File*> files;
	vector<Directory*> dirs;
	// За 2
	Directory *up;
};

constexpr uint64_t getDirectorySize(const Directory &d) {
	uint64_t sz = 0;
	for (size_t i = 0; i < d.files.size(); i++) {
		sz += d.files[i]->byteSize;
	}
	for (size_t i = 0; i < d.dirs.size(); i++) {
		sz += getDirectorySize(*d.dirs[i]);
	}
	return sz;
}

// 0. Новиот метод за додавање датотеки во директориум
bool addFile(Directory &d, File &f) {
	for (size_t i = 0; i < d.files.size(); i++) {
		if (getFullFilename(*d.files[i]) == getFullFilename(f)) return false;
	}
	d.files.push_back(&f);
	return true;
}

// 0. Новиот метод за додавање директориуми во директориум
bool addDir(Directory &D, Directory &d) {
	for (size_t i = 0; i < D.dirs.size(); i++) {
		if (D.dirs[i]->name == d.name) return false;
	}
	D.dirs.push_back(&d);
	// 2
	d.up = &D;
	return true;
}

bool moveFile(const string &filename, Directory &from, Directory &to) {
	for (size_t i = 0; i < from.files.size(); i++) {
		if (filename == getFullFilename(*from.files[i])) {
			File *file = from.files[i];
			for (size_t j = i; j < from.files.size() - 1; j++) {
				from.files[j] = from.files[j + 1];
			}
			// Ако не може да се додаде фајлот во другата датотека, да се абортира сѐ (да не се загуби фајлот)
			if (addFile(to, *file)) {
				from.files.pop_back();
				return true;
			}
			return false;
		}
	}
	return false;
}

// 1. Имплементирано е рекурзивно принтање на директориуми
void printDir(const Directory &d, const string &padstr = "") {
	cout << padstr << "d: " << d.name << ": " << getDirectorySize(d) << '\n';
	for (size_t i = 0; i < d.dirs.size(); i++) {
		printDir(*d.dirs[i], padstr + "| ");
	}
	for (size_t i = 0; i < d.files.size(); i++) {
		cout << padstr << "| f: " << getFullFilename(*d.files[i]) << ": " << d.files[i]->byteSize << '\n';
	}
}

// 2. Up функција за директориуми (за датотеки не се бара)
inline constexpr Directory *Up(const Directory &d) {
	return d.up;
}

// 3. Пребарување
Directory *findDir(const Directory &d, const string &name, bool recursive = true) {
	// Давам предност на папките што се директно во папката, потоа ќе ги пребарува вгнездените
	for (size_t i = 0; i < d.dirs.size(); i++) {
		if (d.dirs[i]->name == name) return d.dirs[i];
	}
	if (!recursive) return nullptr;
	for (size_t i = 0; i < d.dirs.size(); i++) {
		if (const auto r = findDir(*d.dirs[i], name); r) return r;
	}
	return nullptr;
}

// 3. Пребарување
File *findFile(const Directory &d, const string &name, bool recursive = true) {
	for (size_t i = 0; i < d.files.size(); i++) {
		if (getFullFilename(*d.files[i]) == name) return d.files[i];
	}
	if (!recursive) return nullptr;
	for (size_t i = 0; i < d.dirs.size(); i++) {
		if (const auto r = findFile(*d.dirs[i], name); r) return r;
	}
	return nullptr;
}

// За принтање на тоа '/(root)'
inline constexpr string identifyDir(Directory *d) {
	if (d) return d->name;
	return "/(root)";
}

int main() {
	Directory my{.name = "documents"};
	
	File x{
		.name = "X",
		.extension = "txt",
		.byteSize = 1024
	}, y{
		.name = "Y",
		.extension = "jpeg",
		.byteSize = 56281022
	}, z{
		.name = "z",
		.extension = "mov",
		.byteSize = 1556281022
	};

	assert(addFile(my, x) == true);
	assert(addFile(my, y) == true);
	assert(addFile(my, x) == false);
	assert(addFile(my, z) == true);
	assert(addFile(my, z) == false);

	// Овдека треба да пазиме името да не се совпадне; ако не користиме подобра
	// структура (со std::map) за чување фајлови нема ефикасен начин да се проверува
	y.name = "YYY";

	Directory child{
		.name = "Child One"
	};

	assert(addDir(my, child) == true);
	assert(addDir(my, child) == false);
	
	assert(Up(child) == &my);
	assert(identifyDir(Up(child)) == "documents");
	assert(identifyDir(Up(my)) == "/(root)");

	printDir(my);

	moveFile(getFullFilename(z), my, child);
	cout << "AFTER MOVE\n";
	printDir(my);
	return 0;
}
