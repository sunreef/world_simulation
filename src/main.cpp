#include <iostream>

#include "world.h"

int main(int argc, char** argv) {
	World w;

	for (int i = 0; i < 1000; i++) {
		w.update();
		w.showWorldState();
	}
	return 0;
}