#include"game.h"

void gameRun(Game* game)
{
	if (connector.connect())
		::std::cout << "�����ӵ�������\n";
	else
		::std::cout << "���ӷ�����ʧ��\n";
	game = new Game();
	game->Run();
}

int main()
{
	::std::thread th_1 = ::std::thread(LoadAnim);
	Game* game;
	::std::thread th_2 = ::std::thread(gameRun, game);
	::std::thread th_3 = ::std::thread(Game::reciever);
	::std::thread th_4 = ::std::thread(Game::sender);
	th_1.join();
	th_2.join();
	th_3.join();
	th_4.join();

	delete game;
	return 0;
}