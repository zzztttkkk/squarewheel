#include <sw.hpp>
#include <list>

using namespace sw;

int main() {
	Vec<int> vec({1, 2, 3, 40});

	for (int i = 0; i < 100; ++i) {
		vec.push(i);
	}

	std::vector<int> stdvec({1, 2, 3, 4});
	for (int i = 0; i < 40; ++i) {
		stdvec.push_back(i);
	}

	vec.concat(stdvec.data(), stdvec.size());

	std::list<int> stdlst({1, 2, 45, 89});
	vec.concat(stdlst);

	sw::println("{}", vec.printable_string());
	vec.clear();
	sw::println("{}", vec.printable_string());
	return 0;
}