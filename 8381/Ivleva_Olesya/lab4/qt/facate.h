#include "ui_mainwindow.h"
#include <base.h>

class Command {
public:
    virtual string exec(string&) = 0;
    virtual ~Command() = default;
};

class Command_Field_Neutrals : public Command{
protected:
    Field *field;
    unsigned int x1;
    unsigned int y1;
    string type;

public:
    Command_Field_Neutrals(Field *field, unsigned int x1, unsigned int y1, string type) :
        field(field), x1(x1), y1(y1), type(type){}

    string exec(string &logs){
        if(type == "B_ALL"){
            Item* bonus = new Bonus_all();
            if(field->add_item(x1, y1, bonus, logs))
                logs += "Add bonus all specifications...\n";
        }
        else if (type == "B_A") {
            Item* bonus = new Bonus_attack();
            if(field->add_item(x1, y1, bonus, logs))
                logs += "Add bonus attack...\n";
        }
        else if (type == "B_H") {
            Item* bonus = new Bonus_health();
            if(field->add_item(x1, y1, bonus, logs))
                logs += "Add bonus health...\n";
        }
    }
};

class Command_Field : public Command{
protected:
    Field *field;
    //Command_Field_Neutrals *comm_f_n;
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
    string type;

public:
    Command_Field(Field *field, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, string type) :
        field(field), x1(x1), y1(y1), x2(x2), y2(y2), type(type){
    }

    Command_Field(Field *field, unsigned int x1, unsigned int y1, string type) :
                  field(field), x1(x1), y1(y1), type(type){
    }

    string exec(string &logs){
        if (type == "m"){
            field->move_unit(x1, y1, x2, y2, logs);

        }
        else if (type == "d"){
            field->delete_unit(x1, y1, logs);
        }
        else if (type == "a") {
            logs += "Attack!!!\n";
            field->attack_unit(x1, y1, x2, y2, logs);
            field->get_unit(x2, y2, logs);
        }
        else if(type == "B_ALL" || type == "B_A" || type == "B_H"){
            Command_Field_Neutrals comm_f_n(field, x1, y1, type);
            //comm_f_n = new Command_Field_Neutrals(field, x1, y1, type);
            comm_f_n.exec(logs);
            //delete comm_f_n;
        }
        string print = "";
        field->print_unit(print);
        return print;
    }
};

class Command_Base : public Command{
protected:
    Field *field;
    Base* base;
    unsigned int x;
    unsigned int y;
    string type;

public:
    Command_Base(Field* field, Base* base, unsigned int x, unsigned int y, string type) :
        field(field), base(base), x(x), y(y), type(type){}

    string exec(string &logs){
        if (type == "D_G"){
            base->add_green_dr(x, y, logs);
        }
        else if (type == "D_R"){
            base->add_red_dr(x, y, logs);
        }
        else if (type == "W_R"){
            base->add_war_ronin(x, y, logs);
        }
        else if (type == "W_F"){
            base->add_war_fight(x, y, logs);
        }
        else if (type == "S_L"){
            base->add_sen_lion(x, y, logs);
        }
        else if (type == "S_F") {
            base->add_sen_fire(x, y, logs);
        }
        else if (type == "P_B") {
            logs += "\nOutput the base state:\n ";
            logs += base->get_comp()->getUnits_WhoAmI();
            logs += "\n";
            int health;
            health = base->getHealth();
            logs += "Health : ";
            logs += to_string(health);
            logs += "\n";
            return logs;
        }
        string print = "";
        field->print_unit(print);
        return print;
    }
};

class Facade {
protected:
    Ui::MainWindow *ui;
    Field *field;
    Base *base;
    Adapter *adapter;
    unsigned int n = 5;
    unsigned int m = 5;
    string logs = "";
    string print = "";
    int flag = 1;
    int form = 1;

public:
    Facade(Ui::MainWindow *ui, Field *field) : ui(ui), field(field){
        adapter = new Adapter(flag);
        start_game();
    }

    ~Facade(){
        delete adapter;
    }

