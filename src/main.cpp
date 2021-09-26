#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <random>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FRAME_RATE 60U

#define Move_Speed 10.0f
#define FONT "consolaz.ttf"

static bool gameStarted = false;
static sf::Font font;
static sf::Text gameText;
static sf::Vector2f ballDirection;
static bool gameOver = false;

std::default_random_engine device;
std::uniform_real_distribution<float> dist(-1, 1);
auto randomPos = std::bind(dist, device);

bool
compareFloats(float a, float b, float epsilon = 0.01) {
  if (std::fabs(a - b) < epsilon)
    return true;
  return false;
}

void initWindow(sf::RenderWindow &window, const char *title,
                sf::Uint32 style = 7U);
bool initResources();

void initRect(sf::RectangleShape &rect, const sf::Vector2f &&position,
              const sf::Color &color = sf::Color::White, const float rot = 0);

void initBall(sf::CircleShape &circle, const sf::Vector2f &&position,
              const sf::Color &color = sf::Color::Red,
              const size_t &&segments = 15);
void initGame();

void moveRect(sf::RectangleShape &rect, const sf::Vector2f &&direction);
void checkBallCollision(sf::CircleShape &circle, sf::RectangleShape &left,
                        sf::RectangleShape &right);
void moveBallRandomPos();

void endGame();

bool
initResources() {
  if (!font.loadFromFile(FONT)) {
    std::cerr << "Failed to load font, exiting" << std::endl;
    return false;
  }
  gameText.setFont(font);
  gameText.setFillColor(sf::Color::White);
  gameText.setString(L"W / S to Move left side\n"
                     L"Up / Down to move right side\n "
                     L"Space to start game");
  gameText.setPosition(
      WINDOW_WIDTH / 2.f - gameText.getGlobalBounds().width / 2, 10);
  return true;
}

void
initRect(sf::RectangleShape &rect, const sf::Vector2f &&position,
         const sf::Color &color, const float rot) {
  rect.setOrigin(rect.getSize() * 0.5f);
  rect.setPosition(position);
  rect.setFillColor(color);
  rect.setRotation(rot);
}

void
initBall(sf::CircleShape &circle, const sf::Vector2f &&position,
         const sf::Color &color, const size_t &&segments) {
  circle.setPointCount(segments);
  circle.setOrigin(sf::Vector2f(1, 1) * circle.getRadius());
  circle.setFillColor(color);
  circle.setPosition(position);
}

void
initWindow(sf::RenderWindow &window, const char *title, sf::Uint32 style) {
  window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), title, style);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(FRAME_RATE);
}

void
moveRect(sf::RectangleShape &rect, const sf::Vector2f &&direction) {
  auto magnitude = (float)std::sqrt(direction.x * direction.x
                                    + direction.y * direction.y);
  auto normalizedDir
      = sf::Vector2f(direction.x / magnitude, direction.y / magnitude);
  auto movement = normalizedDir * Move_Speed;
  auto newPosY = (rect.getPosition() + movement).y;
  if (newPosY - rect.getOrigin().y > 0
      && newPosY + rect.getOrigin().y < WINDOW_HEIGHT) {
    rect.move(movement);
  }
}

void
checkBallCollision(sf::CircleShape &circle, sf::RectangleShape &left,
                   sf::RectangleShape &right) {
  auto radius = circle.getRadius();
  auto position = circle.getPosition();
  if (position.y - radius <= 0 || position.y + radius >= WINDOW_HEIGHT) {
    ballDirection.y = -ballDirection.y;
  }
  if (circle.getGlobalBounds().intersects(left.getGlobalBounds())
      || circle.getGlobalBounds().intersects(right.getGlobalBounds())) {
    ballDirection.x = -ballDirection.x;
  } else if (position.x - radius <= 0 || position.x + radius >= WINDOW_WIDTH) {
    endGame();
  }
}

void
moveBallRandomPos() {

  float randomX = randomPos();
  float randomY = randomPos() * 0.5f;
  float magnitude = std::sqrt(randomX * randomX + randomY * randomY);
  ballDirection = sf::Vector2f(randomX / magnitude, randomY / magnitude);
}

void
initGame() {
  if (!gameStarted && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    gameStarted = true;
    gameText.setString(L"");
    moveBallRandomPos();
  }
}

void
moveBall(sf::CircleShape &circle) {
  circle.move(ballDirection * Move_Speed);
}

void
handleRectInput(sf::RectangleShape &leftRect, sf::RectangleShape &rightRect) {
  if (!gameStarted)
    return;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    moveRect(leftRect, sf::Vector2f(0, 1.f));
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    moveRect(leftRect, sf::Vector2f(0, -1.f));
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    moveRect(rightRect, sf::Vector2f(0, 1.f));
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    moveRect(rightRect, sf::Vector2f(0, -1.f));
}

void
endGame() {
  gameOver = true;
  if (ballDirection.x < 0)
    gameText.setString(L"Right Wins");
  else
    gameText.setString(L"Left wins");
  gameText.setPosition(
      WINDOW_WIDTH / 2.f - gameText.getGlobalBounds().width / 2, 10);
}

int
main(int argc, char **argv) {
  sf::RenderWindow window;
  initWindow(window, "S::Pong", sf::Style::Default ^ sf::Style::Resize);

  if (!initResources())
    return (EXIT_FAILURE);

  const float aspectRatio = (float)WINDOW_WIDTH / WINDOW_HEIGHT;

  float rectX = 20, rectY = 80;
  sf::Vector2f rectSize(rectX, rectY);

  sf::RectangleShape left(rectSize * aspectRatio);
  initRect(left, sf::Vector2f(0, WINDOW_HEIGHT / 2.f));

  sf::RectangleShape right(rectSize * aspectRatio);
  initRect(right, sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2.f));

  float ballRadius = 8.0f * aspectRatio;
  size_t ballVertCount = 15UL;
  sf::CircleShape ball(ballRadius, ballVertCount);
  initBall(ball, sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f),
           sf::Color::White, size_t(ballVertCount));

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (gameOver) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        gameOver = false;
        gameStarted = false;
        initRect(left, sf::Vector2f(0, WINDOW_HEIGHT / 2.f));
        initRect(right, sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2.f));
        initBall(ball, sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f),
                 sf::Color::White, size_t(ballVertCount));
      }

    } else {
      initGame();
      moveBall(ball);
      checkBallCollision(ball, left, right);
      handleRectInput(left, right);
    }
    window.clear();
    window.draw(left);
    window.draw(right);
    window.draw(ball);
    window.draw(gameText);
    window.display();
  }

  return 0;
}
