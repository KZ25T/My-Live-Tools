#include "loadconfig.hpp"

int main(int argc, const char* argv[]) {
	LoadConfig ld;
	if (ld.success()) {
		printf("founded .live!");
		int val;
		scanf("%d", &val);
		printf("%d", val);
	}
	return 0;
}
