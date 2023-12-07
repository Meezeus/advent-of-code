/////////////////////////////////////////
// Advent of Code 2023 Day 7 Part 2    //
// https://adventofcode.com/2023/day/7 //
/////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

class Hand {
public:
    enum Card { JOKER=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, QUEEN, KING, ACE };
    enum HandType { HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, FULL_HOUSE, FOUR_OF_A_KIND, FIVE_OF_A_KIND };

    static Card determineCardFromChar(const char c) {
        switch (c) {
            case 'J': return JOKER;
            case '2': return TWO;
            case '3': return THREE;
            case '4': return FOUR;
            case '5': return FIVE;
            case '6': return SIX;
            case '7': return SEVEN;
            case '8': return EIGHT;
            case '9': return NINE;
            case 'T': return TEN;
            case 'Q': return QUEEN;
            case 'K': return KING;
            case 'A': return ACE;
        }
    }

    static char getCardAsChar(const Card card) {
        switch (card) {
            case JOKER: return 'J';
            case TWO: return '2';
            case THREE: return '3';
            case FOUR: return '4';
            case FIVE: return '5';
            case SIX: return '6';
            case SEVEN: return '7';
            case EIGHT: return '8';
            case NINE: return '9';
            case TEN: return 'T';
            case QUEEN: return 'Q';
            case KING: return 'K';
            case ACE: return 'A';
        }
    }

    static HandType determineHandType(const std::vector<Card> cards) {
        std::map<Card, int> cardToCount = {};
        for (Card card : cards) {
            cardToCount[card] = (cardToCount.find(card) != cardToCount.end()) ? cardToCount[card] + 1 : 1;
        }

        std::vector<int> counts = {};
        for (std::pair<char, int> cardAndCount : cardToCount) {
            if (cardAndCount.first != JOKER) {
                counts.push_back(cardAndCount.second);
            }
        }
        std::sort(counts.begin(), counts.end(), std::greater<int>());


        if (cardToCount[JOKER] == 5) {
            return FIVE_OF_A_KIND;
        }
        else if (!counts.empty()) {
            counts[0] += cardToCount[JOKER];
        }

        if (counts[0] == 5) {
            return FIVE_OF_A_KIND;
        }
        else if (counts[0] == 4) {
            return FOUR_OF_A_KIND;
        }
        else if (counts[0] == 3 && counts[1] == 2) {
            return FULL_HOUSE;
        }
        else if (counts[0] == 3) {
            return THREE_OF_A_KIND;
        }
        else if (counts[0] == 2 && counts[1] == 2) {
            return TWO_PAIR;
        }
        else if (counts[0] == 2) {
            return ONE_PAIR;
        }
        else if (counts[0] == 1) {
            return HIGH_CARD;
        }
    }

    static std::string getHandTypeName(const HandType handType) {
        switch (handType) {
            case HIGH_CARD: return "HIGH_CARD";
            case ONE_PAIR: return "ONE_PAIR";
            case TWO_PAIR: return "TWO_PAIR";
            case THREE_OF_A_KIND: return "THREE_OF_A_KIND";
            case FULL_HOUSE: return "FULL_HOUSE";
            case FOUR_OF_A_KIND: return "FOUR_OF_A_KIND";
            case FIVE_OF_A_KIND: return "FIVE_OF_A_KIND";
        }
    }

private:
    std::vector<Card> cards = {};
    HandType handType;
    int bid;

public:
    Hand(const std::string cardsString, const int bid) {
        for (char c : cardsString) {
            cards.push_back(determineCardFromChar(c));
        }
        handType = determineHandType(cards);
        this->bid = bid;
    }

    bool operator<(const Hand& other) const {
        if (handType == other.handType) {
            for (int i = 0; i < 5; ++i) {
                if (cards[i] != other.cards[i]) {
                    return cards[i] < other.cards[i];
                }
            }
            return false;
        }
        else {
            return handType < other.handType;
        }
    }

    std::vector<Card> getCards() {
        return cards;
    }

    std::string getCardsAsString() {
        std::string cardsString;
        for (Card card : cards) {
            cardsString += getCardAsChar(card);
        }
        return cardsString;
    }

    HandType getHandType() {
        return handType;
    }

    std::string getHandTypeName() {
        return getHandTypeName(handType);
    }

    int getBid() {
        return bid;
    }

};

int solution(std::string fileName);
std::vector<std::string> readLines(std::string fileName);

int main() {
    std::cout << std::endl;
    std::cout << "Example Solution: " << solution("example.txt") << " (expected 5905)" << std::endl;
    std::cout << "Input Solution: " << solution("input.txt") << std::endl;
    std::cout << std::endl;
}

int solution(std::string fileName) {
    std::vector<std::string> input = readLines(fileName);

    std::vector<Hand> hands = {};
    for (std::string line : input) {
        std::istringstream iss(line);
        std::string cards, bid;
        iss >> cards >> bid;
        hands.push_back(Hand(cards, std::stoi(bid)));
    }
    std::sort(hands.begin(), hands.end());

    int sum = 0;
    for (int i = 0; i < hands.size(); ++i) {
        sum += (i + 1) * hands[i].getBid();
    }

    return sum;
}

std::vector<std::string> readLines(std::string fileName) {
    std::vector<std::string> lines;
    std::ifstream inputFile(fileName);

    if (inputFile.fail()) {
        std::cout << "The file doesn't exist you dummy!" << std::endl;
    }
    else {
        std::string line;
        while (std::getline(inputFile, line)) {
            lines.push_back(line);
        }
        inputFile.close();
    }

    return lines;
}
