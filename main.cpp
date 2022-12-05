#include <ncurses.h>

struct Map {
public:
    int size = 15;

    void init() {
        initscr();

        for (int i = 2; i < size+ 2; i++) {
            const char c[] = { static_cast<char>(65 + i - 2), '\0' };
            mvprintw(0,i, "%s", c);
            mvprintw(size+1,i, "%s", c);
        }
        for (int i = 1; i < size+ 1; i++) {
            mvprintw(i,0, "%2d", i);
            mvprintw(i,0+size+2, "%d", i);
        }
    }
};

struct Game {
    void init() {
        Map *map = new Map();
        map->init();
    }

    int end() {
        getch();
        endwin();
        return 0;
    }
};



int main() {
    Game *g = new Game();
    g->init();
    return g->end();
}



