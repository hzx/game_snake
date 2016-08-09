#include <time.h>
#include <iostream>
// use this for Windows, Linux
/* #include <GL/glut.h> */
// use this for MaxOS
#include <GLUT/glut.h>


enum class SnakeDirection {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

struct SnakePiece {
  int x;
  int y;
};

const int ROWS = 30;
const int COLS = 20;
const int SCALE = 25;

const int WIDTH = SCALE * ROWS;
const int HEIGHT = SCALE * COLS;

const int SNAKE_DEFAULT_LENGTH = 4;
const int SNAKE_MAX_LENGTH = 100;
const int SNAKE_MOVE_DELTA = 1;
const int FRUIT_COUNT = 10;

int window = 0;

SnakeDirection snakeDirection = SnakeDirection::UP;
int snakeLength = SNAKE_DEFAULT_LENGTH;
SnakePiece snake[SNAKE_MAX_LENGTH];
SnakePiece fruits[FRUIT_COUNT];


void display();
void timer(int time);
void keyboard(int key, int a, int b);
void drawGrid();
void drawSnake();
void updateSnake();
void relocateFruit(SnakePiece& fruit);
void drawFruit(SnakePiece& fruit);


int main(int argc, const char **argv) {
  for (int i = 0; i < FRUIT_COUNT; ++i) {
    relocateFruit(fruits[i]);
  }

  snake[0].x = 10;
  snake[0].y = 10;
  for (int i = 1; i < SNAKE_DEFAULT_LENGTH; ++i) {
    snake[i].x = snake[i-1].x;
    snake[i].y = snake[i-1].y;
  }

  glutInit(&argc, const_cast<char**>(argv));
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  /* glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); */
  glutInitWindowSize(WIDTH, HEIGHT);
  window = glutCreateWindow("Test");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WIDTH, 0, HEIGHT);
  // for redraw, no needs right now, we use timer
  glutDisplayFunc(display);
  glutTimerFunc(50, timer, 0);
  glutSpecialFunc(keyboard);
  glutMainLoop();

  return 0;
}


void display() {
  /* glClearColor(0.0f, 0.0f, 0.0f, 1.0f); */
  /* glClearDepth(0.0f); */
  glClear(GL_COLOR_BUFFER_BIT);

  for (int i = 0; i < FRUIT_COUNT; ++i) {
    drawFruit(fruits[i]);
  }

  drawSnake();
  drawGrid();

  glFlush();
}


void timer(int time = 0) {
  updateSnake();
  display();
  glutTimerFunc(250, timer, 0);
}


void keyboard(int key, int a, int b) {
  switch (key) {
    case GLUT_KEY_F1:
      glutDestroyWindow(window);
      break;
    case GLUT_KEY_LEFT:
      snakeDirection = SnakeDirection::LEFT;
      break;
    case GLUT_KEY_UP:
      snakeDirection = SnakeDirection::UP;
      break;
    case GLUT_KEY_RIGHT:
      snakeDirection = SnakeDirection::RIGHT;
      break;
    case GLUT_KEY_DOWN:
      snakeDirection = SnakeDirection::DOWN;
      break;
    default:
      std::cout << "key: " << key << std::endl;
      break;
  }
}


void drawGrid() {
  glColor3f(0.0f, 1.0f, 0.0f);

  glBegin(GL_LINES);
    // vertical lines
    for (int x = 0; x < WIDTH; x += SCALE) {
      glVertex2f(x, 0); glVertex2f(x, HEIGHT);
    }
    // horizontal lines
    for (int y = 0; y < HEIGHT; y += SCALE) {
      glVertex2f(0, y); glVertex2f(WIDTH, y);
    }
  glEnd();
}


void drawSnake() {
  glColor3f(0.0f, 0.0f, 1.0f);

  for (int i = 0; i < snakeLength; ++i) {
    glRectf(snake[i].x * SCALE, snake[i].y * SCALE,
        (snake[i].x + 1) * SCALE, (snake[i].y + 1) * SCALE);
  }
}


void updateSnake() {
  // move snake head
  for (int i = snakeLength; i > 0; --i) {
    snake[i].x = snake[i-1].x;
    snake[i].y = snake[i-1].y;
  }

  // move snake
  switch (snakeDirection) {
    case SnakeDirection::UP:
      snake[0].y += SNAKE_MOVE_DELTA;
      break;
    case SnakeDirection::RIGHT:
      snake[0].x += SNAKE_MOVE_DELTA;
      break;
    case SnakeDirection::DOWN:
      snake[0].y -= SNAKE_MOVE_DELTA;
      break;
    case SnakeDirection::LEFT:
      snake[0].x -= SNAKE_MOVE_DELTA;
      break;
  }

  // edges collision
  if (snake[0].x > ROWS) snakeDirection = SnakeDirection::LEFT;
  if (snake[0].x < 0) snakeDirection = SnakeDirection::RIGHT;
  if (snake[0].y > COLS) snakeDirection = SnakeDirection::DOWN;
  if (snake[0].y < 0) snakeDirection = SnakeDirection::UP;

  // check snake collide with fruits
  for (int i = 0; i < FRUIT_COUNT; ++i) {
    if ((snake[0].x == fruits[i].x) and (snake[0].y == fruits[i].y)) {
      ++snakeLength;
      relocateFruit(fruits[i]);
    }
    // dont allow to grow more or end level here
    if (snakeLength == SNAKE_MAX_LENGTH) snakeLength = SNAKE_MAX_LENGTH - 1;
  }

  // check self collision, cut until collided piece
  for (int i = 1; i < snakeLength; ++i) {
    if (snake[0].x == snake[i].x and snake[0].y == snake[i].y) snakeLength = i;
  }
}


void relocateFruit(SnakePiece& fruit) {
  fruit.x = rand() % ROWS;
  fruit.y = rand() % COLS;
}


void drawFruit(SnakePiece& fruit) {
  glColor3f(0.0f, 1.0f, 0.0f);
  glRectf(fruit.x * SCALE, fruit.y * SCALE,
      (fruit.x + 1) * SCALE, (fruit.y + 1) * SCALE);
}
