#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include "Team.h"

class Game {
private:
    Team* team1;
    Team* team2;

    /*
    * Подбор игроков
    * 
    * @param first_team текущие игроки первой команды
    * @param second_team текущие игроки второй команды
    * @param f_team_pl номер игрока первой команды
    * @param s_team_pl номер игрока первой команды
    * @param chance_first_team шанс первой команды на подбор
    * @param chance_second_team шанс второй команды на подбор
    * @return массив, в котором первое значение - номер команды, второе - индекс игрока
    */
    std::array<int, 2> doRebound(std::array<int, 5> first_team, std::array<int, 5> second_team) {
        int f_team_pl = 0 + rand() % 5;
        int s_team_pl = 0 + rand() % 5;
        int chance_first_team = team1->reboundByPlayer(first_team[f_team_pl]);
        int chance_second_team = team2->reboundByPlayer(second_team[s_team_pl]);
        if (chance_first_team > chance_second_team) {
            team1->addReboundStat(first_team[f_team_pl]);
            return std::array<int, 2> { 0, f_team_pl };
        }
        team2->addReboundStat(second_team[s_team_pl]);
        return std::array<int, 2> { 1, s_team_pl };
    }

    /*
    * Новая минута
    * 
    * @param f_team массив текущих игроков первой команды
    * @param s_team массив текущих игроков первой команды
    * Использую метод для каждой команды передавая в них массив текущего состава
    */
    void newMinute(std::array<int, 5> f_team, std::array<int, 5> s_team) {
        team1->changeEnergyMin(f_team);
        team2->changeEnergyMin(s_team);
    }

    /*
    * Новая четверть
    *
    * Вызвыаю метод добавления энергии к каждой команде с значением 10
    */
    void newQuater() {
        team1->addEnergyForAll(10);
        team2->addEnergyForAll(10);
    }

    /*
    * Новая половина
    *
    * Вызвыаю метод добавления энергии к каждой команде с значением 20
    */
    void newHalf() {
        team1->addEnergyForAll(20);
        team2->addEnergyForAll(20);
    }

    void printStartTeam(Team* team, std::array<int, 5> current_pl) {
        std::cout << team->getName() << ": ";
        for (int i = 0; i < 5; i++)
            std::cout << team->getPlayerNumber(current_pl[i]) << " ";
        std::cout << std::endl;
    }

public:
    /*
    * Конструктор
    * 
    * @param team1 первая команда
    * @param team2 вторая команда
    * Генерирую команды и вывожу информацию о них
    */
    Game() {
        team1 = new Team("Первая команда");
        team2 = new Team("Вторая команда");
        std::cout << "Составы команд\n";
        team1->printTeam();
        team2->printTeam();
    }

