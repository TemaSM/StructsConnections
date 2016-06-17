#pragma once
#include "includes.h"

namespace DB {
	/// <summary>Принудительное сохранение структур в БД на диске</summary>
	bool Save() {
		bool success = false;
		success = ProductStorage->Save();
		success = ShopStorage->Save();
		success = OrderStorage->Save();
		return success;
	};
}

template<typename DBStruct>
/// <summary>Сохранение всей структуры в файл на диске</summary>
bool SaveStruct(DBStruct _DBStruct) {
	json DataJSON;
	ofstream fs;
	const char* type_name = typeid(_DBStruct).name();

	// ProductStorage->Save()
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
	// ShopStorage->Save()
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
	// OrderStorage->Save()
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
				// Если товар будет удалён из магазина, сохраним его копию прямо в заказе
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
				// Товар не будет удалён, поэтому просто оставляем в массиве его ID
				else {
					DataJSON[order->id]["Products"].push_back(product->id);
				}
			}
			order = order->_prev;
		}
		fs.open("Orders.json");	// Открываем файл
	}
	else
		return false;

	if (!fs) return false;		// Если не удалось, возвращаем false
	fs << DataJSON.dump(2);		// Отправляем JSON
	fs.close();					// Закрываем stream
	return true;
}

//TODO: Чтение данных с диска
/*
std::ifstream str_stream;
//ofstream fout("test.json");
ifstream fin("test.json");

str_stream << fin.rdbuf();
in = json::parse(str_stream.str());
*/