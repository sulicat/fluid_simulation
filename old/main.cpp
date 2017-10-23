#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>


// global VARS
int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 400;
int FRAME_TIME = 25;

std::string MAIN_FLAG = "play";
std::string SECOND_FLAG = "play";
std::string MOUSE_MODE = "up";

float GRAVITY_Y = 0.05;
float GRAVITY_X = 0;
float NODE_BREAKING_POINT = 20;
float NODE_LINK_DISTANCE = 100;
float NODE_POINT_SIZE = 5;


// classes
class NODE{
	public:
	float x;
	float y;
	float vx;
	float vy;
	float ax;
	float ay;
	NODE * links;	
	int links_len; 	

	void init( float x_, float y_ ){
		x = x_;
		y = y_;
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;	
		links = new NODE[0];	
		links_len = 0;	
	}

	void set_accel( float ax_, float ay_ ){
		ax = ax_;
		ay = ay_;
	}

	void clear_links( void ){
		delete[] links;
		links = new NODE[0];
		links_len = 0;
	}

	void create_link( NODE  &n, int again ){
		NODE * temp_arr = new NODE[ links_len+1 ];	
		std::copy( links, links + links_len +1, temp_arr );
		delete[] links;
		links = temp_arr;
		links_len += 1;
		
		links[ links_len-1 ] = n;
		if( again == 1 ){
			n.create_link( *this, 0 );
		}
	}

	void draw_links( void ){
		glColor3f( 0.0, 0.0, 1.0 );		
		for( int i = 0; i < links_len; i++ ){
			glLineWidth(5);			
			glBegin( GL_LINES );
				glVertex2f( x, y );
				glVertex2f( links[i].x, links[i].y );
			glEnd();
		}
	}


	void update( void ){
		float sum_ax = 0 + GRAVITY_X;
		float sum_ay = 0 + GRAVITY_Y;
		vx += sum_ax;
		vy += sum_ay;
		x += vx;
		y += vy;
	}


};

NODE * nodes = new NODE[0];
int nodes_len = 0;




// prototypes
void display_callback( void );
void render_callback( int );
void keyboard_in_callback( unsigned char, int, int );
void mouse_in_callback( int, int, int, int );
void mouse_clicked_move_callback( int, int );
void draw_rect( float, float, float, float );
void draw_rect_outline( float, float, float, float, float, float );
void update_links( void );
float get_distance( NODE, NODE );


// cycle through node array and connect nodes that want to be connected
void update_links( void ){
	for( int i = 0; i < nodes_len; i++ ){
		//nodes[i].clear_links();		
		for( int x = i; x < nodes_len; x++ ){
			
			if( get_distance( nodes[i], nodes[x]) <= NODE_LINK_DISTANCE ){
				nodes[i].create_link( nodes[x], 1 );	
				std::cout << "connect: " << nodes[i].links_len << "  " << get_distance( nodes[i], nodes[x]) << "\n";
			}
		}	
	}
}


// get distance between 2 nodes
float get_distance( NODE n1, NODE n2 ){
	float dx = abs( n1.x - n2.x );
	float dy = abs( n1.y - n2.y );
	return sqrt((dx * dx) + (dy * dy));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////

// callback functions 

void display_callback( void ){
	glClear( GL_COLOR_BUFFER_BIT );
	glutSwapBuffers();
}

void render_callback( int a ){
	glClear( GL_COLOR_BUFFER_BIT );
	update_links();
	for( int i = 0; i < nodes_len; i++ ){
		glColor3f( 1.0, 0.0, 0.0 );
		draw_rect( nodes[i].x - NODE_POINT_SIZE/2, nodes[i].y - NODE_POINT_SIZE/2, NODE_POINT_SIZE, NODE_POINT_SIZE );
		nodes[i].draw_links();
		nodes[i].update();
	}	
	
	glutSwapBuffers();
	glutTimerFunc( FRAME_TIME, render_callback, 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void init_gl( void ){

	// set the background color
	glClearColor( 0.0, 0.0, 0.0, 1.0 );		
	glViewport( 0.0, 0.0, SCREEN_WIDTH, SCREEN_HEIGHT );

	// reset the model and projection matrices
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );	
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity(); 

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//input

void keyboard_in_callback( unsigned char key, int x, int y ){
	std::cout << key << "\n";
}

void mouse_in_callback( int button, int state, int x, int y ){
	std::cout << "x: " << x << " y: " << y << " state:  " << state << "\n";

	if( state == 0 && button == 0){
		MOUSE_MODE = "down";
		NODE * temp_arr = new NODE[ nodes_len+1 ];	
		std::copy( nodes, nodes + nodes_len +1, temp_arr );
		delete[] nodes;
		nodes = temp_arr;
		nodes_len += 1;
	
		nodes[ nodes_len - 1 ].init( (float)x, (float)y );

	}else if(state == 1 && button == 0){
		MOUSE_MODE = "up";
		update_links();	
	}

	std::cout << MOUSE_MODE << "\n";	
}

void mouse_clicked_move_callback( int x, int y ){
	// this is to resize the nodes array dynamically
	// make new array with new size
	// copy contents to new array
	// delete old array
	// point old array to new array	
/*
	NODE * temp_arr = new NODE[ nodes_len+1 ];	
	std::copy( nodes, nodes + std::min( nodes_len, nodes_len +1 ), temp_arr );
	delete[] nodes;
	nodes = temp_arr;
	nodes_len += 1;
	
	nodes[ nodes_len - 1 ].init( (float)x, (float)y );
*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// drawing funcs

void draw_rect( float x, float y, float width, float height ){
	glBegin( GL_POLYGON );
		glVertex3f( x, y, 0.0 );
		glVertex3f( x, y+height, 0.0 );
		glVertex3f( x+width, y+height, 0.0 );
		glVertex3f( x+width, y, 0.0 );
	glEnd();	
}


void draw_rect_outline( float x, float y, float width, float height, float thickness ){
	glBegin( GL_POLYGON );
		draw_rect( x,y,width, thickness );
		draw_rect( x,y,thickness, height );		
		draw_rect( x+width,y,thickness, height );
		draw_rect( x,y+width,width, thickness );		
	glEnd();	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char * * argv ){

	glutInit( &argc, argv );
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	glutInitWindowPosition( 0, 0 );	
	glutInitDisplayMode( GLUT_DOUBLE );				// make it a double buffer window
	glutCreateWindow( "fluid ?" );
	
	init_gl();
	
	glutDisplayFunc( display_callback );	
	glutTimerFunc( FRAME_TIME, render_callback, 0 );
	glutKeyboardFunc( keyboard_in_callback );
	glutMouseFunc( mouse_in_callback );	
	glutMotionFunc( mouse_clicked_move_callback );	

	glutMainLoop();

	return 0;
}
