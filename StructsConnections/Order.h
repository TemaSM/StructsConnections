#pragma once
#include "includes.h"

struct Order {
private:
	Shop* _Shop;
	std::vector <Product*> _Products;
	float _costs = 0;
public:
	/// <summary>���������� ������������� ��������</summary>
	unsigned int id;
	Order *_prev = NULL, *_next = NULL;

	/// <summary>���������� ����� ��������� ���� ������� � ������</summary>
	float Costs() {
		return _costs;
	};

	template<typename DBStruct>
	friend bool SaveStruct(DBStruct _DBStruct);

	/// <summary>����������� ������</summary>
	/// <param name="shop">������� � ������� ��� �������� �����</param>
	Order(Shop* shop) {
		if (OrderStorage == NULL) { // ������������� ���������
			id = 0;
			OrderStorage = this;			// ����� ������� ���������� ������� ���������
			OrderStorage->_prev = NULL;		// ��� ����� ������
			OrderStorage->_next = NULL;		// ��� ������ ������
		}
		else // ��������� ��� ����������������
		{
			// ����� ������� = this (����������� �������������)			
			this->_prev = OrderStorage;				// � ����� �������� ��������� ����� ������� �������
			OrderStorage->_next = this;				// � ������� ������� ��������� ����� �����
			OrderStorage = this;						// ����� ������� ���������� ������� ���������
			this->id = (OrderStorage->_prev != NULL) ? (OrderStorage->_prev->id + 1) : 1;	// ����� ���������� ������������ ����������� �� ���������� �������� ������				
			this->_next = NULL;						// � ������� �������� ���������� ����� NULL (����� ������)
		}
		this->_Shop = shop;
		OrderStorage->Save();
	};

	/// <summary>��������� ����� ������� ������</summary>
	std::vector <Product*> AddProduct(Product* product, unsigned int quantity = 1) {
		for (Product* _product : this->_Shop->Products->All()) {
			if (product->id == _product->id) {
				if (product->quantity - quantity >= 0) {
					this->_costs = this->_costs + (quantity * product->price);
					product->quantity -= quantity;			// ��������� ���-�� ������
					this->_Products.push_back(product);		// ��������� � ������ ��������� �� ��� �����
				}
			}
		}
		OrderStorage->Save();
		return this->_Products;
	};
	/// <summary>���������� ������ ������ ������ ������</summary>
	bool backupProduct(Product* product) {
		for (Product* _product : this->_Products) {
			if (product->id == _product->id) {
				_product->available = false; // ����� ����� �� ��������
				ProductStorage->Save();
				OrderStorage->Save();
				return true; // ������� ��������� �����
			}
		}
		return false; // �� ���� ��������� ����� (��� ��� � ������)
	};
	/// <summary>��������� ������ �� ����</summary>
	static bool Save() {
		return SaveStruct(OrderStorage);
	};

} *OrderStorage;