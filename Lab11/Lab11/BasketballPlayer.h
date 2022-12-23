#pragma once
#include "TypePlayers.h"

class BasketballPlayer {
protected:
    TypePlayers pos;    // �������
    int number;         // ������� �����
    int double_pt;      // ������
    int triple_pt;      // ������
    int rebound;        // �������
    int pass;           // ����
    int steal;          // ��������
    int block;          // �������
    int have_ball;      // �������� �����
    int endurance;      // ������������
    int energy;         // �������
    bool is_tired;      // �������� �� �����

    int time_on_field;  // ����� �� ���� (���)
    int point_done;     // ���������� �����
    int pass_done;      // ������� �����
    int rebound_done;   // ������� ��������
    int steels_done;    // ��������� �������

    bool is_injured;
public:
    BasketballPlayer() {};

    /*
    * �����������
    * 
    * @param pos �������
    * @param number ������� �����
    * @param double_pt ������
    * @param triple_pt ������
    * @param rebound �������
    * @param pass ����
    * @param steal ������
    * @param block ��������
    * @param have_ball ��������
    * @param endurance ������������
    * ������������� ��������� ������
    */
    BasketballPlayer(TypePlayers pos, int number, int double_pt, int triple_pt, int rebound,
        int pass, int steal, int block, int have_ball, int endurance) :
        pos(pos),
        number(number),
        double_pt(double_pt),
        triple_pt(triple_pt),
        rebound(rebound),
        pass(pass),
        steal(steal),
        block(block),
        have_ball(have_ball),
        endurance(endurance),
        energy(100),
        is_tired(false),
        time_on_field(0),
        point_done(0),
        pass_done(0),
        rebound_done(0),
        steels_done(0),
        is_injured(false)
    {};

    bool isInjured() {
        return this->is_injured;
    }

    /*
    * ��������� �������� ������
    * 
    * @param enemy_pl ������ �� ����������
    * @param pass_chance ���� ����
    * @param double_pt_chance ���� ������
    * @param triple_pt_chance ���� ������
    * @param energy �������
    * @param chance_action ���� �������
    * � ����������� �� ������� ������ ����������� ����� ����
    * ��������� ����������� �������� ������
    * � ������������ � ���������� ������������ ������� �����
    * ��������� true ���� ��� ������ ���
    */
    bool tryDoSomething(BasketballPlayer* enemy_pl) {
        if ((1 + rand() % 100) <= (1 + rand() % (101 - this->energy))) {
            this->is_injured = true;
            throw - 10;
        }

        double pass_chance = 1.5 * pass / (1.5 * pass + double_pt + triple_pt);
        double double_pt_chance = double_pt / (1.5 * pass + double_pt + triple_pt);
        double triple_pt_chance = triple_pt / (1.5 * pass + double_pt + triple_pt);

        energy -= 1 + rand() % (endurance);
        enemy_pl->energy = 1 + rand() % (enemy_pl->endurance);

        int chance_action = 1 + rand() % 100;

        if (double_pt_chance > pass_chance && double_pt_chance > triple_pt_chance)
            tryToDoublePt(enemy_pl, chance_action);
        if (triple_pt_chance > pass_chance && triple_pt_chance > double_pt_chance)
            tryToTriplePt(enemy_pl, chance_action);
        doPass(enemy_pl, chance_action);
        return true;
    }

    /*
    * ����
    * 
    * @param enemy_pl ������ �� ���������� 
    * @param chance_steal ���� ������
    * @param pass_done �������� ���� ������
    * ���� ���� ������ �������� �� �������� � ���������� ����������
    * �������� ����� � ���������� � ����� -3
    * ����� �������� � ���������� �������� ����
    */
    void doPass(BasketballPlayer* enemy_pl, int chance_steal) {
        if (chance_steal < (15 + enemy_pl->steal - pass)) {
            enemy_pl->steels_done++;
            throw - 3;
        }
        pass_done++;
    }

    /*
    * ������
    * 
    * @param enemy_pl ������ �� ���������� 
    * @param chance_2pt ���� ������
    * @param point_done ������� ����
    * ���� ����� �����, �� ���������� � ����� -1
    * ���� ��������, �� � ����� -4
    */
    void tryToDoublePt(BasketballPlayer* enemy_pl, int chance_2pt) {
        if (chance_2pt < 40 + (double_pt - enemy_pl->block)) {
            point_done += 2;
            throw - 1;
        }
        throw - 4;
    }