    void start_game(){
        field = new Field(n, m, adapter);
        base = new Base(field, adapter);
        field->add_item(2, 2, base, logs);
        //fence = new Fence;
        //field->add_item(4, 3, fence, logs);
        for(unsigned int i = 0; i < n; i++){
            for(unsigned int j = 0; j < m; j++){
                if(j % 3 == 0){
                    Land* landsc = new Proxy("R");
                    field->add_land(i, j, landsc);
                }
                else if (j % 3 == 1) {
                    Land* landsc = new Proxy("F");
                    field->add_land(i, j, landsc);
                }
                else {
                    Land* landsc = new Proxy("D");
                    field->add_land(i, j, landsc);
                }
            }
        }
        field->print_unit(print);
        ui->print_item->setText(QString::fromStdString(print));
        adapter->print("Start of game...\n");
    }

    void set_flag(int fl){
        if(fl == 1){
            adapter->print("Logging to file\n");
        }
        else if (fl == 2) {
            adapter->print("Logging to console\n");
        }
        if(flag != fl){
            flag = fl;
            adapter->change_flag(flag);
        }
    }

    void set_form(int fl){
        if(fl == 1){
            adapter->print("Long form\n");
        }
        else if (fl == 2) {
            adapter->print("Short form\n");
        }
        if(form != fl){
            form = fl;
            adapter->change_form(form);
        }
    }

    void add_green_dr(){
        adapter->print("The button for adding a green dragon is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "D_G");
        print = comm_b.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void add_red_dr(){
        adapter->print("The button for adding a red dragon is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "D_R");
        print = comm_b.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void add_war_fight(){
        adapter->print("The button for adding a warrior is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "W_F");
        print = comm_b.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void add_war_ronin(){
        adapter->print("The button for adding a ronin is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "W_R");
        print = comm_b.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void add_sen_fire(){
        adapter->print("The button for adding a sentry is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "S_F");
        print = comm_b.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void add_sen_lion(){
        adapter->print("The button for adding a lion is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "S_L");
        print = comm_b.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void bonus(){
        adapter->print("The button for adding bonus is clicked\n");
        if((ui->set_x->value()+ui->set_y->value())%3 == 0){
            Command_Field comm_f(field, ui->set_x->value(), ui->set_y->value(), "B_ALL");
            adapter->print("Add bonus all specifications\n");
            print = comm_f.exec(logs);
        }
        else if ((ui->set_x->value()+ui->set_y->value())%3 == 1){
            Command_Field comm_f(field, ui->set_x->value(), ui->set_y->value(), "B_A");
            adapter->print("Add bonus attack\n");
            print = comm_f.exec(logs);
        }
        else if ((ui->set_x->value()+ui->set_y->value())%3 == 2) {
            Command_Field comm_f(field, ui->set_x->value(), ui->set_y->value(), "B_H");
            adapter->print("Add bonus health\n");
            print = comm_f.exec(logs);
        }
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void delete_item(){
        adapter->print("The button for deleting unit is clicked\n");
        Command_Field comm_f(field, ui->set_x->value(), ui->set_y->value(), "d");
        print = comm_f.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

    void move_unit(){
        adapter->print("The button for moving unit is clicked\n");
        Command_Field comm_f(field, ui->set_x->value(), ui->set_y->value(), ui->set_x1->value(), ui->set_y1->value(), "m");
        print = comm_f.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));

    }

    void print_base(){
        adapter->print("The button for showing base is clicked\n");
        Command_Base comm_b(field, base, ui->set_x->value(), ui->set_y->value(), "P_B");
        print = adapter->print(comm_b.exec(logs));
        ui->print->setText(QString::fromStdString(logs));
    }

    void attack(){
        adapter->print("The button for attack is clicked\n");
        Command_Field comm_f(field, ui->set_x->value(), ui->set_y->value(), ui->set_x1->value(), ui->set_y1->value(), "a");
        print = comm_f.exec(logs);
        ui->print_item->setText(QString::fromStdString(print));
        ui->print->setText(QString::fromStdString(logs));
    }

};
