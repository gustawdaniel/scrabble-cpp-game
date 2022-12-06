#include <ncurses.h>
#include <string>
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
        if (!size) return '#';
        int index = rand() % size;
        char c = letters[index];

        for (int i = index; i < size; i++)
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
    int lettersSize = 8;
    char letters[8]{};

    explicit Player(std::string init_name) {
        name = std::move(init_name);
    }

    void init(LettersSet *ls);
    void display(LettersSet *ls);

    int turn(); // 0 - ok, 1 - fail
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
        const char x = get_letter(p->pos.x);
        mvprintw(1, size + 5, "Pos : %c%d ", x, p->pos.y);
        mvprintw(p->pos.y, p->pos.x, "");
    }

    static void showUserDirection(const Player *p) {
        mvprintw(2, size + 5, "Dir : %s ", p->direction == 'R' ? "Right" : "Down");
        mvprintw(p->pos.y, p->pos.x, "");
    }

    static void showUserLetters(const Player *p) {
        mvprintw(3, size + 5, "Lett: ");
        for (int i = 0; i < 8; i++) {
            mvprintw(3, size + 5 + 6 + i, "%c ", i < p->lettersSize ? p->letters[i] : ' ');
        }
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
    this->display(ls);
}

void Player::display(LettersSet *ls) {
    Map::showUserName(this);
    Map::showUserPosition(this);
    Map::showUserDirection(this);
    Map::showUserLetters(this);
    Map::showLeftLetters(this, ls);
}

int indexOf(const char *arr, int len, char target) {
    int dashIndex = -1;
    for (int i = 0; i < len; i++) {
        if (arr[i] == target)
            return i;
        if (arr[i] == '_')
            dashIndex = i;
    }
    if (dashIndex != -1)
        return dashIndex;
    return -1;
}

int Player::turn() {
    while (int c = getch()) {
        if (c >= 97 && c <= 122) {
            int charPos = indexOf(letters, lettersSize, (char) c);
            if (charPos != -1) {
                mvprintw(pos.y, pos.x, "%c", c);
                if (direction == 'R') {
                    pos.x++;
                } else {
                    pos.y++;
                }
                for (int i = charPos; i < lettersSize; i++)
                    letters[i] = letters[i + 1];

                lettersSize--;
            }
        } else {
            switch (c) {
                case 10: {
                    return 0;
                }
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
        Map::showUserLetters(this);

        mvprintw(pos.y, pos.x, "");
    }
}

struct Game {
    int playersCount = 2;

    Player players[4] = {
            Player("Player 1"),
            Player("Player 2"),
            Player("Player 3"),
            Player("Player 4"),
    };

    void init() {
        srand(time(nullptr));

        auto *ls = new LettersSet();

        Map::init();

        for (int i = 0; i < playersCount; i++) {
            players[i].init(ls);
        }

        while (true) {
            for (int i = 0; i < playersCount; i++) {

                for (int j=players[i].lettersSize; j<8; j++) {
                    char letter = ls->getRandomLetter();
                    if(letter == '#') break;
                    players[i].letters[j] = letter;
                    players[i].lettersSize++;
                }

                players[i].display(ls);
//                printw("i = %d", i);
                players[i].turn();
            }
        }
    }

    int end() {
        getch();
        endwin();
        return 0;
    }
};


int main() {
    Game* g = new Game();
    g->init();
    return g->end();
}



