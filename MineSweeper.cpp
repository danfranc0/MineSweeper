#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "extra.h"

using namespace std;

void setMines(int minecount, int rows, int cols, int** boardArr, Tile** tileObjs); 
void revealTile(int xIndex, int yIndex, int width, vector<sf::Sprite>& tileSprites, Tile** tileObjs, int& flagCount);
void setHints(int height, int width, Tile** tileObjs);
void drawHints(int width, int height, Tile** tileObjs, vector<sf::Sprite>& hintSprites);

void newGame(int **boardArr, int width, int height, vector<sf::Sprite>& tileSprites, Tile** tileObjs, int mineCount) {
    // create board
    float xPos = 0, yPos = 0;
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            sf::Sprite tempTile(TextureManager::GetTexture("tile_hidden"));

            tempTile.setPosition(sf::Vector2f(xPos, yPos));
            tileSprites.push_back(tempTile);
            boardArr[i][j] = 0;
            tileObjs[i][j].mine = false;
            tileObjs[i][j].flag = false;
            tileObjs[i][j].revealed = false;
            tileObjs[i][j].mineNum = 0;
            tileObjs[i][j].tileX = j;
            tileObjs[i][j].tileY = i;
            tileObjs[i][j].adjacentTiles.clear();
            xPos += 32.f;
        }
        xPos = 0;
        yPos += 32.f;
    }
    
    // set adjacent pointers
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (((i - 1) >= 0) && ((j - 1) >= 0)) {           // top left
                Tile* adjacent = &tileObjs[i - 1][j - 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if ((i - 1) >= 0) {                                 // top
                Tile* adjacent = &tileObjs[i - 1][j];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i - 1) >= 0) && ((j + 1) < width)) {          // top right
                Tile* adjacent = &tileObjs[i - 1][j + 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((j - 1) >= 0)) {                               // left
                Tile* adjacent = &tileObjs[i][j - 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((j + 1) < width)) {                               // right
                Tile* adjacent = &tileObjs[i][j + 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i + 1) < height) && ((j - 1) >= 0)) {                               // bottom left
                Tile* adjacent = &tileObjs[i + 1][j - 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i + 1) < height)) {                               // bottom
                Tile* adjacent = &tileObjs[i + 1][j];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i + 1) < height) && ((j + 1) < width)) {                               // bottom right
                Tile* adjacent = &tileObjs[i + 1][j + 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
        }
    }
    setMines(mineCount, height, width, boardArr, tileObjs);
}


void loadBoard(string filename, vector<sf::Sprite>& tileSprites, Tile** tileObjs, int& mineCount, vector<sf::Sprite>& hintSprites) {
    ifstream file(filename);

    int height = 0, width = 0;
    string line;
    vector<string> s_board;

    if (file.is_open()) {
        string lineFromFile;
        while (getline(file, lineFromFile)) {
            s_board.push_back(lineFromFile);
            width = lineFromFile.size();
        }
        height = s_board.size();
    }

    // setup
    tileSprites.clear();
    float xPos = 0, yPos = 0;
    mineCount = 0;

    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            sf::Sprite tempTile(TextureManager::GetTexture("tile_hidden"));
            tempTile.setPosition(sf::Vector2f(xPos, yPos));
            tileSprites.push_back(tempTile);

            if (s_board[i].at(j) == '0') {
                tileObjs[i][j].mine = false;
            }
            else {
                tileObjs[i][j].mine = true;
                mineCount++;
            }
            tileObjs[i][j].flag = false;
            tileObjs[i][j].revealed = false;
            tileObjs[i][j].mineNum = 0;
            tileObjs[i][j].tileX = j;
            tileObjs[i][j].tileY = i;
            tileObjs[i][j].adjacentTiles.clear();
            xPos += 32.f;
        }
        xPos = 0;
        yPos += 32.f;
    }

    // set adjacent pointers
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (((i - 1) >= 0) && ((j - 1) >= 0)) {           // top left
                Tile* adjacent = &tileObjs[i - 1][j - 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if ((i - 1) >= 0) {                                 // top
                Tile* adjacent = &tileObjs[i - 1][j];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i - 1) >= 0) && ((j + 1) < width)) {          // top right
                Tile* adjacent = &tileObjs[i - 1][j + 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((j - 1) >= 0)) {                               // left
                Tile* adjacent = &tileObjs[i][j - 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((j + 1) < width)) {                               // right
                Tile* adjacent = &tileObjs[i][j + 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i + 1) < height) && ((j - 1) >= 0)) {                               // bottom left
                Tile* adjacent = &tileObjs[i + 1][j - 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i + 1) < height)) {                               // bottom
                Tile* adjacent = &tileObjs[i + 1][j];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
            if (((i + 1) < height) && ((j + 1) < width)) {                               // bottom right
                Tile* adjacent = &tileObjs[i + 1][j + 1];
                tileObjs[i][j].adjacentTiles.push_back(adjacent);
            }
        }
    }
}

