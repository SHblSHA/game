#include <iostream>
#include <string>
#include <cstdlib> // для rand() и srand()
#include <ctime> // для time()

// Генерируем рандомное число между min и max
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Creature
{
protected:
	std::string m_name;
	char m_symbol;
	int m_health;
	int m_damage;
	int m_gold;

public:
	Creature(std::string name, char symbol, int health, int damage, int gold) :
		m_name(name), m_symbol(symbol), m_health(health), m_damage(damage), m_gold(gold)
	{
	}

	char getSymbol() { return m_symbol; }
	const std::string& getName() { return m_name; }
	bool isDead() { return m_health <= 0; }
	int getGold() { return m_gold; }
	void addGold(int gold) { m_gold += gold; }
	void reduceHealth(int health) { m_health -= health; }
	int getHealth() { return m_health; }
	int getDamage() { return m_damage; }
};

class Player : public Creature
{
	int m_level = 1;

public:
	Player(std::string name)
		: Creature(name, '@', 10, 1, 0)
	{
	}

	void levelUp()
	{
		++m_level;
		++m_damage;
	}

	int getLevel() { return m_level; }
	bool hasWon() { return m_level >= 20; }
};

class Monster : public Creature
{
public:
	enum Type
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};

	struct MonsterData
	{
		const char* name;
		char symbol;
		int health;
		int damage;
		int gold;
	};

	static MonsterData monsterData[MAX_TYPES];

	Monster(Type type)
		: Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].health, monsterData[type].damage, monsterData[type].gold)
	{
	}

	static Monster getRandomMonster()
	{
		int num = getRandomNumber(0, MAX_TYPES - 1);
		return Monster(static_cast<Type>(num));
	}
};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};

void attackMonster(Player& player, Monster& monster)
{
	if (player.isDead())
		return;
	monster.reduceHealth(player.getDamage());
	std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";

	if (monster.isDead())
	{
		std::cout << "You killed the " << monster.getName() << ".\n";
		player.levelUp();
		std::cout << "You are now level " << player.getLevel() << ".\n";
		player.addGold(monster.getGold());
		std::cout << "You found " << monster.getGold() << " gold.\n";
	}
}

void attackPlayer(Player& player, Monster& monster)
{
	if (monster.isDead())
		return;

	player.reduceHealth(monster.getDamage());
	std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
}

void fightMonster(Player& player)
{
	Monster monster = Monster::getRandomMonster();
	std::cout << "You have encountered a " << monster.getName() << " (" << monster.getSymbol() << ").\n";
	while (!player.isDead() && !monster.isDead())
	{
		std::cout << "(R)un or (F)ight: ";
		char changePlaeyr;
		std::cin >> changePlaeyr;
		if (changePlaeyr == 'r' || changePlaeyr == 'R')
		{
			if (getRandomNumber(0, 1))
			{
				std::cout << "You successfully fled.\n";
				return;
			}

			else
			{
				std::cout << "You failed to flee.\n";
				attackPlayer(player, monster);
				continue;
			}
		}

		if (changePlaeyr == 'f' || changePlaeyr == 'F')
		{
			attackMonster(player, monster);
			attackPlayer(player, monster);
		}
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа
	rand(); // сбрасываем первый результат

	std::cout << "Enter your name: ";
	std::string name;
	std::cin >> name;
	std::cout << "Welcome, " << name << "\n";
	Player player(name);
	std::cout << "You have " << player.getHealth() << " health and are carrying " << player.getGold() << " gold.";

	while (!player.isDead() && !player.hasWon())
		fightMonster(player);

	if (player.isDead())
	{
		std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold\n";
		std::cout << "Too bad you can't take it with you!\n";
	}
	if (player.hasWon())
	{
		std::cout << "You win!!! You have " << player.getGold() << " gold.\n";
	}
	return 0;
}