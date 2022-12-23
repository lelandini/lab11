#pragma once
#include "BasketballPlayer.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>

class Team {
private:
    std::string name;
    BasketballPlayer** players;
    const int num_players = 12;
    const int min_num_dif = 4;
    const int min_num_forw = 4;
    const int min_num_centr = 2;
    std::vector<int> numbers;

    /*
    * ��������� ������
    * 
    * @param current_index ������� ������ � ������� �������
    * @param count ���������� ����������� �������
    * @param pos ������� ������
    * @param number ����� ������
    * @param players ������ �������
    * ������ ����� �� �������� ������� ������� � �������� �����
    * ����������� ���� �� �������
    */
    void generatePlayers(int current_index, int count, TypePlayers pos) {
        for (int i = current_index; i < count + current_index; i++) {
            int number = 0 + rand() % 101;
            while (!numbers.empty() && std::find(numbers.begin(), numbers.end(), number) != numbers.end())
                number = 0 + rand() % 101;

            numbers.push_back(number);
            switch (pos)
            {
            case PosDefender:
                players[i] = new Defender(number);
                break;
            case PosForward:
                players[i] = new Forward(number);
                break;
            case PosCentre:
                players[i] = new Centre(number);
                break;
            default:
                break;
            }
        }
    }

    /*
    * ��������� ������� ������� �������
    * 
    * @param type ������� �������
    * @param pl_vect ������ ������� �������
    * @param players ������ �������
    * @param num_players ���������� �������
    * ��������� �� ���� ������� � ���� �� �������
    * ����� ���������� - �������� � ������
    * @return ������ ������� ���������� �������
    */
    std::vector<int> getPlayersByPos(TypePlayers type) {
        std::vector<int> pl_vect;
        for (int i = 0; i < num_players; i++)
            if (players[i]->getPos() == type)
                pl_vect.push_back(i);
        return pl_vect;
    }
public:
    /*
    * �����������
    * 
    * @param name �������� �������
    * @param players ������ �������
    * @param num_def ���������� ����������
    * @param num_forw ���������� ���������
    * @param num_centr ���������� ���������
    * ��������� ������� � �������
    */
    Team(std::string name) :
        name(name)
    {
        players = new BasketballPlayer * [num_players];

        //��������� ����������
        int num_def = min_num_dif + rand() % (num_players - min_num_forw - min_num_centr - min_num_dif + 1);
        generatePlayers(0, num_def, PosDefender);

        //��������� ���������
        int num_forw = min_num_forw + rand() % (num_players - min_num_forw - min_num_centr - num_def + 1);
        generatePlayers(num_def, num_forw, PosForward);

        //��������� ���������
        int num_centr = num_players - num_forw- num_def;
        generatePlayers(num_def + num_forw, num_centr, PosCentre);
    }
    /*
    * ��������� ���������� �������
    * 
    * @param start_team ��������� ������
    * ������ ������ ������� �� ������ �������
    */
    std::array<int, 5> getStarterTeam() {
        std::vector<int> defenders = getPlayersByPos(PosDefender);
        std::vector<int> forwards = getPlayersByPos(PosForward);
        std::vector<int> centers = getPlayersByPos(PosCentre);
        return std::array<int, 5> {
                defenders[0],
                defenders[1],
                forwards[0],
                forwards[1],
                centers[0]
        };
    }

