#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>		// _getch
#include "DB.h"

using namespace std;

Shop* shop;
Product* product;

int main() {
	setlocale(LC_ALL, "Russian");

	DB::Save(); // Т.к. сейчас нет никаких данных в структурах программы, БД просто очищается
	shop = new Shop();
	product = new Product();

	cout << "Введите название создаваемого магазина: \n";
	cin >> shop->name;	
	cout << "--- Создание нового товара ---\n" ;	
	cout << "Название товара: \n";
	cin >> product->name;
	cout << "Артикул товара: \n";
	cin >> product->vendorCode;
	cout << "Стоимость товара: \n";
	cin >> product->price;
	cout << "Кол-во на складе: \n";
	cin >> product->quantity;
	ProductStorage->Save(); // Сохраняем все товары в БД на диске

	cout << "Присвоить товар " << product->name << " к магазину " << shop->name << " ?\n";
	char inputKey = _getch();
	switch (inputKey) {
		case 'y':
			shop->Products->Add(product); // Добавляю товар в магазин
		break;
	}
	DB::Save(); // Сохраняю все данные в БД на диск
	Order* order = new Order(shop);
	order->AddProduct(product); // Добавляю товар в заказ
	cout << "Стоимость всех товаров в заказе: " << order->Costs();
	_getch();

	order->backupProduct(product);	// Сохраняю информацию о товаре во все заказы магазина
	cout << "Резервная копия товара " << product->name << " успешно сохранена во всех заказах магазина " << shop->name << endl;
	// shop->Products->Remove(product); // Удаление товара из магазина
	// product->Delete(); // Удаление товара из БД

	system("pause");
}