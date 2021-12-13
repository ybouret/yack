#include "yack/system/env.hpp"
#include "yack/associative/hash/map.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(env)
{
	hash_map<string, string> db;
	environment::show();
	environment::get(db);

}
YACK_UDONE()
