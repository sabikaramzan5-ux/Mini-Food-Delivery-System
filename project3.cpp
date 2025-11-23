#include<iostream>
#include<vector>
#include<string>
using namespace std;
class Order;
class Restaurant;
class RestaurantOwner;
class MenuItem;

class User{
    protected:
    int id;
    string name;
    string phoneNumber;
    public:
    User():id(0),name(""),phoneNumber("") {}
    User(int id_, string nm, string phone):id(id_),name(nm),phoneNumber(phone){}
    void print(){
        cout<<"Name: "<<name<<" id: "<<id<<endl;
    }
    int getuserid(){ return id;}
    string getusername() { return name;}
    void setuserid(int id_){ id=id_;}
    void setusername(string nm) {name=nm;}

};
class Customer: public User{
private:
    string deliveryAddress;
    int walletBalance;
    vector<Order*>orders;
public:
    Customer():User(),deliveryAddress(""),walletBalance(0){}
    Customer(int r,string nm,string p,string deliveryAdd,int wallet):User(r,nm,p),deliveryAddress(deliveryAdd),walletBalance(wallet){}
    void addToWallet(int amt){
        walletBalance+=amt;
    }
    void deductFromWallet(int amt){
        walletBalance-=amt;
    }
    string getdeliveryAddress(){ return deliveryAddress;}
    int getWalletBalance(){ return walletBalance;}
    void placeOrder(Order* o){
        orders.push_back(o);
    }
    void print(){
    User::print();
    cout<<"DeliveryAddress: "<<deliveryAddress<<"  WalletBalance: "<<walletBalance<<endl;
    }
};
class RestaurantOwner : public User{
private:
  
   string businessName;
   int taxNumber;
   vector<Restaurant*>managedRestaurants;
public:
   RestaurantOwner():User(),businessName(""),taxNumber(0){}
   RestaurantOwner(int r,string nm,string phone,string bname,int taxno):User(r,nm,phone),businessName(bname),taxNumber(taxno){}
   string getBusinessname() { return businessName;}
   vector<Restaurant*>& getmanagedRestaurants() { return managedRestaurants;}
   void print(){
    User::print();
    cout<<" BusinessName: "<<businessName<<" TaxNumber: "<<taxNumber<<endl;
   }
};
class Courier:public User{
    private:
   
    string vehicleType;
    string AreaAssigned;
    vector<Order*>orders;
    public:
    Courier():User(),vehicleType(""),AreaAssigned(""){}
    Courier(int r,string nm,string phone,string vehicle,string area):User(r,nm,phone),vehicleType(vehicle),AreaAssigned(area){}
    void print(){
        User::print();
        cout<<" Area assigned: "<<AreaAssigned<<" VehicleType: "<<vehicleType<<endl;

    }
    vector<Order*>& getOrders(){
       return orders;
    }
    string getVehicleType(){ return vehicleType;}
    string getAreaAssigned(){ return AreaAssigned;}

};

