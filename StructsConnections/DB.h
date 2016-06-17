#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct Shop;
struct Product;
struct Order;

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

struct Product {
	/// <summary>Уникальный идентификатор товара</summary>
	unsigned int id;
	/// <summary>Артикул товара</summary>
	unsigned int vendorCode = NULL;
	/// <summary>Название товара</summary>
	string name = "Товар";
	/// <summary>Стоимость товара за шт.</summary>
	float price = 1;
	/// <summary>Доступность товара</summary>
	bool available = true;
	/// <summary>Кол-во товара на складе поставщика</summary>
	unsigned int quantity = 1;
	Product *_prev = NULL;		// Указывает на предыдущий (текущий элемент списка)
	Product *_next = NULL;		// Указывает на следующий (последний добавленный элемент списка)
	
	template<typename DBStruct>
	friend bool SaveStruct(DBStruct db_struct);

	/// <summary>Конструктор товара</summary>
	Product() {
		if (ProductStorage == NULL) { // Инициализация структуры
			id = 0;
			ProductStorage = this;			// Новый элемент становится текущим элементом
			ProductStorage->_prev = NULL;	// Это хвост списка
			ProductStorage->_next = NULL;	// Это голова списка
		}
		else // Структура уже инициализирована
		{
			// Новый элемент = this (создаваемый конструктором)			
			this->_prev = ProductStorage;				// В новом элементе предыдуим будет текущий элемент
			ProductStorage->_next = this;				// В текущем элемент следующим будет новый
			ProductStorage = this;						// Новый элемент становится текущим элементом
			this->id = (ProductStorage->_prev != NULL) ? (ProductStorage->_prev->id + 1) : 1;	// Новый уникальный идетификатор основываясь на предыдущем элементе списка
			this->_next = NULL;							// В текущем элементе следующими будет NULL (конец списка)
		}
		ProductStorage->Save();
	};

	/// <summary>Конструктор товара в inline стиле</summary>
	static Product* Create(unsigned int vendorCode, string name = "Товар", float price = 1, unsigned int quantity = 1) {
		Product* newProduct = new Product();
		newProduct->vendorCode = vendorCode;
		newProduct->name = name;
		newProduct->price = price;
		newProduct->quantity = quantity;
		ProductStorage->Save();
		return newProduct;
	};

	/// <summary>Конструктор товара в стиле интерфейса</summary>
	static Product* Create() {
		Product* newProduct = new Product();
		cout << "Название товара: "; // Console::UI::write("Название товара: ");
		cin >> newProduct->name;
		// scanf_s("%s", &newProduct->name);
		cout << "Артикул [целое число]: "; // Console::UI::write("Артикул [целое число]: ");
		cin >> newProduct->vendorCode;
		//scanf_s("%u", &newProduct->vendorCode);
		cout << "Стоимость в у.е. (с плавающей точкой) : "; //Console::UI::write("Стоимость в у.е. (с плавающей точкой) : ");
		//scanf_s("%li", &newProduct->quantity);
		cin >> newProduct->price;
		cout << "Количество на складе [целое число]: "; // Console::UI::write("Количество на складе [целое число]: ");
		cin >> newProduct->quantity;
		//scanf_s("%u", &newProduct->quantity);
		// Console::UI::wait(0);
		ProductStorage->Save();
		return newProduct;
	};

	/// <summary>Удаление товара из магазина</summary>
	/*bool Remove(Product* product) {
		ShopStorage->Products
		// Order::backupProduct(product);
		unsigned int index = NULL;
		index = this->findIndexByID(product->id);
		if (index != NULL) {
			this->_Products.erase(this->_Products.begin() + index);
			ShopStorage->Save();
			return true;
		}
		return false;
	};*/

	/// <summary>Сохраняет данные на диск</summary>
	static bool Save() {
		return SaveStruct(ProductStorage);
	};

} *ProductStorage;

struct Shop {

private:
	std::vector <Order> _Orders;
public:
	/// <summary>Уникальный идентификатор магазина</summary>
	unsigned int id;
	/// <summary>Название магазина</summary>
	string name = "Магазин";
	Shop *_prev = NULL, *_next = NULL;

	/// <summary>Конструктор магазина</summary>
	Shop(string name = "Магазин") {
		if (ShopStorage == NULL) { // Инициализация структуры
			id = 0;
			ShopStorage = this;			// Новый элемент становится текущим элементом
			ShopStorage->_prev = NULL;	// Это хвост списка
			ShopStorage->_next = NULL;	// Это голова списка
		}
		else // Структура уже инициализирована
		{
			// Новый элемент = this (создаваемый конструктором)			
			this->_prev = ShopStorage;				// В новом элементе предыдуим будет текущий элемент
			ShopStorage->_next = this;				// В текущем элемент следующим будет новый
			ShopStorage = this;						// Новый элемент становится текущим элементом
			this->id = (ShopStorage->_prev != NULL) ? (ShopStorage->_prev->id + 1) : 1;	// Новый уникальный идетификатор основываясь на предыдущем элементе списка				
			this->_next = NULL;						// В текущем элементе следующими будет NULL (конец списка)
		}
		if (name != "Магазин") this->name = name; // Если при вызове конструктора указали название магазина, используем его
		ShopStorage->Save();
	};

	/// <summary>Товары в магазине</summary>
	struct products {
	private:
		std::vector <Product*> _Products; // Тут храним указатели на структуры самих товаров
	public:
		/// <summary>Все товары этого магазина</summary>
		/// <returns>Вовзращает массив товаров (vector)</returns>
		std::vector <Product*> All() {
			return _Products;
		};
		