void setMines(int minecount, int rows, int cols, int **boardArr, Tile **tileObjs) {
    for (int i = 0; i < minecount; i++) {
        bool empty = true;

        while (empty) {
            int xIndex = Random::randNumber(0, cols - 1);
            int yIndex = Random::randNumber(0, rows - 1);
            if (boardArr[yIndex][xIndex] == 0) {            // place mine
                boardArr[yIndex][xIndex] = 1;
                tileObjs[yIndex][xIndex].mine = true;
                empty = false;
            }
        }
    }
}

void setHints(int height, int width, Tile** tileObjs) {
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            if (tileObjs[i][j].mine == false) {
                for (int k = 0; k < tileObjs[i][j].adjacentTiles.size(); k++) {
                    if (tileObjs[i][j].adjacentTiles[k]->mine == true) {
                        tileObjs[i][j].mineNum = tileObjs[i][j].mineNum + 1;
                    }
                }
            }
        }
    }
}

void setFlags(int width, int height, vector<sf::Sprite>& flags, Tile** tileObjs) {
    
    float xPos = 0, yPos = 0;
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            sf::Sprite tempTile(TextureManager::GetTexture("flag"));

            tempTile.setPosition(sf::Vector2f(xPos, yPos));
            flags.push_back(tempTile);
            xPos += 32.f;
        }
        xPos = 0;
        yPos += 32.f;
    }
    /*
    // first time
    if (flags.size() == 0) {
        float xPos = 0, yPos = 0;
        for (int i = 0; i < height; i++) {

            for (int j = 0; j < width; j++) {
                sf::Sprite tempTile(TextureManager::GetTexture("flag"));

                tempTile.setPosition(sf::Vector2f((width * 32), 0));
                flags.push_back(tempTile);
                xPos += 32.f;
            }
            xPos = 0;
            yPos += 32.f;
        }
    }
    else {
        for (unsigned int i = 0; i < (width * height); i++) {
            flags[i].setPosition(sf::Vector2f((width * 32), 0));
        }
    }
    */
}

void hideMines(vector<sf::Sprite>& mines, int mineCount, int width) {
    mines.clear();

    for (unsigned int i = 0; i < mineCount; i++) {
        sf::Sprite tempSprite(TextureManager::GetTexture("mine"));
        tempSprite.setPosition(sf::Vector2f((width * 32), 0));
        mines.push_back(tempSprite);
    }
}


