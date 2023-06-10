function play_sound(audio_file){
    console.log(audio_file);
    audio_file.play();
}

drums = document.getElementsByClassName("drum");

audio_files = [
    new Audio("./sounds/crash.mp3"    ),
    new Audio("./sounds/kick-bass.mp3"),
    new Audio("./sounds/snare.mp3"    ),
    new Audio("./sounds/tom-1.mp3"    ),
    new Audio("./sounds/tom-2.mp3"    ),
    new Audio("./sounds/tom-3.mp3"    ),
    new Audio("./sounds/tom-4.mp3"    )
]


for( let i=0; i<drums.length; i++){
    const btn = drums[i];
    const c = btn.classList[0];
    mp3_file = undefined
    switch(c){
        case "w" : btn.onclick=function() { play_sound(audio_files[0]);}; break;
        case "a" : btn.onclick=function() { play_sound(audio_files[1]);}; break;
        case "s" : btn.onclick=function() { play_sound(audio_files[2]);}; break;
        case "d" : btn.onclick=function() { play_sound(audio_files[3]);}; break;
        case "j" : btn.onclick=function() { play_sound(audio_files[4]);}; break;
        case "k" : btn.onclick=function() { play_sound(audio_files[5]);}; break;
        case "l" : btn.onclick=function() { play_sound(audio_files[6]);}; break;
    }
    console.log(btn, c, mp3_file);
    
    //btn.onclick=function() { play_sound(mp3_file); }
}
