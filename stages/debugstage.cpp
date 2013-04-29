
#include "DebugStage.h"

DebugStage::DebugStage(unsigned int gameNumber)
{
	gameNum = gameNumber;
	srand( gameNumber );
	speed = 0;
	rotation = 0;
	accDown = false;
	decDown = false;
	dirDown = 0;
	lastRot = 0;
	lastRow = 0;
	carPos = 0;
	carGfxAt = 0;
	carGfxTo = 0;
	crashed = 0;
	score = 0;
	crashHiScore = -1;

	for( int i = 0; i < 64; i++ )
	{
		LeftWall[i] = 4;
		RightWall[i] = 4;
		Cat[i] = -1;
	}
}

DebugStage::~DebugStage(void)
{
}

void DebugStage::Begin()
{
	carTexture = spLoadSurface( ".//resource//car.png" );
	roadTexture = spLoadSurface( ".//resource//road.png" );
	offroadTexture = spLoadSurface( ".//resource//off.png" );
	roadsideTexture = spLoadSurface( ".//resource//roadside.png" );
	carMesh = spMeshLoadObjSize( ".//resource/car.obj", carTexture, 65535, 12000 );
	fontHnd = spFontLoad( "./resource/standard.ttf", 32 );
	spFontAddRange( fontHnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );

	fontHiHnd = spFontLoad( "./resource/standard.ttf", 64 );
	spFontAddRange( fontHiHnd, ' ', '~', SDL_MapRGB( display->format, 255, 220, 128 ) );
	fontHiSHnd = spFontLoad( "./resource/standard.ttf", 64 );
	spFontAddRange( fontHiSHnd, ' ', '~', SDL_MapRGB( display->format, 0, 0, 0 ) );

}

void DebugStage::Pause()
{
}

void DebugStage::Resume()
{
}

void DebugStage::Finish()
{
	spMeshDelete( carMesh );
	SDL_FreeSurface( carTexture );
	SDL_FreeSurface( roadTexture );
	SDL_FreeSurface( offroadTexture );
	SDL_FreeSurface( roadsideTexture );
	spFontDelete( fontHnd );
}

void DebugStage::Event(SDL_Event *e)
{
	if( e->type == SDL_KEYDOWN )
	{
		if( e->key.keysym.sym == gameSettings->keyQuit )
			gameStack->Pop();
		if( e->key.keysym.sym == gameSettings->keyAccelerate )
			accDown = true;
		if( e->key.keysym.sym == gameSettings->keyDecelerate )
			decDown = true;
		if( e->key.keysym.sym == gameSettings->keyLeft )
			dirDown = -1;
		if( e->key.keysym.sym == gameSettings->keyRight )
			dirDown = 1;

		if( crashed == 0 )
		{
			carPos += dirDown;
			carGfxTo = carPos<<SP_ACCURACY;
		}

		if( e->key.keysym.sym == gameSettings->keyMenuAccept && crashed != 0 )
		{
			gameStack->Pop();
		}
	}
	if( e->type == SDL_KEYUP )
	{
		if( e->key.keysym.sym == gameSettings->keyAccelerate )
			accDown = false;
		if( e->key.keysym.sym == gameSettings->keyDecelerate )
			decDown = false;
		if( e->key.keysym.sym == gameSettings->keyLeft || e->key.keysym.sym == gameSettings->keyRight )
			dirDown = 0;
	}
}