    /*
    * ������
    *
    * @param enemy_pl ������ �� ����������
    * @param chance_3pt ���� ������
    * @param point_done ������� ����
    * ���� ����� �����, �� ���������� � ����� -2
    * ���� ��������, �� � ����� -4
    */
    void tryToTriplePt(BasketballPlayer* enemy_pl, int chance_3pt) {
        if (chance_3pt < 30 + (triple_pt - enemy_pl->block)) {
            point_done += 3;
            throw - 2;
        }
        throw - 4;
    }

    /*
    * ��������� ����������� ������
    * 
    * @param is_tired ���������
    * @param double_pt ������
    * @param triple_pt ������
    * @param rebound �������
    * @param pass ����
    * @param steal ������
    * @param block ��������
    * �������� ���������� ��������� � ������� � �������� ����
    * ���������� ���������� ������ � ������������
    */
    void changeStats() {
        if (is_tired == false && energy < 50) {
            is_tired = true;
            double_pt /= 2;
            triple_pt /= 2;
            rebound /= 2;
            pass /= 2;
            steal /= 2;
            block /= 2;
        }
        else if (is_tired == true && energy >= 50) {
            is_tired = false;
            double_pt *= 2;
            triple_pt *= 2;
            rebound *= 2;
            pass *= 2;
            steal *= 2;
            block *= 2;
        }
    }

    /*
    * ���������� �������
    * 
    * @param number ���������� ������� ��� ����������
    * �������� ������� ����� ���� �������� ��������� ������������� ������
    */
    void addEnergy(int number) {
        energy += number;
        if (energy > 100)
            energy = 100;
        changeStats();
    }

    /*
    * ����� ������ �� ��������
    * 
    * @param energy �������� ������ 
    * �������� ������� ������ � �������� �� ��������� �������������
    */
    void newMinuteBanch() {
        energy += 5;
        if (energy > 100)
            energy = 100;
        changeStats();
    }

    /*
    * ����� ������ �� ����
    *
    * @param time_on_field ���������� ����� �� ����
    * �������� ������� � ������� �� ����
    */
    void newMinuteField() {
        time_on_field += 1;
    }

    TypePlayers getPos() {
        return pos;
    }

    void addReboundStat() {
        rebound_done++;
    }

    int getEnergy() {
        return energy;
    }

    int getNumber() {
        return number;
    }

    virtual int doRebound() { return 0; };
    virtual void printPlayer() { };
    virtual void printStats() {};
};

//����� ���������
class Defender : public BasketballPlayer {
public:
    /*
    * �����������
    * 
    * @param number ����� ������
    * ������������� ����� �������� � ������������ �����������
    */
    Defender(int number) :
        BasketballPlayer(
            PosDefender,
            number,
            1 + rand() % 5,
            1 + rand() % 5,
            1 + rand() % 5,
            6 + rand() % (10 - 6 + 1),
            6 + rand() % (10 - 6 + 1),
            1 + rand() % 5,
            6 + rand() % (10 - 6 + 1),
            2 + rand() % (5 - 2 + 1)
        )
    {}

    /*
    * ������
    * 
    * �������� ������� ������ � ������ ���� �������
    */
    int doRebound() {
        energy -= 1 + rand() % endurance;
        return rebound + (1 + rand() % 10);
    }

    /*
    * ����� ������������� ������
    *
    * ��������� �� ���� ���������� ������ � ������ ��
    */
    void printPlayer() {
        std::cout << "------------------" << std::endl
            << "������� ����� - " << number << std::endl
            << "������� - ��������" << std::endl
            << "������ - " << double_pt << std::endl
            << "������ - " << triple_pt << std::endl
            << "������� - " << rebound << std::endl
            << "�������� - " << pass << std::endl
            << "��������� - " << steal << std::endl
            << "�������� - " << block << std::endl
            << "�������� ����� - " << have_ball << std::endl
            << "������������ - " << endurance << std::endl;
    }

