#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>

using namespace sf;
using namespace std;

bool initial = true;
// uninforced grid limits of playing space
// M = rows, N = columns
const int M = 20; 
const int N = 10;
// field is 2d array representing each square, initiated with 0
int field[M][N] = {0};

// each square is 18px

/***
 * 	Point, figures[][], field[][]
 * 	field[][]
 * 		-playing field
 * 	Point[4][4]
 * 		-occupy-able space of a moving shape
 * 			(current in play)
 * 		-4x4 to accomodate rotation
 * 	figures[7][4]
 * 		-initially occupy left half of 4x4 Point() space
		--
		0 1
		2 3
		4 5
		6 7
***/


// each new piece as it falls.
struct Point
{int x,y;} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
   for (int i=0;i<4;i++){
	  	if (a[i].x<0 || a[i].x>=N || a[i].y>=M){ 
			return 0;
		}
      	else if (field[a[i].y][a[i].x]){ 
			return 0;
		}
   }

   return 1;
};

void writeToDebugWindow(Text &debugText, String txt){
	debugText.setString(txt);	
}

void debugWriteToWindow(RenderWindow &window, Text &debugText){
	Vector2<int> windowPosition = window.getPosition();
	int absX = windowPosition.x;
	int absY = windowPosition.y;

	int mouseX = Mouse::getPosition(window).x;
	int mouseY = Mouse::getPosition(window).y;

	int relativeX = mouseX;
	int relativeY = mouseY;
	String mousePos =  to_string(relativeX) + " " + to_string(relativeY);
	writeToDebugWindow(debugText, mousePos);
};

int main()
{
    srand(time(0));	 

	RenderWindow window(VideoMode(320, 480), "Tetris!");

    Texture t1;
	Texture t2;
	Texture t3;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	t3.loadFromFile("images/frame.png");
	Sprite s(t1);
	Sprite background(t2);
	Sprite frame(t3);

    int dx=0; bool rotate=0; int colorNum=1;
	float timer=0,delay=0.3; 


	Clock clock;

	///////// DebugText box ///////////
	Font debugFont;
	debugFont.loadFromFile("images/Roboto/Roboto-Black.ttf");
	Text debugText("",debugFont);
	debugText.setFont(debugFont);
	debugText.setCharacterSize(30);
	debugText.setStyle(Text::Bold);
	debugText.setFillColor(Color::Black);
	debugText.setPosition(20,410);

	///////// ScoreText box ///////////
	Font scoreFont;
	scoreFont.loadFromFile("images/Roboto/Roboto-Black.ttf");
	Text scoreText("",scoreFont);
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(30);
	scoreText.setStyle(Text::Bold);
	scoreText.setFillColor(Color::Black);
	scoreText.setPosition(200,410);		

    while (window.isOpen())
    {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed){
				window.close();
			}

			if (e.type == Event::KeyPressed)
			{
				if (e.key.code==Keyboard::Up){
					rotate=true;
			  	}
			  	else if (e.key.code==Keyboard::Left){
					dx-=1;
			  	}
			  	else if (e.key.code==Keyboard::Right){
					dx+=1;
				}
			}

			// debug events
			if (e.type == Event::MouseMoved)
			{
				debugWriteToWindow(window, debugText);
				debugWriteToWindow(window, scoreText);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)){
			delay=0.05;
		}

		//// <- Move -> ///
		for (int i=0;i<4;i++){ 
			b[i] = a[i]; 
			a[i].x += dx; 
		}

		if (!check()){
			for (int i=0;i<4;i++){ 
				a[i]=b[i];
			}
		}

		//////Rotate//////
		if (rotate)
		{
			Point p = a[1]; //center of rotation
			for (int i=0;i<4;i++)
			{
				int x = a[i].y-p.y;
				int y = a[i].x-p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()){
				for (int i=0;i<4;i++) {
					a[i]=b[i];
				}
			}
		}


		///////Tick//////
		if (timer>delay)
		{
			for (int i=0;i<4;i++) {
				b[i]=a[i]; a[i].y+=1; 
			}

			
			if (!check())  // new piece now
			{
				for (int i=0;i<4;i++){
					field[b[i].y][b[i].x]=colorNum;
				}

				colorNum=1+rand()%7;
				int figIdx=rand()%7;
				for (int i=0;i<4;i++){
					// in original tetris pieces fall from middle					
					a[i].x = figures[figIdx][i] % 2 + (int)(N / 2 - 1); 
					a[i].y = figures[figIdx][i] / 2;
				}
			}

			timer=0;
		}

		///////check lines//////////
		// iterating through field...
		int k=M-1; //M = num rows
		for (int i=M-1;i>0;i--) // reverse iterate rows
		{
			int count=0;
			for (int j=0;j<N;j++) // iterate columns
			{
				if (field[i][j]){ 
					count++;
				}
				field[k][j]=field[i][j]; // swap?
			}
			if (count<N) { //if count < number of columns
				k--;
			}
		}

		dx=0; rotate=0; delay=0.3;

		/////////draw//////////
		window.clear(Color::White);	
		window.draw(background);

		// for each square in field	
		for (int i=0;i<M;i++) // row
			for (int j=0;j<N;j++) // column
			{
				if (field[i][j]==0) {
					continue;
				}
				else {
					s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
					s.setPosition(j*18,i*18);
					s.move(28,31); //offset
					window.draw(s);
				}
			}

			for (int i=0;i<4;i++)
			{
				s.setTextureRect(IntRect(colorNum*18,0,18,18));
				s.setPosition(a[i].x*18,a[i].y*18);
				s.move(28,31); //offset
				window.draw(s);
			}

		window.draw(debugText);		
		window.draw(scoreText);
		

		/////----  
		window.draw(frame);	
		window.display();

	}



    return 0;
}

