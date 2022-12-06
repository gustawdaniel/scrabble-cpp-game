#include <ncurses.h>
#include <string>
#include <utility>
#include <ctime>

struct Pos {
    int x;
    int y;
};

struct Map;

struct LettersSet {
    int size = 100;

    char letters[100] = {
            'k',
            'j',
            'x',
            'q',
            'z',
            'b', 'b',
            'c', 'c',
            'm', 'm',
            'p', 'p',
            'f', 'f',
            'h', 'h',
            'v', 'v',
            'w', 'w',
            'y', 'y',
            'g', 'g', 'g',
            'l', 'l', 'l', 'l',
            's', 's', 's', 's',
            'u', 'u', 'u', 'u',
            'd', 'd', 'd', 'd',
            'n', 'n', 'n', 'n', 'n', 'n',
            'r', 'r', 'r', 'r', 'r', 'r',
            't', 't', 't', 't', 't', 't',
            'o', 'o', 'o', 'o', 'o', 'o', 'o', 'o',
            'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
            'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i',
            'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e',
            '_', '_'
    };

    char getRandomLetter() {
        if(!size) return '#';
        int index = rand() % size;
        char c = letters[index];

        for(int i=index; i<size; i++)
            letters[i] = letters[i + 1];

        size--;

        return c;
    }
};

struct Player {
public:
    std::string name;
    Pos pos{};
    char direction = 'R';
    char letters[8];

    explicit Player(std::string init_name) {
        name = std::move(init_name);
    }

    void init(LettersSet *ls);

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

    static void showUserLetters(const Player *p) {
        mvprintw(3, size + 5, "Lett: %s ", p->letters);
        mvprintw(p->pos.y, p->pos.x, "");
    }

    static void showLeftLetters(const Player *p, const LettersSet *ls) {
        mvprintw(4, size + 5, "Left: %d   ", ls->size);
        mvprintw(p->pos.y, p->pos.x, "");
    }
};


void Player::init(LettersSet *ls) {
    pos.x = 2 + 7;
    pos.y = 1 + 7;
    for (char &letter: letters) {
        letter = ls->getRandomLetter();
    }
    mvprintw(pos.y, pos.x, "");
    Map::showUserName(this);
    Map::showUserPosition(this);
    Map::showUserDirection(this);
    Map::showUserLetters(this);
    Map::showLeftLetters(this, ls);
}

void Player::setPosition() {
    while (int c = getch()) {
        if(c >= 97 && c <= 122) {
            mvprintw(pos.y, pos.x, "%c", c);
            if(direction == 'R') {
                pos.x++;
            } else {
                pos.y++;
            }
        } else {
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
        }
        Map::showUserPosition(this);
        Map::showUserDirection(this);
        mvprintw(pos.y, pos.x, "");
    }
}

struct Game {
    static void init() {
        srand(time(nullptr));

        auto *ls = new LettersSet();

        Map::init();

        auto *p1 = new Player("Player 1");
        p1->init(ls);

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



