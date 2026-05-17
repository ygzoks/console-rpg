#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class UI {
public:
    static void printHP(int hp, int max) {
        if (hp > max * 0.6) cout << "\033[32m";
        else if (hp > max * 0.3) cout << "\033[33m";
        else cout << "\033[31m";
        cout << hp << "/" << max << "\033[0m";
    }

    static void printStamina(int stamina) {
        if (stamina > 60) cout << "\033[32m";
        else if (stamina > 30) cout << "\033[33m";
        else cout << "\033[31m";
        cout << stamina << "/100\033[0m";
    }

    static void printPotion(int potion) {
        if (potion > 0) cout << "\033[36m";
        else cout << "\033[31m";
        cout << potion << "\033[0m";
    }

    static void printGold(int gold) {
        cout << "\033[33m" << gold << "\033[0m";
    }

    static void printBattleScreen(int playerHp, int playerStamina, int playerPotion,
        int bossHp, int bossMaxHp, int level) {
        cout << "\n\033[33m========================================\033[0m\n";
        cout << "  \033[31m[ BOSS  Lv." << level << " ]\033[0m";
        cout << "          ";
        cout << "\033[32m[ KAHRAMAN ]\033[0m\n";
        cout << "\033[33m========================================\033[0m\n";
        cout << "  HP : "; UI::printHP(bossHp, bossMaxHp);
        cout << "        ";
        cout << "HP : "; UI::printHP(playerHp, 100);
        cout << "\n";
        cout << "                      ";
        cout << "Stamina: "; UI::printStamina(playerStamina);
        cout << "\n";
        cout << "                      ";
        cout << "Iksir  : "; UI::printPotion(playerPotion);
        cout << "\n";
        cout << "\033[33m========================================\033[0m\n";
    }

    static void printCritical(int damage) {
        cout << "\033[1m\033[93m-=[ KRİTİK VURUŞ ]=-\033[0m\n";
        cout << "\033[1m\033[93m      " << damage << " HASAR!\033[0m\n";
    }

    static void printBossCritical(int damage) {
        cout << "\033[1m\033[91m-=[ BOSS KRİTİK ]=-\033[0m\n";
        cout << "\033[1m\033[91m      " << damage << " HASAR!\033[0m\n";
    }
};

class Player;

class Enemy {
public:
    int damage;
    int hp;
    int maxHp;
    bool isDefending = false;

    Enemy(int d, int h) {
        damage = d;
        hp = h;
        maxHp = h;
    }

    int takeDamage(int amount) {
        if (isDefending) {
            amount /= 2;
            cout << "Boss savundu hasar azaldi.\n";
            isDefending = false;
        }
        hp -= amount;
        if (hp < 0) hp = 0;
        return amount;
    }

    void attack(Player& cPlayer);
};

class Player {
public:
    int damage;
    int hp;
    int stamina;
    int potion = 3;
    bool isDefending = false;
    int gold = 0;

    Player(int d, int h) {
        damage = d;
        hp = h;
        stamina = 100;
    }

    int takeDamage(int amount) {
        if (isDefending) {
            amount /= 2;
            isDefending = false;
        }
        hp -= amount;
        if (hp < 0) hp = 0;
        return amount;
    }

    void defend() {
        if (stamina < 5) {
            cout << "Yorgunsun, savunamazsin!\n";
            return;
        }
        stamina -= 5;
        isDefending = true;
    }

    void attack(Enemy& cEnemy) {
        int chance = rand() % 100;
        int finalDamage = damage;

        if (stamina < 10) {
            cout << "Yorgunsun!\n";
            return;
        }
        stamina -= 10;

        if (chance < 10) {
            finalDamage *= 2;
            int actualDamage = cEnemy.takeDamage(finalDamage);
            UI::printCritical(actualDamage);
        }
        else {
            int actualDamage = cEnemy.takeDamage(finalDamage);
            cout << "Oyuncu " << actualDamage << " hasar verdi\n";
        }
    }

    void heavyAttack(Enemy& cEnemy) {
        int chance = rand() % 100;
        int finalDamage = damage;

        if (stamina < 25) {
            cout << "Yorgunsun!\n";
            return;
        }
        stamina -= 25;

        if (chance < 30) {
            finalDamage *= 2;
            int actualDamage = cEnemy.takeDamage(finalDamage);
            cout << "Guclu saldirin ekstra hasar verdi!\n";
            cout << "Oyuncu " << actualDamage << " hasar verdi\n";
        }
        else {
            cout << "Guclu saldiri kacti ve acikta kaldin!\n";
            takeDamage(5);
            cout << "Oyuncu 5 hasar aldi!\n";
        }
    }

    void potionControl() {
        if (potion <= 0) {
            cout << "Iksirlerin bitti!\n";
        }
        else {
            if (hp >= 100) {
                cout << "Canin full!\n";
            }
            else {
                potion--;
                hp += 30;
                if (hp > 100) hp = 100;
                cout << "Iksir kullanildi +30 HP eklendi!\n";
            }
        }
    }
};

