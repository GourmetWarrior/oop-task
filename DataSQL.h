#pragma once

#include <iostream>
#include "sqlite3.h"
#include "Data.h"

class DataSQL : public Data {
private:
    static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
        for (int i = 0; i < argc; i++) {
            std::cout << '\n' << azColName[i] << ' ' << argv[i] ? argv[i] : "NULL";
        }
        std::cout << '\n';
        return 0;
    }

    char* zErrMsg = NULL;
    int rc;
    std::string sql;
public:
    ~DataSQL() {
        delete[] zErrMsg;
    }
    bool read(const std::string& fileName) override;
    bool write(const std::string& fileName) override;
};

bool DataSQL::read(const std::string& fileName) {
    sqlite3* db;
    sqlite3_stmt* res;

    rc = sqlite3_open(fileName.c_str(), &db);
    if (rc) {
        std::cout << "Can't open database:\n" << sqlite3_errmsg(db);
    }

    sql = "SELECT ID, NAME, PRICE_PER_GRAM FROM MATERIALS;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(res) == SQLITE_ROW) {
            Material material;
            material.setID(sqlite3_column_int(res, 0));
            material.setName(reinterpret_cast<char const*>(sqlite3_column_text(res, 1)));
            material.setPricePerGram(reinterpret_cast<char const*>(sqlite3_column_text(res, 2)));
            m_registries.materials().add(material);
        }
    }

    sql = "SELECT ID, NAME, TYPE, WEIGHT, PRICE FROM PRODUCTS;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(res) == SQLITE_ROW) {
            Product product;
            product.setID(sqlite3_column_int(res, 0));
            product.setName(reinterpret_cast<char const*>(sqlite3_column_text(res, 1)));
            product.setType(reinterpret_cast<char const*>(sqlite3_column_text(res, 2)));
            product.setWeight(reinterpret_cast<char const*>(sqlite3_column_text(res, 3)));
            product.setProductPrice(reinterpret_cast<char const*>(sqlite3_column_text(res, 4)));
            m_registries.products().add(product);
        }
    }

    sql = "SELECT DISTINCT PRODUCT_ID, MATERIAL_ID FROM PRODUCTS_MATERIALS;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);

    if (rc == SQLITE_OK) {
        for (auto iter = m_registries.products().begin(); iter != m_registries.products().end(); ++iter) {
            std::vector<unsigned int> materialsID;
            while (sqlite3_step(res) == SQLITE_ROW) {
            
                if (*iter == sqlite3_column_int(res, 0)) {
                    materialsID.push_back(sqlite3_column_int(res, 1));
                }
            }
            m_registries.products().getObjectByID(*iter).setMaterialsID(materialsID);
        }
    }

    sql = "SELECT ID, SURNAME, NAME, SECNAME FROM CUSTOMERS;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(res) == SQLITE_ROW) {
            Customer customer;
            customer.setID(sqlite3_column_int(res, 0));
            customer.setSurname(reinterpret_cast<char const*>(sqlite3_column_text(res, 1)));
            customer.setName(reinterpret_cast<char const*>(sqlite3_column_text(res, 2)));
            customer.setSecname(reinterpret_cast<char const*>(sqlite3_column_text(res, 3)));
            m_registries.customers().add(customer);
        }
    }

    sql = "SELECT ID, DATE, CUSTOMER_ID FROM SALE_EVENTS;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(res) == SQLITE_ROW) {
            SaleEvent saleEvent;
            saleEvent.setID(sqlite3_column_int(res, 0));
            saleEvent.setDate(reinterpret_cast<char const*>(sqlite3_column_text(res, 1)));
            saleEvent.setCustomerID(sqlite3_column_int(res, 2));
            std::vector<unsigned int> productsID;
            saleEvent.setProductsID(productsID);
            m_registries.saleEvents().add(saleEvent);
        }
    }

    sql = "SELECT DISTINCT SALE_EVENT_ID, PRODUCT_ID FROM SALE_EVENTS_PRODUCTS;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &res, 0);
    if (rc == SQLITE_OK) {
        for (auto iter = m_registries.saleEvents().begin(); iter != m_registries.saleEvents().end(); ++iter) {
            std::vector<unsigned int> productsID;
            while (sqlite3_step(res) == SQLITE_ROW) {

                if (*iter == sqlite3_column_int(res, 0)) {
                    productsID.push_back(sqlite3_column_int(res, 1));
                }
            }
            m_registries.saleEvents().getObjectByID(*iter).setProductsID(productsID);
        }
    }

    sqlite3_close(db);
    return true;
}


