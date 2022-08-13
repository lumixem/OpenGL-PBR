#pragma once
#include "BaseObject.h"

class Player : public BaseObject
{
public:
	Player(const ModelLibrary* ml);

	void Init() override;
	void Draw() override;
	void Update() override;

};
