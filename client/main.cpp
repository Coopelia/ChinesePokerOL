#include"game.h"

void run()
{
	srand(time(0));
	Game game;
	game.Run();
}

int main()
{
	::std::thread th_1 = ::std::thread(LoadAnim);
	::std::thread th_2 = ::std::thread(run);
	th_1.join();
	th_2.join();

	return 0;
}