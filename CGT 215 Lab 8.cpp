#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

const float KB_SPEED = 0.2;

void LoadTex(Texture& tex, string filename) {
	if (!tex.loadFromFile(filename)) {
		cout << "Can't Load" << filename << endl;
	}
}

void MoveCrossbow(PhysicsSprite& crossbow, int elapsedMS) {
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		Vector2f newPos(crossbow.getCenter());
		newPos.x = newPos.x + (KB_SPEED * elapsedMS);
		crossbow.setCenter(newPos);
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		Vector2f newPos(crossbow.getCenter());
		newPos.x = newPos.x - (KB_SPEED * elapsedMS);
		crossbow.setCenter(newPos);
	}
}

int main()
{
	RenderWindow window(VideoMode(800, 600), "Duck Hunting");
	World world(Vector2f(0, 0));
	int arrows(5);
	int score(0);

	PhysicsSprite& crossBow = *new PhysicsSprite();
	Texture cbowTex;
	LoadTex (cbowTex, "Lab08Images/crossbow.png");
	crossBow.setTexture(cbowTex);
	Vector2f sz = crossBow.getSize();
	crossBow.setCenter(Vector2f(400, 600 - (sz.y / 2)));

	PhysicsSprite arrow;
	Texture arrowTex;
	LaodTex(arrowTex, "Lab08Images/arrow.png");
	arrow.setTexture(arrowTex);
	bool drawingArrow(false);

	PhysicsRectangle top;
	top.setSize(Vector2f(800, 10));
	top.setCenter(Vector2f(400, 5));
	top.setStatic(true);
	world.AddPhysicsBody(top);
}