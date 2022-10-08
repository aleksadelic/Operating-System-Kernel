#include <iostream.h>
#include "System.h"

int userMain(int argc, char* argv[]);

void main(int argc, char* argv[]) {

	init();

	int ret = userMain(argc, argv);

	restore();

	cout << "Happy end!\n";

}
