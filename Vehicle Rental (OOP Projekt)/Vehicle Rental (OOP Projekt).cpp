#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

class DayOfYear                                                 //class -DayOfYear- to store a date and is responsible for date validataions
{
private:
    unsigned short int dayOfYear, year;
    static const int days[];

public:
    DayOfYear(int d = 01, int y = 2021)
    {
        year = y;
        dayOfYear = d;
    }

    DayOfYear(int day, int month, int year2)
    {
        dayOfYear = days[month-1] + day;
        year = year2;
    }

    unsigned short int get_dayOfYear(){return dayOfYear;}

    void operator ++()                                          // -increment operator- to increment a date exactly one day
    {
        if(dayOfYear == 365)
        {
            dayOfYear = 1;
            year += 1;
        }
        else
        {
            dayOfYear ++;
        }
    }

    friend istream& operator>>(istream& in, DayOfYear& d)       //-Input operator- to input the date
    {
        int day1, month, year;
        char c;
        cin >> year >> c >> month >> c >> day1;
        DayOfYear a(day1, month, year);
        d = a;
        return in;
    }

    friend ostream& operator<<(ostream& out, DayOfYear& d)      //-Output operator- to output the date
    {
        int day2, month, year1;
        char c = '-';
        year1 = d.year;
        for (int i = 0; i < 12; i++)
            {
                if (d.dayOfYear - 1 >= days[i] && d.dayOfYear - 1 < days[i + 1]) {
				day2 = d.dayOfYear - days[i];
				month = i + 1;
			}
		}
		out << year1 << c;
		if (month < 10) {
			out << "0";
		}
		out << month << c;

		if (day2 < 10) {
			out << "0";
		}
		out << day2 << endl;

        return out;
    }
};
const int DayOfYear :: days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};


/*
The vehicle class is an abstract class and holds vehicle data.
Its child classes are vehicles that can be rented
*/
class Vehicle                                                   //abstract class -Vehicle-
{
private:
    const int no;
    string model;
    float price24h;

public:
    Vehicle(int n, string m, float p) : no(n), model(m), price24h(p){}

    virtual ~Vehicle(){}

    const int get_no()
    {
        return no;
    }

    string get_model()
    {
        return model;
    }

    float get_price(int p)
    {
        return price24h*p;
    }

    void virtual print() = 0;
};



class Bike : public Vehicle                                     //class for bicycles -Bike- inheriting public from class -Vehicle-
{
public:
    Bike(int n, string m) : Vehicle(n, m, 9.99){}

    void virtual print()
    {
        cout << get_no() << ": " << get_model() << " (Bike)" <<endl;
    }
};



class EMotorVehicle : public Vehicle                            //abstract class -EMotorVehicle- inheriting public from class -Vehicle-
{
public:
    EMotorVehicle(int n, string m, float p) : Vehicle(n, m, p){}

    bool virtual is_streetLegal(){}

    void virtual print()
    {
        cout << get_no() << ": " << get_model();
        if(EMotorVehicle::is_streetLegal() == false)
            cout << " (not street legal)";
    }
};



class EBike : public EMotorVehicle                              //a class -EBike- inheriting public from class -EMotorVehicle-
{
public:
    EBike(int n, string m, float p=29.99) : EMotorVehicle(n, m, p){}

    bool virtual is_streetLegal()
    {
        return true;
    }

    void virtual print()
    {
        EMotorVehicle::print();
        cout << " (EBike)" <<endl;
    }
};



class EScooter : public EMotorVehicle                           //class -EScooter- inheriting public from class -EMotorVehicle-
{
private:
    bool streetLegal;

public:
    EScooter(int n, string m, float p = 19.99, bool s = false) : EMotorVehicle(n, m, p), streetLegal(s){}

    bool virtual is_streetLegal()
    {
        return streetLegal;
    }

    void virtual print()
    {
        EMotorVehicle::print();
        cout << " (EScooter)" <<endl;
    }
};



class Rental                                                    //class for a rental -Rental- represents a rental of a vehicle
{
private:
    const int no;
    DayOfYear from;
    int days;
    string customer;
    static int Last_no;

public:
    Rental(string c, DayOfYear f, int ds = 1) : customer(c), from(f), days(ds), no(++Last_no){}

    int get_days()
    {
        return days;
    }

    DayOfYear get_from()
    {
        return from;
    }

