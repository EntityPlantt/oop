#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Homework for 21.10.2026
// To complete the homework, it MUST be uploaded to your oop repo on Github!! 

// 0. Enforce that no file/directory with the same nameext can be created in the same dir.
// 1. When printing the directory, print the contents of all child directories!!!
// 2. Implement 'Up' function that return the parent directory of the current directory
//    If no parent is available, print '/(root)'
// 3. Implement "Find" that searches for a file or directory inside a specific directory.
//    The file/directory can be nested inside a subdirectory
// 4. DirectorySize function includes the size of child directoriesg



using namespace std;

struct File {
	string name;
	string extension;
	uint64_t byteSize;
};

string getFullFilename(File &f) { return f.name + '.' + f.extension; }

struct Directory {
	string name;
	vector<File *> files;
	vector<Directory *> dirs;
};

uint64_t getDirectorySize(Directory &d) {
	uint64_t sz = 0;
	for (size_t i = 0; i < d.files.size(); i++) {
		sz += d.files[i]->byteSize;
	}

	return sz;
}

bool moveFile(const string &filename, Directory &from, Directory &to) {
	File *file = nullptr;
	for (size_t i = 0; i < from.files.size(); i++) {
		if (filename == getFullFilename(*from.files[i])) {
			file = from.files[i];
			for (size_t j = i; j < from.files.size() - 1; j++) {
				from.files[j] = from.files[j + 1];
			}
			from.files.pop_back();

			to.files.push_back(file);
			return true;
		}
	}
	return false;
}

void printDir(Directory &d) {
	std::cout << "d: " << d.name << '\n';
	for (size_t i = 0; i < d.dirs.size(); i++) {
		std::cout << " d " << d.dirs[i]->name << '\n';
	}
	for (size_t i = 0; i < d.files.size(); i++) {
		std::cout << " f " << getFullFilename(*d.files[i]) << '\n';
	}
	std::cout << "Size: " << getDirectorySize(d) << " B\n";
}

int main() {
	Directory my;
	my.name = "documents";
	File x, y, z;
	x.name = "X";
	x.extension = "txt";
	x.byteSize = 1024;

	y.name = "Y";
	y.extension = "jpeg";
	y.byteSize = 56281022;

	z.name = "z";
	z.extension = "mov";
	z.byteSize = 1556281022;

	my.files.push_back(&x);
	my.files.push_back(&y);
	my.files.push_back(&z);

	y.name = "YYY";

	Directory child;
	child.name = "Child One";

	my.dirs.push_back(&child);

	printDir(my);

	moveFile(getFullFilename(z), my, child);
	cout << "AFTER MOVE\n";
	printDir(my);

	printDir(child);
	return 0;
}
