const start = document.getElementsByClassName('gameView')[0];
const input = document.getElementsByClassName('txtInput')[0];
const times = document.getElementsByClassName('time');
const time = document.getElementsByClassName('time')[times.length-1].innerHTML;
const WordBank = start.childNodes[0].childNodes[1].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes[0].childNodes;
let Words = "";
for (let i = 0; i<WordBank.length; i++){
    Words+=WordBank[i].innerHTML;
}
let left = parseInt(time.slice(1));
left+=1;
const b = setTimeout(begin,left*1000);
function begin(){
    const a = setInterval(type,27);
    let k = 0;
    function type(){
        if(k>=Words.length-1){
            stopInterval();
        }
        input.value+=Words[k];
        input.dispatchEvent(new KeyboardEvent('keydown', {'key': Words[k]}));
        input.dispatchEvent(new KeyboardEvent('keyup', {'key': Words[k]}));
        k+=1;
    }
    function stopInterval(){
        clearInterval(a);
    }
}
//Go into a game, and wait until the timer starts
//Press F12 and insert the script
//Done
//WARNING: IT WILL NOT WORK IF YOU GO INTO A GAME WHILE IN F12(It won't write all of it) OR START THE SCRIPT BEFORE THE TIMER (OR ELSE IT WILL START PREMATURELY)