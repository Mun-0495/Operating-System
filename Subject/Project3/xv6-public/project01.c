#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char* argv[]) {
	int sid = 2020028377;
	printf(1, "My Student id is %d\n", sid);
	printf(1, "My pid is %d\n", getpid());
	printf(1, "My gpid is %d\n", gpid());
	exit();
}
