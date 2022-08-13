#pragma once

class Model;
class ModelLibrary;

class BaseObject
{
public:
	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Update() = 0;

	//virtual ~BaseObject() = 0;
	Model* model;

protected:
	ModelLibrary* modelLibrary;
};