class MenuItem{
    private:
    string itemName;
    int price;
    public:
    MenuItem(string iname,int prc):itemName(iname),price(prc){}
    string getItemName(){ return itemName;}
    int getPrice(){ return price;}
    void print(){
        cout<<"Item Name: "<<itemName<<"  Price: "<<price<<endl;
    }
};
class Restaurant{
    private:
    string resName;
    string location;
    RestaurantOwner* owner;
    vector<MenuItem*>menuitems;
    public:
    Restaurant(string rnm,string loc,RestaurantOwner* own):resName(rnm),location(loc),owner(own){
        owner->getmanagedRestaurants().push_back(this);
    }
    void print(){
        cout<<"Restaurant Name: "<<resName<<" Location: "<<location<<" Restaurant Owner: "<<owner->getusername()<<endl;
        cout<<"MenuItems: "<<endl;
        for(int i=0;i<menuitems.size();i++){
            menuitems[i]->print();
        }
    }
    string getName(){
        return resName;
    }
    void addMenuitem(MenuItem* m){
        menuitems.push_back(m);
    }
    vector<MenuItem*>& getmenuitems(){ 
    return menuitems;
	}
    ~Restaurant(){
        for(int i=0;i<menuitems.size();i++){
            delete menuitems[i];
        }
    }
};
class PaymentMethod{
    public:
    PaymentMethod(){}
    virtual ~PaymentMethod()=default;
    virtual bool Pay(int amt)=0;
};
class WalletPayment : public PaymentMethod {
private:
    Customer* customer;
public:
    WalletPayment(Customer* c) : customer(c) {}
    bool Pay(int amt) override {
        if(customer->getWalletBalance() >= amt) {
            customer->deductFromWallet(amt);
            cout << "Wallet payment successful!\n";
            return true;
        }
        cout << "Insufficient wallet balance!\n";
        return false;
    }
};
class CardPayment : public PaymentMethod {
public:
    bool Pay(int amt) override {
        cout<<"Card payment approved!\n";
        return true;
    }
};
class CashOnDelivery : public PaymentMethod {
public:
    bool Pay(int amt) override {
        cout<<"Payment pending (Cash on Delivery)\n";
        return true;
    }
};
class OrderItem{
    private:
    MenuItem* item;
    int quantity;
    public:
    OrderItem(MenuItem* m,int qty):item(m),quantity(qty){}
    int calculatePrice(){ return item->getPrice() * quantity;}
    void print(){
        item->print();
        cout<<" Quantity: "<<quantity<<endl;
    }
};
enum OrderStatus{Created,Confirmed,Preparing,OutForDelivery,Delivered,Cancelled};
class Order{
private:
   PaymentMethod* payment;
   Customer* c;
   Courier* cur;
   vector<OrderItem*>orderitems;
   int orderid;
   OrderStatus status;
public:
    Order(int oid,Customer* cus):orderid(oid),c(cus),payment(nullptr),cur(nullptr){
        status=Created;
        c->placeOrder(this);
    }
    void nextStatus(){
        switch(status){
            case Created: status= Confirmed; break;
            case Confirmed: status = Preparing; break;
            case Preparing: status = OutForDelivery; break;
            case OutForDelivery: status= Delivered; break;

        }
    }
    string getStatus(){
        switch(status){
            case Created: return "Created"; break;
            case Confirmed: return "Confirmed"; break;
            case Preparing: return "Preparing"; break;
            case OutForDelivery: return "OutforDelivery"; break;
            case Delivered: return "Delivered"; break;
        }
        return "Unknown";
    }
    void setPaymentMethod(PaymentMethod* p){
    	payment = p;
	}
    void cancelOrder(){
        if(status<OutForDelivery){
            status = Cancelled;
            return;
        }
        cout<<"Cannot cancel order"<<endl;

    }
    int totalPrice(){
        int total=0;
        for(int i=0;i<orderitems.size();i++){
            total+=orderitems[i]->calculatePrice();
        }
        return total;
    }
    void additem(OrderItem* o){
        orderitems.push_back(o);
    }
    void assignCurior(Courier* c){
        cur=c;
        cur->getOrders().push_back(this);
    }
    
    void printOrderReceipt(){
        cout<<"Customer: "<<c->getusername()<<" placed order"<<endl;
        cout<<"Courier: "<<cur->getusername()<<" has assigned this order in area "<<cur->getAreaAssigned()<<" having vehicle "<<cur->getVehicleType()<<endl;
        cout<<"Order items include: "<<endl;
        for(int i=0;i<orderitems.size();i++){
            orderitems[i]->print();
        }
        cout<<"Total Price: "<<totalPrice()<<endl;
        cout<<"Order status: "<<getStatus()<<endl;
        bool succeed=payment->Pay(totalPrice());
        cout<<"Payment Method: "<<(succeed ? "Succeed" : "Failed")<<endl;
    }
    ~Order(){
    for(auto x : orderitems) 
	delete x;
    }
};
int main(){
    Customer c1(513,"Sabika","1234","Lahore",1500);
    RestaurantOwner owner(501,"Abdullah","2314","Cakes and Bakes",510);
    Courier cr(502,"Ali","3214","MotorCycle","JoharTown");
    Restaurant res1("Cakes and Bakes","Johar Town",&owner);
    Restaurant res2("Bundu Khan","Model Town",&owner);
    
   
    res1.addMenuitem(new MenuItem("Sandwich",120));
    res1.addMenuitem(new MenuItem("Brownie",220));
    res1.addMenuitem(new MenuItem("Cup cake",100));
   
    res2.addMenuitem(new MenuItem("Chicken karai",500));
    res2.addMenuitem(new MenuItem("Beef Pulao",330));
    res2.addMenuitem(new MenuItem("Mutton Roast",400));
   
    Order o1(1,&c1);
    o1.additem(new OrderItem(res1.getmenuitems()[0],2));
    o1.additem(new OrderItem(res1.getmenuitems()[1],2));
    o1.assignCurior(&cr);
    o1.setPaymentMethod(new WalletPayment(&c1));
    o1.printOrderReceipt();
    return 0;
}