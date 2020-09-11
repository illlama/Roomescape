#include <bangtal.h>
#include <string>
using namespace bangtal;

int main() {
	// 장면 생성, 불빛
	auto scene1 = Scene::create("Room1", "images/배경-1.png");
	auto scene2 = Scene::create("Room2", "images/배경-2.png");

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
	auto Memo = Object::create("images/쪽지.png", scene1, 900, 250, false);
	auto trash_count = 0;
	trash->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (trash_count == 1) {
			Memo->pick();
			showMessage("까마귀, 까마귀가 안에 있습니다. 까마귀를 조심하세요..");
			trash->setImage("images/빈휴지통.png");
		}
		else if (trash_count < 1) {
			trash_count++;
			showMessage("까마귀가 모든 비밀을 알고있어");
		}
		else {
			showMessage("휴지통엔 아무것도 없습니다.");
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
		showKeypad("AAAB", door1);		//door3 콜백
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
			endGame();
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


	//게임 시작
	startGame(scene1);
	return 0;
}