    DayOfYear get_until()
    {
        DayOfYear f = from;
        for(int i; i < days; i++)
        {
            ++f;
        }
        return f;
    }

    void print()
    {
        DayOfYear f = get_until();

        cout << from << " to " << f << ", rental no. " << no << " for " << customer;
    }
};
int Rental::Last_no = 0;



class Schedule                                                  //class -Schedule- for a rental reservation schedule for a vehicle with a maximum of one year ahead
{
private:
    Vehicle *vehicle;
    Rental *year[365];
    Schedule *next;

public:
    Schedule(Vehicle* v) {
		vehicle = v;
		next = NULL;
		for (int i = 0; i < 365; i++) {
			this->year[i] = NULL;
		}
	}

    Vehicle* get_vehicle()
    {
        return vehicle;
    }

    Schedule* get_next()
    {
        return next;
    }

    void set_next(Schedule* n)
    {
        next = n;
    }

    bool isFree(DayOfYear sd, int nd)
    {
        while(nd--)
        {
            if(year[sd.get_dayOfYear()+nd] != NULL)
            {
                return false;
            }
        }
        return true;
    }

    float book(string c, DayOfYear sd, int nd)
    {
        Rental *r = new Rental(c, sd, nd);
        for(int i = 0; i < nd; i++)
        {
            year[sd.get_dayOfYear()-1] = r;
            ++sd;
        }
        return vehicle->get_price(nd);
    }

    void print(DayOfYear d)
    {
        vehicle->print();

        for(int i = 0; i < d.get_dayOfYear(); i++)
            {
                if(year[i]!=NULL)
                {
                    cout << endl;
                    year[i]->print();
                }
            }
    }

    void print()
    {
        cout << "SCHEDULE ";

        vehicle->print();

        for(int i = 0; i < 365; i++)
        {
            if(year[i]!=NULL)
                {
                    cout << endl;
                    year[i]->print();
                    cout << ", total: " << get_vehicle()->get_price(year[i]->get_days()) << " EUR";
                }
        }
    }
};



class Agency                                                    //class for the renting agency -Agency-
{
private:
    float profitPercent;   //for the profit percentage of the agency for each rental
    float profit;          //for the summed up profit of the agency
    Schedule *head;
    Schedule *last;

public:
    Agency()
    {
        profitPercent = 0.2;
        profit = 0.00;
        head = NULL;
        last = NULL;
    }

    float get_profit()
    {
        return profit;
    }

    void set_profitPercentage(float p)
    {
        profitPercent = p;
    }

    void add(Vehicle *p)
    {
        Schedule *nS = new Schedule(p);

        if (head == NULL)
        {
                head = nS;
                last = nS;
        }
		else
		{
			last->set_next(nS);
			last = nS;
		}
    }

    bool isFree(int vn, DayOfYear dr, int nd)
    {
        Schedule* s = searchFor(vn);
        if (s != NULL)
            {
                return s->isFree(dr, nd);
			}
		return true;
    }

    float book(int vn, string c, DayOfYear sd, int nd)
    {
        Schedule* s = searchFor(vn);

		float pro = 0, pri = 0;

		float h = 100.0;

		if (s != NULL)
            {
                pri = s->book(c, sd, nd);
                pro = pri * profitPercent / h;
                profit += pro;
            }
		return pri;
    }

    int chooseVehicle()
    {
        Schedule* t = head;

        for(int i = 1; i<365; i++)
        {
            if (t != NULL)
            {
                t->get_vehicle()->print();
                cout << endl;
                t = t->get_next();
            }
        }

        int vehicleNum;
		cout << "choose vehicle no: ";
		cin >> vehicleNum;

		return vehicleNum;
    }

    void print(DayOfYear d)
    {
        Schedule* t = head;

		for(int i = 1; i<365; i++)
            {
                if (t != NULL)
                {
                    t->print(d);
                    cout << endl;
                    t = t->get_next();
                }
            }
    }

    void print()
    {
        Schedule* t = head;

		for(int i = 1; i<365; i++)
            {
                if (t != NULL)
                {
                    t->print();
                    cout << endl;
                    t = t->get_next();
                }
            }

        cout << "profit: " << setprecision(2) << fixed << get_profit() << " EUR" << endl;
    }

    protected : Schedule *searchFor(int s)
    {
        Schedule* t = head;
        while (t != NULL)
            {
                if (t->get_vehicle()->get_no() == s) {
				return t;
			}
			t = t->get_next();
		}
        return NULL;
    }
};



