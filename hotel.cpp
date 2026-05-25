#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <fstream>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>
using namespace std;

int CID = 1000;

class Customer {
public:
    int id;
    string name, gender, phone, native_place;
    bool booked;
    int room_no;
    string room_type;
    bool parking;
    string vehicle_type;
    int parking_id;
    string check_in, check_out;
    int rating;
    bool paid;
    Customer* next;
    vector<pair<string,int>> foodOrders;
    Customer(string n = "", string g = "", string p = "", string nat = "") {
        id = ++CID;
        name = n; gender = g; phone = p; native_place = nat;
        booked = false; room_no = -1;
        room_type = ""; parking = false; vehicle_type = "";
        parking_id = -1; rating = 0; paid = false;
        check_in = check_out = "";
        next = NULL;
    }
    void display() {
        cout << "\n-----------------------------";
        cout << "\nID: " << id;
        cout << "\nName: " << name;
        cout << "\nGender: " << gender;
        cout << "\nPhone: " << phone;
        cout << "\nNative: " << native_place;
        if (booked)
            cout << "\nRoom No: " << room_no << " (" << room_type << ")";
        else
            cout << "\nRoom: Not booked";
        if (parking)
            cout << "\nParking: " << vehicle_type << " at Slot " << parking_id;
        else
            cout << "\nParking: None";
        if (rating)
            cout << "\nRating: " << rating << "/5";
        if (!foodOrders.empty()) {
            cout << "\nFood Orders:\n";
            for (auto &f : foodOrders)
                cout << "  " << f.first << " x" << f.second << "\n";
        }
        cout << "\nPayment Status: " << (paid ? "Paid" : "Unpaid");
        cout << "\n-----------------------------\n";
    }
    void showFoodOrders() {
        if (foodOrders.empty()) {
            cout << "No food ordered.\n";
            return;
        }
        cout << "--- Food Orders ---\n";
        for (auto &f : foodOrders)
            cout << f.first << " x" << f.second << "\n";
    }
};

class Room {
public:
    int no;
    string type;
    bool booked;
    string customer;
    int fee;
    Room(int n = 0, string t = "Single", int f = 1000) {
        no = n; type = t; fee = f; booked = false; customer = "";
    }
};

class Parking {
public:
    vector<int> twoWheelerSlots;
    vector<int> fourWheelerSlots;
    queue<int> twoQueue, fourQueue;
    Parking(int two = 15, int four = 15) {
        twoWheelerSlots.assign(two + 1, 0);
        fourWheelerSlots.assign(four + 1, 0);
    }
    int assign(string type, int cid) {
        if (type == "2") {
            for (int i = 1; i < (int)twoWheelerSlots.size(); i++) {
                if (twoWheelerSlots[i] == 0) { twoWheelerSlots[i] = cid; return i; }
            }
            twoQueue.push(cid);
            return -1;
        } else {
            for (int i = 1; i < (int)fourWheelerSlots.size(); i++) {
                if (fourWheelerSlots[i] == 0) { fourWheelerSlots[i] = cid; return i; }
            }
            fourQueue.push(cid);
            return -1;
        }
    }
    void release(int cid) {
        for (int i = 1; i < (int)twoWheelerSlots.size(); i++) {
            if (twoWheelerSlots[i] == cid) {
                twoWheelerSlots[i] = 0;
                if (!twoQueue.empty()) {
                    twoWheelerSlots[i] = twoQueue.front();
                    twoQueue.pop();
                }
                return;
            }
        }
        for (int i = 1; i < (int)fourWheelerSlots.size(); i++) {
            if (fourWheelerSlots[i] == cid) {
                fourWheelerSlots[i] = 0;
                if (!fourQueue.empty()) {
                    fourWheelerSlots[i] = fourQueue.front();
                    fourQueue.pop();
                }
                return;
            }
        }
    }
    void display() {
        cout << "\n--- Parking Status ---\n";
        cout << "2-Wheeler Slots:\n";
        for (int i = 1; i < (int)twoWheelerSlots.size(); i++)
            cout << "Slot " << i << ": " << (twoWheelerSlots[i] ? to_string(twoWheelerSlots[i]) : "Empty") << endl;
        cout << "4-Wheeler Slots:\n";
        for (int i = 1; i < (int)fourWheelerSlots.size(); i++)
            cout << "Slot " << i << ": " << (fourWheelerSlots[i] ? to_string(fourWheelerSlots[i]) : "Empty") << endl;
    }
};

