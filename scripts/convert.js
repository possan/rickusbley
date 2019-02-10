const fs = require('fs');
const midifile = JSON.parse(fs.readFileSync('../data/loop.json', 'UTF-8'));
const track = midifile.tracks[0];

// console.log('// NOTE( midinote, freq, fixedpointperiod, start step, duration),')
// console.log('// WAIT( steps ),')
// console.log('');

const TIMESCALE = 1024;

let time = 0;

track.notes.forEach(n => {

    const delay = n.time - time;
    const delayfixed = Math.round(TIMESCALE * delay);

    const freq = Math.pow(2, (n.midi - 49) / 12) * 440;
    const freqfixed = Math.round(freq * 1);

    const durfixed = Math.round(TIMESCALE * n.duration / 16);

    // #define LIBRICK_DELAY(duration) { 1, duration, 0 },
    // #define LIBRICK_NOTE(freq, dur) { 2, freq, dur / 10 },
    // #define LIBRICK_LOOP() { 3, 0, 0 },

    if (delayfixed > 0) {
        // console.log('// at ' + time + ' >> ' + JSON.stringify(n));
        console.log('{ 1, ' + delayfixed + ', 0 },');
    }

    console.log('{ 2, ' + freqfixed + ', ' + durfixed + ' },');
    // console.log('LIBRICK_DELAY(' + durfixed + ')');
    // console.log('');

    time = n.time; //  + n.duration;

    // console.log();
});

const lastdelay2 = (16 - time);
const lastdelayfixed2 = Math.round(TIMESCALE * lastdelay2);

console.log('{ 1, ' + lastdelayfixed2 + ', 0 },');
console.log('{ 3, 0, 0 },');
