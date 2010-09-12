#!/bin/bash

date >> results.txt

./playall.sh ../bots/5/MyBot ../bots/5-issueOrder-simple/MyBot ../bots/5-min-planet-ships-5/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/5-issueOrder-simple/MyBot ../bots/5-min-planet-ships-5/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/5-min-planet-ships-5/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/5-skip-first-turn/MyBot ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/5-skip-first-turn/MyBot ../bots/5-unlimited-fleets/MyBot ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/5-unlimited-fleets/MyBot ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

./playall.sh ../bots/5-unlimited-fleets-of-10/MyBot  ../bots/4-no-fleets-limit/MyBot ../bots/3/MyBot example_bots/DualBot.jar example_bots/RageBot.jar >> results.txt

echo >> results.txt
