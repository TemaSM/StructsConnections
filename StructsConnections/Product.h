#pragma once
#include "includes.h";

struct Product {
	/// <summary>���������� ������������� ������</summary>
	unsigned int id = 0;
	/// <summary>������� ������</summary>
	unsigned int vendorCode = NULL;
	/// <summary>�������� ������</summary>
	string name = "�����";
	/// <summary>��������� ������ �� ��.</summary>
	float price = 1;
	/// <summary>����������� ������</summary>
	bool available = true;
	/// <summary>���-�� ������ �� ������ ����������</summary>
	unsigned int quantity = 1;
	Product *_prev = NULL;		// ��������� �� ���������� (������� ������� ������)
	Product *_next = NULL;		// ��������� �� ��������� (��������� ����������� ������� ������)

	template<typename DBStruct>
	friend bool SaveStruct(DBStruct _DBStruct);

	/// <summary>����������� ������</summary>
	Product() {
		if (ProductStorage == NULL) { // ������������� ���������
			id = 0;
			ProductStorage = this;			// ����� ������� ���������� ������� ���������
			ProductStorage->_prev = NULL;	// ��� ����� ������
			ProductStorage->_next = NULL;	// ��� ������ ������
		}
		else // ��������� ��� ����������������
		{
			// ����� ������� = this (����������� �������������)			
			this->_prev = ProductStorage;				// � ����� �������� ��������� ����� ������� �������
			ProductStorage->_next = this;				// � ������� ������� ��������� ����� �����
			ProductStorage = this;						// ����� ������� ���������� ������� ���������
			this->id = (ProductStorage->_prev != NULL) ? (ProductStorage->_prev->id + 1) : 1;	// ����� ���������� ������������ ����������� �� ���������� �������� ������
			this->_next = NULL;							// � ������� �������� ���������� ����� NULL (����� ������)
		}
		ProductStorage->Save();
	};

	/// <summary>����������� ������ � inline �����</summary>
	static Product* Create(unsigned int vendorCode, string name = "�����", float price = 1, unsigned int quantity = 1) {
		Product* newProduct = new Product();
		newProduct->vendorCode = vendorCode;
		newProduct->name = name;
		newProduct->price = price;
		newProduct->quantity = quantity;
		ProductStorage->Save();
		return newProduct;
	};

	/// <summary>����������� ������ � ����� ����������</summary>
	static Product* Create() {
		Product* newProduct = new Product();
		cout << "�������� ������: "; // Console::UI::write("�������� ������: ");
		cin >> newProduct->name;
		// scanf_s("%s", &newProduct->name);
		cout << "������� [����� �����]: "; // Console::UI::write("������� [����� �����]: ");
		cin >> newProduct->vendorCode;
		//scanf_s("%u", &newProduct->vendorCode);
		cout << "��������� � �.�. (� ��������� ������) : "; //Console::UI::write("��������� � �.�. (� ��������� ������) : ");
															//scanf_s("%li", &newProduct->quantity);
		cin >> newProduct->price;
		cout << "���������� �� ������ [����� �����]: "; // Console::UI::write("���������� �� ������ [����� �����]: ");
		cin >> newProduct->quantity;
		//scanf_s("%u", &newProduct->quantity);
		// Console::UI::wait(0);
		ProductStorage->Save();
		return newProduct;
	};

	/// <summary>�������� ������ �� ��������</summary>
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

	/// <summary>��������� ������ �� ����</summary>
	static bool Save() {
		return SaveStruct(ProductStorage);
	};

} *ProductStorage;