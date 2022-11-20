#pragma once
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include <time.h>
#include <thread>
#include<cstdlib>
#include<stdio.h>
#include<cstring>
#include <mmsystem.h>
using namespace std;
#pragma pack(1)

//Constants 
#define MAX_SIZE_SNAKE 40 
#define MAX_SIZE_FOOD 5 
#define MAX_SPEED 5
#define WIDTH_CONSOLE_DEFAULT 80 
#define HEIGH_CONSOLE_DEFAULT 25 
#define mainColorText 14 
#define mainColorBarrier  11
#define MAX_SIZE_SUB_SNAKE 25

struct PLAYER
{
	char name[50];
	int score;
};

//overload toan tu == cho kieu POINT
bool operator == (POINT a, POINT b)
{
	if (a.x == b.x && a.y == b.y)
	{
		return true;
	}
	return false;
}