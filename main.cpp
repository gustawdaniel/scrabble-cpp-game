#include <ncurses.h>
#include <string>
#include <utility>

struct Pos {
    int x;
    int y;
};

struct Map;

struct Player {
public:
    std::string name;
    Pos pos{};
    char direction = 'R';

    explicit Player(std::string init_name) {
        name = std::move(init_name);
    }

    void init();

    void setPosition();
};


struct Map {
public:
    static const int size = 15;

    static char get_letter(int from_a) {
        return static_cast<char>(65 + from_a - 2);
    }

    static void init() {
        initscr();
        noecho(); // no print typed characters
        keypad(stdscr, TRUE); // allow for arrays

        for (int i = 2; i < size + 2; i++) {
            const char c[] = {get_letter(i), '\0'};
            mvprintw(0, i, "%s", c);
            mvprintw(size + 1, i, "%s", c);
        }
        for (int i = 1; i < size + 1; i++) {
            mvprintw(i, 0, "%2d", i);
            mvprintw(i, 0 + size + 2, "%d", i);
        }
    }

    static void showUserName(const Player *p) {
        mvprintw(0, size + 5, "Name: %s", p->name.c_str());
        mvprintw(p->pos.y, p->pos.x, "");
    }

    static void showUserPosition(const Player *p) {
        const char x[] = {get_letter(p->pos.x)};
        mvprintw(1, size + 5, "Pos : %s%d ", x, p->pos.y);
        mvprintw(p->pos.y, p->pos.x, "");
    }

    static void showUserDirection(const Player *p) {
        mvprintw(2, size + 5, "Dir : %s ", p->direction == 'R' ? "Right" : "Down");
        mvprintw(p->pos.y, p->pos.x, "");
    }
};


void Player::init() {
    pos.x = 2 + 7;
    pos.y = 1 + 7;
    mvprintw(pos.y, pos.x, "");
    Map::showUserName(this);
    Map::showUserPosition(this);
    Map::showUserDirection(this);
}

void Player::setPosition() {
    while (int c = getch()) {
        switch (c) {
            case KEY_LEFT: {
                pos.x--;
                break;
            }
            case KEY_RIGHT: {
                if (direction == 'R') {
                    pos.x++;
                } else {
                    direction = 'R';
                }
                break;
            }
            case KEY_UP: {
                pos.y--;
                break;
            }
            case KEY_DOWN: {
                if (direction == 'D') {
                    pos.y++;
                } else {
                    direction = 'D';
                }
                break;
            }
            default: {
            }
        }
        Map::showUserPosition(this);
        Map::showUserDirection(this);
        mvprintw(pos.y, pos.x, "");
    }
}


struct Game {
    static void init() {
        Map::init();

        auto *p1 = new Player("Player 1");
        p1->init();

        p1->setPosition();
    }

    static int end() {
        getch();
        endwin();
        return 0;
    }
};


int main() {
    Game::init();
    return Game::end();
}



