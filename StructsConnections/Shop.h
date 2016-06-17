#pragma once
#include "includes.h";

struct Shop {

private:
	/// <summary>Заказы/покупки в магазине</summary>
	std::vector <Order> _Orders;
public:
	/// <summary>Уникальный идентификатор магазина</summary>
	unsigned int id = 0;
	/// <summary>Название магазина</summary>
	string name = "Магазин";
	Shop *_prev = NULL, *_next = NULL;

	template<typename DBStruct>
	friend bool SaveStruct(DBStruct _DBStruct);

	/// <summary>Конструктор магазина</summary>
	Shop(string name = "Магазин") {
		if (ShopStorage == NULL) { // Инициализация структуры
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

} *ShopStorage;