#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>

void DrawFilledRect(int x, int y, int w, int h);
void DrawBorderBox(int x, int y, int w, int h, int thickness);
void DrawLine(int targetX, int targetY);
void DrawString(int x, int y, COLORREF color, const char* text);
struct Vec3;
struct Vec4;

struct Vec2;
bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight);
int esp(bool esp);
