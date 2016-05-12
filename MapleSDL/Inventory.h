#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory
{
public:
	std::vector<GameItemDrop> Items;

	Inventory();
	virtual ~Inventory();
};

#endif