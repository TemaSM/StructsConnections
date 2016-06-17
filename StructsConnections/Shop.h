#pragma once
#include "includes.h";

struct Shop {

private:
	/// <summary>������/������� � ��������</summary>
	std::vector <Order> _Orders;
public:
	/// <summary>���������� ������������� ��������</summary>
	unsigned int id = 0;
	/// <summary>�������� ��������</summary>
	string name = "�������";
	Shop *_prev = NULL, *_next = NULL;

	template<typename DBStruct>
	friend bool SaveStruct(DBStruct _DBStruct);

	/// <summary>����������� ��������</summary>
	Shop(string name = "�������") {
		if (ShopStorage == NULL) { // ������������� ���������
			ShopStorage = this;			// ����� ������� ���������� ������� ���������
			ShopStorage->_prev = NULL;	// ��� ����� ������
			ShopStorage->_next = NULL;	// ��� ������ ������
		}
		else // ��������� ��� ����������������
		{
			// ����� ������� = this (����������� �������������)			
			this->_prev = ShopStorage;				// � ����� �������� ��������� ����� ������� �������
			ShopStorage->_next = this;				// � ������� ������� ��������� ����� �����
			ShopStorage = this;						// ����� ������� ���������� ������� ���������
			this->id = (ShopStorage->_prev != NULL) ? (ShopStorage->_prev->id + 1) : 1;	// ����� ���������� ������������ ����������� �� ���������� �������� ������				
			this->_next = NULL;						// � ������� �������� ���������� ����� NULL (����� ������)
		}
		if (name != "�������") this->name = name; // ���� ��� ������ ������������ ������� �������� ��������, ���������� ���
		ShopStorage->Save();
	};

	/// <summary>����������� ��������</summary>
	static Shop* Create(string name = "�������") {
		Shop* newShop = new Shop(); // ����������� �����������, ��� ��� id ������������� ����� ����������
		newShop->name = name;
		ShopStorage->Save();
		return newShop;
	};

	/// <summary>��������� ������ �� ����</summary>
	static bool Save() {
		return SaveStruct(ShopStorage);
	};

	/// <summary>������ � ��������</summary>
	struct products {
	private:
		std::vector <Product*> _Products; // ��� ������ ��������� �� ��������� ����� �������
	public:
		/// <summary>��� ������ ����� ��������</summary>
		/// <returns>���������� ������ ������� (vector)</returns>
		std::vector <Product*> All() {
			return _Products;
		};

		/// <summary>����� ������ �� ������� �� �������</summary>
		/// <param name="index">������ �� �������� ����� ������ �����</param>
		/// <returns>���������� �����</returns>
		Product* get(int index) {
			try {
				return _Products[index]; // ��� ������ �� �������, ������������ ������ out_of_range
			}
			catch (...) {
				return _Products[0]; // � ����� ������ ������ ������ ������� �������
			}
		};

		/// <summary>����� ������ �� ID</summary>
		/// <param name="ID">������������� ������</param>
		Product* findByID(unsigned int ID) {
			for (Product* product : this->_Products) {
				if (product->id == ID) return product;
			}
		};

		/// <summary>����� ������� ������ �� ID</summary>
		/// <param name="ID">������������� ������</param>
		unsigned int findIndexByID(unsigned int ID) {
			int index = 0;
			for (Product* product : this->_Products) {
				if (product->id == ID) return index;
				index++;
			}
			return NULL;
		};

		/// <summary>����� ���� ������� �� ����� ��� ��������� ����������</summary>
		/// <param name="chars">������� ��� ���������� ���������� (� ������ ��������)</param>
		std::vector <Product*> findByName(string chars) {
			std::vector <Product*> _products;
			for (Product* product : this->_Products) {
				if (product->name.find(chars) != std::string::npos) _products.push_back(product);
			}
			return _products;
		};

		/// <summary>����� ������ �� ����� ��� ��������� ����������</summary>
		/// <param name="chars">������� ��� ���������� ���������� (� ������ ��������)</param>
		Product* findOneByName(string chars) {
			for (Product* product : this->_Products) {
				if (product->name.find(chars) != std::string::npos) return product;
			}
		};

		/// <summary>��������� ����� � ������� � ���������� ������ �� ����� ��������</summary>
		std::vector <Product*> Add(Product* product) {
			this->_Products.push_back(product); // ��������� � ������ ��������� �� ��� �����
			ShopStorage->Save();
			return this->_Products;
		};

		/// <summary>�������� ������ �� ��������</summary>
		static bool Remove(Product* product) {
			// Order::backupProduct(product);
			Shop* shop = ShopStorage;	// ����� ��������� �� ������� ��������
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