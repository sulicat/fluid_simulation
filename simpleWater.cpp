#include <SFML/Graphics.hpp>
#include <iostream>

#define CELL_SIZE 			(15)
#define PRESSURE_AFFECT 	(0.02)
#define MIN_WATER 			(0.0001)

// main window.
int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 800;
int cell_x_count = WINDOW_WIDTH / CELL_SIZE + 1;
int cell_y_count = WINDOW_HEIGHT / CELL_SIZE + 1;

sf::RenderWindow window( sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Circuit Helper" );

enum Cell_Type	{ 	EMPTY,
					OBSTACLE,
					INFLOW,
					OUTFLOW
				};

// class to define cells
class Cell{
 private:
 public:
	float 	x;
	float 	y;
	float 	width;
	float 	height;
	sf::RectangleShape shape;
	enum Cell_Type type;
	float water_content;
	float max_water;


	Cell( float _x, float _y, float _w, float _h ){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
		shape = sf::RectangleShape( sf::Vector2f(width, height) );
		shape.move( x, y );
		//		shape.setOutlineThickness( 1 );
		//shape.setOutlineColor( sf::Color( 200, 200, 200 ) );
		type = EMPTY;
		water_content = 0;
		max_water = 1;
	}

	void draw(){
		if( type == EMPTY ){
			shape.setFillColor( sf::Color(255 - 255*water_content, 255 - 255*water_content, 255) );
		}
		window.draw( shape );
	}

	void changeType( enum Cell_Type t ){
		type = t;
		if( type == EMPTY ){
			shape.setFillColor( sf::Color(255 - 255*water_content, 255 - 255*water_content, 255) );
		}else if( type == OBSTACLE ){
			shape.setFillColor( sf::Color(255, 255, 0) );
		}else if( type == INFLOW ){
			shape.setFillColor( sf::Color(255, 0, 255) );
		}else if( type == OUTFLOW ){
			shape.setFillColor( sf::Color(0, 0, 0) );
		}
	}
};

int main( int argc, char * * argv ){
	// 2D array of sells
	std::vector< std::vector<Cell> > cells;
	//window.setFramerateLimit(60);

	std::vector<Cell> temp_vector;
	for( int r = 0; r < cell_y_count; r++  ){
		for( int c = 0; c < cell_x_count; c++ ){
			temp_vector.push_back( Cell( c*CELL_SIZE, r*CELL_SIZE, CELL_SIZE, CELL_SIZE ) );
		}
		cells.push_back( temp_vector );
		temp_vector.clear();
	}

	for( int c = 0; c < cell_x_count; c++ ){
		//cells[2][c].water_content = 1;
		//cells[4][c].water_content = 1;
	}

	enum Cell_Type obs = OBSTACLE;
	enum Cell_Type flow = INFLOW;

	cells[1][19].changeType( flow );

	for( int c = 5; c < cell_x_count/4; c++ ){
		cells[cell_y_count - 8][c].changeType(obs);
		cells[c + 30][5].changeType(obs);
		cells[c + 30][15].changeType(obs);
	}

	for( int r = 0; r < 8; r++ ){
		cells[r][30].changeType(obs);
	}

	for( int r = 40; r < cell_y_count; r++ ){
		cells[r][30].changeType(obs);
	}


	int _y = 10;
	int _x = 0;
	cells[_y][30 + _x].changeType(obs);
	cells[_y][29 + _x].changeType(obs);
	cells[_y][28 + _x].changeType(obs);
	cells[_y][27 + _x].changeType(obs);
	cells[_y][26 + _x].changeType(obs);
	cells[_y][25 + _x].changeType(obs);
	cells[_y][24 + _x].changeType(obs);
	cells[_y][23 + _x].changeType(obs);

	_y = 11;
	_x = -5;
	cells[_y][30 + _x].changeType(obs);
	cells[_y][29 + _x].changeType(obs);
	cells[_y][28 + _x].changeType(obs);
	cells[_y][27 + _x].changeType(obs);
	cells[_y][26 + _x].changeType(obs);
	cells[_y][25 + _x].changeType(obs);
	cells[_y][24 + _x].changeType(obs);
	cells[_y][23 + _x].changeType(obs);

	_y = 12;
	_x = -9;
	cells[_y][30 + _x].changeType(obs);
	cells[_y][29 + _x].changeType(obs);
	cells[_y][28 + _x].changeType(obs);
	cells[_y][27 + _x].changeType(obs);
	cells[_y][26 + _x].changeType(obs);
	cells[_y][25 + _x].changeType(obs);
	cells[_y][24 + _x].changeType(obs);
	cells[_y][23 + _x].changeType(obs);

	_y = 9;
	_x = 5;
	cells[_y][30 + _x].changeType(obs);
	cells[_y][29 + _x].changeType(obs);
	cells[_y][28 + _x].changeType(obs);
	cells[_y][27 + _x].changeType(obs);
	cells[_y][26 + _x].changeType(obs);
	cells[_y][25 + _x].changeType(obs);
	cells[_y][24 + _x].changeType(obs);
	cells[_y][23 + _x].changeType(obs);

	_y = 8;
	_x = 5;
	cells[_y][30 + _x].changeType(obs);
	cells[_y][29 + _x].changeType(obs);
	cells[_y][28 + _x].changeType(obs);
	cells[_y][27 + _x].changeType(obs);
	cells[_y][26 + _x].changeType(obs);
	cells[_y][25 + _x].changeType(obs);
	cells[_y][24 + _x].changeType(obs);
	cells[_y][23 + _x].changeType(obs);



	// main loop
	while( window.isOpen() ){
		sf::Event event;
		while(window.pollEvent( event )){
			if( event.type == sf::Event::Closed ){
				window.close();
				return 0;
			}
		}

		window.clear();

		for( int r = 0; r < cell_y_count; r++  ){
			for( int c = 0; c < cell_x_count; c++ ){

				if( cells[r][c].type == INFLOW ){
					cells[r][c].water_content = 1;
				}

				if( cells[r][c].water_content > MIN_WATER && r < cell_y_count - 2){
					if( cells[r+1][c].water_content < cells[r+1][c].max_water && cells[r+1][c].type == EMPTY){	// if buttom cell not full

						float _diff = cells[r+1][c].max_water - cells[r+1][c].water_content;

						cells[r+1][c].water_content += cells[r][c].water_content;
						if( cells[r+1][c].water_content > cells[r+1][c].max_water ){ cells[r+1][c].water_content = cells[r+1][c].max_water; }

						cells[r][c].water_content -= _diff;
						if( cells[r][c].water_content < 0 ){ cells[r][c].water_content = 0; }
					}
				}

				if( cells[r][c-1].water_content != cells[r][c].water_content && c > 0 && cells[r][c-1].type == EMPTY  && cells[r][c].type == EMPTY){
					float _t_share = (cells[r][c-1].water_content + cells[r][c].water_content)/2;
					cells[r][c].water_content = _t_share;
					cells[r][c-1].water_content = _t_share;
				}

				if( cells[r][c+1].water_content != cells[r][c].water_content && c < cell_x_count - 1 && cells[r][c+1].type == EMPTY && cells[r][c].type == EMPTY ){
					float _t_share = (cells[r][c+1].water_content + cells[r][c].water_content)/2;
					cells[r][c].water_content = _t_share;
					cells[r][c+1].water_content = _t_share;
					}

				cells[r][c].draw();
			}
		}

		window.display();
	}

	return 0;
}
