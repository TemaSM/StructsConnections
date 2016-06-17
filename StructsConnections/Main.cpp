#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>		// _getch
#include "DB.h"

using namespace std;

Shop* Perekr;
Shop* Diksi;

int main() {
	setlocale(LC_ALL, "Russian");

	/*
	std::ifstream str_stream;
	//ofstream fout("test.json");
	ifstream fin("test.json");

	str_stream << fin.rdbuf();
	in = json::parse(str_stream.str());
	*/	

	Perekr = Shop::Create("Перекресток");
	Diksi = Shop::Create("Дикси");

	//
	//Product* newProduct = Product::Create();
	//Perekr->Products->Add(newProduct);
	//Diksi->Products->Add(newProduct);
	//Diksi->Write();

	Order* newOrder = new Order(Perekr);

	for (int i = 0; i < 3; i++) {
		Product* newProduct = Product::Create(i, "Товар" + to_string(i), 10, 100);
		Diksi->Products->Add(newProduct);
		Perekr->Products->Add(newProduct);
		/*
		if (i % 2 == 0) {
			Perekr->Products->Add(newProduct);
		}
		*/
		newOrder->AddProduct(newProduct);		
	}

	printf("Синхронизация данных ...");
	DB::SyncData();
	_getch();
	Product* newProduct = Product::Create(12345, "Яблоко", 50, 500);
	_getch();
	Perekr->Products->Add(newProduct);
	Diksi->Products->Add(newProduct);

	cout << Diksi->Products->findOneByName("око")->name;
	_getch();
	/*
	_getch();
	newOrder->AddProduct(newProduct, 3);
	_getch();
	newOrder->backupProduct(newProduct);*/
	
	// ShopStorage->Products->Remove(newProduct);
	
	/*
	cout << "Сохранение товаров: " << ProductStorage->Save() << endl;
	cout << "Сохранение магазинов: " << ShopStorage->Save() << endl;
	cout << "Сохранение заказов: " << OrderStorage->Save() << endl;
	*/

	// Product *product0, *product1, *product2, *product3;
	// product0->name = "Яблоко";
	// product1->name = "Яблочко";
	// product2->name = "Апельсин";
	// product3->name = "Ананас";
	// product0->price = 10;
	// product1->price = 15;
	// product2->price = 20;
	// product3->price = 25;
	// Diksi->Products->Add(product0);
	// Diksi->Products->Add(product1);
	// Diksi->Products->Add(product2);
	// Diksi->Products->Add(product3);
	// 
	// ProductStorage->Write();

	// Поиск
	/*
	for (Product product : MainShop->Products->findByName("Ябл")) {
		cout << product->price << endl;
	}
	*/
	
	
	/*
	MainShop->Products.push_back(product0);
	MainShop->Products.push_back(product1);
	MainShop->Products.push_back(product2);
	MainShop->Products.push_back(product3);
	// cout << MainShop->Products.at(2)->name << endl;

	Order order0;
	// order0->price = product0->price + product1->price;
	order0.Products.push_back(product0);
	order0.Products.push_back(product1);
	
	Client client0;
	client0.fisrtName = "Артем";
	client0.lastName = "Смирнов";
	client0.Orders.push_back(order0);

	order0.client = &client0;

	MainShop->Orders.push_back(order0);
	MainShop->Clients.push_back(client0);
	*/
	
	// cout << MainShop->Products->get(4)->name << endl;
	// system("pause");
}