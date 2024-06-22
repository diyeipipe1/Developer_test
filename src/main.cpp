
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


// Function to parse JSON file into Employee struct
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

    if (!doc.IsObject() || !doc.HasMember("employees") || !doc["employees"].IsArray()) {
        cerr << "Error: JSON format is incorrect: " << filename << endl;
        return;
    }

    const Value& employeesArray = doc["employees"];
    for (SizeType i = 0; i < employeesArray.Size(); ++i) {
        const Value& emp = employeesArray[i];

        // Error handling so that if data is missing or wrong type, employee skipped and error logged
        if (!emp.HasMember("name") || !emp.HasMember("id") || !emp.HasMember("department") || !emp.HasMember("salary")) {
            cerr << "Error: Missing required fields in employee object at index " << i << " in JSON: " << filename << endl;
            continue;
        }

        if (!emp["name"].IsString() || !emp["id"].IsInt() || !emp["department"].IsString() || !emp["salary"].IsNumber()) {
            cerr << "Error: Incorrect field types in employee object at index " << i << " in JSON: " << filename << endl;
            continue;
        }


        // Error handling, make sure both ID and Salary are positive numbers
        int id = emp["id"].GetInt();
        double salary = emp["salary"].GetDouble();

        if (id < 0 || salary < 0) {
            cerr << "Error: Negative id or salary in employee object at index " << i << " in JSON: " << filename << endl;
            continue;
        }

        Employee e;
        e.name = emp["name"].GetString();
        e.id = id;
        e.department = emp["department"].GetString();
        e.salary = salary;

        employees.push_back(e);
    }
}

// Function to check if a string represents a valid integer
bool isInteger(const char* str) {
    if (!str || *str == '\0') {
        return false;
    }

    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        ++str;
    }

    return true;
}

void parseXML(const string& filename, vector<Employee>& employees) {
    XMLDocument doc;
    XMLError error = doc.LoadFile(filename.c_str());

    if (error != XML_SUCCESS) {
        cerr << "Error parsing XML: " << filename << " Error: " << error << endl;
        return;
    }

    XMLElement* root = doc.FirstChildElement("employees");
    if (!root) {
        cerr << "Error in XML structure: " << filename << endl;
        return;
    }

    for (XMLElement* elem = root->FirstChildElement("employee"); elem != nullptr; elem = elem->NextSiblingElement("employee")) {
        XMLElement* nameElem = elem->FirstChildElement("name");
        XMLElement* idElem = elem->FirstChildElement("id");
        XMLElement* departmentElem = elem->FirstChildElement("department");
        XMLElement* salaryElem = elem->FirstChildElement("salary");

        if (!nameElem || !idElem || !departmentElem || !salaryElem) {
            cerr << "Error: Missing required fields in employee element in XML: " << filename << endl;
            continue;
        }

        if (!nameElem->GetText() || !idElem->GetText() || !departmentElem->GetText() || !salaryElem->GetText()) {
            cerr << "Error: Empty text in required fields in employee element in XML: " << filename << endl;
            continue;
        }

        const char* idText = idElem->GetText();
        if (!isInteger(idText)) {
            cerr << "Error: Invalid id (not an integer) in employee element in XML: " << filename << endl;
            continue;
        }

        try {
            int id = stoi(idElem->GetText());
            double salary = stod(salaryElem->GetText());

            if (id < 0 || salary < 0) {
                cerr << "Error: Negative id or salary in employee element in XML: " << filename << endl;
                continue;
            }

            Employee e;
            e.name = nameElem->GetText();
            e.id = id;
            e.department = departmentElem->GetText();
            e.salary = salary;

            employees.push_back(e);
        } catch (const invalid_argument& ia) {
            cerr << "Error: Invalid field types in employee element in XML: " << filename << " Error: " << ia.what() << endl;
            continue;
        } catch (const out_of_range& oor) {
            cerr << "Error: Field value out of range in employee element in XML: " << filename << " Error: " << oor.what() << endl;
            continue;
        }
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

    // Check if employees vector is empty
    if (employees.empty()) {
        cerr << "Error: No employee data found in the input file." << endl;
        return 1;
    }

    // Requisites
    double avgSalary = calculateAverageSalary(employees);
    Employee highestPaid = findHighestPaidEmployee(employees);
    sortEmployeesByID(employees);

    // Print results
    cout << "Average salary: $" << fixed << setprecision(2) << avgSalary << "\n\n";
    cout << "Highest paid employee:" << endl;
    cout << "Name: " << highestPaid.name << endl;
    cout << "ID: " << highestPaid.id << endl;
    cout << "Department: " << highestPaid.department << endl;
    cout << "Salary: $" << fixed << setprecision(2) << highestPaid.salary << "\n\n";
    cout << "Sorted list of employees by ID:" << endl;
    for (const auto& emp : employees) {
        cout << "Name: " << emp.name << ", ID: " << emp.id << ", Department: " << emp.department << ", Salary: $" << fixed << setprecision(2) << emp.salary << endl;
    }

    return 0;
}