sf::Sprite getDigitSprite(int num) {       // return digit sprite 

    if (num == 0) {
        sf::Sprite zeroDigit(TextureManager::GetDigit("zero"));
        return zeroDigit;
    }
    else if (num == 1) {
        sf::Sprite oneDigit(TextureManager::GetDigit("one"));
        return oneDigit;
    }
    else if (num == 2) {
        sf::Sprite twoDigit(TextureManager::GetDigit("two"));
        return twoDigit;
    }
    else if (num == 3) {
        sf::Sprite threeDigit(TextureManager::GetDigit("three"));
        return threeDigit;
    }
    else if (num == 4) {
        sf::Sprite fourDigit(TextureManager::GetDigit("four"));
        return fourDigit;
    }
    else if (num == 5) {
        sf::Sprite fiveDigit(TextureManager::GetDigit("five"));
        return fiveDigit;
    }
    else if (num == 6) {
        sf::Sprite sixDigit(TextureManager::GetDigit("six"));
        return sixDigit;
    }
    else if (num == 7) {
        sf::Sprite sevenDigit(TextureManager::GetDigit("seven"));
        return sevenDigit;
    }
    else if (num == 8) {
        sf::Sprite eightDigit(TextureManager::GetDigit("eight"));
        return eightDigit;
    }
    else if (num == 9) {
        sf::Sprite nineDigit(TextureManager::GetDigit("nine"));
        return nineDigit;
    }
}

vector<sf::Sprite> setCounter(vector<sf::Sprite>& digitSprites, int mineCount) {
    int hun = mineCount / 100;
    int ten = (mineCount % 100) / 10;
    int one = mineCount % 10;
    vector<sf::Sprite> counterSprites;
    counterSprites.push_back(getDigitSprite(hun));
    counterSprites.push_back(getDigitSprite(ten));
    counterSprites.push_back(getDigitSprite(one));
    return counterSprites;
}

void updateCounter(Tile** tileObjs, vector<sf::Sprite>& counterSprites, int mineCount, int width, int height) {
    // check number of flagged tiles
    int flagCount = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            if (tileObjs[i][j].flag == true) {
                flagCount++;
            }
        }
    }

    // check if negative
    int count = mineCount - flagCount;

    // update counter
    if (count < 0) {
        cout << "count: " << count << endl;
        count *= -1;
        int ten = (count % 100) / 10;
        int one = count % 10;
        sf::Sprite negativeDigit(TextureManager::GetDigit("neg"));
        counterSprites[0] = negativeDigit;
        counterSprites[0].setPosition(0, (height * 32));

        counterSprites[1] = getDigitSprite(ten);
        counterSprites[2] = getDigitSprite(one);
        counterSprites[1].setPosition(21, (height * 32));
        counterSprites[2].setPosition(42, (height * 32));

    }
    else {
        int hun = count / 100;
        int ten = (count % 100) / 10;
        int one = count % 10;
        counterSprites[0] = getDigitSprite(hun);
        counterSprites[1] = getDigitSprite(ten);
        counterSprites[2] = getDigitSprite(one);
        counterSprites[0].setPosition(0, (height * 32));
        counterSprites[1].setPosition(21, (height * 32));
        counterSprites[2].setPosition(42, (height * 32));
    }

}

bool checkMine(int xIndex, int yIndex, Tile** tileObjs) {
    /*
    int index = ((yIndex * width) + xIndex);

    sf::Sprite tempSprite(TextureManager::GetTexture("tile_revealed"));
    tempSprite.setPosition(sf::Vector2f(xIndex * 32, yIndex * 32));
    tileSprites[index] = tempSprite;

    */
    if (tileObjs[yIndex][xIndex].mine == true) {
        // reveal mine
        return true;
    }
    else {
        // reveal adjecent
        return false;
    }
}

void drawHints(int width, int height, Tile** tileObjs, vector<sf::Sprite>& hintSprites) {
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            if (tileObjs[i][j].mineNum > 0) {
                if (tileObjs[i][j].mineNum == 1) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_1"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 2) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_2"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 3) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_3"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 4) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_4"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 5) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_5"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 6) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_6"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 7) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_7"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
                if (tileObjs[i][j].mineNum == 8) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_8"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
            }
            /*
            Tile tempTile = tileObjs[i][j];
            cout << tempTile.mineNum << endl;
            if (tileObjs[i][j].mineNum > 0) {
                cout << tileObjs[i][j].mineNum << endl;
                if (tileObjs[i][j].mineNum == 1) {
                    sf::Sprite tempSprite(TextureManager::GetTexture("number_1"));
                    tempSprite.setPosition(sf::Vector2f((j * 32), (i * 32)));
                    hintSprites.push_back(tempSprite);
                }
            }
            */
        }
    }
}


