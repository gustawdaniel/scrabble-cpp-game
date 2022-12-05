#include <ncurses.h>

struct Map {
public:
    int size = 15;

    void init() const {
        initscr();
        noecho(); // no print typed characters
        keypad(stdscr, TRUE); // allow for arrays

        for (int i = 2; i < size + 2; i++) {
            const char c[] = {static_cast<char>(65 + i - 2), '\0'};
            mvprintw(0, i, "%s", c);
            mvprintw(size + 1, i, "%s", c);
        }
        for (int i = 1; i < size + 1; i++) {
            mvprintw(i, 0, "%2d", i);
            mvprintw(i, 0 + size + 2, "%d", i);
        }
    }
};

struct Pos {
    int x;
    int y;
};

struct Player {
    Pos pos;

    void init() {
        pos.x = 2;
        pos.y = 1;
        mvprintw(pos.y, pos.x, "");
    }

    void setPosition() {
        while (int c = getch()) {
            switch (c) {
                case KEY_LEFT: {
                    pos.x--;
                    break;
                }
                case KEY_RIGHT: {
                    pos.x++;
                    break;
                }
                case KEY_UP: {
                    pos.y--;
                    break;
                }
                case KEY_DOWN: {
                    pos.y++;
                    break;
                }
                default: {}
            }
            mvprintw(pos.y, pos.x, "");
        }
    }
};

struct Game {
    static void init() {
        Map *map = new Map();
        map->init();

        auto *p1 = new Player();
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



