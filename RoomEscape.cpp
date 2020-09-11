#include <bangtal.h>
#include <iostream>
using namespace bangtal;

int main() {
	// ��� ����, �Һ�
	auto scene1 = Scene::create("Room1", "images/���-1.png");
	auto scene2 = Scene::create("Room2", "images/���-2.png");
	auto scene3 = Scene::create("Room3", "images/���-2.png");
	auto scene4 = Scene::create("End", "images/ending.png");
	auto scene5 = Scene::create("Fail", "images/fail.png");


	//1�� room
	auto locked = true;
	// ���� ����
	auto key = Object::create("images/����.png", scene1, 600, 150);
	key->setScale(0.2f);
	key->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		key->pick();
		return true;
		});
	//������ ����
	auto trash = Object::create("images/������.png", scene1, 1000, 200);
	trash->setScale(0.3f);
	auto trash_count = 0;
	trash->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (trash_count == 1) {				//�ι�° ��Ʈ
			showMessage("��Ϳ��� ������ ��Ӱ� �۾�������.");
			trash->setImage("images/��������.png");
			trash_count++;
		}
		else if (trash_count < 1) {			//ù��° ��Ʈ
			trash_count++;
			showMessage("�װ� ����ϱ� ����ϱ�? ��Ͱ� ��� ����� �˰��־�"); 
		}
		else {								//��Ʈ �ʱ�ȭ
			showMessage("�����뿣 �ƹ��͵� �����ϴ�.");
			trash->setImage("images/������.png");
			trash_count = 0;
		}
		return true;
		});
	// ȭ�� ����
	auto flowerpot_moved = false;
	auto flowerpot = Object::create("images/ȭ��.png", scene1, 550, 150);
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

	// 1�� �� ����
	auto door1_open = false;
	auto door1 = Object::create("images/��-������-����.png", scene1, 800, 270);

	//Ű�е� ����
	door1->setOnKeypadCallback([&](ObjectPtr object)->bool {
		locked = false;
		showMessage("öĿ��!~~");
		return true;
		});

	auto keypad = Object::create("images/Ű�е�.png", scene1, 915, 400);
	keypad->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showKeypad("CROW", door1);		//door3 �ݹ�
		return true;
		});

	// ���� ���µ��� Ű�� ��й�ȣ�� �Է��ؾ���
	door1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		//���� ���� -> scene2��, ���� ���� -> �� ����
		if (door1_open == true) {	//�� ����
			scene2->enter();
		}
		else {
			if (key->isHanded() && !locked) {
				door1->setImage("images/��-������-����.png");
				door1_open = true;
			}
			else if (key->isHanded() && locked) {
				showMessage("��й�ȣ�� Ǯ����Ѵ�.");
			}
			else {
				showMessage("���� ����ִ� ���谡 �켱 �ʿ��ҰŰ���.");
			}
		}
		return true;
		});



	// 2�� room
	auto scene2_lighted = true;
	auto door2 = Object::create("images/��-����-����.png", scene2, 320, 270);
	door2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene1->enter();
		return true;
		});

	auto door3_open = false;
	auto door3 = Object::create("images/��-������-����.png", scene2, 690, 315, false);
	door3->setScale(0.5f);
	door3->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {

		if (door3_open == true) {	//�� ����
			scene3->enter();
		}
		else {					//�� ����
			door3->setImage("images/��-������-����.png");
			door3_open = true;
		}
		return true;
		});

	auto button = Object::create("images/����ġ.png", scene2, 885, 440);
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

	// 3�� room
	auto crow = Object::create("images/���.png", scene3, 400, 150);
	auto wing = Object::create("images/����.png", scene3, 200, 50);
	auto cane = Object::create("images/������.png", scene3, 850, 50);
	wing->setScale(0.4f);
	cane->setScale(0.4f);
	wing->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {		//����
		wing->pick();		
		cane->drop();

		showMessage("������ �����ϼ̽��ϴ�. �׿��� ���� �ż���.");
		return true;
		});
	cane->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {		//����
		cane->pick();
		wing->drop();
		
		showMessage("�����̸� �����ϼ̽��ϴ�. �׿��� ���� �ż���.");
		return true;
		});
	crow ->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		if (wing->isHanded()) {
			showMessage("�����̱�. ����������� ���� �Ǵ��ϴٴ�.");
			scene5->enter();
		}
		else if (cane->isHanded()) {
			showMessage("����ŷ� �Ǵ����� �ʾƼ� ����, �� ����������");
			scene4->enter();
		}
		else {
			showMessage("�� ���簡 �ñ��ϰ���, �� ����ϱ� ����ϱ�?");
		}
		return true;
		});

	// scene4, ���� ��
	auto endGameBtn = Object::create("images/end.png", scene4, 50, 350);
	endGameBtn ->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
		return true;
		});
	// scene, ���� ��
	auto retryBtn = Object::create("images/retry.png", scene5, 500, 450);
	retryBtn->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		startGame(scene3);
		return true;
		});

	//���� ����
	startGame(scene1);
	return 0;
}