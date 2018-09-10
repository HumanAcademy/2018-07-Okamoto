#include <string>
#include "XLibrary11.hpp"
using namespace XLibrary11;

enum Mode
{
	Title,
	Game,
	Gameclear,
	Gameover,
};

Float3 GetRandomPosition()
{
	float range = rand() / (float)RAND_MAX*
		App::GetWindowSize().x + 500.0;
	float radian = rand() / (float)RAND_MAX*
		XM_PI *2.0;

	return Float3(
		cosf(radian)*range,
		sinf(radian)*range,
		0.0
	);
}

bool IsHit(
	Float3 position1,
	Float3 position2,
	float range
)
{
	if (position1.x - range < position2.x &&
		position1.x + range > position2.x &&
		position1.y - range < position2.y &&
		position1.y + range > position2.y)
	{
		//“–‚½‚Á‚½
		return true;
	}
	{
		//“–‚½‚Á‚Ä‚È‚¢
		return false;
	}
}

int MAIN()
{
	Mode mode = Title;

	Camera camera;
	camera.color = Float4(0.0, 2.0, 0.0, 0.0);

	const float playerSpeed = 3.0;

	Sprite player(L"–ê”w–Ê—§‚¿.png");
	player.scale = 5.0;



	Sprite enemy(L"ƒlƒYƒ~@‰¡.png");
	enemy.scale = 0.2;
	const int enemyNum = 100;
	float enemySpeed = 3.0;
	int enemyInterval = 500.0;

	Float3 enemyPosition[enemyNum];

	const int bulletNum = 5;//’e‚Ì”
	const float bulletSpeed = 20.0;//’e‚Ì‘¬‚³
	int bulletIndex = 0;//Ÿ‚É”­Ë‚·‚é’e‚Ì”Ô†
	int bulletInterval = 0;//Ÿ‚É”­Ë‚·‚é‚Ü‚Å‚ÌƒJƒEƒ“ƒg

	Sprite bullet(L"bullet.png");
	bullet.scale = 2.0;
	int count = 0;

	Sprite onigiri(L"‚¨‚É‚¬‚è.png");
	onigiri.scale = 3.0;
	
	
	Float3 bulletPosition[bulletNum];//’e‚ÌÀ•W
	float bulletRadian[bulletNum];//’e‚ÌŠp“x

	Text titleText(L"‚¨‚É‚¬‚è‘åí‘ˆ");//ƒ^ƒCƒgƒ‹
	titleText.position.x = 5.0;
	titleText.color = Float4(1.0, 0.0, 0.0, 1.0);

	Text gameover(L"‚°```‚Ş‚¨``‚Î``");//ƒQ[ƒ€ƒI[ƒo[
	gameover.position.x = 5.0;
	gameover.color = Float4(1.0, 0.0, 0.0, 1.0);
	
	Text gameclear(L"‚°```‚ŞƒNƒŠƒA````");//ƒQ[ƒ€ƒI[ƒo[
	gameclear.position.x = 5.0;
	gameclear.color = Float4(1.0, 0.0, 0.0, 1.0);
	int time = 10800;
	Text timeText(L"0.0", 100.0);//ƒ^ƒCƒ€ŠÔ•\¦
	timeText.position.y = 200;
	timeText.scale = 0.5;


	
	while(App::Refresh())
	{
		camera.Update();
		
		switch (mode)
		{
		case Title:
			

			if (App::GetKeyDown(VK_RETURN))
			{
				//ƒŠƒZƒbƒg
				// ƒvƒŒƒCƒ„[‚ğƒŠƒZƒbƒg
				player.position = Float3(0.0, -150.0, 0.0);
				player.angles.z = 0.0;
				time= 10800;
				enemySpeed = 3.0;
				count = 0;

				//“G‚ÌÀ•W‚Ì‰Šú‰»
				for (int i = 0;i < enemyNum; i++)
				{
					enemyPosition[i] = GetRandomPosition();
				}

				//’e‚ÌÀ•W‚Ì‚Ì‰Šú‰»
				for (int i = 0;i < bulletNum; i++)
				{
					//—y‚©”Ş•û‚É”ò‚Î‚·
					bulletPosition[i].x = 9999.0;
					bulletRadian[i] = 7.85 ; //’e‚ª”ò‚ñ‚Å‚­Šp“x
				}
				
				mode = Game;
			}

			camera.position = 0.0;

			titleText.Draw();
	
			break;

		case Game:

			//©‹@‚ÌˆÚ“®
			if (App::GetKey('A'))
				player.position.x -= playerSpeed;

			if (App::GetKey('D'))
				player.position.x += playerSpeed;

			//’e‚ğ”­Ë‚·‚éˆ—
			bulletInterval++;
			if (App::GetKey('J'))//’·‰Ÿ‚µ‚Å‚¸‚Á‚Æ’e‚ªo‚é
			{
				if (bulletInterval > 10)
				{
					bulletInterval = 5;
					//’e‚ğ©‹@‚É‡‚í‚¹‚é
					bulletPosition[bulletIndex] =
						player.position;


					bulletIndex++;

					bulletIndex++;
					if (bulletIndex >= bulletNum)
						bulletIndex = 0;
				}

			}

			timeText.Draw();
			time--;
			timeText.Create(std::to_wstring(time / 60), 100.0);
		
			//©‹@‚Ì•`‰æ
		
			player.Draw();
			onigiri.position.x = 0;
			onigiri.position.y = 60;
			onigiri.Draw();

			//“G‚ğ“®‚©‚·ˆ—
			for (int i = 0;i < enemyNum; i++)
			{	//“G‚ª‚¨‚É‚¬‚è‚É”ò‚ñ‚Å‚­
			
				float enemyRadian = atan2f(
					onigiri.position.y - enemyPosition[i].y,
					onigiri.position.x - enemyPosition[i].x
				);
			
				//“G‚ğ“®‚©‚·ˆ—+
				enemyPosition[i] += Float3
				(
					cosf(enemyRadian),
					sinf(enemyRadian),
					0.0
				) * enemySpeed;
				enemy.position = enemyPosition[i];

					//“–‚½‚è”»’è
					float hitRange = enemy.GetSize().x / 2.0 *
						enemy.scale.x;
					float hitRage =
						enemy.GetSize().y * 1 *
						enemy.scale.y;
					for (int j = 0; j < bulletNum; j++)
					{
						//“G‚Æ’e‚Ì‚Æ‚Ì“–‚½‚è”»’è
						if (IsHit(
							enemyPosition[i],
							bulletPosition[j],
							hitRange
						))
						{
							enemyPosition[i] =
								camera.position + GetRandomPosition();

							//“G‚ğ“|‚·‚Æ’e‚ÌƒXƒs[ƒh‚ªã‚ª‚é
							count+= 1;
							if (count % 5 == 0)
							{
								enemySpeed += 2;
							}
						}
						
					}
					//onigiri‚Æ‚Ì“–‚½‚è”»’è
						if (IsHit(
						enemyPosition[i],
						onigiri.position,
						hitRange
						))
						{
							mode =Gameover;//“G‚ª‚¨‚É‚¬‚è‚É“–‚½‚é‚Æƒ^ƒCƒgƒ‹‚É–ß‚é
						}
						enemy.Draw();
					
					//’e‚ğã‚É”ò‚Î‚·ˆ—
					bulletPosition[i] += Float3(
						cosf(bulletRadian[i]),
						sinf(bulletRadian[i]),
						0.0
					)* bulletSpeed;
					bullet.position = bulletPosition[i];
					//’e‚Ì•`‰æ
					bullet.Draw();
				
				break;
				
			}
			break;
		case Gameover:
			gameover.Draw();
			if (App::GetKeyDown(VK_RETURN))
			{
				mode = Title;
			}
			break;
		case Gameclear:
			gameclear.Draw();
			if (App::GetKeyDown(VK_RETURN))
			{
				mode = Title;
			}
			break;

		}
	}
	return 0;
}

