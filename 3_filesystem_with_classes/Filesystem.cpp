// Домашната од час 2, но со класи
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

class Directory;

class File {
	string name;
	string extension;
	uint64_t byteSize;
	Directory *up;
public:
	static constexpr string STAYSAME = "<staysame>";
	File(const string &name, const string &ext, const uint64_t &byteSize): name(name), extension(ext), byteSize(byteSize) {
		if (name == STAYSAME || ext == STAYSAME) {
			throw logic_error(getFullFilename() + ": Can't have name or extension be a reserved keyword");
		}
	}
	string getFullFilename() const {
		return name + '.' + extension;
	}
	inline uint64_t size() const {
		return byteSize;
	}
	inline const string &getName() const {
		return name;
	}
	inline const string &getExtension() const {
		return extension;
	}
	// Нови функции поради рестрикции
	bool rename(string name = STAYSAME, string ext = STAYSAME);
	friend class Directory;
};

class Directory {
	string name;
	vector<File*> files;
	vector<Directory*> dirs;
	Directory *up;
public:
	Directory(const string &name): name(name) {}
	uint64_t size() const {
		uint64_t sz = 0;
		for (auto i : files) sz += i->size();
		for (auto i : dirs) sz += i->size();
		return sz;
	}
	string getName() const {
		return name;
	}
	bool add(File &f) {
		for (auto i : files) if (i->getFullFilename() == f.getFullFilename()) return false;
		files.push_back(&f);
		f.up = this;
		return true;
	}
	bool add(Directory &d) {
		for (auto i : dirs) if (i->name == d.name) return false;
		dirs.push_back(&d);
		d.up = this;
		return true;
	}
	File* rm(const string &name) {
		for (size_t i = 0; i < files.size(); i++) {
			if (files[i]->getFullFilename() == name) {
				File* f = files[i];
				f->up = nullptr;
				files[i] = files.back();
				files.pop_back();
				return f;
			}
		}
		return nullptr;
	}
	Directory* rmdir(const string &name) {
		for (size_t i = 0; i < dirs.size(); i++) {
			if (dirs[i]->name == name) {
				Directory* f = dirs[i];
				f->up = nullptr;
				dirs[i] = dirs.back();
				dirs.pop_back();
				return f;
			}
		}
		return nullptr;
	}
	bool move(const string &name, Directory &to) {
		auto f = rm(name);
		if (!f) return false;
		if (to.add(*f)) return true;
		// Врати го назад фајлот ако не успееш да го додадеш на новото место
		add(*f);
		return false;
	}
	void print(const string &padstr = "") const {
		cout << padstr << "d: " << name << ": " << size() << '\n';
		for (auto d : dirs) d->print(padstr + "| ");
		for (auto f : files) cout << padstr << "| f: " << f->getFullFilename() << ": " << f->size() << '\n';
	}
	Directory *Up() const {
		return up;
	}
	Directory *findDir(const string &name, bool recursive = true) const {
		for (auto d : dirs) if (d->getName() == name) return d;
		if (!recursive) return nullptr;
		for (auto d : dirs) if (auto r = d->findDir(name)) return r;
		return nullptr;
	}
	File *findFile(const string &name, bool recursive = true) const {
		for (auto f : files) if (f->getFullFilename() == name) return f;
		if (!recursive) return nullptr;
		for (auto d : dirs) if (auto r = d->findFile(name)) return r;
		return nullptr;
	}
	// Ова мора да е static бидејќи не е баш паметно да се дереференца nullptr
	static inline constexpr string identify(Directory *d) {
		if (d) return d->getName();
		return "/(root)";
	}
};

// Ја дефинирам надвор бидејќи треба да пристапи кон функции што уште не постојат горе
bool File::rename(string name, string ext) {
	auto dir = up; // up ќе се пребрише од dir->rm
	if (dir) dir->rm(getFullFilename());
	if (name != STAYSAME) swap(this->name, name);
	if (ext != STAYSAME) swap(this->extension, ext);
	if (!dir || dir->add(*this)) return true;
	if (name != STAYSAME) swap(this->name, name);
	if (ext != STAYSAME) swap(this->extension, ext);
	dir->add(*this);
	return false;
}

int main() {
	Directory my("documents");
	
	File x("X", "txt", 1024),
		y("Y", "jpeg", 56281022),
		z("z", "mov", 1556281022);

	assert(my.add(x) == true);
	assert(my.add(y) == true);
	assert(my.add(x) == false);
	assert(my.add(z) == true);
	assert(my.add(z) == false);

	assert(y.rename("YYY") == true);
	assert(z.rename("X", "txt") == false);
	assert(z.getFullFilename() == "z.mov");

	Directory child("Child One");

	assert(my.add(child) == true);
	assert(my.add(child) == false);
	
	assert(child.Up() == &my);
	assert(Directory::identify(child.Up()) == "documents");
	assert(Directory::identify(my.Up()) == "/(root)");

	my.print();

	my.move(z.getFullFilename(), child);
	cout << "AFTER MOVE\n";
	my.print();
	return 0;
}
