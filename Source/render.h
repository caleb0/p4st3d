#pragma once

#include "surface.h"

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