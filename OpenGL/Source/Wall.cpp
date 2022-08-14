#include "Wall.h"
#include "Model.h"
#include "ModelLibrary.h"

Wall::Wall(const ModelLibrary* ml)
{
	this->model = ml->GetModel("Wall");
	this->model->scaleFactor = 0.2f;
	this->model->SetPosition(glm::vec3(0.f, 0.f, -2.f));
}