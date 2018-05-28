#include <iostream>
#include "storage/sens.hpp"
int main(int argc, char** argv) {
	SensData sens(argv[1]);
	sens.GenImages(argv[2]);
	sens.GenMeta(argv[2]);
}
