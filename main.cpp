#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <random>

//структура сотрудников
struct Worker
{
	std::string id;
	std::string name;
	bool change;
	double salary;

	//вывели
	void display() const 
	{
		std::cout << "ID: " << id << ". Имя: " << name << ". Смена: " << (change ? "День" : "Ночь") << ". Зарплата: " << salary << std::endl;
	}
};


//сохраняем информацию о сотрудниках в файл
void saveToFile(const std::vector<Worker>& workers, const std::string& fileWorker) 
{
	std::ofstream file(fileWorker);
	if (!file.is_open()) 
	{
		std::cerr << "Ошибка открытия файла " << fileWorker << std::endl;
		return;
	}
	for (const auto& worker : workers) 
	{
		file << worker.id << "," << worker.name << "," << (worker.change ? "День" : "Ночь") << "," << worker.salary << std::endl;
	}

	file.close();
};

//
bool findElement(std::vector<std::string> ids, std::string targetID) {
	for (std::string i : ids) {
		if (i == targetID)
		{
			return true;
		}
	}
	return false;
}

//считываем с файла
std::vector<Worker> readFromFile(const std::string& fileWorker)
{
	std::ifstream file(fileWorker);
	if (!file.is_open()) 
	{
		std::cerr << "Ошибка открытия файла " << fileWorker << std::endl;
		return {};
	}

	//пихни сюда цикл для считывания
	std::vector<Worker> workers;
	std::vector<std::string> uniqueId;  //вот он сет

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string id, name, change;
		double salary;

		if (std::getline(iss, id, ',') && std::getline(iss, name, ',') && std::getline(iss, change, ',') && iss >> salary)
		{
			//std::string workerData = id + name + change;
			if (!findElement(uniqueId,id)) 
			{
				Worker worker;
				worker.id = id;
				worker.name = name;
				worker.change = (change == "День");
				worker.salary = salary;

				workers.push_back(worker);
			}
		}
	}
	file.close();
	//копируем в вектор
	//workers.assign(uniqueId.begin(), uniqueId.end());

	return workers;

}

//пихаем данные
std::vector<Worker> inputWorkers(int numWorkers)
{
	std::vector<Worker> workers;

	srand(time(nullptr));

	for (int i = 0; i < numWorkers; i++)
	{
		Worker worker;
		// я захотела сделать именно в таком формате, поскольку не понимаю для чего нам генерировать абсолютно все разные данные
		worker.id = rand() % 100 + 1;
		//worker.id = "1";

		std::cout << "Введиет имя сотрудника " << i + 1 << ": ";
		std::getline(std::cin >> std::ws, worker.name);

		// Генерация случайной смены (true - дневная, false - ночная)
		worker.change = rand() % 2 == 0;
		//worker.change = true;

		std::cout << "Введите зарплату сотрудника " << i + 1 << ": ";
		std::cin >> worker.salary;

		workers.push_back(worker);
	}

	return workers;
}

//сравниваем зарплату
bool compareSalary(const Worker& w1, const Worker& w2) 
{
	return w1.salary > w2.salary;
};

// Функция сравнения для сортировки по убыванию зп
bool compareSalaryMin(const Worker& w1, const Worker& w2) 
{
	return w1.salary < w2.salary;
}

void displayTopThree(const std::vector<Worker>& workers, const std::string& title) {
	std::cout << title << std::endl;
	for (size_t i = 0; i < std::min(workers.size(), size_t(3)); ++i) {
		workers[i].display();
	}
	std::cout << std::endl;
}


int main() {
	setlocale(LC_ALL, "Russian");

	const std::string file = "workers.txt";
	const int numWorkers = 3;

	std::vector<Worker> workers = inputWorkers(numWorkers);

	//Сохранили
	saveToFile(workers, file);

	workers = readFromFile(file); //чтение уник записей

	// Вывели
	std::cout << "Сотрудники:" << std::endl;
	for (const auto& worker : workers) 
	{
		worker.display();
	}
	std::cout << std::endl;

	//Сохранили
	saveToFile(workers, file);

	//выводим все нужное согласно тз
	std::cout << "Сотрудники дневной смены:" << std::endl;
	for (const auto& worker : workers)
	{
		if (worker.change == true)
		{
			worker.display();
		}
	}
	std::cout << std::endl;
	std::cout << "Сотрудники ночной смены:" << std::endl;
	for (const auto& worker : workers)
	{
		if (worker.change == false)
		{
			worker.display();
		}
	}

	//Сохранили
	saveToFile(workers, file);

	// Сортировка по высокой зп
	std::sort(workers.begin(), workers.end(), compareSalary);

	//// Вывод трех сотрудников с высокой зп
	//std::cout << "Три сотрудника с самой высокой зп:" << std::endl;
	//for (size_t i = 0; i < std::min(workers.size(), size_t(3)); ++i) {
	//	workers[i].display();
	//}
	std::cout << std::endl;
	std::sort(workers.begin(), workers.end(), compareSalary);
	displayTopThree(workers, "Три сотрудника с самой высокой зп:"); 

	//фильтрация посменно
	std::vector<Worker> dayWorkers;
	std::vector<Worker> nightWorkers;
	for (const auto& worker : workers) {
		if (worker.change) {
			dayWorkers.push_back(worker);
		}
		else {
			nightWorkers.push_back(worker);
		}
	}
	std::sort(dayWorkers.begin(), dayWorkers.end(), compareSalary);
	std::sort(nightWorkers.begin(), nightWorkers.end(), compareSalary);

	displayTopThree(dayWorkers, "Три сотрудника с самой высокой зп среди дневных:");
	displayTopThree(nightWorkers, "Три сотрудника с самой высокой зп среди ночных:");

	//Сохранили
	saveToFile(workers, file);

	// Сортировка по маленькой зп
	std::sort(workers.begin(), workers.end(), compareSalaryMin);
	displayTopThree(workers, "Три сотрудника с самой низкой зп:");

	//посменная фильтрация
	std::sort(dayWorkers.begin(), dayWorkers.end(), compareSalaryMin);
	std::sort(nightWorkers.begin(), nightWorkers.end(), compareSalaryMin);

	displayTopThree(dayWorkers, "Три сотрудника с самой низкой зп среди дневных:");
	displayTopThree(nightWorkers, "Три сотрудника с самой низкой зп среди ночных:");

	//Сохранили
	saveToFile(workers, file);

	return 0;
}