		/// <summary>Выбор товара из массива по индексу</summary>
		/// <param name="index">Индекс по которому будет выбран товар</param>
		/// <returns>Возвращает товар</returns>
		Product* get(int index) {
			try {
				return _Products[index]; // При выходе за пределы, вываливается ошибка out_of_range
			}
			catch (...) {
				return _Products[0]; // В таком случае просто отдаем нулевой элемент
			}
		};
		
		/// <summary>Поиск товара по ID</summary>
		/// <param name="ID">Идентификатор товара</param>
		Product* findByID(unsigned int ID) {
			for (Product* product : this->_Products) {
				if (product->id == ID) return product;
			}
		};

		/// <summary>Поиск индекса товара по ID</summary>
		/// <param name="ID">Идентификатор товара</param>
		unsigned int findIndexByID(unsigned int ID) {
			int index = 0;
			for (Product* product : this->_Products) {
				if (product->id == ID) return index;
				index++;
			}
			return NULL;
		};

		/// <summary>Поиск всех товаров по имени при частичном совпадении</summary>
		/// <param name="chars">Символы для частичного совпадения (с учетом регистра)</param>
		std::vector <Product*> findByName(string chars) {
			std::vector <Product*> _products;
			for (Product* product : this->_Products) {
				if (product->name.find(chars) != std::string::npos) _products.push_back(product);
			}
			return _products;
		};
		
		/// <summary>Поиск товара по имени при частичном совпадении</summary>
		/// <param name="chars">Символы для частичного совпадения (с учетом регистра)</param>
		Product* findOneByName(string chars) {
			for (Product* product : this->_Products) {
				if (product->name.find(chars) != std::string::npos) return product;
			}
		};
		
		/// <summary>Добавляет товар в магазин и возвращает массив со всеми товарами</summary>
		std::vector <Product*> Add(Product* product) {
			this->_Products.push_back(product); // Добавляет в массив указатель на сам товар
			ShopStorage->Save();
			return this->_Products;
		};
		
		/// <summary>Удаление товара из магазина</summary>
		static bool Remove(Product* product) {
			// Order::backupProduct(product);
			Shop* shop = ShopStorage;	// Будем проходить по каждому магазину
			while (shop != NULL) {
				unsigned int index = 0;				
				for (Product* _product : shop->Products->_Products) {
					if (_product->id == product->id) break;
					index++;
				}
				shop->Products->_Products.erase(shop->Products->_Products.begin() + index);
				shop = shop->_prev;
			}
			ShopStorage->Save();
			return true;
		};

	} *Products = new products();

	template<typename DBStruct>
	friend bool SaveStruct(DBStruct db_struct);

	/// <summary>Конструктор магазина</summary>
	static Shop* Create(string name = "Магазин") {
		Shop* newShop = new Shop(); // Исползутеся конструктор, так что id автоматически будет установлен
		newShop->name = name;
		ShopStorage->Save();
		return newShop;
	};

	/// <summary>Сохраняет данные на диск</summary>
	static bool Save() {
		return SaveStruct(ShopStorage);
	};

} *ShopStorage;

struct Order {
	private:
		Shop* _Shop;
		std::vector <Product*> _Products;
		float _costs = 0;
	public:
		/// <summary>Уникальный идентификатор магазина</summary>
		unsigned int id;
		Order *_prev = NULL, *_next = NULL;

		/// <summary>Вовзращает общую стоимость всех товаров в заказе</summary>
		float Costs() {
			return _costs;
		};

		template<typename DBStruct>
		friend bool SaveStruct(DBStruct db_struct);

		/// <summary>Конструктор заказа</summary>
		/// <param name="shop">Магазин в котором был оформлен заказ</param>
		Order(Shop* shop) {
			if (OrderStorage == NULL) { // Инициализация структуры
				id = 0;
				OrderStorage = this;			// Новый элемент становится текущим элементом
				OrderStorage->_prev = NULL;		// Это хвост списка
				OrderStorage->_next = NULL;		// Это голова списка
			}
			else // Структура уже инициализирована
			{
				// Новый элемент = this (создаваемый конструктором)			
				this->_prev = OrderStorage;				// В новом элементе предыдуим будет текущий элемент
				OrderStorage->_next = this;				// В текущем элемент следующим будет новый
				OrderStorage = this;						// Новый элемент становится текущим элементом
				this->id = (OrderStorage->_prev != NULL) ? (OrderStorage->_prev->id + 1) : 1;	// Новый уникальный идетификатор основываясь на предыдущем элементе списка				
				this->_next = NULL;						// В текущем элементе следующими будет NULL (конец списка)
			}
			this->_Shop = shop;
			OrderStorage->Save();
		};		

		/// <summary>Добавляет товар корзину заказа</summary>
		std::vector <Product*> AddProduct(Product* product, unsigned int quantity = 1) {
			for (Product* _product : this->_Shop->Products->All()) {
				if (product->id == _product->id) {
					if (product->quantity - quantity >= 0) {
						this->_costs = this->_costs + (quantity * product->price);
						product->quantity -= quantity;			// Уменьшаем кол-во товара
						this->_Products.push_back(product);		// Добавляет в массив указатель на сам товар
					}					
				}
			}
			OrderStorage->Save();
			return this->_Products;
		};
		/// <summary>Сохранение компии товара внутри заказа</summary>
		bool backupProduct(Product* product) {
			for (Product* _product : this->_Products) {
				if (product->id == _product->id) {
					_product->available = false; // Товар более не доступен
					ProductStorage->Save();
					OrderStorage->Save();
					return true; // Успешно сохранили товар
				}
			}
			return false; // Не стаи сохранять товар (его нет в заказе)
		};
		/// <summary>Сохраняет данные на диск</summary>
		static bool Save() {
			return SaveStruct(OrderStorage);
		};

} *OrderStorage;

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