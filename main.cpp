#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//I was tryin to use set precision to make it so then th efloat value would not print out a million numbers so I just went with a string and just tell the use to enter a float value with only two decimals

using namespace std; 

class Item{
  
public:
  string name;
  string price;
  int itemNumber;
  int stock;
  
  Item(string Name, string Price, int Num, int Quan): name(Name), price(Price), itemNumber(Num), stock(Quan){}

  string itemString(){
    return to_string(itemNumber) + "     " + name + "    " + price + "        " + to_string(stock) + "\n"; 
  }

};
//The item class is an object that was created to store the data of each item. It will store the data that we read off of the files when we are loading the file into the beginning of the program, and it will also be where we will input information that will then be stored and outputted onto the text files. This way we can keep track of all of the items within a vector making it easy to iterate through them and find the one we are looking for.




class System{
  public:
    vector<Item> items;
    vector<Item> trashBin;
    
    string filename = "inventory.txt";
    string trashBucket = "trashBucket.txt";
    
    System(){}
    ~System(){}
    //Within the contents of the class we have 2 different vectors, one to hold the items for the inventory.txt file and one to hold the items for the trashBucket.txt file. When i finished the first part of this program I decided that I also wanted to add a trash file, being a person who makes mistakes very often I feel that it is important to account for a person making a mistake. This way if someone deletes something that they shouldnt have deleted they can go into the trashBucket file and retrive it using the menu commands. The hardest part of all of this was making sure that when the item was deleted and transfered from the items vector to the trashBin vector that it would not lag out and do something unexpected. At least 30 times when I was running it an item would somehow appear within the trash file even though I did not ask the program to transfer it. 

    void loadInventoryandTrash(){
      
      ifstream inf(filename);
      
      string name;
      string price;
      int stock;
      string line;
      int itemNumber;
      
      while(getline(inf, line)){
       
        stringstream stream(line);
        
        stream >> itemNumber >> name >> price >> stock;
       
        Item temp(name, price, itemNumber, stock);
        
        items.push_back(temp);
        
      }
      inf.close();
      
      ifstream trash(trashBucket);
      
      string line1;
      while(getline(trash, line1)){

          stringstream stream(line1);

          stream >> itemNumber >> name >> price >> stock;

          Item temp(name, price, itemNumber, stock);

          trashBin.push_back(temp);

      }
    }



    void updateInventory(){
      
      ofstream out(filename);
      
      for(int i = 0; i < items.size(); i++){
        
        out << items[i].itemString(); 
      }
      out.close();
    }
  //I created this function so while the user is using the program and making changes to it, it will update after every decision. You will find this function being called throughout every function that specifically changes teh Inventory file, I was going to try and just add on to this file to also update the trash file but it seemed unneeded since most of the time they need seperate updates. Within the program we open an ofstream file, which is out inventory file, we then create a for loop that will traverse through through the items vector and then print the contents of that vector onto the inventory file.



    void updateTrash(){
      ofstream out(trashBucket);

      for(int i = 0; i < trashBin.size(); i++){

        out << trashBin[i].itemString(); 
      }
      out.close();
    }
  //This is an exact copy of the updateInventory function except it is used to update the Trash file instead.



    void showTrash(){
      for(int i = 0; i < trashBin.size(); i++){
        cout << "\nNumber:   Name:    Price:    Quantity:" << endl;
        cout << trashBin[i].itemString();
      }
    }

    //This program will traverse through the trashBin vector and print out the contents onto the console for the user to see, this can be helpful incase they are looking for an item that they mistakenly deleted, or if they hope to clear the entire trashcan file



    void emptryTrash(){
      trashBin.clear();
      updateTrash();
    }
  //I tried to create a loop that would iterate through an array and then delete the contents within the trashBIN, but turns out vectors have a command that will clear all contents inside of them. So we use .clear() to delete all contents within the trashBin vector and then update the trashBucket file to now be empty. No more trace of those items anymore



