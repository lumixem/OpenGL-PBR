#include "Wall.h"
#include "Model.h"
#include "ModelLibrary.h"

Wall::Wall(const ModelLibrary* ml)
{
	this->model = ml->GetModel("Wall");
	this->model->scaleFactor = 0.2f;
}
