#pragma once
#include "includes.h";

/// <summary>Модель данных "Товар"</summary>
struct Product {
	/// <summary>Уникальный идентификатор товара</summary>
	unsigned int id = 0;
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
	friend bool SaveStruct(DBStruct _DBStruct);

	/// <summary>Конструктор товара</summary>
	Product() {
		if (ProductStorage == NULL) { // Инициализация структуры
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

	/// <summary>Удаление товара</summary>
	bool Delete(Product* product) {
		delete product;
		ProductStorage->Save();
	}

	/// <summary>Сохраняет элементы модели данных Product на диск в Products.json</summary>
	static bool Save() { return SaveStruct(ProductStorage); };

} *ProductStorage;