    /*
    * ����� ���������� ������
    *
    * ��������� �� ���� ���������� ������ � ������ ��
    */
    void printStats() {
        std::cout << "#" << std::setw(3) << number << std::setw(10) << "(Guard)"
            << ": ����� �� ���� - " << std::setw(2) << time_on_field
            << "| ���� - " << std::setw(2) << point_done
            << "| ���� - " << std::setw(2) << pass_done
            << "| ������� - " << std::setw(2) << rebound_done
            << "| ��������� - " << std::setw(2) << steels_done << std::endl;
    }
};

//����� ��������
class Forward : public BasketballPlayer {
public:
    /*
    * �����������
    *
    * @param number ����� ������
    * ������������� ����� �������� � ������������ �����������
    */
    Forward(int number) :
        BasketballPlayer(
            PosForward,
            number,
            6 + rand() % (10 - 6 + 1),
            6 + rand() % (10 - 6 + 1),
            6 + rand() % (10 - 6 + 1),
            1 + rand() % 5,
            1 + rand() % 5,
            1 + rand() % 5,
            1 + rand() % 5,
            2 + rand() % (5 - 2 + 1)
        )
    {}

    /*
    * ������
    *
    * �������� ������� ������ � ������ ���� �������
    */
    int doRebound() {
        energy -= 1 + rand() % endurance;
        return rebound + (1 + rand() % 7);
    }

    /*
    * ����� ������������� ������
    *
    * ��������� �� ���� ���������� ������ � ������ ��
    */
    void printPlayer() {
        std::cout << "------------------" << std::endl
            << "������� ����� - " << number << std::endl
            << "������� - �������" << std::endl
            << "������ - " << double_pt << std::endl
            << "������ - " << triple_pt << std::endl
            << "������� - " << rebound << std::endl
            << "�������� - " << pass << std::endl
            << "��������� - " << steal << std::endl
            << "�������� - " << block << std::endl
            << "�������� ����� - " << have_ball << std::endl
            << "������������ - " << endurance << std::endl;
    }

    /*
    * ����� ���������� ������
    *
    * ��������� �� ���� ���������� ������ � ������ ��
    */
    void printStats() {
        std::cout << "#" << std::setw(3) << number << std::setw(10) << "(Forward)"
            << ": ����� �� ���� - " << std::setw(2) << time_on_field
            << "| ���� - " << std::setw(2) << point_done
            << "| ���� - " << std::setw(2) << pass_done
            << "| ������� - " << std::setw(2) << rebound_done
            << "| ��������� - " << std::setw(2) << steels_done << std::endl;
    }
};

//����� ����������
class Centre :public BasketballPlayer {
public:
    /*
    * �����������
    *
    * @param number ����� ������
    * ������������� ����� �������� � ������������ �����������
    */
    Centre(int number) :
        BasketballPlayer(
            PosCentre,
            number,
            6 + rand() % (10 - 6 + 1),
            1 + rand() % 5,
            1 + rand() % 5,
            1 + rand() % 5,
            1 + rand() % 5,
            6 + rand() % (10 - 6 + 1),
            1 + rand() % 5,
            2 + rand() % (5 - 2 + 1)
        )
    {}

    /*
    * ������
    *
    * �������� ������� ������ � ������ ���� �������
    */
    int doRebound() {
        energy -= 1 + rand() % endurance;
        return rebound;
    }

    /*
    * ����� ������������� ������
    *
    * ��������� �� ���� ���������� ������ � ������ ��
    */
    void printPlayer() {
        std::cout << "------------------" << std::endl
            << "������� ����� - " << number << std::endl
            << "������� - ���������" << std::endl
            << "������ - " << double_pt << std::endl
            << "������ - " << triple_pt << std::endl
            << "������� - " << rebound << std::endl
            << "�������� - " << pass << std::endl
            << "��������� - " << steal << std::endl
            << "�������� - " << block << std::endl
            << "�������� ����� - " << have_ball << std::endl
            << "������������ - " << endurance << std::endl;
    }

    /*
    * ����� ���������� ������
    *
    * ��������� �� ���� ���������� ������ � ������ ��
    */
    void printStats() {
        std::cout << "#" << std::setw(3) << number << std::setw(10) << "(Center)"
            << ": ����� �� ���� - " << std::setw(2) << time_on_field
            << "| ���� - " << std::setw(2) << point_done
            << "| ���� - " << std::setw(2) << pass_done
            << "| ������� - " << std::setw(2) << rebound_done
            << "| ��������� - " << std::setw(2) << steels_done << std::endl;
    }
};