// reveal tiles
void revealTile(int xIndex, int yIndex, int width, vector<sf::Sprite>& tileSprites, Tile** tileObjs, int& flagCount) {
    // if not mine and no flag
    if (tileObjs[yIndex][xIndex].mine == false && tileObjs[yIndex][xIndex].flag == false) {
        // reveal tile
        int index = ((yIndex * width) + xIndex);
        tileObjs[yIndex][xIndex].revealed = true;
        tileSprites[index].setColor(sf::Color::Transparent);

        // remove flag is any
        if (tileObjs[yIndex][xIndex].flag == true) {
            tileObjs[yIndex][xIndex].flag = false;
            flagCount--;
        }

        // end if tile contains hint
        if (tileObjs[yIndex][xIndex].mineNum == 0) {

            // reveal adjacent recursion
            for (int i = 0; i < tileObjs[yIndex][xIndex].adjacentTiles.size(); i++) {

                // check if contains mine or is already revealed
                if (tileObjs[yIndex][xIndex].adjacentTiles[i]->mine == false && tileObjs[yIndex][xIndex].adjacentTiles[i]->revealed == false) {
                    int newX = tileObjs[yIndex][xIndex].adjacentTiles[i]->tileX;
                    int newY = tileObjs[yIndex][xIndex].adjacentTiles[i]->tileY;

                    // recursion
                    revealTile(newX, newY, width, tileSprites, tileObjs, flagCount);
                }
            }
        }
    }
}

// reveal mines
void revealMines(int height, int width, Tile **tileObjs, vector<sf::Sprite>& mines, vector<sf::Sprite>& tileSprites) {
    int count = 0, index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (tileObjs[i][j].mine == true) {
                tileObjs[i][j].flag = false;
                mines[count].setPosition(sf::Vector2f((j * 32), (i * 32)));
                tileSprites[index].setColor(sf::Color::Transparent);
                count++;
            }
            index++;
        }
    }
}

void toggleDebug(bool debugMode, vector<sf::Sprite>& mines, Tile** tileObjs, int width, int height) {
    if (!debugMode) {
        for (int i = 0; i < mines.size(); i++) {
            mines[i].setPosition(sf::Vector2f((width * 32), 0));
        }
    }
    else {
        int count = 0, index = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (tileObjs[i][j].mine == true) {
                    mines[count].setPosition(sf::Vector2f((j * 32), (i * 32)));\
                    count++;
                }
                index++;
            }
        }
    }
}

bool checkFlag(int xIndex, int yIndex, Tile **tileObjs) {
    // check tile
    if (tileObjs[yIndex][xIndex].flag == false) {
        return false;
    }
    return true;
}

void toggleFlag(int xIndex, int yIndex, int width, Tile** tileObjs, vector<sf::Sprite>& flags, int& flagCount, int& counter) {

    // check tile
    if (tileObjs[yIndex][xIndex].flag == false) {
        // place flag sprite

        // update
        tileObjs[yIndex][xIndex].flag = true;
        flagCount++;
        cout << "placed flag\n";
        cout << flagCount << endl;
    }
    else {
        // remove flag sprite

        // update
        tileObjs[yIndex][xIndex].flag = false;
        flagCount--;
        cout << "removed flag\n";
        cout << flagCount << endl;
    }
}

bool checkWin(int tileCount, int mineCount, Tile** tileObjs, int width, int height) {
    int revealedCount = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            if (tileObjs[i][j].revealed == true) {
                revealedCount++;
            }
        }
    }

    if (revealedCount == (tileCount - mineCount)) {
        return true;
    }
    return false;
}

