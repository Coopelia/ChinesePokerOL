#include"Connection.h"
#include"Customor.h"

Connection::Connection()
{
	port_self = 5678;
	listener.listen(port_self);
	clientIdTurn = 1;
	stop = false;
}

void Connection::Stop()
{
	this->stop = true;
	::std::list<Customor*>::iterator itr;
	mt_c.lock();
	for (itr = clients.begin(); itr != clients.end();)
	{
		(*itr)->disconnect();
		itr = clients.erase(itr);
	}
	mt_c.unlock();
}

void Connection::recieve_new_cust()
{
	Customor* c = new Customor();
	if (listener.accept(c->getSocketIn()) == ::sf::Socket::Done)
	{
		::std::cout << "接收到一个新连接: " << c->getSocketIn().getRemoteAddress() << "\n";
		::sf::Packet p;
		c->getSocketIn().receive(p);
		int port;
		p >> port;
		c->connect(c->getSocketIn().getRemoteAddress(), port, clientIdTurn++);
		mt_c.lock();
		c->runThreads(*&c);
		clients.push_back(*&c);
		mt_c.unlock();
	}
}

void Connection::pth_recieve_new_cust(Connection* _this)
{
	while (!_this->stop)
	{
		_this->recieve_new_cust();
	}
}

void Connection::pth_update(Connection* _this)
{
	while (!_this->stop)
	{
		::std::list<Customor*>::iterator itr;
		mt_c.lock();
		for (itr = clients.begin(); itr != clients.end();)
		{
			if (!(*itr)->isConnected)
			{
				itr = clients.erase(itr);
				_this->clientIdTurn--;
			}
			else
				itr++;
		}
		mt_c.unlock();
	}
}

void Connection::runThreads(Connection* _this)
{
	::std::thread th1 = ::std::thread(Connection::pth_recieve_new_cust, _this);
	::std::thread th2 = ::std::thread(Connection::pth_update, _this);
	th1.detach();
	th2.detach();
}