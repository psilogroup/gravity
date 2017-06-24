//
// Created by gabriel on 24/06/17.
//

#ifndef GRAVITY_TIMER_H
#define GRAVITY_TIMER_H


class Timer {
private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();

    void stop();

    void pause();

    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();

    bool is_paused();
};

#endif //GRAVITY_TIMER_H
