
#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
    score1_ = 0;
    score2_ = 0;
    strike_ = false;
    spare_ = false;
    roll_one_ = false;
    roll_two_ = false;

    total_score_ = 0;
    frame_num_ = fnum;
    frame_score_ = 0;

    bonus1_ = 0;
    bonus2_ = 0;
    bonus_rolls_ = 0;

    next_frame_ = nullptr;
    prev_frame_ = nullptr;
}

Bowling_Frame::~Bowling_Frame()
{
    // Frames are deleted individually in main.
}

Bowling_Frame* Bowling_Frame::next_frame()
{
    return next_frame_;
}

void Bowling_Frame::next_frame(Bowling_Frame* new_frame)
{
    next_frame_ = new_frame;
}

Bowling_Frame* Bowling_Frame::prev_frame()
{
    return prev_frame_;
}

void Bowling_Frame::prev_frame(Bowling_Frame* new_frame)
{
    prev_frame_ = new_frame;
}

int Bowling_Frame::score1()
{
    return score1_;
}

void Bowling_Frame::score1(int score)
{
    score1_ = score;
}

int Bowling_Frame::score2()
{
    return score2_;
}

void Bowling_Frame::score2(int score)
{
    score2_ = score;
}

bool Bowling_Frame::strike()
{
    return strike_;
}

void Bowling_Frame::strike(bool value)
{
    strike_ = value;
}

bool Bowling_Frame::spare()
{
    return spare_;
}

void Bowling_Frame::spare(bool value)
{
    spare_ = value;
}

int Bowling_Frame::frame_num()
{
    return frame_num_;
}

int Bowling_Frame::frame_score()
{
    int total = score1_ + score2_;

    if (frame_num_ == 10)
    {
        return total + bonus1_ + bonus2_;
    }

    if (!strike_ && !spare_)
    {
        return total;
    }

    int rolls[2] = {0, 0};
    int count = 0;

    Bowling_Frame* temp = next_frame_;

    while (temp != nullptr && count < 2)
    {
        rolls[count++] = temp->score1_;

        if (count < 2 && !temp->strike_)
        {
            rolls[count++] = temp->score2_;
        }

        if (temp->frame_num_ == 10 && count < 2)
        {
            if (temp->strike_ || temp->spare_)
            {
                rolls[count++] = temp->bonus1_;

                if (count < 2 && temp->strike_)
                {
                    rolls[count++] = temp->bonus2_;
                }
            }
        }

        temp = temp->next_frame_;
    }

    if (strike_)
    {
        total += rolls[0] + rolls[1];
    }
    else if (spare_)
    {
        total += rolls[0];
    }

    return total;
}

void Bowling_Frame::update_scores()
{
    std::uniform_int_distribution<int> dist10(0, 10);

    if (!roll_one_)
    {
        score1_ = dist10(rng);
        roll_one_ = true;

        if (score1_ == 10)
        {
            strike_ = true;
            roll_two_ = true;
        }
    }
    else if (!roll_two_)
    {
        std::uniform_int_distribution<int> distX(
            0, 10 - score1_
        );

        score2_ = distX(rng);
        roll_two_ = true;

        if (score1_ + score2_ == 10)
        {
            spare_ = true;
        }
    }
    else if (frame_num_ == 10 &&
             (strike_ || spare_))
    {
        if (bonus_rolls_ == 0)
        {
            bonus1_ = dist10(rng);
            bonus_rolls_++;
        }
        else if (bonus_rolls_ == 1 && strike_)
        {
            int maxPins = 10;

            if (bonus1_ != 10)
            {
                maxPins = 10 - bonus1_;
            }

            std::uniform_int_distribution<int> distBonus(
                0, maxPins
            );

            bonus2_ = distBonus(rng);
            bonus_rolls_++;
        }
    }

    frame_score_ = frame_score();
    total_score_ = frame_score_;
}
