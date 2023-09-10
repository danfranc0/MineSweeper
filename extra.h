#pragma once
#include <iostream>
#include <string>
#include <map>
#include <random>
#include <SFML/Graphics.hpp>
using namespace std;

struct Tile {
	bool flag;
	bool mine;
	int mineNum = 0;
	vector<Tile*> adjacentTiles;
	int tileX;
	int tileY;
	bool revealed;
};

class TextureManager
{
	static map<string, sf::Texture> textures;
public:
	static void LoadTexture(string textureName);
	static sf::Texture& GetTexture(string textureName);

	// for getting digits
	static void LoadDigit(string digit);
	static sf::Texture& GetDigit(string digit);
	static void Clear(); // call at end of main
};

class Random
{
	static std::mt19937 random;
public:
	static int randNumber(int min, int max);
};