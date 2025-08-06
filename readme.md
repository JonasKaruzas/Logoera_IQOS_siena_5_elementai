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
group 1 - led 0 to 41,
group 2 - 42 to 51,
group 3 - 52 to 61,
group 4 - 62 to 71,
group 5 - 72 to 81,
group 6 - 82 to 91

animation modes:
1 - All groups are on. Group 1 always stays on, but from group 2 to group 6, one by one fades off, and fades in, one at a time. example - group 2 fades off, group 2 fades on, group 3 fades off, group 3 fades on, group 4 fades off, group 4 fades on ...

2 - All groups are on. Group 1 always stays on, but from group 2 to group 6, one by one crossfades. example - group 2 fades off, group 2 fades in and in the same time group 3 fades off, group 3 fades in and in the same time as group 4 fades out ...

Mode selector needs to save to memory, so it's the same even if the device gets reset
