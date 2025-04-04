//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
#include <SDL2/SDL_mixer.h>
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 60
#include <fstream>

string * dictionary;
int dictionarysize = 369646;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 1320, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
const int nalphabets = 26;
int Swidth = 930, Sheight = 660;
int col = Swidth / 30;
int row = Sheight / 30;
int tiles[22][44];
int shootbubble = rand() % 26;
float bvx = 0,bvy = 0;
bool moving = false;
int ox,oy;
int tempx = (width / 2) - 1,tempy = 8;
int timecount=120;
int nx =((width / 2)) + 30 ,ny = height - 650;
int countmoving = 0;
int cx,cy;
int sbx,sby;
int tempshootbubble = shootbubble;
int countb=0;
int checkall=0;
int numfdiagonals=0;
bool foundv = false,foundl = false, foundr = false, foundD = false;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(0, 25);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
	cx = (px - (awidth / 2)) / (awidth + 3);
	cy = (height - py - 50 - (aheight / 2)) / (aheight + 3);
}
void Cell2Pixels(int cx, int cy, int & px, int &py) {
	px = cx * (awidth + 3) + (awidth / 2);
	py = height - 50 - ((cy + 1) * (aheight + 3)) + (aheight / 2);
	px -= 30;
	py -= 20;
}

void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */

