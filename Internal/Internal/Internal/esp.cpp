#include "stdafx.h"
#include "esp.h"


#include <Windows.h>
#include <iostream>

DWORD entityListStart = 0x0050F4F4;
DWORD viewMatrix = 0x501AE8;
DWORD healthOffset = 0xF8;
HWND hwndAC_Client;
HBRUSH Brush;
HDC hdcAC_client;
HFONT Font;
float Matrix[16];
COLORREF TextCOLOR;
COLORREF TextCOLORRED;
bool bEsp;


//defining our vectors
struct Vec3
{
    float x, y, z;
};

struct Vec4
{
    float x, y, z, w;
};

struct Vec2
{
    float x, y;
};

//These two function are from a old fleep post, i was able to use them to create the box style however this was just to save time :) Thx Fleep
void DrawFilledRect(int x, int y, int w, int h)
{
    RECT rect = { x, y, x + w, y + h };
    FillRect(hdcAC_client, &rect, Brush);
}


void DrawBorderBox(int x, int y, int w, int h, int thickness)
{

    DrawFilledRect(x, y, w, thickness);

    DrawFilledRect(x, y, thickness, h);

    DrawFilledRect((x + w), y, thickness, h);

    DrawFilledRect(x, y + h, w + thickness, thickness);
}


void DrawLine(int targetX, int targetY)
{
    MoveToEx(hdcAC_client, 960, 1080, NULL);
    LineTo(hdcAC_client, targetX, targetY);

}


void DrawString(int x, int y, COLORREF color, const char* text)
{
    SetTextAlign(hdcAC_client, TA_CENTER | TA_NOUPDATECP);

    SetBkColor(hdcAC_client, RGB(0, 0, 0));
    SetBkMode(hdcAC_client, TRANSPARENT);

    SetTextColor(hdcAC_client, color);

    SelectObject(hdcAC_client, Font);

    TextOutA(hdcAC_client, x, y, text, strlen(text));

    DeleteObject(Font);
}
//this is the world to screen function for openGL
bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight) // 3D to 2D
{
    //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
    Vec4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f)
        return false;

    //perspective division, dividing by clip.W = Normalized Device Coordinates
    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    //Transform to window coordinates
    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}
int esp(bool esp)
{
    TextCOLOR = RGB(255, 255, 255);
    hwndAC_Client = FindWindow(0, (L"AssaultCube"));//Gets Window


    DWORD entityList = *(DWORD*)(entityListStart + 0x4);

    if (esp)
    {
        memcpy(&Matrix, (PBYTE*)(viewMatrix), sizeof(Matrix));
        hdcAC_client = GetDC(hwndAC_Client);
        //Base of player
        Vec2 vScreen;
        //Head of player
        Vec2 vHead;

        //Sets the ammount of Players
        DWORD amountOfPlayers = *(DWORD*)(0x50F500);


        //our entity list loop
        for (short int i = 0; i < amountOfPlayers; i++)
        {


            //Create the entity
            DWORD entity = *(DWORD*)(entityList + 0x4 * i);

            if (entity != NULL)
            {

                //The entitys Pos
                float enemyX = *(float*)(entity + 0x34);
                float enemyY = *(float*)(entity + 0x38);
                float enemyZ = *(float*)(entity + 0x3C);

                Vec3 enemyPos = { enemyX, enemyY, enemyZ };

                //Enemys Head Pos
                float enemyXHead = *(float*)(entity + 0x4);
                float enemyYHead = *(float*)(entity + 0x8);
                float enemyZHead = *(float*)(entity + 0xC);

                Vec3 enemyHeadPos = { enemyXHead, enemyYHead, enemyZHead };


                //Sets each entitys health
                DWORD health = *(DWORD*)(entity + healthOffset);

                //converts 3d to 2d
                if (WorldToScreen(enemyPos, vScreen, Matrix, 1920, 1080))
                {


                    if (WorldToScreen(enemyHeadPos, vHead, Matrix, 1920, 1080))
                    {


                        //Creates the head height
                        float head = vHead.y - vScreen.y;
                        //Creates Width
                        float width = head / 2;
                        //Creates Center
                        float center = width / -2;
                        //Creates Extra area above head
                        float extra = head / -6;

                        //Sets the Brush Color
                        Brush = CreateSolidBrush(RGB(158, 66, 244));
                        //Draws the box
                        DrawBorderBox(vScreen.x + center, vScreen.y, width, head - extra, 1);
                        DeleteObject(Brush);


                        //Draw our heath by converting a int to a char
                        char healthChar[255];
                        sprintf_s(healthChar, sizeof(healthChar), " %i", (int)(health));
                        DrawString(vScreen.x, vScreen.y, TextCOLOR, healthChar);


                        //Turns on snaplines
                        if (GetKeyState(VK_F2) & 1)
                        {
                            DrawLine(vScreen.x, vScreen.y);
                        }
                    }
                }
            }
        }
        //This prevents a fat memory leak
        Sleep(1);
        DeleteObject(hdcAC_client);
        return 0;


    }
}