class FoodMenu {
public:
    map<int, pair<string,int>> foodList;
    map<int, pair<string,int>> juiceList;
    FoodMenu() {
        foodList = {
            {1, {"Idly", 50}}, {2, {"Dosa", 60}}, {3, {"Pongal", 70}},
            {4, {"Meals", 120}}, {5, {"Biriyani", 150}}, {6, {"Fried Rice", 130}},
            {7, {"Parotta", 80}}, {8, {"Chapathi", 70}}, {9, {"Poori", 70}},
            {10, {"Noodles", 120}}
        };
        juiceList = {
            {1, {"Orange Juice", 50}}, {2, {"Apple Juice", 60}},
            {3, {"Watermelon Juice", 50}}, {4, {"Lime Juice", 30}},
            {5, {"Mango Shake", 80}}
        };
    }
    void showMenu() {
        cout << "\n--- Food Menu ---\n";
        for (auto &f : foodList)
            cout << "F" << f.first << ". " << f.second.first << " - ₹" << f.second.second << endl;
        cout << "\n--- Juice Menu ---\n";
        for (auto &j : juiceList)
            cout << "J" << j.first << ". " << j.second.first << " - ₹" << j.second.second << endl;
        cout << "(Enter F<number> or J<number> without space, e.g. F1 or J3)\n";
    }
    int getPrice(const string &item) {
        for (auto &f : foodList) if (f.second.first == item) return f.second.second;
        for (auto &j : juiceList) if (j.second.first == item) return j.second.second;
        return 0;
    }
    bool decodeCode(const string &code, string &name, int &price) {
        if (code.size() < 2) return false;
        char kind = code[0];
        int id;
        try { id = stoi(code.substr(1)); } catch (...) { return false; }
        if ((kind == 'F' || kind == 'f') && foodList.count(id)) { name = foodList[id].first; price = foodList[id].second; return true; }
        if ((kind == 'J' || kind == 'j') && juiceList.count(id)) { name = juiceList[id].first; price = juiceList[id].second; return true; }
        return false;
    }
};

struct Operation {
    string type;
    int cust_id;
    Customer backup;
    Operation(string t = "", int id = 0, const Customer &b = Customer()) : type(t), cust_id(id), backup(b) {}
};

