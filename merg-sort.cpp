#include <iostream>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
#include <SFML/Graphics.hpp>
using namespace std;

typedef vector<int> vi;
int dTimeDraggin = 0;
bool isDraggin = false;
int previousMouseX = 0;

int delay = 100;
vi items;
sf::Font FONT;
sf::Text TEXT("Merg Sort", FONT);

void pollEvents(sf::RenderWindow &window)
{ 

    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    delay += sf::Mouse::getPosition(window).x > previousMouseX ? -1: 1;
                }
                break;
            case sf::Event::KeyReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    isDraggin = false; 
                }
                break;
        }
    }
}

void setupFont()
{   FONT.loadFromFile("font.ttf");    
    TEXT.setCharacterSize(200);
    TEXT.setStyle(sf::Text::Bold);
    TEXT.setFillColor(sf::Color(168, 212, 173));
}

void generate_random_list()
{
    for (int i = 1; i < 125; i++)
    {
       items.push_back(i);
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(items.begin(), items.end(), default_random_engine(seed)); 
}
sf::Color rectColor(sf::Color(168, 212, 173));
sf::RectangleShape rect(sf::Vector2f(10.f, 100.f));
// this need to return a bool so you can close the main loop.
void drawItems(sf::RenderWindow &window, vi &list)
{
    pollEvents(window);

    window.clear(sf::Color(24,34,59));
    rect.setFillColor(sf::Color(7,56,161));
    for (int i = 0; i < list.size(); i++)
    {
        int item = list.at(i); 
        rect.setSize(sf::Vector2f(19.f, item*5.f));
        rect.setPosition(i*20, 1080 - item * 5.f);
        window.draw(rect);
    }
    window.draw(TEXT);
    window.display();
    this_thread::sleep_for(chrono::milliseconds(delay));
    return;
}

vi subArray(vi &arr, int dist, int start = 0)
{ 
    vi newArray;
    for (int i = 0; i < dist; i++)
    {
        newArray.push_back(arr.at(start+i));
    } 
    return newArray;
}

void merge(vi &left,vi &right,vi &output)
{ 
    int iLeft = 0;
    int iRight = 0;
    int iOutput = 0;
    while (iOutput < output.size())
    {
        if (iLeft == left.size() || iRight == right.size())
        {
            if (iLeft == left.size())
            {
                while (iRight < right.size())
                {
                    output.at(iOutput++) = right.at(iRight++);
                }
                return;
            }
            else
            {
                while (iLeft < left.size())
                {
                    output.at(iOutput++) = left.at(iLeft++);
                }        
                return;
            }
        }
        if (left.at(iLeft) < right.at(iRight))
        {
            output.at(iOutput++) = left.at(iLeft++);
        } 
        else
        {
            output.at(iOutput++) = right.at(iRight++);
        }
    } 

    return;
}

void sort(vi &list, sf::RenderWindow &window, int leftStart, int rightStop)
{
    int listSize = list.size();
    if (listSize < 2)
    {
        return;          
    }

    int midpoint = listSize / 2;
    vi left = subArray(list, midpoint);
    vi right = subArray(list, (listSize-midpoint), midpoint);

    sort(left, window, leftStart, leftStart+midpoint);
    sort(right, window, leftStart+midpoint, rightStop);
    merge(left, right, list);
    for (int i = 0; i < list.size(); i++)
    {
        items.at(leftStart+i) = list.at(i);
    }
    drawItems(window, items);
}


int main()
{
    setupFont();
    bool isComplete = false;
    generate_random_list();
    sf::RenderWindow window(sf::VideoMode(4320, 1080), "Merge Sort");
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
               window.close();
            } 
        }

        if (!isComplete)
        {
            sort(items, window, 0, items.size());
        }
    }
    return 0;        
}

