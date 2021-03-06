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
		//当たった
		return true;
	}
	{
		//当たってない
		return false;
	}
}

int MAIN()
{
	Mode mode = Title;

	Camera camera;
	camera.color = Float4(0.0, 2.0, 0.0, 0.0);

	const float playerSpeed = 3.0;

	Sprite player(L"爺背面立ち.png");
	player.scale = 5.0;



	Sprite enemy(L"ネズミ　横.png");
	enemy.scale = 0.2;
	const int enemyNum = 100;
	float enemySpeed = 3.0;
	int enemyInterval = 500.0;

	Float3 enemyPosition[enemyNum];

	const int bulletNum = 5;//弾の数
	const float bulletSpeed = 20.0;//弾の速さ
	int bulletIndex = 0;//次に発射する弾の番号
	int bulletInterval = 0;//次に発射するまでのカウント

	Sprite bullet(L"bullet.png");
	bullet.scale = 2.0;
	int count = 0;

	Sprite onigiri(L"おにぎり.png");
	onigiri.scale = 3.0;
	
	
	Float3 bulletPosition[bulletNum];//弾の座標
	float bulletRadian[bulletNum];//弾の角度

	Text titleText(L"おにぎり大戦争");//タイトル
	titleText.position.x = 5.0;
	titleText.color = Float4(1.0, 0.0, 0.0, 1.0);

	Text gameover(L"げ〜〜〜むお〜〜ば〜〜");//ゲームオーバー
	gameover.position.x = 5.0;
	gameover.color = Float4(1.0, 0.0, 0.0, 1.0);
	
	Text gameclear(L"げ〜〜〜むクリア〜〜〜〜");//ゲームオーバー
	gameclear.position.x = 5.0;
	gameclear.color = Float4(1.0, 0.0, 0.0, 1.0);
	int time = 10800;
	Text timeText(L"0.0", 100.0);//タイム時間表示
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
				//リセット
				// プレイヤーをリセット
				player.position = Float3(0.0, -150.0, 0.0);
				player.angles.z = 0.0;
				time= 10800;
				enemySpeed = 3.0;
				count = 0;

				//敵の座標の初期化
				for (int i = 0;i < enemyNum; i++)
				{
					enemyPosition[i] = GetRandomPosition();
				}

				//弾の座標のの初期化
				for (int i = 0;i < bulletNum; i++)
				{
					//遥か彼方に飛ばす
					bulletPosition[i].x = 9999.0;
					bulletRadian[i] = 7.85 ; //弾が飛んでく角度
				}
				
				mode = Game;
			}

			camera.position = 0.0;

			titleText.Draw();
	
			break;

		case Game:

			//自機の移動
			if (App::GetKey('A'))
				player.position.x -= playerSpeed;

			if (App::GetKey('D'))
				player.position.x += playerSpeed;

			//弾を発射する処理
			bulletInterval++;
			if (App::GetKey('J'))//長押しでずっと弾が出る
			{
				if (bulletInterval > 10)
				{
					bulletInterval = 5;
					//弾を自機に合わせる
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
		
			//自機の描画
		
			player.Draw();
			onigiri.position.x = 0;
			onigiri.position.y = 60;
			onigiri.Draw();

			//敵を動かす処理
			for (int i = 0;i < enemyNum; i++)
			{	//敵がおにぎりに飛んでく
			
				float enemyRadian = atan2f(
					onigiri.position.y - enemyPosition[i].y,
					onigiri.position.x - enemyPosition[i].x
				);
			
				//敵を動かす処理+
				enemyPosition[i] += Float3
				(
					cosf(enemyRadian),
					sinf(enemyRadian),
					0.0
				) * enemySpeed;
				enemy.position = enemyPosition[i];

					//当たり判定
					float hitRange = enemy.GetSize().x / 2.0 *
						enemy.scale.x;
					float hitRage =
						enemy.GetSize().y * 1 *
						enemy.scale.y;
					for (int j = 0; j < bulletNum; j++)
					{
						//敵と弾のとの当たり判定
						if (IsHit(
							enemyPosition[i],
							bulletPosition[j],
							hitRange
						))
						{
							enemyPosition[i] =
								camera.position + GetRandomPosition();

							//敵を倒すと弾のスピードが上がる
							count+= 1;
							if (count % 5 == 0)
							{
								enemySpeed += 2;
							}
						}
						
					}
					//onigiriとの当たり判定
						if (IsHit(
						enemyPosition[i],
						onigiri.position,
						hitRange
						))
						{
							mode =Gameover;//敵がおにぎりに当たるとタイトルに戻る
						}
						enemy.Draw();
					
					//弾を上に飛ばす処理
					bulletPosition[i] += Float3(
						cosf(bulletRadian[i]),
						sinf(bulletRadian[i]),
						0.0
					)* bulletSpeed;
					bullet.position = bulletPosition[i];
					//弾の描画
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

