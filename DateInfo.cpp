#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
class Date {
private:
  int day_;
  int month_;
  int year_;

  bool isValidDate(int day, int month, int year) {
    if (year < 1) {
      return false;
    }

    if (month < 1 || month > 12) {
      return false;
    }

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (isLeapYear(year)) {
      daysInMonth[1] = 29;
    }
    if (day < 1 || day > daysInMonth[month - 1]) {
      return false;
    }

    return true;
  }

  bool isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
  }

public:
  Date(int day, int month, int year) {
    if (isValidDate(day, month, year)) {
      day_ = day;
      month_ = month;
      year_ = year;
    }
    else {
      day_ = 1;
      month_ = 1;
      year_ = 1970;
    }
  }

  int GetDay() {
    return day_;
  }
  int GetMonth() {
    return month_;
  }
  int GetYear(){
    return year_;
  }

  Date add(int days){
    int daysInMonth[] = { 31, 28 + isLeapYear(year_), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int totalDays = day_ + days;
    int month = month_;
    int year = year_;

    while (totalDays > daysInMonth[month - 1]) {
      totalDays -= daysInMonth[month - 1];
      month++;
      if (month > 12) {
        month = 1;
        year++;
        if (isLeapYear(year)) {
          daysInMonth[1] = 29;
        }
        else {
          daysInMonth[1] = 28;
        }
      }
    }

    return Date(totalDays, month, year);
  }

  Date subtract(int days){
    int totalDays = day_ - days;
    int month = month_;
    int year = year_;

    while (totalDays < 1) {
      if (month == 1) {
        year--;
        month = 12;
      }
      else {
        month--;
      }

      int daysInPrevMonth = (month == 2 && isLeapYear(year)) ? 29 : 28;
      totalDays += daysInPrevMonth;
    }

    return Date(totalDays, month, year);
  }

  Date replace(int day = 0, int month = 0, int year = 0){
    if (day == 0) {
      day = day_;
    }
    if (month == 0) {
      month = month_;
    }
    if (year == 0) {
      year = year_;
    }
    return Date(day, month, year);
  }

  int week(){
    int jan1Weekday = (1 + year_ + (year_ - 1) / 4 - (year_ - 1) / 100 + (year_ - 1) / 400) % 7;
    int dayOfYear = 0;
    for (int m = 1; m < month_; ++m) {
      dayOfYear += (m == 2 && isLeapYear(year_)) ? 29 : 28;
    }
    dayOfYear += day_;

    return (dayOfYear + jan1Weekday - 1) / 7 + 1;
  }

  int totalWeeks(){
    Date jan1(1, 1, 1970);
    return DiffDays(jan1) / 7;
  }

  char* isoformat() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << year_ << "-"
      << std::setw(2) << month_ << "-" << std::setw(2) << day_;
    std::string str = ss.str();

    char* result = new char[str.length() + 1];
    std::copy(str.begin(), str.end(), result);
    result[str.length()] = '\0';

    return result;
  }
private:
  int DiffDays(Date& otherDate){
    int daysPerMonth[] = { 31, 28 + isLeapYear(year_), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int days1 = day_ + monthDays(month_, year_) + yearDays(year_);
    int days2 = otherDate.GetDay() + monthDays(otherDate.GetMonth(), otherDate.GetYear()) + yearDays(otherDate.GetYear());

    return abs(days1 - days2);
  }

  int monthDays(int month, int year){
    int daysInMonth[] = { 31, 28 + isLeapYear(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int days = 0;
    for (int i = 0; i < month - 1; ++i) {
      days += daysInMonth[i];
    }
    return days;
  }

  int yearDays(int year){
    int days = 0;
    for (int y = 1; y < year; ++y) {
      days += 365 + isLeapYear(y);
    }
    return days;
  }
};

int main() {
  Date date(15, 5, 2024);
  std::cout << "Current date: " << date.isoformat() << '\n';

  delete[] date.isoformat();
  return 0;
}
