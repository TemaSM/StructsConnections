#pragma once
#include "includes.h"

template<typename DBStruct>
bool SaveStruct(DBStruct _DBStruct) {
	json DataJSON;
	ofstream fs;
	const char* type_name = typeid(_DBStruct).name();

	if (type_name == typeid(ProductStorage).name())
	{
		Product* product = ProductStorage;
		while (product != NULL) {
			DataJSON[product->id] = {
				{ "id", product->id },
				{ "name", product->name },
				{ "vendorCode", product->vendorCode },
				{ "price", product->price },
				{ "quantity", product->quantity },
				{ "available", product->available }
			};
			product = product->_prev;
		}
		fs.open("Products.json"); // Открываем файл
	}
	else if (type_name == typeid(ShopStorage).name())
	{
		Shop* shop = ShopStorage;
		while (shop != NULL) {
			DataJSON[shop->id] = {
				{ "id", shop->id },
				{ "name", shop->name }
			};
			for (Product* product : shop->Products->All()) {
				DataJSON[shop->id]["Products"].push_back(product->id);
			}
			shop = shop->_prev;
		}
		fs.open("Shops.json");	// Открываем файл
	}
	else if (type_name == typeid(OrderStorage).name())
	{
		Order* order = OrderStorage;
		while (order != NULL) {
			DataJSON[order->id] = {
				{ "id", order->id },
				{ "costs", order->Costs() },
				{ "shop", order->_Shop->id }
			};

			for (Product* product : order->_Products) {
				if (product->available == false) {
					DataJSON[order->id]["Products"].push_back({
						{ "id", product->id },
						{ "name", product->name },
						{ "vendorCode", product->vendorCode },
						{ "price", product->price },
						{ "quantity", product->quantity },
						{ "available", product->available }
					});
				}
				else {
					DataJSON[order->id]["Products"].push_back(product->id);
				}
			}
			order = order->_prev;
		}
		fs.open("Orders.json");	// Открываем файл
	}
	else return false;

	if (!fs) return false;		// Если не удалось, возвращаем false
	fs << DataJSON.dump(2);		// Отправляем JSON
	fs.close();					// Закрываем stream
	return true;
}

namespace DB {
	/// <summary>Синхронизация данных структор с БД на диске</summary>
	bool SyncData() {
		bool success = false;
		success = ProductStorage->Save();
		success = ShopStorage->Save();
		success = OrderStorage->Save();
		return success;
	};
}
/*
template<typename T> struct type_name
{
	static const char* name() { static_assert(false, "You are missing a DECL_TYPE_NAME"); }
};
*/