    void getFromTrash(int itemNumber){
      int c = 0;
      for(int i = 0; i < items.size(); i++){
        if(trashBin[i].itemNumber == itemNumber){
          items.push_back(trashBin[i]);
          trashBin.erase(trashBin.begin() + i);
          updateTrash();
          updateInventory();
          c = 1;
          break;
        }
      }
      if(c == 0){
        cout << "Could not find item" << endl;
      }
    }
    //As stated earlier this program was created as a fail safe incase someone accidentally deleted an item that was not supposed to be deleted. okay so teh parameter for this program is the itemNumber, the show trash function will show the contents, and then you can find the item you are looking for, input the itemNumber into the machine, adn then a loop will begin looking through the trashBin vector to find the item that you are looking for. After we find the item using the if statement, we will then add that item to our inventory vector. After adding it to the inventory we then have to delete it from the vector. I used the .erase() command, we then will set the point of where we will begin iterating which is the beginning of the vector shown by the vectorname.begin() and then add what iteration we are on. This is alot like a for loop suprisingly and will find the index of the item that we would like to delete. After it has been added to the inventory and deleted from the trashBin we then have to update both of the files so then we will now see that the item has moved from the trashBin to the inventory. Sorry that was glaring, the could not find item would have printed with every iteration within the program so I made sure to fix it and put it outside of the for loo, ther we go, i also added a bool variable to also indicate if the item has been found or not. this way it can trigger the if statement on teh console for the user to see. Also it seems that it was not doing what I wanted it to do there, so I fixed it. !exist will run if the statement is false which only happens if the loop was unable to find the item inputted by the user.



    void add(Item &item){
      if(!checkNotTaken(item)){
        items.push_back(item);
        updateInventory();
      }else{
        cout << "Item Number already taken" << endl;
      }
    }
  //So this is a function that will add a new item to the items vector. In the main method you will see that we take inputs for all of the different variables within the item object, we then create the item and then pass it to the add function. I ended up creating another function to check if the itemnumber had been used before, this way someone cannot enter in teh same singature twice which would mess up the entire 
  bool checkNotTaken(Item &item){
    for(int i = 0; i < items.size(); i++ ){
      if(items[i].itemNumber == item.itemNumber){
        return true;
      }
    }

    for(int i = 0; i < trashBin.size(); i++){
      if(trashBin[i].itemNumber == item.itemNumber){
        return true;
      }
    }
    return false;
  }
//This will check to see if the itemNumber that the user has inputted already exists or not. We will traverse through both the items vector and teh trashBin vector and compare the iteNumbers to the passed items Itemnumber value



    void remove(int id){
      int c = 0;
      for(int i = 0; i < items.size(); i++){
        if(items[i].itemNumber == id){
          string name = items[i].name;
          string price = items[i].price;
          int itemNumber = items[i].itemNumber;
          int stock = items[i].stock;
          
          Item temp(name, price,itemNumber,stock);
          items.erase(items.begin() + i);
          trashBin.push_back(temp);
          updateInventory();
          updateTrash();
          
          cout << "Item deleted";
          c = 1;
          break;
        }
      }
     if(c == 0){
       cout << "Item was not found" << endl;
     }
    }
  //This function was created to remove items from the items vector and move them to the trashbin vector, it will then update both of the files to show that the item has moved, it will also check to see if the item number entered even exists, and if not it will return the user back to the main menu.



    void listItems(){
      for(int i = 0; i < items.size(); i++){
        cout << "\nNumber:   Name:    Price:    Quantity:" << endl;
        cout << items[i].itemString();
        
      }
    }
  //Thi function will list the Items within the inventory text file by traversing through the string and then call the itemString() funciton in the item object that will then output the item in a nice and clean way.
  
  
    void find(int id){
      int c = 0;
      for(int i = 0; i < items.size(); i++){
        if(items[i].itemNumber == id){
          cout << items[i].itemString();
          c = 1;
          break;
        }  
      
      }
      if(c == 0){
        cout << "No item was found" << endl;
      }
    
    }
  //This function allows the user to find and then print out the details of an item. I made the look up process using the itemNumber. I need to add a condition that will print out that the item does not exist so then the user will know

