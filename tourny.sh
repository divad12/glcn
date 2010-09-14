#!/bin/bash

date >> results.txt

./playall.sh ../bots/8/MyBot ../bots/7/MyBot ../bots/7-frugally-take-future-ops/MyBot ../bots/7-only-frugal-future-neutrals/MyBot ../bots/7-spread-second-turn/MyBot ../bots/7-unlimited-fleets/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

echo >> results.txt
