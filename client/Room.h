class Room
{
public:
	Room();
	~Room();
	void setRest(int rest);
	void setState(bool isPlaying);
	void setID(int id);
	int getRest();
	int getState();
	int getID();
private:
	int ID;
	int rest;//∑øº‰ £”‡Œª÷√(max:3,min:1)
	bool isPlaying;
};