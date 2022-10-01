#include <iostream>
#include <random>
#include <stdexcept>
#include <cmath>
#include <stdlib.h>
#include <ctime>

const float pi = 3.1415;

class List {
private:

    typedef struct node {
        int day;
        int hour;
        int min;
        float posX;
        float posY;
        bool virus;
        node* next;
    }*nodePtr;

    nodePtr head;
    nodePtr curr;
    nodePtr temp;
public:
    List() {
        head = NULL;
        curr = NULL;
        temp = NULL;
    }

    void AddNode(int addDay, int addHour, int addMinute, float addX, float addY, bool addVirus) {
        nodePtr n = new node;
        n->next = NULL;
        n->day = addDay;
        n->hour = addHour;
        n->min = addMinute;
        n->posX = addX;
        n->posY = addY;
        n->virus = addVirus;

        if (head != NULL) {
            curr = head;
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = n;
        }
        else {
            head = n;
        }
    }

    void DeleteNode(int delDay, int delHour, int delMinute, List Trajectory) {
        nodePtr delPtr = NULL;
        temp = Trajectory.FindNode(delDay);
        curr = temp;
        //&& curr->min != delMinute
        while (curr->next != NULL && curr->day == delDay && curr->hour != delHour) {
            temp = curr;
            curr = curr->next;
        }
        while (curr->next != NULL && curr->min != delMinute) {
            temp = curr;
            curr = curr->next;
        }
        if (curr == NULL) {
            std::cout << "Not in the list" << std::endl;
            delete delPtr;
        }
        else {
            delPtr = curr;
            curr = curr->next;
            temp->next = curr;
            if (delPtr == head) {
                head = head->next;
                temp = NULL;
            }
            //std::cout << "Day: " << delPtr->day << " Hour: " << delPtr->hour << " Minute: "<< delPtr->min << std::endl;
            delete delPtr;
            //std::cout << "Deleted" << std::endl;
        }
    }
    void PrintList() {
        curr = head;
        while (curr != NULL) {
            std::cout << "Day: " << curr->day << " Hour: " << curr->hour << " Minute: " << curr->min << " X: " << curr->posX << " Y: " << curr->posY << " Infected: " << curr->virus << std::endl;
            curr = curr->next;
        }
        std::cout << std::endl;
    }

    void PrintElement(int k) {
        curr = head;
        for (int i = 1; i <= k - 1; i++) {
            curr = curr->next;
        }
        if (curr != NULL) {
            std::cout << "Day: " << curr->day << " Hour: " << curr->hour << " Minute: " << curr->min << " X: " << curr->posX << " Y: " << curr->posY << std::endl;
        }
    }

    void AddToPosition(int k, int addDay, int addHour, int addMinute, float addX, float addY, bool addVirus, List& Trajectory) {
        nodePtr newNode = new node;
        newNode->next = NULL;
        newNode->day = addDay;
        newNode->hour = addHour;
        newNode->min = addMinute;
        newNode->posX = addX;
        newNode->posY = addY;
        newNode->virus = addVirus;
        curr = Trajectory.FindNode(addDay);
        //traversing the list
        for (int i = 1; i <= k - 1; i++) {
            if (curr->next != NULL) {
                curr = curr->next;
            }
        }
        if (k > 0 && !curr) {
            std::cout << "Not found" << std::endl;
        }
        if (k) {
            newNode->next = curr->next;
            curr->next = newNode;
        }
        else {
            //not found, inserting as first
            newNode->next = head;
            head = newNode;
        }
    }

    bool Possible_Covid_19_Infection(List Trajectory, int Day, List Patients[]);
    node* FindNode(int day);
    node* FindNodePrivate(int day, node* ptr);
    int Find_Crowded_Places(int day, int timeInterval, float x4, float y4, int side, int minimumStayDuration, List users[]);
    void Repair(int day, List& Trajectory);
    void Summarize_Trajectory(int day, int daysBefore, List Trajectory);
    void CircularMovement(int chanceMove, int chanceAngle, int speed, float& a, float& b);
    int getPosition(int day, int hour, int minute, List Trajectory);
    void PrintDay(int day, List Trajectory);
    bool Possible_Covid_EndOfDay_NextDay(node* ptr, int Day, List Patient, int &timeStayed);
};

int List::Find_Crowded_Places(int day, int timeInterval, float x0, float y0, int side, int minimumStayDuration, List users[]) {
    float y1 = y0 + side;
    float x1 = x0 + side;
    int usersStayed = 0;
    int startTime = users[0].FindNode(day)->hour;
    for (int i = 0; i < 6; i++) {
        node* ptr = users[i].FindNode(day);
        int timeStayed = 0;
        while (ptr != NULL && ptr->day == day && ptr->hour - startTime <= timeInterval) {
            if (ptr->posX >= x0 && ptr->posX <= x1 && ptr->posY >= y0 && ptr->posY <= y1) {
                timeStayed += 1;
            } ptr = ptr->next;
            if (timeStayed >= minimumStayDuration) {
                usersStayed += 1;
            }
        }
    }
    std::cout << "Users inside the square: " << usersStayed << std::endl;
    return usersStayed;
}

