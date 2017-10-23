#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

void display_callback( void );
void render( int );
void init_glut( void );
void resize_callback( int, int );
void draw_rect( float, float, float, float );
void draw_rect_outline( float, float, float, float, float );
void draw_circle( float, float, float, float );
void mouse_passive_callback( int, int );
void mouse_motion_callback( int, int );
void mouse_click_callback( int, int, int, int );
void add_new_particle( float, float, float, float );

int window_width = 600;
int window_height = 400;
int frame_delay = 10;
int particle_creation_size = 20;
int particle_creation_collision_size = 10;
int particle_creation_interval = 5;
int prtcl_i = 0;
int attraction_distance = 500;
float attraction_strength = 1000;
float attraction_distance_factor = 0.1;
float max_particle_acc = 5;
float friction = 0.99;
float gravity = 1;
float wall_force = 1000;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASSES


class Particle{
	public:
	float x;
	float y;
	float v_x;
	float v_y;
	float a_x;
	float a_y;
	float size;
	float collision_size;
	
	Particle(){
		x = 0;
		y = 0;
	}

	void init( float _x, float _y, float _s ){
		x = _x;
		y = _y;
		size = _s;
		collision_size = _s;	
	}

	void update( void ){
		if( abs(a_x) > max_particle_acc ){
			a_x = max_particle_acc * (a_x/abs(a_x));
		}if( abs(a_y) > max_particle_acc){
			a_y = max_particle_acc * (a_y/abs(a_y));
		}	

		v_x += a_x;
		v_x = a_x * friction;

		v_y += a_y;
		v_y = a_y * friction;

		x += v_x;
		y += v_y;
	}

	void draw( void ){
		draw_circle( x, y, size, 20 );
	}

	void add_force( float _x, float _y ){
		a_x += _x;
		a_y += _y;
	}

	void clear_forces( void ){
		a_x = gravity;
		a_y = gravity;
	}

	void set_coll_size( float cs ){
		collision_size = cs;
	}

};


Particle * particles = new Particle[0];
int particles_len = 0;


void add_new_particle( float _x, float _y, float _s, float _cs ){

	Particle * temp_arr = new Particle[ particles_len + 1 ];
	std::copy( particles, particles + particles_len, temp_arr );
	delete particles;
	particles = temp_arr;
	particles_len += 1;
	particles[ particles_len -1 ].init( (float)_x, (float)_y, (float)_s );	
	particles[ particles_len -1 ].set_coll_size( _cs );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN


int main( int argc, char * * argv ){
	
	glutInit( &argc, argv );
	glutInitWindowSize( window_width, window_height );
	glutInitWindowPosition( 0, 0 );
	glutInitDisplayMode( GLUT_DOUBLE );
	glutCreateWindow( "fluid sim" );

	init_glut( );	
	glutDisplayFunc( display_callback );
	glutReshapeFunc( resize_callback );	
	glutMouseFunc( mouse_click_callback );
	glutMotionFunc( mouse_motion_callback );
	glutPassiveMotionFunc( mouse_passive_callback );

	//add_new_particle( 100,100,10 );


	

	glutMainLoop();
	
	
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// settings

void init_glut( void ){
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glViewport( 0.0, 0.0, window_width, window_height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();	
	
	glOrtho( 0.0, window_width, window_height, 0.0, 1.0, -1.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}


void resize_callback( int x, int y ){
	window_width = x;
	window_height = y;
	init_glut();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// render

void display_callback( void ){
	
	glClearColor( 1.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	glutSwapBuffers();	
	glutTimerFunc( frame_delay, render, 0  );
}

void render( int a ){

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );

	float temp_dist;
	float x_diff;
	float y_diff;	
	float angle;
	for( int i = 0; i < particles_len; i++ ){
		particles[i].clear_forces();
		particles[i].add_force( 0, gravity );
		for( int x = 0; x < particles_len; x++ ){
			if( x != i ){
			temp_dist = ((particles[i].x - particles[x].x) * (particles[i].x - particles[x].x)) + ((particles[i].y - particles[x].y) * (particles[i].y - particles[x].y));
			angle = abs(asin((particles[i].y - particles[x].y)/(particles[i].x - particles[x].x)));			
			x_diff = (particles[i].x - particles[x].x);
			y_diff = (particles[i].y - particles[x].y);
				if( temp_dist <= attraction_distance * attraction_distance && temp_dist >= (particles[i].collision_size + particles[x].collision_size)*(particles[i].collision_size + particles[x].collision_size) ){
					std::cout << (angle * 180) / PI << "\n";	
					particles[i].add_force(  (-abs(x_diff)/x_diff) * attraction_strength * cos(angle) / temp_dist, (-abs(y_diff)/y_diff) * attraction_strength * sin(angle) / temp_dist );
				}else if(temp_dist < (particles[i].collision_size + particles[x].collision_size)*(particles[i].collision_size + particles[x].collision_size) ){
					particles[i].add_force( (abs(x_diff)/x_diff) * 100 * attraction_strength * cos(angle) / temp_dist, (abs(y_diff)/y_diff) * 100 * attraction_strength * sin(angle) / temp_dist);
				}			
				
			}
		}

		// check collision with border
		if(particles[i].x-particles[i].collision_size < 0){
			particles[i].add_force( wall_force, 0 );
		}else if( particles[i].x+particles[i].collision_size > window_width ){
			particles[i].add_force( -wall_force, 0 );
		}else if(particles[i].y+particles[i].collision_size > window_height){
			particles[i].add_force( 0, -wall_force );
		}else if( particles[i].y-particles[i].collision_size < 0 ){
			particles[i].add_force( 0, wall_force );
		}
				
		particles[i].update();		
		particles[i].draw();	
	}



	glutSwapBuffers();
	glutTimerFunc( frame_delay, render, 0  );

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// input


void mouse_click_callback( int button, int state, int x, int y ){
	if( button == 0 && state == 0 ){
		add_new_particle( x, y, particle_creation_size, particle_creation_collision_size );	
	}	
	//std::cout << state << " " << button << "\n";
}

void mouse_passive_callback( int x, int y ){
	//std::cout << "passive: " << x << " " << y << " \n";
}

void mouse_motion_callback( int x, int y ){
	//std::cout << "active: " << x << " " << y << "adding particle" << " \n";
	prtcl_i += 1;
	if( prtcl_i >= particle_creation_interval){	
		add_new_particle( x, y, particle_creation_size, particle_creation_collision_size );
		prtcl_i = 0;
	}	
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
	draw_rect( x,y,width, thickness );
	draw_rect( x,y,thickness, height );		
	draw_rect( x+width,y,thickness, height );
	draw_rect( x,y+width,width, thickness );		
}

void draw_circle( float x, float y, float r, float angle_interval ){
	glBegin( GL_POLYGON );
		for( int i = 0; i < 360; i += angle_interval ){
			glVertex3f( x + r*cos(i*(PI/180)), y + r*sin(i*(PI/180)), 0.0 );
		}	
	glEnd();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////



