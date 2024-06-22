
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "rapidjson/document.h"
#include "tinyxml2.h"

using namespace std;
using namespace rapidjson;
using namespace tinyxml2;

struct Employee {
    string name;
    int id;
    string department;
    double salary;
};

// Function to parse JSON file into Employee strcut
void parseJSON(const string& filename, vector<Employee>& employees) {
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        cerr << "Error opening JSON: " << filename << endl;
        return;
    }

    stringstream buffer;
    buffer << ifs.rdbuf();
    string content = buffer.str();

    Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError()) {
        cerr << "Error JSON parse error: " << filename << endl;
        return;
    }

    if (!doc.IsArray()) {
        cerr << "Error: JSON is not array: " << filename << endl;
        return;
    }

    for (SizeType i = 0; i < doc.Size(); ++i) {
        const Value& emp = doc[i];

        Employee e;
        e.name = emp["name"].GetString();
        e.id = emp["id"].GetInt();
        e.department = emp["department"].GetString();
        e.salary = emp["salary"].GetDouble();

        employees.push_back(e);
    }
}

// Function to parse XML file into Employee struct
void parseXML(const string& filename, vector<Employee>& employees) {
    XMLDocument doc;
    XMLError error = doc.LoadFile(filename.c_str());

    if (error != XML_SUCCESS) {
        cerr << "Error parsing XML: " << filename << endl;
        return;
    }

    XMLElement* root = doc.FirstChildElement("employees");
    if (!root) {
        cerr << "Error in XML structure: " << filename << endl;
        return;
    }

    for (XMLElement* elem = root->FirstChildElement("employee"); elem != nullptr; elem = elem->NextSiblingElement("employee")) {
        Employee e;
        e.name = elem->Attribute("name");
        e.id = stoi(elem->Attribute("id"));
        e.department = elem->Attribute("department");
        e.salary = stod(elem->Attribute("salary"));

        employees.push_back(e);
    }
}

// Function to calculate average salary of employees
double calculateAverageSalary(const vector<Employee>& employees) {
    if (employees.empty()) return 0.0;

    double totalSalary = 0.0;
    for (const auto& emp : employees) {
        totalSalary += emp.salary;
    }

    return totalSalary / employees.size();
}

// Function to find highest paid employee
Employee findHighestPaidEmployee(const vector<Employee>& employees) {
    if (employees.empty()) return Employee();

    auto maxSalaryEmp = max_element(employees.begin(), employees.end(),
        [](const Employee& a, const Employee& b) { return a.salary < b.salary; });

    return *maxSalaryEmp;
}

// Function to sort employees by ID
void sortEmployeesByID(vector<Employee>& employees) {
    sort(employees.begin(), employees.end(),
        [](const Employee& a, const Employee& b) { return a.id < b.id; });
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Using: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    vector<Employee> employees;

    // Switch to JSON or XML according to input file :d
    if (filename.find(".json") != string::npos) {
        parseJSON(filename, employees);
    }
    else if (filename.find(".xml") != string::npos) {
        parseXML(filename, employees);
    }
    else {
        cerr << "Error file type. Only JSON and XML files supported." << endl;
        return 1;
    }

    // Requisites
    double avgSalary = calculateAverageSalary(employees);
    Employee highestPaid = findHighestPaidEmployee(employees);
    sortEmployeesByID(employees);

    // Print results
    cout << "Average salary: $" << fixed << setprecision(2) << avgSalary << endl;
    cout << "Highest paid employee:" << endl;
    cout << "Name: " << highestPaid.name << endl;
    cout << "ID: " << highestPaid.id << endl;
    cout << "Department: " << highestPaid.department << endl;
    cout << "Salary: $" << fixed << setprecision(2) << highestPaid.salary << endl;
    cout << "Sorted list of employees by ID:" << endl;
    for (const auto& emp : employees) {
        cout << "Name: " << emp.name << ", ID: " << emp.id << ", Department: " << emp.department << ", Salary: $" << fixed << setprecision(2) << emp.salary << endl;
    }

    return 0;
}