bool List::Possible_Covid_19_Infection(List Trajectory, int Day, List Patients[]) {
    for (int i = 0; i < 3; i++) {
        //std::cout << "In for user " << i <<std::endl;
        int timeStayed = 0;
        node* ptr = Trajectory.FindNode(Day);
        //std::cout << ptr->day << std::endl;
        node* ptr2 = Patients[i].FindNode(Day);
        while (ptr != NULL && ptr->day == Day) {
            //std::cout << "First While" << std::endl;
            float x1 = ptr->posX;
            float y1 = ptr->posY;
            node* ptr2 = Patients[i].FindNode(Day);
            while (ptr2 != NULL && ptr2->day == Day) {
                //std::cout << "Second while" << std::endl;
                //std::cout << "Hour: " << ptr2->hour << std::endl;
                float x2 = ptr2->posX;
                float y2 = ptr2->posY;
                double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
                //std::cout << "Distance is: " << distance << std::endl;
                if (distance < 100 && abs(ptr2->hour - ptr->hour) < 3) {
                    timeStayed += 1;
                }
                //std::cout << "Time stayed is: " << timeStayed << std::endl;
                //std::cout << "minute is: " << ptr2->min << std::endl;
                if (timeStayed >= 50) {
                    return true;
                }
                ptr2 = ptr2->next;
            }
            if (ptr->hour == 23 || ptr->hour == 24 && timeStayed >= 25) {
                bool statement = Possible_Covid_EndOfDay_NextDay(ptr, Day, Patients[i], timeStayed);
                if (statement) {
                    return true;
                }
            }
           ptr = ptr->next;
        }

    }
    return false;
}

bool List::Possible_Covid_EndOfDay_NextDay(node* ptr, int Day, List Patient, int &timeStayed) {
    node* ptr2 = Patient.FindNode(Day + 1);
    while (ptr != NULL && ptr->day == Day) {
        float x1 = ptr->posX;
        float y1 = ptr->posY;
        while (ptr2 != NULL && ptr->day == (Day + 1) && ptr->hour <= 2) {
            float x2 = ptr->posX;
            float y2 = ptr->posY;
            double distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
            if (distance < 100 && abs(ptr2->hour - ptr->hour) == 21 || abs(ptr2->hour - ptr->hour) == 23 || abs(ptr2->hour - ptr->hour) == 24); {
                timeStayed += 1;
            }
            if (timeStayed >= 50) {
                return true;
            }
            ptr2 = ptr2->next;

        }
        ptr = ptr->next;
    }
    return false;
    
}

List::node* List::FindNode(int day) {
    return FindNodePrivate(day, head);
}

List::node* List::FindNodePrivate(int day, node* ptr) {
    curr = head;
    if (day == 1) {
        return curr;
    }
    else {
        while (curr->next != NULL) {
            curr = curr->next;
            if (curr->day == day) {
                return curr;
            }
        }
    }
}

void List::Repair(int day, List& Trajectory) {
    node* ptr = Trajectory.FindNode(day);
    //node* ptrnext = ptr->next;
    while (ptr->next != NULL && ptr->day == day) {
        int position = 0;
        if (((ptr->next->min) - (ptr->min)) > 1) { //if there is a gap between two consecutive node's minutes
            float x1 = ptr->posX; //keeping current and next node coordinates to calculate speed for the movement function (speed in meters/minute)
            float y1 = ptr->posY;
            float x2 = ptr->next->posX;
            float y2 = ptr->next->posY;
            float speed = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); //speed is ds/dt but dt=1 because we check consecutive minutes
            int chanceMove = rand() % 2;
            int chanceAngle = rand() % 361;
            Trajectory.CircularMovement(chanceMove, chanceAngle, speed, x1, y1);
            //std::cout << "Day " << ptr->day << " Hour " << ptr->hour << " Minute " << ptr->min << std::endl;
            position = Trajectory.getPosition(ptr->day, ptr->hour, ptr->min, Trajectory);
            //std::cout << "Adding to "<< position << std::endl;
            AddToPosition(position, ptr->day, ptr->hour, (ptr->min) + 1, x1, y1, ptr->virus, Trajectory); //adding the new node
        }
        ptr = ptr->next;
        //ptrnext = ptrnext->next;
    }
}

void List::Summarize_Trajectory(int day, int daysBefore, List Trajectory) {
    node* sumptr = Trajectory.FindNode(day - daysBefore); //new pointer to the start of the day we are summarizing
    while (sumptr->next->day == (day - daysBefore) && sumptr->next != NULL && sumptr->next->next != NULL) { //while sumptr points to the same day and doesnt point to null
        float x1 = sumptr->posX;
        float y1 = sumptr->posY;
        float x2 = sumptr->next->posX;
        float y2 = sumptr->next->posY;
        float distance = sqrt(((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))); //distance formula
        if (distance < 10) { //if distance is smaller than value
            Trajectory.DeleteNode(sumptr->next->day, sumptr->next->hour, sumptr->next->min, Trajectory); //delete the node we are checking
        }
        else {
            sumptr = sumptr->next; //else main node is next node and repeat process
        }
    }
}