void DisplayFunction() {
	int ppx,ppy;
	int tempr,tempc;
	int cpx,cpy;
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions..
	for(int i = 0 ; i < 22 ; i++){
		for(int j = 0 ; j < 44 ; j++){
			if(tiles[i][j] != -1){
				Cell2Pixels(j,i,ppx,ppy);
				DrawAlphabet((alphabets)tiles[i][j], ppx, ppy, awidth, aheight);
				tempr = i;
				tempc = j;
			}
		}
	}
	DrawAlphabet((alphabets)shootbubble, nx- 30, ny, awidth, aheight);
	DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	DrawString(width / 2 - 30, height - 25, width, height,
		"Time Left:" + Num2Str(timecount) + " secs", colors[RED]);

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

void fillboard(){
	for(int i = 0 ; i < 22 ; i++){
		for(int j = 0 ; j < 44 ; j++){
			if(i < 2 && j < 21){
				tiles[i][j] = GetAlphabet();
			}else{
				tiles[i][j] = -1;
			}
		}
	}
}

void newbubble(){
	for(int i = 0 ; i < 2 ; i++){
		for(int j = 0 ; j < 44 ; j++){
			if(tiles[i][j] == -1){
				tiles[i][j] = GetAlphabet();
			}
		}
	}
}

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */
void MouseClicked(int button, int state, int x, int y) {
	double difx,dify,magntd;
	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{
			if(moving == false){
				ox = x;
				oy = y;
				difx = x - (((width / 2) - 1));
				dify = (height - y);
				magntd = sqrt((difx * difx) + (dify * dify));
				bvx = difx / magntd;
				bvy = dify / magntd;
				moving = true;	
			}

		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
int temph = 10;
void Timer(int m){
    int px,py,qx,qy,mx,my,lbound=0,ubound=369645,mid,lhc=0,rhc=0,vc=0,colsfordiagonals=0,tempc=0,dc=0;
    float dis;
	string temp="";
	string verticalword="",horizontalwordleft="",horizontalwordright="",diagonalword="";
	tempshootbubble = shootbubble;
	fstream readfile;
	readfile.open("storedwords.txt",ios::out | ios::app);

    if(moving == true){
        nx += (bvx * 12);
        ny += (bvy * 12);
        if((nx - bradius) < 0){
            nx = bradius;
            bvx = -bvx;
        }else{
			if((nx + bradius) > width){
				nx = width - bradius;
				bvx = -bvx;
			}
		}
        if((ny + bradius) > height){
            ny = height - bradius;
            moving = false;
            Pixels2Cell(nx,ny,cx,cy);
            if(cx >= 0 && cx < 44 && cy >= 0 && cy < 22){
                if(tiles[cy][cx] == -1){
                    tiles[cy][cx] = tempshootbubble;
					sbx = cx;
					sby = cy;
					Cell2Pixels(cx,cy,nx,ny);
                }
            }
            nx = (width / 2) + 30;
            ny = height - 650;
			tempshootbubble = shootbubble;
            shootbubble = GetAlphabet();
			if(m == 60){
				timecount -= 1;	
				m = 0;
			}
			glutPostRedisplay();
			glutTimerFunc(1000.0/FPS, Timer, m+1);
			return;
        }
        for(int i = 0; i < 22; i++){
            for(int j = 0; j < 44; j++){
                if(tiles[i][j] != -1){
                    Cell2Pixels(j,i,px,py);
                    dis = sqrt(((nx - px) * (nx - px)) + ((ny - py) * (ny - py)));
                    if(dis < (bradius * 2)){
                        Pixels2Cell(nx,ny,cx,cy);
                        if(cx >= 0 && cx < 44 && cy >= 0 && cy < 22){
                            if(tiles[cy][cx] == -1){
                                tiles[cy][cx] = tempshootbubble;
								sbx = cx;
								sby = cy;
								Cell2Pixels(cx,cy,nx,ny);
                            }else{
								moving = false;
                            }
                        }
                        moving = false;
                        nx = ((width / 2)) + 30;
                        ny = height - 650;
						tempshootbubble = shootbubble;
                        shootbubble = GetAlphabet();
						if(m == 60){
							timecount -= 1;	
							m = 0;
						}
						glutPostRedisplay();
						glutTimerFunc(1000.0/FPS, Timer, m+1);
						return;
                    }
                }
            }
        }
		countb += 1;
    }else{
		if(countb > 0){
			lbound=0;
			ubound=369645;
			lhc=0;
			rhc=0;
			vc=0;
			foundl = false;
			foundr = false;
			foundv = false;
			foundD = false;
			tempc = 0;
			colsfordiagonals = 0;
			if(foundl == false && foundr == false && foundv == false && foundD == false|| checkall <= sbx && checkall <= 21){ // add all the checks inside this
				for(int i = checkall; i <= sbx; i++){
					horizontalwordleft += char(tiles[1][i] + 97);
					lhc += 1;
				}
				for(int j = 2; j <= sby; j++){
					horizontalwordleft += char(tiles[j][sbx] + 97);
					lhc += 1;
				}
				while(lbound <= ubound && foundl == false){
					mid = (lbound + ubound) / 2;
					if(dictionary[mid] == horizontalwordleft){
						foundl = true;
					}else{
						if(dictionary[mid] < horizontalwordleft){
							lbound = mid + 1;
						}else{
							ubound = mid - 1;
						}
					}
				}
				lbound=0;
				ubound=369645;
				for(int i = 0; i <= sby-2; i++){
					horizontalwordright += char(tiles[sby-i][sbx] + 97);
					rhc += 1;
				}			
				for(int i = sbx; i < 21 - checkall; i++){
					horizontalwordright += char(tiles[1][i] + 97);
					rhc += 1;
				}
				while(lbound <= ubound && foundr == false){
					mid = (lbound + ubound) / 2;
					if(dictionary[mid] == horizontalwordright){
						foundr = true;
					}else{
						if(dictionary[mid] < horizontalwordright){
							lbound = mid + 1;
						}else{
							ubound = mid - 1;
						}
					}
				}
				lbound = 0;
				ubound=369645;
				for(int j = sby; j >= 0; j--){
					verticalword += char(tiles[j][sbx] + 97);
					vc += 1;
				}
				while(lbound <= ubound && foundv == false){
					mid = (lbound + ubound) / 2;
					if(dictionary[mid] == verticalword){
						foundv = true;
					}else{
						if(dictionary[mid] < verticalword){
							lbound = mid + 1;
						}else{
							ubound = mid - 1;
						}
					}
				}
				lbound = 0;
				ubound=369645;
				for(int j = 0; j <= sby; j++){
					colsfordiagonals +=1;
				}
				numfdiagonals = sbx - colsfordiagonals;
				for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
					diagonalword += char(tiles[tempc][i] + 97);
					tempc += 1;
					dc += 1;
				}
				while(lbound <= ubound && foundD == false){
					mid = (lbound + ubound) / 2;
					if(dictionary[mid] == diagonalword){
						foundD = true;
					}else{
						if(dictionary[mid] < diagonalword){
							lbound = mid + 1;
						}else{
							ubound = mid - 1;
						}
					}
				}
				readfile << horizontalwordleft << endl;
				readfile << horizontalwordright << endl;
				readfile << verticalword << endl;
				readfile << diagonalword << endl;
				readfile.close();
			}
			if(foundl == true && foundr == true && foundv == true && foundD == true){
				if(lhc > rhc && lhc > vc && lhc > dc){
					for(int i = checkall; i <= sbx; i++){
						tiles[1][i] = -1;
					}
					for(int j = 2; j <= sby; j++){
						tiles[j][sbx] = -1;
					}
					for(int i = 0 ; i < 22 ; i++){
						for(int j = 0 ; j < 44 ; j++){
							if(i < 2 && j < 21){
								tiles[i][j] = GetAlphabet();
							}else{
								tiles[i][j] = -1;
							}
						}
					}
					newbubble();
					score += lhc;
					glutPostRedisplay();
				}else{
					if(rhc > lhc && rhc > vc && rhc > dc){
						for(int i = 0; i <= sby-2; i++){
							tiles[sby-i][sbx] = -1;
						}			
						for(int i = sbx; i < 21 - checkall; i++){
							tiles[1][i] = -1;
						}
						newbubble();
						score += rhc;
						glutPostRedisplay();
					}else{
						if(vc > rhc && vc > lhc && vc > dc){
							for(int j = sby; j >= 0; j--){
								tiles[j][sbx] = -1;
							}
							newbubble();
							score += vc;
							glutPostRedisplay();
						}else{
							if(dc > rhc && dc > lhc && dc > vc){
								for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
									tiles[tempc][i] = -1;
								}
								newbubble();
								score += dc;
								glutPostRedisplay();
							}
						}
					}
				}
			}else{
				if(foundl == true && foundr == true && foundD == true && foundv == false){
					if(lhc > rhc && lhc > dc){
						for(int i = checkall; i <= sbx; i++){
							tiles[1][i] = -1;
						}
						for(int j = 2; j <= sby; j++){
							tiles[j][sbx] = -1;
						}
						newbubble();
						score += lhc;
						glutPostRedisplay();
					}else{
						if(rhc > lhc && rhc > dc){
							for(int i = 0; i <= sby-2; i++){
								tiles[sby-i][sbx] = -1;
							}			
							for(int i = sbx; i < 21 - checkall; i++){
								tiles[1][i] = -1;
							}
							newbubble();
							score += rhc;
							glutPostRedisplay();
						}else{
							if(dc > rhc && dc > lhc){
								for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
									tiles[tempc][i] = -1;
								}
								newbubble();
								score += dc;
								glutPostRedisplay();
							}
						}
					}
				}else{
					if(foundl == true && foundr == false && foundD == false && foundv == true){
						if(lhc > vc){
							for(int i = checkall; i <= sbx; i++){
								tiles[1][i] = -1;
							}
							for(int j = 2; j <= sby; j++){
								tiles[j][sbx] = -1;
							}
							newbubble();
							score += lhc;
							glutPostRedisplay();
						}else{
							if(lhc <= vc){
								for(int j = sby; j >= 0; j--){
									tiles[j][sbx] = -1;
								}
								newbubble();
								score += vc;
								glutPostRedisplay();
							}
						}
					}else{
						if(foundl == false && foundr == true && foundD == true && foundv == true){
							if(rhc > vc && rhc > dc){
								for(int i = 0; i <= sby-2; i++){
									tiles[sby-i][sbx] = -1;
								}			
								for(int i = sbx; i < 21 - checkall; i++){
									tiles[1][i] = -1;
								}
								newbubble();
								score += rhc;
								glutPostRedisplay();
							}else{
								if(vc > rhc && vc > dc){
									for(int j = sby; j >= 0; j--){
										tiles[j][sbx] = -1;
									}
									newbubble();
									score += vc;
									glutPostRedisplay();
								}else{
									if(dc > vc && dc > rhc){
										for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
											tiles[tempc][i] = -1;
										}
										newbubble();
										score += dc;
										glutPostRedisplay();	
									}
								}
							}
						}else{
							if(foundD == true && foundl == false && foundr == false && foundv == false){
								for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
									tiles[tempc][i] = -1;
								}
								newbubble();
								score += dc;
								glutPostRedisplay();
							}else{
								if(foundD == true && foundl == true && foundr == false && foundv == false){
									if(dc > lhc){
										for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
											tiles[tempc][i] = -1;
										}
										newbubble();
										score += dc;
										glutPostRedisplay();
									}else{
										if(dc <= lhc){
											for(int i = checkall; i <= sbx; i++){
												tiles[1][i] = -1;
											}
											for(int j = 2; j <= sby; j++){
												tiles[j][sbx] = -1;
											}
											newbubble();
											score += lhc;
											glutPostRedisplay();
										}
									}
								}else{
									if(foundD == true && foundl == false && foundr == true && foundv == false){
										if(dc > rhc){
											for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
												tiles[tempc][i] = -1;
											}
											newbubble();
											score += dc;
											glutPostRedisplay();
										}else{
											if(dc <= rhc){
												for(int i = 0; i <= sby-2; i++){
													tiles[sby-i][sbx] = -1;
												}			
												for(int i = sbx; i < 21 - checkall; i++){
													tiles[1][i] = -1;
												}
												newbubble();
												score += rhc;
												glutPostRedisplay();
											}
										}
									}else{
										if(foundD == true && foundl == false && foundr == false && foundv == true){
											if(dc > vc){
												for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
													tiles[tempc][i] = -1;
												}
												newbubble();
												score += dc;
												glutPostRedisplay();	
											}else{
												if(dc <= vc){
													for(int j = sby; j >= 0; j--){
														tiles[j][sbx] = -1;
													}
													newbubble();
													score += vc;
													glutPostRedisplay();
												}
											}
										}else{
											if(foundD == true && foundl == true && foundr == true && foundv == false){
												if(dc > lhc && dc > rhc){
													for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
														tiles[tempc][i] = -1;
													}
													newbubble();
													score += dc;
													glutPostRedisplay();		
												}else{
													if(lhc > dc && lhc > rhc){
														for(int i = checkall; i <= sbx; i++){
															tiles[1][i] = -1;
														}
														for(int j = 2; j <= sby; j++){
															tiles[j][sbx] = -1;
														}
														for(int i = 0 ; i < 22 ; i++){
															for(int j = 0 ; j < 44 ; j++){
																if(i < 2 && j < 21){
																	tiles[i][j] = GetAlphabet();
																}else{
																	tiles[i][j] = -1;
																}
															}
														}
														newbubble();
														score += lhc;
														glutPostRedisplay();
													}else{
														if(rhc > dc && rhc > lhc){
															for(int i = 0; i <= sby-2; i++){
																tiles[sby-i][sbx] = -1;
															}			
															for(int i = sbx; i < 21 - checkall; i++){
																tiles[1][i] = -1;
															}
															newbubble();
															score += rhc;
															glutPostRedisplay();
														}
													}
												}
											}else{
												if(foundD == true, foundl == true, foundr == false, foundv == true){
													if(dc > lhc && dc > vc){
														for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
															tiles[tempc][i] = -1;
														}
														newbubble();
														score += dc;
														glutPostRedisplay();
													}else{
														if(lhc > dc && lhc > vc){
															for(int i = checkall; i <= sbx; i++){
																tiles[1][i] = -1;
															}
															for(int j = 2; j <= sby; j++){
																tiles[j][sbx] = -1;
															}
															for(int i = 0 ; i < 22 ; i++){
																for(int j = 0 ; j < 44 ; j++){
																	if(i < 2 && j < 21){
																		tiles[i][j] = GetAlphabet();
																	}else{
																		tiles[i][j] = -1;
																	}
																}
															}
															newbubble();
															score += lhc;
															glutPostRedisplay();
														}else{
															if(vc > dc && vc > lhc){
																for(int j = sby; j >= 0; j--){
																	tiles[j][sbx] = -1;
																}
																newbubble();
																score += vc;
																glutPostRedisplay();
															}
														}
													}
												}else{
													if(foundD == true && foundl == false && foundr == true && foundv == true){
														if(dc > rhc && dc > vc){
															for(int i = numfdiagonals+1; i <= sbx-checkall; i++){
																tiles[tempc][i] = -1;
															}
															newbubble();
															score += dc;
															glutPostRedisplay();
														}else{
															if(rhc > dc && rhc > lhc){
																for(int i = 0; i <= sby-2; i++){
																	tiles[sby-i][sbx] = -1;
																}			
																for(int i = sbx; i < 21 - checkall; i++){
																	tiles[1][i] = -1;
																}
																newbubble();
																score += rhc;
																glutPostRedisplay();
															}else{
																if(lhc > dc && lhc > rhc){
																	for(int i = checkall; i <= sbx; i++){
																		tiles[1][i] = -1;
																	}
																	for(int j = 2; j <= sby; j++){
																		tiles[j][sbx] = -1;
																	}
																	for(int i = 0 ; i < 22 ; i++){
																		for(int j = 0 ; j < 44 ; j++){
																			if(i < 2 && j < 21){
																				tiles[i][j] = GetAlphabet();
																			}else{
																				tiles[i][j] = -1;
																			}
																		}
																	}
																	newbubble();
																	score += lhc;
																	glutPostRedisplay();
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		    checkall += 1;
		}
	}
	if(m == 60){
        timecount -= 1;		
		m = 0;
	}
	if(timecount <= 0){
		timecount = 0;
		return;
	}
    glutPostRedisplay();
    glutTimerFunc(1000.0/FPS, Timer, m+1);
}

/*
* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets
	fillboard();
	Mix_Music* bgMusic = Mix_LoadMUS("/home/adam/release-Ubuntu/just a chill guy listening to chill guy music.mp3");
	Mix_PlayMusic(bgMusic, -1);
	Mix_VolumeMusic(32);

	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Adam 24I-0013"); // set the title of our game window
	//SetCanvasSize(660, 910); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */