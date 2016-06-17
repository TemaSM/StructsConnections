#pragma once
#include "includes.h"

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
	friend bool SaveStruct(DBStruct _DBStruct);

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