//
// Created by ztk on 2022/8/23.
//

#include <sw.hpp>

using namespace sw;

int main() {
	SimpleBitmap bm;

	bm.add(1);
	bm.add(2);
	println("{} {}", bm.has(1), bm.has(0));
	bm.del(2);
	println("{}", bm.has(2));

	return 0;
}