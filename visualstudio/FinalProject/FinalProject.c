
/************************************************************************************

File: 			FinalProject.c

Description:	Flight Simulator

****************************BONUS*****************************
BONUS FEATURE:

Press z to do a barrel roll. The bonus feature takes the current tilt angle and applys it to
the barrel roll angle to make a smooth barrel roll. I set it up so you can only do 1 barrel roll
at a time allowing for a smooth transition factoring in the tilt angle. If the mouse is to the left,
the plane will do a barrel roll right If the mouse is to the right, the plane will do a barrel roll
to the left.

***************************************************************
Author:			Kyle Asaff

*************************************************************************************/


/* include the library header files */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>

#define ESC 27
#define C 100 // sets the grid size to be 100x100 with center 50
#define PI (3.141592653589793)

#define GRIDSIZE 40 // makes 40x40 mountain grid size (1600 quads)


/************************************************************************

Function:      Speed Variables

Description:    adjust these to change the speeds of the animations

*************************************************************************/

// The speed the space ship moves at
GLfloat speedCamera = 0.001 * 1;

GLfloat planeSpeed = 0.01 * 1;

GLfloat planeSpeedInterval = 0.0001 * 1;

// Adjust propeller speed
GLfloat rotationSpeed = 0.1 * 10;

GLfloat tiltSpeed = 10 * 1;

// Adjust barrelroll speed
GLfloat barrelRollSpeed = 0.01 * 1;

// the boolean to check which way to barrel roll
int barrelLeft = 0;

/************************************************************************



*************************************************************************/

// texture variables
GLuint skyID, seaID, mountainID;

// used to store ppm height and width
int imageWidth, imageHeight;


//  position the light source at the origin 
GLfloat lightPosition[] = {0.0, 1000.0, 0.0, 0.0};

