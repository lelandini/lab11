#pragma once
#include "TypePlayers.h"

class BasketballPlayer {
protected:
    TypePlayers pos;    // Позиция
    int number;         // Игровой номер
    int double_pt;      // Двушки
    int triple_pt;      // Трешки
    int rebound;        // Подборы
    int pass;           // Пасы
    int steal;          // Перехват
    int block;          // Блокшот
    int have_ball;      // Владение мячом
    int endurance;      // Выносливость
    int energy;         // Энергия
    bool is_tired;      // Уставший ли игрок

    int time_on_field;  // Время на поле (мин)
    int point_done;     // Заработано очков
    int pass_done;      // Сделано пасов
    int rebound_done;   // Сделано подборов
    int steels_done;    // Выполнено отборов

    bool is_injured;
public:
    BasketballPlayer() {};

    /*
    * Конструктор
    * 
    * @param pos позиция
    * @param number игровой номер
    * @param double_pt двушки
    * @param triple_pt трешки
    * @param rebound подборы
    * @param pass пасы
    * @param steal отборы
    * @param block блокшоты
    * @param have_ball владение
    * @param endurance выносливость
    * Инициализирую параметры класса
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
    * Активация действия игрока
    * 
    * @param enemy_pl ссылка на опекающего
    * @param pass_chance шанс паса
    * @param double_pt_chance шанс двушки
    * @param triple_pt_chance шанс трешки
    * @param energy энергия
    * @param chance_action шанс события
    * В зависимости от позиции создаю коэффициент после чего
    * определяю вероятности действий игрока
    * В соответствии с наибольшей вероятностью вызываю метод
    * Возвращаю true если был сделан пас
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
    * Пасс
    * 
    * @param enemy_pl ссылка на опекающего 
    * @param chance_steal шанс отбора
    * @param pass_done успешные пасы игрока
    * Если шанс отбора подходит то добавляю в статистику опекающего
    * успешный отбор и выбрасываю с кодом -3
    * Иначе добавляю в статистику успешный пасс
    */
    void doPass(BasketballPlayer* enemy_pl, int chance_steal) {
        if (chance_steal < (15 + enemy_pl->steal - pass)) {
            enemy_pl->steels_done++;
            throw - 3;
        }
        pass_done++;
    }

    /*
    * Двушка
    * 
    * @param enemy_pl ссылка на опекающего 
    * @param chance_2pt шанс двушки
    * @param point_done забитые очки
    * Если игрок забил, то выбрасываю с кодом -1
    * Если промазал, то с кодом -4
    */
    void tryToDoublePt(BasketballPlayer* enemy_pl, int chance_2pt) {
        if (chance_2pt < 40 + (double_pt - enemy_pl->block)) {
            point_done += 2;
            throw - 1;
        }
        throw - 4;
    }

    /*
    * Трешка
    *
    * @param enemy_pl ссылка на опекающего
    * @param chance_3pt шанс трешки
    * @param point_done забитые очки
    * Если игрок забил, то выбрасываю с кодом -2
    * Если промазал, то с кодом -4
    */
    void tryToTriplePt(BasketballPlayer* enemy_pl, int chance_3pt) {
        if (chance_3pt < 30 + (triple_pt - enemy_pl->block)) {
            point_done += 3;
            throw - 2;
        }
        throw - 4;
    }

    /*
    * Изменение показателей игрока
    * 
    * @param is_tired усталость
    * @param double_pt двушки
    * @param triple_pt трешки
    * @param rebound подборы
    * @param pass пасы
    * @param steal отборы
    * @param block блокшоты
    * Проверяю показатели усталости и энергии и уменьшаю либо
    * увеличиваю показатели игрока в соответствии
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
    * Добавление энергии
    * 
    * @param number количество энергии для добавления
    * Добавляю энергию после чего проверяю изменения характеристик игрока
    */
    void addEnergy(int number) {
        energy += number;
        if (energy > 100)
            energy = 100;
        changeStats();
    }

    /*
    * Новая минута на скамейке
    * 
    * @param energy энгергия игрока 
    * Добавляю энергию игроку и проверяю на изменение характеристик
    */
    void newMinuteBanch() {
        energy += 5;
        if (energy > 100)
            energy = 100;
        changeStats();
    }

    /*
    * Новая минута на поле
    *
    * @param time_on_field количество минут на поле
    * Добавляю единицу к времени на поле
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

//КЛАСС ЗАЩИТНИКА
class Defender : public BasketballPlayer {
public:
    /*
    * Конструктор
    * 
    * @param number номер игрока
    * Инициализирую класс родитель с необходимыми параметрами
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
    * Подбор
    * 
    * Уменьшаю энергию игрока и вывожу шанс подбора
    */
    int doRebound() {
        energy -= 1 + rand() % endurance;
        return rebound + (1 + rand() % 10);
    }

