#include <bangtal.h>
#include <iostream>
using namespace bangtal;

int main() {
	// 장면 생성, 불빛
	auto scene1 = Scene::create("Room1", "images/배경-1.png");
	auto scene2 = Scene::create("Room2", "images/배경-2.png");
	auto scene3 = Scene::create("Room3", "images/배경-2.png");
	auto scene4 = Scene::create("End", "images/ending.png");
	auto scene5 = Scene::create("Fail", "images/fail.png");


	//1번 room
	auto locked = true;
	// 열쇠 생성
	auto key = Object::create("images/열쇠.png", scene1, 600, 150);
	key->setScale(0.2f);
	key->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		key->pick();
		return true;
		});
	//휴지통 생성
	auto trash = Object::create("images/휴지통.png", scene1, 1000, 200);
	trash->setScale(0.3f);
	auto trash_count = 0;
	trash->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (trash_count == 1) {				//두번째 힌트
			showMessage("까마귀에게 가려면 어둡고 작아져야해.");
			trash->setImage("images/빈휴지통.png");
			trash_count++;
		}
		else if (trash_count < 1) {			//첫번째 힌트
			trash_count++;
			showMessage("그건 까마귀일까 사람일까? 까마귀가 모든 비밀을 알고있어"); 
		}
		else {								//힌트 초기화
			showMessage("휴지통엔 아무것도 없습니다.");
			trash->setImage("images/휴지통.png");
			trash_count = 0;
		}
		return true;
		});
	// 화분 생성
	auto flowerpot_moved = false;
	auto flowerpot = Object::create("images/화분.png", scene1, 550, 150);
	flowerpot->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (flowerpot_moved == false) {
			if (action == MouseAction::MOUSE_DRAG_LEFT) {
				flowerpot->locate(scene1, 450, 150);
				flowerpot_moved = true;
			}
			else if (action == MouseAction::MOUSE_DRAG_RIGHT) {
				flowerpot->locate(scene1, 640, 150);
				flowerpot_moved = true;
			}
		}
		return true;
		});

	// 1번 문 생성
	auto door1_open = false;
	auto door1 = Object::create("images/문-오른쪽-닫힘.png", scene1, 800, 270);

	//키패드 생성
	door1->setOnKeypadCallback([&](ObjectPtr object)->bool {
		locked = false;
		showMessage("철커덕!~~");
		return true;
		});

	auto keypad = Object::create("images/키패드.png", scene1, 915, 400);
	keypad->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showKeypad("CROW", door1);		//door3 콜백
		return true;
		});

	// 문을 여는데에 키와 비밀번호를 입력해야함
	door1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		//문이 열림 -> scene2로, 문이 닫힘 -> 문 열기
		if (door1_open == true) {	//문 열림
			scene2->enter();
		}
		else {
			if (key->isHanded() && !locked) {
				door1->setImage("images/문-오른쪽-열림.png");
				door1_open = true;
			}
			else if (key->isHanded() && locked) {
				showMessage("비밀번호를 풀어야한다.");
			}
			else {
				showMessage("문이 잠겨있다 열쇠가 우선 필요할거같다.");
			}
		}
		return true;
		});



	// 2번 room
	auto scene2_lighted = true;
	auto door2 = Object::create("images/문-왼쪽-열림.png", scene2, 320, 270);
	door2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();
		return true;
		});

	auto door3_open = false;
	auto door3 = Object::create("images/문-오른쪽-닫힘.png", scene2, 690, 315, false);
	door3->setScale(0.5f);
	door3->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {

		if (door3_open == true) {	//문 열림
			scene3->enter();
		}
		else {					//문 닫힘
			door3->setImage("images/문-오른쪽-열림.png");
			door3_open = true;
		}
		return true;
		});

	auto button = Object::create("images/스위치.png", scene2, 885, 440);
	button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (scene2_lighted) {
			scene2->setLight(0.2f);
			door3->show();
			scene2_lighted = false;
		}
		else {
			scene2->setLight(1.0f);
			door3->hide();
			scene2_lighted = true;
		}
		return true;
		});

	// 3번 room
	auto crow = Object::create("images/까마귀.png", scene3, 400, 150);
	auto wing = Object::create("images/날개.png", scene3, 200, 50);
	auto cane = Object::create("images/지팡이.png", scene3, 850, 50);
	wing->setScale(0.4f);
	cane->setScale(0.4f);
	wing->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {		//실패
		wing->pick();		
		cane->drop();

		showMessage("날개를 선택하셨습니다. 그에게 말을 거세요.");
		return true;
		});
	cane->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {		//성공
		cane->pick();
		wing->drop();
		
		showMessage("지팡이를 선택하셨습니다. 그에게 말을 거세요.");
		return true;
		});
	crow ->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (wing->isHanded()) {
			showMessage("유감이군. 생김새만으로 나를 판단하다니.");
			scene5->enter();
		}
		else if (cane->isHanded()) {
			showMessage("생긴거로 판단하지 않아서 고마워, 널 내보내주지");
			scene4->enter();
		}
		else {
			showMessage("내 존재가 궁금하겠지, 난 사람일까 까마귀일까?");
		}
		return true;
		});

	// scene4, 게임 끝
	auto endGameBtn = Object::create("images/end.png", scene4, 50, 350);
	endGameBtn ->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
		return true;
		});
	// scene, 게임 끝
	auto retryBtn = Object::create("images/retry.png", scene5, 500, 450);
	retryBtn->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		startGame(scene3);
		return true;
		});

	//게임 시작
	startGame(scene1);
	return 0;
}