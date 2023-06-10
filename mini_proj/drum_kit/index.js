
audio_files = {
    "w" : new Audio("./sounds/crash.mp3"    ),
    "a" : new Audio("./sounds/kick-bass.mp3"),
    "s" : new Audio("./sounds/snare.mp3"    ),
    "d" : new Audio("./sounds/tom-1.mp3"    ),
    "j" : new Audio("./sounds/tom-2.mp3"    ),
    "k" : new Audio("./sounds/tom-3.mp3"    ),
    "l" : new Audio("./sounds/tom-4.mp3"    )
}

function play_sound(audio_file){
    for(let i in audio_files){
        console.log(i, audio_files[i], audio_files[i].paused);
        audio_files[i].pause();
        audio_files[i].currentTime = 0;
        console.log(i, audio_files[i], audio_files[i].paused);
    }
    console.log("All paused.");
    console.log(audio_file);
    audio_file.play();
}

drums = document.getElementsByClassName("drum");




for( let i=0; i<drums.length; i++){
    const btn = drums[i];
    const c = btn.classList[0];
    mp3_file = undefined
    switch(c){
        case "w" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
        case "a" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
        case "s" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
        case "d" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
        case "j" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
        case "k" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
        case "l" : btn.onclick=function() { play_sound(audio_files[c]);}; break;
    }
    console.log(btn, c, mp3_file);
    
    //btn.onclick=function() { play_sound(mp3_file); }
}

// works on  firefox

document.addEventListener("keypress", function (e){
    console.log(e, e.key + " pressed.");
    if(e.key in audio_files){
        file = audio_files[e.key];
        console.log(file);
        play_sound(file);
    }
});