class Hotel {
public:
    Customer* head;
    vector<Room> rooms;
    Parking park;
    map<string, string> staff;
    unordered_map<string, int> nameMap;
    set<int> freeRooms;
    priority_queue<pair<int, string>> feedbacks;
    stack<Operation> undo;
    int totalIncome;
    FoodMenu menu;
    vector<int> unpaidIDs;
    Hotel() {
        head = NULL;
        park = Parking(15, 15);
        staff["admin"] = "1234";
        totalIncome = 0;
        for (int i = 1; i <= 400; i++) {
            string t; int fee;
            if (i <= 100) { t = "Single"; fee = 1000; }
            else if (i <= 200) { t = "Double"; fee = 1500; }
            else if (i <= 300) { t = "Family"; fee = 2000; }
            else { t = "VIP"; fee = 3000; }
            rooms.push_back(Room(i, t, fee));
            freeRooms.insert(i);
        }
        addDefaultCustomersBooked();
    }
    void addDefaultCustomersBooked() {
        Customer* c1 = new Customer("Dev","Male","9876543210","Chennai");
        c1->booked = true; c1->room_no = 1; c1->room_type = rooms[0].type; c1->check_in = "2025-11-03";
        rooms[0].booked = true; rooms[0].customer = c1->name; freeRooms.erase(1);
        head = c1; nameMap[c1->name] = c1->id;
        Customer* c2 = new Customer("Aswanth","Male","9876543222","Madurai");
        c2->booked = true; c2->room_no = 102; c2->room_type = rooms[101].type; c2->check_in = "2025-11-03";
        rooms[101].booked = true; rooms[101].customer = c2->name; freeRooms.erase(102);
        head->next = c2; nameMap[c2->name] = c2->id;
        Customer* c3 = new Customer("Mathan","Male","9876543333","Trichy");
        c3->booked = true; c3->room_no = 203; c3->room_type = rooms[202].type; c3->check_in = "2025-11-03";
        rooms[202].booked = true; rooms[202].customer = c3->name; freeRooms.erase(203);
        head->next->next = c3; nameMap[c3->name] = c3->id;
        Customer* c4 = new Customer("Shafeek","Male","9876543444","Coimbatore");
        c4->booked = true; c4->room_no = 305; c4->room_type = rooms[304].type; c4->check_in = "2025-11-03";
        rooms[304].booked = true; rooms[304].customer = c4->name; freeRooms.erase(305);
        head->next->next->next = c4; nameMap[c4->name] = c4->id;
    }
    void addCustomerSimple() {
        string n,g,ph,nat;
        cout<<"Enter Name Gender Phone Native: ";
        cin>>n>>g>>ph>>nat;
        addCustomer(n,g,ph,nat);
        cout<<"Customer added.\n";
    }
    void addCustomer(string n,string g,string p,string nat){
        Customer*c=new Customer(n,g,p,nat);
        if(!head) head=c; else {Customer*t=head;while(t->next)t=t->next;t->next=c;}
        nameMap[n]=c->id;
        cout<<"Customer id "<<c->id<<endl;
    }
    Customer* findById(int id){Customer*t=head;while(t){if(t->id==id)return t;t=t->next;}return NULL;}
    void showRoomTypesAndFees(){
        cout<<"\n--- ROOM TYPES & PRICES ---\nRooms 1-100: Single ₹1000\nRooms 101-200: Double ₹1500\nRooms 201-300: Family ₹2000\nRooms 301-400: VIP ₹3000\n";
    }
    void bookRoom(Customer*c){
        if(!c){cout<<"Customer NULL\n";return;}
        showRoomTypesAndFees();
        int num; cout<<"Enter room number (1-400): ";cin>>num;
        if(num<1||num>400||rooms[num-1].booked){cout<<"Invalid or booked!\n";return;}
        rooms[num-1].booked=true;rooms[num-1].customer=c->name;
        c->booked=true;c->room_no=num;c->room_type=rooms[num-1].type;freeRooms.erase(num);
        cout<<"Enter check-in: ";cin>>c->check_in;
        cout<<"Room booked.\n";
        undo.push(Operation("book",c->id,*c));
    }
    void assignParking(Customer*c){
        if(!c){cout<<"Customer NULL\n";return;}
        string type;cout<<"Enter vehicle type (2 or 4): ";cin>>type;
        int slot=park.assign(type,c->id);
        if(slot!=-1){
            c->parking=true;c->vehicle_type=(type=="2")?"2-wheeler":"4-wheeler";c->parking_id=slot;
            cout<<"Slot "<<slot<<" assigned.\n";undo.push(Operation("parking",c->id,*c));
        }else cout<<"Waitlist.\n";
    }
    void orderFood(Customer* c) {
        if (!c || !c->booked) {
            cout << "Customer not found or room not booked!\n";
            return;
        }
        string code;
        int qty;
        do {
            menu.showMenu();
            cout << "Enter item code (e.g. F1 or J2) or 0 to finish: ";
            cin >> code;
            if (code == "0") break;
            string name; int price;
            if (!menu.decodeCode(code, name, price)) {
                cout << "Invalid code. Use format F<number> or J<number>.\n";
                continue;
            }
            cout << "Enter quantity: ";
            cin >> qty;
            if (qty <= 0) { cout << "Quantity must be positive.\n"; continue; }
            c->foodOrders.push_back({name, qty});
            cout << name << " added x" << qty << " @" << price << " each.\n";
        } while (true);
    }
    void deleteCustomer(int id){
        Customer*t=head,*p=NULL;while(t&&t->id!=id){p=t;t=t->next;}
        if(!t){cout<<"Not found!\n";return;}
        undo.push(Operation("delete",t->id,*t));
        if(t->booked){rooms[t->room_no-1].booked=false;rooms[t->room_no-1].customer="";freeRooms.insert(t->room_no);}
        if(t->parking)park.release(t->id);
        if(p)p->next=t->next;else head=t->next;
        cout<<"Deleted "<<t->name<<"\n";delete t;
    }
    void undoLast(){
        if(undo.empty()){cout<<"Nothing to undo.\n";return;}
        Operation op = undo.top(); undo.pop();
        if(op.type=="delete"){
            Customer*c=new Customer(op.backup.name,op.backup.gender,op.backup.phone,op.backup.native_place);
            *c = op.backup; c->next = head; head = c;
            if(c->booked){rooms[c->room_no-1].booked=true;rooms[c->room_no-1].customer=c->name;freeRooms.erase(c->room_no);}
            if(c->parking && c->parking_id>0){
                if(c->vehicle_type=="2-wheeler") park.twoWheelerSlots[c->parking_id]=c->id;
                else park.fourWheelerSlots[c->parking_id]=c->id;
            }
            cout<<"Undo delete.\n";
        } else if(op.type=="book"){
            Customer*c=findById(op.cust_id);
            if(c && c->booked){rooms[c->room_no-1].booked=false;rooms[c->room_no-1].customer="";freeRooms.insert(c->room_no);c->booked=false;c->room_no=-1;c->room_type="";cout<<"Undo booking.\n";}
        } else if(op.type=="parking"){
            Customer*c=findById(op.cust_id);
            if(c && c->parking){park.release(c->id);c->parking=false;c->parking_id=-1;c->vehicle_type="";cout<<"Undo parking.\n";}
        }
    }
    void generateInvoice(Customer* c) {
        if (!c || !c->booked) {
            cout << "Invalid.\n";
            return;
        }
        int d;
        cout << "Enter days: ";
        cin >> d;
        int base = (c->room_type == "Single" ? 1000 :
                    c->room_type == "Double" ? 1500 :
                    c->room_type == "Family" ? 2000 : 3000);
        int parkfee = (c->parking ? ((c->vehicle_type == "2-wheeler" ? 100 : 200) * d) : 0);
        int foodTotal = 0;
        for (auto &f : c->foodOrders) {
            int price = menu.getPrice(f.first);
            foodTotal += price * f.second;
        }
        int total = base * d + parkfee + foodTotal;
        totalIncome += total;
        cout << "\n---- INVOICE ----\n";
        cout << "Customer ID : " << c->id << "\n";
        cout << "Name        : " << c->name << "\n";
        cout << "Room No     : " << c->room_no << " (" << c->room_type << ")\n";
        cout << "Check-in    : " << c->check_in << "\n";
        cout << "Days Stayed : " << d << "\n";
        cout << "Room Charge : ₹" << base * d << "\n";
        cout << "Parking Fee : ₹" << parkfee << "\n";
        cout << "Food Charges: ₹" << foodTotal << "\n";
        if (!c->foodOrders.empty()) {
            cout << "  Items:\n";
            for (auto &f : c->foodOrders) {
                cout << "   " << f.first << " x" << f.second << " = ₹" << menu.getPrice(f.first) * f.second << "\n";
            }
        }
        cout << "Total Bill  : ₹" << total << "\n";
        cout << "--------------------------\n";
        ofstream fout("invoices.txt", ios::app);
        if (!fout.is_open()) {
            cout << "❌ Error: Could not open invoices.txt for writing!\n";
        } else {
            fout << "==== DSP Residency Invoice ====\n";
            fout << "Customer ID : " << c->id << "\n";
            fout << "Name        : " << c->name << "\n";
            fout << "Room No     : " << c->room_no << " (" << c->room_type << ")\n";
            fout << "Check-in    : " << c->check_in << "\n";
            fout << "Check-out   : " << c->check_out << "\n";
            fout << "Days Stayed : " << d << "\n";
            fout << "Room Charge : ₹" << base * d << "\n";
            fout << "Parking Fee : ₹" << parkfee << "\n";
            fout << "Food Charges: ₹" << foodTotal << "\n";
            if (!c->foodOrders.empty()) {
                fout << "  Items:\n";
                for (auto &f : c->foodOrders) {
                    fout << "   " << f.first << " x" << f.second << " = ₹" << menu.getPrice(f.first) * f.second << "\n";
                }
            }
            fout << "Total Bill  : ₹" << total << "\n";
            fout << "--------------------------------\n\n";
            fout.close();
            cout << "Invoice saved successfully to invoices.txt\n";
        }
        cout << "Customer invoice generated.\nDid the customer pay? (yes/no): ";
        string ans; cin >> ans;
        if(ans=="yes"||ans=="Yes"||ans=="y"||ans=="Y"){
            c->paid=true;
            deleteCustomer(c->id);
            auto it=find(unpaidIDs.begin(),unpaidIDs.end(),c->id);
            if(it!=unpaidIDs.end()) unpaidIDs.erase(it);
            cout<<"Customer paid and deleted.\n";
        } else {
            c->paid=false;
            if(find(unpaidIDs.begin(),unpaidIDs.end(),c->id)==unpaidIDs.end()) unpaidIDs.push_back(c->id);
            cout<<"Added to unpaid list.\n";
        }
    }
    void showAll(){Customer*t=head;if(!t){cout<<"No customers.\n";return;}while(t){t->display();t=t->next;}}
    void feedback(Customer*c){if(!c){cout<<"NULL\n";return;}cout<<"Enter rating(1-5): ";cin>>c->rating;feedbacks.push({c->rating,c->name});}
    void showFeedbacks(){priority_queue<pair<int,string>>tmp=feedbacks;if(tmp.empty()){cout<<"No feedbacks.\n";return;}while(!tmp.empty()){cout<<tmp.top().second<<" rated "<<tmp.top().first<<"/5\n";tmp.pop();}}
    void showFreeRooms(){cout<<"Free rooms count:"<<freeRooms.size()<<"\n";}
    void showParking(){park.display();}
    void showIncome(){cout<<"Total Income: ₹"<<totalIncome<<"\n";}
    void showUnpaid(){ if(unpaidIDs.empty()){cout<<"No unpaid customers.\n";return;} for(int id:unpaidIDs){Customer*c=findById(id); if(c) c->display();} }
    void markPaidAndDelete(){ if(unpaidIDs.empty()){cout<<"No unpaid customers.\n";return;} int id; cout<<"Enter unpaid customer ID to mark paid and delete: "; cin>>id; auto it=find(unpaidIDs.begin(),unpaidIDs.end(),id); if(it==unpaidIDs.end()){cout<<"ID not in unpaid list.\n"; return;} Customer*c=findById(id); if(c){c->paid=true; deleteCustomer(id);} unpaidIDs.erase(it); cout<<"Customer marked paid and deleted.\n";}
    void searchByRoom(){int r;cout<<"Enter room no:";cin>>r;Customer*t=head;bool f=false;while(t){if(t->room_no==r){t->display();f=true;}t=t->next;} if(!f) cout<<"Not found\n";}
    void searchById(){int id;cout<<"Enter customer id:";cin>>id;Customer*t=findById(id);if(t)t->display();else cout<<"Not found\n";}
    void searchByParking(){int pid;cout<<"Enter parking id:";cin>>pid;Customer*t=head;bool f=false;while(t){if(t->parking_id==pid){t->display();f=true;}t=t->next;} if(!f) cout<<"Not found\n";}
    void searchByRating(){int r;cout<<"Enter rating:";cin>>r;Customer*t=head;bool f=false;while(t){if(t->rating==r){t->display();f=true;}t=t->next;} if(!f) cout<<"No customers with rating "<<r<<"\n";}
};