void DebugStage::Update()
{
	int r;

	if( crashed != 0 )
	{

		crashSpeed -= 2;
		if( crashSpeed > 0 )
		{
			rotation = (rotation + (speed * 10) ) % (2*SP_PI);
			crashTick++;
		}
		return;
	}

	r = ((rotation / (SP_PI/32)) + 4) % 64;
	if( carPos <= LeftWall[63-r] - 15 || carPos >= 15 - RightWall[63-r] )
	{
		for( int hs = 0; hs < 10; hs++ )
		{
			if( gameSettings->Top10GameScores[hs] < score )
			{
				for( int hsc = 9; hsc > hs; hsc-- )
				{
					gameSettings->Top10GameNum[hsc] = gameSettings->Top10GameNum[hsc - 1];
					gameSettings->Top10GameScores[hsc] = gameSettings->Top10GameScores[hsc - 1];
				}
				gameSettings->Top10GameNum[hs] = gameNum;
				gameSettings->Top10GameScores[hs] = score;
				gameSettings->Save();
				crashHiScore = hs;
			}
			if( crashHiScore >= 0 )
				break;
		}
		crashed = 1;
		crashSpeed = speed;
		crashTick = 0;

#ifdef PANDORA
		char c4a_cmd[255];
		sprintf( (char*)&c4a_cmd, "./sc so push cyracer pandora %i", score );
		system( (char*)&c4a_cmd );
#endif

		return;
	}

	if( carGfxTo > carGfxAt )
		carGfxAt += (1<<SP_ACCURACY) / 8;
	if( carGfxTo < carGfxAt )
		carGfxAt -= (1<<SP_ACCURACY) / 8;

	if( accDown && speed < 200 )
		speed++;
	if( decDown && speed > 0 )
		speed -= 2;
	if( speed < 0 )
		speed = 0;

	rotation = (rotation + (speed * 10) ) % (2*SP_PI);

	//carPos += dirDown;

	// Used to identify a fixed line
	// if( ((rotation / (SP_PI/32)) + a) % 64 != 60 )
	r = rotation / (SP_PI / 32);
	if( lastRot != r )
	{
		switch( rand() % 3 )
		{
		case 0:
			LeftWall[63-r] = LeftWall[(63-r+1)%64]+1;
			break;
		case 1:
			LeftWall[63-r] = LeftWall[(63-r+1)%64];
			break;
		case 2:
			LeftWall[63-r] = LeftWall[(63-r+1)%64]-1;
			break;
		}
		switch( rand() % 3 )
		{
		case 0:
			RightWall[63-r] = RightWall[(63-r+1)%64]+1;
			break;
		case 1:
			RightWall[63-r] = RightWall[(63-r+1)%64];
			break;
		case 2:
			RightWall[63-r] = RightWall[(63-r+1)%64]-1;
			break;
		}

		if( LeftWall[63-r] < 4 )
			LeftWall[63-r] = 4;
		if( RightWall[63-r] < 4 )
			RightWall[63-r] = 4;
		while( LeftWall[63-r] + RightWall[63-r] >= 27 )
		{
			if( LeftWall[63-r] + RightWall[63-r] >= 27 && LeftWall[63-r] > 4 )
				LeftWall[63-r]--;
			if( LeftWall[63-r] + RightWall[63-r] >= 27 && RightWall[63-r] > 4 )
				RightWall[63-r]--;
			if( LeftWall[63-r] < 4 )
				LeftWall[63-r] = 4;
			if( RightWall[63-r] < 4 )
				RightWall[63-r] = 4;

		}

		// lastRow++;
		lastRot = r % 64;
		score += (double)speed / 100.0;
		if( score > 9999999 )
			score = 9999999;
	}

}

