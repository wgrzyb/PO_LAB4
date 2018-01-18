#include "Run.h"

int main(int argc, char* argv[]) {
	Run *run = new Run("orcl", "c##PO_LAB4", "PO_LAB4");
	run->menu();
	delete run;
	system("pause");
	EXIT_SUCCESS;
}