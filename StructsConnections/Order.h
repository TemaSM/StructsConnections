#pragma once
#include "includes.h"

/// <summary>Модель данных "Заказ"</summary>
struct Order {

private:
	/// <summary>Магазин которому принадлежит заказ</summary>
	Shop* _Shop;
	/// <summary>Общая стоимость всех товаров в заказе</summary>
	float _costs = 0;
public:
	/// <summary>Уникальный идентификатор магазина</summary>
	unsigned int id = 0;
	Order *_prev = NULL, *_next = NULL;

	/// <summary>Все товары в заказе</summary>
	vector <Product*> Products;

	/// <summary>Общая стоимость всех товаров в заказе</summary>
	float Costs() {
		return _costs;
	};

	template<typename DBStruct>
	friend bool SaveStruct(DBStruct _DBStruct);

	/// <summary>Конструктор заказа</summary>
	/// <param name="shop">Магазин в котором был оформлен заказ</param>
	Order(Shop* shop) {
		if (OrderStorage == NULL) { // Инициализация структуры
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
		this->_Shop = shop;					// Устанавливаем связь с магазином
		shop->Orders.push_back(this);		// Добавляем текущий заказ в массив заказов/покупок магазина
		OrderStorage->Save();				// Сохранение заказов в БД на диске
		ShopStorage->Save();				// Сохранение магазинов в БД на диске
	};

	/// <summary>Добавляет товар корзину заказа</summary>
	/// <param name="product">Товар, который будет добавлен в корзину</summary>
	/// <param name="quantity">Количество товара (по умолчанию = 1)</summary>
	vector <Product*> AddProduct(Product* product, unsigned int quantity = 1) {
		for (Product* _product : this->_Shop->Products->All()) {
			if (product->id == _product->id) {
				if (product->quantity - quantity >= 0) {	// Проверяем, можем ли купить столько товара
					this->_costs = this->_costs + (quantity * product->price);	// Подсчитываем стооимость покупки такого кол-ва товара
					product->quantity -= quantity;			// Уменьшаем кол-во товара на складе
					this->Products.push_back(product);		// Добавляет в массив указатель на сам товар
				}
			}
		}
		OrderStorage->Save();		// Сохранение заказов в БД на диске
		return this->Products;
	};
	/// <summary>Сохранение копии товара внутри заказа</summary>
	/// <param name="product">Товар, который будет сохранен в историю заказа</summary>
	bool backupProduct(Product* product) {
		for (Product* _product : this->Products) {
			if (product->id == _product->id) {
				_product->available = false; // Товар более не доступен
				ProductStorage->Save();		// Сохранение товаров в БД на диске
				OrderStorage->Save();		// Сохранение заказов в БД на диске
				return true; // Успешно сохранили товар
			}
		}
		return false; // Не стали сохранять товар (его нет в заказе)
	};
	/// <summary>Сохраняет элементы модели данных Order на диск в Orders.json</summary>
	static bool Save() { return SaveStruct(OrderStorage); };

} *OrderStorage;