    /*
    * Симуляция игры
    * 
    * @param current_pl_t1 текущий состав первой команды
    * @param current_pl_t2 текущий состав второй команды
    * @param points количество очков первой и второй команды
    * @param current_team текущая команда
    * @param current_player номер текущего игрока
    * @param end_time_for_attack закончено ли время на атаку
    * @param old_team предыдущая команда с мячом
    * Генерирую текущие составы игроков
    * Создаю массив счета команд
    * Генерирую номер текущей команды и номер игрока с мячом
    * Прохожусь по 4 четвертям (в конце каждой добавляю энергию игрокам)
    * Внутри цикла прохожусь по времени атаки пока не закончилось время одной четверти
    * В соответствии с тем у какой команды и какого игрока мяч использую метод атаки
    * В соответствии с отловлей событий вывожу информацию о том что произошло и меняю
    * статистику матча, текущую команду и игрока с мячом
    * Вывожу статистику матча и каждого игрока каждой команды
    */
    void gameSimulation() {
        std::array<int, 5> current_pl_t1 = team1->getStarterTeam();
        std::array<int, 5> current_pl_t2 = team2->getStarterTeam();

        std::cout << "Стартовые составы" << std::endl
            << "------------------------------------------" << std::endl;
        printStartTeam(team1, current_pl_t1);
        printStartTeam(team2, current_pl_t2);
        std::cout << "------------------------------------------" << std::endl;

        std::array<int, 2> points{ 0, 0 };

        int current_team = rand() % 2;
        int current_player = 0 + rand() % 5;

        bool team1_can_continue = true;
        bool team2_can_continue = true;

        for (int qt = 0; qt < 4 && (team1_can_continue && team2_can_continue); qt++) {
            int time = 0;
            while (time < 10 * 60 && (team1_can_continue && team2_can_continue)) {
                bool end_time_for_attack = true;

                int enemy_cover = 0 + rand() % 5;
                for (int i = 0; i < 24; i++) {
                    time++;
                    if (time % 60 == 0) {
                        newMinute(current_pl_t1, current_pl_t2);
                        current_pl_t1 = team1->checForChanges(current_pl_t1, team1_can_continue);
                        current_pl_t2 = team2->checForChanges(current_pl_t2, team2_can_continue);
                        if (time == 600)
                            break;
                    }

                    if (i >= 10) {
                        //if()

                        try {
                            if (current_team == 0)
                                current_player = team1->doAttack(current_pl_t1, current_player, team2, current_pl_t2[enemy_cover]);
                            else
                                current_player = team2->doAttack(current_pl_t2, current_player, team1, current_pl_t1[enemy_cover]);
                        }
                        catch (int e) {
                            end_time_for_attack = false;
                            if (e <= -1 && e > -4) {
                                //Залетела двушка
                                if (e == -1) {
                                    points[current_team] += 2;
                                    std::cout << ((current_team == 0) ? team1->getName() : team2->getName()) << ": игрок #"
                                        << ((current_team == 0) ? team1->getPlayerNumber(current_pl_t1[current_player]) : team2->getPlayerNumber(current_pl_t2[current_player]))
                                        << " забил 2-очковый\n";
                                }
                                //Залетела трешка
                                if (e == -2) {
                                    points[current_team] += 3;
                                    std::cout << ((current_team == 0) ? team1->getName() : team2->getName()) << ": игрок #"
                                        << ((current_team == 0) ? team1->getPlayerNumber(current_pl_t1[current_player]) : team2->getPlayerNumber(current_pl_t2[current_player]))
                                        << " забил 3-очковый\n";
                                }

                                if (current_team == 0)
                                    current_team = 1;
                                else
                                    current_team = 0;
                                current_player = rand() % 5;
                                //Перехватили
                                if (e == -3) {
                                    current_player = enemy_cover;
                                    std::cout << ((current_team == 0) ? team1->getName() : team2->getName()) << ": игрок #"
                                        << ((current_team == 0) ? team1->getPlayerNumber(current_pl_t1[current_player]) : team2->getPlayerNumber(current_pl_t2[current_player]))
                                        << " перехватил мяч\n";
                                }
                            }
                            //Промах
                            if (e == -4) {
                                std::cout << ((current_team == 0) ? team1->getName() : team2->getName()) << ": игрок #"
                                    << ((current_team == 0) ? team1->getPlayerNumber(current_pl_t1[current_player]) : team2->getPlayerNumber(current_pl_t2[current_player]))
                                    << " промазал при броске, после чего ";
                                std::array<int, 2> team_pl_reb = doRebound(current_pl_t1, current_pl_t2);

                                

                                current_team = team_pl_reb[0];
                                current_player = team_pl_reb[1];
                                std::cout << " игрок " << ((current_team == 0) ? team1->getName() : team2->getName()) << " #"
                                    << ((current_team == 0) ? team1->getPlayerNumber(current_pl_t1[current_player]) : team2->getPlayerNumber(current_pl_t2[current_player]))
                                    << " сделал подбор\n";
                            }

                            if (e == -10) {
                                std::cout<<" игрок " << ((current_team == 0) ? team1->getName() : team2->getName()) << " #"
                                    << ((current_team == 0) ? team1->getPlayerNumber(current_pl_t1[current_player]) : team2->getPlayerNumber(current_pl_t2[current_player]))
                                    << " получил травму и выбывает из игры\n";
                            }
                            break;
                        }
                    }
                }
                if (end_time_for_attack) {
                    std::cout << "У команды " << ((current_team == 0) ? team1->getName() : team2->getName())
                        << " вышло время на атаку, из-за чего мяч переходит команде противника\n";
                    if (current_team == 0)
                        current_team = 1;
                    else
                        current_team = 0;
                    current_player = rand() % 5;
                }

            }
            (qt == 1) ? newHalf() : newQuater();
        }
        std::cout << "Результат встречи" << std::endl
            << team1->getName() << " " << points[0] << " : " << points[1] << " " << team2->getName() << std::endl;
        std::cout << "Статистика игроков " << team1->getName() << std::endl;
        team1->printPlayerStats();
        std::cout << "Статистика игроков " << team2->getName() << std::endl;
        team2->printPlayerStats();
    }

    ~Game() {
        delete team1;
        delete team2;
    }
};