int main()
{
    // config
    ifstream inFile("boards/config.cfg");
    int width = 0, height = 0, mineCount = 0, tileCount;
    if (inFile.is_open()) {
        string lineFromFile;
        getline(inFile, lineFromFile);
        width = stoi(lineFromFile);
        getline(inFile, lineFromFile);
        height = stoi(lineFromFile);
        getline(inFile, lineFromFile);
        mineCount = stoi(lineFromFile);
    }
    tileCount = width * height;
    int counter = mineCount;    // bottom left counter
    int configMines = mineCount;
    int flagCount = 0;      // numb of flags


    // board setup 

    // create board with hidden tiles
    vector<sf::Sprite> tileSprites;           // tile sprites
    int** boardArr = new int* [height];   // console board
    Tile** tileObjs = new Tile * [height];  // tile objects

    for (unsigned int i = 0; i < height; i++) {     // number of rows
        boardArr[i] = new int[width];               // number of columns
        tileObjs[i] = new Tile[width];
    }



    // creating textures
    sf::Sprite debugSprite(TextureManager::GetTexture("debug"));
    sf::Sprite happySprite(TextureManager::GetTexture("face_happy"));
    sf::Sprite loseSprite(TextureManager::GetTexture("face_lose"));
    sf::Sprite winSprite(TextureManager::GetTexture("face_win"));
    sf::Sprite flagSprite(TextureManager::GetTexture("flag"));
    sf::Sprite mineSprite(TextureManager::GetTexture("mine"));
    sf::Sprite oneSprite(TextureManager::GetTexture("number_1"));
    sf::Sprite twoSprite(TextureManager::GetTexture("number_2"));
    sf::Sprite threeSprite(TextureManager::GetTexture("number_3"));
    sf::Sprite fourSprite(TextureManager::GetTexture("number_4"));
    sf::Sprite fiveSprite(TextureManager::GetTexture("number_5"));
    sf::Sprite sixSprite(TextureManager::GetTexture("number_6"));
    sf::Sprite sevenSprite(TextureManager::GetTexture("number_7"));
    sf::Sprite eightSprite(TextureManager::GetTexture("number_8"));
    sf::Sprite test1Sprite(TextureManager::GetTexture("test_1"));
    sf::Sprite test2Sprite(TextureManager::GetTexture("test_2"));
    sf::Sprite test3Sprite(TextureManager::GetTexture("test_3"));
    sf::Sprite hiddenSprite(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite revealedSprite(TextureManager::GetTexture("tile_revealed"));


    // creating counter
    vector<sf::Sprite> digitSprites;
    sf::Sprite zeroDigit(TextureManager::GetDigit("zero"));
    sf::Sprite oneDigit(TextureManager::GetDigit("one"));
    sf::Sprite twoDigit(TextureManager::GetDigit("two"));
    sf::Sprite threeDigit(TextureManager::GetDigit("three"));
    sf::Sprite fourDigit(TextureManager::GetDigit("four"));
    sf::Sprite fiveDigit(TextureManager::GetDigit("five"));
    sf::Sprite sixDigit(TextureManager::GetDigit("six"));
    sf::Sprite sevenDigit(TextureManager::GetDigit("seven"));
    sf::Sprite eightDigit(TextureManager::GetDigit("eight"));
    sf::Sprite nineDigit(TextureManager::GetDigit("nine"));
    sf::Sprite negativeDigit(TextureManager::GetDigit("neg"));
    digitSprites.push_back(zeroDigit);
    digitSprites.push_back(oneDigit);
    digitSprites.push_back(twoDigit);
    digitSprites.push_back(threeDigit);
    digitSprites.push_back(fourDigit);
    digitSprites.push_back(fiveDigit);
    digitSprites.push_back(sixDigit);
    digitSprites.push_back(sevenDigit);
    digitSprites.push_back(eightDigit);
    digitSprites.push_back(nineDigit);
    digitSprites.push_back(negativeDigit);
    vector<sf::Sprite> counterSprites = setCounter(digitSprites, mineCount);
    
    counterSprites[0].setPosition(0, (height * 32));
    counterSprites[1].setPosition(21, (height * 32));
    counterSprites[2].setPosition(42, (height * 32));


    // drawing board
    vector<sf::Sprite> revealedSprites;           // first layer sprites (revealed tiles)
    float xPos = 0, yPos = 0;
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {
            sf::Sprite tempTile(TextureManager::GetTexture("tile_revealed"));

            tempTile.setPosition(sf::Vector2f(xPos, yPos));
            revealedSprites.push_back(tempTile);
            xPos += 32.f;
        }
        xPos = 0;
        yPos += 32.f;
    }
    
    newGame(boardArr, width, height, tileSprites, tileObjs, mineCount);
    happySprite.setPosition(sf::Vector2f(((width * 32) / 2) - 16, height * 32));
    loseSprite.setPosition(sf::Vector2f((width * 32), 0));
    winSprite.setPosition(sf::Vector2f((width * 32), 0));
    debugSprite.setPosition(sf::Vector2f((width * 32) - 256, height * 32));
    test1Sprite.setPosition(sf::Vector2f((width * 32) - 192, height * 32));
    test2Sprite.setPosition(sf::Vector2f((width * 32) - 128, height * 32));
    test3Sprite.setPosition(sf::Vector2f((width * 32) - 64, height * 32));



    // mine sprites
    vector<sf::Sprite> mines;
    hideMines(mines, mineCount, width);

    // set hints
    setHints(height, width, tileObjs);
    vector<sf::Sprite> hintSprites;
    drawHints(width, height, tileObjs, hintSprites);

    // flag sprites
    vector<sf::Sprite> flags;
    setFlags(width, height, flags, tileObjs);

    bool debugMode = false;
    bool lose = false;
    bool win = false;


    sf::RenderWindow window(sf::VideoMode(width * 32, (height * 32) + 100), "Minesweeper");
    while (window.isOpen())
    {
        window.clear(sf::Color(255, 255, 255));
        // draw board
        for (int i = 0; i < tileCount; i++) {
            window.draw(revealedSprites[i]);
        }
        for (int i = 0; i < hintSprites.size(); i++) {
            window.draw(hintSprites[i]);
        }
        for (int i = 0; i < tileCount; i++) {
            window.draw(tileSprites[i]);
        } 
        window.draw(happySprite);
        window.draw(counterSprites[0]);
        window.draw(counterSprites[1]);
        window.draw(counterSprites[2]);
        window.draw(debugSprite);
        window.draw(test1Sprite);
        window.draw(test2Sprite);
        window.draw(test3Sprite);
        // draw flags
        int flagIndex = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (tileObjs[i][j].flag == true) {
                    window.draw(flags[flagIndex]);
                }
                flagIndex++;
            }
        }
        for (int i = 0; i < mineCount; i++) {
            window.draw(mines[i]);
        }
        window.draw(loseSprite);
        window.draw(winSprite);
        window.display();


        // check for clicks
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)                            // left click
                {
                    if (event.mouseButton.y < ((height * 32) + 64)) {
                        if (event.mouseButton.y > height * 32) {                            // outside board

                            // check which button is pressed
                            if (event.mouseButton.x > (width * 32) - 64) {
                                debugMode = false;
                                lose = false;
                                win = false;
                                toggleDebug(debugMode, mines, tileObjs, width, height);
                                loseSprite.setPosition(sf::Vector2f((width * 32), 0));
                                winSprite.setPosition(sf::Vector2f((width * 32), 0));

                                loadBoard("boards/testboard3.brd", tileSprites, tileObjs, mineCount, hintSprites);
                                hideMines(mines, mineCount, width);
                                hintSprites.clear();
                                setHints(height, width, tileObjs);
                                drawHints(width, height, tileObjs, hintSprites);

                                updateCounter(tileObjs, counterSprites, mineCount, width, height);
                            }
                            else if (event.mouseButton.x > (width * 32) - 128) {
                                debugMode = false;
                                lose = false;
                                win = false;
                                toggleDebug(debugMode, mines, tileObjs, width, height);
                                loseSprite.setPosition(sf::Vector2f((width * 32), 0));
                                winSprite.setPosition(sf::Vector2f((width * 32), 0));

                                loadBoard("boards/testboard2.brd", tileSprites, tileObjs, mineCount, hintSprites);
                                hideMines(mines, mineCount, width);
                                hintSprites.clear();
                                setHints(height, width, tileObjs);
                                drawHints(width, height, tileObjs, hintSprites);

                                updateCounter(tileObjs, counterSprites, mineCount, width, height);
                            }
                            else if (event.mouseButton.x > (width * 32) - 192) {
                                debugMode = false;
                                lose = false;
                                win = false;
                                toggleDebug(debugMode, mines, tileObjs, width, height);
                                loseSprite.setPosition(sf::Vector2f((width * 32), 0));
                                winSprite.setPosition(sf::Vector2f((width * 32), 0));

                                loadBoard("boards/testboard1.brd", tileSprites, tileObjs, mineCount, hintSprites);
                                hideMines(mines, mineCount, width);
                                hintSprites.clear();
                                setHints(height, width, tileObjs);
                                drawHints(width, height, tileObjs, hintSprites);

                                updateCounter(tileObjs, counterSprites, mineCount, width, height);
                            }
                            else if (event.mouseButton.x > (width * 32) - 256) {
                                if (!lose) {
                                    if (debugMode) {
                                        debugMode = false;
                                    }
                                    else {
                                        debugMode = true;
                                    }
                                    toggleDebug(debugMode, mines, tileObjs, width, height);
                                }
                            }
                            else if ((event.mouseButton.x > ((width * 32) / 2) - 16) && (event.mouseButton.x < ((width * 32) / 2) + 48)) {
                                // reset
                                lose = false;
                                win = false;
                                tileSprites.clear();
                                flagCount = 0;
                                mineCount = configMines;

                                loseSprite.setPosition(sf::Vector2f((width * 32), 0));
                                winSprite.setPosition(sf::Vector2f((width * 32), 0));
                                newGame(boardArr, width, height, tileSprites, tileObjs, mineCount);
                                hideMines(mines, mineCount, width);

                                hintSprites.clear();
                                setHints(height, width, tileObjs);
                                drawHints(width, height, tileObjs, hintSprites);
                                debugMode = false;
                                toggleDebug(debugMode, mines, tileObjs, width, height);
                                updateCounter(tileObjs, counterSprites, mineCount, width, height);
                            }
                        }
                        else {                                                             // inside board

                            if (!win) {
                                int xIndex = event.mouseButton.x / 32;
                                int yIndex = event.mouseButton.y / 32;
                                // check flag
                                if (!checkFlag(xIndex, yIndex, tileObjs)) {

                                    // check mine
                                    bool hasMine = checkMine(xIndex, yIndex, tileObjs);
                                    if (hasMine) {
                                        // lose
                                        lose = true;
                                        loseSprite.setPosition(sf::Vector2f(((width * 32) / 2) - 16, height * 32));
                                        revealMines(height, width, tileObjs, mines, tileSprites);
                                    }
                                    else if (!lose) {
                                        revealTile(xIndex, yIndex, width, tileSprites, tileObjs, flagCount);

                                        // check win condition
                                        if (checkWin(tileCount, mineCount, tileObjs, width, height)) {
                                            win = true;
                                            winSprite.setPosition(sf::Vector2f(((width * 32) / 2) - 16, height * 32));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (event.mouseButton.button == sf::Mouse::Right)                                   // right click
                {
                    if (event.mouseButton.y > height * 32) {

                    }
                    else {

                        int xIndex = event.mouseButton.x / 32;
                        int yIndex = event.mouseButton.y / 32;

                        // if revealed
                        if (tileObjs[yIndex][xIndex].revealed == false) {
                            toggleFlag(xIndex, yIndex, width, tileObjs, flags, flagCount, counter);
                        }
                        updateCounter(tileObjs, counterSprites, mineCount, width, height);
                    }

                }
            }
        }
    }


    // clear out textures
    TextureManager::Clear();
    return 0;
}