void DebugStage::Render()
{
	int elevation = 24;

	spResetZBuffer();
	spIdentity();
	spClearTarget(SDL_MapRGB( display->format, 190, 190, 220 ));

	spSetLight(1);
	spSetCulling(1);
	spSetZSet(1);
	spSetZTest(1);
	spSetAlphaTest(1);

	Sint32 brightness = 65535; // (spCos(rotation+a*SP_PI/8)>>SP_HALF_ACCURACY)*abs(spCos(rotation+a*SP_PI/8)>>SP_HALF_ACCURACY)/2+(3<<SP_ACCURACY-1);
	Uint16 color = 0x8410; // 0xFFFF; // ((brightness>>SP_ACCURACY-4)<<11)+((brightness>>SP_ACCURACY-5)<<5)+(brightness>>SP_ACCURACY-4);

	spTranslate(0,-6<<SP_ACCURACY,-24<<SP_ACCURACY);

	spRotateX(rotation);
	for( int a = 0; a < 64; a++ )
	{
		spRotateX(SP_PI/32);

		if( ((rotation / (SP_PI/32)) + a) % 64 > 32 )
		{
			int yA, yB;

			////////////////////////////////////////// Left OffRoad
			yA = -15; // + LeftWall[a+1];
			yB = -15 + LeftWall[a];
			if( yA > yB )
			{
				int yT = yB;
				yB = yA;
				yA = yT;
			}
			elevation = 25;
			spBindTexture(roadsideTexture);
			if( ((rotation / (SP_PI/32)) + a) % 64 > 56 )
			{
				spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
					((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
			}

			yA = -15 + LeftWall[(a+1) % 64] + 1;
			if( LeftWall[a+1] != LeftWall[a] )
			{
				spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
					((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
			}

			spQuadTex3D(((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
				((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);


			yA = -15; // + LeftWall[a+1];
			spBindTexture(offroadTexture);
			spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
				((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);


			////////////////////////////////////////// Right OffRoad
			yA = 15 - RightWall[a];
			yB = 15;
			if( yA > yB )
			{
				int yT = yB;
				yB = yA;
				yA = yT;
			}
			elevation = 25;
			spBindTexture(roadsideTexture);

			if( ((rotation / (SP_PI/32)) + a) % 64 > 56 )
			{
				spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
					((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
			}

			yB = 15 - RightWall[(a+1) % 64] - 1;
			if( RightWall[a+1] != RightWall[a] )
			{
				spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
					((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
					((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
			}

			spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
				((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,roadsideTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadsideTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);


			yB = 15; // + LeftWall[a+1];
			spBindTexture(offroadTexture);
			spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
				((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);


			// Draw Road
			yA = -14 + LeftWall[a];
			yB = 14 - RightWall[a];
			spBindTexture(roadTexture);
			elevation = 24;
			spQuadTex3D(((4*yA)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
				((4*yA)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,roadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadTexture->w-SP_FONT_EXTRASPACE-1,roadTexture->h-SP_FONT_EXTRASPACE-1,
				((4*yB)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,roadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);




			/*
			for( int y = -15; y <= 15; y++ )
			{


				if( y <= -15 + LeftWall[a] || y >= 15 - RightWall[a] )
				{
					elevation = 25;
					//if( ((rotation / (SP_PI/32)) + a) % 64 == 60 && y == carPos )
					//      spBindTexture(carTexture);
					//else

					spBindTexture(roadsideTexture);
					if( ((rotation / (SP_PI/32)) + a) % 64 > 56 )
					{
						spQuadTex3D(((4*y)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
							((4*y)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
							((4*y)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
							((4*y)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
					}

					if( y == -15 + LeftWall[a] || y == 15 - RightWall[a] )
					{
						if( (y == -15 + LeftWall[a] && LeftWall[a] > LeftWall[a+1]) || (y == 15 - RightWall[a] && RightWall[a] > RightWall[a+1]) )
						{
							spQuadTex3D(((4*y)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
								((4*y)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
								((4*y)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
								((4*y)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
						}

						spQuadTex3D(((4*y)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
							((4*y)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
							((4*y)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
							((4*y)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);

						spQuadTex3D(((4*y)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
							((4*y)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, (elevation-1)<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
							((4*y)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
							((4*y)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);
					}

					spBindTexture(offroadTexture);
					spQuadTex3D(((4*y)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
						((4*y)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
						((4*y)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
						((4*y)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);

				} else {
					//if( ((rotation / (SP_PI/32)) + a) % 64 == 60 && y == carPos )
					//      spBindTexture(carTexture);
					//else
					spBindTexture(roadTexture);
					elevation = 24;
					spQuadTex3D(((4*y)-2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,SP_FONT_EXTRASPACE,SP_FONT_EXTRASPACE,
						((4*y)-2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
						((4*y)+2)<<SP_ACCURACY-2,-3<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,offroadTexture->h-SP_FONT_EXTRASPACE-1,
						((4*y)+2)<<SP_ACCURACY-2,+2<<SP_ACCURACY-2, elevation<<SP_ACCURACY-1,offroadTexture->w-SP_FONT_EXTRASPACE-1,SP_FONT_EXTRASPACE,color);

				}

			}
			*/
		}
	}

	spIdentity();
	spBindTexture(carTexture);
	spTranslate( carGfxAt, -3<<SP_ACCURACY, -12<<SP_ACCURACY );
	spRotateY( (SP_PI/8)*12 );
	spRotateZ( (SP_PI/16)*26 );

	if( crashed != 0 )
	{
		spRotateX( (SP_PI/16)*(crashTick < 6 ? crashTick : 6) );
		spRotateY( (SP_PI/16)*crashTick );
		spRotateZ( (SP_PI/16)*crashTick );

		char crashText[80];
		int crashYAdj;
		if( crashHiScore >= 0 )
		{
			sprintf((char*)&crashText, "Well Done\nNew High Score\nRank %d", (crashHiScore+1) );
			crashYAdj = (int)((double)fontHiSHnd->maxheight * 1.5);
		} else {
			sprintf((char*)&crashText, "Game Over" );
			crashYAdj = (int)(fontHiSHnd->maxheight / 2);
		}
		spSetZSet( 0 );
		spSetZTest( 0 );

		spFontDrawMiddle( (gameSettings->ScreenWidth / 2) + 4, (gameSettings->ScreenHeight / 2) + 4 - crashYAdj, -1, (char*)&crashText, fontHiSHnd );
		spFontDrawMiddle( (gameSettings->ScreenWidth / 2), (gameSettings->ScreenHeight / 2) - crashYAdj, -1, (char*)&crashText, fontHiHnd );

		spSetZSet( 1 );
		spSetZTest( 1 );
	} else 	if( carGfxTo > carGfxAt ) {
		spRotateY( -(SP_PI/16) );
	}	else if( carGfxTo < carGfxAt ) {
		spRotateY( (SP_PI/16) );
	}


	spMesh3D( carMesh, 0 );

	char scoreText[15];
	sprintf((char*)&scoreText, "Score: %d", (int)(score > 9999999 ? 9999999 : score) );
	spFontDraw( 10, 10, -1, (char*)&scoreText, fontHnd );
}