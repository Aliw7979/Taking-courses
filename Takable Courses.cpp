#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include <algorithm>
using namespace std;
#define UNTAKABLE 1;

typedef vector<int> pre;
typedef vector<double> times;
typedef struct schedule date;

struct schedule {
	string days;
	double start;
	double end;
};

struct course {
	int id;
	string name;
	int units;
	vector<date> schedule;
	pre prerequisites;
};

struct grades_course {
	int id;
	double grade;
};

void reading_grades(vector<grades_course>& grades, string address_of_grades)
{
	int num_of_row = 0;
	string oneline, temp;
	ifstream grades_file(address_of_grades);
	grades_file.ignore(1000, '\n');
	while (getline(grades_file, oneline)) {
		grades.push_back(grades_course());
		stringstream line(oneline);
		getline(line, temp, ',');
		grades[num_of_row].id = stoi(temp);
		temp.clear();
		getline(line, temp);
		grades[num_of_row].grade = stod(temp);
		num_of_row += 1;
	}
}


void import_schedule(string schedule, vector<course>&all_courses, int num_of_row)
{
	int counter_of_slash = 0;
	string temp;
	int number_of_days = 0;
	for (int i = 0; i < schedule.length(); i++) {
		if (schedule[i] == ':')
			schedule[i] = '.';
		else
			continue;
	}
	for (int i = 0; i < schedule.length(); i++) {
		if (schedule[i] == '/')
			number_of_days += 1;
		else
			continue;
	}
	number_of_days += 1;
	stringstream schedule_stream(schedule);
	for (int i = 0; i < number_of_days; i++) {
		all_courses[num_of_row].schedule.push_back(date());
		getline(schedule_stream, temp, '-');
		all_courses[num_of_row].schedule[i].days = temp;
		getline(schedule_stream, temp, '-');
		all_courses[num_of_row].schedule[i].start = stod(temp);

		if (number_of_days > 1) {
			if (counter_of_slash == number_of_days - 1)
				getline(schedule_stream, temp, '/');
			else
				getline(schedule_stream, temp, ',');
		}
		else
			getline(schedule_stream, temp, ',');

		all_courses[num_of_row].schedule[i].end = stod(temp);
	}

}

void reading_courses(vector<course>& all_courses, string address_of_courses)
{
	int num_of_row = 0;
	string oneline, temp;
	ifstream courses_file(address_of_courses);
	courses_file.ignore(1000, '\n');
	while (getline(courses_file, oneline)) {
		all_courses.push_back(course());
		stringstream line(oneline);
		getline(line, temp, ',');
		all_courses[num_of_row].id = stoi(temp);
		temp.clear();
		getline(line, temp, ',');
		all_courses[num_of_row].name = temp;
		temp.clear();
		getline(line, temp, ',');
		all_courses[num_of_row].units = stoi(temp);
		temp.clear();
		getline(line, temp, ',');
		import_schedule(temp, all_courses, num_of_row);
		temp.clear();
		getline(line, temp, ',');
		stringstream temp2(temp);
		while (getline(temp2, temp, '-')) {
			all_courses[num_of_row].prerequisites.push_back(stoi(temp));
			temp.clear();
		}
		num_of_row += 1;

	}
}

vector <int> takable_courses(vector<grades_course> grades, vector<course>all_courses)
{
	int counter = 0;
	vector<int>passed;
	vector<int>takables;

	for (int i = 0; i < grades.size(); i++) {
		if (grades[i].grade >= 10)
			passed.push_back(grades[i].id);
	}

	for (int i = 0; i < all_courses.size(); i++) {
		for (int j = 0; j < passed.size(); j++) {
			if (all_courses[i].id == passed[j])
				counter = UNTAKABLE;
		}
		if (counter == 1) {
			counter = 0;
			continue;
		}
		for (int z = 0; z < all_courses[i].prerequisites.size(); z++) {
			if (all_courses[i].prerequisites[0] == 0) {
				counter = 1;
				break;
			}
			for (int h = 0; h < passed.size(); h++) {
				if (all_courses[i].prerequisites[z] == passed[h])
					counter += 1;
			}
		}
		if (counter == all_courses[i].prerequisites.size()) {
			counter = 0;
			takables.push_back(all_courses[i].id);
		}
	}
	return takables;


}

vector<int> sorter_of_takable_courses(vector<course>all_courses, vector<int>takables)
{
	vector<int>sorted_id;
	vector<string>course_name;
	for (int i = 0; i < all_courses.size(); i++) {
		for (int j = 0; j < takables.size(); j++) {
			if (all_courses[i].id == takables[j])
				course_name.push_back(all_courses[i].name);
		}
	}
	sort(course_name.begin(), course_name.end());
	for (int i = 0; i< course_name.size(); i++) {
		for (int j = 0; j < all_courses.size(); j++) {
			if (course_name[i] == all_courses[j].name) {
				sorted_id.push_back(all_courses[j].id);
			}
		}
	}
	return sorted_id;
}

void display_output(vector<int>takables)
{
	for (int i = 0; i < takables.size(); i++) {
		cout << takables[i] << endl;
	}
}


int main(int argc,char* argv[])
{
	vector<int>takables;
	vector<grades_course> grades;
	vector<course> all_courses;

	reading_courses(all_courses, argv[1]);
	reading_grades(grades, argv[2]);
	takables = takable_courses(grades, all_courses);
	takables = sorter_of_takable_courses(all_courses, takables);
	display_output(takables);

}