void Enemy::attack(Player& cPlayer) {
    int chance = rand() % 100;
    int choice = rand() % 100;
    int finalDamage = damage;

    if (choice < 60) {
        cout << "Boss saldirdi!\n";
        if (chance < 5) {
            finalDamage *= 2;
            int actualDamage = cPlayer.takeDamage(finalDamage);
            UI::printBossCritical(actualDamage);
        }
        else {
            int actualDamage = cPlayer.takeDamage(finalDamage);
            cout << "Boss " << actualDamage << " hasar verdi\n";
        }
    }
    else if (choice < 85) {
        cout << "Boss guclu saldiri yapti!\n";
        if (chance < 30) {
            finalDamage *= 2;
            int actualDamage = cPlayer.takeDamage(finalDamage);
            cout << "Boss " << actualDamage << " hasar verdi\n";
        }
        else {
            cout << "Boss acikta kaldi!\n";
            takeDamage(5);
        }
    }
    else {
        isDefending = true;
        cout << "Boss savunmaya hazirlaniyor!\n";
    }
}

class Game {
public:
    Player player;
    Enemy boss;
    int level = 1;

    Game() : player(10, 100), boss(15, 50) {}

    void spawnNewBoss() {
        boss.hp = 50 + (level * 20);
        boss.maxHp = boss.hp;
        boss.damage = 15 + (level * 5);
        cout << "\n--- Yeni Boss Geldi Level " << level << " ---\n";
    }

    void rewardPlayer() {
        player.hp += 25;
        player.damage = 15 + ((level + 1) * 4);
        if (player.hp > 100) player.hp = 100;
        cout << "\nTebrikler guclendirildin: +25 HP & +" << (level + 1) * 4 << " Hasar\n";
    }

    void bossFight() {
        while (boss.hp > 0 && player.hp > 0) {
            UI::printBattleScreen(player.hp, player.stamina, player.potion,
                boss.hp, boss.maxHp, level);
            playerTurn();
            enemyTurn();
            cout << "\n";
        }
    }

    void shop() {
        while (true) {
            int dukkanSecim;
            cout << "\n----- Dukkana hosgeldin savasci -----\n";
            cout << "Altinin: "; UI::printGold(player.gold); cout << "\n";
            cout << "1 - Kilicini bile(+5 Hasar)   ---> 30 Altin\n";
            cout << "2 - Sargi bezi kullan(+20 HP) ---> 30 Altin\n";
            cout << "3 - Dukkandan cik\n";
            cout << "Seciminiz> ";

            while (!(cin >> dukkanSecim)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Gecersiz giris! Sayi giriniz> ";
            }

            if (dukkanSecim == 1 && player.gold >= 30) {
                player.damage += 5;
                player.gold -= 30;
                cout << "\nHasarin +5 artti savasci!\n";
            }
            else if (dukkanSecim == 2 && player.gold >= 30) {
                player.hp += 20;
                if (player.hp > 100) player.hp = 100;
                player.gold -= 30;
                cout << "\nCanin +20 artti savasci!\n";
            }
            else if (dukkanSecim == 3) {
                break;
            }
            else if (player.gold < 30) {
                cout << "\nYeterli altinin yok!\n";
            }
            else {
                cout << "\nGecersiz secim yaptiniz.\n";
                continue;
            }
        }
    }

    void start() {
        while (player.hp > 0) {
            bossFight();
            if (player.hp > 0) {
                player.gold += 20 + (level * 10);
                cout << "\nKazandigin altin: " << 20 + (level * 10) << "\n";
                cout << "Durumun: " << player.hp << " HP | " << player.damage << " Hasar\n";
                rewardPlayer();
                shop();
                level++;
                spawnNewBoss();
            }
        }
        cout << "\nOYUN BITTI!\n";
    }

    void playerTurn() {
        int combatSelect;
        cout << "\n1 - Saldir\n"
            << "2 - Savun\n"
            << "3 - Guclu Saldiri\n"
            << "4 - Iksir kullan\n"
            << "Secimin> ";

        while (!(cin >> combatSelect)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Gecersiz giris! Sayi gir: ";
        }
        cout << "\n";

        if (combatSelect == 1) player.attack(boss);
        else if (combatSelect == 2) player.defend();
        else if (combatSelect == 3) player.heavyAttack(boss);
        else if (combatSelect == 4) player.potionControl();
        else {
            cout << "Gecersiz secim, tekrar dene!\n";
            playerTurn();
        }
    }

    void enemyTurn() {
        if (boss.hp > 0) boss.attack(player);
        player.stamina += 5;
        if (player.stamina > 100) player.stamina = 100;
    }
};

int main() {
    srand(time(0));
    Game game;
    game.start();
    return 0;
}