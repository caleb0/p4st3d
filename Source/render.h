#pragma once

#include "surface.h"
#include "matrix3x4.h"
class CRender
{
public:

	void DrawF( int X, int Y, CColor Color, int Font, bool Center, char* _Input, ... )
	{
		/* set up buffer */
		char Buffer[ 1024 ] = { '\0' };

		/* set up varargs*/
		va_list Args;

		va_start( Args, _Input );
		vsprintf_s( Buffer, _Input, Args );
		va_end( Args );

		size_t Size = strlen( Buffer ) + 1;

		/* set up widebuffer*/
		wchar_t* WideBuffer = new wchar_t[ Size ];

		/* char -> wchar */
		mbstowcs_s( 0, WideBuffer, Size, Buffer, Size - 1 );

		/* check center */
		
		if ( Center )
		{
			int Width = 0, Height = 0;
			Surface->GetTextSize( Font, WideBuffer, Width, Height );
			X -= Width / 2;
			Y -= Height / 2;
		}

		/* call and draw*/
		Surface->DrawSetTextColor( Color );
		Surface->DrawSetTextFont( Font );
		Surface->DrawSetTextPos( X, Y );
		Surface->DrawPrintText( WideBuffer, wcslen( WideBuffer ), 0 );
	}

	void DrawRect( int X, int Y, int W, int H, CColor Color )
	{
		Surface->DrawSetColor( Color );
		Surface->DrawOutlinedRect( X, Y, X + W, Y + H );
	}
	void DrawInlineRect( int X, int Y, int W, int H, CColor Color )
	{
		DrawRect( X, Y, W, H, Color );
		DrawRect( X + 1, Y + 1, W - 2, H - 2, CColor( 0, 0, 0, 255 ) );
		DrawRect( X - 1, Y - 1, W + 2, H + 2, CColor( 0, 0, 0, 255 ) );
	}
	void DrawEsp(int X, int Y, int W, int H, CColor Color) {
		float lineW = (W / 5);
		float lineH = (H / 6);
		float lineT = 1;

		//outline
		Surface->DrawSetColor(CColor(0, 0, 0, 255));
		Surface->DrawLine(X - lineT, Y - lineT, X + lineW, Y - lineT); //top left
		Surface->DrawLine(X - lineT, Y - lineT, X - lineT, Y + lineH);
		Surface->DrawLine(X - lineT, Y + H - lineH, X - lineT, Y + H + lineT); //bot left
		Surface->DrawLine(X - lineT, Y + H + lineT, X + lineW, Y + H + lineT);
		Surface->DrawLine(X + W - lineW, Y - lineT, X + W + lineT, Y - lineT); // top right
		Surface->DrawLine(X + W + lineT, Y - lineT, X + W + lineT, Y + lineH);
		Surface->DrawLine(X + W + lineT, Y + H - lineH, X + W + lineT, Y + H + lineT); // bot right
		Surface->DrawLine(X + W - lineW, Y + H + lineT, X + W + lineT, Y + H + lineT);

		//inline
		Surface->DrawSetColor(Color);
		Surface->DrawLine(X, Y, X, Y + lineH);//top left
		Surface->DrawLine(X, Y, X + lineW, Y);
		Surface->DrawLine(X + W - lineW, Y, X + W, Y); //top right
		Surface->DrawLine(X + W , Y, X + W, Y + lineH);
		Surface->DrawLine(X, Y + H - lineH, X, Y + H); //bot left
		Surface->DrawLine(X, Y + H, X + lineW, Y + H);
		Surface->DrawLine(X + W - lineW, Y + H, X + W, Y + H);//bot right
		Surface->DrawLine(X + W, Y + H - lineH, X + W, Y + H);

	}
	void DDrawLine(int x0, int y0, int x1, int y1, CColor color) {
		Surface->DrawSetColor(color);
		Surface->DrawLine(x0, y0, x1, y1);
	}
	/*void Draw3DBox(CVector* boxVectors, CColor color)
	{
		Matrix3x4_t Matrix = EngineClient->GetMatrix();
		CVector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
			boxVectors4, boxVectors5, boxVectors6, boxVectors7;


		if (WorldToScreen(boxVectors[0], boxVectors0, Matrix) &&
			WorldToScreen(boxVectors[1], boxVectors1, Matrix) &&
			WorldToScreen(boxVectors[2], boxVectors2, Matrix) &&
			WorldToScreen(boxVectors[3], boxVectors3, Matrix) &&
			WorldToScreen(boxVectors[4], boxVectors4, Matrix) &&
			WorldToScreen(boxVectors[5], boxVectors5, Matrix) &&
			WorldToScreen(boxVectors[6], boxVectors6, Matrix) &&
			WorldToScreen(boxVectors[7], boxVectors7, Matrix))
		{

			/*
			.+--5---+
			.8 4    6'|
			+--7---+'  11
			9   |  10  |
			|  ,+--|3--+
			|.0    | 2'
			+--1---+'
			

			Vector2D lines[12][2];
			//bottom of box
			lines[0][0] = { boxVectors0.x, boxVectors0.y };
			lines[0][1] = { boxVectors1.x, boxVectors1.y };
			lines[1][0] = { boxVectors1.x, boxVectors1.y };
			lines[1][1] = { boxVectors2.x, boxVectors2.y };
			lines[2][0] = { boxVectors2.x, boxVectors2.y };
			lines[2][1] = { boxVectors3.x, boxVectors3.y };
			lines[3][0] = { boxVectors3.x, boxVectors3.y };
			lines[3][1] = { boxVectors0.x, boxVectors0.y };

			lines[4][0] = { boxVectors0.x, boxVectors0.y };
			lines[4][1] = { boxVectors6.x, boxVectors6.y };

			// top of box
			lines[5][0] = { boxVectors6.x, boxVectors6.y };
			lines[5][1] = { boxVectors5.x, boxVectors5.y };
			lines[6][0] = { boxVectors5.x, boxVectors5.y };
			lines[6][1] = { boxVectors4.x, boxVectors4.y };
			lines[7][0] = { boxVectors4.x, boxVectors4.y };
			lines[7][1] = { boxVectors7.x, boxVectors7.y };
			lines[8][0] = { boxVectors7.x, boxVectors7.y };
			lines[8][1] = { boxVectors6.x, boxVectors6.y };


			lines[9][0] = { boxVectors5.x, boxVectors5.y };
			lines[9][1] = { boxVectors1.x, boxVectors1.y };

			lines[10][0] = { boxVectors4.x, boxVectors4.y };
			lines[10][1] = { boxVectors2.x, boxVectors2.y };

			lines[11][0] = { boxVectors7.x, boxVectors7.y };
			lines[11][1] = { boxVectors3.x, boxVectors3.y };

			for (int i = 0; i < 12; i++)
			{
				DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, color);
			}
		}
	}*/
	void DrawRainbow(int X, int Y, int W, int H, float fSpeed, float &fRainbow) {
		CColor color(0, 0, 0, 0);

		fRainbow += fSpeed;
		if (fRainbow > 1.f)
			fRainbow = 0.f;

		for (int i = 0; i < W; i++) {
			float hue = (1.f / (float)W * i);
			hue -= fRainbow;
			if (hue < 0.f)
				hue += 1.f;
			CColor cRainbow = color.FromHSB(hue, 1.f, 1.f);
			DrawRect(X + i, Y, 1, H, cRainbow);
		}
	}
	void FillRGB(int X, int Y, int H, int W, CColor Color)
	{
		Surface->DrawSetColor(Color);
		Surface->DrawFilledRect(X, Y, X + W, Y + H);
	}

}; extern CRender * Render;