void List::CircularMovement(int chanceMove, int chanceAngle, int speed, float& a, float& b) {
    double f = chanceAngle * 2 * pi / 360; //converting angle from degrees to rad
    if (chanceMove == 1 && a + speed * cos(f) > 0 && a + speed * cos(f) <= 1000 && b + speed * sin(f) > 0 && b + speed * sin(f) <= 1000) { //if our moving chance variable is 1 and our new coordinates are within our borders, keep the new coordinates
        a += speed * cos(f); //explained in the pdf
        b += speed * sin(f); //explained in the pdf
    }
}

int List::getPosition(int day, int hour, int minute, List Trajectory) {
    node* ptr = Trajectory.FindNode(day);
    int position = 1;
    while (ptr->next != NULL) {
        if (ptr->day == day && ptr->hour == hour && ptr->min == minute) {
            return position;
        }
        else {
            position += 1;
            ptr = ptr->next;
        }
    }
}

void List::PrintDay(int day, List Trajectory) {
    curr = Trajectory.FindNode(day);
    while (curr != NULL && curr->day == day) {
        std::cout << "Day: " << curr->day << " Hour: " << curr->hour << " Minute: " << curr->min << " X: " << curr->posX << " Y: " << curr->posY << std::endl;
        curr = curr->next;
    }
}

int main() {
    std::cout << "Test" << std::endl;
    List test1;
    List test2;
    List test3;
    List test4;
    List test5;
    List test6;
    List users[] = { test1, test2, test3, test4, test5, test6 };
    float XY[][2] = { {130,830}, {150,250}, {50,20}, {999,80}, {900,580}, {140,400} };
    bool inf[6] = { true, true, true, false, false ,false };
    test1.AddNode(1, 1, 1, XY[0][0], XY[0][1], true); //adding the first node of each user
    test2.AddNode(1, 1, 1, XY[1][0], XY[1][1], true);
    test3.AddNode(1, 1, 1, XY[2][0], XY[2][1], true);
    test4.AddNode(1, 1, 1, XY[3][0], XY[3][1], false);
    test5.AddNode(1, 1, 1, XY[4][0], XY[4][1], false);
    test6.AddNode(1, 1, 1, XY[5][0], XY[5][1], false);
    List Patients[] = { test1, test2, test3 };
    List Healthy[] = { test4,test5,test6 };
    std::cout << "6 users added, 3 healthy and 3 infected." << std::endl;
    int chanceMove;
    int chanceAngle;
    int speed;
    int breakMinute;
    int breakTime;
    bool status;
    srand((unsigned)time(0));
    for (int d = 1; d <= 7; d++) { //day counter
        for (int h = 1; h <= 24; h++) { //hour counter
            breakMinute = rand() % 35 + 1;
            breakTime = rand() % 25;
            //std::cout << "In day " << d << ", hour " << h << " the GPS was broken between minutes " << breakMinute << " and " << (breakMinute+breakTime) << std::endl;
            for (int m = 1; m <= 60; m++) { //minute counter
                for (int u = 0; u < 6; u++) { //user counter
                    chanceMove = rand() % 2; //if this equals 0, user doesnt move (see CircularMovement()) else user moves
                    chanceAngle = rand() % 361; //angle determines which way the user goes
                    speed = rand() % 7; //determines the speed of the user
                    if (speed < 3) {
                        speed += 3; //we want speeds between 3 and 6 so we add 3 if speed is 0,1 or 2
                    }
                    if (m < breakMinute || m >(breakMinute + breakTime)) { //we dont add nodes inside the time period with the broken gps system
                        users[u].CircularMovement(chanceMove, chanceAngle, speed, XY[u][0], XY[u][1]); //calling our movement function
                        users[u].AddNode(d, h, m, XY[u][0], XY[u][1], inf[u]); //adding the node with the new coordinates
                    }
                }
            }
        }
        for (int u = 0; u < 6; u++) { //users loop
            users[u].Repair(d, users[u]); //repairing the trajectory of every user
        }
        for (int i = 3; i < 6; i++) {
            status = users[i].Possible_Covid_19_Infection(users[i], d, users);
            std::cout << "Status is: " << status << std::endl;
            if (status) {
                std::cout << "True for user " << i << " in day " << d << std::endl;
            }
            else {
                std::cout << "False for user " << i << " in day " << d << std::endl;
            }
        }
        /*
        for(int h=0;h<3;h++) {
            status = users[h+3].Possible_Covid_19_Infection(Healthy[h],d,users); //checking for possible infection of healthy users
            if(status) {
                std::cout << "Healthy user " << h << " has possibly been infected in day " << d << std::endl;
                //break;
            } else {
                std::cout << "No possible infection for healthy user "<< h << " in day " << d << std::endl;
            }
        }
         */
         /*
         if(d-1>0) {
             for(int u=0;u<6;u++) {
                 users[u].Summarize_Trajectory(d,1,users[u]);
             }
         }
          */
          //users[0].Find_Crowded_Places(d,24,25,25,10,45,users);
    }
}
    