    /*
    * �������� ��������� �������
    *
    * @param current_team ������ �������� �������� �������
    * @param changes ���������
    * @param players
    * ��������� �� �������� ������� � �������� �� ������ �� ������
    * ���� ���� ������ ��� ��� � ����� ������� ����������� �������, ��
    * ����� ��
    * ������ ���������� ���������
    * @return ����� ������� ������
    */
    std::array<int, 5> checForChanges(std::array<int, 5> current_team, bool &canContinue) {
        std::string changes = "";
        for (int i = 0; i < current_team.size(); i++) {
            if (players[current_team[i]]->getEnergy() < 50 || players[current_team[i]]->isInjured()) {
                
                players[current_team[i]]->changeStats();
                std::vector<int> players_can_change = getPlayersByPos(players[current_team[i]]->getPos());

                for (int index : players_can_change) {

                    if (players[current_team[i]]->isInjured() && !(players[index]->isInjured())) {

                        changes += "#" + std::to_string(players[index]->getNumber()) + " ������� ������ #" + std::to_string(players[current_team[i]]->getNumber()) + "; ";
                        current_team[i] = index;
                        break;

                    } else if ((std::find(current_team.begin(), current_team.end(), index) == current_team.end())
                        && (players[index]->getEnergy() > players[current_team[i]]->getEnergy()) ) {
                        changes += "#" + std::to_string(players[index]->getNumber()) + " ������� ������ #" + std::to_string(players[current_team[i]]->getNumber()) + "; ";
                        current_team[i] = index;
                        break;
                    }
                }
            }
        }
        if (changes != "")
            std::cout << "������ � " << name << ": " << changes << std::endl;

        bool haveInjured = false;
        for (int i = 0; i < current_team.size();++i) {
            if (players[current_team[i]]->isInjured()) {
                haveInjured = true;
                break;
            }
        }

        if (haveInjured) {
            std::cout << " � ������� " << name << " �������� ������ 5 ������� �� ����. ����������� ���������!" << std::endl;
            canContinue = false;
        }

        return current_team;
    }

    /*
    * ����� �������
    *
    * @param player_indecies ������� �������� ������� �������
    * @param index_pl ����� � ������� player_indecies
    * @param enemy_team ������� �������� ������� ������� ������ �������
    * @param enemy_index ����� � ������� enemy_team
    * @param new_current_pl ����� ������� �����
    * ��������� ����� ����� ������, ����� ���� ���� ��� ��� �� �������� ������� ������ �������� �� ��� ������
    * ������ ������ � ����
    * @return ������ �������� ������
    */
    int doAttack(std::array<int, 5> player_indecies, int index_pl, Team* enemy_team, int enemy_index) {
        if (players[player_indecies[index_pl]]->isInjured())
            throw - 9;
        players[player_indecies[index_pl]]->tryDoSomething(enemy_team->players[enemy_index]);

        int new_current_pl = 0 + rand() % 5;
        while (new_current_pl == index_pl)
            new_current_pl = 0 + rand() % 5;
        std::cout << name << ": ����� #" << players[player_indecies[index_pl]]->getNumber() << " ����� ��� ������ #" << players[player_indecies[new_current_pl]]->getNumber() << std::endl;
        return new_current_pl;
    }

    BasketballPlayer** getPlayers() {
        return players;
    }

    int reboundByPlayer(int pl_index) {
        return players[pl_index]->doRebound();
    }

    /*
    * ��������� �������
    * 
    * @param current_team ������� ������ �������
    * �������� �� �������� ��� �� ���� ����� � ��������� ��������������� �����
    */
    void changeEnergyMin(std::array<int, 5> current_team) {
        for (int i = 0; i < num_players; i++) {
            if (std::find(current_team.begin(), current_team.end(), i) == current_team.end())
                players[i]->newMinuteBanch();
            else
                players[i]->newMinuteField();
        }
    }

    /*
    * ���������� ���� ������� �������
    * 
    * @param enegry ������� ������� ����� ��������
    * ��������� �� ���� ������� ������� � ��������� �����,
    * � ������� ������� ������� ��� ����������
    */
    void addEnergyForAll(int enegry) {
        for (int i = 0; i < num_players; i++)
            players[i]->addEnergy(enegry);
    }

    /*
    * ���������� ���������� �������
    * 
    * @param index
    * �� ������� ������ �������� ��� ������
    */
    void addReboundStat(int index) {
        players[index]->addReboundStat();
    }

    std::string getName() {
        return name;
    }

    /*
    * ����� �������
    *
    * @param name �������� �������
    * @param players ������ �������
    * ������ �������� ������� � �������������� ���� �������
    */
    void printTeam() {
        std::cout << "-------------------------------" << std::endl
            << "-------------------------------" << std::endl
            << "������� " << name << std::endl;
        for (int i = 0; i < num_players; i++)
            players[i]->printPlayer();
    }

    int getPlayerNumber(int index) {
        return players[index]->getNumber();
    }

    void printPlayerStats() {
        for (int i = 0; i < num_players; i++)
            players[i]->printStats();
    }

    ~Team() {
        for (int i = 0; i < num_players; i++)
            delete[] players[i];
        delete[] players;
    }
};