    /*
    * Вывод характиристик игрока
    *
    * Прохожусь по всем параметрам игрока и вывожу их
    */
    void printPlayer() {
        std::cout << "------------------" << std::endl
            << "Игровой номер - " << number << std::endl
            << "Позиция - защитник" << std::endl
            << "Двушки - " << double_pt << std::endl
            << "Трешки - " << triple_pt << std::endl
            << "Подборы - " << rebound << std::endl
            << "Передачи - " << pass << std::endl
            << "Перехваты - " << steal << std::endl
            << "Блокшоты - " << block << std::endl
            << "Владение мячом - " << have_ball << std::endl
            << "Выносливость - " << endurance << std::endl;
    }

    /*
    * Вывод статистики игрока
    *
    * Прохожусь по всем параметрам игрока и вывожу их
    */
    void printStats() {
        std::cout << "#" << std::setw(3) << number << std::setw(10) << "(Guard)"
            << ": время на поле - " << std::setw(2) << time_on_field
            << "| очки - " << std::setw(2) << point_done
            << "| пасы - " << std::setw(2) << pass_done
            << "| подборы - " << std::setw(2) << rebound_done
            << "| перехваты - " << std::setw(2) << steels_done << std::endl;
    }
};

//КЛАСС ФОРВАРДА
class Forward : public BasketballPlayer {
public:
    /*
    * Конструктор
    *
    * @param number номер игрока
    * Инициализирую класс родитель с необходимыми параметрами
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
    * Подбор
    *
    * Уменьшаю энергию игрока и вывожу шанс подбора
    */
    int doRebound() {
        energy -= 1 + rand() % endurance;
        return rebound + (1 + rand() % 7);
    }

    /*
    * Вывод характиристик игрока
    *
    * Прохожусь по всем параметрам игрока и вывожу их
    */
    void printPlayer() {
        std::cout << "------------------" << std::endl
            << "Игровой номер - " << number << std::endl
            << "Позиция - форвард" << std::endl
            << "Двушки - " << double_pt << std::endl
            << "Трешки - " << triple_pt << std::endl
            << "Подборы - " << rebound << std::endl
            << "Передачи - " << pass << std::endl
            << "Перехваты - " << steal << std::endl
            << "Блокшоты - " << block << std::endl
            << "Владение мячом - " << have_ball << std::endl
            << "Выносливость - " << endurance << std::endl;
    }

    /*
    * Вывод статистики игрока
    *
    * Прохожусь по всем параметрам игрока и вывожу их
    */
    void printStats() {
        std::cout << "#" << std::setw(3) << number << std::setw(10) << "(Forward)"
            << ": время на поле - " << std::setw(2) << time_on_field
            << "| очки - " << std::setw(2) << point_done
            << "| пасы - " << std::setw(2) << pass_done
            << "| подборы - " << std::setw(2) << rebound_done
            << "| перехваты - " << std::setw(2) << steels_done << std::endl;
    }
};

//КЛАСС ЦЕНТРОВОГО
class Centre :public BasketballPlayer {
public:
    /*
    * Конструктор
    *
    * @param number номер игрока
    * Инициализирую класс родитель с необходимыми параметрами
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
    * Подбор
    *
    * Уменьшаю энергию игрока и вывожу шанс подбора
    */
    int doRebound() {
        energy -= 1 + rand() % endurance;
        return rebound;
    }

    /*
    * Вывод характиристик игрока
    *
    * Прохожусь по всем параметрам игрока и вывожу их
    */
    void printPlayer() {
        std::cout << "------------------" << std::endl
            << "Игровой номер - " << number << std::endl
            << "Позиция - Центровой" << std::endl
            << "Двушки - " << double_pt << std::endl
            << "Трешки - " << triple_pt << std::endl
            << "Подборы - " << rebound << std::endl
            << "Передачи - " << pass << std::endl
            << "Перехваты - " << steal << std::endl
            << "Блокшоты - " << block << std::endl
            << "Владение мячом - " << have_ball << std::endl
            << "Выносливость - " << endurance << std::endl;
    }

    /*
    * Вывод статистики игрока
    *
    * Прохожусь по всем параметрам игрока и вывожу их
    */
    void printStats() {
        std::cout << "#" << std::setw(3) << number << std::setw(10) << "(Center)"
            << ": время на поле - " << std::setw(2) << time_on_field
            << "| очки - " << std::setw(2) << point_done
            << "| пасы - " << std::setw(2) << pass_done
            << "| подборы - " << std::setw(2) << rebound_done
            << "| перехваты - " << std::setw(2) << steels_done << std::endl;
    }
};