// struct for each quad in the mountain mesh
struct block {
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

// initalize structs to create mountains
struct block blocks[GRIDSIZE + 1][GRIDSIZE + 1];
struct block tempblock;
struct block nblock;
struct block allNormals[GRIDSIZE + 1][GRIDSIZE + 1][4];
struct block avgNormals[GRIDSIZE + 1][GRIDSIZE + 1];

// the struts for random mountain 1
struct block mountain1[GRIDSIZE + 1][GRIDSIZE + 1];
struct block mountain1AllNormals[GRIDSIZE + 1][GRIDSIZE + 1][4];
struct block mountain1AvgNormals[GRIDSIZE + 1][GRIDSIZE + 1];

// the structs for random mountain 2
struct block mountain2[GRIDSIZE + 1][GRIDSIZE + 1];
struct block mountain2AllNormals[GRIDSIZE + 1][GRIDSIZE + 1][4];
struct block mountain2AvgNormals[GRIDSIZE + 1][GRIDSIZE + 1];

// the structs for random mountain 3
struct block mountain3[GRIDSIZE + 1][GRIDSIZE + 1];
struct block mountain3AllNormals[GRIDSIZE + 1][GRIDSIZE + 1][4];
struct block mountain3AvgNormals[GRIDSIZE + 1][GRIDSIZE + 1];


// a material that is all zeros
GLfloat zeroMaterial[] = { 0.0, 0.0, 0.0, 1.0 };
// a red ambient material
GLfloat redAmbient[] = { 0.83, 0.0, 0.0, 1.0 };
// a dark green diffuse material
GLfloat darkGreenDiffse[] = { 0.0, 1.0, 0.0, 1.0 };
// a blue diffuse material
GLfloat lightblueDiffuse[] = { 0.1, 0.5, 0.8, 1.0 };
// a blue diffuse material
GLfloat blueDiffuse[] = { 0.0, 0.0, 1.0, 1.0 };
// a yellow diffuse material
GLfloat yellowDiffuse[] = {1.0, 1.0, 0.0, 1.0 };
// a black diffuse material
GLfloat blackDiffuse[] = {0.0, 0.0, 0.0, 1.0 };
// a light purple diffuse material
GLfloat lightPurpleDiffuse[] = {0.87, 0.58, 0.98, 1.0 };
// a red diffuse material
GLfloat redDiffuse[] = { 1.0, 0.0, 0.0, 1.0 };
// a dark diffuse material
GLfloat darkGreenDiffuse[] = { 0.0, 0.38, 0.199, 1.0 };
// a green diffuse material
GLfloat greenDiffuse[] = { 0.0, 1.0, 0.0, 1.0 };
// a white diffuse material
GLfloat whiteDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
// a white specular material
GLfloat greyDiffuse[] = { 0.75, 0.75, 0.75, 0.75 };
// a white specular material
GLfloat whiteSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
// a white specular material
GLfloat whiteSpecular2[] = { 1.0, 1.0, 1.0, 0.5 };

// set fog color pink
GLfloat fogColor[4] = {0.737255, 0.560784, 0.560784};

// calculate degrees to radians
GLfloat rad = PI/180.0;

// angles for titling the plane direction
GLfloat tiltAngle = 0.0;
GLfloat angleRate = 0.0;
GLfloat planeAngle = 0.0;

// toggles for features
int enableWireFrame = 1;
int enableGrid = 1;
int enableFullscreen = 0;
int enableFog = 0;
int enableMountainTex = 0;
int enableMountains = 0;

int enableBarrelRoll = 0;

// mouse position
float mousex = 0;
float mousey = 0;

/********************************************************/
/* Arrays for the plane/propeller  faces and vertices  */
/******************************************************/

// Allocate enough array space for sub object 0-3 (yellow)
GLfloat face03[432 + 6 + 253 + 10][30];
// Allocate enough array space for sub object 4-5 (black)
GLfloat face45[19 + 137][30];
// Allocate enough array space for sub object 6 (light purple)
GLfloat face6[186][30];
// Allocate enough array space for sub object 7 (blue)
GLfloat face7[175][30];
// Allocate enough array space for sub object 8-10 (yellow)
GLfloat face810[9 + 10 + 33][30];
// Allocate enough array space for sub object 11 (black)
GLfloat face11[3][30];
// Allocate enough array space for sub object 12-13 (yellow)
GLfloat face1213[2 + 5][30];
// Allocate enough array space for sub object 14-25 (black)
GLfloat face1425[14 + 33 + 27 + 63 + 14 + 106 + 27 + 5 + 162 + 453 + 693 + 20][30];
// Allocate enough array space for sub object 26-32 (yellow)
GLfloat face2632[18 + 52 + 328 + 36 + 91 + 56 + 162][30];

// Allocate enough array space for sub object 0 of propeller
GLfloat proface0[120][30];
// Allocate enough array space for sub object 1 of propeller
GLfloat proface1[12][30];

// Arrays for the propeller
GLfloat provertex[6763][3];
GLfloat pronormal[6763][3];

// Arrays for the plane
GLfloat vertex[6763][3];
GLfloat normal[6763][3];

/*******************************************************/
/******************************************************/

// The camera movement
GLfloat zMove = 0.0;
GLfloat xMove = 0.0;
GLfloat yMove = 0.0;

// Thetas for axis (x, y, z)
static GLfloat theta[] = { 0.0, 0.0, 0.0 };

GLfloat barrelRoll = 0.0;

// inital plane/camera position
GLfloat xCamera = 0.0;
GLfloat yCamera = 1.5;
GLfloat zCamera = 0.0;

// vector representing inital camera direction
GLfloat xlCamera = 0.0;
GLfloat ylCamera = 0.5;
GLfloat zlCamera = 0.0;

// window dimensions
GLint windowWidth = 1000;
GLint windowHeight = 800;

/************************************************************************

Function:      randomNumber

Description:    Generates a random float number between 0.0 and a given max

*************************************************************************/

float randomNumber(float max) {
	float x = ((float)rand() / (float)(RAND_MAX)) * max;
	return x;
}

/************************************************************************

Function:		mountainGrid

Description:	Generates 3 random mountain grids with a random midpoint 
				algorithm

*************************************************************************/
void mountainGrid(struct block blocks[GRIDSIZE+1][GRIDSIZE+1]) {


	int i, j, level;
	int idown = 0; // 0 = count up, 1 = count down
	int jdown = 0; // 0 = count up, 1 = count down
	float random; // holds the random number
	for (i = 0; i < GRIDSIZE + 1; i++) {
		// random = randomNumber(0.5) - 0.25;
		// count up the mountain
		if (idown == 0)
			level = i;
		// count down the mountain
		if (idown == 1)
			level--;
		// switch to count down the mountain
		if (level == (GRIDSIZE + 1) / 2)
			idown = 1;
		// currentlevel starts the mountain count at 0 inside the j loop
		int currentlevel = -1;
		// initalize jdown to count up the mountain
		int jdown = 0;
		for (j = 0; j < GRIDSIZE + 1; j++) {
			random = randomNumber(0.5) - 0.25;
			//level division, divide by 2 at each level
			float leveldivision = 1 / (pow(2, currentlevel));
			int insidelevel = currentlevel;
			// start counting down the mountain
			if (currentlevel == ((GRIDSIZE + 1) - j) && jdown == 0)
				jdown = 1;
			// increase mountain height
			if (currentlevel < level && jdown == 0)
				currentlevel++;
			// decrease the mountain height
			if (jdown == 1)
				currentlevel--;
			float leveldvision = (leveldivision) / (1 / (pow(2, insidelevel)));
			blocks[i][j].x = i;
			blocks[i][j].z = j;
			if (currentlevel != 0)
				// sets the y at a random height
				blocks[i][j].y = ((currentlevel*0.25) + random)*leveldvision;
			else
				// sets the y at the ground (0) if the level is 0
				blocks[i][j].y = (currentlevel*0.25);
		}
	}
}

/************************************************************************

Function:		drawMountain

Description:	draws Mountains

*************************************************************************/
void drawMountain(struct block blocks[GRIDSIZE + 1][GRIDSIZE + 1], struct block avgNormals[GRIDSIZE + 1][GRIDSIZE + 1]) {
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	int i, j;

	float gridsize = GRIDSIZE;

	// get the highest point of the mountain to make the tops as a function of Y
	GLfloat highestpoint = blocks[GRIDSIZE/2][GRIDSIZE/2].y;
	GLfloat whiteCapRange = highestpoint - 1.5;
	GLfloat midRange = highestpoint / 2;

	// for every quad in the mesh, draw the quad
	for (i = 0; i < GRIDSIZE; i++) {
		for (j = 0; j < GRIDSIZE; j++) {

			// Color the top of the mountains white, bottom green if textures = off
			if (enableMountainTex == 0) {
				if (blocks[i][j].y > whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else if (blocks[i][j].y > midRange && blocks[i][j].y < whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
			}
			// Turn off green & white color if mountain texture = on
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
				glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
				glMaterialf(GL_FRONT, GL_SHININESS, 25);
			}

			// Draw normal and vertex of specified quad in mountain mesh
			glNormal3f(avgNormals[i][j].x, avgNormals[i][j].y, avgNormals[i][j].z);
			glTexCoord2f(0/gridsize + i/gridsize, 0/gridsize + j/gridsize);
			glVertex3f(blocks[i][j].x, blocks[i][j].y, blocks[i][j].z);

			// Color the top of the mountains white, bottom green if textures = off
			if (enableMountainTex == 0) {
				if (blocks[i][j].y > whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else if (blocks[i][j].y > midRange && blocks[i][j].y < whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
			}
			// Turn off green & white color if mountain texture = on
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
				glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
				glMaterialf(GL_FRONT, GL_SHININESS, 25);
			}

			// Draw normal and vertex of specified quad in mountain mesh
			glNormal3f(avgNormals[i][j + 1].x, avgNormals[i][j + 1].y, avgNormals[i][j + 1].z);
			glTexCoord2f(0 / gridsize + (i) / gridsize, 1 / gridsize + (j + 1) / gridsize);
			glVertex3f(blocks[i][j + 1].x, blocks[i][j + 1].y, blocks[i][j + 1].z);

			// Color the top of the mountains white, bottom green if textures = off
			if (enableMountainTex == 0) {
				if (blocks[i][j].y > whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else if (blocks[i][j].y > midRange && blocks[i][j].y < whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
			}
			// Turn off green & white color if mountain texture = on
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
				glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
				glMaterialf(GL_FRONT, GL_SHININESS, 25);
			}

			// Draw normal and vertex of specified quad in mountain mesh
			glNormal3f(avgNormals[i + 1][j + 1].x, avgNormals[i + 1][j + 1].y, avgNormals[i + 1][j + 1].z);
			glTexCoord2f(1/gridsize + (i+1)/gridsize, 1/gridsize + (j+1)/gridsize);
			glVertex3f(blocks[i+1][j+1].x, blocks[i+1][j+1].y, blocks[i+1][j+1].z);

			// Color the top of the mountains white, bottom green if textures = off
			if (enableMountainTex == 0) {
				if (blocks[i][j].y > whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else if (blocks[i][j].y > midRange && blocks[i][j].y < whiteCapRange) {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, greenDiffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
				else {
					glShadeModel(GL_SMOOTH);
					glMaterialfv(GL_FRONT, GL_AMBIENT, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, darkGreenDiffse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
					glMaterialf(GL_FRONT, GL_SHININESS, 25);
				}
			}
			// Turn off green & white color if mountain texture = on
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
				glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
				glMaterialf(GL_FRONT, GL_SHININESS, 25);
			}
			// Draw normal and vertex of specified quad in mountain mesh
			glNormal3f(avgNormals[i + 1][j].x, avgNormals[i + 1][j].y, avgNormals[i + 1][j].z);
			glTexCoord2f(1/gridsize + (i+1)/gridsize, 0/gridsize + j/gridsize);
			glVertex3f(blocks[i+1][j].x, blocks[i+1][j].y, blocks[i+1][j].z);
		}
	}
	glEnd();
}


/************************************************************************

Function:		crossProduct

Description:	calculates cross product for the normals
				vec = vector you wish to calculate the cross product for

*************************************************************************/
struct block cross(struct block v1, struct block v2, struct block v3, struct block v4, int vec) {
	// calculate cross depending on what vector specified with vec
	if (vec == 0) {
		nblock.x = ((v4.y * v2.z) - (v4.z * v2.y))*-1;
		nblock.y = ((v4.z * v2.x) - (v4.x * v2.z))*-1;
		nblock.z = ((v4.x * v2.y) - (v4.y * v2.x))*-1;
	}
	else if (vec == 1) {
		nblock.x = ((v2.y * v1.z) - (v2.z * v1.y))*-1;
		nblock.y = ((v2.z * v1.x) - (v2.x * v1.z))*-1;
		nblock.z = ((v2.x * v1.y) - (v2.y * v1.x))*-1;
	}
	else if (vec == 2) {
		nblock.x = ((v2.y * v3.z) - (v2.z * v3.y))*-1;
		nblock.y = ((v2.z * v3.x) - (v2.x * v3.z))*-1;
		nblock.z = ((v2.x * v3.y) - (v2.y * v3.x))*-1;
	}
	else if (vec == 3) {
		nblock.x = ((v4.y * v3.z) - (v4.z * v3.y))*-1;
		nblock.y = ((v4.z * v3.x) - (v4.x * v3.z))*-1;
		nblock.z = ((v4.x * v3.y) - (v4.y * v3.x))*-1;
	}
	// return a block with the cross product
	return nblock;
}

/************************************************************************

Function:		mountainNormals

Description:	calculate all normals of a mountain
				3D size is 4 because 4 is the maximum amount of normals at one vertex in quad

*************************************************************************/
void mountainNormals(struct block allNormals[GRIDSIZE + 1][GRIDSIZE + 1][4], struct block blocks[GRIDSIZE + 1][GRIDSIZE + 1]) {
	int i, j, k, storex, storey, storez;

	// initalize array to -999 so we can check if the spot is filled with a normal value
	for (i = 0; i < GRIDSIZE + 1; i++) {
		for (j = 0; j < GRIDSIZE + 1; j++) {
			for (k = 0; k < 4; k++) {
				allNormals[i][j][k].x = -999;
				allNormals[i][j][k].y = -999;
				allNormals[i][j][k].z = -999;
			}
		}
	}

	for (i = 0; i < GRIDSIZE; i++) {
		for (j = 0; j < GRIDSIZE; j++) {
			// get the cross product of current quad in iteration
			tempblock = cross(blocks[i][j], blocks[i][j + 1], blocks[i + 1][j + 1], blocks[i + 1][j], 0);

			// reset the storage values (used to check if a spot is filled with a normal)
			storex = 0;
			storey = 0;
			storez = 0;

			// store normal in a spot that doesn't have a normal already
			while (allNormals[i][j][storex].x != -999)
				storex++;
			while (allNormals[i][j][storey].y != -999)
				storey++;
			while (allNormals[i][j][storez].z != -999)
				storez++;

			// store normal in an empty spot
			allNormals[i][j][storex].x = tempblock.x;
			allNormals[i][j][storey].y = tempblock.y;
			allNormals[i][j][storez].z = tempblock.z;

			// get the cross product of current quad in iteration
			tempblock = cross(blocks[i][j], blocks[i][j + 1], blocks[i + 1][j + 1], blocks[i + 1][j], 1);

			// reset the storage values (used to check if a spot is filled with a normal)
			storex = 0;
			storey = 0;
			storez = 0;

			// store normal in a spot that doesn't have a normal already
			while (allNormals[i][j + 1][storex].x != -999)
				storex++;
			while (allNormals[i][j + 1][storey].y != -999)
				storey++;
			while (allNormals[i][j + 1][storez].z != -999)
				storez++;

			// store normal in an empty spot
			allNormals[i][j + 1][storex].x = tempblock.x;
			allNormals[i][j + 1][storey].y = tempblock.y;
			allNormals[i][j + 1][storez].z = tempblock.z;

			// store normal in an empty spot
			tempblock = cross(blocks[i][j], blocks[i][j + 1], blocks[i + 1][j + 1], blocks[i + 1][j], 2);

			// reset the storage values (used to check if a spot is filled with a normal)
			storex = 0;
			storey = 0;
			storez = 0;

			// store normal in a spot that doesn't have a normal already
			while (allNormals[i + 1][j + 1][storex].x != -999)
				storex++;
			while (allNormals[i + 1][j + 1][storey].y != -999)
				storey++;
			while (allNormals[i + 1][j + 1][storez].z != -999)
				storez++;

			// store normal in an empty spot
			allNormals[i + 1][j + 1][storex].x = tempblock.x;
			allNormals[i + 1][j + 1][storey].y = tempblock.y;
			allNormals[i + 1][j + 1][storez].z = tempblock.z;


			// store normal in an empty spot
			tempblock = cross(blocks[i][j], blocks[i][j + 1], blocks[i + 1][j + 1], blocks[i + 1][j], 3);

			// reset the storage values (used to check if a spot is filled with a normal)
			storex = 0;
			storey = 0;
			storez = 0;

			// store normal in a spot that doesn't have a normal already
			while (allNormals[i + 1][j][storex].x != -999)
				storex++;
			while (allNormals[i + 1][j][storey].y != -999)
				storey++;
			while (allNormals[i + 1][j][storez].z != -999)
				storez++;

			// store normal in an empty spot
			allNormals[i + 1][j][storex].x = tempblock.x;
			allNormals[i + 1][j][storey].y = tempblock.y;
			allNormals[i + 1][j][storez].z = tempblock.z;
		}
	}

}


/************************************************************************

Function:		averageMountainNormals

Description:	calculate the average normals for each vertex for smooth shading

*************************************************************************/
void averageMountainNormals(struct block allNormals[GRIDSIZE + 1][GRIDSIZE + 1][4], struct block avgNormals[GRIDSIZE + 1][GRIDSIZE + 1]) {
	int i, j, k, storex, storey, storez;

	// calculate average normals for smooth shading
	for (i = 0; i < GRIDSIZE; i++) {
		for (j = 0; j < GRIDSIZE; j++) {

			storex = 0;
			storey = 0;
			storez = 0;

			// check how many normals are at current vertex
			while (allNormals[i][j][storex].x != -999 && storex != 4)
				storex++;
			while (allNormals[i][j][storey].y != -999 && storey != 4)
				storey++;
			while (allNormals[i][j][storez].z != -999 && storez != 4)
				storez++;

			// get all the normals at a given vertex then average them and store in avgNormals
			for (k = 0; k < storex; k++)
				avgNormals[i][j].x = avgNormals[i][j].x + allNormals[i][j][k].x;
			for (k = 0; k < storey; k++)
				avgNormals[i][j].y = avgNormals[i][j].y + allNormals[i][j][k].y;
			for (k = 0; k < storez; k++)
				avgNormals[i][j].z = avgNormals[i][j].z + allNormals[i][j][k].z;

			avgNormals[i][j].x = (avgNormals[i][j].x) / (storex);
			avgNormals[i][j].y = (avgNormals[i][j].y) / (storey);
			avgNormals[i][j].z = (avgNormals[i][j].z) / (storez);

			// quad2 average normal
			storex = 0;
			storey = 0;
			storez = 0;

			// check how many normals are at current vertex
			while (allNormals[i][j + 1][storex].x != -999 && storex != 4)
				storex++;
			while (allNormals[i][j + 1][storey].y != -999 && storey != 4)
				storey++;
			while (allNormals[i][j + 1][storez].z != -999 && storez != 4)
				storez++;

			// get all the normals at a given vertex then average them and store in avgNormals
			for (k = 0; k < storex; k++)
				avgNormals[i][j + 1].x = avgNormals[i][j + 1].x + allNormals[i][j + 1][k].x;
			for (k = 0; k < storey; k++)
				avgNormals[i][j + 1].y = avgNormals[i][j + 1].y + allNormals[i][j + 1][k].y;
			for (k = 0; k < storez; k++)
				avgNormals[i][j + 1].z = avgNormals[i][j + 1].z + allNormals[i][j + 1][k].z;

			avgNormals[i][j + 1].x = (avgNormals[i][j + 1].x) / (storex);
			avgNormals[i][j + 1].y = (avgNormals[i][j + 1].y) / (storey);
			avgNormals[i][j + 1].z = (avgNormals[i][j + 1].z) / (storez);

			// quad4 average normals
			storex = 0;
			storey = 0;
			storez = 0;

			// check how many normals are at current vertex
			while (allNormals[i + 1][j + 1][storex].x != -999 && storex != 4)
				storex++;
			while (allNormals[i + 1][j + 1][storey].y != -999 && storey != 4)
				storey++;
			while (allNormals[i + 1][j + 1][storez].z != -999 && storez != 4)
				storez++;

			// get all the normals at a given vertex then average them and store in avgNormals
			for (k = 0; k < storex; k++)
				avgNormals[i + 1][j + 1].x = avgNormals[i + 1][j + 1].x + allNormals[i + 1][j + 1][k].x;
			for (k = 0; k < storey; k++)
				avgNormals[i + 1][j + 1].y = avgNormals[i + 1][j + 1].y + allNormals[i + 1][j + 1][k].y;
			for (k = 0; k < storez; k++)
				avgNormals[i + 1][j + 1].z = avgNormals[i + 1][j + 1].z + allNormals[i + 1][j + 1][k].z;

			avgNormals[i + 1][j + 1].x = (avgNormals[i + 1][j + 1].x) / (storex);
			avgNormals[i + 1][j + 1].y = (avgNormals[i + 1][j + 1].y) / (storey);
			avgNormals[i + 1][j + 1].z = (avgNormals[i + 1][j + 1].z) / (storez);

			// quad4 average normals
			storex = 0;
			storey = 0;
			storez = 0;

			// check how many normals are at current vertex
			while (allNormals[i + 1][j][storex].x != -999 && storex != 4)
				storex++;
			while (allNormals[i + 1][j][storey].y != -999 && storey != 4)
				storey++;
			while (allNormals[i + 1][j][storez].z != -999 && storez != 4)
				storez++;

			// get all the normals at a given vertex then average them and store in avgNormals
			for (k = 0; k < storex; k++)
				avgNormals[i + 1][j].x = avgNormals[i + 1][j].x + allNormals[i + 1][j][k].x;
			for (k = 0; k < storey; k++)
				avgNormals[i + 1][j].y = avgNormals[i + 1][j].y + allNormals[i + 1][j][k].y;
			for (k = 0; k < storez; k++)
				avgNormals[i + 1][j].z = avgNormals[i + 1][j].z + allNormals[i + 1][j][k].z;

			avgNormals[i + 1][j].x = (avgNormals[i + 1][j].x) / (storex);
			avgNormals[i + 1][j].y = (avgNormals[i + 1][j].y) / (storey);
			avgNormals[i + 1][j].z = (avgNormals[i + 1][j].z) / (storez);

		}
	}

}


/************************************************************************

Function:		fog

Description:	enable fog

*************************************************************************/
void fog()
{
	// set fog color and density
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1);
}



/************************************************************************

	Function:		loadppm

	Description:	Loads in the PPM image (taken from lecture notes)

*************************************************************************/
unsigned int loadppm(char filename[])
{
	GLuint myTextureID;
	GLubyte *imageData;

	// the ID of the image file
	FILE *fileID;

	// maxValue
	int  maxValue;	
		
	// total number of pixels in the image
	int  totalPixels;

	// temporary character
	char tempChar;

	// counter variable for the current pixel in the image
	int i;

	// array for reading in header information
	char headerLine[100];

	float RGBScaling;

	// temporary variables for reading in the red, green and blue data of each pixel
	int red, green, blue;

	// open the image file for reading
	fileID = fopen(filename, "r");

	// read in the first header line
	fscanf(fileID,"%[^\n] ", headerLine);

	// make sure that the image begins with 'P3', which signifies a PPM file
	if ((headerLine[0] != 'P') || (headerLine[1] != '3'))
	{
		// printf("This is not a PPM file!\n"); 
		exit(0);
	}

	// we have a PPM file
	// printf("This is a PPM file\n");

	// read in the first character of the next line
	fscanf(fileID, "%c", &tempChar);

	// while we still have comment lines (which begin with #)
	while(tempChar == '#') 
	{
		// read in the comment
		fscanf(fileID, "%[^\n] ", headerLine);

		// print the comment
		// printf("%s\n", headerLine);
		
		// read in the first character of the next line
		fscanf(fileID, "%c",&tempChar);
	}

	// the last one was not a comment character '#', so we nee dto put it back into the file stream (undo)
	ungetc(tempChar, fileID); 

	// read in the image hieght, width and the maximum value
	fscanf(fileID, "%d %d %d", &imageWidth, &imageHeight, &maxValue);

	// print out the information about the image file
	// printf("%d rows  %d columns  max value= %d\n", imageHeight, imageWidth, maxValue);

	// compute the total number of pixels in the image
	totalPixels = imageWidth * imageHeight;

	// allocate enough memory for the image  (3*) because of the RGB data
	imageData = malloc(3 * sizeof(GLuint) * totalPixels);


	// determine the scaling for RGB values
	RGBScaling = 255.0 / maxValue;


	// if the maxValue is 255 then we do not need to scale the 
	//    image data values to be in the range or 0 to 255
	if (maxValue == 255) 
	{
		for(i = 0; i < totalPixels; i++) 
		{
			// read in the current pixel from the file
			fscanf(fileID,"%d %d %d",&red, &green, &blue );

			// store the red, green and blue data of the current pixel in the data array
			imageData[3*totalPixels - 3*i - 3] = red;
			imageData[3*totalPixels - 3*i - 2] = green;
			imageData[3*totalPixels - 3*i - 1] = blue;
		}
	}
	else  // need to scale up the data values
	{
		for(i = 0; i < totalPixels; i++) 
		{
			// read in the current pixel from the file
			fscanf(fileID,"%d %d %d",&red, &green, &blue );

			// store the red, green and blue data of the current pixel in the data array
			imageData[3*totalPixels - 3*i - 3] = red   * RGBScaling;
			imageData[3*totalPixels - 3*i - 2] = green * RGBScaling;
			imageData[3*totalPixels - 3*i - 1] = blue  * RGBScaling;
		}
	}


	// close the image file
	fclose(fileID);

	glGenTextures(1, &myTextureID);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, myTextureID);

	//configuration
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//wrap or cut off based on param
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build 2d mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imageWidth, imageHeight,
	GL_RGB, GL_UNSIGNED_BYTE, imageData);

	free(imageData);

	return myTextureID;

}

/************************************************************************

Function:      map

Description:    creates the disk and cylinder for the map textures

*************************************************************************/

void map() {

	GLUquadricObj *qobj;

	
	// Construct Cylinder walls of the map and texture when enabled
	qobj = gluNewQuadric();
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricTexture(qobj,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, skyID);
	gluQuadricTexture(qobj, skyID);
	glTranslatef(0, -3, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(qobj, C/2, C/2, 35, 100, 60);
	glDisable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_EMISSION, blackDiffuse);

	glPopMatrix(); 
	

	// draw fog on the ocean if enabled
	if(enableFog == 1) {
		glEnable(GL_FOG);
		fog();
	}

	// Construct disk floor of the map and texture when enabled
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteSpecular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glRotatef(-90, 1.0, 0.0, 0.0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL); /* smooth shaded */
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricTexture(qobj, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, seaID);
	gluQuadricTexture(qobj, seaID);
	gluDisk(qobj, 0, C/2, 100, 100);
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	// turn off fog after done drawing ocean
	if(enableFog == 1)
		glDisable(GL_FOG);
}


/************************************************************************

Function:      mouseMotion

Description:    updates mouse co-ordinates

*************************************************************************/

void mouseMotion(int x, int y) {
	float var = windowWidth/2;

	// get mouse co-ords for the tilt of the plane
	mousex = ((x - var)/var)*0.5;
	tiltAngle = ((x-var)/var)*45;

	// inverse rotation to match mouse orientation
	angleRate = mousex*-1;
}


/************************************************************************

Function:      generateScene

Description:    generates the grid scene (basic scene)

*************************************************************************/

void generateScene() {

	// red line for x frame of reference
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	// green line for y frame of reference
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);

	// blue line for blue frame of reference
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	glLineWidth(1.0);

	// white sphere for origin
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL); /* smooth shaded */
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glColor3f(1.0, 1.0, 1.0);
	gluSphere(qobj, 0.05, 15, 10);
	gluDeleteQuadric(qobj);

	// center grid of CxC
	float xStart = (-1)*(C/2);
	float zStart = (-1)*(C/2);


	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightblueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	// draw the quad flooring for the basic scene
	glBegin(GL_QUADS);
		int x;
		int z;
		for (x = xStart; x < C/2; x++) {
			for (z = zStart; z < C/2; z++) {
				glNormal3d(0, 1, 0);  // normal of the floor is pointing up
				glVertex3f(x, 0, z);
				glVertex3f(x+1, 0, z);
				glVertex3f(x+1, 0, z+1);
				glVertex3f(x, 0, z+1);			}
		}
	glEnd();

}


/************************************************************************

Function:		parsePlane

Description:	Handles parseing plane from cessna.txt into vertex and normal array

*************************************************************************/

void parsePlane() {
	FILE *fp;
	char buff[256];
	int vCount = 0;
	int nCount = 0;
	int subObjCount = -1; // This is so the subObjCount starts at 0 when reading into array
	int fCount = 0;
	GLfloat n = 0;
	int offset, i = 0, j;

	fp = fopen("cessna.txt", "r");

	if (fp == NULL)
		printf("Can't open file");
	else {
		while (fgets(buff, sizeof(buff), fp) != NULL) {
			char *data = buff+2; // start data pointer where the numbers start
			if (buff[0] == 'v') {
				sscanf(buff, "v %f %f %f", &vertex[vCount][0], &vertex[vCount][1], &vertex[vCount][2]);
				vCount++;

			}
			else if (buff[0] == 'n') {
				sscanf(buff, "n %f %f %f", &normal[nCount][0], &normal[nCount][1], &normal[nCount][2]);
				nCount++;
			}
			else if (buff[0] == 'g') {
				subObjCount++;
				// reset indexing for row array parsing
				if (subObjCount == 4) i = 0;
				if (subObjCount == 6) i = 0;
				if (subObjCount == 7) i = 0;
				if (subObjCount == 8) i = 0;
				if (subObjCount == 11) i = 0;
				if (subObjCount == 12) i = 0;
				if (subObjCount == 14) i = 0;
				if (subObjCount == 26) i = 0;
				if (subObjCount == 33) i = 0;
			}
			else if (buff[0] == 'f' && subObjCount <= 3) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face03[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount >= 4 && subObjCount <= 5) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face45[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount == 6) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face6[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount == 7) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face7[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount >= 8 && subObjCount <= 10) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face810[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount == 11) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face11[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount >= 12 && subObjCount <= 13) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face1213[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount >= 14 && subObjCount <= 25) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face1425[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount >= 26 && subObjCount <= 32) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					face2632[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
		}
	}
	fclose(fp);
} 


/************************************************************************

Function:		parsePropeller

Description:	Handles parseing propeller from propeller.txt into provetrex and pronormal array

*************************************************************************/

void parsePropeller() {
	FILE *fp;
	char buff[256];
	int vCount = 0;
	int nCount = 0;
	int subObjCount = -1; // This is so the subObjCount starts at 0 when reading into array
	int fCount = 0;
	GLfloat n = 0;
	int offset, i = 0, j;

	fp = fopen("propeller.txt", "r");

	if (fp == NULL)
		printf("Can't open file");
	else {
		while (fgets(buff, sizeof(buff), fp) != NULL) {
			char *data = buff + 2; // start data pointer where the numbers start
			if (buff[0] == 'v') {
				sscanf(buff, "v %f %f %f", &provertex[vCount][0], &provertex[vCount][1], &provertex[vCount][2]);
				vCount++;

			}
			else if (buff[0] == 'n') {
				sscanf(buff, "f %f %f %f", &pronormal[nCount][0], &pronormal[nCount][1], &pronormal[nCount][2]);
				nCount++;
			}
			else if (buff[0] == 'g') {
				subObjCount++;
				// reset indexing for row array parsing
				if (subObjCount == 1) i = 0;
			}
			else if (buff[0] == 'f' && subObjCount == 0) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					proface0[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
			else if (buff[0] == 'f' && subObjCount == 1) {
				j = 0; // reset indexing for column array parsing
				while (1 == sscanf(data, " %f%n", &n, &offset)) {
					data += offset;
					proface1[i][j] = n;
					j++; // increase column
				}
				i++; // increase row
			}
		}
	}
	fclose(fp);
}

/************************************************************************

Function:		drawPlane

Description:	Handles drawing the plane from the stored values

*************************************************************************/


void drawPlane() {
	int i = 0;
	int j = 0;
	int pos = 0;

	//yellow subsection of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	for (i = 0; i < 432 + 6 + 253 + 10; i++) {
		glBegin(GL_POLYGON);
		glColor3f(0.75, 0.75, 0.75);
		for (j = 0; face03[i][j] > 0; j++) {
			pos = face03[i][j] - 1;
			glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
			glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
		}
		glEnd();
	}

	//blue subsection of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 19 + 137; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face45[i][j] > 0; j++) {
				pos = face45[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//light purple sub section of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightPurpleDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 186; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face6[i][j] > 0; j++) {
				pos = face6[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//blue subsection of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 175; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face7[i][j] > 0; j++) {
				pos = face7[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//yellow subsection of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 9 + 10 + 33; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face810[i][j] > 0; j++) {
				pos = face810[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//blue blue subsection of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 3; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face11[i][j] > 0; j++) {
				pos = face11[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//yellow sub section of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 2 + 5; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face1213[i][j] > 0; j++) {
				pos = face1213[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//blue sub section of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 14 + 33 + 27 + 63 + 14 + 106 + 27 + 5 + 162 + 453 + 693 + 20; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face1425[i][j] > 0; j++) {
				pos = face1425[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}

	//yellow sub section of plane
	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
		for (i = 0; i < 18 + 52 + 328 + 36 + 91 + 56 + 162; i++) {
			glBegin(GL_POLYGON);
			glColor3f(0.75, 0.75, 0.75);
			for (j = 0; face2632[i][j] > 0; j++) {
				pos = face2632[i][j] - 1;
				glNormal3f(normal[pos][0], normal[pos][1], normal[pos][2]);
				glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			}
			glEnd();
		}
}

/************************************************************************

Function:		drawPropeller

Description:	Handles drawing the propeller from the stored values

*************************************************************************/


void drawPropeller() {
	int i = 0;
	int j = 0;
	int pos = 0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, zeroMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	// draw first propeller subsection
	for (i = 0; i < 120; i++) {
		glBegin(GL_POLYGON);
		glColor3f(0.75, 0.75, 0.75);
		for (j = 0; proface0[i][j] > 0; j++) {
			pos = proface0[i][j] - 1;
			glNormal3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
		}
		glEnd();
	}

	// draw second propeller subsection
	for (i = 0; i < 12; i++) {
		glBegin(GL_POLYGON);
		glColor3f(0.75, 0.75, 0.75);
		for (j = 0; proface1[i][j] > 0; j++) {
			pos = proface1[i][j] - 1;
			glNormal3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
			glVertex3f(vertex[pos][0], vertex[pos][1], vertex[pos][2]);
		}
		glEnd();
	}
}


/************************************************************************

Function:		myDisplay

Description:	Display callback, clears frame buffer and depth buffer,
positions the camera and draws the cube.

*************************************************************************/
void myDisplay(void)
{

	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix into the model view matrix
	glLoadIdentity();

	glPushMatrix();
		// set the camera position
		gluLookAt(xCamera, yCamera, zCamera,
				  xlCamera, ylCamera, zlCamera,
				  0, 1, 0);

		// position light 0
		glPushMatrix();
			glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glPopMatrix();

		if (enableGrid == 1) 
			// Generate Scene
			generateScene();
		else {
			// Generate the map
			map();
		} 

		if (enableMountains == 1) {
			// Draw the 3 randomly generated mountains
			glPushMatrix();
			glTranslatef(10, 0, 10);
			glScalef(0.4, 0.5, 0.4);
			if (enableMountainTex == 1) glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mountainID);
			drawMountain(mountain1, mountain1AvgNormals);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(20, 0, -5);
			glScalef(0.25, 0.5, 0.25);
			if (enableMountainTex == 1)	glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mountainID);
			drawMountain(mountain2, mountain2AvgNormals);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-25, 0, -20);
			glScalef(0.6, 0.5, 0.6);
			if (enableMountainTex == 1)	glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, mountainID);
			drawMountain(mountain3, mountain3AvgNormals);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}

		// enables wireframe if == 1
		if (enableWireFrame == 1)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();

	glPushMatrix();

		// Draw the plane
		glTranslatef(0.0, -0.5, -2.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);

		// do a barrel roll if z is pressed
		if (enableBarrelRoll == 1) {
			// smooth barrel roll right is mouse is left
			if (enableBarrelRoll == 1 && barrelRoll == 0 && barrelLeft == 0) {
					barrelRoll = barrelRoll - tiltAngle;
			}
			// smooth barrel roll left if mouse is right
			if (enableBarrelRoll == 1 && barrelRoll == 0 && barrelLeft == 1) {
					barrelRoll = barrelRoll - tiltAngle;
			}
			glRotatef(barrelRoll, 1, 0, 0);
		}
		// stop the tilt if barrel rolling
		else
			glRotatef(-tiltAngle, 1.0, 0.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		drawPlane();

		// Draw the left propeller
		glPushMatrix();
			glTranslatef(-2.105031, -0.174200, 0.3499989);
			glRotatef(theta[2] * 20, 1.0, 0.0, 0.0);
			glTranslatef(2.105031, 0.174200, -0.3499989);
			drawPropeller();
		glPopMatrix();

		// Draw the right propeller
		glPushMatrix();
			glTranslatef(0.0, 0.0, -0.72);
			glTranslatef(-2.105031, -0.174200, 0.3499989);
			glRotatef(theta[2] * 20, 1.0, 0.0, 0.0);
			glTranslatef(2.105031, 0.174200, -0.3499989);
			drawPropeller();
		glPopMatrix();
	glPopMatrix();


	// swap the drawing buffers
	glutSwapBuffers();


}

/************************************************************************

Function:		myIdle

Description:	Updates the animation when idle.

*************************************************************************/
void myIdle()
{

	// change plane altitude
	if (yMove) {
		yCamera += yMove*speedCamera;
		ylCamera += yMove*speedCamera;
	}

	// calculate tilt angle of plane with the mouse
	planeAngle += angleRate;
	zCamera += (cos(planeAngle*(rad))*planeSpeed);
	xCamera += (sin(planeAngle*(rad))*planeSpeed);
	xlCamera = xCamera + (sin(planeAngle*(rad))*tiltSpeed);
	zlCamera = zCamera + (cos(planeAngle*(rad))*tiltSpeed);

	// rotation speed of propeller
	theta[2] += (0.1*rotationSpeed);
	if (theta[2] >= 360.0)
	theta[2] -= (360.0*rotationSpeed);

	// check which side mouse is on before barrel rolling
	if (mousex < 0.0 && barrelRoll == 0)
		barrelLeft = 1;
	if (mousex > 0.0 && barrelRoll == 0)
		barrelLeft = 0;

	// barrel roll right if tilting left
	if (enableBarrelRoll == 1 && barrelLeft == 0) {
		barrelRoll += (0.1*rotationSpeed);
		if (barrelRoll >= 360) {
			barrelRoll = 0;
			enableBarrelRoll = 0;
		}
	}

	//barrel roll left if tilting right
	if (enableBarrelRoll == 1 && barrelLeft == 1) {
		barrelRoll -= (0.1*rotationSpeed);
		if (barrelRoll <= -360) {
			barrelRoll = 0;
			enableBarrelRoll = 0;
		}
	}

	// Redraw the new state
	glutPostRedisplay();
}

/************************************************************************

Function:		normalKey, pressKey, releaseKey

Description:	User input callbacks

*************************************************************************/
void normalKey(unsigned char key, int xx, int yy)
{
	if (key == ESC || key == 'q' || key == 'Q')
		exit(0);

	// enable barrel roll
	if (key == 'z' && enableBarrelRoll == 0 || key == 'Z' && enableBarrelRoll == 0)
		enableBarrelRoll = 1;

	// enable wireframe if w pressed
	if (key == 'w' && enableWireFrame == 0 || key == 'W' && enableWireFrame == 0)
		enableWireFrame = 1;
	else if (key == 'w' && enableWireFrame == 1 || key == 'W' && enableWireFrame == 1)
		enableWireFrame = 0;

	// enable the scene if s is pressed
	if (key == 's' && enableGrid == 0 || key == 'S' && enableGrid == 0)
		enableGrid = 1;
	else if (key == 's' && enableGrid == 1 || key == 'S' && enableGrid == 1)
		enableGrid = 0;

	// enable fog if b is pressed
	if (key == 'b' && enableFog == 0 || key == 'B' && enableFog == 0)
		enableFog = 1;
	else if (key == 'b' && enableFog == 1 || key == 'B' && enableFog == 1)
		enableFog = 0;

	// enable mountains if m is pressed
	if (key == 'm' && enableMountains == 0 || key == 'm' && enableMountains == 0) {
		enableMountains = 1;

		// factor the speed variables to compensate for slow down due to mountain rendering
		speedCamera = speedCamera*4.25;
		planeSpeed = planeSpeed*4.25;
		planeSpeedInterval = planeSpeedInterval*4.25;
		rotationSpeed = rotationSpeed * 4.25;
	}
	else if (key == 'm' && enableMountains == 1 || key == 'm' && enableMountains == 1) {
		enableMountains = 0;

		// factor the speed variables to compensate for slow down due to mountain rendering
		speedCamera = speedCamera/4.25;
		planeSpeed = planeSpeed/4.25;
		planeSpeedInterval = planeSpeedInterval/4.25;
		rotationSpeed = rotationSpeed/4.25;
	}

	// enable mountain textures if t is pressed
	if (key == 't' && enableMountainTex == 0 || key == 'T' && enableMountainTex == 0)
		enableMountainTex = 1;
	else if (key == 't' && enableMountainTex == 1 || key == 'T' && enableMountainTex == 1)
		enableMountainTex = 0;

	// enable fullscreen if f is pressed
	if (key == 'f' && enableFullscreen == 0 || key == 'F' && enableFullscreen == 0) {
		enableFullscreen = 1;
		glutFullScreen();

	}
	else if (key == 'f' && enableFullscreen == 1 || key == 'F' && enableFullscreen == 1) {
		enableFullscreen = 0;
		// Set window position on screen
		glutPositionWindow(100, 150);
		// Set window size
		glutReshapeWindow(1000, 800);
	}

}

// handles speeding up plane and changing plane altitude with PAGE UP/DOWN and UP/DOWN arrow
void pressKey(int key, int xx, int yy)
{
	int i;
	switch (key) {
	case GLUT_KEY_UP: yMove = 1.0; break;
	case GLUT_KEY_DOWN: 
		if (ylCamera > 0 || yCamera > 0)
			yMove = -1.0; break;
	case GLUT_KEY_PAGE_UP: planeSpeed += planeSpeedInterval; break;
	case GLUT_KEY_PAGE_DOWN:
		if (planeSpeed > planeSpeedInterval*25)
			planeSpeed -= planeSpeedInterval; break;
	}
}

void releaseKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: yMove = 0.0; break;
	case GLUT_KEY_DOWN: yMove = 0.0; break;
	case GLUT_KEY_PAGE_UP: planeSpeed += 0.0; break;
	case GLUT_KEY_PAGE_DOWN: planeSpeed += 0.0; break;
	}
}




/************************************************************************

Function:		initializeGL

Description:	Initializes the OpenGL rendering context for display.

*************************************************************************/
void initializeGL(void)
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Set background color to be black
	glClearColor(0, 0, 0, 1.0);

	// define the light color and intensity
	GLfloat ambientLight[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuseLight[] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	//  the global ambient light level
	GLfloat globalAmbientLight[] = { 0.4, 0.4, 0.4, 1.0 };

	// set the global ambient light level
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);

	// define the color and intensity for light 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	// enable lighting 
	glEnable(GL_LIGHTING);

	// enable light 0
	glEnable(GL_LIGHT0);

	// make sure the normals are unit vectors
	glEnable(GL_NORMALIZE);

	glDisable(GL_CULL_FACE);


}


/************************************************************************

Function:		myResize

Description:	Handles a user resize of the window.

*************************************************************************/
void myResize(int newWidth, int newHeight)
{

	// Update the new width
	windowWidth = newWidth;
	// Update the new height
	windowHeight = newHeight;

	// Update the viewport to still be all of the window
	glViewport(0, 0, windowWidth, windowHeight);

	// Change into projection mode so that we can change the camera properties
	glMatrixMode(GL_PROJECTION);

	// Load the identity matrix into the projection matrix
	glLoadIdentity();

	// gluPerspective(fovy, aspect, near, far)
	gluPerspective(45, (float)windowWidth / (float)windowHeight, 1, 10000);

	// Change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************

Function:		printControls

Description:	Print the keyboard controls.

*************************************************************************/
void printControls()
{
	printf("Scene Controls\n");
	printf("----------------\n");
	printf("f:	toggle fullscreen\n");
	printf("b:	toggle fog\n");
	printf("m:	toggle mountains\n");
	printf("t:	toggle mountain texture\n");
	printf("s:	toggle sea & sky\n");
	printf("w:	toggle wireframe\n\n");
	printf("q:	quit\n");

	printf("Camera Controls\n");
	printf("----------------\n");
	printf("Up Arrow:	move up\n");
	printf("Down Arrow:	move down\n");
	printf("PAGE UP:	speed up\n");
	printf("PAGE DOWN:	slow down\n\n");

	printf("BONUS\n");
	printf("----------------\n");
	printf("z:      BARREL ROLL!");

}


/************************************************************************

Function:		main

Description:	Sets up the openGL rendering context and the windowing
system, then begins the display loop.

*************************************************************************/
void main(int argc, char** argv)
{
	//seed the random time (used for random mountain generation)
	srand(((unsigned)time(NULL)));

	// Print Keyboard Controls
	printControls();

	// Initialize the toolkit
	glutInit(&argc, argv);

	//Parses the plane
	parsePlane();

	//Parses the propeller
	parsePropeller();

	// Generate first Mountain Grid
	mountainGrid(mountain1);
	mountainNormals(mountain1AllNormals, mountain1);
	averageMountainNormals(mountain1AllNormals, mountain1AvgNormals);
	
	// Generate second Mountain Grid
	mountainGrid(mountain2);
	mountainNormals(mountain2AllNormals, mountain2);
	averageMountainNormals(mountain2AllNormals, mountain2AvgNormals);

	// Generate third Mountain Grid
	mountainGrid(mountain3);
	mountainNormals(mountain3AllNormals, mountain3);
	averageMountainNormals(mountain3AllNormals, mountain3AvgNormals);


	// Set display mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// Set window size
	glutInitWindowSize(windowWidth, windowHeight);
	// Set window position on screen
	glutInitWindowPosition(100, 150);
	// Open the screen window
	glutCreateWindow("Flight Simulation - Kyle Asaff");

	// register redraw function
	glutDisplayFunc(myDisplay);
	// register the idle function
	glutIdleFunc(myIdle);
	// register the resize function
	glutReshapeFunc(myResize);

	// Process standard key clicks
	glutKeyboardFunc(normalKey);
	// Process special key pressed
	glutSpecialFunc(pressKey);
	// Process special key release
	glutSpecialUpFunc(releaseKey);
	// Enable Mouse Function
	// glutMouseFunc(mouse);
	// Enable Motion Function
	glutPassiveMotionFunc(mouseMotion);

	// Initialize the rendering context
	initializeGL();

	//Load ppm's
	skyID = loadppm("sky08.ppm");
	seaID = loadppm("sea02.ppm");
	mountainID = loadppm("mount03.ppm");

	// Go into a perpetual loop
	glutMainLoop();
}