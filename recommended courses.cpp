#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include <algorithm>
using namespace std;
#define UNTAKABLE 1;
#define MAX_UNIT_QUANTITY 4;
#define EXTRA_MAX_UNIT_CAN_BE_TAKEN 24;
#define NORMAL_MAX_UNIT_CAN_BE_TAKEN 20;

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
	int counter = 0, can_be_taken = 0;
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
				can_be_taken = 1;
				break;
			}
	
	
			for (int h = 0; h < passed.size(); h++) {
				if (all_courses[i].prerequisites[z] == passed[h])
					can_be_taken += 1;
			}
		}
	
	
		if (can_be_taken == all_courses[i].prerequisites.size()) {
			can_be_taken = 0;
			takables.push_back(all_courses[i].id);
		}
	}
	return takables;


}


bool compare_by_units(const course &a, const course &b)
{
	return a.units < b.units;
}

bool compare_by_alphabet(const course &a, const course &b)
{
	return a.name < b.name;
}



vector<course> sort_by_units_and_alphabet(vector<int>takables, vector<course>courses)
{
	vector<course>sorted_by_unit_and_alphabet;
	vector<course>sorted_by_unit;
	vector<int>unit;
	for (int i = 0; i < takables.size(); i++) {
		for (int j = 0; j < courses.size(); j++) {
			if (takables[i] == courses[j].id) {
				sorted_by_unit.push_back(courses[j]);
			}
		}
	}
	sort(sorted_by_unit.rbegin(), sorted_by_unit.rend(), compare_by_units);


	int begin, end, counter = 0;
	int i = MAX_UNIT_QUANTITY;
	for (i; i >= 0; i--) {
		for (int j = 0; j < sorted_by_unit.size(); j++) {
			if (sorted_by_unit[j].units == i && counter == 0) {
				begin = j;
				counter += 1;
			}
			if (sorted_by_unit[j].units == i && counter > 0) {
				end = j;
			}
			if (j == sorted_by_unit.size() - 1 && counter > 0) {
				counter = 0;
				sort(sorted_by_unit.begin() + begin, sorted_by_unit.begin() + end, compare_by_alphabet);
			}
		}

	}
	sorted_by_unit_and_alphabet = sorted_by_unit;

	return sorted_by_unit_and_alphabet;
}

vector<course> sort_by_alphabet(vector<course>courses) {
	sort(courses.begin(), courses.end(), compare_by_alphabet);
	return courses;
}



double averaging_from_grades(vector<grades_course>grades, vector<course>all_courses)
{
	vector<int>units_of_grades;
	vector<double>average;
	double sum_of_multiply = 0;
	int sum_of_weight = 0;
	for (int i = 0; i < grades.size(); i++) {
		for (int j = 0; j < all_courses.size(); j++) {
			if (grades[i].id == all_courses[j].id) {
				units_of_grades.push_back(all_courses[j].units);
			}
		}
	}
	for (int i = 0; i < units_of_grades.size(); i++) {
		sum_of_multiply += (grades[i].grade)*(units_of_grades[i]);
	}

	for (int i = 0; i < units_of_grades.size(); i++)
		sum_of_weight += units_of_grades[i];

	return sum_of_multiply / sum_of_weight;

}



vector<course> recommend_course_schedule(double average, vector<course>sorted)
{

	vector<course>final_recommended_courses;
	int max_unit, taken_course_units = 0, counter = 0;
	if (average >= 17) {
		max_unit = EXTRA_MAX_UNIT_CAN_BE_TAKEN;
	}
	else
		max_unit = NORMAL_MAX_UNIT_CAN_BE_TAKEN;

	for (int i = 0; i < sorted.size(); i++) {
		if (taken_course_units + sorted[i].units > max_unit)
			continue;

		for (int j = 0; j < final_recommended_courses.size(); j++) {
			for (int z = 0; z < sorted[i].schedule.size(); z++) {
				for (int h = 0; h < final_recommended_courses[j].schedule.size(); h++) {

					if (sorted[i].schedule[z].days == final_recommended_courses[j].schedule[h].days) {
						if (sorted[i].schedule[z].start < final_recommended_courses[j].schedule[h].start) {
							if (sorted[i].schedule[z].end > final_recommended_courses[j].schedule[h].start)
								counter = UNTAKABLE;
						}
						else if (sorted[i].schedule[z].start > final_recommended_courses[j].schedule[h].start) {
							if (sorted[i].schedule[z].start < final_recommended_courses[j].schedule[h].end)
								counter = UNTAKABLE;
						}
						else if (sorted[i].schedule[z].start == final_recommended_courses[j].schedule[h].start)
							counter = UNTAKABLE;
					}


				}
			}

		}
		if (counter != 1) {
			final_recommended_courses.push_back(sorted[i]);
			taken_course_units += sorted[i].units;
		}
		counter = 0;
	}
	return final_recommended_courses;
}

void display_output(vector<course>final_recommended_courses)
{
	
	for (int i = 0; i < final_recommended_courses.size(); i++) {
		cout<< final_recommended_courses[i].id << endl;
	}
}

int main(int argc,char* argv[])
{
	double average_of_grades;
	vector<course>sorted;
	vector<int>takables;
	vector<grades_course> grades;
	vector<course> all_courses;
	vector<course> final_recommended_courses;
	

	reading_courses(all_courses, argv[1]);
	reading_grades(grades, argv[2]);
	takables = takable_courses(grades, all_courses);
	sorted = sort_by_units_and_alphabet(takables, all_courses);
	average_of_grades = averaging_from_grades(grades, all_courses);
	final_recommended_courses = recommend_course_schedule(average_of_grades, sorted);
	final_recommended_courses = sort_by_alphabet(final_recommended_courses);
	display_output(final_recommended_courses);

}