    void editItem(int id){
      int c = 0;
      for(int i = 0; i < items.size(); i++){
        if(items[i].itemNumber == id){
          int choice;
          string temp;
          int temp2;
          float temp3;
          do{cout << "Enter a number to chose what to do" << endl;
            cout << "1. change ItemName" << endl;
            cout << "2. change ItemPrice" << endl;
            cout << "3. change ItemQuantity" << endl;
            cout << "4. Exit and Return to main menu" << endl;
             cin >> choice;
             if(choice == 1){
               cout << "\nPlease enter what you would like to rename the item: ";
               cin >> temp;
               items[i].name = temp;
               updateInventory();
                
             }else if(choice == 2){
               cout << "\nPlease enter what you would like to rename the item: ";
                cin >> temp3;
                items[i].price = temp3;
                updateInventory();

             }else if(choice == 3){
              cout << "\nPlease enter what you would like to rename the item: ";
              cin >> temp2;
              items[i].stock = temp2;
              updateInventory();
               
             }else if(choice == 4){
               cout << "exiting change menu" << endl;
             }else{
               cout << "invalid input" << endl;
             }
            }while(choice != 4);
          c = 1;
          break;
        }
      }
      if(c == 0){
        cout << "Could not find item" << endl;
      }
    }
  //I also wanted to make a function that would allow the user to chaneg aspects of a item, this is incase the price drops, it is renamed, or the quantity has decreased. So this function will traverse through the items vector until it finds a match with the enetered itemNumber.WE use a similiar loop to see if the itemNumber does exist within the program. After this we make a do while loop alot like the one within main that will stop once the user inputs 4. I also made sure that the user could not change the itemNumber, it is important that they do not change it as it is the identifier for the item. We can simply use the for loop i to then find the index of the vector we would like to be in and then we can change it, and continue the loop in case there is anything else they would like to change.
};

int main(){
  System system;
  system.loadInventoryandTrash();
  //We create an object and load the files into the program. If they dont exist this funciton will create them, if they already exist and have content it will read the content and create items that will be stored into one of two vectors
  int choice;
  int itemNumber;
  string name;
  string price;
  int stock;
  //These are different variables that had to be declared outside of our while loop, this way we can use them in multiple different conditions without having to create new variables 
  cout <<"This program is a system that will organize items within a store based on attributes given to them. When an item is deleted it is moved to the trash bin, this is incase the worker does not want to permamently get rid of the item, and instead wants to take it out of the system so then they will not have to re enter information after they restock. It works alot like a file manager and the trash bin on your computer" << endl;
  do{
    
    cout << "\n\n";
    cout << "1. List Items in the System" << endl;
    cout << "2. Look up Item using itemNumber" << endl;
    cout << "3. Remove Item using Item Number" << endl;
    cout << "4. Create new Item" << endl;
    cout << "5. Empty TrashBin" << endl;
    cout << "6. List Items in TrashBin" << endl;
    cout << "7. Retrieve Item from TrashBin" << endl;
    cout << "8. Change Contents of an Item" << endl;
    cout << "9. Exit Program" << endl;
    cout << "\n\n";
    cin >> choice;

      
      if(choice == 1){
        system.listItems();
        
      
      }else if(choice == 2){
        cout << "Enter item number you would like to look up: ";
        cin >> itemNumber;
        cout << "Item Number: ItemName: ItemPrice: ItemQuantity\n";
        system.find(itemNumber);
        
      
      }else if(choice == 3){
        cout << "Enter item number you would like to delete: ";
        cin >> itemNumber;
        system.remove(itemNumber);
        
      
      }else if(choice == 4){
        cout << "Enter items ItemNumber as a four digit integer: ";
        cin >> itemNumber;
        cout << "\nEnter items Name as a string with no spaces: ";
        cin >> name;
        cout << "\nEnter items Price as a float value with 2 decimal places: ";
        cin >> price;
        cout << "\nEnter items quantity as an integer: ";
        cin >> stock;
        Item temp(name, price,itemNumber,stock);
        system.add(temp);
        
      }else if(choice == 5){
        system.emptryTrash();
        cout << "The trash file has been emptied" << endl;
      }
      else if (choice == 6){
        cout << "ITEMS FROM TRASH" << endl;
        cout << "Item Number: ItemName: ItemPrice: ItemQuantity" << endl;
        system.showTrash();
      }
      else if(choice == 7){
        cout << "Enter item number you would like to retrive: " << endl;
        cin >> itemNumber;
        system.getFromTrash(itemNumber);
        
      }
      else if(choice == 8){
        cout << "Enter item number you would like to change: " << endl;
        cin >> itemNumber;
        system.editItem(itemNumber);
      }else if(choice == 9){
        cout << "Thank you for using this program" << endl;
      }
      else{
        cout << "Invalid input" << endl;
      }
  }while(choice != 9);
}

//The rest of the program is a simple menu system that will allow the user to access the contents of the system class. This is my final and it took a very long time I hope that you like it.