int main(){
    Hotel h;
    cout<<"=== DSP Residency ===\nUsername:";string u,p;cin>>u;
    cout<<"Password:";cin>>p;
    if(h.staff[u]!=p){cout<<"Login failed\n";return 0;}
    int ch;
    do{
        cout<<"\n1.ShowAll 2.Add 3.Book 4.Park 5.Delete 6.Invoice 7.Feedback 8.ShowFB 9.Undo 10.Parking 11.FreeRooms 12.Income 13.OrderFood 14.Exit 15.Search/Unpaid\nChoice:";
        cin>>ch;
        if(ch==1)h.showAll();
        else if(ch==2)h.addCustomerSimple();
        else if(ch==3){int id;cout<<"Enter ID:";cin>>id;Customer*c=h.findById(id);if(c)h.bookRoom(c);else cout<<"Not found\n";}
        else if(ch==4){int id;cout<<"Enter ID:";cin>>id;Customer*c=h.findById(id);if(c)h.assignParking(c);else cout<<"Not found\n";}
        else if(ch==5){int id;cout<<"Enter ID:";cin>>id;h.deleteCustomer(id);}
        else if(ch==6){int id;cout<<"Enter ID:";cin>>id;Customer*c=h.findById(id);if(c)h.generateInvoice(c);else cout<<"Not found\n";}
        else if(ch==7){int id;cout<<"Enter ID:";cin>>id;Customer*c=h.findById(id);if(c)h.feedback(c);else cout<<"Not found\n";}
        else if(ch==8)h.showFeedbacks();
        else if(ch==9)h.undoLast();
        else if(ch==10)h.showParking();
        else if(ch==11)h.showFreeRooms();
        else if(ch==12)h.showIncome();
        else if(ch==13){int id; cout<<"Enter ID:"; cin>>id; Customer* c = h.findById(id); if(c) h.orderFood(c); else cout<<"Not found\n";}
        else if(ch==15){
            cout<<"\n1.Search by Room 2.By ID 3.By Parking 4.By Rating 5.Show Unpaid 6.Mark Unpaid as Pa