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
	LoadTex(cbowTex, "Images/crossbow.png");
	crossBow.setTexture(cbowTex);
	Vector2f sz = crossBow.getSize();
	crossBow.setCenter(Vector2f(400, 600 - (sz.y / 2)));

	PhysicsSprite arrow;
	Texture arrowTex;
	LoadTex(arrowTex, "Images/arrow.png");
	arrow.setTexture(arrowTex);
	bool drawingArrow(false);

	PhysicsRectangle top;
	top.setSize(Vector2f(800, 10));
	top.setCenter(Vector2f(400, 5));
	top.setStatic(true);
	world.AddPhysicsBody(top);

	PhysicsRectangle left; left.setSize(Vector2f(10, 600));
	left.setSize(Vector2f(10, 600));
	left.setCenter(Vector2f(5, 300));
	left.setStatic(true);
	world.AddPhysicsBody(left);

	PhysicsRectangle right;
	right.setSize(Vector2f(10, 600));
	right.setCenter(Vector2f(795, 300));
	right.setStatic(true);
	world.AddPhysicsBody(right);

	Texture redTex;
	LoadTex(redTex, "Images/Digital_Duck.jpg");
	PhysicsShapeList<PhysicsSprite> ducks;

	top.onCollision = [&drawingArrow, &world, &arrow]
	(PhysicsBodyCollisionResult result) {
		drawingArrow = false;
		world.RemovePhysicsBody(arrow);
	};

	Text scoreText;
	Font font;
	if (!font.loadFromFile("Images/8bit.ttf")) {
		cout << "Can't load font from Images/8bit.ttf" << endl;
		exit(1);
	}
	scoreText.setFont(font);
	Text arrowCountText;
	arrowCountText.setFont(font);

	Clock clock;
	Time lastTime(clock.getElapsedTime());
	Time currentTime(lastTime);
	long deltaDuck = (0);

	while ((arrows > 0) || drawingArrow) {
		currentTime = clock.getElapsedTime();
		Time deltaTime = currentTime - lastTime;
		long deltaMS = deltaTime.asMilliseconds();
		if (deltaDuck > 590) {
			deltaDuck = 0;
			world.UpdatePhysics(deltaDuck);
			PhysicsSprite& duck = ducks.Create();
			duck.setTexture(redTex);
			Vector2f sz = duck.getSize();
			duck.setCenter(Vector2f(10, 20 + (sz.y / 2)));
			duck.setVelocity(Vector2f(0.25, 0));
			world.AddPhysicsBody(duck);
			duck.onCollision = [&drawingArrow, &world, &arrow, &duck, &ducks, &score, &right]
			(PhysicsBodyCollisionResult result) {
				if (result.object2 == arrow) {
					drawingArrow = false;
					world.RemovePhysicsBody(arrow);
					world.RemovePhysicsBody(duck);
					ducks.QueueRemove(duck);
					score += 10;
				}
				if (result.object2 == right) {
					world.RemovePhysicsBody(duck);
					ducks.QueueRemove(duck);
				}
			};
		}

		if (deltaMS > 9) {
			deltaDuck = deltaMS;
			lastTime = currentTime;
			world.UpdatePhysics(deltaMS);
			MoveCrossbow(crossBow, deltaMS);
			if (Keyboard::isKeyPressed(Keyboard::Space) && !drawingArrow) {
				drawingArrow = true;
				arrow.setCenter(crossBow.getCenter());
				arrow.setVelocity(Vector2f(0, -1));
				world.AddPhysicsBody(arrow);
				arrows -= 1;
			}

			window.clear();
			if (drawingArrow) {
				window.draw(arrow);
			}
			for (PhysicsShape& duck : ducks) {
				window.draw((PhysicsSprite&)duck);
			}

			window.draw(crossBow);
			scoreText.setString(to_string(score));
			FloatRect textBounds = scoreText.getGlobalBounds();
			scoreText.setPosition(Vector2f(790 - textBounds.width, 590 - textBounds.height));
			window.draw(scoreText);
			arrowCountText.setString(to_string(arrows));
			textBounds = arrowCountText.getGlobalBounds();
			arrowCountText.setPosition(Vector2f(10, 590 - textBounds.height));
			window.draw(arrowCountText);

			window.display();
			ducks.DoRemovals();
		}
	}
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("GAME OVER");
	FloatRect textBounds = gameOverText.getGlobalBounds();
	gameOverText.setPosition(Vector2f(400 - (textBounds.width / 2), 300 - (textBounds.height / 2)));
	window.draw(gameOverText);

	window.display();
	bool LeaveVar(true);
	do {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			LeaveVar = false;
		}
	} while (LeaveVar);
}