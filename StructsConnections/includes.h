#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct Product;
struct Shop;
struct Order;

#include "Product.h"
#include "Shop.h"
#include "Order.h"