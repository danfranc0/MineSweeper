#include "extra.h"


map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string filename) {
	string path = "images/";
	path += filename + ".png";

	textures[filename].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName) {
	// if the texture doesnt exist
	if (textures.find(textureName) == textures.end())
		LoadTexture(textureName);

	return textures[textureName];
}

// digits
void TextureManager::LoadDigit(string digit) {

	// determine digit
	if (digit == "zero")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
	else if (digit == "one")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(21, 0, 21, 32));
	else if (digit == "two")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(42, 0, 21, 32));
	else if (digit == "three")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(63, 0, 21, 32));
	else if (digit == "four")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(84, 0, 21, 32));
	else if (digit == "five")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(105, 0, 21, 32));
	else if (digit == "six")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(126, 0, 21, 32));
	else if (digit == "seven")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(147, 0, 21, 32));
	else if (digit == "eight")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(168, 0, 21, 32));
	else if (digit == "nine")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(189, 0, 21, 32));
	else if (digit == "neg")
		textures[digit].loadFromFile("images/digits.png", sf::IntRect(210, 0, 21, 32));
}

sf::Texture& TextureManager::GetDigit(string digit) {
	// if the texture doesnt exist
	if (textures.find(digit) == textures.end())
		LoadDigit(digit);

	return textures[digit];
}

void TextureManager::Clear() {
	textures.clear();
}


// randomizer
std::mt19937 Random::random;

int Random::randNumber(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(random);
}
