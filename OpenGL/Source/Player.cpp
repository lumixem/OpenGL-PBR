#include "Player.h"
#include "Model.h"
#include "ModelLibrary.h"
#include "glm/glm.hpp"

Player::Player(const ModelLibrary* ml)
{
	this->model = ml->GetModel("Player");
	this->model->SetPosition(glm::vec3(0.f, 0.f, -1.f));
	this->model->scaleFactor = 0.01f;
}

//void Player::Init()
//{
//	
//}
//
//void Player::Draw()
//{
//
//}
//
//void Player::Update()
//{
//	
//}