int main()
{
    int bike, ebike, escooter, vehiclenummer, numberofvehicle;
    int nummervontagen;
    int IDV = 1, IDB = 1, IDeB = 1, IDeS = 1;
    float price, percentage;
    string model, nameofrenter;
    char c;
    DayOfYear date;
    DayOfYear today, tomorrow;
    ++tomorrow;
    Agency agency1;

    cout << "SIMPLIFIED SIMULATION OF A RENTAL AGENCY" << endl;
    cout << endl;
	cout << "========================================" << endl;
	cout << endl;

    do
    {
        cout << "MENUE" <<endl;
        cout << "A end of simulation" <<endl;
        cout << "B set new simulation date" <<endl;
        cout << "C new rental manually" <<endl;
        cout << "D print rental of today " << today;
        cout << "E print rental of tomorrow " << tomorrow;
        cout << "F print all rentals" <<endl;
        cout << "G print agency profit" <<endl;
        cout << "H set agency profit percent" <<endl;
        cout << "I add bikes" <<endl;
        cout << "J add E-bikes" <<endl;
        cout << "K add E-scooters" <<endl;
        cout << "L new rental simulation" <<endl;
        cout << "your choice: ";
        cin >> c;
        if (c == 'B'|| c =='b')
        {
            cout << "input date of today: ";
            cin >> today;
            tomorrow = today;
            ++tomorrow;
            cout << "________________________________________" <<endl;
        }
        else if (c == 'D'|| c == 'd')
        {
            cout << "SCHEDULE FOR " << today;
            agency1.print(today);
            cout << "________________________________________" <<endl;
        }
        else if (c == 'E'|| c == 'e')
        {
            cout << "SCHEDULE FOR " << tomorrow;
            agency1.print(tomorrow);
            cout << "________________________________________" <<endl;
        }
        else if (c == 'G'|| c == 'g')
        {
            cout << "agency profit: " << agency1.get_profit() << " EUR" << endl;
            cout << "________________________________________" <<endl;
        }
        else if (c == 'H'|| c == 'h')
        {
            cout << "agency profit percent: ";
            cin >> percentage;
            agency1.set_profitPercentage(percentage);
            cout << "________________________________________" <<endl;
        }
        else if (c == 'I'|| c == 'i')
        {
            cout << "how many Bikes in this simulation? ";
            cin >> bike;

            for (int i = 0; i < bike; i++)
                {
                    numberofvehicle = IDB++;
                    model = "City";
                    agency1.add(new Bike(IDV++, model));
                }
            cout << "________________________________________" <<endl;
        }
        else if (c == 'F'|| c == 'f')
        {
            agency1.print();
            cout << "________________________________________" <<endl;
        }
        else if (c == 'J'|| c == 'j')
        {
            cout << "how many EBikes in this simulation? ";
            cin >> ebike;

            for (int i = 0; i < ebike; i++)
                {
                    numberofvehicle = IDeB++;
                    model = "Trekky";
                    agency1.add(new EBike(IDV++, model));
                }
            cout << "________________________________________" <<endl;
        }
        else if (c == 'K'|| c == 'k')
        {
            cout << "how many EScooters in this simulation? ";
            cin >> escooter;

            for (int i = 0; i < escooter; i++)
                {
                    numberofvehicle = IDeS++;
                    model = "Scooty";
                    agency1.add(new EScooter(IDV++, model));
                }
            cout << "________________________________________" <<endl;
        }
        else if (c == 'C'|| c == 'c')
        {
            vehiclenummer = agency1.chooseVehicle();

            cout << "day of rental: ";
            cin >> date;
            cout << "number of days: ";
            cin >> nummervontagen;

            if (agency1.isFree(vehiclenummer, date, nummervontagen))
                {
                    cout << "input data vehicle renter name : " << endl;
                    cin >> nameofrenter;
                    price = agency1.book(vehiclenummer, nameofrenter, date, nummervontagen);
                    cout << "booked, price for this rental: " << setprecision(2) << fixed << price << " EUR" << endl;
                    cout << "________________________________________" <<endl;
                }
            else
                {
                    cout << "sorry vehicle already booked" << endl;
                    cout << "________________________________________" <<endl;
                }
        }
    }while(c != 'A'&& c != 'a');

    return 0;
}
