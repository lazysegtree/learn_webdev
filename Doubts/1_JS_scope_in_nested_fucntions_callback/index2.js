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
        case "w" : mp3_file = audio_files[0]; break;
        case "a" : mp3_file = audio_files[1]; break;
        case "s" : mp3_file = audio_files[2]; break;
        case "d" : mp3_file = audio_files[3]; break;
        case "j" : mp3_file = audio_files[4]; break;
        case "k" : mp3_file = audio_files[5]; break;
        case "l" : mp3_file = audio_files[6]; break;
    }
    console.log(btn, c, mp3_file);
    
    btn.onclick=function() { play_sound(mp3_file); }
}
