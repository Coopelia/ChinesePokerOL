#include"game.h"

Game::Game()
{
	this->app = new RenderWindow(VideoMode(WIDTH, HEIGHT), "ChinesePoker", Uint32(5));
	this->app->setFramerateLimit(60);
	this->start_scene.app = app;
	this->start_scene.Initial_window(app);
	this->start_scene.Initial_assets();
	this->game_scene = NULL;
	this->OnStartScene = true;
	this->OnPlayScene = false;
	this->isOvered = false;
	this->isOnWel = true;
	this->gameOver = 0;
	isLoading = false;
}

Game::~Game()
{
	delete this->app;
}

void Game::InitialGameScene()
{
	if (this->start_scene.next_scene == danji)
		this->game_scene = new GameScene();
	else
		this->game_scene = new GameSceneOL();
	this->game_scene->app = app;
	this->game_scene->Initial_window(app);
	this->game_scene->Initial_assets();
}

void Game::GameOver()
{
	if (gameOver == 1)
	{
		if (!isOvered)
		{
			game_scene->bgm.stop();
			isOvered = true;
		}
	}
	if (gameOver == 2)
	{
		if (!isOvered)
		{
			game_scene->bgm.stop();
			isOvered = true;
		}
	}
}

void Game::Update()
{
	if (OnStartScene)
	{
		if (start_scene.bool_list["isExit"])
		{
			OnStartScene = false;
			OnPlayScene = true;
			start_scene.bool_list["isExit"] = false;
			InitialGameScene();
			return;
		}
		if (!start_scene.bool_list["isRunning"])
		{
			if (game_scene != NULL)
				delete game_scene;
			start_scene.Start();
		}
		start_scene.Update();
	}
	if (OnPlayScene)
	{
		if (game_scene->bool_list["isExit"])
		{
			OnStartScene = true;
			OnPlayScene = false;
			game_scene->bool_list["isExit"] = false;
			return;
		}
		if (!game_scene->bool_list["isRunning"])
		{
			start_scene.SceneClose();
			game_scene->Start();
		}
		game_scene->Update();
	}
}

void Game::Input(Event& e)
{
	if (OnStartScene)
		start_scene.Input(e);
	else if (OnPlayScene)
		game_scene->Input(e);
	e.type = Event::Count;
}

void Game::Draw()
{
	if (OnStartScene)
		start_scene.Draw();
	else if (OnPlayScene)
		game_scene->Draw();
}

void Game::show_Wel(int i)
{
	std::stringstream ss;
	ss << "assets/image/wel/ddz-wel_" << i << ".png";
	tWel.loadFromFile(ss.str());
	sWel.setTexture(tWel);
	sWel.setPosition(0, 0);
	(*app).draw(sWel);
}

void Game::Run()
{
	int i = 1000;
	isOnWel = false;
	while ((*app).isOpen())
	{
		app->clear();
		Event e;
		(*app).pollEvent(e);
		if (e.type == Event::Closed)
			(*app).close();
		if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space)
			isOnWel = false;
		if (isOnWel)
		{
			show_Wel(i++);
			if (i == 1296)
				isOnWel = false;
		}
		if (!isOnWel)
		{
			Input(e);
			Update();
			Draw();
		}
		(*app).display();
	}
}

void Game::sender()
{
	while (true)
	{
		while (!q_sender.empty())
			connector.sendEvent();
	}
}

void Game::reciever()
{
	while (true)
		connector.receiveEvent();
}