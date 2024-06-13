#include "MyWindow.h"
#include "Player.h"
#include "Aliens.h"
#include "Bullet.h"
#include "Defense.h"
#include "Enums.h"

// TODO: dodati vrijeme kao u slideovima
//	- mislim da je ok, jedino provjeriti za laser kak se mice
// TODO: obrambeni objekti se uniste ako ih dotakne alien
//	- mislim da je ok, ali nije istestirano do kraja
//	- mozda moze biti malo blize ili ovisno ako je vec unistena obrana djelomicno
// TODO: u izbornik na pocetku / kraju dodati koliko bodova nosi koji alien
// TODO: usporediti sa igrom kako bi trebala izgledati
// TODO: sig mi ne treba includeat iostream, provjeriti ostale includeove jel mis svi trebaju po fileovima

class Game {
public:
	Game();
	~Game();

	MyWindow* getWindow();
	void processInput();
	void update();
	void render();
	void restartClock();
	Time elapsedTime();

private:
	Clock clock;
	Time time;
	Time time1;
	Time time2;
	Time time3;
	Time time4;
	Time time5;
	MyWindow w;
	Player p;
	Aliens a;
	Defense d;
	Bullet aBullet;
	Bullet pBullet;
	GameFlow play;
	Font font;
	Text menuText;
	Text pointsText;
	
	void setText(Text& text, int x, int y);
	int isGameOver();
	void drawPlay();
	void drawMenu();
	void processMenuAction();
	void handleKeyboardInput();
	void restart();
	void moveObjects();
};