bool DataSQL::write(const std::string& fileName) {
    sqlite3* db;

    rc = sqlite3_open(fileName.c_str(), &db);
    if (rc) {
        std::cout << "Can't open database:\n" << sqlite3_errmsg(db);
    }

    sql = "PRAGMA foreign_keys = ON;";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);


    sql = "CREATE TABLE IF NOT EXISTS MATERIALS("  \
        "ID INT PRIMARY KEY NOT NULL," \
        "NAME TEXT NOT NULL," \
        "PRICE_PER_GRAM TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    sql = "INSERT OR IGNORE INTO MATERIALS (ID,NAME,PRICE_PER_GRAM) VALUES ";
    for (auto iter = m_registries.materials().begin(); iter != m_registries.materials().end(); ++iter) {
        Material material = m_registries.materials().getObjectByID(*iter);
        sql.append("(").append(std::to_string(material.ID())).append(", '").append(material.name()).append("', '")
            .append(material.pricePerGram()).append("')");
        if (iter != m_registries.materials().end() - 1) {
            sql.append(", ");
        }
        else {
            sql.append(";");
        }
    }
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 


    sql = "CREATE TABLE IF NOT EXISTS PRODUCTS("\
        "ID INT PRIMARY KEY NOT NULL,"\
        "NAME TEXT NOT NULL," \
        "TYPE TEXT NOT NULL,"\
        "WEIGHT TEXT NOT NULL,"\
        "PRICE TEXT NOT NULL);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);


    sql = "INSERT OR IGNORE INTO PRODUCTS (ID,NAME,TYPE,WEIGHT,PRICE) VALUES ";
    for (auto iter = m_registries.products().begin(); iter != m_registries.products().end(); ++iter) {
        Product product = m_registries.products().getObjectByID(*iter);
        sql.append("(").append(std::to_string(product.ID())).append(", '").append(product.name()).append("', '")
            .append(product.type()).append("', '").append(product.weight()).append("', '").append(product.price()).append("')");

        if (iter != m_registries.products().end() - 1) {
            sql.append(", ");
        }
        else {
            sql.append(";");
        }
    }
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 


    sql = "CREATE TABLE IF NOT EXISTS PRODUCTS_MATERIALS("\
        "PRODUCT_ID INT NOT NULL,"\
        "MATERIAL_ID INT NOT NULL,"\
        "FOREIGN KEY(PRODUCT_ID) REFERENCES PRODUCTS(ID) ON UPDATE CASCADE ON DELETE CASCADE,"\
        "FOREIGN KEY(MATERIAL_ID) REFERENCES MATERIALS(ID) ON UPDATE CASCADE ON DELETE CASCADE);"; 
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 

    sql = "INSERT OR IGNORE INTO PRODUCTS_MATERIALS (PRODUCT_ID,MATERIAL_ID) VALUES ";
    for (auto iter1 = m_registries.products().begin(); iter1 != m_registries.products().end(); ++iter1) {
        Product product = m_registries.products().getObjectByID(*iter1);

        std::vector<unsigned int> materialsID = product.materialsID();
        for (auto iter2 = materialsID.begin(); iter2 != materialsID.end(); ++iter2) {
            sql.append("(").append(std::to_string(product.ID())).append(", ").append(std::to_string(*iter2)).append(")");
            if (iter2 != materialsID.end() - 1) {
                sql.append(", ");
            }
        }
        if (iter1 != m_registries.products().end() - 1) {
            sql.append(", ");
        }
        else {
            sql.append(";");
        }
    }
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 


    sql = "CREATE TABLE IF NOT EXISTS CUSTOMERS("\
        "ID INT PRIMARY KEY NOT NULL,"\
        "SURNAME TEXT NOT NULL," \
        "NAME TEXT NOT NULL,"\
        "SECNAME TEXT);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    sql = "INSERT OR IGNORE INTO CUSTOMERS (ID,SURNAME,NAME,SECNAME) VALUES ";
    for (auto iter = m_registries.customers().begin(); iter != m_registries.customers().end(); ++iter) {
        Customer customer = m_registries.customers().getObjectByID(*iter);
        sql.append("(").append(std::to_string(customer.ID())).append(", '").append(customer.surname()).append("', '")
            .append(customer.name()).append("', '").append(customer.secname()).append("')");
        if (iter != m_registries.customers().end() - 1) {
            sql.append(", ");
        }
        else {
            sql.append(";");
        }
    }
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 


    sql = "CREATE TABLE IF NOT EXISTS SALE_EVENTS("\
        "ID INT PRIMARY KEY NOT NULL,"\
        "DATE TEXT NOT NULL,"\
        "CUSTOMER_ID INT NOT NULL,"\
        "FOREIGN KEY(CUSTOMER_ID) REFERENCES CUSTOMERS(ID) ON UPDATE CASCADE ON DELETE SET NULL);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 
    
    sql = "INSERT OR IGNORE INTO SALE_EVENTS (ID,DATE,CUSTOMER_ID) VALUES ";
    for (auto iter = m_registries.saleEvents().begin(); iter != m_registries.saleEvents().end(); ++iter) {
        SaleEvent saleEvent = m_registries.saleEvents().getObjectByID(*iter);
        sql.append("(").append(std::to_string(saleEvent.ID())).append(", '").append(saleEvent.date()).append("', ")
            .append(std::to_string(saleEvent.customerID())).append(")");
        if (iter != m_registries.saleEvents().end() - 1) {
            sql.append(", ");
        }
        else {
            sql.append(";");
        }
    }
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 


    sql = "CREATE TABLE IF NOT EXISTS SALE_EVENTS_PRODUCTS("\
        "SALE_EVENT_ID INT NOT NULL,"\
        "PRODUCT_ID INT NOT NULL,"\
        "FOREIGN KEY(SALE_EVENT_ID) REFERENCES SALE_EVENTS(ID) ON UPDATE CASCADE ON DELETE CASCADE,"\
        "FOREIGN KEY(PRODUCT_ID) REFERENCES PRODUCTS(ID) ON UPDATE CASCADE ON DELETE CASCADE);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);

    sql = "INSERT OR IGNORE INTO SALE_EVENTS_PRODUCTS (SALE_EVENT_ID, PRODUCT_ID) VALUES ";
    for (auto iter1 = m_registries.saleEvents().begin(); iter1 != m_registries.saleEvents().end(); ++iter1) {
        SaleEvent saleEvent = m_registries.saleEvents().getObjectByID(*iter1);

        std::vector<unsigned int> productsID = saleEvent.productsID();
        for (auto iter2 = productsID.begin(); iter2 != productsID.end(); ++iter2) {
            sql.append("(").append(std::to_string(saleEvent.ID())).append(", ").append(std::to_string(*iter2)).append(")");
            if (iter2 != productsID.end() - 1) {
                sql.append(", ");
            }
        }
        if (iter1 != m_registries.saleEvents().end() - 1) {
            sql.append(", ");
        }
        else {
            sql.append(";");
        }
    }
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg); 

    sqlite3_close(db);
    return true;
}