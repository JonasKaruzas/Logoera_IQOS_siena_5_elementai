query

write a arduino program, we will be using a ws2812b led strip, a button, and 5 potentiometers.
button connected to pin - d13,
pot's are - a0, a1, a2, a3, a6,
leds are - d7.

a0 - controls the red,
a1 - green,
a2 - blue,
a3 - brightness,
a6 - animation speed.

button controlls animation mode.

leds are divided into 6 groups:
group 1 - led 1 to 42,
group 2 - 43 to 53,
group 3 - 54 to 64,
group 4 - 65 to 75,
group 5 - 76 to 86,
group 6 - 87 to 97

animation modes:
1 - group 1 is always on, but other groups one by one turn on (only one at a time),
2 - group 1 is always on, but other groups one by one turn on (only one at a time) but smoothly with fadein fade out.
3 - same as 1 but including group 1,
4 - same as 2 but including group 2.

Mode selector needs to save to memory, so it's the same even if the device gets reset
